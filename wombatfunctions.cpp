#include "wombatfunctions.h"

std::string GetTime()
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

void LogEntry(int caseid, int evidenceid, int jobid, int type, QString msg)
{
    QSqlQuery logquery(logdb);
    logquery.prepare("INSERT INTO msglog (caseid, evidenceid, jobid, type, datetime, logmsg) VALUES(?, ?, ?, ?, DATETIME('now', 'unixepoch'), ?)");
    logquery.addBindValue(caseid);
    logquery.addBindValue(evidenceid);
    logquery.addBindValue(jobid);
    logquery.addBindValue(type);
    logquery.addBindValue(msg);
    logquery.exec();
}

void StartJob(int type, int caseid, int evidenceid)
{
    QSqlQuery jobquery(logdb);
    jobquery.prepare("INSERT INTO joblog (type, state, filecount, processcount, caseid, evidenceid, start, finish, errorcount) VALUES(?, 0, 0, 0, ?, ?, DATETIME('now', 'unixepoch'), 0, 0)");
    jobquery.addBindValue(type);
    jobquery.addBindValue(caseid);
    jobquery.addBindValue(evidenceid);
    if(jobquery.exec())
        currentjobid = jobquery.lastInsertId().toInt();
}

void EndJob(int jobid, int filecount, int processcount, int errorcount)
{
    QSqlQuery jobquery(logdb);
    jobquery.prepare("UPDATE joblog SET state = 1, filecount = ?, processcount = ?, finish = DATETIME('now', 'unixepoch'), errorcount = ? WHERE jobid = ?");
    jobquery.addBindValue(filecount);
    jobquery.addBindValue(processcount);
    jobquery.addBindValue(errorcount);
    jobquery.addBindValue(jobid);
    jobquery.exec();
}

char* TskTimeToStringUTC(time_t time, char buf[128])
{
    buf[0] = '\0';
    if (time <= 0) {
        strncpy(buf, "", 128);
    }
    else {
        //struct tm *tmTime = localtime(&time);
        struct tm *tmTime = gmtime(&time);

        snprintf(buf, 128, "%.4d-%.2d-%.2d %.2d:%.2d:%.2d", (int) tmTime->tm_year + 1900, (int) tmTime->tm_mon + 1, (int) tmTime->tm_mday, tmTime->tm_hour, (int) tmTime->tm_min, (int) tmTime->tm_sec);//, tzname[(tmTime->tm_isdst == 0) ? 0 : 1]);
    }
    return buf;
}

int GetChildCount(int type, int address, int parimgid)
{
    QSqlQuery childquery(fcasedb);
    QString querystring = "SELECT COUNT(objectid) FROM data WHERE parentid = ?";
    if(type < 4)
        querystring += " AND objecttype < 5";
    else
        querystring += " AND objecttype = 5";
    if(type != 1)
        querystring += " AND parimgid = ?";
    childquery.prepare(querystring);
    childquery.addBindValue(address);
    if(type != 1)
        childquery.addBindValue(parimgid);
    if(childquery.exec())
    {
        childquery.next();
        return childquery.value(0).toInt();
    }
    return 0;
}

bool FileExists(const std::string& filename)
{
    struct stat buf;
    if (stat(filename.c_str(), &buf) != -1)
    {
        return true;
    }
    return false;
}

bool ProcessingComplete()
{
    bool processingcomplete = false;
    if(threadvector.count() == 0)
        return true;
    for(int i = 0; i < threadvector.count(); i++)
    {
        processingcomplete = threadvector[i].isFinished();
    }
    
    return processingcomplete;
}

