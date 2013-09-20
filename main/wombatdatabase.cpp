#include "wombatdatabase.h"

std::string WombatDatabase::GetTime()
{
    struct tm *newtime;
    time_t aclock;

    time(&aclock);   // Get time in seconds
    newtime = localtime(&aclock);   // Convert time to struct tm form 
    char timeStr[64];
    snprintf(timeStr, 64, "%.2d/%.2d/%.2d %.2d:%.2d:%.2d",
        newtime->tm_mon+1,newtime->tm_mday,newtime->tm_year % 100, 
        newtime->tm_hour, newtime->tm_min, newtime->tm_sec);

    return timeStr;
}

WombatDatabase::WombatDatabase()
{
}

bool WombatDatabase::FileExists(const std::string& filename)
{
    struct stat buf;
    if (stat(filename.c_str(), &buf) != -1)
    {
        return true;
    }
    return false;
}

const char* WombatDatabase::CreateCaseDB(QString wombatdbname)
{
    std::vector<const char *> wombatTableSchema;
    wombatTableSchema.clear();
    wombatTableSchema.push_back("CREATE TABLE cases(caseid INTEGER PRIMARY KEY, name TEXT, creation TEXT, deleted INTEGER);");
    wombatTableSchema.push_back("CREATE TABLE job(jobid INTEGER PRIMARY KEY, type INTEGER, state INTEGER, filecount INTEGER, processcount INTEGER, caseid INTEGER, evidenceid INTEGER, start TEXT, end TEXT);");
    wombatTableSchema.push_back("CREATE TABLE evidence(evidenceid INTEGER PRIMARY KEY, fullpath TEXT, name TEXT, caseid INTEGER, creation TEXT, deleted INTEGER);");
    wombatTableSchema.push_back("CREATE TABLE settings(settingid INTEGER PRIMARY KEY, name TEXT, value TEXT, type INT);");
    wombatTableSchema.push_back("CREATE TABLE msglog(logid INTEGER PRIMARY KEY, caseid INTEGER, evidenceid INTEGER, jobid INTEGER, msgtype INTEGER, msg TEXT, datetime TEXT);");
    wombatTableSchema.push_back("CREATE TABLE objects(objectid INTEGER PRIMARY KEY, caseid INTEGER, evidenceid INTEGER, fileid INTEGER);");
    if(sqlite3_open(wombatdbname.toStdString().c_str(), &wombatdb) == SQLITE_OK)
    {
        const char* tblString;
        foreach(tblString, wombatTableSchema)
        {
            sqlite3_stmt* tmpstmt;
            //if(sqlite3_prepare_v2(wombatdb, tblString, -1, &sqlstatement, NULL) == SQLITE_OK)
            if(sqlite3_prepare_v2(wombatdb, tblString, -1, &tmpstmt, NULL) == SQLITE_OK)
            {
                int ret = sqlite3_step(tmpstmt);
                if(ret != SQLITE_ROW && ret != SQLITE_DONE)
                {
                    return sqlite3_errmsg(wombatdb);
                }
            }
            else
            {
                return sqlite3_errmsg(wombatdb);
            }
            sqlite3_finalize(tmpstmt);
        }
        //sqlite3_close(wombatdb);
    }
    else
    {
        return sqlite3_errmsg(wombatdb);
    }

    return "";
}

const char* WombatDatabase::CreateLogDB(QString dbname)
{
    if(sqlite3_open(dbname.toStdString().c_str(), &logdb) == SQLITE_OK)
    {
        if(sqlite3_prepare_v2(logdb, "CREATE TABLE log(logid INTEGER PRIMARY KEY, caseid INTEGER, evidenceid INTEGER, jobid INTEGER, msgtype INTEGER, msgdatetime TEXT, msg TEXT);", -1, &logstatement, NULL) == SQLITE_OK)
        {
            int ret = sqlite3_step(logstatement);
            if(ret != SQLITE_ROW && ret != SQLITE_DONE)
            {
                return sqlite3_errmsg(logdb);
            }
        }
    }
    sqlite3_finalize(logstatement);
    sqlite3_close(logdb);
    return "";
}

