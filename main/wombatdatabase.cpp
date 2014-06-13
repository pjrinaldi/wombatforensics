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
        QSqlQuery casequery(wombatptr->casedb);
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
        else
        {
            //qDebug() << wombatptr->casedb.lastError().text();
        }
        casequery.finish();
    }
    else
    {
        //qDebug() << wombatptr->casedb.lastError().text();
    }

    return tmplist;

}

void WombatDatabase::InsertSql(QString query, QVariantList invalues)
{
   if(wombatptr->casedb.isOpen())
   {
       QSqlQuery casequery(wombatptr->casedb);
       casequery.prepare(query);
       for(int i=0; i < invalues.count(); i++)
       {
           casequery.addBindValue(invalues[i]);
       }
       if(casequery.exec())
       {
           while(casequery.next())
           {
               //qDebug() << "successful insert with bind";
           }
       }
       else
       {
           //qDebug() << wombatptr->casedb.lastError().text();
       }
       casequery.finish();
   }
   else
   {
       //qDebug() << wombatptr->casedb.lastError().text();
   }
}


void WombatDatabase::InsertSql(QString query)
{
   if(wombatptr->casedb.isOpen())
   {
       QSqlQuery casequery(wombatptr->casedb);
       casequery.prepare(query);
       if(casequery.exec())
       {
           //qDebug() << "successful query";
       }
       else
       {
           //qDebug() << wombatptr->casedb.lastError().text();
       }
       casequery.finish();
   }
   else
   {
       //qDebug() << wombatptr->casedb.lastError().text();
   }
}

int WombatDatabase::InsertSqlGetID(QString query, QVariantList invalues)
{
   int tmpid = 0;
   if(wombatptr->casedb.isOpen())
   {
       QSqlQuery casequery(wombatptr->casedb);
       casequery.prepare(query);
       for(int i=0; i < invalues.count(); i++)
       {
           casequery.addBindValue(invalues[i]);
       }
       if(casequery.exec())
           tmpid = casequery.lastInsertId().toInt();
       else
       {
           //qDebug() << wombatptr->casedb.lastError().text();
       }
       casequery.finish();
   }
   else
   {
       //qDebug() << wombatptr->casedb.lastError().text();
   }

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
    wombattableschema << "CREATE TABLE settings(settingid INTEGER PRIMARY KEY, name TEXT, value TEXT, type INTEGER);";
    wombattableschema << "CREATE TABLE dataruns(id INTEGER PRIMARY KEY, objectid INTEGER, fullpath TEXT, seqnum INTEGER, start INTEGER, length INTEGER, datattype INTEGER, originalsectstart INTEGER, allocationstatus INTEGER);";
    wombattableschema << "CREATE TABLE artifacts(id INTEGER PRIMARY KEY, objectid INTEGER, context TEXT, attrtype INTEGER, valuetype INTEGER value BLOB);";
    wombattableschema << "CREATE TABLE msglog(logid INTEGER PRIMARY KEY, caseid INTEGER, evidenceid INTEGER, jobid INTEGER, msgtype INTEGER, msg TEXT, datetime TEXT);";
    wombattableschema << "CREATE TABLE data(objectid INTEGER PRIMARY KEY, objecttype INTEGER, type INTEGER, name TEXT, fullpath TEXT, parentid INTEGER, parimgid INTEGER, flags INTEGER, childcount INTEGER, endian INTEGER, address INTEGER, size INTEGER, sectsize INTEGER, sectstart INTEGER, sectlength INTEGER, dirtype INTEGER, metattype INTEGER, dirflags INTEGER, metaflags INTEGER, ctime INTEGER, crtime INTEGER, atime INTEGER, mtime INTEGER, mode INTEGER, uid INTEGER, gid INTEGER, status INTEGER, md5 TEXT, sha1 TEXT, sha_256 TEXT, sha_512 TEXT, known INTEGER, indoenumber INTEGER, mftattrid INTEGER, mftattrtype INTEGER, byteoffset INTEGER, blockcount INTEGER, rootinum INTEGER, firstinum INTEGER, lastinum INTEGER, derivationdetails TEXT);";
    if(wombatptr->casedb.open())
    {
        QSqlQuery casequery(wombatptr->casedb);
        wombatptr->casedb.transaction();
        for(int i=0; i < wombattableschema.count(); i++)
        {
            casequery.exec(wombattableschema[i]);
        }
        wombatptr->casedb.commit();
        casequery.finish();
    }
    else
    {
        //qDebug() << wombatptr->casedb.lastError().text();
    }

}

