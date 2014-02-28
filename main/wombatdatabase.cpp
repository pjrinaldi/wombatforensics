#include "wombatdatabase.h"

WombatDatabase::WombatDatabase(WombatVariable* wombatvarptr)
{
    wombatptr = wombatvarptr;
}

QList<QSqlRecord> WombatDatabase::GetSqlResults(QString query, QVariantList invalues)
{
    QList<QSqlRecord> tmplist;
    if(wombatptr->casedb.isOpen())
    {
        QSqlQuery casequery;
        casequery.prepare(query);
        for(int i=0; i < invalues.count(); i++)
        {
            casequery.addBindValue(invalues[i]);
        }
        if(casequery.exec())
        {
            while(casequery.next())
            {
                tmplist.append(casequery.record());
            }
        }
        else qDebug() << wombatptr->casedb.lastError().text();
    }
    else
        qDebug() << wombatptr->casedb.lastError().text();

    return tmplist;

}

void WombatDatabase::InsertSql(QString query, QVariantList invalues)
{
   if(wombatptr->casedb.isOpen())
   {
       QSqlQuery casequery;
       casequery.prepare(query);
       for(int i=0; i < invalues.count(); i++)
       {
           casequery.addBindValue(invalues[i]);
       }
       if(casequery.exec())
       {
           while(casequery.next())
           {
               qDebug() << "successful insert with bind";
           }
       }
       else
           qDebug() << wombatptr->casedb.lastError().text();
   }
   else
       qDebug() << wombatptr->casedb.lastError().text();
}


void WombatDatabase::InsertSql(QString query)
{
   if(wombatptr->casedb.isOpen())
   {
       QSqlQuery casequery;
       casequery.prepare(query);
       if(casequery.exec())
       {
           qDebug() << "successful query";
       }
       else
           qDebug() << wombatptr->casedb.lastError().text();
   }
   else
       qDebug() << wombatptr->casedb.lastError().text();
}

int WombatDatabase::InsertSqlGetID(QString query, QVariantList invalues)
{
   int tmpid = 0;
   if(wombatptr->casedb.isOpen())
   {
       QSqlQuery casequery;
       casequery.prepare(query);
       for(int i=0; i < invalues.count(); i++)
       {
           casequery.addBindValue(invalues[i]);
       }
       if(casequery.exec())
           tmpid = casequery.lastInsertId().toInt();
       else
           qDebug() << wombatptr->casedb.lastError().text();
   }
   else
       qDebug() << wombatptr->casedb.lastError().text();

   return tmpid;
}

void WombatDatabase::CreateCaseDB(void)
{
    #define IMGDB_CHUNK_SIZE 1024*1024*1 // what size chunks should the database use when growing and shrinking
    #define IMGDB_MAX_RETRY_COUNT 50    // how many times will we retry a SQL statement
    #define IMGDB_RETRY_WAIT 100   // how long (in milliseconds) are we willing to wait between retries
    // set page size -- 4k is much faster on Windows than the default
    //executeStatement("PRAGMA page_size = 4096;", casestatement, "TskImgDBSqlite::initialize");
    //sqlite3_finalize(casestatement);

    // we don't have a mechanism to recover from a crash anyway
    //executeStatement("PRAGMA synchronous = 0;", casestatement, "TskImgDBSqlite::initialize");
    //sqlite3_finalize(casestatement);
    QStringList wombattableschema;
    wombattableschema.clear();
    wombattableschema << "CREATE TABLE job(jobid INTEGER PRIMARY KEY, type INTEGER, state INTEGER, filecount INTEGER, processcount INTEGER, caseid INTEGER, evidenceid INTEGER, start TEXT, end TEXT);";
    wombattableschema << "CREATE TABLE settings(settingid INTEGER PRIMARY KEY, name TEXT, value TEXT, type INT);";
    wombattableschema << "CREATE TABLE dataruns(id INTEGER PRIMARY KEY, objectid INTEGER, fullpath TEXT, seqnum INTEGER, start INTEGER, length INTEGER, datattype INTEGER, originalsectstart INTEGER, allocationstatus INTEGER);";
    wombattableschema << "CREATE TABLE artifacts(id INTEGER PRIMARY KEY, objectid INTEGER, context TEXT, attrtype INTEGER, valuetype INTEGER value BLOB);";
    wombattableschema << "CREATE TABLE msglog(logid INTEGER PRIMARY KEY, caseid INTEGER, evidenceid INTEGER, jobid INTEGER, msgtype INTEGER, msg TEXT, datetime TEXT);";
    wombattableschema << "CREATE TABLE data(objectid INTEGER PRIMARY KEY, objecttype INTEGER, type INTEGER, name TEXT, fullpath TEXT, parentid INTEGER, flags INTEGER, childcount INTEGER, endian INTEGER, address INTEGER, size INTEGER, sectsize INTEGER, sectstart INTEGER, sectlength INTEGER, dirtype INTEGER, metattype INTEGER, dirflags INTEGER, metaflags INTEGER, ctime INTEGER, crtime INTEGER, atime INTEGER, mtime INTEGER, mode INTEGER, uid INTEGER, gid INTEGER, status INTEGER, md5 TEXT, sha1 TEXT, sha_256 TEXT, sha_512 TEXT, known INTEGER, indoenumber INTEGER, mftattrid INTEGER, mftattrtype INTEGER, byteoffset INTEGER, blockcount INTEGER, rootinum INTEGER, firstinum INTEGER, lastinum INTEGER, derivationdetails TEXT);";
    if(wombatptr->casedb.open())
    {
        QSqlQuery casequery;
        for(int i=0; i < wombattableschema.count(); i++)
        {
            casequery.exec(wombattableschema[i]);
        }
    }
    else
        qDebug() << wombatptr->casedb.lastError().text();

}