const char* WombatDatabase::OpenCaseDB(QString dbname)
{
    if(sqlite3_open(dbname.toStdString().c_str(), &wombatdb) != SQLITE_OK)
        return sqlite3_errmsg(wombatdb);
    else
        return "";
}

const char* WombatDatabase::CloseCaseDB()
{
    if(sqlite3_finalize(sqlstatement) == SQLITE_OK)
    {
        if(sqlite3_close(wombatdb) == SQLITE_OK)
        {
            fprintf(stderr, "CloseDB was successful\n");
            return "";
        }
        else
        {
            fprintf(stderr, "CloseDB: %s\n", sqlite3_errmsg(wombatdb));
            return sqlite3_errmsg(wombatdb);
        }
    }
    else
    {
        fprintf(stderr, "CloseDB: %s\n", sqlite3_errmsg(wombatdb));
        return sqlite3_errmsg(wombatdb);
    }
}

WombatDatabase::~WombatDatabase()
{
    CloseCaseDB();
}

int WombatDatabase::ReturnCaseCount()
{
    int casecount = 0;
    if(sqlite3_prepare_v2(wombatdb, "SELECT COUNT(caseid) FROM cases WHERE deleted = 0;", -1, &sqlstatement, NULL) == SQLITE_OK)
    {
        int ret = sqlite3_step(sqlstatement);
        if(ret == SQLITE_ROW || ret == SQLITE_DONE)
            casecount = sqlite3_column_int(sqlstatement, 0);
    }
    else
        emit DisplayError("1.3", "SQL Error. ", sqlite3_errmsg(wombatdb));

    return casecount;
}

int WombatDatabase::InsertCase(QString caseText)
{
    int caseid = 0;
    if(sqlite3_prepare_v2(wombatdb, "INSERT INTO cases (name, creation, deleted) VALUES(?, ?, 0);", -1, &sqlstatement, NULL) == SQLITE_OK)
    {
        if(sqlite3_bind_text(sqlstatement, 1, caseText.toStdString().c_str(), -1, SQLITE_TRANSIENT) == SQLITE_OK && sqlite3_bind_text(sqlstatement, 2, GetTime().c_str(), -1, SQLITE_TRANSIENT) == SQLITE_OK)
        {
            int ret = sqlite3_step(sqlstatement);
            if(ret == SQLITE_ROW || ret == SQLITE_DONE)
            {
                caseid = sqlite3_last_insert_rowid(wombatdb);
            }
            else
                emit DisplayError("1.4", "INSERT CASE - RETURN CASEID", sqlite3_errmsg(wombatdb));
        }
        else
            emit DisplayError("1.4", "INSERT CASE - BIND CASENAME", sqlite3_errmsg(wombatdb));
    }
    else
        emit DisplayError("1.4", "INSERT CASE - PREPARE INSERT", sqlite3_errmsg(wombatdb));

    return caseid;
}

QStringList WombatDatabase::ReturnCaseNameList()
{
    QStringList tmpList;
    if(sqlite3_prepare_v2(wombatdb, "SELECT name FROM cases WHERE deleted = 0 ORDER by caseid;", -1, &sqlstatement, NULL) == SQLITE_OK)
    {
        while(sqlite3_step(sqlstatement) == SQLITE_ROW)
        {
            tmpList << (const char*)sqlite3_column_text(sqlstatement, 0);
        }
    }
    else
    {
        emit DisplayError("1.5", "RETURN CASE NAMES LIST", sqlite3_errmsg(wombatdb));
    }

    return tmpList;
}

int WombatDatabase::ReturnCaseID(QString caseName)
{
    int caseid = 0;
    if(sqlite3_prepare_v2(wombatdb, "SELECT caseid FROM cases WHERE name = ?;", -1, &sqlstatement, NULL) == SQLITE_OK)
    {
        if(sqlite3_bind_text(sqlstatement, 1, caseName.toStdString().c_str(), -1, SQLITE_TRANSIENT) == SQLITE_OK)
        {
            int ret = sqlite3_step(sqlstatement);
            if(ret == SQLITE_ROW || ret == SQLITE_DONE)
            {
                caseid = sqlite3_column_int(sqlstatement, 0);
            }
            else
                emit DisplayError("1.6", "RETURN CURRENT CASE ID", sqlite3_errmsg(wombatdb));
        }
        else
            emit DisplayError("1.6", "RETURN CURRENT CASE ID", sqlite3_errmsg(wombatdb));
    }
    else
        emit DisplayError("1.6", "RETURN CURRENT CASE ID", sqlite3_errmsg(wombatdb));
    
    return caseid;
}