void ProcessFile(QVector<QString> tmpstrings, QVector<int> tmpints, QStringList tmplist)
{

    if(fcasedb.isValid() && fcasedb.isOpen())
    {
        QSqlQuery fquery(fcasedb);
        fquery.prepare("INSERT INTO data(objecttype, type, name, parentid, fullpath, atime, ctime, crtime, mtime, size, address, md5, parimgid, parfsid, blockaddress, filesignature) VALUES(5, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?,?,?);");
        fquery.addBindValue(tmpints[0]);
        fquery.addBindValue(tmpstrings[0]);
        fquery.addBindValue(tmpints[1]);
        fquery.addBindValue(tmpstrings[1]);
        fquery.addBindValue(tmpints[2]);
        fquery.addBindValue(tmpints[3]);
        fquery.addBindValue(tmpints[4]);
        fquery.addBindValue(tmpints[5]);
        fquery.addBindValue(tmpints[6]);
        fquery.addBindValue(tmpints[7]);
        fquery.addBindValue(tmpstrings[2]);
        fquery.addBindValue(currentevidenceid);
        fquery.addBindValue(tmpints[8]);
        fquery.addBindValue(tmpstrings[3]);
        fquery.addBindValue(tmpstrings[4]);

        //qDebug() << tmpstrings[0] << tmpstrings[3];
        
        fquery.exec();
        int tmpid = fquery.lastInsertId().toInt();
        fquery.finish();
        for(int i=0; i < tmplist.count()/3; i++)
        {
            fquery.prepare("INSERT INTO properties (objectid, name, value, description) VALUES(?, ?, ?, ?);");
            fquery.addBindValue(tmpid);
            fquery.addBindValue(tmplist.at(3*i));
            fquery.addBindValue(tmplist.at(3*i+1));
            fquery.addBindValue(tmplist.at(3*i+2));
            fquery.exec();
            fquery.finish();
        }
        filesprocessed++;
        isignals->ProgUpd();
    }
    else
    {
        LogEntry(0, currentevidenceid, currentjobid, 0, QString("Error while processing " + tmpstrings[1] + " " + fcasedb.lastError().text()));
        errorcount++;
    }
}

TSK_WALK_RET_ENUM GetBlockAddress(TSK_FS_FILE* tmpfile, TSK_OFF_T off, TSK_DADDR_T addr, char* buf, size_t size, TSK_FS_BLOCK_FLAG_ENUM flags, void *ptr)
{
    if(off < 0)
    {
        // remove compile warning
    }
    if(buf)
    {
        // remove compile warning
    }
    if(ptr)
    {
        // remove compile warning
    }
    // WILL HAVE TO CREATE A SWITCH TO ACCOUNT FOR THE DIFFERENT FILE SYSTEMS
    if(tmpfile->fs_info->ftype == TSK_FS_TYPE_HFS_DETECT)
    {
        blockstring += QString::number(addr) + "|";
        // NEED TO FIGURE OUT HOW TO GET EACH BLOCK SO I CAN STORE THE RESPECTIVE VALUES
        //qDebug() << "File Name:" << tmpfile->name->name << "Block Address:" << addr;
    }
    else if(tmpfile->fs_info->ftype == TSK_FS_TYPE_ISO9660_DETECT)
    {
        // iso is already done in the previous function
    }
    else if(tmpfile->fs_info->ftype == TSK_FS_TYPE_FAT_DETECT || tmpfile->fs_info->ftype == TSK_FS_TYPE_NTFS_DETECT)
    {
        blockstring += QString::number(addr) + "|";
        //qDebug() << "File Name:" << tmpfile->name->name << "Address:" << addr;
    }
    else if(tmpfile->fs_info->ftype == TSK_FS_TYPE_YAFFS2_DETECT)
    {
        if(flags & TSK_FS_BLOCK_FLAG_CONT)
        {
            blockstring += QString::number(addr) + "|";
            //qDebug() << "File Name:" << tmpfile->name->name << "Address:" << addr;
        }
    }
    else
    {
        if(flags & TSK_FS_BLOCK_FLAG_CONT)
        {
            int i, s;
            for(i = 0, s = (int) size; s > 0; s -= tmpfile->fs_info->block_size, i++)
            {
                if(addr)
                {
                    blockstring += QString::number(addr + i) + "|";
                    //qDebug() << "File Name:" << tmpfile->name->name << "Address:" << addr + i;
                }
            }
        }
    }
    return TSK_WALK_CONT;
}

