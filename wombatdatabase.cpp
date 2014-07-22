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
            LogEntry(0, 0, 0, 0, wombatptr->casedb.lastError().text());
        }
        casequery.finish();
    }
    else
    {
        LogEntry(0, 0, 0, 0, wombatptr->casedb.lastError().text());
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
           LogEntry(0, 0, 0, 0, wombatptr->casedb.lastError().text());
       casequery.finish();
   }
   else
        LogEntry(0, 0, 0, 0, wombatptr->casedb.lastError().text());
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
        LogEntry(0, 0, 0, 0, wombatptr->casedb.lastError().text());
       casequery.finish();
   }
   else
        LogEntry(0, 0, 0, 0, wombatptr->casedb.lastError().text());
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
           casequery.addBindValue(invalues.at(i));
       }
       if(casequery.exec())
           tmpid = casequery.lastInsertId().toInt();
       else
        LogEntry(0, 0, 0, 0, wombatptr->casedb.lastError().text());
       casequery.finish();
   }
   else
        LogEntry(0, 0, 0, 0, wombatptr->casedb.lastError().text());

   return tmpid;
}

int WombatDatabase::ReturnSqlRowsAffected(QString query, QVariantList invalues)
{
    int tmpcount = 0;
    if(wombatptr->casedb.isOpen())
    {
        QSqlQuery casequery(wombatptr->casedb);
        casequery.prepare(query);
        for(int i=0; i < invalues.count(); i++)
            casequery.addBindValue(invalues.at(i));
        if(casequery.exec())
            tmpcount = casequery.numRowsAffected();
        else
        {
        }
        casequery.finish();
    }
    else
    {
    }
    return tmpcount;
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
    wombattableschema << "CREATE TABLE settings(settingid INTEGER PRIMARY KEY, name TEXT, value TEXT, type INTEGER);";
    wombattableschema << "CREATE TABLE dataruns(id INTEGER PRIMARY KEY, objectid INTEGER, fullpath TEXT, seqnum INTEGER, start INTEGER, length INTEGER, datattype INTEGER, originalsectstart INTEGER, allocationstatus INTEGER);";
    wombattableschema << "CREATE TABLE artifacts(id INTEGER PRIMARY KEY, objectid INTEGER, context TEXT, attrtype INTEGER, valuetype INTEGER value BLOB);";
    wombattableschema << "CREATE TABLE data(objectid INTEGER PRIMARY KEY, objecttype INTEGER, type INTEGER, name TEXT, fullpath TEXT, parentid INTEGER, parimgid INTEGER, parfsid INTEGER, flags INTEGER, childcount INTEGER, endian INTEGER, address INTEGER, size INTEGER, sectsize INTEGER, sectstart INTEGER, sectlength INTEGER, dirtype INTEGER, metattype INTEGER, dirflags INTEGER, metaflags INTEGER, ctime INTEGER, crtime INTEGER, atime INTEGER, mtime INTEGER, mode INTEGER, uid INTEGER, gid INTEGER, status INTEGER, md5 TEXT, sha1 TEXT, sha_256 TEXT, sha_512 TEXT, known INTEGER, inodenumber INTEGER, mftattrid INTEGER, mftattrtype INTEGER, byteoffset INTEGER, blocksize INTEGER, blockcount INTEGER, rootinum INTEGER, firstinum INTEGER, lastinum INTEGER, derivationdetails TEXT);";
    if(wombatptr->casedb.open())
    {
        QSqlQuery casequery(wombatptr->casedb);
        wombatptr->casedb.transaction();
        for(int i=0; i < wombattableschema.count(); i++)
        {
            casequery.exec(wombattableschema.at(i));
        }
        wombatptr->casedb.commit();
        casequery.finish();
    }
    else
        LogEntry(0, 0, 0, 0, wombatptr->casedb.lastError().text());

}