int WombatDatabase::ReturnObjectFileID(int objectid)
{
    int fileid = 0;
    if(sqlite3_prepare_v2(wombatdb, "SELECT fileid FROM objects WHERE objectid = ?;", -1, &sqlstatement, NULL) == SQLITE_OK)
    {
        if(sqlite3_bind_int(sqlstatement, 1, objectid) == SQLITE_OK)
        {
            int ret = sqlite3_step(sqlstatement);
            if(ret == SQLITE_ROW || ret == SQLITE_DONE)
            {
                fileid = sqlite3_column_int(sqlstatement, 0);
            }
            else
                emit DisplayError("1.18", "RETURN OBJECT'S FILEID ", sqlite3_errmsg(wombatdb));
        }
        else
            emit DisplayError("1.18", "RETURN OBJECT'S FILEID ", sqlite3_errmsg(wombatdb));
    }
    else
        emit DisplayError("1.18", "RETURN OBJECT'S FILEID ", sqlite3_errmsg(wombatdb));

    return fileid;
}

int WombatDatabase::ReturnObjectEvidenceID(int objectid)
{
    int evidenceid = 0;
    if(sqlite3_prepare_v2(wombatdb, "SELECT evidenceid FROM objects WHERE objectid = ?;", -1, &sqlstatement, NULL) == SQLITE_OK)
    {
        if(sqlite3_bind_int(sqlstatement, 1, objectid) == SQLITE_OK)
        {
            int ret = sqlite3_step(sqlstatement);
            if(ret == SQLITE_ROW || ret == SQLITE_DONE)
                evidenceid = sqlite3_column_int(sqlstatement, 0);
            else
                emit DisplayError("1.20", "RETURN OBJECT EVIDENCEID ", sqlite3_errmsg(wombatdb));
        }
        else
            emit DisplayError("1.20", "RETURN OBJECT EVIDENCEID ", sqlite3_errmsg(wombatdb));
    }
    else
        emit DisplayError("1.20", "RETURN OBJECT EVIDENCEID ", sqlite3_errmsg(wombatdb));

    return evidenceid;
}

int WombatDatabase::ReturnObjectID(int caseid, int evidenceid, int fileid)
{
    int objectid = 0;
    if(sqlite3_prepare_v2(wombatdb, "SELECT objectid FROM objects WHERE caseid = ? AND evidenceid = ? AND fileid = ?;", -1, &sqlstatement, NULL) == SQLITE_OK)
    {
        if(sqlite3_bind_int(sqlstatement, 1, caseid) == SQLITE_OK && sqlite3_bind_int(sqlstatement, 2, evidenceid) == SQLITE_OK && sqlite3_bind_int(sqlstatement, 3, fileid) == SQLITE_OK)
        {
            int ret = sqlite3_step(sqlstatement);
            if(ret == SQLITE_ROW || ret == SQLITE_DONE)
                objectid = sqlite3_column_int(sqlstatement, 0);
            else
                emit DisplayError("1.19", "RETURN OBJECT ID ", sqlite3_errmsg(wombatdb));
        }
        else
            emit DisplayError("1.19", "RETURN OBJECT ID ", sqlite3_errmsg(wombatdb));
    }
    else
        emit DisplayError("1.19", "RETURN OBJECT ID ", sqlite3_errmsg(wombatdb));

    return objectid;
}