void WombatDatabase::CreateAppDB()
{
    if(wombatptr->appdb.open())
    {
        QSqlQuery appquery(wombatptr->appdb);
        appquery.exec("CREATE TABLE cases(caseid INTEGER PRIMARY KEY, name TEXT, creation TEXT, deleted INTEGER);");
        appquery.finish();
    }
    else
    {
        //qDebug() << wombatptr->appdb.lastError().text();
    }
}

void WombatDatabase::OpenAppDB()
{
    if(wombatptr->appdb.isOpen())
    {
        //qDebug() << "appdb is open";
    }
    else
        wombatptr->appdb.open();
}

void WombatDatabase::OpenCaseDB()
{
    if(wombatptr->casedb.isOpen())
    {
        //qDebug() << "case is open.";
    }
    else
        wombatptr->casedb.open();
}

void WombatDatabase::CloseAppDB()
{
    qDebug() << "AppDB closing";
    if(wombatptr->appdb.isOpen())
    {
        wombatptr->appdb.close();
        wombatptr->appdb = QSqlDatabase();
        //delete wombatptr->appdb;
        QSqlDatabase::removeDatabase("wombatapp");
    }
}

void WombatDatabase::CloseCaseDB()
{
    qDebug() << "CaseDB Closing";
    if(wombatptr->casedb.isOpen())
        wombatptr->casedb.close();
    if(fcasedb.isOpen())
        fcasedb.close();
    wombatptr->casedb = QSqlDatabase();
    fcasedb = QSqlDatabase();
    QSqlDatabase::removeDatabase("casedb");
}

WombatDatabase::~WombatDatabase()
{
    CloseAppDB();
}

void WombatDatabase::InsertVolumeObject()
{
    // might need to add endianordering INTEGER column
    wombatptr->currentvolumeid = 0;
    if(wombatptr->evidenceobject.volinfo != NULL)
    {
        wombatptr->bindvalues.clear();
        wombatptr->bindvalues.append(wombatptr->evidenceobject.volinfo->vstype);
        wombatptr->bindvalues.append(wombatptr->evidenceobject.volinfo->block_size);
        wombatptr->bindvalues.append(wombatptr->evidenceobject.volinfo->part_count);
        wombatptr->bindvalues.append((int)wombatptr->evidenceobject.volinfo->offset);
        wombatptr->bindvalues.append(wombatptr->currentevidenceid);
        wombatptr->bindvalues.append(wombatptr->currentevidenceid);
        wombatptr->bindvalues.append(wombatptr->currentvolumename);
        wombatptr->currentvolumeid = InsertSqlGetID("INSERT INTO data (objecttype, type, size, childcount, byteoffset, parentid, parimgid, name) VALUES(2, ?, ?, ?, ?, ?, ?, ?);", wombatptr->bindvalues);
    }
    else
    {
        wombatptr->bindvalues.clear();
        wombatptr->bindvalues.append(wombatptr->evidenceobject.imageinfo->sector_size);
        wombatptr->bindvalues.append(wombatptr->currentevidenceid);
        wombatptr->bindvalues.append(wombatptr->currentevidenceid);
        wombatptr->bindvalues.append(wombatptr->currentvolumename);
        wombatptr->currentvolumeid = InsertSqlGetID("INSERT INTO data (objecttype, type, childcount, size, byteoffset, parentid, parimgid, name) VALUES(2, 240, 0, ?, 0, ?, ?, ?);", wombatptr->bindvalues);
    }
}
// may not need
void WombatDatabase::GetVolumeObjects()
{
    /*
    wombatptr->volumeobjectvector.clear();
    for(int i=0; i < wombatptr->evidenceobjectvector.count(); i++)
    {
        wombatptr->bindvalues.clear();
        wombatptr->bindvalues.append(wombatptr->evidenceobjectvector[i].id);
        wombatptr->sqlrecords.clear();
        wombatptr->sqlrecords = GetSqlResults("SELECT objectid, type, size, byteoffset, parentid, parimgid, childcount, name, objecttype FROM data WHERE parentid = ? and objecttype = 2", wombatptr->bindvalues);
        wombatptr->volumeobject.id = wombatptr->sqlrecords[0].value(0).toInt();
        wombatptr->volumeobject.type = wombatptr->sqlrecords[0].value(1).toInt();
        wombatptr->volumeobject.blocksize = wombatptr->sqlrecords[0].value(2).toInt();
        wombatptr->volumeobject.byteoffset = wombatptr->sqlrecords[0].value(3).toInt();
        wombatptr->volumeobject.parentid = wombatptr->sqlrecords[0].value(4).toInt();
        wombatptr->volumeobject.parimgid = wombatptr->sqlrecords[0].value(5).toInt();
        wombatptr->volumeobject.childcount = wombatptr->sqlrecords[0].value(6).toInt();
        wombatptr->volumeobject.name = wombatptr->sqlrecords[0].value(7).toString();
        wombatptr->volumeobject.objecttype = wombatptr->sqlrecords[0].value(8).toInt();
        wombatptr->volumeobjectvector.append(wombatptr->volumeobject);
    }*/
}

