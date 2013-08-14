#include "wombatcasedb.h"

WombatCaseDb::WombatCaseDb(QWidget *Parent)
{
    wombatparent = Parent;
}
// Function: fileExists
/**
    Check if a file exists
@param[in] filename - the name of the file to check

@return    true if the file exists, else false

*/
std::string WombatCaseDb::GetTime()
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

bool WombatCaseDb::FileExists(const std::string& filename)
{
    struct stat buf;
    if (stat(filename.c_str(), &buf) != -1)
    {
        return true;
    }
    return false;
}

void WombatCaseDb::DisplayError(QWidget *parent, QString errorNumber, QString errorType, QString errorValue)
{
    QString tmpString = errorNumber;
    tmpString += ". SqlError: ";
    tmpString += errorType;
    tmpString += " Returned ";
    tmpString += errorValue;
    //errlog->log(SqlErrLog::Error, tmpString.toStdString().c_str());
    QMessageBox::warning(parent, "Error", tmpString, QMessageBox::Ok);
}

const char* WombatCaseDb::CreateCaseDB(QString wombatdbname)
{
    std::vector<const char *> wombatTableSchema;
    wombatTableSchema.clear();
    wombatTableSchema.push_back("CREATE TABLE log(logid INTEGER PRIMARY KEY, caseid INTEGER, imageid INTEGER, analysistype INTEGER, msgtype INTEGER, msgdatetime TEXT, logmsg TEXT);");
    wombatTableSchema.push_back("CREATE TABLE cases(caseid INTEGER PRIMARY KEY, casename TEXT, creation TEXT);");
    wombatTableSchema.push_back("CREATE TABLE caseimages(imageid INTEGER PRIMARY KEY, imagefullpath TEXT, imagename TEXT, caseid INTEGER, creation TEXT);");
    wombatTableSchema.push_back("CREATE TABLE settings(settingid INTEGER PRIMARY KEY, settingname TEXT, settingvalue TEXT, settingtype INT);");

    if(sqlite3_open(wombatdbname.toStdString().c_str(), &wombatdb) == SQLITE_OK)
    {
        const char* tblString;
        foreach(tblString, wombatTableSchema)
        {
            if(sqlite3_prepare_v2(wombatdb, tblString, -1, &sqlstatement, NULL) == SQLITE_OK)
            {
                int ret = sqlite3_step(sqlstatement);
                if(ret != SQLITE_ROW && ret != SQLITE_DONE)
                {
                    return sqlite3_errmsg(wombatdb);
                }
            }
            else
            {
                return sqlite3_errmsg(wombatdb);
            }
        }
    }
    else
    {
        return sqlite3_errmsg(wombatdb);
    }

    return "";
}

const char* WombatCaseDb::OpenCaseDB(QString dbname)
{
    if(sqlite3_open(dbname.toStdString().c_str(), &wombatdb) != SQLITE_OK)
        return sqlite3_errmsg(wombatdb);
    else
        return "";
}

const char* WombatCaseDb::CloseCaseDB()
{
    if(sqlite3_finalize(sqlstatement) == SQLITE_OK)
    {
        if(sqlite3_close(wombatdb) == SQLITE_OK)
            return "";
        else
            return sqlite3_errmsg(wombatdb);
    }
    else
        return sqlite3_errmsg(wombatdb);

}

WombatCaseDb::~WombatCaseDb()
{

}

int WombatCaseDb::ReturnCaseCount()
{
    int casecount = 0;
    if(sqlite3_prepare_v2(wombatdb, "SELECT COUNT(caseid) FROM cases;", -1, &sqlstatement, NULL) == SQLITE_OK)
    {
        int ret = sqlite3_step(sqlstatement);
        if(ret != SQLITE_ROW && ret != SQLITE_DONE)
            DisplayError(wombatparent, "1.3", "SQL Error. ", sqlite3_errmsg(wombatdb));
        else
            casecount = sqlite3_column_int(sqlstatement, 0);
    }
    else
        DisplayError(wombatparent, "1.3", "SQL Error. ", sqlite3_errmsg(wombatdb));

    return casecount;
}