void WombatDatabase::CreateLogDB()
{
    QStringList loglist;
    loglist.clear();
    loglist << "CREATE TABLE joblog(jobid INTEGER PRIMARY KEY, type INTEGER, state INTEGER, filecount INTEGER, processcount INTEGER, caseid INTEGER, evidenceid INTEGER, start INT, finish INT, errorcount INT)";
    loglist << "CREATE TABLE msglog(logid INTEGER PRIMARY KEY, caseid INTEGER, evidenceid INTEGER, jobid INTEGER, type INTEGER, datetime INT, logmsg TEXT)";
    if(logdb.open())
    {
        QSqlQuery logquery(logdb);
        logdb.transaction();
        for(int i=0; i < loglist.count(); i++)
        {
            logquery.exec(loglist.at(i));
        }
        logdb.commit();
        logquery.finish();
    }
    else
    {
        wombatptr->curerrmsg = logdb.lastError().text();
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
        LogEntry(0, 0, 0, 0, wombatptr->appdb.lastError().text());
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

void WombatDatabase::OpenLogDB()
{
    if(!logdb.isOpen())
        logdb.open();
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
    if(wombatptr->appdb.isOpen())
    {
        wombatptr->appdb.close();
        wombatptr->appdb = QSqlDatabase();
        QSqlDatabase::removeDatabase("appdb");
    }
}

void WombatDatabase::CloseLogDB()
{
    if(logdb.isOpen())
    {
        logdb.close();
        logdb = QSqlDatabase();
        QSqlDatabase::removeDatabase("logdb");
    }
}

void WombatDatabase::CloseCaseDB()
{
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
    CloseLogDB();
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
        wombatptr->bindvalues.append((int)wombatptr->evidenceobject.imageinfo->size);
        wombatptr->bindvalues.append(wombatptr->evidenceobject.volinfo->block_size);
        wombatptr->bindvalues.append(wombatptr->evidenceobject.volinfo->part_count);
        wombatptr->bindvalues.append((int)wombatptr->evidenceobject.volinfo->offset);
        wombatptr->bindvalues.append(wombatptr->currentevidenceid);
        wombatptr->bindvalues.append(wombatptr->currentevidenceid);
        wombatptr->bindvalues.append(wombatptr->currentvolumename);
        wombatptr->currentvolumeid = InsertSqlGetID("INSERT INTO data (objecttype, type, size, sectsize, childcount, byteoffset, parentid, parimgid, name) VALUES(2, ?, ?, ?, ?, ?, ?, ?, ?);", wombatptr->bindvalues);
    }
    else
    {
        wombatptr->bindvalues.clear();
        wombatptr->bindvalues.append((int)wombatptr->evidenceobject.imageinfo->size);
        wombatptr->bindvalues.append(wombatptr->evidenceobject.imageinfo->sector_size);
        wombatptr->bindvalues.append(wombatptr->currentevidenceid);
        wombatptr->bindvalues.append(wombatptr->currentevidenceid);
        wombatptr->bindvalues.append(wombatptr->currentvolumename);
        wombatptr->currentvolumeid = InsertSqlGetID("INSERT INTO data (objecttype, type, childcount, size, sectsize, byteoffset, parentid, parimgid, name) VALUES(2, 240, 0, ?, ?, 0, ?, ?, ?);", wombatptr->bindvalues);
    }
    filesprocessed++;
}

void WombatDatabase::InsertPartitionObjects()
{
    if(wombatptr->evidenceobject.volinfo != NULL)
    {
        for(uint32_t i=0; i < wombatptr->evidenceobject.partinfovector.size(); i++)
        {
            if(wombatptr->evidenceobject.partinfovector[i]->flags == 0x02) // unallocated partition
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
                wombatptr->bindvalues.append((int)(wombatptr->evidenceobject.partinfovector[i]->len * wombatptr->evidenceobject.volinfo->block_size));
                wombatptr->currentpartitionid = InsertSqlGetID("INSERT INTO data (objecttype, flags, sectstart, sectlength, name, sectsize, parentid, parimgid, size) VALUES(3, ?, ?, ?, ?, ?, ?, ?, ?);", wombatptr->bindvalues);
                filesprocessed++;
            }
            else if(wombatptr->evidenceobject.partinfovector[i]->flags == 0x01) // allocated partition
            {
                TSK_FS_INFO* tmpfsinfo = tsk_fs_open_vol(wombatptr->evidenceobject.partinfovector[i], TSK_FS_TYPE_DETECT);
                if(tmpfsinfo != NULL)
                {
                    wombatptr->evidenceobject.fsinfovector.push_back(tmpfsinfo);
                    filesfound++;
                }
                else
                {
                    LogEntry(wombatptr->caseobject.id, wombatptr->currentevidenceid, currentjobid, 0, "Failed to Open Partition/FileSystem");
                    errorcount++;
                }
                // BEGIN VOLUME NAME TEST
                TSK_FS_FILE* rootfileinfo = tsk_fs_file_open_meta(tmpfsinfo, NULL, tmpfsinfo->root_inum);
                if(tsk_fs_file_attr_getsize(rootfileinfo) > 0)
                {
                    for(int i=0; i < tsk_fs_file_attr_getsize(rootfileinfo); i++)
                    {
                        TSK_FS_ATTR* tmpattr;
                        if(i == 0)
                            tmpattr = rootfileinfo->meta->attr->head;
                        else
                            tmpattr = rootfileinfo->meta->attr->head->next;
                        if(tmpattr != NULL)
                        {
                            if(tmpattr->name != NULL)
                                qDebug() << QString("Attribute ID|Name is: ") << QString::number(tmpattr->id) << "|" << QString(tmpattr->name);
                            else
                                qDebug() << "Attribute Name is null ID is: " << QString::number(tmpattr->id);
                        }
                        else
                        {
                            qDebug() << "attribute is null";
                        }
                    }
                }
                else
                {
                    qDebug() << "root inum has no attributes.";
                }
                // END VOLUME NAME TEST
                wombatptr->currentfilesystemid = 0;
                wombatptr->bindvalues.clear();
                wombatptr->bindvalues.append(QString::fromUtf8(tsk_fs_type_toname(tmpfsinfo->ftype)).toUpper());
                wombatptr->bindvalues.append(QString("/"));
                wombatptr->bindvalues.append(tmpfsinfo->ftype);
                wombatptr->bindvalues.append(tmpfsinfo->flags);
                wombatptr->bindvalues.append((int)tmpfsinfo->offset);
                wombatptr->bindvalues.append(wombatptr->currentvolumeid);
                wombatptr->bindvalues.append(wombatptr->currentevidenceid);
                wombatptr->bindvalues.append((int)tmpfsinfo->block_size * (int)tmpfsinfo->block_count);
                wombatptr->bindvalues.append(tmpfsinfo->block_size);
                wombatptr->bindvalues.append((int)tmpfsinfo->block_count);
                wombatptr->bindvalues.append((int)tmpfsinfo->first_inum);
                wombatptr->bindvalues.append((int)tmpfsinfo->last_inum);
                wombatptr->bindvalues.append((int)tmpfsinfo->root_inum);
                wombatptr->bindvalues.append((int)tmpfsinfo->root_inum);
                wombatptr->bindvalues.append((int)wombatptr->evidenceobject.volinfo->block_size);
                wombatptr->bindvalues.append((int)wombatptr->evidenceobject.partinfovector[i]->start);
                wombatptr->bindvalues.append((int)wombatptr->evidenceobject.partinfovector[i]->len);
                wombatptr->currentfilesystemid = InsertSqlGetID("INSERT INTO data (objecttype, name, fullpath, type, flags, byteoffset, parentid, parimgid, size, blocksize, blockcount, firstinum, lastinum, rootinum, address, sectsize, sectstart, sectlength) VALUES(4, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?);", wombatptr->bindvalues);
                wombatptr->evidenceobject.fsidvector.push_back(wombatptr->currentfilesystemid);
                filesprocessed++;
            }
        }
    }
}
/*
void WombatDatabase::InsertFileSystemObjects()
{
    wombatptr->evidenceobject.fsidvector.clear();
    if(wombatptr->evidenceobject.fsinfovector.size() > 0)
    {
        for(uint32_t i=0; i < wombatptr->evidenceobject.fsinfovector.size(); i++)
        {
            wombatptr->currentfilesystemid = 0;
            wombatptr->bindvalues.clear();
            wombatptr->bindvalues.append(QString::fromUtf8(tsk_fs_type_toname(wombatptr->evidenceobject.fsinfovector[i]->ftype)).toUpper());
            wombatptr->bindvalues.append(QString("/"));
            wombatptr->bindvalues.append(wombatptr->evidenceobject.fsinfovector[i]->ftype);
            wombatptr->bindvalues.append(wombatptr->evidenceobject.fsinfovector[i]->flags);
            wombatptr->bindvalues.append((int)wombatptr->evidenceobject.fsinfovector[i]->offset);
            wombatptr->bindvalues.append(wombatptr->currentvolumeid);
            wombatptr->bindvalues.append(wombatptr->currentevidenceid);
            wombatptr->bindvalues.append((int)wombatptr->evidenceobject.fsinfovector[i]->block_size * (int)wombatptr->evidenceobject.fsinfovector[i]->block_count);
            wombatptr->bindvalues.append(wombatptr->evidenceobject.fsinfovector[i]->block_size);
            wombatptr->bindvalues.append((int)wombatptr->evidenceobject.fsinfovector[i]->block_count);
            wombatptr->bindvalues.append((int)wombatptr->evidenceobject.fsinfovector[i]->first_inum);
            wombatptr->bindvalues.append((int)wombatptr->evidenceobject.fsinfovector[i]->last_inum);
            wombatptr->bindvalues.append((int)wombatptr->evidenceobject.fsinfovector[i]->root_inum);
            wombatptr->bindvalues.append((int)wombatptr->evidenceobject.fsinfovector[i]->root_inum);
            wombatptr->bindvalues.append((int)wombatptr->evidenceobject.fsinfovector[i]->
            wombatptr->currentfilesystemid = InsertSqlGetID("INSERT INTO data (objecttype, name, fullpath, type, flags, byteoffset, parentid, parimgid, size, blocksize, blockcount, firstinum, lastinum, rootinum, address, sectsize, sectstart, sectlength) VALUES(4, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?);", wombatptr->bindvalues);
            wombatptr->evidenceobject.fsidvector.push_back(wombatptr->currentfilesystemid);
            filesprocessed++;
        }
    }
}
*/
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
    wombatptr->evidenceobject.id = wombatptr->currentevidenceid;
    currentevidenceid = wombatptr->currentevidenceid;
    for(int i=0; i < wombatptr->evidenceobject.itemcount; i++)
    {
        wombatptr->bindvalues.clear();
        wombatptr->bindvalues.append(wombatptr->currentevidenceid);
        wombatptr->bindvalues.append(QString::fromStdString(wombatptr->evidenceobject.fullpathvector[i]));
        wombatptr->bindvalues.append(i+1);
        InsertSql("INSERT INTO dataruns (objectid, fullpath, seqnum) VALUES(?, ?, ?);", wombatptr->bindvalues);
    }
    filesprocessed++;
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
    wombatptr->evidenceobject.parimgid = 0;
    wombatptr->bindvalues.clear();
    wombatptr->evidenceobject.fullpathvector.clear();
    wombatptr->bindvalues.append(wombatptr->evidenceobject.id);
    wombatptr->sqlrecords.clear();
    wombatptr->sqlrecords = GetSqlResults("SELECT fullpath FROM dataruns WHERE objectid = ? ORDER BY seqnum", wombatptr->bindvalues);
    for(int i=0; i < wombatptr->sqlrecords.count(); i++)
        wombatptr->evidenceobject.fullpathvector.push_back(wombatptr->sqlrecords[i].value(0).toString().toStdString());
    wombatptr->evidenceobject.itemcount = wombatptr->evidenceobject.fullpathvector.size();
}

