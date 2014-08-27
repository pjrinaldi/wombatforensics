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
    wombattableschema << "CREATE TABLE attributes(id INTEGER PRIMARY KEY, objectid INTEGER, context TEXT, attrtype INTEGER, valuetype INTEGER value BLOB);";
    wombattableschema << "CREATE TABLE properties(id INTEGER PRIMARY KEY, objectid INTEGER, name TEXT, description TEXT, value BLOB);";
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
                wombatptr->currentfilesystemid = 0;
                wombatptr->bindvalues.clear();
                // ONLY TAKES CARE OF AN EXT2FS, I NEED TO MAKE A FUNCTION THAT WILL IF OUT THE VARIOUS FILESYSTEMS AND GET THE RESPECTIVE VALUES AND THEN RETURN THE QSTRING FOR IT
                wombatptr->bindvalues.append(QString::fromStdString(string(((EXT2FS_INFO*)tmpfsinfo)->fs->s_volume_name)));
                //wombatptr->bindvalues.append(QString::fromUtf8(tsk_fs_type_toname(tmpfsinfo->ftype)).toUpper());
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
                InsertFileSystemProperties(wombatptr->currentfilesystemid, tmpfsinfo);
                filesprocessed++;
            }
        }
    }
    else // handle when there is no volume...
    {
        wombatptr->evidenceobject.fsidvector.clear();
        if(wombatptr->evidenceobject.fsinfovector.size() > 0)
        {
            for(uint32_t i=0; i < wombatptr->evidenceobject.fsinfovector.size(); i++)
            {
                wombatptr->currentfilesystemid = 0;
                wombatptr->bindvalues.clear();
                // ONLY TAKES CARE OF AN EXT2FS, I NEED TO MAKE A FUNCTION THAT WILL IF OUT THE VARIOUS FILESYSTEMS AND GET THE RESPECTIVE VALUES AND THEN RETURN THE QSTRING FOR IT
                wombatptr->bindvalues.append(QString::fromStdString(string(((EXT2FS_INFO*)wombatptr->evidenceobject.fsinfovector[i])->fs->s_volume_name)));
                //wombatptr->bindvalues.append(QString::fromUtf8(tsk_fs_type_toname(wombatptr->evidenceobject.fsinfovector[i]->ftype)).toUpper());
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
                wombatptr->bindvalues.append((int)wombatptr->evidenceobject.imageinfo->sector_size);
                wombatptr->bindvalues.append((int)wombatptr->evidenceobject.fsinfovector[i]->offset);
                wombatptr->bindvalues.append((int)wombatptr->evidenceobject.fsinfovector[i]->block_count);
                wombatptr->currentfilesystemid = InsertSqlGetID("INSERT INTO data (objecttype, name, fullpath, type, flags, byteoffset, parentid, parimgid, size, blocksize, blockcount, firstinum, lastinum, rootinum, address, sectsize, sectstart, sectlength) VALUES(4, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?);", wombatptr->bindvalues);
                wombatptr->evidenceobject.fsidvector.push_back(wombatptr->currentfilesystemid);
                InsertFileSystemProperties(wombatptr->currentfilesystemid, wombatptr->evidenceobject.fsinfovector[i]);
                filesprocessed++;
            }
        }
    }
}