void WombatDatabase::InsertPartitionObjects()
{
    if(wombatptr->evidenceobject.volinfo != NULL)
    {
        for(uint32_t i=0; i < wombatptr->evidenceobject.volinfo->part_count; i++)
        {
            wombatptr->currentpartitionid = 0;
            wombatptr->bindvalues.clear();
            wombatptr->bindvalues.append(wombatptr->evidenceobject.partinfovector[i]->flags);
            wombatptr->bindvalues.append((int)wombatptr->evidenceobject.partinfovector[i]->start);
            wombatptr->bindvalues.append((int)wombatptr->evidenceobject.partinfovector[i]->len);
            wombatptr->bindvalues.append(wombatptr->evidenceobject.partinfovector[i]->desc);
            wombatptr->bindvalues.append(wombatptr->evidenceobject.volinfo->block_size);
            wombatptr->bindvalues.append(wombatptr->currentvolumeid);
            wombatptr->bindvalues.append(wombatptr->currentevidenceid);
            wombatptr->currentpartitionid = InsertSqlGetID("INSERT INTO data (objecttype, flags, sectstart, sectlength, name, size, parentid, parimgid) VALUES(3, ?, ?, ?, ?, ?, ?, ?);", wombatptr->bindvalues);
        }
    }
}
// may not need
void WombatDatabase::GetPartitionObjects()
{
    /*
    wombatptr->partitionobjectvector.clear();
    for(int j=0; j < wombatptr->volumeobjectvector.count(); j++)
    {
        wombatptr->bindvalues.clear();
        wombatptr->bindvalues.append(wombatptr->volumeobjectvector[j].id);
        wombatptr->sqlrecords.clear();
        wombatptr->sqlrecords = GetSqlResults("SELECT objectid, flags, sectstart, sectlength, name, size, parentid, parimgid, objecttype FROM data WHERE parentid = ? AND objecttype = 3 ORDER BY objectid", wombatptr->bindvalues);
        for(int i=0; i < wombatptr->sqlrecords.count(); i++)
        {
            wombatptr->partitionobject.id = wombatptr->sqlrecords[i].value(0).toInt();
            wombatptr->partitionobject.flags = wombatptr->sqlrecords[i].value(1).toInt();
            wombatptr->partitionobject.sectstart = wombatptr->sqlrecords[i].value(2).toInt();
            wombatptr->partitionobject.sectlength = wombatptr->sqlrecords[i].value(3).toInt();
            wombatptr->partitionobject.name = wombatptr->sqlrecords[i].value(4).toString();
            wombatptr->partitionobject.blocksize = wombatptr->sqlrecords[i].value(5).toInt();
            wombatptr->partitionobject.parentid = wombatptr->sqlrecords[i].value(6).toInt();
            wombatptr->partitionobject.parimgid = wombatptr->sqlrecords[i].value(7).toInt();
            wombatptr->partitionobject.objecttype = wombatptr->sqlrecords[i].value(8).toInt();
            wombatptr->partitionobjectvector.append(wombatptr->partitionobject);
        }
    }*/
}
// may not need
void WombatDatabase::InsertFileObjects() // loop over fileinfovector and add to db.
{
}