void WombatDatabase::GetEvidenceObjects()
{
    wombatptr->evidenceobjectvector.clear();
    wombatptr->evidenceobject.Clear();
    wombatptr->bindvalues.clear();
    wombatptr->sqlrecords.clear();
    wombatptr->sqlrecords = GetSqlResults("SELECT objectid, objecttype, type, size, sectsize, name, fullpath FROM data WHERE objecttype = 1;", wombatptr->bindvalues);
    for(int i=0; i < wombatptr->sqlrecords.count(); i++)
    {
        wombatptr->evidenceobject.id = wombatptr->sqlrecords[i].value(0).toInt();
        currentevidenceid = wombatptr->evidenceobject.id;
        wombatptr->evidenceobject.objecttype = wombatptr->sqlrecords[i].value(1).toInt();
        wombatptr->evidenceobject.type = wombatptr->sqlrecords[i].value(2).toInt();
        wombatptr->evidenceobject.size = wombatptr->sqlrecords[i].value(3).toInt();
        wombatptr->evidenceobject.sectsize = wombatptr->sqlrecords[i].value(4).toInt();
        wombatptr->evidenceobject.name = wombatptr->sqlrecords[i].value(5).toString();
        wombatptr->evidenceobject.fullpath = wombatptr->sqlrecords[i].value(6).toString();
        wombatptr->evidenceobject.parimgid = 0;
        wombatptr->evidenceobjectvector.append(wombatptr->evidenceobject);
    }
    for(int i=0; i < wombatptr->evidenceobjectvector.count(); i++)
    {
        wombatptr->bindvalues.clear();
        wombatptr->sqlrecords.clear();
        wombatptr->bindvalues.append(wombatptr->evidenceobjectvector[i].id);
        wombatptr->sqlrecords = GetSqlResults("SELECT fullpath FROM dataruns WHERE objectid = ? ORDER BY seqnum", wombatptr->bindvalues);
        for(int j=0; j < wombatptr->sqlrecords.count(); j++)
            wombatptr->evidenceobjectvector[i].fullpathvector.push_back(wombatptr->sqlrecords[j].value(0).toString().toStdString());
        wombatptr->evidenceobjectvector[i].itemcount = wombatptr->evidenceobjectvector.at(i).fullpathvector.size();
    }
}