void WombatDatabase::CreateAppDB()
{
    if(wombatptr->appdb.open())
    {
        QSqlQuery appquery(wombatptr->appdb);
        appquery.exec("CREATE TABLE cases(caseid INTEGER PRIMARY KEY, name TEXT, creation TEXT, deleted INTEGER);");
    }
    else
        qDebug() << wombatptr->appdb.lastError().text();
}

void WombatDatabase::OpenAppDB()
{
    if(wombatptr->appdb.isOpen())
        qDebug() << "appdb is open";
    else
        wombatptr->appdb.open();
}

void WombatDatabase::OpenCaseDB()
{
    if(wombatptr->casedb.isOpen())
        qDebug() << "case is open.";
    else
        wombatptr->casedb.open();
}

void WombatDatabase::CloseAppDB()
{
    /*
    if(sqlite3_finalize(wombatstatement) == SQLITE_OK)
    {
        if(sqlite3_close(wombatdb) == SQLITE_OK)
        {
            //fprintf(stderr, "CloseDB was successful\n");
            wombatptr->curerrmsg = "";
        }
        else
        {
            fprintf(stderr, "CloseDB: %s\n", sqlite3_errmsg(wombatdb));
            wombatptr->curerrmsg = QString(sqlite3_errmsg(wombatdb));
        }
    }
    else
    {
        fprintf(stderr, "CloseDB: %s\n", sqlite3_errmsg(wombatdb));
        wombatptr->curerrmsg = QString(sqlite3_errmsg(wombatdb));
    }
    */
}

WombatDatabase::~WombatDatabase()
{
    CloseAppDB();
}

void WombatDatabase::InsertVolumeObject()
{
    // might need to add endianordering INTEGER column
    wombatptr->volumeobject.id = 0;
    if(wombatptr->evidenceobject.volinfo != NULL)
    {
        wombatptr->bindvalues.clear();
        wombatptr->bindvalues.append(wombatptr->evidenceobject.volinfo->vstype);
        wombatptr->bindvalues.append(wombatptr->evidenceobject.volinfo->block_size);
        wombatptr->bindvalues.append(wombatptr->evidenceobject.volinfo->part_count);
        wombatptr->bindvalues.append((int)wombatptr->evidenceobject.volinfo->offset);
        wombatptr->bindvalues.append(wombatptr->evidenceobject.id);
        wombatptr->bindvalues.append(wombatptr->volumeobject.name);
        wombatptr->volumeobject.id = InsertSqlGetID("INSERT INTO data (objecttype, type, childcount, size, byteoffset, parentid, name) VALUES(2, ?, ?, ?, ?, ?, ?);", wombatptr->bindvalues);
    }
    else
    {
        wombatptr->bindvalues.clear();
        wombatptr->bindvalues.append(wombatptr->evidenceobject.imageinfo->sector_size);
        wombatptr->bindvalues.append(wombatptr->evidenceobject.id);
        wombatptr->bindvalues.append(wombatptr->volumeobject.name);
        wombatptr->volumeobject.id = InsertSqlGetID("INSERT INTO data (objecttype, type, childcount, size, byteoffset, parentid, name) VALUES(2, 240, 0, ?, 0, ?, ?);", wombatptr->bindvalues);
    }
}