int WombatDatabase::InsertJob(int jobType, int caseID, int evidenceID)
{
    int jobid = 0;

    if(sqlite3_prepare_v2(wombatdb, "INSERT INTO job (type, caseid, evidenceid, start) VALUES(?, ?, ?, ?);", -1, &sqlstatement, NULL) == SQLITE_OK)
    {
        if(sqlite3_bind_int(sqlstatement, 1, jobType) == SQLITE_OK && sqlite3_bind_int(sqlstatement, 2, caseID) == SQLITE_OK && sqlite3_bind_int(sqlstatement, 3, evidenceID) == SQLITE_OK && sqlite3_bind_text(sqlstatement, 4, GetTime().c_str(), -1, SQLITE_TRANSIENT) == SQLITE_OK)
        {
            int ret = sqlite3_step(sqlstatement);
            if(ret == SQLITE_ROW || ret == SQLITE_DONE)
            {
                jobid = sqlite3_last_insert_rowid(wombatdb);
            }
            else
                emit DisplayError("1.4", "INSERT JOB ", sqlite3_errmsg(wombatdb));
        }
        else
            emit DisplayError("1.4", "INSERT JOB ", sqlite3_errmsg(wombatdb));
    }
    else
        emit DisplayError("1.4", "INSERT JOB ", sqlite3_errmsg(wombatdb));
    
    return jobid;
}

int WombatDatabase::InsertEvidence(QString evidenceName, QString evidenceFilePath, int caseID)
{
    int evidenceid = 0;
    if(sqlite3_prepare_v2(wombatdb, "INSERT INTO evidence (fullpath, name, caseid, creation, deleted) VALUES(?, ?, ?, ?, 0);", -1, &sqlstatement, NULL) == SQLITE_OK)
    {
        if(sqlite3_bind_text(sqlstatement, 1, evidenceFilePath.toStdString().c_str(), -1, SQLITE_TRANSIENT) == SQLITE_OK && sqlite3_bind_text(sqlstatement, 2, evidenceName.toStdString().c_str(), -1, SQLITE_TRANSIENT) == SQLITE_OK && sqlite3_bind_int(sqlstatement, 3, caseID) == SQLITE_OK && sqlite3_bind_text(sqlstatement, 4, GetTime().c_str(), -1, SQLITE_TRANSIENT) == SQLITE_OK)
        {
            int ret = sqlite3_step(sqlstatement);
            if(ret == SQLITE_ROW || ret == SQLITE_DONE)
            {
                // it was successful
                evidenceid = sqlite3_last_insert_rowid(wombatdb);
            }
            else
                emit DisplayError("1.7", "INSERT EVIDENCE INTO CASE", sqlite3_errmsg(wombatdb));
        }
        else
            emit DisplayError("1.7", "INSERT EVIDENCE INTO CASE", sqlite3_errmsg(wombatdb));
    }
    else
        emit DisplayError("1.7", "INSERT EVIDENCE INTO CASE", sqlite3_errmsg(wombatdb));

    return evidenceid;
}

int WombatDatabase::InsertObject(int caseid, int evidenceid, int fileid)
{
    int objectid = 0;
    if(sqlite3_prepare_v2(wombatdb, "INSERT INTO objects (caseid, evidenceid, fileid) VALUES(?, ?, ?);", -1, &sqlstatement, NULL) == SQLITE_OK)
    {
        if(sqlite3_bind_int(sqlstatement, 1, caseid) == SQLITE_OK && sqlite3_bind_int(sqlstatement, 2, evidenceid) == SQLITE_OK && sqlite3_bind_int(sqlstatement, 3, fileid) == SQLITE_OK)
        {
            int ret = sqlite3_step(sqlstatement);
            if(ret == SQLITE_ROW || ret == SQLITE_DONE)
            {
                // it was successful
                objectid = sqlite3_last_insert_rowid(wombatdb);
            }
            else
               DisplayError("1.17", "INSERT OBJECT INTO CASE ", sqlite3_errmsg(wombatdb));
        }
        else
            DisplayError("1.17", "INSERT OBJECT INTO CASE ", sqlite3_errmsg(wombatdb));
    }
    else
        DisplayError("1.17", "INSERT OBJECT INTO CASE ", sqlite3_errmsg(wombatdb));

    return objectid;
}