int WombatDatabase::ReturnCaseCount() // from appdb
{
    int retval = 0;
    QSqlQuery appquery("SELECT COUNT(caseid) FROM cases WHERE deleted = 0;", wombatptr->appdb);
    if(appquery.first())
        retval = appquery.value(0).toInt();
    else
        LogEntry(0, 0, 0, 0, wombatptr->appdb.lastError().text());
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
        LogEntry(0, 0, 0, 0, wombatptr->appdb.lastError().text());
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
        LogEntry(0, 0, 0, 0, wombatptr->appdb.lastError().text());
    appquery.finish();
}

void WombatDatabase::ReturnEvidenceNameList()
{
    wombatptr->bindvalues.clear();
    wombatptr->sqlrecords.clear();
    wombatptr->sqlrecords = GetSqlResults("SELECT objectid, name FROM data WHERE objecttype == 1", wombatptr->bindvalues);
    for(int i=0; i < wombatptr->sqlrecords.count(); i++)
    {
        QString tmpstring = wombatptr->sqlrecords[i].value(0).toString() + ". " + wombatptr->sqlrecords[i].value(1).toString();
        wombatptr->evidencenamelist.append(tmpstring);
    }
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
            LogEntry(0, 0, 0, 0, wombatptr->appdb.lastError().text());
    else
        LogEntry(0, 0, 0, 0, wombatptr->appdb.lastError().text());
    appquery.finish();
}
void WombatDatabase::ReturnFileSystemObjectList(int curevidenceid)
{
    fsobjectlist.clear();
    FileSystemObject tmpobject;
    wombatptr->bindvalues.clear();
    wombatptr->bindvalues.append(curevidenceid);
    wombatptr->sqlrecords.clear();
    wombatptr->sqlrecords = GetSqlResults("SELECT objectid, rootinum FROM data WHERE objecttype = 4 and parimgid = ?", wombatptr->bindvalues);
    for(int i=0; i < wombatptr->sqlrecords.count(); i++)
    {
        tmpobject.id = wombatptr->sqlrecords.at(i).value(0).toInt();
        tmpobject.rootinum = wombatptr->sqlrecords.at(i).value(1).toInt();
        fsobjectlist.append(tmpobject);
    }
}