void WombatDatabase::InsertFileSystemObjects()
{
    if(wombatptr->evidenceobject.fsinfovector.size() > 0)
    {
        for(uint32_t i=0; i < wombatptr->evidenceobject.fsinfovector.size(); i++)
        {
            //wombatptr->filesystemobject.name = QString::fromUtf8(tsk_fs_type_toname(wombatptr->evidenceobject.fsinfovector[i]->ftype));
            wombatptr->currentfilesystemid = 0;
            wombatptr->bindvalues.clear();
            wombatptr->bindvalues.append(QString::fromUtf8(tsk_fs_type_toname(wombatptr->evidenceobject.fsinfovector[i]->ftype)).toUpper());
            wombatptr->bindvalues.append(QString("/"));
            wombatptr->bindvalues.append(wombatptr->evidenceobject.fsinfovector[i]->ftype);
            wombatptr->bindvalues.append(wombatptr->evidenceobject.fsinfovector[i]->flags);
            wombatptr->bindvalues.append((int)wombatptr->evidenceobject.fsinfovector[i]->offset);
            wombatptr->bindvalues.append(wombatptr->currentvolumeid);
            wombatptr->bindvalues.append(wombatptr->currentevidenceid);
            wombatptr->bindvalues.append(wombatptr->evidenceobject.fsinfovector[i]->block_size);
            wombatptr->bindvalues.append((int)wombatptr->evidenceobject.fsinfovector[i]->block_count);
            wombatptr->bindvalues.append((int)wombatptr->evidenceobject.fsinfovector[i]->first_inum);
            wombatptr->bindvalues.append((int)wombatptr->evidenceobject.fsinfovector[i]->last_inum);
            wombatptr->bindvalues.append((int)wombatptr->evidenceobject.fsinfovector[i]->root_inum);
            wombatptr->currentfilesystemid = InsertSqlGetID("INSERT INTO data (objecttype, name, fullpath, type, flags, byteoffset, parentid, parimgid, size, blockcount, firstinum, lastinum, rootinum) VALUES(4, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?);", wombatptr->bindvalues);
            //wombatptr->filesystemobject.id = InsertSqlGetID("INSERT INTO data (objecttype, name, fullpath, type, flags, byteoffset, parentid, parimgid, size, blockcount, firstinum, lastinum, rootinum) VALUES(4, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?);", wombatptr->bindvalues);
        }
    }
}
// may not need
void WombatDatabase::GetFileSystemObjects()
{
    /*
    wombatptr->filesystemobjectvector.clear();
    for(int j=0; j < wombatptr->volumeobjectvector.count(); j++)
    {
        wombatptr->bindvalues.clear();
        wombatptr->bindvalues.append(wombatptr->volumeobjectvector[j].id);
        wombatptr->sqlrecords.clear();
        wombatptr->sqlrecords = GetSqlResults("SELECT objectid, type, flags, byteoffset, parentid, parimgid, size, blockcount, firstinum, lastinum, rootinum, objecttype FROM data WHERE parentid = ? and objecttype = 4 ORDER BY objectid", wombatptr->bindvalues);
        for(int i=0; i < wombatptr->sqlrecords.count(); i++)
        {
            wombatptr->filesystemobject.id = wombatptr->sqlrecords[i].value(0).toInt(); 
            wombatptr->filesystemobject.type = wombatptr->sqlrecords[i].value(1).toInt();
            wombatptr->filesystemobject.flags = wombatptr->sqlrecords[i].value(2).toInt();
            wombatptr->filesystemobject.byteoffset = wombatptr->sqlrecords[i].value(3).toInt();
            wombatptr->filesystemobject.parentid = wombatptr->sqlrecords[i].value(4).toInt();
            wombatptr->filesystemobject.parimgid = wombatptr->sqlrecords[i].value(5).toInt();
            wombatptr->filesystemobject.blocksize = wombatptr->sqlrecords[i].value(6).toInt();
            wombatptr->filesystemobject.blockcount = wombatptr->sqlrecords[i].value(7).toInt();
            wombatptr->filesystemobject.firstinum = wombatptr->sqlrecords[i].value(8).toInt();
            wombatptr->filesystemobject.lastinum = wombatptr->sqlrecords[i].value(9).toInt();
            wombatptr->filesystemobject.rootinum = wombatptr->sqlrecords[i].value(10).toInt();
            wombatptr->filesystemobject.objecttype = wombatptr->sqlrecords[i].value(10).toInt();
            wombatptr->filesystemobjectvector.append(wombatptr->filesystemobject);
        }
    }*/
}