void WombatDatabase::GetVolumeObject()
{
    wombatptr->bindvalues.clear();
    wombatptr->bindvalues.append(wombatptr->evidenceobject.id);
    wombatptr->sqlrecords.clear();
    wombatptr->sqlrecords = GetSqlResults("SELECT objectid, type, size, byteoffset, parentid, childcount, name FROM data WHERE parentid = ? and objecttype = 2", wombatptr->bindvalues);
    wombatptr->volumeobject.id = wombatptr->sqlrecords[0].value(0).toInt();
    wombatptr->volumeobject.type = wombatptr->sqlrecords[0].value(1).toInt();
    wombatptr->volumeobject.blocksize = wombatptr->sqlrecords[0].value(2).toInt();
    wombatptr->volumeobject.byteoffset = wombatptr->sqlrecords[0].value(3).toInt();
    wombatptr->volumeobject.parentid = wombatptr->sqlrecords[0].value(4).toInt();
    wombatptr->volumeobject.childcount = wombatptr->sqlrecords[0].value(5).toInt();
    wombatptr->volumeobject.name = wombatptr->sqlrecords[0].value(6).toString();
}

void WombatDatabase::InsertPartitionObjects()
{
    if(wombatptr->evidenceobject.volinfo != NULL)
    {
        for(uint32_t i=0; i < wombatptr->evidenceobject.volinfo->part_count; i++)
        {
            wombatptr->partitionobject.name = QString::fromUtf8(tsk_vs_part_get(wombatptr->evidenceobject.volinfo, i)->desc);
            wombatptr->partitionobject.id = 0;
            wombatptr->bindvalues.clear();
            wombatptr->bindvalues.append(wombatptr->evidenceobject.partinfovector[i]->flags);
            wombatptr->bindvalues.append((int)wombatptr->evidenceobject.partinfovector[i]->start);
            wombatptr->bindvalues.append((int)wombatptr->evidenceobject.partinfovector[i]->len);
            wombatptr->bindvalues.append(wombatptr->evidenceobject.partinfovector[i]->desc);
            wombatptr->bindvalues.append(wombatptr->volumeobject.id);
            wombatptr->partitionobject.id = InsertSqlGetID("INSERT INTO data (objecttype, flags, sectstart, sectlength, name, parentid) VALUES(3, ?, ?, ?, ?, ?);", wombatptr->bindvalues);
            wombatptr->partitionobjectvector.append(wombatptr->partitionobject);
        }
    }
}

void WombatDatabase::GetPartitionObjects()
{
    wombatptr->partitionobjectvector.clear();
    wombatptr->bindvalues.clear();
    wombatptr->bindvalues.append(wombatptr->volumeobject.id);
    wombatptr->sqlrecords.clear();
    wombatptr->sqlrecords = GetSqlResults("SELECT objectid, flags, secstart, sectlength, name, parentid FROM data WHERE parentid = ? AND objecttype = 3 ORDER BY objectid", wombatptr->bindvalues);
    for(int i=0; i < wombatptr->sqlrecords.count(); i++)
    {
        wombatptr->partitionobject.id = wombatptr->sqlrecords[i].value(0).toInt();
        wombatptr->partitionobject.flags = wombatptr->sqlrecords[i].value(1).toInt();
        wombatptr->partitionobject.sectstart = wombatptr->sqlrecords[i].value(2).toInt();
        wombatptr->partitionobject.sectlength = wombatptr->sqlrecords[i].value(3).toInt();
        wombatptr->partitionobject.name = wombatptr->sqlrecords[i].value(4).toString();
        wombatptr->partitionobject.parentid = wombatptr->sqlrecords[i].value(5).toInt();
        wombatptr->partitionobjectvector.append(wombatptr->partitionobject);
    }
}

void WombatDatabase::InsertFileSystemObjects()
{
    qDebug() << "fsinfo vector count: " << wombatptr->evidenceobject.fsinfovector.size();
    if(wombatptr->evidenceobject.fsinfovector.size() > 0)
    {
        for(uint32_t i=0; i < wombatptr->evidenceobject.fsinfovector.size(); i++)
        {
            wombatptr->filesystemobject.name = QString::fromUtf8(wombatptr->evidenceobject.fsinfovector[i]->duname); // duname = data unit name (clusters)
            wombatptr->filesystemobject.id = 0;
            wombatptr->bindvalues.clear();
            wombatptr->bindvalues.append(wombatptr->evidenceobject.fsinfovector[i]->ftype);
            wombatptr->bindvalues.append(wombatptr->evidenceobject.fsinfovector[i]->flags);
            wombatptr->bindvalues.append((int)wombatptr->evidenceobject.fsinfovector[i]->offset);
            wombatptr->bindvalues.append(wombatptr->volumeobject.id);
            wombatptr->bindvalues.append(wombatptr->evidenceobject.fsinfovector[i]->block_size);
            wombatptr->bindvalues.append((int)wombatptr->evidenceobject.fsinfovector[i]->block_count);
            wombatptr->bindvalues.append((int)wombatptr->evidenceobject.fsinfovector[i]->first_inum);
            wombatptr->bindvalues.append((int)wombatptr->evidenceobject.fsinfovector[i]->last_inum);
            wombatptr->bindvalues.append((int)wombatptr->evidenceobject.fsinfovector[i]->root_inum);
            wombatptr->filesystemobject.id = InsertSqlGetID("INSERT INTO data (objecttype, type, flags, byteoffset, parentid, size, blockcount, firstinum, lastinum, rootinum) VALUES(4, ?, ?, ?, ?, ?, ?, ?, ?, ?);", wombatptr->bindvalues);
            wombatptr->filesystemobjectvector.append(wombatptr->filesystemobject);
            qDebug() << wombatptr->evidenceobject.fsinfovector[i]->duname;
        }
    }
}

