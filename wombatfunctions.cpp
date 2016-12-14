#include "wombatfunctions.h"

// Copyright 2015 Pasquale J. Rinaldi, Jr.
// Distrubted under the terms of the GNU General Public License version 2

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

void LogMessage(QString logmsg)
{
    QString tmpstring = QDateTime::currentDateTime().toString(QString("MM/dd/yyyy hh:mm:ss t"));
    //msglog->append(QString(tmpstring + " " + logmsg));
    logfile.open(QIODevice::ReadWrite | QIODevice::Append | QIODevice::Text);
    logfile.write(QString(tmpstring + "\t" + logmsg + "\n").toStdString().c_str());
    logfile.close();
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

//unsigned long long GetChildCount(int type, unsigned long long address, unsigned long long parimgid, unsigned long long parfsid)
unsigned long long GetChildCount(QString filefilter)
{
    unsigned long long tmpcount = 0;
    QDir eviddir = QDir(wombatvariable.tmpmntpath);
    QStringList files = eviddir.entryList(QStringList(filefilter), QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot | QDir::NoSymLinks);
    tmpcount = files.count();
    /*
    QSqlQuery childquery(fcasedb);
    QString querystring = "SELECT COUNT(id) FROM data WHERE parid = ?";
    if(type < 4)
        querystring += " AND objtype < 5";
    else
    {
        querystring += " AND objtype >= 5 AND parfsid = ?";
    }
    if(type != 1)
        querystring += " AND parimgid = ?";
    childquery.prepare(querystring);
    childquery.addBindValue(address);
    if(type >= 4)
        childquery.addBindValue(parfsid);
    if(type != 1)
        childquery.addBindValue(parimgid);
    if(childquery.exec())
    {
        childquery.next();
        tmpcount = childquery.value(0).toULongLong();
    }
    childquery.finish();
    */
    return tmpcount;
}

QStringList GetChildFiles(QString filefilter)
{
    QDir eviddir = QDir(wombatvariable.tmpmntpath);
    return eviddir.entryList(QStringList(filefilter), QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot | QDir::NoSymLinks);
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
    if(filesfound == filesprocessed && filesfound > 0)
        return true;
    
    return false;
}
/*
void ProcessFile(QVector<QString> tmpstrings, QVector<unsigned long long> tmpints, FileData adsdata, bool adsbool)
{
    FileData tmpdata;
    tmpdata.type = tmpints[0];
    tmpdata.name = tmpstrings[0];
    tmpdata.paraddr = tmpints[1];
    tmpdata.path = tmpstrings[1];
    tmpdata.atime = tmpints[2];
    tmpdata.ctime = tmpints[3];
    tmpdata.crtime = tmpints[4];
    tmpdata.mtime = tmpints[5];
    tmpdata.size = tmpints[6];
    tmpdata.addr = tmpints[7];
    tmpdata.evid = currentevidenceid;
    tmpdata.fsid = tmpints[8];
    tmpdata.mftattrid = 0;
    //mutex.lock();
    filedatavector.append(tmpdata);
    if(adsbool == true)
    {
        filedatavector.append(adsdata);
    }
    //mutex.unlock();
}
*/
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

    if(tmpfile->fs_info->ftype == TSK_FS_TYPE_HFS_DETECT)
    {
        blockstring += QString::number(addr) + "|";
    }
    else if(tmpfile->fs_info->ftype == TSK_FS_TYPE_ISO9660_DETECT)
    {
        // iso is already done in the previous function
    }
    else if(tmpfile->fs_info->ftype == TSK_FS_TYPE_FAT_DETECT || tmpfile->fs_info->ftype == TSK_FS_TYPE_NTFS_DETECT)
    {
        blockstring += QString::number(addr) + "|";
    }
    else if(tmpfile->fs_info->ftype == TSK_FS_TYPE_YAFFS2_DETECT)
    {
        if(flags & TSK_FS_BLOCK_FLAG_CONT)
        {
            blockstring += QString::number(addr) + "|";
        }
    }
    else if(tmpfile->name != NULL)
    {
        if((strcmp(tmpfile->name->name, "$FAT1") == 0) || (strcmp(tmpfile->name->name, "$FAT2") == 0) || (strcmp(tmpfile->name->name, "$MBR") == 0) || (strcmp(tmpfile->name->name, "$OprhanFiles") == 0))
        {
            blockstring += QString::number(addr) + "|";
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

void FileMap(FileData &filedata)
{
    /*
    QFile filefile(wombatvariable.tmpmntpath + wombatvariable.evidenceobject.name + ".f" + QString::number(filedata.addr));
    filefile.open(QIODevice::Append | QIODevice::Text);
    QTextStream out(&filefile);
    out << filedata.name << "," << filedata.path << "," << filedata.type << "," << filedata.paraddr << "," << filedata.atime << "," << filedata.ctime << "," << filedata.crtime << "," << filedata.mtime << "," << filedata.size << "," << filedata.addr << "," << filedata.evid << "," << filedata.fsid << "," << filedata.mftattrid << "," << filedata.mimetype;
    filefile.close();
    isignals->ProgUpd();
    //emit isignals->FinishSql();
    */

}

TSK_WALK_RET_ENUM FileEntries(TSK_FS_FILE* tmpfile, const char* tmppath, void* tmpptr)
{
    QString outstring = "";
    /*
    //QFile filefile;
    if(tmpfile->meta != NULL)
    {
        qDebug() << "file addr: " << tmpfile->meta->addr;
        //filefile.setFileName(wombatvariable.tmpmntpath + wombatvariable.evidenceobject.name + ".f" + QString::number(tmpfile->meta->addr));
    }
    else
    {
        qDebug() << "file addr: meta is null: " << tmpfile->name->meta_addr;
        //filefile.setFileName(wombatvariable.tmpmntpath + wombatvariable.evidenceobject.name + ".f" + QString::number(tmpfile->name->meta_addr));
    }
    //filefile.open(QIODevice::Append | QIODevice::Text);
    //QTextStream out(&filefile);
    */
    if(tmpfile->name != NULL)
    {
        outstring += QString(tmpfile->name->name) + "," + QString::number(tmpfile->name->type) + "," + QString::number(tmpfile->name->par_addr) + ",";
        //out << tmpfile->name->name << "," << tmpfile->name->type << "," << tmpfile->name->par_addr << ",";
    }
    else
    {
        outstring += "unknown," + QString::number(tmpfile->meta->type) + ",0,";
        //out << "unknown," + tmpfile->meta->type + ",0,";
    }
    outstring += "/" + QString(tmppath) + ",";
    //out << "/" << QString(tmppath) << ",";
    if(tmpfile->meta != NULL)
    {
        outstring += QString::number(tmpfile->meta->atime) + "," + QString::number(tmpfile->meta->ctime) + "," + QString::number(tmpfile->meta->crtime) + "," + QString::number(tmpfile->meta->mtime) + "," + QString::number(tmpfile->meta->size) + "," + QString::number(tmpfile->meta->addr) + ",";
        //out << tmpfile->meta->atime << "," << tmpfile->meta->ctime << "," << tmpfile->meta->crtime << "," << tmpfile->meta->mtime << "," << tmpfile->meta->size << "," << tmpfile->meta->addr;
    }
    else
    {
        outstring += "0,0,0,0,0," + QString::number(tmpfile->name->meta_addr) + ",";
        //out << "0,0,0,0,0," + tmpfile->name->meta_addr + ",";
    }
    char magicbuffer[1024];
    tsk_fs_file_read(tmpfile, 0, magicbuffer, 1024, TSK_FS_FILE_READ_FLAG_NONE);
    QMimeDatabase mimedb;
    QMimeType mimetype = mimedb.mimeTypeForData(QByteArray((char*)magicbuffer));
    outstring += mimetype.name() + ",0,e" + QString::number(evidcnt) + "-v" + QString::number(volcnt) + "-p" + QString::number(partint) + "-f" + QString::number(tmpfile->name->meta_addr);
    //out << mimetype.name() << ",0";
    QFile filefile;
    QTextStream out(&filefile);
    if(tmpfile->name != NULL)
    {
        if(strcmp(tmpfile->name->name, ".") != 0)
        {
            if(strcmp(tmpfile->name->name, "..") != 0)
            {
                    filefile.setFileName(wombatvariable.tmpmntpath + wombatvariable.evidenceobject.name + ".p" + QString::number(partint) + ".f" + QString::number(tmpfile->name->meta_addr) + ".a" + QString::number(tmpfile->name->par_addr));
                    filefile.open(QIODevice::Append | QIODevice::Text);
                    out << outstring;
                    filesfound++;
            }
        }
    }
    else
    {
        filefile.setFileName(wombatvariable.tmpmntpath + wombatvariable.evidenceobject.name + ".p" + QString::number(partint) + ".f" + QString::number(tmpfile->meta->addr) + ".a" + QString::number(tmpfile->name->par_addr));
        filefile.open(QIODevice::Append | QIODevice::Text);
        out << outstring;
        filesfound++;
    }
    filefile.close();
    isignals->ProgUpd();

    if(tmpfile->fs_info->ftype == TSK_FS_TYPE_NTFS_DETECT)
    {
        int tmpattrid = 0;
        unsigned long long adssize = 0;
        TSK_OFF_T curmftentrystart = 0;
        NTFS_INFO* ntfsinfo = (NTFS_INFO*)tmpfile->fs_info;
        int recordsize = 0;
        if(ntfsinfo->fs->mft_rsize_c > 0)
        {
            recordsize = ntfsinfo->fs->mft_rsize_c * ntfsinfo->fs->csize * tsk_getu16(tmpfile->fs_info->endian, ntfsinfo->fs->ssize);
        }
        else
            recordsize = 1 << -ntfsinfo->fs->mft_rsize_c;
        if(tmpfile->meta != NULL)
            curmftentrystart = tsk_getu16(tmpfile->fs_info->endian, ntfsinfo->fs->ssize) * ntfsinfo->fs->csize * tsk_getu64(tmpfile->fs_info->endian, ntfsinfo->fs->mft_clust) + recordsize * tmpfile->meta->addr + 20;
        else
            curmftentrystart = tsk_getu16(tmpfile->fs_info->endian, ntfsinfo->fs->ssize) * ntfsinfo->fs->csize * tsk_getu64(tmpfile->fs_info->endian, ntfsinfo->fs->mft_clust) + recordsize + 20;
        char startoffset[2];
        tsk_fs_read(tmpfile->fs_info, curmftentrystart, startoffset, 2);
        uint16_t teststart = startoffset[1] * 256 + startoffset[0];
        adssize = (unsigned long long)teststart;
        if(strcmp(tmpfile->name->name, ".") != 0)
        {
            if(strcmp(tmpfile->name->name, "..") != 0)
            {
                int cnt, i;
                cnt = tsk_fs_file_attr_getsize(tmpfile);
                for(i = 0; i < cnt; i++)
                {
                    char type[512];
                    const TSK_FS_ATTR* fsattr = tsk_fs_file_attr_get_idx(tmpfile, i);
                    adssize += 24;
                    adssize += (unsigned long long)fsattr->size;
                    if(ntfs_attrname_lookup(tmpfile->fs_info, fsattr->type, type, 512) == 0)
                    {
                        if(QString::compare(QString(type), "$DATA", Qt::CaseSensitive) == 0)
                        {
                            if(QString::compare(QString(fsattr->name), "") != 0 && QString::compare(QString(fsattr->name), "$I30", Qt::CaseSensitive) != 0)
                            {
                                QFile adsfile(wombatvariable.tmpmntpath + wombatvariable.evidenceobject.name + ".p" + QString::number(partint) + ".f" + QString::number(tmpfile->meta->addr) + ".f" + QString::number(adssize - (unsigned long long)fsattr->size + 16));
                                adsfile.open(QIODevice::Append | QIODevice::Text);
                                QTextStream adsout(&adsfile);
                                adsout << QString(":") + QString(fsattr->name) << "," << tmpfile->name->type << "," << tmpfile->meta->addr << "," << QString("/") + QString(tmppath) << ",0, 0, 0, 0," << fsattr->size << "," << adssize - (unsigned long long)fsattr->size + 16 << "," << mimetype.name() << "," << fsattr->id << ",e" + QString::number(evidcnt) + "-v" + QString::number(volcnt) + "-p" + QString::number(partint) + "-f" + QString::number(adssize - (unsigned long long)fsattr->size + 16);
                                adsfile.close();
                            }
                        }
                    }
                }
            }
        }
    }

    /*
    FileData tmpdata;
    filesfound++;
    isignals->ProgUpd();
    if(tmpptr != NULL)
    {
        //LogMessage("TmpPtr got a value somehow");
    }

    if(tmpfile->name != NULL) tmpdata.name = QString(tmpfile->name->name);
    else tmpdata.name = QString("unknown.dat");
    tmpdata.path = QString("/") + QString(tmppath);

    if(tmpfile->name != NULL)
    {
        tmpdata.type = (unsigned long long)tmpfile->name->type;
        tmpdata.paraddr = (unsigned long long)tmpfile->name->par_addr;
    }
    else
    {
        tmpdata.type = 0;
        tmpdata.type = 0;
    }
    if(tmpfile->meta != NULL)
    {
        tmpdata.atime = (unsigned long long)tmpfile->meta->atime;
        tmpdata.ctime = (unsigned long long)tmpfile->meta->ctime;
        tmpdata.crtime = (unsigned long long)tmpfile->meta->crtime;
        tmpdata.mtime = (unsigned long long)tmpfile->meta->mtime;
        tmpdata.size = (unsigned long long)tmpfile->meta->size;
        tmpdata.addr = (unsigned long long)tmpfile->meta->addr;
    }
    else
    {
        tmpdata.atime = 0;
        tmpdata.ctime = 0;
        tmpdata.crtime = 0;
        tmpdata.mtime = 0;
        tmpdata.size = 0;
        tmpdata.addr = 0;
    }
    tmpdata.evid = currentevidenceid;
    tmpdata.fsid = currentfilesystemid;
    tmpdata.mftattrid = 0;
    char magicbuffer[1024];
    tsk_fs_file_read(tmpfile, 0, magicbuffer, 1024, TSK_FS_FILE_READ_FLAG_NONE);
    QMimeDatabase mimedb;
    QMimeType mimetype = mimedb.mimeTypeForData(QByteArray((char*)magicbuffer));
    tmpdata.mimetype = mimetype.name();
    */
    /*
     *    readfileinfo = tsk_fs_file_open_meta(readfsinfo, NULL, secprocobj.address);
    char magicbuffer[1024];
    tsk_fs_file_read(readfileinfo, 0, magicbuffer, 1024, TSK_FS_FILE_READ_FLAG_NONE);


    // Begin Mime Type Determination
    QMimeDatabase mimedb;
    QMimeType mimetype = mimedb.mimeTypeForData(QByteArray((char*)magicbuffer));
    secprocobj.mimetype = mimetype.name();

     *
     */ 
    /*
    filedatavector.append(tmpdata);

    FileData adsdata;
    unsigned long long adssize = 0;
    TSK_OFF_T curmftentrystart = 0;
    if(tmpfile->fs_info->ftype == TSK_FS_TYPE_NTFS_DETECT)
    {
        NTFS_INFO* ntfsinfo = (NTFS_INFO*)tmpfile->fs_info;
        int recordsize = 0;
        if(ntfsinfo->fs->mft_rsize_c > 0)
        {
            recordsize = ntfsinfo->fs->mft_rsize_c * ntfsinfo->fs->csize * tsk_getu16(tmpfile->fs_info->endian, ntfsinfo->fs->ssize);
        }
        else
            recordsize = 1 << -ntfsinfo->fs->mft_rsize_c;
        if(tmpfile->meta != NULL)
            curmftentrystart = tsk_getu16(tmpfile->fs_info->endian, ntfsinfo->fs->ssize) * ntfsinfo->fs->csize * tsk_getu64(tmpfile->fs_info->endian, ntfsinfo->fs->mft_clust) + recordsize * tmpfile->meta->addr + 20;
        else
            curmftentrystart = tsk_getu16(tmpfile->fs_info->endian, ntfsinfo->fs->ssize) * ntfsinfo->fs->csize * tsk_getu64(tmpfile->fs_info->endian, ntfsinfo->fs->mft_clust) + recordsize + 20;
        char startoffset[2];
        tsk_fs_read(tmpfile->fs_info, curmftentrystart, startoffset, 2);
        uint16_t teststart = startoffset[1] * 256 + startoffset[0];
        adssize = (unsigned long long)teststart;
    }
    if(tmpfile->fs_info->ftype == TSK_FS_TYPE_NTFS_DETECT)
    {
        if(strcmp(tmpfile->name->name, ".") != 0)
        {
            if(strcmp(tmpfile->name->name, "..") != 0)
            {
                int cnt, i;
                cnt = tsk_fs_file_attr_getsize(tmpfile);
                for(i = 0; i < cnt; i++)
                {
                    char type[512];
                    const TSK_FS_ATTR* fsattr = tsk_fs_file_attr_get_idx(tmpfile, i);
                    adssize += 24;
                    adssize += (unsigned long long)fsattr->size;
                    if(ntfs_attrname_lookup(tmpfile->fs_info, fsattr->type, type, 512) == 0)
                    {
                        if(QString::compare(QString(type), "$DATA", Qt::CaseSensitive) == 0)
                        {
                            if(QString::compare(QString(fsattr->name), "") != 0 && QString::compare(QString(fsattr->name), "$I30", Qt::CaseSensitive) != 0)
                            {
                                adsdata.type = (unsigned long long)tmpfile->name->type;
                                adsdata.paraddr = (unsigned long long)tmpfile->meta->addr;
                                adsdata.name = QString(":") + QString(fsattr->name);
                                adsdata.path = QString("/") + QString(tmppath);
                                adsdata.atime = 0;
                                adsdata.ctime = 0;
                                adsdata.crtime = 0;
                                adsdata.mtime = 0;
                                adsdata.evid = currentevidenceid;
                                adsdata.fsid = currentfilesystemid;
                                adsdata.size = (unsigned long long)fsattr->size;
                                adsdata.addr = adssize - (unsigned long long)fsattr->size + 16;
                                adsdata.mimetype = mimetype.name();
                                adsdata.mftattrid = (unsigned long long)fsattr->id; // STORE attr id in this variable in the db.
                                filedatavector.append(adsdata);
                            }
                        }
                    }
                }
            }
        }
    }*/
    return TSK_WALK_CONT;
}

void GenerateThumbnails()
{
    /*
    QSqlQuery filequery(fcasedb);
    filequery.prepare("SELECT objectid, parimgid, parfsid, address FROM data WHERE objecttype = 5 AND filemime LIKE '%image/%';");
    if(filequery.exec())
    {
        while(filequery.next())
        {
            const TSK_TCHAR** imagepartspath;
            unsigned long long objectid = 0;
            objectid = filequery.value(0).toULongLong();
            /
            QSqlQuery thumbquery(thumbdb);
            thumbquery.prepare("SELECT COUNT(thumbid) FROM thumbs WHERE objectid = ?;");
            thumbquery.addBindValue(objectid);
            thumbquery.exec();
            thumbquery.next();
            if(thumbquery.value(0).toInt() == 0)
            {
                TSK_IMG_INFO* readimginfo;
                TSK_FS_INFO* readfsinfo;
                TSK_FS_FILE* readfileinfo;
                // Open Parent Image
                std::vector<std::string> pathvector;
                pathvector.clear();
                QSqlQuery imgquery(fcasedb);
                imgquery.prepare("SELECT fullpath FROM dataruns WHERE objectid = ? ORDER BY seqnum;");
                imgquery.bindValue(0, filequery.value(1).toULongLong());
                if(imgquery.exec())
                {
                    while(imgquery.next())
                    {
                        pathvector.push_back(imgquery.value(0).toString().toStdString());
                    }
                }
                imgquery.finish();
                imagepartspath = (const char**)malloc(pathvector.size()*sizeof(char*));
                for(uint i=0; i < pathvector.size(); i++)
                {
                    imagepartspath[i] = pathvector.at(i).c_str();
                }
                readimginfo = tsk_img_open(pathvector.size(), imagepartspath, TSK_IMG_TYPE_DETECT, 0);
                free(imagepartspath);
                //OpenParentFileSystem
                QSqlQuery fsquery(fcasedb);
                fsquery.prepare("SELECT byteoffset FROM data where objectid = ?;");
                fsquery.bindValue(0, filequery.value(2).toULongLong());
                fsquery.exec();
                fsquery.next();
                readfsinfo = tsk_fs_open_img(readimginfo, fsquery.value(0).toULongLong(), TSK_FS_TYPE_DETECT);
                fsquery.finish();
                //OpenFile
                readfileinfo = tsk_fs_file_open_meta(readfsinfo, NULL, filequery.value(3).toULongLong());
                ThumbFile(readfileinfo, objectid);    
                tsk_fs_file_close(readfileinfo);
                tsk_fs_close(readfsinfo);
                tsk_img_close(readimginfo);
            }
            thumbquery.finish();
        }
    }
    filequery.finish();
    */
}

void PropertyFile(TSK_FS_FILE* tmpfile, unsigned long long objid, unsigned long long fsoffset, int blksize, unsigned long long parfsid)
{
    QStringList proplist;
    proplist.clear();
    if(tmpfile->name != NULL) proplist << "Short Name" << tmpfile->name->shrt_name << "Short Name for a file";
    if(tmpfile->meta != NULL)
    {
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

        /*
        QSqlQuery objquery(fcasedb);
        objquery.prepare("SELECT blockaddress, filemime, filesignature, address FROM data WHERE objectid = ?;");
        objquery.bindValue(0, objid);
        objquery.exec();
        objquery.next();
        proplist << "Block Address" << objquery.value(0).toString() << "List of block addresses which contain the contents of the file";
        unsigned long long tmpoffset = 0;
        unsigned long long resoffset = 0;
        unsigned long long fileaddress = 0;
        fileaddress = objquery.value(3).toULongLong();
        proplist << "Byte Offset";
        if(objquery.value(0).toString().compare("") != 0)
            tmpoffset = objquery.value(0).toString().split("|", QString::SkipEmptyParts).at(0).toULongLong()*blksize + fsoffset;
        else
        {
            if(tmpfile->fs_info->ftype == TSK_FS_TYPE_NTFS_DETECT)
            {
                QSqlQuery resquery(fcasedb);
                QStringList inputs;
                QList<unsigned long long> outputs;
                inputs << "%0x0B%" << "%0x0D%" << "%0x30%" << "%0x40%";
                for(int i=0; i < inputs.count(); i++)
                {
                    resquery.prepare("SELECT value from properties where objectid = ? and description like(?);");
                    resquery.addBindValue(parfsid);
                    resquery.addBindValue(inputs.at(i));
                    resquery.exec();
                    resquery.next();
                    outputs.append(resquery.value(0).toULongLong());
                }
                resquery.finish();
                mftrecordsize = outputs.at(3);
                resoffset = ((outputs.at(0) * outputs.at(1) * outputs.at(2)) + (outputs.at(3)*fileaddress));
                tmpoffset = resoffset + fsoffset;
            }
            else
                tmpoffset = fsoffset;
        }
        proplist << QString::number(tmpoffset) << "Byte offset for the start of the file in a block or in the MFT";
        proplist << "File Signature" << objquery.value(1).toString() << objquery.value(2).toString();
        objquery.finish();

        fcasedb.transaction();
        QSqlQuery propquery(fcasedb);
        propquery.prepare("INSERT INTO properties (objectid, name, value, description) VALUES(?, ?, ?, ?);");
        for(int i=0; i < proplist.count()/3; i++)
        {
            propquery.bindValue(0, objid);
            propquery.bindValue(1, proplist.at(3*i));
            propquery.bindValue(2, proplist.at(3*i+1));
            propquery.bindValue(3, proplist.at(3*i+2));
            propquery.exec();
        }
        fcasedb.commit();
        propquery.finish();
        */
        processphase++;
        isignals->ProgUpd();
    }
}

void AlternateDataStreamPropertyFile(TSK_FS_FILE* tmpfile, QVector<unsigned long long> adsobjid, QVector<unsigned long long> adsattrid)
{
    QStringList proplist;
    proplist.clear();
    if(tmpfile->name != NULL)
    {
        QString adsinfo = "Alternate Data Stream for " + QString::fromStdString(std::string(tmpfile->name->name));
        proplist << "Alternate Data Stream (ADS)" << adsinfo << "Alternate data stream which contains different content from what the file's standard content is.";
    }
    for(int i = 0; i < adsobjid.count(); i++)
    {
        /*
        QSqlQuery adsquery(fcasedb);
        adsquery.prepare("SELECT name, parentid, blockaddress FROM data WHERE objectid = ?;");
        adsquery.bindValue(0, adsobjid.at(i));
        adsquery.exec();
        adsquery.next();
        proplist << "Name" << adsquery.value(0).toString() << "Name for the NTFS parent file additional $Data attribute";
        proplist << "Parent Address" << adsquery.value(1).toString() << "NTFS address ID for the parent file";
        if(tmpfile->name != NULL)
            proplist << "Parent File Name" << QString(tmpfile->name->name) << "File name of the parent file";
        proplist << "Block Address" << adsquery.value(2).toString() << "List of block addresses which contain the contents of the ADS";
        proplist << "Attribute ID" << QString::number(adsattrid.at(i)) << "ID for the file's ADS attribute";
        adsquery.finish();
        fcasedb.transaction();
        QSqlQuery propquery(fcasedb);
        propquery.prepare("INSERT INTO properties (objectid, name, value, description) VALUES(?, ?, ?, ?);");
        for(int j = 0; j < proplist.count()/3; j++)
        {
            propquery.bindValue(0, adsobjid.at(i));
            propquery.bindValue(1, proplist.at(3*j));
            propquery.bindValue(2, proplist.at(3*j+1));
            propquery.bindValue(3, proplist.at(3*j+2));
            propquery.exec();
        }
        fcasedb.commit();
        propquery.finish();
        */
   }
}
/*
void BlockFile(TSK_FS_FILE* tmpfile, unsigned long long objid, QVector<unsigned long long> adsattrid)
{
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
                size -= tmpfile->fs_info->block_size;
            }
        }
        else if(tmpfile->fs_info->ftype == TSK_FS_TYPE_NTFS_DETECT)
        {
            unsigned long long minads = 1000;
            for(int i = 0; i < adsattrid.count(); i++)
            {
                if(adsattrid.at(i) < minads)
                    minads = adsattrid.at(i);
            }
            if(tmpfile->meta != NULL)
            {
                if(tmpfile->meta->attr)
                {
                    int cnt, i;
                    cnt = tsk_fs_file_attr_getsize(tmpfile);
                    for(i = 0; i < cnt; i++)
                    {
                        const TSK_FS_ATTR* tmpattr = tsk_fs_file_attr_get_idx(tmpfile, i);
                        if(tmpattr->flags & TSK_FS_ATTR_NONRES) // non resident attribute
                        {
                            if(tmpattr->type == TSK_FS_ATTR_TYPE_NTFS_DATA && tmpattr->id < (int)minads)
                            {
                                tsk_fs_file_walk_type(tmpfile, tmpattr->type, tmpattr->id, (TSK_FS_FILE_WALK_FLAG_ENUM)(TSK_FS_FILE_WALK_FLAG_AONLY | TSK_FS_FILE_WALK_FLAG_SLACK), GetBlockAddress, NULL);
                            }
                        }
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
    QSqlQuery blockquery(fcasedb);
    blockquery.prepare("UPDATE data SET blockaddress = ? WHERE objectid = ?;");
    blockquery.bindValue(0, blockstring);
    blockquery.bindValue(1, objid);
    blockquery.exec();
    blockquery.next();
    blockquery.finish();
    processphase++;
    isignals->ProgUpd();
}

void AlternateDataStreamBlockFile(TSK_FS_FILE* tmpfile, QVector<unsigned long long> adsobjid, QVector<unsigned long long> adsattrid)
{
    blockstring = "";
    if(tmpfile->fs_info->ftype == TSK_FS_TYPE_NTFS_DETECT)
    {
        for(int j=0; j < adsattrid.count(); j++)
        {
            if(tmpfile->meta != NULL)
            {
                tsk_fs_file_walk_type(tmpfile, TSK_FS_ATTR_TYPE_NTFS_DATA, adsattrid.at(j), (TSK_FS_FILE_WALK_FLAG_ENUM)(TSK_FS_FILE_WALK_FLAG_AONLY | TSK_FS_FILE_WALK_FLAG_SLACK), GetBlockAddress, NULL);
            }
            QSqlQuery blockquery(fcasedb);
            blockquery.prepare("UPDATE data SET blockaddress = ? WHERE objectid = ?;");
            blockquery.bindValue(0, blockstring);
            blockquery.bindValue(1, adsobjid.at(j));
            blockquery.exec();
            blockquery.next();
            blockquery.finish();
        }
    }
}
*/
/*
QVariant MagicFile(TSK_FS_FILE* tmpfile, unsigned long long objid)
{
    QVariant tmpvariant;
    // FILE MIME TYPE
    char magicbuffer[1024];
    //const char* mimesig;
    //const char* sigtype;
    //char* sigp1;
    //char* sigp2;
    ssize_t readlen = tsk_fs_file_read(tmpfile, 0, magicbuffer, 1024, TSK_FS_FILE_READ_FLAG_NONE);
    if(readlen > 0)
    {
        mimesig = magic_buffer(magicmimeptr, magicbuffer, readlen);
        sigp1 = strtok((char*)mimesig, ";");
        sigtype = magic_buffer(magicptr, magicbuffer, readlen);
        sigp2 = strtok((char*)sigtype, ",");
    }
    QSqlQuery mimequery(fcasedb);
    mimequery.prepare("UPDATE data SET filemime = ?, filesignature = ? WHERE objectid = ?;");
    if(readlen > 0)
    {
        tmpvariant = QVariant(QString::fromStdString(sigp2));
        mimequery.bindValue(0, QString::fromStdString(sigp1));
        mimequery.bindValue(1, QString::fromStdString(sigp2));
    }
    else
    {
        tmpvariant = QVariant(QString::fromStdString("Zero File"));
        mimequery.bindValue(0, QString("Zero File"));
        mimequery.bindValue(1, QString("Zero File"));
    }
    mimequery.bindValue(2, objid);
    mimequery.exec();
    mimequery.next();
    mimequery.finish();
    processphase++;
    isignals->ProgUpd();
    return tmpvariant;
}
*/
/*
QVariant AlternateDataStreamMagicFile(TSK_FS_FILE* tmpfile, unsigned long long adsobjid)
{
    QVariant tmpvariant;
    off_t retval = 0;
    char magicbuffer[1024];
    int chunksize = 1024;
    //const char* mimesig;
    //const char* sigtype;
    //char* sigp1;
    //char* sigp2;
    QSqlQuery adsquery(fcasedb);
    adsquery.prepare("SELECT name, fullpath, parimgid, parfsid, parentid, address, mftattrid, size, blockaddress FROM data WHERE objectid = ?;");
    adsquery.bindValue(0, adsobjid);
    adsquery.exec();
    adsquery.next();
    if(adsquery.value(7).toULongLong() < (unsigned)chunksize)
        chunksize = adsquery.value(7).toULongLong();
    if(adsquery.value(8).toString().compare("") != 0)
    {
        if(adsquery.value(8).toString().split("|", QString::SkipEmptyParts).at(0).toULongLong() == 0) // block address empty
        {
            retval = tsk_fs_file_read_type(tmpfile, TSK_FS_ATTR_TYPE_NTFS_DATA, adsquery.value(6).toInt(), 0, magicbuffer, chunksize, TSK_FS_FILE_READ_FLAG_NONE);
        }
        else // block address contains valid data
        {
            retval = tsk_fs_read_block(tmpfile->fs_info, adsquery.value(8).toString().split("|", QString::SkipEmptyParts).at(0).toULongLong(), magicbuffer, chunksize);
        }
    }
    else // blockaddress = ""
    {
        if(adsquery.value(7).toULongLong() != 0)
        {
            retval = tsk_fs_file_read_type(tmpfile, TSK_FS_ATTR_TYPE_NTFS_DATA, adsquery.value(6).toInt(), 0, magicbuffer, chunksize, TSK_FS_FILE_READ_FLAG_NONE);
        }
    }
    if(retval > 0)
    {
        mimesig = magic_buffer(magicmimeptr, magicbuffer, chunksize);
        sigp1 = strtok((char*)mimesig, ";");
        sigtype = magic_buffer(magicptr, magicbuffer, chunksize);
        sigp2 = strtok((char*)sigtype, ";");
    }
    QSqlQuery mimequery(fcasedb);
    mimequery.prepare("UPDATE data SET filemime = ?, filesignature = ? WHERE objectid = ?;");
    if(retval > 0)
    {
        tmpvariant = QVariant(QString::fromStdString(sigp2));
        mimequery.bindValue(0, QString::fromStdString(sigp1));
        mimequery.bindValue(1, QString::fromStdString(sigp2));
    }
    else
    {
        tmpvariant = QVariant(QString("Zero File"));
        mimequery.bindValue(0, QString("Zero File"));
        mimequery.bindValue(1, QString("Zero File"));
    }
    mimequery.bindValue(2,  adsobjid);
    mimequery.exec();
    mimequery.next();
    mimequery.finish();
    adsquery.finish();
    return tmpvariant;
}
*/

void ThumbFile(TSK_FS_FILE* tmpfile, unsigned long long objid)
{
    /*
    QSqlQuery thumbquery(thumbdb);
    thumbquery.prepare("SELECT COUNT(thumbid) FROM thumbs WHERE objectid = ?;");
    thumbquery.addBindValue(objid);
    thumbquery.exec();
    thumbquery.next();
    if(thumbquery.value(0).toInt() == 0)
    {
        if(tmpfile->meta != NULL)
        {
            QByteArray thumbdata;
            QImage thumbimage;
            QBuffer thumbuf(&thumbdata);
            QImage origimage;
            char imagebuffer[tmpfile->meta->size];
            ssize_t imglen = tsk_fs_file_read(tmpfile, 0, imagebuffer, tmpfile->meta->size, TSK_FS_FILE_READ_FLAG_NONE);
            bool imageloaded = origimage.loadFromData(QByteArray::fromRawData(imagebuffer, imglen));
            if(imageloaded)
            {
                thumbimage = origimage.scaled(QSize(320, 320), Qt::KeepAspectRatio, Qt::FastTransformation);
                thumbuf.open(QIODevice::WriteOnly);
                thumbimage.save(&thumbuf, "PNG");
                thumbdata = thumbdata.toBase64();
                QSqlQuery imgquery(thumbdb);
                imgquery.prepare("INSERT INTO thumbs(objectid, thumbblob) VALUES(?, ?);");
                imgquery.bindValue(0, objid);
                imgquery.bindValue(1, QString(thumbdata));
                imgquery.exec();
                imgquery.next();
                imgquery.finish();
            }
        }
    }
    */
}

QVariant HashFile(TSK_FS_FILE* tmpfile, unsigned long long objid)
{
    QVariant tmpvariant;
    TSK_FS_HASH_RESULTS hashresults;
    uint8_t retval = tsk_fs_file_hash_calc(tmpfile, &hashresults, TSK_BASE_HASH_MD5);
    if(retval == 0)
    {
        char sbuf[17];
        int sint = 0;
        for(int i=0; i < 16; i++)
        {
            sint = sprintf(sbuf+(2*i), "%02X", hashresults.md5_digest[i]);
        }
        /*
        QSqlQuery hashquery(fcasedb);
        hashquery.prepare("UPDATE data SET md5 = ? where objectid = ?;");
        if(sint > 0)
        {
            hashquery.bindValue(0, QString(sbuf));
            tmpvariant = QVariant(QString(sbuf));
        }
        else
        {
            hashquery.bindValue(0, QString(""));
            tmpvariant = QVariant(QString(""));
        }
        hashquery.bindValue(1, objid);
        hashquery.exec();
        hashquery.next();
        hashquery.finish();
        */
    }
    return tmpvariant;
}

void cnid_to_array(uint32_t cnid, uint8_t array[4])
{
    array[3] = (cnid >> 0) & 0xff;
    array[2] = (cnid >> 8) & 0xff;
    array[1] = (cnid >> 16) & 0xff;
    array[0] = (cnid >> 24) & 0xff;
}

QImage MakeThumb(const QString &img)
{
    QByteArray ba = QByteArray::fromBase64(QByteArray::fromStdString(img.toStdString()));
    if(ba.length() > 0)
        return QImage::fromData(ba, "PNG").scaled(QSize(thumbsize, thumbsize), Qt::KeepAspectRatio, Qt::FastTransformation);
    else
        return QImage(":/bar/missingthumb").scaled(QSize(thumbsize, thumbsize), Qt::KeepAspectRatio, Qt::FastTransformation);
}

void SqlMap(FileData &filedata)
{
    //QMutexLocker locker(&mutex);
    /*
    QSqlQuery fquery(fcasedb);
    //wombattableschema << "CREATE TABLE data(objectid INTEGER PRIMARY KEY, objecttype INTEGER, type INTEGER, name TEXT, fullpath TEXT, address INTEGER, parentid INTEGER, parimgid INTEGER, parfsid INTEGER, ctime INTEGER, crtime INTEGER, atime INTEGER, mtime INTEGER, md5 TEXT NOT NULL DEFAULT "", filemime TEXT, known INTEGER, checked INTEGER NOT NULL DEFAULT 0, mftattrid INTEGER NOT NULL DEFAULT 0);";
    fquery.prepare("INSERT INTO data(objtype, type, name, fullpath, size, addr, parid, parimgid, parfsid, ctime, crtime, atime, mtime, filemime, mftattrid) VALUES(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?);");
    if(filedata.mftattrid == 0)
    {
        fquery.bindValue(0, 5);
        filesprocessed++;
        processphase++;
    }
    else
        fquery.bindValue(0, 6);
    fquery.bindValue(1, filedata.type);
    fquery.bindValue(2, filedata.name);
    fquery.bindValue(3, filedata.path);
    fquery.bindValue(4, filedata.size);
    fquery.bindValue(5, filedata.addr);
    fquery.bindValue(6, filedata.paraddr);
    fquery.bindValue(7, filedata.evid);
    fquery.bindValue(8, filedata.fsid);
    fquery.bindValue(9, filedata.ctime);
    fquery.bindValue(10, filedata.crtime);
    fquery.bindValue(11, filedata.atime);
    fquery.bindValue(12, filedata.mtime);
    fquery.bindValue(13, filedata.mimetype);
    fquery.bindValue(14, filedata.mftattrid);
    /*
    fquery.bindValue(4, filedata.paraddr);
    fquery.bindValue(4, filedata.path);
    fquery.bindValue(6, filedata.ctime);
    fquery.bindValue(9, filedata.size);
    fquery.bindValue(10, filedata.addr);
    //fquery.bindValue(14, filedata.mimetype.split("/").at(0));
    */
    /*
    fquery.exec();
    //fquery.next();
    isignals->ProgUpd();
    //emit isignals->FinishSql();
    fquery.finish();
    */
}
void InitializeEvidenceStructure(WombatVariable &wombatvariable)
{
    readimginfo = NULL;
    readvsinfo = NULL;
    readfsinfo = NULL;
    readfileinfo = NULL;
    //AddNewEvidence();
    const TSK_TCHAR** images;
    images = (const char**)malloc(wombatvariable.evidenceobject.fullpathvector.size()*sizeof(char*));
    for(uint i=0; i < wombatvariable.evidenceobject.fullpathvector.size(); i++)
    {
        images[i] = wombatvariable.evidenceobject.fullpathvector[i].c_str();
    }
    readimginfo = tsk_img_open(wombatvariable.evidenceobject.itemcount, images, TSK_IMG_TYPE_DETECT, 0);
    if(readimginfo == NULL)
    {
        LogMessage("Evidence Image acess failed");
        errorcount++;
    }
    free(images);
    fsobjectlist.clear();
    // PUT FILES IN THE SPARSE FILE NOW.
    QFile evidfile(wombatvariable.tmpmntpath + wombatvariable.evidenceobject.name + ".evid");
    evidfile.open(QIODevice::Append | QIODevice::Text);
    QTextStream out(&evidfile);
    out << (int)readimginfo->itype << "," << (unsigned long long)readimginfo->size << "," << (int)readimginfo->sector_size;
    for(unsigned int i=0; i < wombatvariable.evidenceobject.itemcount; i++)
        out << "," << QString::fromStdString(wombatvariable.evidenceobject.fullpathvector[i]) << "," << i+1 << ",e" + QString::number(evidcnt);
    evidfile.close();
    readvsinfo = tsk_vs_open(readimginfo, 0, TSK_VS_TYPE_DETECT);
    QString volname = "Dummy Volume";
    int voltype = 240;
    int volsectorsize = (int)readimginfo->sector_size;
    unsigned long long voloffset = 0;
    if(readvsinfo != NULL)
    {
        voltype = (int)readvsinfo->vstype;
        volname = QString::fromUtf8(tsk_vs_type_todesc(readvsinfo->vstype));
        volsectorsize = (int)readvsinfo->block_size;
        voloffset = (unsigned long long)readvsinfo->offset;
    }
    QFile volfile(wombatvariable.tmpmntpath + wombatvariable.evidenceobject.name + ".vol");
    volfile.open(QIODevice::Append | QIODevice::Text);
    out.setDevice(&volfile);
    out << voltype << "," << (unsigned long long)readimginfo->size << "," << volname << "," << volsectorsize << "," << voloffset << ",e" + QString::number(evidcnt) + "-v" + QString::number(volcnt);
    volfile.close();
    if(readvsinfo == NULL) // No volume, so a single file system is all there is in the image.
    {
        readfsinfo = tsk_fs_open_img(readimginfo, 0, TSK_FS_TYPE_DETECT);
        QFile pfile(wombatvariable.tmpmntpath + wombatvariable.evidenceobject.name + ".p0");
        pfile.open(QIODevice::Append | QIODevice::Text);
        out.setDevice(&pfile);
        out << readfsinfo->ftype << "," << (unsigned long long)readfsinfo->block_size * (unsigned long long)readfsinfo->block_count << "," << GetFileSystemLabel(readfsinfo) << "," << (unsigned long long)readfsinfo->root_inum << "," << (unsigned long long)readfsinfo->offset << "," << (unsigned long long)readfsinfo->block_count << "," << (int)readfsinfo->block_size << ",0,0,0,e" << QString::number(evidcnt) + "-v" + QString::number(volcnt) + "-p" + QString::number(partint);
        pfile.close();
        uint8_t walkreturn;
        int walkflags = TSK_FS_DIR_WALK_FLAG_ALLOC | TSK_FS_DIR_WALK_FLAG_UNALLOC | TSK_FS_DIR_WALK_FLAG_RECURSE;
        walkreturn = tsk_fs_dir_walk(readfsinfo, readfsinfo->root_inum, (TSK_FS_DIR_WALK_FLAG_ENUM)walkflags, FileEntries, NULL);
        if(walkreturn == 1)
        {
            LogMessage("Issues with traversing the file structure were encountered");
            errorcount++;
        }
    }
    else
    {
        QFile pfile;
        if(readvsinfo->part_count > 0)
        {
            for(uint32_t i=0; i < readvsinfo->part_count; i++)
            {
                if(readpartinfo->flags == 0x02 || readpartinfo->flags == 0x01)
                {
                    readpartinfo = tsk_vs_part_get(readvsinfo, i);
                    pfile.setFileName(wombatvariable.tmpmntpath + wombatvariable.evidenceobject.name + ".p" + QString::number(partint));
                    partint++;
                    pfile.open(QIODevice::Append | QIODevice::Text);
                    out.setDevice(&pfile);
                    //qDebug() << readpartinfo->flags;
                }
                if(readpartinfo->flags == 0x02) // unallocated partition
                {
                    out << "0," << (unsigned long long)readpartinfo->len * readvsinfo->block_size << "," << QString(readpartinfo->desc) << ",0," << readpartinfo->start << "," << (unsigned long long)readpartinfo->len << "," << (int)readvsinfo->block_size << "," << readpartinfo->flags << "," << (unsigned long long)readpartinfo->len << "," << (int)readvsinfo->block_size << ",e" + QString::number(evidcnt) + "-v" + QString::number(volcnt) + "-p" + QString::number(partint);
                    pfile.close();
                }
                else if(readpartinfo->flags == 0x01) // allocated partition
                {
                    readfsinfo = tsk_fs_open_vol(readpartinfo, TSK_FS_TYPE_DETECT);
                    if(readfsinfo != NULL)
                    {
                        out << readfsinfo->ftype << "," << (unsigned long long)readfsinfo->block_size * (unsigned long long)readfsinfo->block_count << "," << GetFileSystemLabel(readfsinfo) << "," << (unsigned long long)readfsinfo->root_inum << "," << (unsigned long long)readfsinfo->offset << "," << (unsigned long long)readfsinfo->block_count << "," << (int)readfsinfo->block_size << "," << readpartinfo->flags << "," << (unsigned long long)readpartinfo->len << "," << (int)readfsinfo->dev_bsize << ",e" + QString::number(evidcnt) + "-v" + QString::number(volcnt) + "-p" + QString::number(partint);
                        pfile.close();
                        uint8_t walkreturn;
                        int walkflags = TSK_FS_DIR_WALK_FLAG_ALLOC | TSK_FS_DIR_WALK_FLAG_UNALLOC | TSK_FS_DIR_WALK_FLAG_RECURSE;
                        walkreturn = tsk_fs_dir_walk(readfsinfo, readfsinfo->root_inum, (TSK_FS_DIR_WALK_FLAG_ENUM)walkflags, FileEntries, NULL);
                        if(walkreturn == 1)
                        {
                            LogMessage("Issues with traversing the file structure were encountered");
                            errorcount++;
                        }
                    }
                }
                //else if(readpartinfo->flags == 0x04) // meta partition
                //pfile.close();
            }
        }
    }
            // finished initial partition/file system information including file info stored into the vector 
            // place InitializeQueryModel(); in here...

}
QString GetFileSystemLabel(TSK_FS_INFO* curinfo)
{
    if(curinfo != NULL)
    {
        if(curinfo->ftype == TSK_FS_TYPE_EXT2 || curinfo->ftype == TSK_FS_TYPE_EXT3 || curinfo->ftype == TSK_FS_TYPE_EXT4 || curinfo->ftype == TSK_FS_TYPE_EXT_DETECT)
        {
            return QString::fromStdString(string(((EXT2FS_INFO*)curinfo)->fs->s_volume_name));
        }
        else if(curinfo->ftype == TSK_FS_TYPE_FFS1 || curinfo->ftype == TSK_FS_TYPE_FFS1B)
        {
            return "UFS1";
        }
        else if(curinfo->ftype == TSK_FS_TYPE_FFS2 || curinfo->ftype == TSK_FS_TYPE_FFS_DETECT)
        {
            return QString::fromUtf8(reinterpret_cast<char*>(((FFS_INFO*)curinfo)->fs.sb2->volname));
        }
        else if(curinfo->ftype == TSK_FS_TYPE_FAT12 || curinfo->ftype == TSK_FS_TYPE_FAT16)
        {
            sprintf(asc, "%c%c%c%c%c%c%c%c%c%c%c", ((FATXXFS_SB*)((FATFS_INFO*)curinfo)->boot_sector_buffer)->a.f16.vol_lab[0], ((FATXXFS_SB*)((FATFS_INFO*)curinfo)->boot_sector_buffer)->a.f16.vol_lab[1], ((FATXXFS_SB*)((FATFS_INFO*)curinfo)->boot_sector_buffer)->a.f16.vol_lab[2], ((FATXXFS_SB*)((FATFS_INFO*)curinfo)->boot_sector_buffer)->a.f16.vol_lab[3], ((FATXXFS_SB*)((FATFS_INFO*)curinfo)->boot_sector_buffer)->a.f16.vol_lab[4], ((FATXXFS_SB*)((FATFS_INFO*)curinfo)->boot_sector_buffer)->a.f16.vol_lab[5], ((FATXXFS_SB*)((FATFS_INFO*)curinfo)->boot_sector_buffer)->a.f16.vol_lab[6], ((FATXXFS_SB*)((FATFS_INFO*)curinfo)->boot_sector_buffer)->a.f16.vol_lab[7], ((FATXXFS_SB*)((FATFS_INFO*)curinfo)->boot_sector_buffer)->a.f16.vol_lab[8], ((FATXXFS_SB*)((FATFS_INFO*)curinfo)->boot_sector_buffer)->a.f16.vol_lab[9], ((FATXXFS_SB*)((FATFS_INFO*)curinfo)->boot_sector_buffer)->a.f16.vol_lab[10]);
            return QString::fromStdString(string(asc));
        }
        else if(curinfo->ftype == TSK_FS_TYPE_FAT32)
        {
            sprintf(asc, "%c%c%c%c%c%c%c%c%c%c%c", ((FATXXFS_SB*)((FATFS_INFO*)curinfo)->boot_sector_buffer)->a.f32.vol_lab[0], ((FATXXFS_SB*)((FATFS_INFO*)curinfo)->boot_sector_buffer)->a.f32.vol_lab[1], ((FATXXFS_SB*)((FATFS_INFO*)curinfo)->boot_sector_buffer)->a.f32.vol_lab[2], ((FATXXFS_SB*)((FATFS_INFO*)curinfo)->boot_sector_buffer)->a.f32.vol_lab[3], ((FATXXFS_SB*)((FATFS_INFO*)curinfo)->boot_sector_buffer)->a.f32.vol_lab[4], ((FATXXFS_SB*)((FATFS_INFO*)curinfo)->boot_sector_buffer)->a.f32.vol_lab[5], ((FATXXFS_SB*)((FATFS_INFO*)curinfo)->boot_sector_buffer)->a.f32.vol_lab[6], ((FATXXFS_SB*)((FATFS_INFO*)curinfo)->boot_sector_buffer)->a.f32.vol_lab[7], ((FATXXFS_SB*)((FATFS_INFO*)curinfo)->boot_sector_buffer)->a.f32.vol_lab[8], ((FATXXFS_SB*)((FATFS_INFO*)curinfo)->boot_sector_buffer)->a.f32.vol_lab[9], ((FATXXFS_SB*)((FATFS_INFO*)curinfo)->boot_sector_buffer)->a.f32.vol_lab[10]);
            return QString::fromStdString(string(asc));
        }
        else if(curinfo->ftype == TSK_FS_TYPE_NTFS)
        {
            TSK_FS_FILE* tmpfile = NULL;
            const TSK_FS_ATTR* tmpattr;
            if((tmpfile = tsk_fs_file_open_meta(curinfo, NULL, NTFS_MFT_VOL)) != NULL)
            {
                tmpattr = tsk_fs_attrlist_get(tmpfile->meta->attr, TSK_FS_ATTR_TYPE_NTFS_VNAME);
                if((tmpattr->flags & TSK_FS_ATTR_RES) && (tmpattr->size))
                {
                    UTF16* name16 = (UTF16*) tmpattr->rd.buf;
                    UTF8* name8 = (UTF8*) asc;
                    int retval;
                    retval = tsk_UTF16toUTF8(curinfo->endian, (const UTF16**)&name16, (UTF16*) ((uintptr_t)name16 + (int) tmpattr->size), &name8, (UTF8*) ((uintptr_t)name8 + sizeof(asc)), TSKlenientConversion);
                    if(retval != TSKconversionOK)
                        *name8 = '\0';
                    else if((uintptr_t)name8 >= (uintptr_t)asc + sizeof(asc))
                        asc[sizeof(asc)-1] = '\0';
                    else
                        *name8 = '\0';
                    return QString::fromStdString(string(asc));
                }
            }
        }
        else if(curinfo->ftype == TSK_FS_TYPE_EXFAT)
        {
            char* databuffer = NULL;
            TSK_DADDR_T cursector = 0;
            TSK_DADDR_T endsector = 0;
            int8_t isallocsec = 0;
            TSK_INUM_T curinum = 0;
            FATFS_DENTRY* dentry = NULL;
            TSK_FS_FILE* tmpfile = NULL;
            ssize_t bytesread = 0;
            if((tmpfile = tsk_fs_file_alloc(curinfo)) != NULL)
            {
                if((tmpfile->meta = tsk_fs_meta_alloc(FATFS_FILE_CONTENT_LEN)) != NULL)
                {
                    if((databuffer = (char*)tsk_malloc(((FATFS_INFO*)curinfo)->ssize)) != NULL)
                    {
                        cursector = ((FATFS_INFO*)curinfo)->rootsect;
                        endsector = (((FATFS_INFO*)curinfo)->firstdatasect + ((FATFS_INFO*)curinfo)->clustcnt * ((FATFS_INFO*)curinfo)->csize) - 1;
                        while(cursector < endsector)
                        {
                        }
                        bytesread = tsk_fs_read_block(curinfo, cursector, databuffer, ((FATFS_INFO*)curinfo)->ssize);
                        if(bytesread == ((FATFS_INFO*)curinfo)->ssize)
                        {
                            isallocsec = fatfs_is_sectalloc(((FATFS_INFO*)curinfo), cursector);
                            if(isallocsec != -1)
                            {
                                curinum = FATFS_SECT_2_INODE(((FATFS_INFO*)curinfo), cursector);
                                for(int i = 0; i < ((FATFS_INFO*)curinfo)->ssize; i+= sizeof(FATFS_DENTRY))
                                {
                                    dentry = (FATFS_DENTRY*)&(databuffer[i]);
                                    if(exfatfs_get_enum_from_type(dentry->data[0]) == EXFATFS_DIR_ENTRY_TYPE_VOLUME_LABEL)
                                    {
                                        if(exfatfs_dinode_copy(((FATFS_INFO*)curinfo), curinum, dentry, isallocsec, tmpfile) == TSK_OK)
                                            return QString::fromStdString(tmpfile->meta->name2->name);
                                    }
                                }
                            }
                        }
                    }
                    tsk_fs_file_close(tmpfile);
                    free(databuffer);
                }
            }
        }
        else if(curinfo->ftype == TSK_FS_TYPE_ISO9660)
        {
            for(p = ((ISO_INFO*)curinfo)->pvd; p != NULL; p = p->next)
            {
                return QString::fromUtf8(reinterpret_cast<char*>(p->pvd.vol_id));
            }
        }
        else if(curinfo->ftype == TSK_FS_TYPE_YAFFS2)
        {
            return "YAFFS2";
        }
        else if(curinfo->ftype == TSK_FS_TYPE_SWAP)
        {
            return "SWAP";
        }
        else if(curinfo->ftype == TSK_FS_TYPE_HFS)
        {
            hfs = (HFS_INFO*)curinfo;
            char fn[HFS_MAXNAMLEN + 1];
            HFS_ENTRY entry;
            if(hfs_cat_file_lookup(hfs, HFS_ROOT_INUM, &entry, FALSE))
            {
                // log error here
            }
            if(hfs_UTF16toUTF8(curinfo, entry.thread.name.unicode, tsk_getu16(curinfo->endian, entry.thread.name.length), fn, HFS_MAXNAMLEN + 1, HFS_U16U8_FLAG_REPLACE_SLASH))
            {
                // log error here
            }
            sprintf(asc, "%s", fn);
            return QString::fromStdString(string(asc));
        }
        return "";
    }
    return "";
}

uint8_t hfs_cat_file_lookup(HFS_INFO* hfs, TSK_INUM_T inum, HFS_ENTRY* entry, unsigned char follow_hard_link)
{
    TSK_FS_INFO *fs = (TSK_FS_INFO *) & (hfs->fs_info);
    hfs_btree_key_cat key;      /* current catalog key */
    hfs_thread thread;          /* thread record */
    hfs_file_folder record;     /* file/folder record */
    TSK_OFF_T off;

    // Test if this is a special file that is not located in the catalog
    if ((inum == HFS_EXTENTS_FILE_ID) ||
        (inum == HFS_CATALOG_FILE_ID) ||
        (inum == HFS_ALLOCATION_FILE_ID) ||
        (inum == HFS_STARTUP_FILE_ID) ||
        (inum == HFS_ATTRIBUTES_FILE_ID)) {
        return 1;
    }


    /* first look up the thread record for the item we're searching for */

    /* set up the thread record key */
    memset((char *) &key, 0, sizeof(hfs_btree_key_cat));
    cnid_to_array((uint32_t) inum, key.parent_cnid);

    /* look up the thread record */
    off = hfs_cat_get_record_offset(hfs, &key);
    if (off == 0)
    {
        // put error code here...
        return 1;
    }

    /* read the thread record */
    if (hfs_cat_read_thread_record(hfs, off, &thread))
    {
        //tsk_error_set_errstr2(" hfs_cat_file_lookup: file (%" PRIuINUM ")", inum);
        return 1;
    }

    /* now look up the actual file/folder record */

    memset((char *) &key, 0, sizeof(hfs_btree_key_cat));
    memset((char *) &key, 0, sizeof(hfs_btree_key_cat));
    memcpy((char *) key.parent_cnid, (char *) thread.parent_cnid,
    sizeof(key.parent_cnid));
    memcpy((char *) &key.name, (char *) &thread.name, sizeof(key.name));

    /* look up the record */
    off = hfs_cat_get_record_offset(hfs, &key);
    if (off == 0)
    {
        // print error here
        return 1;
    }

    /* read the record */
    if (hfs_cat_read_file_folder_record(hfs, off, &record))
    {
        //tsk_error_set_errstr2(" hfs_cat_file_lookup: file (%" PRIuINUM ")", inum);
        return 1;
    }

    /* these memcpy can be gotten rid of, really */
    if (tsk_getu16(fs->endian, record.file.std.rec_type) == HFS_FOLDER_RECORD) {
        memcpy((char *) &entry->cat, (char *) &record, sizeof(hfs_folder));
    }
    else if (tsk_getu16(fs->endian, record.file.std.rec_type) == HFS_FILE_RECORD) {
        memcpy((char *) &entry->cat, (char *) &record, sizeof(hfs_file));
    }
    /* other cases already caught by hfs_cat_read_file_folder_record */

    memcpy((char *) &entry->thread, (char *) &thread, sizeof(hfs_thread));

    entry->flags = TSK_FS_META_FLAG_ALLOC | TSK_FS_META_FLAG_USED;
    entry->inum = inum;

    if (follow_hard_link) {
        // TEST to see if this is a hard link
        unsigned char is_err;
        TSK_INUM_T target_cnid =
            hfs_follow_hard_link(hfs, &(entry->cat), &is_err);
        if (is_err > 1) {
            /*
            error_returned
            ("hfs_cat_file_lookup: error occurred while following a possible hard link for "
            "inum (cnid) =  %" PRIuINUM, inum);
            */
            return 1;
        }
        if (target_cnid != inum) {
            // This is a hard link, and we have got the cnid of the target file, so look it up.
            uint8_t res =
                hfs_cat_file_lookup(hfs, target_cnid, entry, FALSE);
            if (res != 0) {
                /*
                error_returned
                ("hfs_cat_file_lookup: error occurred while looking up the Catalog entry for "
                "the target of inum (cnid) = %" PRIuINUM " target",
                inum);
                */
            }
            return 1;
        }

        // Target is NOT a hard link, so fall through to the non-hard link exit.
    }

    return 0;
}

uint8_t hfs_UTF16toUTF8(TSK_FS_INFO* fs, uint8_t* uni, int ulen, char* asc, int alen, uint32_t flags)
{
    UTF8 *ptr8;
    uint8_t *uniclean;
    UTF16 *ptr16;
    int i;
    TSKConversionResult r;

    // remove nulls from the Unicode string
    // convert / to :
    uniclean = (uint8_t *) tsk_malloc(ulen * 2);
    if (!uniclean)
        return 1;

    memcpy(uniclean, uni, ulen * 2);

    for (i = 0; i < ulen; ++i) {
        uint16_t uc = tsk_getu16(fs->endian, uniclean + i * 2);


        int changed = 0;
        if (uc == UTF16_NULL) {
            uc = UTF16_NULL_REPLACE;
            changed = 1;
        }
        else if ((flags & HFS_U16U8_FLAG_REPLACE_SLASH)
            && uc == UTF16_SLASH) {
            uc = UTF16_COLON;
            changed = 1;
        }

        else if ((flags & HFS_U16U8_FLAG_REPLACE_CONTROL)
            && uc < UTF16_LEAST_PRINTABLE) {
            uc = (uint16_t) UTF16_NULL_REPLACE;
            changed = 1;
        }

        if (changed)
            *((uint16_t *) (uniclean + i * 2)) =
                tsk_getu16(fs->endian, (uint8_t *) & uc);
    }

    // convert to UTF-8
    memset(asc, 0, alen);

    ptr8 = (UTF8 *) asc;
    ptr16 = (UTF16 *) uniclean;
    r = tsk_UTF16toUTF8(fs->endian, (const UTF16 **) &ptr16,
    (const UTF16 *) (&uniclean[ulen * 2]), &ptr8,
    (UTF8 *) & asc[alen], TSKstrictConversion);

    free(uniclean);
    if (r != TSKconversionOK) {
        tsk_error_set_errno(TSK_ERR_FS_UNICODE);
        tsk_error_set_errstr
            ("hfs_UTF16toUTF8: unicode conversion failed (%d)", (int) r);
        return 1;
    }

    return 0;
}
    static uint8_t hfs_cat_get_record_offset_cb(HFS_INFO* hfs, int8_t level_type, const void* targ_data, const hfs_btree_key_cat* cur_key, TSK_OFF_T key_off, void* ptr)
    {
        const hfs_btree_key_cat *targ_key = (hfs_btree_key_cat *) targ_data;
        //if (tsk_verbose)
        //    tsk_fprintf(stderr,
        //        "hfs_cat_get_record_offset_cb: %s node want: %" PRIu32
        //        " vs have: %" PRIu32 "\n",
        //        (level_type == HFS_BT_NODE_TYPE_IDX) ? "Index" : "Leaf",
        //        tsk_getu32(hfs->fs_info.endian, targ_key->parent_cnid),
        //        tsk_getu32(hfs->fs_info.endian, cur_key->parent_cnid));

        if (level_type == HFS_BT_NODE_TYPE_IDX) {
            int diff = hfs_cat_compare_keys(hfs, cur_key, targ_key);
            if (diff < 0)
                return HFS_BTREE_CB_IDX_LT;
            else
                return HFS_BTREE_CB_IDX_EQGT;
        }
        else {
            int diff = hfs_cat_compare_keys(hfs, cur_key, targ_key);

            // see if this record is for our file or if we passed the interesting entries
            if (diff < 0) {
                return HFS_BTREE_CB_LEAF_GO;
            }
            else if (diff == 0) {
                TSK_OFF_T *off = (TSK_OFF_T *) ptr;
                *off =
                    key_off + 2 + tsk_getu16(hfs->fs_info.endian,
                    cur_key->key_len);
            }
            return HFS_BTREE_CB_LEAF_STOP;
        }   
    }
    static TSK_OFF_T hfs_cat_get_record_offset(HFS_INFO* hfs, const hfs_btree_key_cat* needle)
    {
        TSK_OFF_T off = 0;
        if(hfs_cat_traverse(hfs, needle, hfs_cat_get_record_offset_cb, &off))
        {
            return 0;
        }
        return off;
    }
    uint8_t hfs_cat_read_thread_record(HFS_INFO* hfs, TSK_OFF_T off, hfs_thread* thread)
    {
        TSK_FS_INFO *fs = (TSK_FS_INFO *) & (hfs->fs_info);
        uint16_t uni_len;
        size_t cnt;

        memset(thread, 0, sizeof(hfs_thread));
        cnt = tsk_fs_attr_read(hfs->catalog_attr, off, (char *) thread, 10, (TSK_FS_FILE_READ_FLAG_ENUM)0);
        if (cnt != 10) {
            //tsk_error_reset();
            //tsk_error_set_errno(TSK_ERR_FS_READ);
            //tsk_error_set_errstr2("hfs_cat_read_thread_record: Error reading catalog offset %"PRIuOFF " (header)", off);
            return 1;
        }

        if ((tsk_getu16(fs->endian, thread->rec_type) != HFS_FOLDER_THREAD)
            && (tsk_getu16(fs->endian, thread->rec_type) != HFS_FILE_THREAD)) {
            //tsk_error_set_errno(TSK_ERR_FS_GENFS);
            //tsk_error_set_errstr("hfs_cat_read_thread_record: unexpected record type %" PRIu16,tsk_getu16(fs->endian, thread->rec_type));
            return 1;
        }

        uni_len = tsk_getu16(fs->endian, thread->name.length);

        if (uni_len > 255) {
            //tsk_error_set_errno(TSK_ERR_FS_INODE_COR);
            //tsk_error_set_errstr
            //("hfs_cat_read_thread_record: invalid string length (%" PRIu16
            //")", uni_len);
            return 1;
        }

        cnt =
            tsk_fs_attr_read(hfs->catalog_attr, off + 10, (char *) thread->name.unicode, uni_len * 2, (TSK_FS_FILE_READ_FLAG_ENUM)0);
        if (cnt != uni_len * 2) {
                //tsk_error_reset();
            //tsk_error_set_errno(TSK_ERR_FS_READ);
            //tsk_error_set_errstr2
            //("hfs_cat_read_thread_record: Error reading catalog offset %"
            //PRIuOFF " (name)", off + 10);
            return 1;
        }

        return 0;
    }
    uint8_t hfs_cat_read_file_folder_record(HFS_INFO* hfs, TSK_OFF_T off, hfs_file_folder* record)
    {
        TSK_FS_INFO *fs = (TSK_FS_INFO *) & (hfs->fs_info);
        size_t cnt;
        char rec_type[2];

        memset(record, 0, sizeof(hfs_file_folder));

        cnt = tsk_fs_attr_read(hfs->catalog_attr, off, rec_type, 2, (TSK_FS_FILE_READ_FLAG_ENUM)0);
        if (cnt != 2) {
            //tsk_error_reset();
            //tsk_error_set_errno(TSK_ERR_FS_READ);
            //tsk_error_set_errstr2
            //("hfs_cat_read_file_folder_record: Error reading record type from catalog offset %"
            //PRIuOFF " (header)", off);
            return 1;
        }

        if (tsk_getu16(fs->endian, rec_type) == HFS_FOLDER_RECORD) {
            cnt = 
                tsk_fs_attr_read(hfs->catalog_attr, off, (char *) record, sizeof(hfs_folder), (TSK_FS_FILE_READ_FLAG_ENUM)0);
            if (cnt != sizeof(hfs_folder)) {
                //tsk_error_reset();
                //tsk_error_set_errno(TSK_ERR_FS_READ);
                //tsk_error_set_errstr2
                //   ("hfs_cat_read_file_folder_record: Error reading catalog offset %"
                //    PRIuOFF " (folder)", off);
                return 1;
            }
        }
        else if (tsk_getu16(fs->endian, rec_type) == HFS_FILE_RECORD) {
            cnt =
                tsk_fs_attr_read(hfs->catalog_attr, off, (char *) record, sizeof(hfs_file), (TSK_FS_FILE_READ_FLAG_ENUM)0);
            if (cnt != sizeof(hfs_file)) {
                //tsk_error_reset();
                //tsk_error_set_errno(TSK_ERR_FS_READ);
            //tsk_error_set_errstr2
            //    ("hfs_cat_read_file_folder_record: Error reading catalog offset %"
            //    PRIuOFF " (file)", off);
            return 1;
            }
        }
        else {
            //tsk_error_set_errno(TSK_ERR_FS_GENFS);
            //tsk_error_set_errstr
            //    ("hfs_cat_read_file_folder_record: unexpected record type %"
            //    PRIu16, tsk_getu16(fs->endian, rec_type));
            return 1;
        }

        return 0;
    }
    static int hfs_cat_compare_keys(HFS_INFO* hfs, const hfs_btree_key_cat* key1, const hfs_btree_key_cat* key2)
    {
        TSK_FS_INFO *fs = (TSK_FS_INFO *) & (hfs->fs_info);
        uint32_t cnid1, cnid2;

        cnid1 = tsk_getu32(fs->endian, key1->parent_cnid);
        cnid2 = tsk_getu32(fs->endian, key2->parent_cnid);

        if (cnid1 < cnid2)
            return -1;
        if (cnid1 > cnid2)
            return 1;

        return hfs_unicode_compare(hfs, &key1->name, &key2->name);

    }
    uint8_t hfs_cat_traverse(HFS_INFO* hfs, const void* targ_data, TSK_HFS_BTREE_CB a_cb, void* ptr)
    {
        TSK_FS_INFO *fs = &(hfs->fs_info);
        uint32_t cur_node;          /* node id of the current node */
        char *node;

        uint16_t nodesize;
        uint8_t is_done = 0;

        //tsk_error_reset();

        nodesize = tsk_getu16(fs->endian, hfs->catalog_header.nodesize);
        if ((node = (char *) tsk_malloc(nodesize)) == NULL)
            return 1;

        /* start at root node */
        cur_node = tsk_getu32(fs->endian, hfs->catalog_header.rootNode);

        /* if the root node is zero, then the extents btree is empty */
        /* if no files have overflow extents, the Extents B-tree still
           exists on disk, but is an empty B-tree containing only
           the header node */
        if (cur_node == 0) {
            //if (tsk_verbose)
            //    tsk_fprintf(stderr, "hfs_cat_traverse: "
            //        "empty extents btree\n");
            free(node);
            return 1;
        }

        //if (tsk_verbose)
        //    tsk_fprintf(stderr, "hfs_cat_traverse: starting at "
        //        "root node %" PRIu32 "; nodesize = %"
        //        PRIu16 "\n", cur_node, nodesize);

        /* Recurse down to the needed leaf nodes and then go forward */
        is_done = 0;
        while (is_done == 0) {
            TSK_OFF_T cur_off;      /* start address of cur_node */
            uint16_t num_rec;       /* number of records in this node */
            ssize_t cnt;
            hfs_btree_node *node_desc;

            // sanity check 
            if (cur_node > tsk_getu32(fs->endian,
                    hfs->catalog_header.totalNodes)) {
         //       tsk_error_set_errno(TSK_ERR_FS_GENFS);
         //       tsk_error_set_errstr
         //           ("hfs_cat_traverse: Node %d too large for file", cur_node);
                free(node);
                return 1;
            }

            // read the current node
            cur_off = cur_node * nodesize;
            cnt = tsk_fs_attr_read(hfs->catalog_attr, cur_off, node, nodesize, (TSK_FS_FILE_READ_FLAG_ENUM)0);
            if (cnt != nodesize) {
                if (cnt >= 0) {
         //           tsk_error_reset();
         //           tsk_error_set_errno(TSK_ERR_FS_READ);
                }
         //       tsk_error_set_errstr2
         //           ("hfs_cat_traverse: Error reading node %d at offset %"
         //           PRIuOFF, cur_node, cur_off);
                free(node);
                return 1;
            }

            // process the header / descriptor
            node_desc = (hfs_btree_node *) node;
            num_rec = tsk_getu16(fs->endian, node_desc->num_rec);

         //   if (tsk_verbose)
         //       tsk_fprintf(stderr, "hfs_cat_traverse: node %" PRIu32
         //           " @ %" PRIu64 " has %" PRIu16 " records\n",
         //           cur_node, cur_off, num_rec);

            if (num_rec == 0) {
         //       tsk_error_set_errno(TSK_ERR_FS_GENFS);
         //       tsk_error_set_errstr("hfs_cat_traverse: zero records in node %"
         //           PRIu32, cur_node);
                free(node);
                return 1;
            }

            /* With an index node, find the record with the largest key that is smaller
             * to or equal to cnid */
            if (node_desc->type == HFS_BT_NODE_TYPE_IDX) {
                uint32_t next_node = 0;
                int rec;

                for (rec = 0; rec < num_rec; rec++) {
                    size_t rec_off;
                    hfs_btree_key_cat *key;
                    uint8_t retval;
    
                    // get the record offset in the node
                    rec_off =
                        tsk_getu16(fs->endian,
                        &node[nodesize - (rec + 1) * 2]);
                    if (rec_off > nodesize) {
           //             tsk_error_set_errno(TSK_ERR_FS_GENFS);
           //             tsk_error_set_errstr
           //                 ("hfs_cat_traverse: offset of record %d in index node %d too large (%d vs %"
           //                 PRIu16 ")", rec, cur_node, (int) rec_off,
           //                 nodesize);
                        free(node);
                        return 1;
                    }
                    key = (hfs_btree_key_cat *) & node[rec_off];

                    /*
                       if (tsk_verbose)
                       tsk_fprintf(stderr,
                       "hfs_cat_traverse: record %" PRIu16
                       " ; keylen %" PRIu16 " (%" PRIu32 ")\n", rec,
                       tsk_getu16(fs->endian, key->key_len),
                       tsk_getu32(fs->endian, key->parent_cnid));
                     */

                    /* save the info from this record unless it is too big */
                    retval =
                        a_cb(hfs, HFS_BT_NODE_TYPE_IDX, targ_data, key,
                        cur_off + rec_off, ptr);
                    if (retval == HFS_BTREE_CB_ERR) {
         //               tsk_error_set_errno(TSK_ERR_FS_GENFS);
         //               tsk_error_set_errstr2
         //                   ("hfs_cat_traverse: Callback returned error");
                        free(node);
                        return 1;
                    }
                    // record the closest entry
                    else if ((retval == HFS_BTREE_CB_IDX_LT)
                        || (next_node == 0)) {
                        hfs_btree_index_record *idx_rec;
                        int keylen =
                            2 + hfs_get_idxkeylen(hfs, tsk_getu16(fs->endian,
                                key->key_len), &(hfs->catalog_header));
                        if (rec_off + keylen > nodesize) {
         //                   tsk_error_set_errno(TSK_ERR_FS_GENFS);
         //                   tsk_error_set_errstr
         //                       ("hfs_cat_traverse: offset of record and keylength %d in index node %d too large (%d vs %"
         //                       PRIu16 ")", rec, cur_node,
         //                       (int) rec_off + keylen, nodesize);
                            free(node);
                            return 1;
                        }
                        idx_rec =
                            (hfs_btree_index_record *) & node[rec_off +
                            keylen];
                        next_node = tsk_getu32(fs->endian, idx_rec->childNode);
                    }
                    if (retval == HFS_BTREE_CB_IDX_EQGT) {
                        // move down to the next node
                        break;
                    }
                }
                // check if we found a relevant node
                if (next_node == 0) {
         //           tsk_error_set_errno(TSK_ERR_FS_GENFS);
         //           tsk_error_set_errstr
         //               ("hfs_cat_traverse: did not find any keys in index node %d",
         //               cur_node);
                    is_done = 1;
                    break;
                }
                cur_node = next_node;
            }

            /* With a leaf, we look for the specific record. */
            else if (node_desc->type == HFS_BT_NODE_TYPE_LEAF) {
                int rec;

            for (rec = 0; rec < num_rec; rec++) {
                size_t rec_off;
                hfs_btree_key_cat *key;
                uint8_t retval;

                // get the record offset in the node
                rec_off =
                    tsk_getu16(fs->endian,
                    &node[nodesize - (rec + 1) * 2]);
                if (rec_off > nodesize) {
      //              tsk_error_set_errno(TSK_ERR_FS_GENFS);
      //              tsk_error_set_errstr
      //                  ("hfs_cat_traverse: offset of record %d in leaf node %d too large (%d vs %"
      //                  PRIu16 ")", rec, cur_node, (int) rec_off,
      //                  nodesize);
                    free(node);
                    return 1;
                }
                key = (hfs_btree_key_cat *) & node[rec_off];

                /*
                   if (tsk_verbose)
                   tsk_fprintf(stderr,
                   "hfs_cat_traverse: record %" PRIu16
                   "; keylen %" PRIu16 " (%" PRIu32 ")\n", rec,
                   tsk_getu16(fs->endian, key->key_len),
                   tsk_getu32(fs->endian, key->parent_cnid));
                 */
                //                rec_cnid = tsk_getu32(fs->endian, key->file_id);

                retval =
                    a_cb(hfs, HFS_BT_NODE_TYPE_LEAF, targ_data, key,
                    cur_off + rec_off, ptr);
                if (retval == HFS_BTREE_CB_LEAF_STOP) {
                    is_done = 1;
                    break;
                }
                else if (retval == HFS_BTREE_CB_ERR) {
      //              tsk_error_set_errno(TSK_ERR_FS_GENFS);
      //              tsk_error_set_errstr2
      //                  ("hfs_cat_traverse: Callback returned error");
                    free(node);
                    return 1;
                }
            }

            // move right to the next node if we got this far
            if (is_done == 0) {
                cur_node = tsk_getu32(fs->endian, node_desc->flink);
                if (cur_node == 0) {
                    is_done = 1;
                }
      //          if (tsk_verbose)
      //              tsk_fprintf(stderr,
      //                  "hfs_cat_traverse: moving forward to next leaf");
              }
            }
            else {
          //      tsk_error_set_errno(TSK_ERR_FS_GENFS);
          //      tsk_error_set_errstr("hfs_cat_traverse: btree node %" PRIu32
          //          " (%" PRIu64 ") is neither index nor leaf (%" PRIu8 ")",
          //          cur_node, cur_off, node_desc->type);
                free(node);
                return 1;
            }
        }
        free(node);
        return 0;
    }