void WombatDatabase::InsertEvidenceObject()
{
    IMG_AFF_INFO* affinfo = NULL;
    IMG_EWF_INFO* ewfinfo = NULL;
    uint8_t* ewfvalue = (uint8_t*)malloc(sizeof(uint8_t)*64);
    uint8_t uvalue8bit = 0;
    int8_t value8bit = 0;
    uint32_t value32bit = 0;
    uint64_t value64bit = 0;
    size64_t size64bit = 0;
    libewf_error_t* ewferror = NULL;
    QStringList evidpropertylist;
    wombatptr->currentevidenceid = 0;
    evidpropertylist.clear();
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
    evidpropertylist << QString("File Format") << QString(tsk_img_type_todesc((TSK_IMG_TYPE_ENUM)wombatptr->evidenceobject.imageinfo->itype)) << QString("File Format the evidence data is stored in. Usually it is either a raw image (.dd/.001) or an embedded image (.E01/.AFF). A raw image contains only the data from the evidence. The embedded image contains other descriptive information from the acquisition.");
    evidpropertylist << QString("Sector Size") << QString(QString::number(wombatptr->evidenceobject.imageinfo->sector_size) + " bytes") << QString("Sector size of the device. A Sector is a subdivision of a disk where data is stored. It is the smallest value used to divide the disk.");
    evidpropertylist << QString("Sector Count") << QString(QString::number((int)((float)wombatptr->evidenceobject.imageinfo->size/(float)wombatptr->evidenceobject.imageinfo->sector_size)) + " sectors") << QString("The number of sectors in the disk.");
        evidpropertylist << QString("Image Path") << QString::fromStdString(wombatptr->evidenceobject.fullpathvector[0]) << QString("Location where the evidence image is stored and read from.");
    if(TSK_IMG_TYPE_ISAFF(wombatptr->evidenceobject.imageinfo->itype)) // its AFF
    {
        affinfo = (IMG_AFF_INFO*)wombatptr->evidenceobject.imageinfo;
        evidpropertylist << "MD5" << QString::fromStdString(GetSegmentValue(affinfo, AF_MD5)) << "The MD5 hash algorithm of the uncompressed image file, stored as a 128-bit value";
        evidpropertylist << "Image GID" << QString::fromStdString(GetSegmentValue(affinfo, AF_IMAGE_GID)) << "A unique global identifier for the image";
        evidpropertylist << "Device Model" << QString::fromStdString(GetSegmentValue(affinfo, AF_DEVICE_MODEL)) << "Acquired Drive Model number";
        evidpropertylist << "Creator" << QString::fromStdString(GetSegmentValue(affinfo, AF_CREATOR)) << "Examiner who initiated the image acquisition";
        evidpropertylist << "Case Number" << QString::fromStdString(GetSegmentValue(affinfo, AF_CASE_NUM)) << "The case number that the image is associated with";
        evidpropertylist << "SHA1" << QString::fromStdString(GetSegmentValue(affinfo, AF_SHA1)) << "The SHA1 hash algorithm of the uncompressed image file, stored as a 160-bit value";
        evidpropertylist << "Acquisition Date" << QString::fromStdString(GetSegmentValue(affinfo, AF_ACQUISITION_DATE)) << "The date the acquisition was made";
        evidpropertylist << "Acquisition Notes" << QString::fromStdString(GetSegmentValue(affinfo, AF_ACQUISITION_NOTES)) << "Notes regading the acquisition";
        evidpropertylist << "Acquisition Device" << QString::fromStdString(GetSegmentValue(affinfo, AF_ACQUISITION_DEVICE)) << "The device used to acquire the information";
        evidpropertylist << "AFFLib Version" << QString::fromStdString(GetSegmentValue(affinfo, AF_AFFLIB_VERSION)) << "Verion of the AFFLib Library used";
        evidpropertylist << "Device Manufacturer" << QString::fromStdString(GetSegmentValue(affinfo, AF_DEVICE_MANUFACTURER)) << "Maker of the drive";
        evidpropertylist << "Device Serial Number" << QString::fromStdString(GetSegmentValue(affinfo, AF_DEVICE_SN)) << "Serial number of the drive";
    }
    else if(TSK_IMG_TYPE_ISEWF(wombatptr->evidenceobject.imageinfo->itype)) // its EWF
    {
        ewfinfo = (IMG_EWF_INFO*)wombatptr->evidenceobject.imageinfo;
        if(libewf_handle_get_utf8_header_value_case_number(ewfinfo->handle, ewfvalue, 64, &ewferror) == 1)
            evidpropertylist << "Case Number" << QString::fromUtf8(reinterpret_cast<char*>(ewfvalue)) << "The case number the image is associated";
        else
            libewf_error_fprint(ewferror, stdout);
        if(libewf_handle_get_utf8_header_value_description(ewfinfo->handle, ewfvalue, 64, &ewferror) == 1)
            evidpropertylist << "Description" << QString::fromUtf8(reinterpret_cast<char*>(ewfvalue)) << "Description of the acquisition and or evidence item";
        else
            libewf_error_fprint(ewferror, stdout);
        if(libewf_handle_get_utf8_header_value_examiner_name(ewfinfo->handle, ewfvalue, 64, &ewferror) == 1)
            evidpropertylist << "Examiner Name" << QString::fromUtf8(reinterpret_cast<char*>(ewfvalue)) << "Name of the examiner who acquired the image";
        else
            libewf_error_fprint(ewferror, stdout);
        if(libewf_handle_get_utf8_header_value_evidence_number(ewfinfo->handle, ewfvalue, 64, &ewferror) == 1)
            evidpropertylist << "Evidence Number" << QString::fromUtf8(reinterpret_cast<char*>(ewfvalue)) << "Unique number identifying the evidence item";
        else
            libewf_error_fprint(ewferror, stdout);
        if(libewf_handle_get_utf8_header_value_notes(ewfinfo->handle, ewfvalue, 255, &ewferror) == 1)
            evidpropertylist << "Notes" << QString::fromUtf8(reinterpret_cast<char*>(ewfvalue)) << "Any notes related to the acquisition";
        else
            libewf_error_fprint(ewferror, stdout);
        if(libewf_handle_get_utf8_header_value_acquiry_date(ewfinfo->handle, ewfvalue, 64, &ewferror) == 1)
            evidpropertylist << "Acquisition Date" << QString::fromUtf8(reinterpret_cast<char*>(ewfvalue)) << "Date the acquisition was made";
        else
            libewf_error_fprint(ewferror, stdout);
        if(libewf_handle_get_utf8_header_value_system_date(ewfinfo->handle, ewfvalue, 64, &ewferror) == 1)
            evidpropertylist << "System Date" << QString::fromUtf8(reinterpret_cast<char*>(ewfvalue)) << "Date for the system acquiring the image";
        else
            libewf_error_fprint(ewferror, stdout);
        if(libewf_handle_get_utf8_header_value_acquiry_operating_system(ewfinfo->handle, ewfvalue, 64, &ewferror) == 1)
            evidpropertylist << "Aquisition OS" << QString::fromUtf8(reinterpret_cast<char*>(ewfvalue)) << "Operating System acquiring the image";
        else
            libewf_error_fprint(ewferror, stdout);
        if(libewf_handle_get_utf8_header_value_acquiry_software_version(ewfinfo->handle, ewfvalue, 64, &ewferror) == 1)
            evidpropertylist << "Software Version Used" << QString::fromUtf8(reinterpret_cast<char*>(ewfvalue)) << "Version of the software used to acquire the image";
        else
            libewf_error_fprint(ewferror, stdout);
        if(libewf_handle_get_utf8_header_value_password(ewfinfo->handle, ewfvalue, 64, &ewferror) == 1)
            evidpropertylist << "Password" << QString::fromUtf8(reinterpret_cast<char*>(ewfvalue)) << "Password to protect the image";
        else
            libewf_error_fprint(ewferror, stdout);
        if(libewf_handle_get_utf8_header_value_model(ewfinfo->handle, ewfvalue, 64, &ewferror) == 1)
            evidpropertylist << "Model" << QString::fromUtf8(reinterpret_cast<char*>(ewfvalue)) << "Model of the drive acquired";
        else
            libewf_error_fprint(ewferror, stdout);
        if(libewf_handle_get_utf8_header_value_serial_number(ewfinfo->handle, ewfvalue, 64, &ewferror) == 1)
            evidpropertylist << "Serial Number" << QString::fromUtf8(reinterpret_cast<char*>(ewfvalue)) << "Serial number of the drive acquired";
        else
            libewf_error_fprint(ewferror, stdout);
        if(libewf_handle_get_sectors_per_chunk(ewfinfo->handle, &value32bit, &ewferror) == 1)
            evidpropertylist << QString("Sectors Per Chunk") << QString::number(value32bit) << "Number of sectors in a image evidence chunk";
        else
            libewf_error_fprint(ewferror, stdout);
        if(libewf_handle_get_format(ewfinfo->handle, &uvalue8bit, &ewferror) == 1)
        {
            evidpropertylist << QString("File Format");
            switch(uvalue8bit)
            {
                case LIBEWF_FORMAT_EWF:
                    evidpropertylist << QString("Original EWF") << "Format used to store the evidence image";
                    break;
                case LIBEWF_FORMAT_SMART:
                    evidpropertylist << QString("SMART") << "Format used to store the evidence image";
                    break;
                case LIBEWF_FORMAT_FTK:
                    evidpropertylist << QString("FTK Imager") << "Format used to store the evidence image";
                    break;
                case LIBEWF_FORMAT_ENCASE1:
                    evidpropertylist << QString("EnCase 1") << "Format used to store the evidence image";
                    break;
                case LIBEWF_FORMAT_ENCASE2:
                    evidpropertylist << QString("EnCase 2") << "Format used to store the evidence image";
                    break;
                case LIBEWF_FORMAT_ENCASE3:
                    evidpropertylist << QString("EnCase 3") << "Format used to store the evidence image";
                    break;
                case LIBEWF_FORMAT_ENCASE4:
                    evidpropertylist << QString("EnCase 4") << "Format used to store the evidence image";
                    break;
                case LIBEWF_FORMAT_ENCASE5:
                    evidpropertylist << QString("EnCase 5") << "Format used to store the evidence image";
                    break;
                case LIBEWF_FORMAT_ENCASE6:
                    evidpropertylist << QString("EnCase 6") << "Format used to store the evidence image";
                    break;
                case LIBEWF_FORMAT_LINEN5:
                    evidpropertylist << QString("Linen 5") << "Format used to store the evidence image";
                    break;
                case LIBEWF_FORMAT_LINEN6:
                    evidpropertylist << QString("Linen 6") << "Format used to store the evidence image";
                    break;
                case LIBEWF_FORMAT_EWFX:
                    evidpropertylist << QString("EWFX (extended ewf)") << QString("Extended EWF Format used to store the evidence image");
                    break;
                case LIBEWF_FORMAT_LOGICAL_ENCASE5:
                    evidpropertylist << QString("LEF EnCase 5") << QString("Logical Evidence File EnCase 5 Format used to store the evidence image");
                    break;
                case LIBEWF_FORMAT_LOGICAL_ENCASE6:
                    evidpropertylist << QString("LEF EnCase 6") << QString("Logical Evidence File EnCase 6 Format used to store the evidence image");
                    break;
                case LIBEWF_FORMAT_UNKNOWN:
                    evidpropertylist << QString("Unknown Format") << "Format used to store the evidence image";
                    break;
            }
        }
        else
            libewf_error_fprint(ewferror, stdout);
        if(libewf_handle_get_error_granularity(ewfinfo->handle, &value32bit, &ewferror) == 1)
            evidpropertylist << QString("Error Granularity") << QString::number(value32bit) << "Error block size";
        else
            libewf_error_fprint(ewferror, stdout);
        evidpropertylist << "Compression Method" << "Deflate" << "Method used to compress the image";
        if(libewf_handle_get_compression_values(ewfinfo->handle, &value8bit, &uvalue8bit, &ewferror) == 1)
        {
            evidpropertylist << "Compression Level";
            if(value8bit == LIBEWF_COMPRESSION_NONE)
                evidpropertylist << "No Compression";
            else if(value8bit == LIBEWF_COMPRESSION_FAST)
                evidpropertylist << "Good (Fast) Compression";
            else if(value8bit == LIBEWF_COMPRESSION_BEST)
                evidpropertylist << "Best Compression";
            else
                evidpropertylist << "Unknown Compression";
            evidpropertylist << "The more compression, the slower the acquisition but the smaller the file size";
        }
        else
            libewf_error_fprint(ewferror, stdout);
        if(libewf_handle_get_media_type(ewfinfo->handle, &uvalue8bit, &ewferror) == 1)
        {
            evidpropertylist << "Media Type";
            if(uvalue8bit == LIBEWF_MEDIA_TYPE_REMOVABLE)
                evidpropertylist << "Removable Disk";
            else if(uvalue8bit == LIBEWF_MEDIA_TYPE_FIXED)
                evidpropertylist << "Fixed Disk";
            else if(uvalue8bit == LIBEWF_MEDIA_TYPE_SINGLE_FILES)
                evidpropertylist << "Single Files";
            else if(uvalue8bit == LIBEWF_MEDIA_TYPE_OPTICAL)
                evidpropertylist << "Optical Disk (CD/DVD/BD)";
            else if(uvalue8bit == LIBEWF_MEDIA_TYPE_MEMORY)
                evidpropertylist << "Memory (RAM)";
            else
                evidpropertylist << "Unknown";
            evidpropertylist << "Media type of the original evidence";
        }
        else
            libewf_error_fprint(ewferror, stdout);
        if(libewf_handle_get_media_flags(ewfinfo->handle, &uvalue8bit, &ewferror) == 1)
        {
            if(uvalue8bit == LIBEWF_MEDIA_FLAG_PHYSICAL)
                evidpropertylist << "Media Flag" << "Physical" << "Directly connected disk";
            if(uvalue8bit == LIBEWF_MEDIA_FLAG_FASTBLOC)
                evidpropertylist << "Media Flag" << "Fastbloc Write Blocked" << "Write blocked disk using Fastbloc";
            if(uvalue8bit == LIBEWF_MEDIA_FLAG_TABLEAU)
                evidpropertylist << "Media Flag" << "Tableau Write Blocked" << "Write blocked disk using Tableau";
        }
        else
            libewf_error_fprint(ewferror, stdout);
        if(libewf_handle_get_bytes_per_sector(ewfinfo->handle, &value32bit, &ewferror) == 1)
            evidpropertylist << "Bytes Per Sector" << QString::number(value32bit) << "Number of bytes in a sector";
        else
            libewf_error_fprint(ewferror, stdout);
        if(libewf_handle_get_number_of_sectors(ewfinfo->handle, &value64bit, &ewferror) == 1)
            evidpropertylist << "Number of Sectors" << QString::number(value64bit) << "Number of total sectors in the original media";
        else
            libewf_error_fprint(ewferror, stdout);
        if(libewf_handle_get_chunk_size(ewfinfo->handle, &value32bit, &ewferror) == 1)
            evidpropertylist << "Chunk Size" << QString::number(value32bit) << "The size of an image chunk";
        else
            libewf_error_fprint(ewferror, stdout);
        if(libewf_handle_get_media_size(ewfinfo->handle, &size64bit, &ewferror) == 1)
            evidpropertylist << "Media Size" << QString::number(size64bit) << "The size of the media";
        else
            libewf_error_fprint(ewferror, stdout);
        if(libewf_handle_get_utf8_hash_value_md5(ewfinfo->handle, ewfvalue, 64, &ewferror) == 1)
            evidpropertylist << "MD5" << QString::fromUtf8(reinterpret_cast<char*>(ewfvalue)) << "The MD5 hash algorithm of the uncompressed image stored as a 128-bit value";
        else
            libewf_error_fprint(ewferror, stdout);
        if(libewf_handle_get_utf8_hash_value_sha1(ewfinfo->handle, ewfvalue, 64, &ewferror) == 1)
            evidpropertylist << "SHA1" << QString::fromUtf8(reinterpret_cast<char*>(ewfvalue)) << "The SHA1 hash algorithm of the uncompressed image stored as a 160-bit value";
        else
            libewf_error_fprint(ewferror, stdout);
        free(ewfvalue);
    }
    else if(TSK_IMG_TYPE_ISRAW(wombatptr->evidenceobject.imageinfo->itype)) // is raw
    {
        // nothing i want to add for raw right now.
    }
    else // not supported...
    {
        // log error about unsupported image type.
    }
    for(int i=0; i < evidpropertylist.count()/3; i++)
    {
        wombatptr->bindvalues.clear();
        wombatptr->bindvalues.append(wombatptr->currentevidenceid);
        wombatptr->bindvalues.append(evidpropertylist.at(3*i));
        wombatptr->bindvalues.append(evidpropertylist.at(3*i+1));
        wombatptr->bindvalues.append(evidpropertylist.at(3*i+2));
        InsertSql("INSERT INTO properties (objectid, name, value, description) VALUES(?, ?, ?, ?);", wombatptr->bindvalues);
    }
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
    wombatptr->sqlrecords = GetSqlResults("SELECT objecttype, size, parimgid, sectstart, sectlength, sectsize, blockcount, byteoffset, address, type, flags, blocksize, parfsid, fullpath FROM data WHERE objectid = ?", wombatptr->bindvalues);
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
    wombatptr->selectedobject.fullpath = wombatptr->sqlrecords[0].value(13).toString();
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
    wombatptr->bindvalues.clear();
    wombatptr->bindvalues.append(wombatptr->selectedobject.id);
    wombatptr->sqlrecords.clear();
    wombatptr->sqlrecords = GetSqlResults("SELECT name, value, description FROM properties WHERE objectid = ?", wombatptr->bindvalues);
    for(int i=0; i < wombatptr->sqlrecords.count(); i++)
    {
        propertylist << wombatptr->sqlrecords.at(i).value(0).toString() << wombatptr->sqlrecords.at(i).value(1).toString() << wombatptr->sqlrecords.at(i).value(2).toString();
    }
    // PROBABLY DON'T NEED TO DIFFERENTIATE, SINCE I'M PULLING THE DATA FROM THE DB, WHICH DOES THE DIFFERENTIATING BY OBJECTID
    /*
    if(wombatptr->selectedobject.objtype == 1 || wombatptr->selectedobject.) // image file
    {
        wombatptr->bindvalues.clear();
        wombatptr->bindvalues.append(wombatptr->selectedobject.id);
        wombatptr->sqlrecords.clear();
        wombatptr->sqlrecords = GetSqlResults("SELECT name, value, description FROM properties WHERE objectid = ?", wombatptr->bindvalues);
        for(int i=0; i < wombatptr->sqlrecords.count(); i++)
        {
            propertylist << wombatptr->sqlrecords.at(i).value(0).toString() << wombatptr->sqlrecords.at(i).value(1).toString() << wombatptr->sqlrecords.at(i).value(2).toString();
        }
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
    }*/
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

void WombatDatabase::InsertFileSystemProperties(int curfsid, TSK_FS_INFO* curfsinfo)
{
    if(curfsinfo == NULL)
    {
        // log error here
    }
    TSK_FS_FILE* tmpfile = NULL;
    ntfs_sb* ntfssb = NULL;
    FFS_INFO* ffs = NULL;
    ffs_sb1* sb1 = NULL;
    ffs_sb2* sb2 = NULL;
    FATXXFS_DENTRY* tmpfatdentry = NULL;
    FATXXFS_DENTRY* curentry = NULL;
    FATFS_INFO* fatfs = NULL;
    FATXXFS_SB* fatsb = NULL;
    const TSK_FS_ATTR*tmpattr;
    TSK_DADDR_T cursector = 0;
    TSK_DADDR_T endsector = 0;
    int8_t isallocsec = 0;
    TSK_INUM_T curinum = 0;
    FATFS_DENTRY* dentry = NULL;
    EXT2FS_INFO* ext2fs = NULL;
    //ext2fs_sb* sb = NULL;
    ISO_INFO* iso = NULL;
    iso9660_pvd_node* p = NULL;
    iso9660_svd_node* s = NULL;
    HFS_INFO* hfs = NULL;
    hfs_plus_vh* hsb = NULL;
    char fn[HFS_MAXNAMLEN + 1];
    HFS_ENTRY* hfsentry = NULL;
    hfs_btree_key_cat key;
    hfs_thread thread;
    hfs_file_folder record;
    TSK_OFF_T off;
    char* databuffer = NULL;
    ssize_t cnt;
    ssize_t bytesread = 0;
    int a;
    uint len = 0;
    char asc[512];
    QStringList fsproplist;
    fsproplist.clear();
    if(curfsinfo->ftype == TSK_FS_TYPE_EXT2 || curfsinfo->ftype == TSK_FS_TYPE_EXT3 || curfsinfo->ftype == TSK_FS_TYPE_EXT4)
    {
        ext2fs = (EXT2FS_INFO*)curfsinfo;
        fsproplist << "File System Type";
        if(curfsinfo->ftype == TSK_FS_TYPE_EXT2)
            fsproplist << "ext2";
        else if(curfsinfo->ftype == TSK_FS_TYPE_EXT3)
            fsproplist << "ext3";
        else if(curfsinfo->ftype == TSK_FS_TYPE_EXT4)
            fsproplist << "ext4";
        fsproplist << "";
        fsproplist << "File System Label" << QString::fromStdString(string(ext2fs->fs->s_volume_name)) << ""; 
        fsproplist << "Inode Count" << QString::number(tsk_getu32(curfsinfo->endian, ext2fs->fs->s_inodes_count)) << "Number of Inodes in the file system. Found in the superblock at bytes 0 - 3";
        fsproplist << "Block Count" << QString::number(tsk_getu32(curfsinfo->endian, ext2fs->fs->s_inodes_count)) << "Number of Blocks in the file system. Found in the superblock at bytes 4-7";
        fsproplist << "Reserved Blocks" << QString::number(tsk_getu32(curfsinfo->endian, ext2fs->fs->s_r_blocks_count)) << "Number of blocks reserved to prevent the file system from filling up. Found in the superblock at bytes 8-11";
        fsproplist << "Unallocated Blocks" << QString::number(tsk_getu32(curfsinfo->endian, ext2fs->fs->s_free_blocks_count)) << "Number of unallocated blocks. Found in the superblock at bytes 12-15";
        fsproplist << "Unallocated Inodes" << QString::number(tsk_getu32(curfsinfo->endian, ext2fs->fs->s_free_inode_count)) << "Number of unnalocated inodes";
        //fsproplist << "Volume Name" << QString(sb->s_volume_name) << "
        //sb = ext2fs->fs;
        //qDebug() << "EXT2 Volume name: " << sb->s_volume_name;
 
    }
    /*
    switch(curfsinfo->ftype)
    {
        case TSK_FS_TYPE_NTFS:
            len = roundup(sizeof(ntfs_sb), curfsinfo->img_info->sector_size);
            ntfssb = tsk_malloc(len);
            cnt = tsk_fs_read(curfsinfo, (TSK_OFF_T) 0, (char*) ntfssb, len);
            // will come back to NTFS.
            break;
        case TSK_FS_TYPE_EXFAT:


    }*/
    /*
     *
            case TSK_FS_TYPE_NTFS:
                //NTFS_INFO* tmpinfo = (NTFS_INFO*)tmpfsinfo;
                if((tmpfile = tsk_fs_file_open_meta(tmpfsinfo, NULL, NTFS_MFT_VOL)) == NULL)
                {
                    // log error here...
                }
                tmpattr = tsk_fs_attrlist_get(tmpfile->meta->attr, TSK_FS_ATTR_TYPE_NTFS_VNAME);
                //tmpattr = tsk_fs_attrlist_get(tmpfile->meta->attr, NTFS_ATYPE_VNAME);
                if(!tmpattr)
                {
                    // log error here...
                }
                if((tmpattr->flags & TSK_FS_ATTR_RES) && (tmpattr->size))
                {
                    UTF16* name16 = (UTF16*) tmpattr->rd.buf;
                    UTF8* name8 = (UTF8*) asc;
                    int retval;
                    retval = tsk_UTF16toUTF8(tmpfsinfo->endian, (const UTF16**)&name16, (UTF16*) ((uintptr_t) name16 + (int) tmpattr->size), &name8, (UTF8*) ((uintptr_t)name8 + sizeof(asc)), TSKlenientConversion);
                    if(retval != TSKconversionOK)
                    {
                        // log error here                                
                        *name8 = '\0';
                    }
                    else if((uintptr_t) name8 >= (uintptr_t) asc + sizeof(asc))
                        asc[sizeof(asc) - 1] = '\0';
                    else
                        *name8 = '\0';
                }
                qDebug() << "NTFS Volume Name:" << asc;
                tsk_fs_file_close(tmpfile);
                break;
            case TSK_FS_TYPE_EXFAT:
                fatfs = (FATFS_INFO*)tmpfsinfo;
                if((tmpfile = tsk_fs_file_alloc(tmpfsinfo)) == NULL)
                {
                    // log error here
                }
                if((tmpfile->meta = tsk_fs_meta_alloc(FATFS_FILE_CONTENT_LEN)) == NULL)
                {
                    // log error here
                }
                if((databuffer = (char*)tsk_malloc(fatfs->ssize)) == NULL)
                {
                    // log error here
                }
                cursector = fatfs->rootsect;
                endsector = fatfs->firstdatasect + (fatfs->clustcnt * fatfs->csize) - 1;
                while(cursector < endsector)
                {

                }
                bytesread = tsk_fs_read_block(tmpfsinfo, cursector, databuffer, fatfs->ssize);
                if(bytesread != fatfs->ssize)
                {
                    // log error here
                }
                isallocsec = fatfs_is_sectalloc(fatfs, cursector);
                if(isallocsec == -1)
                {
                    // log error here
                }
                curinum = FATFS_SECT_2_INODE(fatfs, cursector);
                for(i = 0; i < fatfs->ssize; i+= sizeof(FATFS_DENTRY))
                {
                    dentry = (FATFS_DENTRY*)&(databuffer[i]);
                    if(exfatfs_get_enum_from_type(dentry->data[0]) == EXFATFS_DIR_ENTRY_TYPE_VOLUME_LABEL)
                    {
                        if(exfatfs_dinode_copy(fatfs, curinum, dentry, isallocsec, tmpfile) == TSK_OK)
                        {
                            qDebug() << "EXFAT Volume Name: " << tmpfile->meta->name2->name;
                            break;
                        }
                        else
                        {
                            // log error here
                        }
                    }
                }
                tsk_fs_file_close(tmpfile);
                free(databuffer);
                break;
            case TSK_FS_TYPE_FAT12:
                fatfs = (FATFS_INFO*)tmpfsinfo;
                fatsb = (FATXXFS_SB*)fatfs->boot_sector_buffer;
                qDebug() << fatsb->a.f16.vol_lab;
                printf("Volume Label (Boot Sector): %c%c%c%c%c%c%c%c%c%c%c\n", fatsb->a.f16.vol_lab[0], fatsb->a.f16.vol_lab[1], fatsb->a.f16.vol_lab[2], fatsb->a.f16.vol_lab[3], fatsb->a.f16.vol_lab[4], fatsb->a.f16.vol_lab[5], fatsb->a.f16.vol_lab[6], fatsb->a.f16.vol_lab[7], fatsb->a.f16.vol_lab[8], fatsb->a.f16.vol_lab[9], fatsb->a.f16.vol_lab[10]);
                if((databuffer = (char*) tsk_malloc(tmpfsinfo->block_size)) == NULL)
                {
                    // log error here
                }
                cnt = tsk_fs_read_block(tmpfsinfo, fatfs->rootsect, databuffer, tmpfsinfo->block_size);
                if(cnt != tmpfsinfo->block_size)
                {
                    // log error here
                }
                tmpfatdentry = NULL;
                if(fatfs->ssize <= tmpfsinfo->block_size)
                {
                    curentry = (FATXXFS_DENTRY*)databuffer;
                    for(int i=0; i < fatfs->ssize; i += sizeof(*curentry))
                    {
                        if(curentry->attrib == FATFS_ATTR_VOLUME)
                        {
                            tmpfatdentry = curentry;
                            break;
                        }
                        curentry++;
                    }
                }
                qDebug() << "FAT12 Volume Label: " << tmpfatdentry->name;
                free(databuffer);
                break;
            case TSK_FS_TYPE_FAT16:
                fatfs = (FATFS_INFO*)tmpfsinfo;
                fatsb = (FATXXFS_SB*)fatfs->boot_sector_buffer;
                qDebug() << fatsb->a.f16.vol_lab;
                printf("Volume Label (Boot Sector): %c%c%c%c%c%c%c%c%c%c%c\n", fatsb->a.f16.vol_lab[0], fatsb->a.f16.vol_lab[1], fatsb->a.f16.vol_lab[2], fatsb->a.f16.vol_lab[3], fatsb->a.f16.vol_lab[4], fatsb->a.f16.vol_lab[5], fatsb->a.f16.vol_lab[6], fatsb->a.f16.vol_lab[7], fatsb->a.f16.vol_lab[8], fatsb->a.f16.vol_lab[9], fatsb->a.f16.vol_lab[10]);
                if((databuffer = (char*) tsk_malloc(tmpfsinfo->block_size)) == NULL)
                {
                    // log error here
                }
                cnt = tsk_fs_read_block(tmpfsinfo, fatfs->rootsect, databuffer, tmpfsinfo->block_size);
                if(cnt != tmpfsinfo->block_size)
                {
                    // log error here
                }
                tmpfatdentry = NULL;
                if(fatfs->ssize <= tmpfsinfo->block_size)
                {
                    curentry = (FATXXFS_DENTRY*)databuffer;
                    for(int i=0; i < fatfs->ssize; i += sizeof(*curentry))
                    {
                        if(curentry->attrib == FATFS_ATTR_VOLUME)
                        {
                            tmpfatdentry = curentry;
                            break;
                        }
                        curentry++;
                    }
                }
                qDebug() << "FAT16 Volume Label: " << tmpfatdentry->name;
                free(databuffer);
                break;
            case TSK_FS_TYPE_FAT32:
                fatfs = (FATFS_INFO*)tmpfsinfo;
                fatsb = (FATXXFS_SB*)fatfs->boot_sector_buffer;
                qDebug() << fatsb->a.f32.vol_lab;
                printf("Volume Label (Boot Sector): %c%c%c%c%c%c%c%c%c%c%c\n", fatsb->a.f32.vol_lab[0], fatsb->a.f32.vol_lab[1], fatsb->a.f32.vol_lab[2], fatsb->a.f32.vol_lab[3], fatsb->a.f32.vol_lab[4], fatsb->a.f32.vol_lab[5], fatsb->a.f32.vol_lab[6], fatsb->a.f32.vol_lab[7], fatsb->a.f32.vol_lab[8], fatsb->a.f32.vol_lab[9], fatsb->a.f32.vol_lab[10]);
                if((databuffer = (char*) tsk_malloc(tmpfsinfo->block_size)) == NULL)
                {
                    // log error here
                }
                cnt = tsk_fs_read_block(tmpfsinfo, fatfs->rootsect, databuffer, tmpfsinfo->block_size);
                if(cnt != tmpfsinfo->block_size)
                {
                    // log error here
                }
                tmpfatdentry = NULL;
                if(fatfs->ssize <= tmpfsinfo->block_size)
                {
                    curentry = (FATXXFS_DENTRY*)databuffer;
                    for(int i=0; i < fatfs->ssize; i += sizeof(*curentry))
                    {
                        if(curentry->attrib == FATFS_ATTR_VOLUME)
                        {
                            tmpfatdentry = curentry;
                            break;
                        }
                        curentry++;
                    }
                }
                qDebug() << "FAT32 Volume Label: " << tmpfatdentry->name;
                free(databuffer);
                break;
            case TSK_FS_TYPE_FFS1:
                qDebug() << "FFS1";
                break;
            case TSK_FS_TYPE_FFS1B:
                qDebug() << "FFS1B";
                break;
            case TSK_FS_TYPE_FFS2:
                ffs = (FFS_INFO*)tmpfsinfo;
                sb1 = ffs->fs.sb1;
                sb2 = ffs->fs.sb2;
                qDebug() << "FFS2 Volume label: " << sb2->volname;
                break;
            case TSK_FS_TYPE_EXT2:
                ext2fs = (EXT2FS_INFO*)tmpfsinfo;
                sb = ext2fs->fs;
                qDebug() << "EXT2 Volume name: " << sb->s_volume_name;
                break;
            case TSK_FS_TYPE_EXT3:
                ext2fs = (EXT2FS_INFO*)tmpfsinfo;
                sb = ext2fs->fs;
                qDebug() << "EXT3 Volume name: " << sb->s_volume_name;
                break;
            case TSK_FS_TYPE_EXT4:
                ext2fs = (EXT2FS_INFO*)tmpfsinfo;
                sb = ext2fs->fs;
                qDebug() << "EXT4 Volume name: " << sb->s_volume_name;
                break;
            case TSK_FS_TYPE_RAW:
                qDebug() << "no file system. store 0, \"\", or message for respective variables";
                break;
            case TSK_FS_TYPE_ISO9660:
                a = 0;
                iso = (ISO_INFO*)tmpfsinfo;
                p = iso->pvd;
                for(p = iso->pvd; p!= NULL; p = p->next)
                {
                    a++;
                    qDebug() << "ISO9660 vol name: " << p->pvd.vol_id;
                }
                a = 0;
                for(s = iso->svd; s!= NULL; s = s->next)
                {
                    a++;
                    qDebug() << "ISO9660 vol name: " << s->svd.vol_id;
                }
                qDebug() << "ISO9660";
                break;
            case TSK_FS_TYPE_HFS:
                hfs = (HFS_INFO*)tmpfsinfo;
                hsb = hfs->fs;
                memset((char*)&key, 0, sizeof(hfs_btree_key_cat));
                cnid_to_array((uint32_t)HFS_ROOT_INUM, key.parent_cnid);
                /*
                off = hfs_cat_get_record_offset(hfs, &key);
                if(off == 0)
                {
                    // log error here
                }
                if(hfs_cat_read_thread_record(hfs, off, &thread))
                {
                    // log error here
                }

                memset((char*)&key, 0, sizeof(hfs_btree_key_cat));
                memcpy((char*)key.parent_cnid, (char*)thread.parent_cnid, sizeof(key.parent_cnid));
                memcpy((char*)&key.name, (char*)&thread.name, sizeof(key.name));
                off = hfs_cat_get_record_offset(hfs, &key);
                if(off = 0)
                {
                    // log error here
                }
                if(hfs_cat_read_file_folder_record(hfs, off, &record))
                {
                    // log error here
                }
                //if(tsk_getU16(tmpfsinfo->endian, record.file.std.rec_type) == HFS_FOLDER_RECORD)
                memcpy((char*)&entry->thread, (char*)&thread, sizeof(hfs_thread));
                entry->flags = TSK_FS_META_FLAG_ALLOC | TSK_FS_META_FLAG_USED;
                entry->inum = HFS_ROOT_INUM;
                if(follow_hard_link)
                {
                    unsigned char is_err;
                    TSK_INUM_T target_cnid = hfs_follow_hard_link(hfs, &(entry->cat), &is_err);
                    if(is_err > 1)
                    {
                        // log error here
                    }*/
                    /*
                    if(target_cnid != HFS_ROOT_INUM)
                    {
                        uint8_t res = hfs_cat_file_lookup(hfs, target_cnid, entry, FALSE);
                        if(res != 0)
                        {
                            // log error here
                        }
                    }*/
                //}
                // NEED TO EXPAND THE HFS_CAT_FILE_LOOKUP() FUNCTION AND THE 
                /*
                if(hfs_cat_file_lookup(hfs, HFS_ROOT_INUM, &hfsentry, FALSE))
                {
                    // log error here
                }
                if(hfs_UTF16toUTF8(tmpfsinfo, hfsentry.thread.name.unicode, tsk_getu16(tmpfsinfo->endian, hfsentry.thread.name.length), fn, HFS_MAXNAMLEN + 1, HFS_U16U8_FLAG_REPLACE_SLASH))
                {
                    // log error here.
                }*/
                /* continuance
                qDebug() << "HFS Volume Name: " << fn;
                free(fn);
                break;
            case TSK_FS_TYPE_YAFFS2:
                qDebug() << "YAFFS2 no volume name, might want other properties though";
                break;
            case TSK_FS_TYPE_SWAP:
                qDebug() << "no file system. store 0, \"\", or message for respective variables";
                break;
           default:
                qDebug() << "what to do for default???";
                break;
        }
    }

     *
     */ 
    for(int i=0; i < fsproplist.count()/3; i++)
    {
        wombatptr->bindvalues.clear();
        wombatptr->bindvalues.append(curfsid);
        wombatptr->bindvalues.append(fsproplist.at(3*i));
        wombatptr->bindvalues.append(fsproplist.at(3*i+1));
        wombatptr->bindvalues.append(fsproplist.at(3*i+2));
        InsertSql("INSERT INTO properties (objectid, name, value, description) VALUES(?, ?, ?, ?);", wombatptr->bindvalues);
    }
}