void WombatDatabase::GetFileSystemObjects()
{
    wombatptr->filesystemobjectvector.clear();
    wombatptr->bindvalues.clear();
    wombatptr->bindvalues.append(wombatptr->volumeobject.id);
    wombatptr->sqlrecords.clear();
    wombatptr->sqlrecords = GetSqlResults("SELECT objectid, type, flags, byteoffset, parentid, size, blockcount, firstinum, lastinum, rootinum FROM data WHERE parentid = ? and objecttype = 4 ORDER BY objectid", wombatptr->bindvalues);
    for(int i=0; i < wombatptr->sqlrecords.count(); i++)
    {
        wombatptr->filesystemobject.id = wombatptr->sqlrecords[i].value(0).toInt(); 
        wombatptr->filesystemobject.type = wombatptr->sqlrecords[i].value(1).toInt();
        wombatptr->filesystemobject.flags = wombatptr->sqlrecords[i].value(2).toInt();
        wombatptr->filesystemobject.byteoffset = wombatptr->sqlrecords[i].value(3).toInt();
        wombatptr->filesystemobject.parentid = wombatptr->sqlrecords[i].value(4).toInt();
        wombatptr->filesystemobject.size = wombatptr->sqlrecords[i].value(5).toInt();
        wombatptr->filesystemobject.blockcount = wombatptr->sqlrecords[i].value(6).toInt();
        wombatptr->filesystemobject.firstinum = wombatptr->sqlrecords[i].value(7).toInt();
        wombatptr->filesystemobject.lastinum = wombatptr->sqlrecords[i].value(8).toInt();
        wombatptr->filesystemobject.rootinum = wombatptr->sqlrecords[i].value(9).toInt();
        wombatptr->filesystemobjectvector.append(wombatptr->filesystemobject);
    }
}

void WombatDatabase::InsertEvidenceObject()
{
    wombatptr->evidenceobject.id = 0;
    wombatptr->bindvalues.clear();
    wombatptr->bindvalues.append(wombatptr->evidenceobject.imageinfo->itype);
    wombatptr->bindvalues.append((int)wombatptr->evidenceobject.imageinfo->size);
    wombatptr->bindvalues.append(wombatptr->evidenceobject.imageinfo->sector_size);
    wombatptr->bindvalues.append(wombatptr->evidenceobject.name);
    wombatptr->bindvalues.append(QString::fromStdString(wombatptr->evidenceobject.fullpathvector[0]));
    wombatptr->evidenceobject.id = InsertSqlGetID("INSERT INTO data (objecttype, type, size, sectsize, name, fullpath) VALUES(1, ?, ?, ?, ?, ?);", wombatptr->bindvalues);
    for(int i=0; i < wombatptr->evidenceobject.itemcount; i++)
    {
        wombatptr->bindvalues.clear();
        wombatptr->bindvalues.append(wombatptr->evidenceobject.id);
        wombatptr->bindvalues.append(QString::fromStdString(wombatptr->evidenceobject.fullpathvector[i]));
        wombatptr->bindvalues.append(i+1);
        InsertSql("INSERT INTO dataruns (objectid, fullpath, seqnum) VALUES(?, ?, ?);", wombatptr->bindvalues);
    }
}

void WombatDatabase::GetEvidenceObject()
{
    // already have id and name from adding/opening the evidence
    wombatptr->bindvalues.clear();
    wombatptr->bindvalues.append(wombatptr->evidenceobject.id);
    wombatptr->sqlrecords.clear();
    wombatptr->sqlrecords = GetSqlResults("SELECT objecttype, type, size, sectsize, name, fullpath FROM data WHERE objectid = ?;", wombatptr->bindvalues);
    wombatptr->evidenceobject.objecttype = wombatptr->sqlrecords[0].value(0).toInt();
    wombatptr->evidenceobject.type = wombatptr->sqlrecords[0].value(1).toInt();
    wombatptr->evidenceobject.size = wombatptr->sqlrecords[0].value(2).toInt();
    wombatptr->evidenceobject.sectsize = wombatptr->sqlrecords[0].value(3).toInt();
    wombatptr->evidenceobject.name = wombatptr->sqlrecords[0].value(4).toString();
    wombatptr->evidenceobject.fullpath = wombatptr->sqlrecords[0].value(5).toString();
}