void WombatDatabase::InsertEvidenceObject()
{
    wombatptr->currentevidenceid = 0;
    currentevidenceid = 0;
    wombatptr->bindvalues.clear();
    wombatptr->bindvalues.append(wombatptr->evidenceobject.imageinfo->itype);
    wombatptr->bindvalues.append((int)wombatptr->evidenceobject.imageinfo->size);
    wombatptr->bindvalues.append(wombatptr->evidenceobject.imageinfo->sector_size);
    wombatptr->bindvalues.append(wombatptr->currentevidencename);
    wombatptr->bindvalues.append(QString::fromStdString(wombatptr->evidenceobject.fullpathvector[0]));
    wombatptr->currentevidenceid = InsertSqlGetID("INSERT INTO data (objecttype, type, size, sectsize, name, fullpath, parimgid) VALUES(1, ?, ?, ?, ?, ?, NULL);", wombatptr->bindvalues);
    //qDebug() << "item count 2: " << wombatptr->evidenceobject.itemcount;
    currentevidenceid = wombatptr->currentevidenceid;
    for(int i=0; i < wombatptr->evidenceobject.itemcount; i++)
    {
        wombatptr->bindvalues.clear();
        wombatptr->bindvalues.append(wombatptr->currentevidenceid);
        wombatptr->bindvalues.append(QString::fromStdString(wombatptr->evidenceobject.fullpathvector[i]));
        wombatptr->bindvalues.append(i+1);
        InsertSql("INSERT INTO dataruns (objectid, fullpath, seqnum) VALUES(?, ?, ?);", wombatptr->bindvalues);
    }
}

void WombatDatabase::GetEvidenceObject()
{
    // already have id and name from adding/opening the evidence
    wombatptr->bindvalues.clear();
    wombatptr->bindvalues.append(wombatptr->currentevidenceid);
    wombatptr->sqlrecords.clear();
    wombatptr->sqlrecords = GetSqlResults("SELECT objecttype, type, size, sectsize, name, fullpath, objectid FROM data WHERE objectid = ?;", wombatptr->bindvalues);
    wombatptr->evidenceobject.objecttype = wombatptr->sqlrecords[0].value(0).toInt();
    wombatptr->evidenceobject.type = wombatptr->sqlrecords[0].value(1).toInt();
    wombatptr->evidenceobject.size = wombatptr->sqlrecords[0].value(2).toInt();
    wombatptr->evidenceobject.sectsize = wombatptr->sqlrecords[0].value(3).toInt();
    wombatptr->evidenceobject.name = wombatptr->sqlrecords[0].value(4).toString();
    wombatptr->evidenceobject.fullpath = wombatptr->sqlrecords[0].value(5).toString();
    wombatptr->evidenceobject.id = wombatptr->sqlrecords[0].value(6).toInt();
    wombatptr->evidenceobject.parimgid = NULL;
    wombatptr->bindvalues.clear();
    wombatptr->evidenceobject.fullpathvector.clear();
    wombatptr->bindvalues.append(wombatptr->evidenceobject.id);
    wombatptr->sqlrecords.clear();
    wombatptr->sqlrecords = GetSqlResults("SELECT fullpath FROM dataruns WHERE objectid = ? ORDER BY seqnum", wombatptr->bindvalues);
    for(int i=0; i < wombatptr->sqlrecords.count(); i++)
        wombatptr->evidenceobject.fullpathvector.push_back(wombatptr->sqlrecords[i].value(0).toString().toStdString());
}