QString GetFilePermissions(TSK_FS_META* tmpmeta)
{
    QString tmpstring = "----------";
    tmpstring.replace(0, 1, tsk_fs_meta_type_str[tmpmeta->type][0]);
    if(tmpmeta->mode & TSK_FS_META_MODE_IRUSR)
        tmpstring.replace(1, 1, "r");
    if(tmpmeta->mode & TSK_FS_META_MODE_IWUSR)
        tmpstring.replace(2, 1, "w");
    if(tmpmeta->mode & TSK_FS_META_MODE_ISUID)
    {
        if(tmpmeta->mode & TSK_FS_META_MODE_IXUSR)
            tmpstring.replace(3, 1, "s");
        else
            tmpstring.replace(3, 1, "S");
    }
    else if(tmpmeta->mode & TSK_FS_META_MODE_IXUSR)
        tmpstring.replace(3, 1, "x");
    if(tmpmeta->mode & TSK_FS_META_MODE_IRGRP)
        tmpstring.replace(4, 1, "r");
    if(tmpmeta->mode & TSK_FS_META_MODE_IWGRP)
        tmpstring.replace(5, 1, "w");
    if(tmpmeta->mode && TSK_FS_META_MODE_ISGID)
    {
        if(tmpmeta->mode & TSK_FS_META_MODE_IXGRP)
            tmpstring.replace(6, 1, "s");
        else
            tmpstring.replace(6, 1, "S");
    }
    else if(tmpmeta->mode & TSK_FS_META_MODE_IXGRP)
        tmpstring.replace(6, 1, "x");
    if(tmpmeta->mode & TSK_FS_META_MODE_IROTH)
        tmpstring.replace(7, 1, "r");
    if(tmpmeta->mode & TSK_FS_META_MODE_IWOTH)
        tmpstring.replace(8, 1, "w");
    if(tmpmeta->mode & TSK_FS_META_MODE_ISVTX) // sticky bit
    {
        if(tmpmeta->mode & TSK_FS_META_MODE_IXOTH)
            tmpstring.replace(9, 1, "t");
        else
            tmpstring.replace(9, 1, "T");
    }
    else if(tmpmeta->mode & TSK_FS_META_MODE_IXOTH)
        tmpstring.replace(9, 1, "x");
    return tmpstring;
}