void WombatDatabase::GetEvidenceObjects()
{
    for(int i=0; i < wombatptr->evidenceobjectvector.count(); i++)
    {
        // need a way to call getevidenceobject() for each evidencevectorobject
    }
}

/*
void WombatDatabase::InitializeEvidenceDatabase()
{
}
*/

int WombatDatabase::ReturnCaseCount() // from appdb
{
    QSqlQuery appquery("SELECT COUNT(caseid) FROM cases WHERE deleted = 0;", wombatptr->appdb);
    if(appquery.first())
        return appquery.value(0).toInt();
    else
        qDebug() << wombatptr->appdb.lastError().text();
    
    return 0;

}

void WombatDatabase::InsertCase()
{
    QSqlQuery appquery(wombatptr->appdb);
    appquery.prepare("INSERT INTO cases (name, creation, deleted) VALUES(?, ?, 0);");
    appquery.addBindValue(wombatptr->caseobject.name);
    appquery.addBindValue(QString::fromStdString(GetTime()));
    if(appquery.exec())
        wombatptr->caseobject.id = appquery.lastInsertId().toInt();
    else
        qDebug() << "insert case failed: " << wombatptr->appdb.lastError().text();
}

void WombatDatabase::ReturnCaseNameList()
{
    QSqlQuery appquery(wombatptr->appdb);
    if(appquery.exec("SELECT name FROM cases WHERE deleted = 0 ORDER BY caseid;"))
    {
        while(appquery.next())
            wombatptr->casenamelist << appquery.value(0).toString();
    }
    else
        qDebug() << wombatptr->appdb.lastError().text();
}

void WombatDatabase::ReturnCaseID()
{
    QSqlQuery appquery(wombatptr->appdb);
    appquery.prepare("SELECT caseid FROM cases WHERE name = ?;");
    appquery.addBindValue(wombatptr->caseobject.name);
    if(appquery.exec())
        if(appquery.first())
            wombatptr->caseobject.id = appquery.value(0).toInt();
        else
            qDebug() << wombatptr->appdb.lastError().text();
    else
        qDebug() << wombatptr->appdb.lastError().text();
}

void WombatDatabase::GetObjectType()
{
    wombatptr->bindvalues.clear();
    wombatptr->bindvalues.append(wombatptr->selectedobject.id);
    wombatptr->sqlrecords.clear();
    wombatptr->sqlrecords = GetSqlResults("SELECT objecttype FROM data WHERE objectid = ?", wombatptr->bindvalues);
    wombatptr->selectedobject.type = wombatptr->sqlrecords[0].value(0).toInt();
}