void WombatDatabase::GetEvidenceObjects()
{
    wombatptr->evidenceobjectvector.clear();
    wombatptr->evidenceobject.Clear();
    wombatptr->bindvalues.clear();
    wombatptr->sqlrecords.clear();
    wombatptr->sqlrecords = GetSqlResults("SELECT objectid, objecttype, type, size, sectsize, name, fullpath FROM data WHERE objecttype = 1;", wombatptr->bindvalues);
    //qDebug() << "evid sqlrecords count: " << wombatptr->sqlrecords.count();
    for(int i=0; i < wombatptr->sqlrecords.count(); i++)
    {
        wombatptr->evidenceobject.id = wombatptr->sqlrecords[i].value(0).toInt();
        wombatptr->evidenceobject.objecttype = wombatptr->sqlrecords[i].value(1).toInt();
        wombatptr->evidenceobject.type = wombatptr->sqlrecords[i].value(2).toInt();
        wombatptr->evidenceobject.size = wombatptr->sqlrecords[i].value(3).toInt();
        wombatptr->evidenceobject.sectsize = wombatptr->sqlrecords[i].value(4).toInt();
        wombatptr->evidenceobject.name = wombatptr->sqlrecords[i].value(5).toString();
        wombatptr->evidenceobject.fullpath = wombatptr->sqlrecords[i].value(6).toString();
        wombatptr->evidenceobject.parimgid = NULL;
        wombatptr->evidenceobjectvector.append(wombatptr->evidenceobject);
    }
    //qDebug() << "evidobjvec count: " << wombatptr->evidenceobjectvector.count();
    for(int i=0; i < wombatptr->evidenceobjectvector.count(); i++)
    {
        wombatptr->bindvalues.clear();
        wombatptr->sqlrecords.clear();
        wombatptr->bindvalues.append(wombatptr->evidenceobjectvector[i].id);
        wombatptr->sqlrecords = GetSqlResults("SELECT fullpath FROM dataruns WHERE objectid = ? ORDER BY seqnum", wombatptr->bindvalues);
        //qDebug() << "fullpath sqlrecords count: " << wombatptr->sqlrecords.count();
        for(int j=0; j < wombatptr->sqlrecords.count(); j++)
            wombatptr->evidenceobjectvector[i].fullpathvector.push_back(wombatptr->sqlrecords[j].value(0).toString().toStdString());
    }
}

int WombatDatabase::ReturnCaseCount() // from appdb
{
    int retval = 0;
    QSqlQuery appquery("SELECT COUNT(caseid) FROM cases WHERE deleted = 0;", wombatptr->appdb);
    if(appquery.first())
        retval = appquery.value(0).toInt();
    else
    {
        //qDebug() << wombatptr->appdb.lastError().text();
    }
    appquery.finish();

    return retval;

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
    {
        //qDebug() << "insert case failed: " << wombatptr->appdb.lastError().text();
    }
    appquery.finish();
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
    {
        //qDebug() << wombatptr->appdb.lastError().text();
    }
    appquery.finish();
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
        {
            //qDebug() << wombatptr->appdb.lastError().text();
        }
    else
    {
        //qDebug() << wombatptr->appdb.lastError().text();
    }
    appquery.finish();
}

void WombatDatabase::GetObjectValues()
{
    //qDebug() << "wbtdb wombatptr->selectedobject.id: " << wombatptr->selectedobject.id;
    wombatptr->bindvalues.clear();
    wombatptr->bindvalues.append(wombatptr->selectedobject.id);
    wombatptr->sqlrecords.clear();
    wombatptr->sqlrecords = GetSqlResults("SELECT objecttype, size, parimgid, sectstart, sectlength, sectsize, blockcount, byteoffset, address FROM data WHERE objectid = ?", wombatptr->bindvalues);
    wombatptr->selectedobject.type = wombatptr->sqlrecords[0].value(0).toInt();
    wombatptr->selectedobject.size = wombatptr->sqlrecords[0].value(1).toInt();
    wombatptr->selectedobject.parimgid = wombatptr->sqlrecords[0].value(2).toInt();
    wombatptr->selectedobject.sectstart = wombatptr->sqlrecords[0].value(3).toInt();
    wombatptr->selectedobject.sectlength = wombatptr->sqlrecords[0].value(4).toInt();
    wombatptr->selectedobject.blocksize = wombatptr->sqlrecords[0].value(5).toInt();
    wombatptr->selectedobject.blockcount = wombatptr->sqlrecords[0].value(6).toInt();
    wombatptr->selectedobject.byteoffset = wombatptr->sqlrecords[0].value(7).toInt();
    wombatptr->selectedobject.address = wombatptr->sqlrecords[0].value(8).toInt();
    //qDebug() << "wombatptr->selectedobject.address (inum): " << wombatptr->selectedobject.address;
}