void WombatDatabase::GetObjectValues()
{
    wombatptr->bindvalues.clear();
    wombatptr->bindvalues.append(wombatptr->selectedobject.id);
    wombatptr->sqlrecords.clear();
    wombatptr->sqlrecords = GetSqlResults("SELECT objecttype, size, parimgid, sectstart, sectlength, sectsize, blockcount, byteoffset, address, type, flags, blocksize, parfsid FROM data WHERE objectid = ?", wombatptr->bindvalues);
    wombatptr->selectedobject.objtype = wombatptr->sqlrecords[0].value(0).toInt();
    wombatptr->selectedobject.size = wombatptr->sqlrecords[0].value(1).toInt();
    wombatptr->selectedobject.parimgid = wombatptr->sqlrecords[0].value(2).toInt();
    wombatptr->selectedobject.sectstart = wombatptr->sqlrecords[0].value(3).toInt();
    wombatptr->selectedobject.sectlength = wombatptr->sqlrecords[0].value(4).toInt();
    wombatptr->selectedobject.sectsize = wombatptr->sqlrecords[0].value(5).toInt();
    wombatptr->selectedobject.blockcount = wombatptr->sqlrecords[0].value(6).toInt();
    wombatptr->selectedobject.byteoffset = wombatptr->sqlrecords[0].value(7).toInt();
    wombatptr->selectedobject.address = wombatptr->sqlrecords[0].value(8).toInt();
    wombatptr->selectedobject.type = wombatptr->sqlrecords[0].value(9).toInt();
    wombatptr->selectedobject.flags = wombatptr->sqlrecords[0].value(10).toInt();
    wombatptr->selectedobject.blocksize = wombatptr->sqlrecords[0].value(11).toInt();
    wombatptr->selectedobject.parfsid = wombatptr->sqlrecords[0].value(12).toInt();
}