TSK_WALK_RET_ENUM FileEntries(TSK_FS_FILE* tmpfile, const char* tmppath, void* tmpptr)
{
    // NEED TO WORK ON THE DESCRIPTIONS FOR THE FILE PROPERTIES A LITTLE
    QStringList proplist;
    proplist.clear();
    proplist << "Short Name" << tmpfile->name->shrt_name << "Short name for a file";
    proplist << "File Permissions" << GetFilePermissions(tmpfile->meta) << "Unix Style Permissions. r - file, d - directory, l - symbolic link, c - character device, b - block device, p - named pipe, v - virtual file created by the forensic tool; r - read, w - write, x - execute, s - set id and executable, S - set id, t - sticky bit executable, T - sticky bit. format is type|user|group|other - [rdlcbpv]|rw[sSx]|rw[sSx]|rw[tTx]";
    proplist << "User ID" << QString::number(tmpfile->meta->uid) << "User ID";
    proplist << "Group ID" << QString::number(tmpfile->meta->gid) << "Group ID";
    proplist << "Allocation Status";
    if(tmpfile->meta->flags == TSK_FS_META_FLAG_ALLOC)
        proplist << "Currently Allocated";
    else if(tmpfile->meta->flags == TSK_FS_META_FLAG_UNALLOC)
        proplist << "Currently Unallocated";
    else if(tmpfile->meta->flags == TSK_FS_META_FLAG_USED)
        proplist << "Allocated at least once";
    else if(tmpfile->meta->flags == TSK_FS_META_FLAG_UNUSED)
        proplist << "Never allocated";
    else if(tmpfile->meta->flags == TSK_FS_META_FLAG_COMP)
        proplist << "Contents are compressed";
    else
        proplist << "Unspecified";
    proplist << "allocation status for the file.";
    if(tmpptr != NULL)
        LogEntry(0, 0, currentjobid, 2, "TmpPtr got a value somehow");
    TSK_FS_HASH_RESULTS hashresults;
    uint8_t retval = tsk_fs_file_hash_calc(tmpfile, &hashresults, TSK_BASE_HASH_MD5);
    QString tmpstring;
    if(retval == 0)
    {
        char sbuf[17];
        int sint = 0;
        for(int i=0; i < 16; i++)
        {
            sint = sprintf(sbuf+(2*i), "%02X", hashresults.md5_digest[i]);
        }
        if(sint > 0)
            tmpstring = QString(sbuf);
    }
    else
        tmpstring = QString("");

    QVector<QString> filestrings;
    if(tmpfile->name != NULL) filestrings.append(QString(tmpfile->name->name));
    else filestrings.append(QString("unknown.wbt"));
    filestrings.append(QString("/") + QString(tmppath));
    filestrings.append(tmpstring);

    // BEGIN TEST AREA FOR GETTING THE BLOCK ADDRESSES FOR A FILE
    blockstring = "";
    if(tmpfile->fs_info->ftype == TSK_FS_TYPE_HFS_DETECT || tmpfile->fs_info->ftype == TSK_FS_TYPE_ISO9660_DETECT || tmpfile->fs_info->ftype == TSK_FS_TYPE_NTFS_DETECT || tmpfile->fs_info->ftype == TSK_FS_TYPE_FAT_DETECT)
    {
        if(tmpfile->fs_info->ftype == TSK_FS_TYPE_HFS_DETECT)
        {
            tsk_fs_file_walk_type(tmpfile, TSK_FS_ATTR_TYPE_HFS_DATA, HFS_FS_ATTR_ID_DATA, (TSK_FS_FILE_WALK_FLAG_ENUM)(TSK_FS_FILE_WALK_FLAG_AONLY | TSK_FS_FILE_WALK_FLAG_SLACK), GetBlockAddress, NULL);
        }
        else if(tmpfile->fs_info->ftype == TSK_FS_TYPE_ISO9660_DETECT)
        {
            iso9660_inode* dinode;
            dinode = (iso9660_inode*)tsk_malloc(sizeof(iso9660_inode));
            iso9660_inode_node* n;
            n = ((ISO_INFO*)tmpfile->fs_info)->in_list;
            while(n && (n->inum != tmpfile->meta->addr))
                n = n->next;
            if(n)
                memcpy(dinode, &n->inode, sizeof(iso9660_inode));
            int block = tsk_getu32(tmpfile->fs_info->endian, dinode->dr.ext_loc_m);
            TSK_OFF_T size = tmpfile->meta->size;
            while((int64_t)size > 0)
            {
                blockstring += QString::number(block++) + "|";
                //qDebug() << "File Name:" << tmpfile->name->name << "Block Address:" << block++;
                size -= tmpfile->fs_info->block_size;
            }
        }
        else if(tmpfile->fs_info->ftype == TSK_FS_TYPE_NTFS_DETECT)
        {
            if(tmpfile->meta->attr)
            {
                int cnt, i;
                cnt = tsk_fs_file_attr_getsize(tmpfile);
                for(i = 0; i < cnt; i++)
                {
                    //char type[512];
                    const TSK_FS_ATTR* tmpattr = tsk_fs_file_attr_get_idx(tmpfile, i);
                    if(tmpattr->flags & TSK_FS_ATTR_NONRES)
                    {
                        tsk_fs_file_walk_type(tmpfile, tmpattr->type, tmpattr->id, (TSK_FS_FILE_WALK_FLAG_ENUM)(TSK_FS_FILE_WALK_FLAG_AONLY | TSK_FS_FILE_WALK_FLAG_SLACK), GetBlockAddress, NULL);
                    }
                }
            }
        }
        else if(tmpfile->fs_info->ftype == TSK_FS_TYPE_FAT_DETECT)
        {
            tsk_fs_file_walk(tmpfile, (TSK_FS_FILE_WALK_FLAG_ENUM)(TSK_FS_FILE_WALK_FLAG_AONLY | TSK_FS_FILE_WALK_FLAG_SLACK), GetBlockAddress, NULL);
        }
    }
    else
        tsk_fs_file_walk(tmpfile, TSK_FS_FILE_WALK_FLAG_AONLY, GetBlockAddress, NULL);
    // END TEST AREA FOR GETTING THE BLOCK ADDRESSES FOR A FILE

    filestrings.append(blockstring);
    proplist << "Block Address" << blockstring << "List of block addresses which contain the contents of the file";

    // BEGIN TEST AREA FOR GETTING THE FILE SIGNATURE STRING
    char magicbuffer[1024];
    ssize_t readlen = tsk_fs_file_read(tmpfile, 0, magicbuffer, 1024, TSK_FS_FILE_READ_FLAG_NONE);
    if(readlen > 0)
    {
        const char* sigtype = magic_buffer(magicptr, magicbuffer, readlen);
        char* sigp1 = strtok((char*)sigtype, ";");
        filestrings.append(QString::fromStdString(string(sigp1)));
        //qDebug() << "sigtype:" << QString::fromStdString(string(sigp1));
    }
    else
        filestrings.append(QString("Zero File"));
    // END TEST AREA FOR GETTING THE FILE SIGNATURE STRING

    QVector<int> fileints;
    qDebug() << "file name:" << tmpfile->name->name;
    if(tmpfile->name != NULL)
    {
        qDebug() << "name type:" << tmpfile->name->type;
        fileints.append((int)tmpfile->name->type);
        fileints.append((int)tmpfile->name->par_addr);
    }
    else
    {
        fileints.append(0);
        fileints.append(0);
    }
    if(tmpfile->meta != NULL)
    {
        qDebug() << "meta type:" << tmpfile->meta->type;
        fileints.append((int)tmpfile->meta->atime);
        fileints.append((int)tmpfile->meta->ctime);
        fileints.append((int)tmpfile->meta->crtime);
        fileints.append((int)tmpfile->meta->mtime);
        fileints.append((int)tmpfile->meta->size);
        fileints.append((int)tmpfile->meta->addr);
    }
    else
    {
        fileints.append(0);
        fileints.append(0);
        fileints.append(0);
        fileints.append(0);
        fileints.append(0);
        fileints.append(0);
    }
    fileints.append(currentfilesystemid);

    QFuture<void> tmpfuture = QtConcurrent::run(ProcessFile, filestrings, fileints, proplist);
    filewatcher.setFuture(tmpfuture);
    threadvector.append(tmpfuture);
    filesfound++;

    return TSK_WALK_CONT;
}