void WombatDatabase::GetRootInum()
{
    wombatptr->bindvalues.clear();
    wombatptr->bindvalues.append(wombatptr->currentfilesystemid);
    wombatptr->sqlrecords.clear();
    wombatptr->sqlrecords = GetSqlResults("SELECT rootinum FROM data WHERE objectid = ?", wombatptr->bindvalues);
    wombatptr->currentrootinum = wombatptr->sqlrecords[0].value(0).toInt();
    qDebug() << "root inum: " << wombatptr->currentrootinum;
}
void WombatDatabase::GetRootNodes()
{
    wombatptr->bindvalues.clear();
    wombatptr->sqlrecords.clear();
    //wombatptr->sqlrecords = GetSqlResults("SELECT objectid, name, fullpath, size, objecttype, address, crtime, atime, mtime, ctime, md5, parentid FROM data WHERE objecttype < 5 OR (objecttype == 5 AND parentid = ?)", wombatptr->bindvalues);
    wombatptr->sqlrecords = GetSqlResults("SELECT objectid, name, fullpath, size, objecttype, address, crtime, atime, mtime, ctime, md5, parentid FROM data", wombatptr->bindvalues);
    for(int i=0; i < wombatptr->sqlrecords.count(); i++)
    {
        currentnode = 0;
        colvalues.clear();
        for(int j=0; j < wombatptr->sqlrecords[i].count(); j++)
        {
            colvalues.append(wombatptr->sqlrecords[i].value(j));
        }
        currentnode = new Node(colvalues);
        if(colvalues.at(4).toInt() < 5) // not file or directory
        {
            if(colvalues.at(4).toInt() == 1) // image node
            {
                currentnode->nodevalues[5] = wombatptr->currentrootinum;
                dummynode = new Node(colvalues);
                dummynode->children.append(currentnode);
                dummynode->nodevalues[5] = wombatptr->currentrootinum;
                dummynode->parent = NULL;
                dummynode->haschildren = true;
                dummynode->childcount = 1;
                currentnode->parent = dummynode;
                currentnode->childcount = 1;
                currentnode->haschildren = true;
                parentnode = currentnode;
            }
            else // volume or partition or filesystem
            {
                currentnode->nodevalues[5] = wombatptr->currentrootinum;
                parentnode->children.append(currentnode);
                currentnode->parent = parentnode;
                currentnode->haschildren = true;
                currentnode->childcount = 1;
                parentnode = currentnode;
            }
        }
        else // its a file or directory at the rootinum level...
        {
            /*
            parentnode->children.append(currentnode);
            currentnode->parent = parentnode;
            QSqlQuery childcountquery(wombatptr->casedb);
            childcountquery.prepare("SELECT COUNT(objectid) as children FROM data WHERE parentid = ?");
            childcountquery.addBindValue(currentnode->nodevalues.at(5).toInt());
            if(childcountquery.exec())
            {
                childcountquery.next();
                currentnode->childcount = childcountquery.value(0).toInt();
                if(currentnode->childcount > 0)
                    currentnode->haschildren = true;
            }*/
            bool nodefound = FindParentNode(currentnode, parentnode, wombatptr->currentrootinum);
            qDebug() << "node found: " << nodefound;
        }
        /*else // file or directory nodes.
        {
            nodefound = FindParentNode(currentnode, parentnode, wombatvarptr->currentrootinum);
            qDebug() << "Node found: " << nodefound;
            if(nodefound)
            {
                qDebug() << "node was found.";
            }
            else
            {
                qDebug() << "node wasn't found. check inum.";
            }
        }
        */
    }
    qDebug() << "get root nodes complete.";
}