int WombatDatabase::GetEvidenceFileCount()
{
    wombatptr->bindvalues.clear();
    wombatptr->bindvalues.append(wombatptr->currentevidenceid);
    wombatptr->bindvalues.append(wombatptr->currentevidenceid);
    wombatptr->sqlrecords.clear();
    wombatptr->sqlrecords = GetSqlResults("SELECT COUNT(objectid) FROM data WHERE objectid = ? OR parimgid = ?", wombatptr->bindvalues);
    return wombatptr->sqlrecords.at(0).value(0).toInt();
}

void WombatDatabase::ReturnObjectPropertyList()
{
    propertylist.clear();
    if(wombatptr->selectedobject.objtype == 1) // image file
    {
        propertylist << QString("File Format") << QString(tsk_img_type_todesc((TSK_IMG_TYPE_ENUM)wombatptr->selectedobject.type)) << QString("File Format the evidence data is stored in. Usually it is either a raw image (.dd/.001) or an embedded image (.E01/.AFF). A raw image contains only the data from the evidence. The embedded image contains other descriptive information from the acquisition.");
        propertylist << QString("Sector Size") << QString(QString::number(wombatptr->selectedobject.blocksize) + " bytes") << QString("Sector size of the device. A Sector is a subdivision of a disk where data is stored. It is the smallest value used to divide the disk.");
        propertylist << QString("Sector Count") << QString(QString::number((int)((float)wombatptr->selectedobject.size/(float)wombatptr->selectedobject.blocksize)) + " sectors") << QString("The number of sectors in the disk.");
    }
    else if(wombatptr->selectedobject.objtype == 2) // volume information
    {
        if(wombatptr->selectedobject.type == 240) // dummy volume system
        {
            // there is no relevant information for a dummy volume.
        }
        else // actual volume system
        {
            // get the partition table information, offset, block size, endian ordering, volume system type.
        }
    }
    else if(wombatptr->selectedobject.objtype == 3) // partition information (shouldn't exist) its combined with fs
    {
    }
    else if(wombatptr->selectedobject.objtype == 4) // file system information
    {
    }
    else if(wombatptr->selectedobject.objtype == 5) // dir/file information
    {
    }
}

void WombatDatabase::RemoveEvidence()
{
    wombatptr->bindvalues.clear();
    wombatptr->bindvalues.append(wombatptr->evidremoveid);
    wombatptr->bindvalues.append(wombatptr->evidremoveid);
    wombatptr->evidrowsremoved = ReturnSqlRowsAffected("DELETE FROM data WHERE objectid = ? or parimgid = ?", wombatptr->bindvalues);
    wombatptr->bindvalues.clear();
    wombatptr->bindvalues.append(wombatptr->evidremoveid);
    InsertSql("DELETE FROM dataruns WHERE objectid = ?", wombatptr->bindvalues);
}
int WombatDatabase::ReturnFileSystemOffset(int fsid)
{
    wombatptr->bindvalues.clear();
    wombatptr->bindvalues.append(fsid);
    wombatptr->sqlrecords.clear();
    wombatptr->sqlrecords = GetSqlResults("SELECT byteoffset FROM data WHERE objectid = ?", wombatptr->bindvalues);
    return wombatptr->sqlrecords.at(0).value(0).toInt();
}