QStringList WombatDatabase::ReturnCaseEvidence(int caseID)
{
    QStringList tmpList;
    if(sqlite3_prepare_v2(wombatdb, "SELECT fullpath FROM evidence WHERE caseid = ? AND deleted = 0 ORDER BY evidenceid;", -1, &sqlstatement, NULL) == SQLITE_OK)
    {
        if(sqlite3_bind_int(sqlstatement, 1, caseID) == SQLITE_OK)
        {
            while(sqlite3_step(sqlstatement) == SQLITE_ROW)
            {
                tmpList << (const char*)sqlite3_column_text(sqlstatement, 0);
            }
        }
        else
        {
            emit DisplayError("1.8", "RETURN CASE EVIDENCE", sqlite3_errmsg(wombatdb));
        }
    }
    else
        emit DisplayError("1.8", "RETURN CASE EVIDENCE", sqlite3_errmsg(wombatdb));

    return tmpList;
}

QStringList WombatDatabase::ReturnCaseEvidenceID(int caseID)
{
    QStringList tmpList;
    if(sqlite3_prepare_v2(wombatdb, "SELECT evidenceid FROM evidence WHERE caseid = ? AND deleted = 0;", -1, &sqlstatement, NULL) == SQLITE_OK)
    {
        if(sqlite3_bind_int(sqlstatement, 1, caseID) == SQLITE_OK)
        {
            while(sqlite3_step(sqlstatement) == SQLITE_ROW)
            {
                tmpList << QString::number(sqlite3_column_int(sqlstatement, 0));
            }
        }
        else
            emit DisplayError("1.2", "RETURN CASE EVIDENCE ID ", sqlite3_errmsg(wombatdb));
    }
    else
        emit DisplayError("1.2", "RETURN CASE EVIDENCE ID ", sqlite3_errmsg(wombatdb));

    return tmpList;
}

QStringList WombatDatabase::ReturnCaseEvidenceAddJobID(int caseid, QStringList evidenceidlist)
{
    QStringList tmplist;
    foreach(QString evid, evidenceidlist)
    {
        if(sqlite3_prepare_v2(wombatdb, "SELECT jobid FROM job WHERE caseid = ? and evidenceid = ? and type = 1;", -1, &sqlstatement, NULL) == SQLITE_OK)
        {
            if(sqlite3_bind_int(sqlstatement, 1, caseid) == SQLITE_OK && sqlite3_bind_int(sqlstatement, 2, evid.toInt()) == SQLITE_OK)
            {
                while(sqlite3_step(sqlstatement) == SQLITE_ROW)
                {
                    tmplist << QString::number(sqlite3_column_int(sqlstatement, 0));
                }
            }
            else
                emit DisplayError("1.15", "RETURN CASE EVIDENCE ADD JOB ID ", sqlite3_errmsg(wombatdb));

        }
        else
            emit DisplayError("1.15", "RETURN CASE EVIDENCE ADD JOB ID ", sqlite3_errmsg(wombatdb));
    }

    return tmplist;
}