void cnid_to_array(uint32_t cnid, uint8_t array[4])
{
    array[3] = (cnid >> 0) & 0xff;
    array[2] = (cnid >> 8) & 0xff;
    array[1] = (cnid >> 16) & 0xff;
    array[0] = (cnid >> 24) & 0xff;
}


std::string GetSegmentValue(IMG_AFF_INFO* curaffinfo, const char* segname)
{
    unsigned char buf[512];
    size_t buflen = 512;
    std::stringstream stm;
    std::string s;
    int ilimit = 0;
    if(af_get_seg(curaffinfo->affile, segname, NULL, buf, &buflen) == 0)
    {
        if(strcmp(segname, AF_MD5) == 0 || strcmp(segname,AF_SHA1) == 0 || strcmp(segname, AF_IMAGE_GID) == 0)
        {
            if(strcmp(segname,AF_MD5) == 0)
                ilimit = 16;
            else if(strcmp(segname, AF_SHA1) == 0)
                ilimit = 20;
            else
                ilimit = buflen;
            unsigned char* bytebuf = buf;
            stm << std::hex << std::setfill('0');
            for(int i=0; i < ilimit; i++)
            {
                stm << std::setw(2) << static_cast<int>(bytebuf[i]);
            }
            s = stm.str();
        }
        else
        {
            buf[buflen] = '\0';
            stm << buf;
            s = stm.str();
        }
    }
    return s;
}