int64_t WombatCaseDb::InsertCase(QString caseText)
{
    int64_t caseid = 0;
    if(sqlite3_prepare_v2(wombatdb, "INSERT INTO cases (casename, creation) VALUES(?, ?);", -1, &sqlstatement, NULL) == SQLITE_OK)
    {
        if(sqlite3_bind_text(sqlstatement, 1, caseText.toStdString().c_str(), -1, SQLITE_TRANSIENT) == SQLITE_OK)
        {
            if(sqlite3_bind_text(sqlstatement, 2, GetTime().c_str(), -1, SQLITE_TRANSIENT) == SQLITE_OK)
            {
                int ret = sqlite3_step(sqlstatement);
                if(ret == SQLITE_ROW || ret == SQLITE_DONE)
                {
                    caseid = sqlite3_last_insert_rowid(wombatdb);
                }
                else
                    DisplayError(wombatparent, "1.4", "INSERT CASE - RETURN CASEID", sqlite3_errmsg(wombatdb));
            }
            else
                DisplayError(wombatparent, "1.4", "INSERT CASE - BIND CREATION", sqlite3_errmsg(wombatdb));
        }
        else
            DisplayError(wombatparent, "1.4", "INSERT CASE - BIND CASENAME", sqlite3_errmsg(wombatdb));
    }
    else
        DisplayError(wombatparent, "1.4", "INSERT CASE - PREPARE INSERT", sqlite3_errmsg(wombatdb));

    return caseid;
}

QStringList WombatCaseDb::ReturnCaseNameList()
{
    QStringList tmpList;
    if(sqlite3_prepare_v2(wombatdb, "SELECT casename FROM cases ORDER by caseid;", -1, &sqlstatement, NULL) == SQLITE_OK)
    {
        while(sqlite3_step(sqlstatement) == SQLITE_ROW)
        {
            tmpList << (const char*)sqlite3_column_text(sqlstatement, 0);
        }
    }
    else
    {
        DisplayError(wombatparent, "1.5", "RETURN CASE NAMES LIST", sqlite3_errmsg(wombatdb));
    }

    return tmpList;
}

int WombatCaseDb::ReturnCaseID(QString caseName)
{
    int caseid = 0;
    if(sqlite3_prepare_v2(wombatdb, "SELECT caseid FROM cases WHERE casename = ?;", -1, &sqlstatement, NULL) == SQLITE_OK)
    {
        if(sqlite3_bind_text(sqlstatement, 1, caseName.toStdString().c_str(), -1, SQLITE_TRANSIENT) == SQLITE_OK)
        {
            int ret = sqlite3_step(sqlstatement);
            if(ret == SQLITE_ROW || ret == SQLITE_DONE)
            {
                caseid = sqlite3_column_int(sqlstatement, 0);
            }
            else
                DisplayError(wombatparent, "1.6", "RETURN CURRENT CASE ID", sqlite3_errmsg(wombatdb));
        }
        else
            DisplayError(wombatparent, "1.6", "RETURN CURRENT CASE ID", sqlite3_errmsg(wombatdb));
    }
    else
        DisplayError(wombatparent, "1.6", "RETURN CURRENT CASE ID", sqlite3_errmsg(wombatdb));
    
    return caseid;
}

void WombatCaseDb::InsertImage(QString imageName, QString imageFilePath, int caseID)
{
    if(sqlite3_prepare_v2(wombatdb, "INSERT INTO caseimages (imagefullpath, imagename, caseid) VALUES(?, ?, ?);", -1, &sqlstatement, NULL) == SQLITE_OK)
    {
        if(sqlite3_bind_text(sqlstatement, 1, imageFilePath.toStdString().c_str(), -1, SQLITE_TRANSIENT) == SQLITE_OK)
        {
            if(sqlite3_bind_text(sqlstatement, 2, imageName.toStdString().c_str(), -1, SQLITE_TRANSIENT) == SQLITE_OK)
            {
                if(sqlite3_bind_int(sqlstatement, 3, caseID) == SQLITE_OK)
                {
                    int ret = sqlite3_step(sqlstatement);
                    if(ret == SQLITE_ROW || ret == SQLITE_DONE)
                    {
                        // it was successful
                    }
                    else
                        DisplayError(wombatparent, "1.7", "INSERT IMAGE INTO CASE", sqlite3_errmsg(wombatdb));
                }
                else
                    DisplayError(wombatparent, "1.7", "INSERT IMAGE INTO CASE", sqlite3_errmsg(wombatdb));
            }
            else
                DisplayError(wombatparent, "1.7", "INSERT IMAGE INTO CASE", sqlite3_errmsg(wombatdb));
        }
        else
            DisplayError(wombatparent, "1.7", "INSERT IMAGE INTO CASE", sqlite3_errmsg(wombatdb));
    }
    else
        DisplayError(wombatparent, "1.7", "INSERT IMAGE INTO CASE", sqlite3_errmsg(wombatdb));
}

QStringList WombatCaseDb::ReturnCaseImages(int caseID)
{
    QStringList tmpList;
    if(sqlite3_prepare_v2(wombatdb, "SELECT imagefullpath FROM caseimages WHERE caseid = ?;", -1, &sqlstatement, NULL) == SQLITE_OK)
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
            DisplayError(wombatparent, "1.8", "RETURN CASE IMAGES", sqlite3_errmsg(wombatdb));
        }
    }
    else
        DisplayError(wombatparent, "1.8", "RETURN CASE IMAGES", sqlite3_errmsg(wombatdb));

    return tmpList;
}