QStringList WombatDatabase::ReturnEvidenceData(int evidenceid)
{
    QStringList tmplist;
    if(sqlite3_prepare_v2(wombatdb, "SELECT fullpath, name FROM evidence WHERE evidenceid = ?;", -1, &sqlstatement, NULL) == SQLITE_OK)
    {
        if(sqlite3_bind_int(sqlstatement, 1, evidenceid) == SQLITE_OK)
        {
            int ret = sqlite3_step(sqlstatement);
            if(ret == SQLITE_ROW || ret == SQLITE_DONE)
            {
                tmplist << QString((const char*)sqlite3_column_text(sqlstatement, 0));
                tmplist << QString((const char*)sqlite3_column_text(sqlstatement, 1));
            }
            else
                emit DisplayError("1.21", "RETURN EVIDENCE DATA ", sqlite3_errmsg(wombatdb));
        }
        else
            emit DisplayError("1.21", "RETURN EVIDENCE DATA ", sqlite3_errmsg(wombatdb));
    }
    else
        emit DisplayError("1.21", "RETURN EVIDENCE DATA ", sqlite3_errmsg(wombatdb));

    return tmplist;
}
QStringList WombatDatabase::ReturnMessageTableEntries(int caseid, int evidenceid, int jobid)
{
    QStringList tmpstringlist;
    QString tmptype;
    if(sqlite3_prepare_v2(wombatdb, "SELECT msgtype, msg FROM msglog WHERE caseid = ? AND evidenceid = ? and jobid = ?;", -1, &sqlstatement, NULL) == SQLITE_OK)
    {
        if(sqlite3_bind_int(sqlstatement, 1, caseid) == SQLITE_OK && sqlite3_bind_int(sqlstatement, 2, evidenceid) == SQLITE_OK && sqlite3_bind_int(sqlstatement, 3, jobid) == SQLITE_OK)
        {
            while(sqlite3_step(sqlstatement) == SQLITE_ROW)
            {
                if(sqlite3_column_int(sqlstatement, 0) == 0)
                    tmptype = "[ERROR]";
                else if(sqlite3_column_int(sqlstatement, 0) == 1)
                    tmptype = "[WARN]";
                else
                    tmptype = "[INFO]";
                tmpstringlist << tmptype << QString((const char*)sqlite3_column_text(sqlstatement, 1));
            }
        }
        else
            emit DisplayError("1.9", "RETURN MSGTABLE ENTIRES ", sqlite3_errmsg(wombatdb));
    }
    else
        emit DisplayError("1.9", "RETURN MSGTABLE ENTRIES ", sqlite3_errmsg(wombatdb));

    return tmpstringlist;
}
void WombatDatabase::InsertMsg(int caseid, int evidenceid, int jobid, int msgtype, const char* msg)
{
    if(sqlite3_prepare_v2(wombatdb, "INSERT INTO msglog (caseid, evidenceid, jobid, msgtype, msg, datetime) VALUES(?, ?, ?, ?, ?, ?);", -1, &sqlstatement, NULL) == SQLITE_OK)
    {
        if(sqlite3_bind_int(sqlstatement, 1, caseid) == SQLITE_OK && sqlite3_bind_int(sqlstatement, 2, evidenceid) == SQLITE_OK && sqlite3_bind_int(sqlstatement, 3, jobid) == SQLITE_OK && sqlite3_bind_int(sqlstatement, 4, msgtype) == SQLITE_OK && sqlite3_bind_text(sqlstatement, 5, msg, -1, SQLITE_TRANSIENT) == SQLITE_OK && sqlite3_bind_text(sqlstatement, 6, GetTime().c_str(), -1, SQLITE_TRANSIENT) == SQLITE_OK)
        {
            int ret = sqlite3_step(sqlstatement);
            if(ret == SQLITE_ROW || ret == SQLITE_DONE)
            {
                // do nothing, it was successful
            }
            else
                emit DisplayError("1.10", "INSERT MSG ", sqlite3_errmsg(wombatdb));
        }
        else
            emit DisplayError("1.10", "INSERT MSG ", sqlite3_errmsg(wombatdb));
    }
    else
        emit DisplayError("1.10", "INSERT MSG ", sqlite3_errmsg(wombatdb));
}

QStringList WombatDatabase::ReturnJobDetails(int jobid)
{
    QStringList tmplist;
    if(sqlite3_prepare_v2(wombatdb, "SELECT end, filecount, processcount, state FROM job WHERE jobid = ?;", -1, &sqlstatement, NULL) == SQLITE_OK)
    {
        if(sqlite3_bind_int(sqlstatement, 1, jobid) == SQLITE_OK)
        {
            int ret = sqlite3_step(sqlstatement);
            if(ret == SQLITE_ROW || ret == SQLITE_DONE)
            {
                tmplist << QString((const char*)sqlite3_column_text(sqlstatement, 0)) << QString::number(sqlite3_column_int(sqlstatement, 1)) << QString::number(sqlite3_column_int(sqlstatement, 2));
                if(sqlite3_column_int(sqlstatement, 3) == 1)
                    tmplist << "Adding Evidence Finished";
                else
                    tmplist << "Adding Evidence Finished with Errors";
            }
            else
                emit DisplayError("1.22", "RETURN JOB DETAILS ", sqlite3_errmsg(wombatdb));
        }
        else
            emit DisplayError("1.22", "RETURN JOB DETAILS ", sqlite3_errmsg(wombatdb));
    }
    else
        emit DisplayError("1.22", "RETURN JOB DETAILS ", sqlite3_errmsg(wombatdb));

    return tmplist;
}