// not used will eventually be deleted.
/*
int WombatDatabase::ReturnObjectFileID(int objectid)
{
    int fileid = 0;
    if(sqlite3_prepare_v2(wombatdb, "SELECT fileid FROM objects WHERE objectid = ?;", -1, &casestatement, NULL) == SQLITE_OK)
    {
        if(sqlite3_bind_int(casestatement, 1, objectid) == SQLITE_OK)
        {
            int ret = sqlite3_step(casestatement);
            if(ret == SQLITE_ROW || ret == SQLITE_DONE)
            {
                fileid = sqlite3_column_int(casestatement, 0);
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
    if(sqlite3_prepare_v2(wombatdb, "SELECT evidenceid FROM objects WHERE objectid = ?;", -1, &casestatement, NULL) == SQLITE_OK)
    {
        if(sqlite3_bind_int(casestatement, 1, objectid) == SQLITE_OK)
        {
            int ret = sqlite3_step(casestatement);
            if(ret == SQLITE_ROW || ret == SQLITE_DONE)
                evidenceid = sqlite3_column_int(casestatement, 0);
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
    if(sqlite3_prepare_v2(wombatdb, "SELECT objectid FROM objects WHERE caseid = ? AND evidenceid = ? AND fileid = ?;", -1, &casestatement, NULL) == SQLITE_OK)
    {
        if(sqlite3_bind_int(casestatement, 1, caseid) == SQLITE_OK && sqlite3_bind_int(casestatement, 2, evidenceid) == SQLITE_OK && sqlite3_bind_int(casestatement, 3, fileid) == SQLITE_OK)
        {
            int ret = sqlite3_step(casestatement);
            if(ret == SQLITE_ROW || ret == SQLITE_DONE)
                objectid = sqlite3_column_int(casestatement, 0);
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

    if(sqlite3_prepare_v2(wombatdb, "INSERT INTO job (type, caseid, evidenceid, start) VALUES(?, ?, ?, ?);", -1, &casestatement, NULL) == SQLITE_OK)
    {
        if(sqlite3_bind_int(casestatement, 1, jobType) == SQLITE_OK && sqlite3_bind_int(casestatement, 2, caseID) == SQLITE_OK && sqlite3_bind_int(casestatement, 3, evidenceID) == SQLITE_OK && sqlite3_bind_text(casestatement, 4, GetTime().c_str(), -1, SQLITE_TRANSIENT) == SQLITE_OK)
        {
            int ret = sqlite3_step(casestatement);
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
    if(sqlite3_prepare_v2(wombatdb, "INSERT INTO evidence (fullpath, name, caseid, creation, deleted) VALUES(?, ?, ?, ?, 0);", -1, &casestatement, NULL) == SQLITE_OK)
    {
        if(sqlite3_bind_text(casestatement, 1, evidenceFilePath.toStdString().c_str(), -1, SQLITE_TRANSIENT) == SQLITE_OK && sqlite3_bind_text(casestatement, 2, evidenceName.toStdString().c_str(), -1, SQLITE_TRANSIENT) == SQLITE_OK && sqlite3_bind_int(casestatement, 3, caseID) == SQLITE_OK && sqlite3_bind_text(casestatement, 4, GetTime().c_str(), -1, SQLITE_TRANSIENT) == SQLITE_OK)
        {
            int ret = sqlite3_step(casestatement);
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

int WombatDatabase::InsertObject(int caseid, int evidenceid, int itemtype, int curid)
{
    int objectid = 0;
    std::string tmpquery = "INSERT INTO objects (caseid, evidenceid, ";
    if(itemtype == 0) // item is file
        tmpquery += "fileid";
    else if(itemtype == 1) // item is partition
        tmpquery += "partid";
    else if(itemtype == 2) // item is a volume
        tmpquery += "volid";
    else if(itemtype == 3) // item is an image
        tmpquery += "imgid";
    tmpquery += ") VALUES(?, ?, ?);";
    if(sqlite3_prepare_v2(wombatdb, tmpquery.c_str(), -1, &casestatement, NULL) == SQLITE_OK)
    {
        if(sqlite3_bind_int(casestatement, 1, caseid) == SQLITE_OK && sqlite3_bind_int(casestatement, 2, evidenceid) == SQLITE_OK && sqlite3_bind_int(casestatement, 3, curid) == SQLITE_OK)
        {
            int ret = sqlite3_step(casestatement);
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

QStringList WombatDatabase::ReturnCaseActiveEvidenceID(int caseID)
{
    QStringList tmpList;
    if(sqlite3_prepare_v2(wombatdb, "SELECT evidenceid,fullpath,name FROM evidence WHERE caseid = ? AND deleted = 0 ORDER BY evidenceid;", -1, &casestatement, NULL) == SQLITE_OK)
    {
        if(sqlite3_bind_int(casestatement, 1, caseID) == SQLITE_OK)
        {
            while(sqlite3_step(casestatement) == SQLITE_ROW)
            {
                tmpList << QString::number(sqlite3_column_int(casestatement, 0)) << (const char*)sqlite3_column_text(casestatement, 1) << (const char*)sqlite3_column_text(casestatement, 2);
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

QStringList WombatDatabase::ReturnCaseActiveEvidence(int caseID)
{
    QStringList tmpList;
    if(sqlite3_prepare_v2(wombatdb, "SELECT fullpath FROM evidence WHERE caseid = ? AND deleted = 0 ORDER BY evidenceid;", -1, &casestatement, NULL) == SQLITE_OK)
    {
        if(sqlite3_bind_int(casestatement, 1, caseID) == SQLITE_OK)
        {
            while(sqlite3_step(casestatement) == SQLITE_ROW)
            {
                tmpList << (const char*)sqlite3_column_text(casestatement, 0);
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

QStringList WombatDatabase::ReturnCaseEvidenceIdJobIdType(int caseid)
{
    QStringList tmplist;
    if(sqlite3_prepare_v2(wombatdb, "SELECT jobid,type,evidenceid FROM job WHERE caseid = ?;", -1, &casestatement, NULL) == SQLITE_OK)
    {
        if(sqlite3_bind_int(casestatement, 1, caseid) == SQLITE_OK)
        {
            while(sqlite3_step(casestatement) == SQLITE_ROW)
            {
                tmplist << QString::number(sqlite3_column_int(casestatement, 0)) << QString::number(sqlite3_column_int(casestatement, 1)) << QString::number(sqlite3_column_int(casestatement, 2));
            }
        }
        else
            emit DisplayError("1.15", "RETURN CASE EVIDENCE ADD JOB ID/TYPE ", sqlite3_errmsg(wombatdb));
    }
    else
        emit DisplayError("1.15", "RETURN CASE EVIDENCE ADD JOB ID/TYPE ", sqlite3_errmsg(wombatdb));

    return tmplist;
}

QStringList WombatDatabase::ReturnEvidenceData(int evidenceid)
{
    QStringList tmplist;
    if(sqlite3_prepare_v2(wombatdb, "SELECT fullpath, name FROM evidence WHERE evidenceid = ?;", -1, &casestatement, NULL) == SQLITE_OK)
    {
        if(sqlite3_bind_int(casestatement, 1, evidenceid) == SQLITE_OK)
        {
            int ret = sqlite3_step(casestatement);
            if(ret == SQLITE_ROW || ret == SQLITE_DONE)
            {
                tmplist << QString((const char*)sqlite3_column_text(casestatement, 0));
                tmplist << QString((const char*)sqlite3_column_text(casestatement, 1));
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

QStringList WombatDatabase::ReturnMessageTableEntries(int jobid)
{
    QStringList tmpstringlist;
    QString tmptype;
    if(sqlite3_prepare_v2(wombatdb, "SELECT msgtype, msg FROM msglog WHERE jobid = ?;", -1, &casestatement, NULL) == SQLITE_OK)
    {
        if(sqlite3_bind_int(casestatement, 1, jobid) == SQLITE_OK)
        {
            while(sqlite3_step(casestatement) == SQLITE_ROW)
            {
                if(sqlite3_column_int(casestatement, 0) == 0)
                    tmptype = "[ERROR]";
                else if(sqlite3_column_int(casestatement, 0) == 1)
                    tmptype = "[WARN]";
                else
                    tmptype = "[INFO]";
                tmpstringlist << tmptype << QString((const char*)sqlite3_column_text(casestatement, 1));
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
    if(sqlite3_prepare_v2(wombatdb, "INSERT INTO msglog (caseid, evidenceid, jobid, msgtype, msg, datetime) VALUES(?, ?, ?, ?, ?, ?);", -1, &casestatement, NULL) == SQLITE_OK)
    {
        if(sqlite3_bind_int(casestatement, 1, caseid) == SQLITE_OK && sqlite3_bind_int(casestatement, 2, evidenceid) == SQLITE_OK && sqlite3_bind_int(casestatement, 3, jobid) == SQLITE_OK && sqlite3_bind_int(casestatement, 4, msgtype) == SQLITE_OK && sqlite3_bind_text(casestatement, 5, msg, -1, SQLITE_TRANSIENT) == SQLITE_OK && sqlite3_bind_text(casestatement, 6, GetTime().c_str(), -1, SQLITE_TRANSIENT) == SQLITE_OK)
        {
            int ret = sqlite3_step(casestatement);
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
    if(sqlite3_prepare_v2(wombatdb, "SELECT end, filecount, processcount, state FROM job WHERE jobid = ?;", -1, &casestatement, NULL) == SQLITE_OK)
    {
        if(sqlite3_bind_int(casestatement, 1, jobid) == SQLITE_OK)
        {
            int ret = sqlite3_step(casestatement);
            if(ret == SQLITE_ROW || ret == SQLITE_DONE)
            {
                tmplist << QString((const char*)sqlite3_column_text(casestatement, 0)) << QString::number(sqlite3_column_int(casestatement, 1)) << QString::number(sqlite3_column_int(casestatement, 2));
                if(sqlite3_column_int(casestatement, 3) == 1)
                    tmplist << "Processing Finished";
                else
                    tmplist << "Processing Finished with Errors";
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
    if(sqlite3_prepare_v2(wombatdb, "UPDATE job SET end = ?, filecount = ?, processcount = ?, state = 1 WHERE jobid = ?;", -1, &casestatement, NULL) == SQLITE_OK)
    {
        if(sqlite3_bind_text(casestatement, 1, GetTime().c_str(), -1, SQLITE_TRANSIENT) == SQLITE_OK && sqlite3_bind_int(casestatement, 2, filecount) == SQLITE_OK && sqlite3_bind_int(casestatement, 3, processcount) == SQLITE_OK && sqlite3_bind_int(casestatement, 4, jobid) == SQLITE_OK)
        {
            int ret = sqlite3_step(casestatement);
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
    if(sqlite3_prepare_v2(wombatdb, "SELECT caseid FROM job WHERE jobid = ?;", -1, &casestatement, NULL) == SQLITE_OK)
    {
        if(sqlite3_bind_int(casestatement, 1, jobid) == SQLITE_OK)
        {
            int ret = sqlite3_step(casestatement);
            if(ret == SQLITE_ROW || ret == SQLITE_DONE)
            {
                caseid = sqlite3_column_int(casestatement, 0);
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
    if(sqlite3_prepare_v2(wombatdb, "SELECT evidenceid FROM job WHERE jobid = ?;", -1, &casestatement, NULL) == SQLITE_OK)
    {
        if(sqlite3_bind_int(casestatement, 1, jobid) == SQLITE_OK)
        {
            int ret = sqlite3_step(casestatement);
            if(ret == SQLITE_ROW || ret == SQLITE_DONE)
            {
                evidenceid = sqlite3_column_int(casestatement, 0);
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
    if(sqlite3_prepare_v2(wombatdb, "UPDATE evidence SET deleted = 1 WHERE fullpath = ?;", -1, &casestatement, NULL) == SQLITE_OK)
    {
        if(sqlite3_bind_text(casestatement, 1, evidencename.toStdString().c_str(), -1, SQLITE_TRANSIENT) == SQLITE_OK)
        {
            int ret = sqlite3_step(casestatement);
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

int WombatDatabase::ReturnEvidenceID(QString evidencename)
{
    int evidenceid = 0;
    if(sqlite3_prepare_v2(wombatdb, "SELECT evidenceid FROM evidence WHERE fullpath = ? and deleted = 0;", -1, &casestatement, NULL) == SQLITE_OK)
    {
        if(sqlite3_bind_text(casestatement, 1, evidencename.toStdString().c_str(), -1, SQLITE_TRANSIENT) == SQLITE_OK)
        {
            int ret = sqlite3_step(casestatement);
            if(ret == SQLITE_ROW || ret == SQLITE_DONE)
            {
                evidenceid = sqlite3_column_int(casestatement, 0);
            }
            else
                emit DisplayError("1.26", "RETURN EVIDENCE ID FROM NAME ", sqlite3_errmsg(wombatdb));
        }
        else
            emit DisplayError("1.26", "RETURN EVIDENCE ID FROM NAME ", sqlite3_errmsg(wombatdb));
    }
    else
        emit DisplayError("1.26", "RETURN EVIDENCE ID FROM NAME ", sqlite3_errmsg(wombatdb));

    return evidenceid;
}

int WombatDatabase::ReturnEvidenceDeletedState(int evidenceid)
{
    int isdeleted = 0;
    if(sqlite3_prepare_v2(wombatdb, "SELECT deleted FROM evidence WHERE evidenceid = ?;", -1, &casestatement, NULL) == SQLITE_OK)
    {
        if(sqlite3_bind_int(casestatement, 1, evidenceid) == SQLITE_OK)
        {
            int ret = sqlite3_step(casestatement);
            if(ret == SQLITE_ROW || ret == SQLITE_DONE)
            {
                isdeleted = sqlite3_column_int(casestatement, 0);
            }
            else
                emit DisplayError("1.28", "RETURN EVIDENCE DELETED STATE FROM ID ", sqlite3_errmsg(wombatdb));
        }
        else
            emit DisplayError("1.28", "RETURN EVIDENCE DELETED STATE FROM ID ", sqlite3_errmsg(wombatdb));
    }
    else
        emit DisplayError("1.28", "RETURN EVIDENCE DELETED STATE FROM ID ", sqlite3_errmsg(wombatdb));

    return isdeleted;
}

QString WombatDatabase::ReturnEvidencePath(int evidenceid)
{
    QString epath = "";
    if(sqlite3_prepare_v2(wombatdb, "SELECT fullpath FROM evidence WHERE evidenceid = ?;", -1, &casestatement, NULL) == SQLITE_OK)
    {
        if(sqlite3_bind_int(casestatement, 1, evidenceid) == SQLITE_OK)
        {
            int ret = sqlite3_step(casestatement);
            if(ret == SQLITE_ROW || ret == SQLITE_DONE)
            {
                epath = QString((const char*)sqlite3_column_text(casestatement, 0));
            }
            else
                emit DisplayError("1.27", "RETURN EVIDENCE PATH FROM ID ", sqlite3_errmsg(wombatdb));
        }
        else
            emit DisplayError("1.27", "RETURN EVIDENCE PATH FROM ID ", sqlite3_errmsg(wombatdb));
    }
    else
        emit DisplayError("1.27", "RETURN EVIDENCE PATH FROM ID ", sqlite3_errmsg(wombatdb));

    return epath;
}*/