void WombatDatabase::UpdateJobEnd(int jobid, int filecount, int processcount)
{
    if(sqlite3_prepare_v2(wombatdb, "UPDATE job SET end = ?, filecount = ?, processcount = ?, state = 1 WHERE jobid = ?;", -1, &sqlstatement, NULL) == SQLITE_OK)
    {
        if(sqlite3_bind_text(sqlstatement, 1, GetTime().c_str(), -1, SQLITE_TRANSIENT) == SQLITE_OK && sqlite3_bind_int(sqlstatement, 2, filecount) == SQLITE_OK && sqlite3_bind_int(sqlstatement, 3, processcount) == SQLITE_OK && sqlite3_bind_int(sqlstatement, 4, jobid) == SQLITE_OK)
        {
            int ret = sqlite3_step(sqlstatement);
            if(ret == SQLITE_ROW || ret == SQLITE_DONE)
            {
                // do nothing, it was successful
            }
            else
            {
                emit DisplayError("1.16", "UPDATE FINISHED JOB ", sqlite3_errmsg(wombatdb));
            }
        }
        else
            emit DisplayError("1.16", "UPDATE FINISHED JOB ", sqlite3_errmsg(wombatdb));
    }
    else
        emit DisplayError("1.16", "UPDATE FINISHED JOB ", sqlite3_errmsg(wombatdb));
}

int WombatDatabase::ReturnJobCaseID(int jobid)
{
    int caseid = 0;
    if(sqlite3_prepare_v2(wombatdb, "SELECT caseid FROM job WHERE jobid = ?;", -1, &sqlstatement, NULL) == SQLITE_OK)
    {
        if(sqlite3_bind_int(sqlstatement, 1, jobid) == SQLITE_OK)
        {
            int ret = sqlite3_step(sqlstatement);
            if(ret == SQLITE_ROW || ret == SQLITE_DONE)
            {
                caseid = sqlite3_column_int(sqlstatement, 0);
            }
            else
                emit DisplayError("1.23", "RETURN JOB CASE ID ", sqlite3_errmsg(wombatdb));
        }
        else
            emit DisplayError("1.23", "RETURN JOB CASE ID ", sqlite3_errmsg(wombatdb));
    }
    else
        emit DisplayError("1.23", "RETURN JOB CASE ID ", sqlite3_errmsg(wombatdb));

    return caseid;
}

int WombatDatabase::ReturnJobEvidenceID(int jobid)
{
    int evidenceid = 0;
    if(sqlite3_prepare_v2(wombatdb, "SELECT evidenceid FROM job WHERE jobid = ?;", -1, &sqlstatement, NULL) == SQLITE_OK)
    {
        if(sqlite3_bind_int(sqlstatement, 1, jobid) == SQLITE_OK)
        {
            int ret = sqlite3_step(sqlstatement);
            if(ret == SQLITE_ROW || ret == SQLITE_DONE)
            {
                evidenceid = sqlite3_column_int(sqlstatement, 0);
            }
            else
                emit DisplayError("1.24", "RETURN JOB EVIDENCE ID ", sqlite3_errmsg(wombatdb));
        }
        else
            emit DisplayError("1.24", "RETURN JOB EVIDENCE ID ", sqlite3_errmsg(wombatdb));
    }
    else
        emit DisplayError("1.24", "RETURN JOB EVIDENCE ID ", sqlite3_errmsg(wombatdb));

    return evidenceid;
}
void WombatDatabase::RemoveEvidence(QString evidencename)
{
    if(sqlite3_prepare_v2(wombatdb, "UPDATE evidence SET deleted = 1 WHERE fullpath = ?;", -1, &sqlstatement, NULL) == SQLITE_OK)
    {
        if(sqlite3_bind_text(sqlstatement, 1, evidencename.toStdString().c_str(), -1, SQLITE_TRANSIENT) == SQLITE_OK)
        {
            int ret = sqlite3_step(sqlstatement);
            if(ret == SQLITE_ROW || ret == SQLITE_DONE)
            {
                // do nothing, successful
            }
            else
                emit DisplayError("1.25", "REMOVE EVIDENCE ", sqlite3_errmsg(wombatdb));
        }
        else
            emit DisplayError("1.25", "REMOVE EVIDENCE ", sqlite3_errmsg(wombatdb));
    }
    else
        emit DisplayError("1.25", "REMOVE EVIDENCE ", sqlite3_errmsg(wombatdb));
}
