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
/*
void LogMessage(QString logmsg)
{
    QByteArray ba;
    ba.clear();
    QString tmpstring = QDateTime::currentDateTime().toString(QString("MM/dd/yyyy hh:mm:ss t"));
    ba.append(tmpstring + "\t" + logmsg  +"\n");
    msglog->append(QString(tmpstring + ": " + logmsg));
    logfile.open(QIODevice::ReadWrite | QIODevice::Append | QIODevice::Text);
    logfile.write(ba);
    logfile.write(QString(tmpstring + "\t" + logmsg + "\n").toStdString().c_str());
    logfile.close();
}
*/

qint64 GetChildCount(QString filefilter)
{
    qint64 tmpcount = 0;
    QDir eviddir = QDir(wombatvariable.tmpmntpath);
    QStringList files = eviddir.entryList(QStringList(filefilter), QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot | QDir::NoSymLinks);
    tmpcount = files.count();
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
        *(QString*)ptr += QString::number(addr) + "^^";
        //blockstring += QString::number(addr) + "^^";
    }
    else if(tmpfile->fs_info->ftype == TSK_FS_TYPE_ISO9660_DETECT)
    {
        // iso is already done in the previous function
    }
    else if(tmpfile->fs_info->ftype == TSK_FS_TYPE_FAT_DETECT || tmpfile->fs_info->ftype == TSK_FS_TYPE_NTFS_DETECT)
    {
        if(tmpfile->name != NULL)
        {
            if((strcmp(tmpfile->name->name, "$FAT1") == 0) || (strcmp(tmpfile->name->name, "$FAT2") == 0) || (strcmp(tmpfile->name->name, "$MBR") == 0) || (strcmp(tmpfile->name->name, "$OrphanFiles") == 0))
               *(QString*)ptr += QString::number(addr) + "^^";
            else
                *(QString*)ptr += QString::number(addr) + "^^";
        }
        else
            *(QString*)ptr += QString::number(addr) + "^^";
    }
    else if(tmpfile->fs_info->ftype == TSK_FS_TYPE_YAFFS2_DETECT)
    {
        if(flags & TSK_FS_BLOCK_FLAG_CONT)
        {
            *(QString*)ptr += QString::number(addr) + "^^";
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
                    *(QString*)ptr += QString::number(addr + i) + "^^";
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
    if(tmpmeta->mode & TSK_FS_META_MODE_ISGID)
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

void BuildStatFile(TSK_FS_FILE* tmpfile, const char* tmppath, AddEvidenceVariable* aevar)
{
    qint64 curaddress = 0;
    qint64 paraddress = 0;
    QString parentstr = "";
    QList<QVariant> nodedata;
    nodedata.clear();
    QString outstring = "";
    QStringList treeout;
    treeout.clear();
    QByteArray ba2;
    ba2.append(QString("/" + QString(tmppath)));
    QByteArray ba;
    ba.append(QString(tmpfile->name->name));
    outstring += ba.toBase64() + "," + QString::number(tmpfile->name->type) + "," + QString::number(tmpfile->name->par_addr) + ",";
    treeout << ba.toBase64();
    if(tmpfile->name->par_addr == tmpfile->fs_info->root_inum)
        parentstr = "e" + QString::number(aevar->evidcnt) + "-v" + QString::number(aevar->volcnt) + "-p" + QString::number(aevar->partint);
    else
        parentstr = "e" + QString::number(aevar->evidcnt) + "-v" + QString::number(aevar->volcnt) + "-p" + QString::number(aevar->partint) + "-f" + QString::number(tmpfile->name->par_addr);
    curaddress = tmpfile->name->meta_addr;
    paraddress = tmpfile->name->par_addr;
    outstring += ba2.toBase64() + ",";
    treeout << ba2.toBase64();
    if(tmpfile->meta != NULL)
    {
        outstring += QString::number(tmpfile->meta->atime) + "," + QString::number(tmpfile->meta->ctime) + "," + QString::number(tmpfile->meta->crtime) + "," + QString::number(tmpfile->meta->mtime) + "," + QString::number(tmpfile->meta->size) + "," + QString::number(tmpfile->meta->addr) + ",";
        treeout << QString::number(tmpfile->meta->size) << QString::number(tmpfile->meta->crtime) << QString::number(tmpfile->meta->mtime) << QString::number(tmpfile->meta->atime) << QString::number(tmpfile->meta->ctime);
    }
    else
    {
        outstring += "0,0,0,0,0," + QString::number(tmpfile->name->meta_addr) + ",";
        treeout << "0" << "0" << "0" << "0" << "0";
    }

    //char* magicbuffer = reinterpret_cast<char*>(malloc(1024));
    char magicbuffer[1024];
    //char* magicbuffer = new char[1024];
    QByteArray tmparray("intro");
    tmparray.clear();
    tsk_fs_file_read(tmpfile, 0, magicbuffer, 1024, TSK_FS_FILE_READ_FLAG_NONE);
    tmparray = QByteArray::fromRawData(magicbuffer, 1024);
    QMimeDatabase mimedb;
    const QMimeType mimetype = mimedb.mimeTypeForData(tmparray);
    //delete[] magicbuffer;
    //free(magicbuffer);
    outstring += mimetype.name() + ",0,e" + QString::number(aevar->evidcnt) + "-v" + QString::number(aevar->volcnt) + "-p" + QString::number(aevar->partint) + "-f";
    if(tmpfile->name->meta_addr == 0 && strcmp(tmpfile->name->name, "$MFT") != 0)
        outstring += "*" + QString::number(orphancount);
    else
        outstring += QString::number(tmpfile->name->meta_addr);
    outstring += "-a" + QString::number(tmpfile->name->par_addr);
    /* hash method using TSK */
/*    TSK_FS_HASH_RESULTS hashresults;
    uint8_t retval = tsk_fs_file_hash_calc(tmpfile, &hashresults, TSK_BASE_HASH_MD5);
    if(retval == 0)
    {
        char sbuf[17];
        int hashint;
        hashint = 0;
        if(hashint < 0)
        {
        }
        for(int i=0; i < 16; i++)
        {
            hashint = sprintf(sbuf+(2*i), "%02X", hashresults.md5_digest[i]);
        }
        outstring +=  "," + QString(sbuf);
        treeout << QString(sbuf);
    }
    else
    {
        outstring += ",0";
        treeout << "0";
    }
    */
    outstring += ",0";
    treeout << "0";
    treeout << mimetype.name().split("/").at(0) << mimetype.name().split("/").at(1);
    treeout << "0"; // empty bookmark value
    // PUT ID INFO HERE FOR NAME IN FIRST COLUMN
    if(tmpfile->name->meta_addr == 0 && strcmp(tmpfile->name->name, "$MFT") != 0)
        treeout << "e" + QString::number(aevar->evidcnt) + "-v" + QString::number(aevar->volcnt) + "-p" + QString::number(aevar->partint) + "-f*" + QString::number(orphancount) + "-a" + QString::number(tmpfile->name->par_addr);
    else
        treeout << "e" + QString::number(aevar->evidcnt) + "-v" + QString::number(aevar->volcnt) + "-p" + QString::number(aevar->partint) + "-f" + QString::number(tmpfile->name->meta_addr) + "-a" + QString::number(tmpfile->name->par_addr);
    if(tmpfile->meta != NULL)
        treeout << QString::number(tmpfile->meta->type);
    else
        treeout << QString::number(tmpfile->name->type);

    if(tmpfile->name->meta_addr == 0 && strcmp(tmpfile->name->name, "$MFT") != 0)
    {
        outstring += ",1";
        treeout << "1";
    }
    else
    {
        if(tmpfile->meta != NULL)
        {
            if(((tmpfile->meta->flags & TSK_FS_META_FLAG_UNALLOC) == 2) && ((tmpfile->meta->flags & TSK_FS_META_FLAG_USED) == 4))
            {
                outstring += ",1";
                treeout << "1";
            }
            else
            {
                    outstring += ",0";
                    treeout << "0";
            }
        }
        else
        {
            if((tmpfile->name->flags & TSK_FS_NAME_FLAG_UNALLOC) == 0x02)
            {
                outstring += ",1";
                treeout << "1";
            }
            else
            {
                outstring += ",0";
                treeout << "0";
            }
        }
    }
    outstring += ",0"; // empty bookmark value
    QFile filefile;
    QTextStream out(&filefile);
    if(tmpfile->name->meta_addr == 0 && strcmp(tmpfile->name->name, "$MFT") != 0)
        filefile.setFileName(aevar->partitionpath + "f*" + QString::number(orphancount) + ".a" + QString::number(paraddress) + ".stat");
    else
        filefile.setFileName(aevar->partitionpath + "f" + QString::number(tmpfile->name->meta_addr) + ".a" + QString::number(paraddress) + ".stat");
    filefile.open(QIODevice::Append | QIODevice::Text);
    out << outstring;
    out.flush();
    if(filefile.isOpen())
        filefile.close();
    else
        qDebug() << "file file failed to open.";
    nodedata.clear();
    for(int i=0; i < 12; i++)
        nodedata << treeout.at(i);
    mutex.lock();
    treenodemodel->AddNode(nodedata, parentstr, treeout.at(12).toInt(), treeout.at(13).toInt());
    mutex.unlock();
    listeditems.append(treeout.at(11));
    if(tmpfile->name != NULL)
    {
        if(!TSK_FS_ISDOT(tmpfile->name->name))
            filesfound++;
    }
    else
        qDebug() << "tmpfile->name is suddenly null";
    isignals->ProgUpd();

    if(tmpfile->fs_info->ftype == TSK_FS_TYPE_NTFS_DETECT)
    {
        QByteArray adsba;
        qint64 adssize = 0;
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
        adssize = (qint64)teststart;
        int cnt, i;
        cnt = tsk_fs_file_attr_getsize(tmpfile);
        for(i = 0; i < cnt; i++)
        {
            char type[512];
            const TSK_FS_ATTR* fsattr = tsk_fs_file_attr_get_idx(tmpfile, i);
            adssize += 24;
            adssize += (qint64)fsattr->size;
            if(ntfs_attrname_lookup(tmpfile->fs_info, fsattr->type, type, 512) == 0)
            {
                if(QString::compare(QString(type), "$DATA", Qt::CaseSensitive) == 0)
                {
                    if(QString::compare(QString(fsattr->name), "") != 0 && QString::compare(QString(fsattr->name), "$I30", Qt::CaseSensitive) != 0)
                    {
                        //char* fbuf = new char[fsattr->size];
                        char fbuf[fsattr->size];
                        ssize_t flen = tsk_fs_attr_read(fsattr, 0, fbuf, fsattr->size, TSK_FS_FILE_READ_FLAG_NONE);
                        QByteArray fdata = QByteArray::fromRawData(fbuf, flen);
                        QMimeDatabase adsmimedb;
                        QMimeType adsmimetype = mimedb.mimeTypeForData(fdata);
                        //delete[] fbuf;
                        QFile adsfile(aevar->partitionpath + "f" + QString::number(curaddress) + "-" + QString::number(fsattr->id)  + ".a" + QString::number(curaddress) + ".stat");
                        adsfile.open(QIODevice::Append | QIODevice::Text);
                        QTextStream adsout(&adsfile);
                        adsba.append(QString(tmpfile->name->name) + QString(":") + QString(fsattr->name));
                        adsout << adsba.toBase64() << "," << tmpfile->name->type << "," << tmpfile->meta->addr << "," << ba2.toBase64() << ",0, 0, 0, 0," << fsattr->size << "," << adssize - (qint64)fsattr->size + 16 << "," << adsmimetype.name() << "," << QString::number(fsattr->id) << ",e" + QString::number(aevar->evidcnt) + "-v" + QString::number(aevar->volcnt) + "-p" + QString::number(aevar->partint) + "-f" + QString::number(tmpfile->name->meta_addr) + ":" + QString::number(fsattr->id) + "-a" + QString::number(tmpfile->name->meta_addr) << ",0,0,0";
                        adsout.flush();
                        if(adsfile.isOpen())
                            adsfile.close();
                        else
                            qDebug() << "ads file failed to open.";
                        treeout.clear();
                        treeout << adsba.toBase64() << ba2.toBase64() << QString::number(fsattr->size) << "0" << "0" << "0" << "0" << "0" << adsmimetype.name().split("/").at(0) << adsmimetype.name().split("/").at(1) << "0" << QString("e" + QString::number(aevar->evidcnt) + "-v" + QString::number(aevar->volcnt) + "-p" + QString::number(aevar->partint) + "-f" + QString::number(tmpfile->name->meta_addr) + ":" + QString::number(fsattr->id) + "-a" + QString::number(tmpfile->name->meta_addr)) << "10" << "0"; // NAME IN FIRST COLUMN
                        nodedata.clear();
                        for(int i=0;  i < 12; i++)
                            nodedata << treeout.at(i);
                        mutex.lock();
                        treenodemodel->AddNode(nodedata, QString("e" + QString::number(aevar->evidcnt) + "-v" + QString::number(aevar->volcnt) + "-p" + QString::number(aevar->partint) + "-f" + QString::number(tmpfile->name->meta_addr)), treeout.at(12).toInt(), treeout.at(13).toInt());
                        mutex.unlock();
                        listeditems.append(treeout.at(11)); // UNCOMMENT ONCE I CAN CAPTURE ADS IN GEN HASH AND IMG THUMB
                        filesfound++;
                        isignals->ProgUpd();
                        WriteAlternateDataStreamProperties(tmpfile, QString(tmpfile->name->name) + QString(":") + QString(fsattr->name), (qint64)(adssize - fsattr->size + 16), QString::number(fsattr->id), aevar);
                    }
                }
            }
        }
    }
}

TSK_WALK_RET_ENUM TreeEntries(TSK_FS_FILE* tmpfile, const char* tmppath, void* tmpptr)
{
    if(tmpfile->name != NULL)
    {
        if(strcmp(tmpfile->name->name, ".") != 0)
        {
            if(strcmp(tmpfile->name->name, "..") != 0)
            {
                QStringList treeout;
                treeout.clear();
                QFile filefile;
                QString tmpstr = "";
                QStringList tmplist;
                tmplist.clear();
                QList<QVariant> nodedata;
                nodedata.clear();
                //QString evidid = ((AddEvidenceVariable*)tmpptr)->partitionpath.split("
                QString partitionpath = ((AddEvidenceVariable*)tmpptr)->partitionpath;
                QByteArray ba2;
                ba2.append(QString("/" + QString(tmppath)));
                QByteArray ba;
                ba.append(QString(tmpfile->name->name));
                treeout << ba.toBase64();
                QString parentstr = "";
                //unsigned int rootinum = tmpfile->fs_info->root_inum;
                if(tmpfile->name->par_addr == tmpfile->fs_info->root_inum)
                    parentstr = "e" + QString::number(((AddEvidenceVariable*)tmpptr)->evidcnt) + "-v" + QString::number(((AddEvidenceVariable*)tmpptr)->volcnt) + "-p" + QString::number(((AddEvidenceVariable*)tmpptr)->partint);
                else
                    parentstr = "e" + QString::number(((AddEvidenceVariable*)tmpptr)->evidcnt) + "-v" + QString::number(((AddEvidenceVariable*)tmpptr)->volcnt) + "-p" + QString::number(((AddEvidenceVariable*)tmpptr)->partint) + "-f" + QString::number(tmpfile->name->par_addr);
                //curaddress = tmpfile->name->meta_addr;
                //paraddress = tmpfile->name->par_addr;
                treeout << ba2.toBase64();
                if(tmpfile->meta != NULL)
                {
                    treeout << QString::number(tmpfile->meta->size) << QString::number(tmpfile->meta->crtime) << QString::number(tmpfile->meta->mtime) << QString::number(tmpfile->meta->atime) << QString::number(tmpfile->meta->ctime);
                }
                else
                {
                    treeout << "0" << "0" << "0" << "0" << "0";
                }
                char magicbuffer[1024];
                //char* magicbuffer = new char[1024];
                //char* magicbuffer = reinterpret_cast<char*>(malloc(1024));
                //char* fbuf = new char[fsattr->size];
                QByteArray tmparray;
                tmparray.clear();
                tsk_fs_file_read(tmpfile, 0, magicbuffer, 1024, TSK_FS_FILE_READ_FLAG_NONE);
                tmparray = QByteArray::fromRawData(magicbuffer, 1024);
                QMimeDatabase mimedb;
                QMimeType mimetype = mimedb.mimeTypeForData(tmparray);
                //free(magicbuffer);
                //delete[] magicbuffer;
                QFile hshfile;
                if(tmpfile->name->meta_addr == 0 && strcmp(tmpfile->name->name, "$MFT") != 0)
                    hshfile.setFileName(partitionpath + "f" + QString::number(orphancount) + ".a" + QString::number(tmpfile->name->par_addr) + ".stat");
                else
                    hshfile.setFileName(partitionpath + "f" + QString::number(tmpfile->name->meta_addr) + ".a" + QString::number(tmpfile->name->par_addr) + ".stat");
                hshfile.open(QIODevice::ReadOnly | QIODevice::Text);
                QString tmpfilestr = "";
                if(hshfile.isOpen())
                    tmpfilestr = hshfile.readLine();
                hshfile.close();
                if(tmpfilestr.split(",").count() > 12)
                {
                    if(tmpfilestr.split(",").at(13).compare("0") != 0)
                    {
                        treeout << tmpfilestr.split(",").at(13);
                        if(hashsum == 0)
                        {
                            if(tmpfilestr.split(",").at(13).size() == 32) // md5
                                hashsum = 1;
                            else if(tmpfilestr.split(",").at(13).size() == 40) // sha1
                                hashsum = 2;
                            else if(tmpfilestr.split(",").at(13).size() == 64) // sha256
                                hashsum = 4;
                        }
                    }
                    else
                        treeout << "0";
                }
                else
                    treeout << "0";
                // ADD THE HASH VALUE FROM THE FILE TO TREEOUT
                //treeout << "0";
                treeout << mimetype.name().split("/").at(0) << mimetype.name().split("/").at(1); // category << signature
                treeout << tmpfilestr.split(",").at(15); // bookmark value
                // PUT ID INFO HERE FOR NAME IN FIRST COLUMN
                if(tmpfile->name->meta_addr == 0 && strcmp(tmpfile->name->name, "$MFT") != 0)
                {
                    //QDir partdir = QDir(((AddEvidenceVariable*)tmpptr)->partitionpath);
                    //QStringList fileslist = partdir.entryList(QStringList("*." + astring), QDir::NoSymLinks | QDir::Files);
                    //QFile filefile(((AddEvidenceVariable*)tmpptr)->partitionpath + "
                    // REPLACE THE BELOW WITH OPENING THE FILE AND GETTING THE ORPHAN COUNT #...
                    treeout << "e" + QString::number(((AddEvidenceVariable*)tmpptr)->evidcnt) + "-v" + QString::number(((AddEvidenceVariable*)tmpptr)->volcnt) + "-p" + QString::number(((AddEvidenceVariable*)tmpptr)->partint) + "-f*" + QString::number(orphancount) + "-a" + QString::number(tmpfile->name->par_addr);
                    orphancount++;
                }
                else
                    treeout << "e" + QString::number(((AddEvidenceVariable*)tmpptr)->evidcnt) + "-v" + QString::number(((AddEvidenceVariable*)tmpptr)->volcnt) + "-p" + QString::number(((AddEvidenceVariable*)tmpptr)->partint) + "-f" + QString::number(tmpfile->name->meta_addr) + "-a" + QString::number(tmpfile->name->par_addr);
                if(tmpfile->meta != NULL)
                    treeout << QString::number(tmpfile->meta->type);
                else
                    treeout << QString::number(tmpfile->name->type);
            
                if(tmpfile->name->meta_addr == 0 && strcmp(tmpfile->name->name, "$MFT") != 0)
                {
                    treeout << "1";
                }
                else
                {
                    if(tmpfile->meta != NULL)
                    {
                        if(((tmpfile->meta->flags & TSK_FS_META_FLAG_UNALLOC) == 2) && ((tmpfile->meta->flags & TSK_FS_META_FLAG_USED) == 4))
                        {
                            treeout << "1";
                        }
                        else
                        {
                                treeout << "0";
                        }
                    }
                    else
                    {
                        if((tmpfile->name->flags & TSK_FS_NAME_FLAG_UNALLOC) == 0x02)
                        {
                            treeout << "1";
                        }
                        else
                        {
                            treeout << "0";
                        }
                    }
                }
                nodedata.clear();
                for(int i=0; i < 12; i++)
                    nodedata << treeout.at(i);
                mutex.lock();
                treenodemodel->AddNode(nodedata, parentstr, treeout.at(12).toInt(), treeout.at(13).toInt());
                listeditems.append(treeout.at(11));
                mutex.unlock();
                if(tmpfile->name != NULL)
                {
                    if(!TSK_FS_ISDOT(tmpfile->name->name))
                        filesfound++;
                }
                isignals->ProgUpd();
            
                if(tmpfile->fs_info->ftype == TSK_FS_TYPE_NTFS_DETECT)
                {
                    QByteArray adsba;
                    qint64 adssize = 0;
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
                    adssize = (qint64)teststart;
                    int cnt, i;
                    cnt = tsk_fs_file_attr_getsize(tmpfile);
                    for(i = 0; i < cnt; i++)
                    {
                        char type[512];
                        const TSK_FS_ATTR* fsattr = tsk_fs_file_attr_get_idx(tmpfile, i);
                        adssize += 24;
                        adssize += (qint64)fsattr->size;
                        if(ntfs_attrname_lookup(tmpfile->fs_info, fsattr->type, type, 512) == 0)
                        {
                            if(QString::compare(QString(type), "$DATA", Qt::CaseSensitive) == 0)
                            {
                                if(QString::compare(QString(fsattr->name), "") != 0 && QString::compare(QString(fsattr->name), "$I30", Qt::CaseSensitive) != 0)
                                {
                                    char fbuf[fsattr->size];
                                    //char* fbuf = new char[fsattr->size];
                                    ssize_t flen = tsk_fs_attr_read(fsattr, 0, fbuf, fsattr->size, TSK_FS_FILE_READ_FLAG_NONE);
                                    QByteArray fdata = QByteArray::fromRawData(fbuf, flen);
                                    QMimeDatabase adsmimedb;
                                    QMimeType adsmimetype = mimedb.mimeTypeForData(fdata);
                                    //delete[] fbuf;
                                    adsba.append(QString(tmpfile->name->name) + QString(":") + QString(fsattr->name));
                                    treeout.clear();
                                    // ADD THE HASH VALUE TO FROM THE FILE
                                    QFile hshfile;
                                    hshfile.setFileName(partitionpath + "f" + QString::number(tmpfile->name->meta_addr) + "-" + QString::number(fsattr->id) + ".a" + QString::number(tmpfile->name->meta_addr) + ".stat");
                                    hshfile.open(QIODevice::ReadOnly | QIODevice::Text);
                                    QString tmpfilestr = "";
                                    if(hshfile.isOpen())
                                        tmpfilestr = hshfile.readLine();
                                    hshfile.close();
                                    QString hshstr = "";
                                    if(tmpfilestr.split(",").count() > 12)
                                    {
                                        if(tmpfilestr.split(",").at(13).compare("0") != 0)
                                            hshstr = tmpfilestr.split(",").at(13);
                                        else
                                            hshstr = "0";
                                    }
                                    else
                                        hshstr = "0";

                                    treeout << adsba.toBase64() << ba2.toBase64() << QString::number(fsattr->size) << "0" << "0" << "0" << "0" << hshstr << adsmimetype.name().split("/").at(0) << adsmimetype.name().split("/").at(1) << tmpfilestr.split(",").at(15) << QString("e" + QString::number(((AddEvidenceVariable*)tmpptr)->evidcnt) + "-v" + QString::number(((AddEvidenceVariable*)tmpptr)->volcnt) + "-p" + QString::number(((AddEvidenceVariable*)tmpptr)->partint) + "-f" + QString::number(tmpfile->name->meta_addr) + ":" + QString::number(fsattr->id) + "-a" + QString::number(tmpfile->name->meta_addr)) << "10" << "0"; // NAME IN FIRST COLUMN
                                    nodedata.clear();
                                    for(int i=0;  i < 12; i++)
                                        nodedata << treeout.at(i);
                                    mutex.lock();
                                    treenodemodel->AddNode(nodedata, QString("e" + QString::number(((AddEvidenceVariable*)tmpptr)->evidcnt) + "-v" + QString::number(((AddEvidenceVariable*)tmpptr)->volcnt) + "-p" + QString::number(((AddEvidenceVariable*)tmpptr)->partint) + "-f" + QString::number(tmpfile->name->meta_addr)), treeout.at(12).toInt(), treeout.at(13).toInt());
                                    mutex.unlock();
                                    listeditems.append(treeout.at(11)); // UNCOMMENT ONCE I HANDLE ADS IN GEN IMG THUMB AND HASH
                                    filesfound++;
                                    isignals->ProgUpd();
                                }
                            }
                        }
                    }
                }
            }
            return TSK_WALK_CONT;
        }
        return TSK_WALK_CONT;
    }
    return TSK_WALK_CONT;
}

TSK_WALK_RET_ENUM RootEntries(TSK_FS_FILE* tmpfile, const char* tmppath, void* tmpptr)
{
    if(tmpptr){}
    if(tmpfile->name != NULL)
    {
        if(strcmp(tmpfile->name->name, ".") != 0)
        {
            if(strcmp(tmpfile->name->name, "..") != 0)
            {
                BuildStatFile(tmpfile, tmppath, (AddEvidenceVariable*)tmpptr);
                WriteFileProperties(tmpfile, (AddEvidenceVariable*)tmpptr);
                if(tmpfile->name->meta_addr == 0 && strcmp(tmpfile->name->name, "$MFT") != 0)
                    orphancount++;
            }
            return TSK_WALK_CONT;
        }
        return TSK_WALK_CONT;
    }
    else
        qDebug() << "what is going on...";
    return TSK_WALK_CONT;
}

void ProcessExport(QString objectid)
{
    TSK_IMG_INFO* readimginfo;
    TSK_FS_INFO* readfsinfo;
    TSK_FS_FILE* readfileinfo;
    QString tmpstr = "";
    QDir eviddir = QDir(wombatvariable.tmpmntpath);
    std::vector<std::string> pathvector;
    const TSK_TCHAR** imagepartspath;
    pathvector.clear();
    QString estring = objectid.split("-", QString::SkipEmptyParts).at(0);
    QString vstring = objectid.split("-", QString::SkipEmptyParts).at(1);
    QString pstring = objectid.split("-", QString::SkipEmptyParts).at(2);
    QString fstring = objectid.split("-", QString::SkipEmptyParts).at(3);
    if(fstring.contains(":") == true)
        fstring = fstring.split(":").first() + "-" + fstring.split(":").last();
    //qint64 curaddress = objectid.split("-f").at(1).toLongLong();
    qint64 curaddress = objectid.split("-f").at(1).split("-a").at(0).split(":").at(0).toLongLong(); 
    QStringList evidfiles = eviddir.entryList(QStringList(QString("*." + estring)), QDir::NoSymLinks | QDir::Dirs);
    QString evidencename = evidfiles.at(0).split(".e").first();
    QFile evidfile(wombatvariable.tmpmntpath + evidencename + "." + estring + "/stat");
    evidfile.open(QIODevice::ReadOnly);
    tmpstr = evidfile.readLine();
    int partcount = tmpstr.split(",").at(3).split("|").size();
    if(evidfile.isOpen())
        evidfile.close();
    for(int i=0; i < partcount; i++)
        pathvector.push_back(tmpstr.split(",").at(3).split("|").at(i).toStdString());
    imagepartspath = (const char**)malloc(pathvector.size()*sizeof(char*));
    for(uint i=0; i < pathvector.size(); i++)
        imagepartspath[i] = pathvector[i].c_str();
    readimginfo = tsk_img_open(partcount, imagepartspath, TSK_IMG_TYPE_DETECT, 0);
    if(readimginfo == NULL)
    {
        qDebug() << tsk_error_get_errstr();
        //LogMessage("Image opening error");
    }
    free(imagepartspath);
    tmpstr = "";
    QFile partfile(wombatvariable.tmpmntpath + evidencename + "." + estring + "/" + vstring + "/" + pstring + "/stat");
    partfile.open(QIODevice::ReadOnly);
    tmpstr = partfile.readLine();
    if(partfile.isOpen())
        partfile.close();
    readfsinfo = tsk_fs_open_img(readimginfo, tmpstr.split(",").at(4).toLongLong(), TSK_FS_TYPE_DETECT);
    readfileinfo = tsk_fs_file_open_meta(readfsinfo, NULL, curaddress);
    if(readfileinfo->meta != NULL)
    {
        char imgbuf[readfileinfo->meta->size];
        //char* imgbuf = reinterpret_cast<char*>(malloc(readfileinfo->meta->size));
        //char* imgbuf = new char[readfileinfo->meta->size];
        ssize_t imglen = tsk_fs_file_read(readfileinfo, 0, imgbuf, readfileinfo->meta->size, TSK_FS_FILE_READ_FLAG_NONE);
        QDir filedir = QDir(wombatvariable.tmpmntpath + evidencename + "." + estring + "/" + vstring + "/" + pstring);
        QStringList filefiles = filedir.entryList(QStringList(fstring + ".a*.stat"), QDir::NoSymLinks | QDir::Files);
        QFile filefile(wombatvariable.tmpmntpath + evidencename + "." + estring + "/" + vstring + "/" + pstring + "/" + filefiles.at(0));
        filefile.open(QIODevice::ReadOnly);
        tmpstr = filefile.readLine();
        if(filefile.isOpen())
        {
            filefile.close();
        }
        QString tmppath = "";
        QByteArray ba;
        QByteArray ba2;
        ba.append(tmpstr.split(",", QString::SkipEmptyParts).at(0));
        ba2.append(tmpstr.split(",", QString::SkipEmptyParts).at(3));
        QString tmpname = QByteArray::fromBase64(ba);
        if(originalpath == true)
            tmppath = exportpath + QByteArray::fromBase64(ba2);
        else
            tmppath = exportpath + "/";
        if(tmpstr.split(",", QString::SkipEmptyParts).at(1).toInt() == 3) // directory
        {
            QDir dir;
            bool tmpdir = dir.mkpath(QString(tmppath + tmpname));
            //QDir dir;
            //dir.mkpath(wombatvariable.tmpfilepath);
            //bool tmpdir = (new QDir())->mkpath(QString(tmppath + tmpname));
            if(!tmpdir)
            {
                qWarning() << "Creation of export directory tree for file:" << tmppath << "failed";
                //LogMessage(QString("Creation of export directory tree for file: " + tmppath + " failed"));
                errorcount++;
            }
        }
        else
        {
            QDir dir;
            bool tmpdir = dir.mkpath(QDir::cleanPath(tmppath));
            //bool tmpdir = (new QDir())->mkpath(QDir::cleanPath(tmppath));
            if(tmpdir == true)
            {
                QFile tmpfile(tmppath + tmpname);
                if(tmpfile.open(QIODevice::WriteOnly))
                {
                    QDataStream outbuffer(&tmpfile);
                    outbuffer.writeRawData(imgbuf, imglen);
                    if(tmpfile.isOpen())
                        tmpfile.close();
                }
            }
        }
        //delete[] imgbuf;
        //free(imgbuf);
    }
    tsk_fs_file_close(readfileinfo);
    tsk_fs_close(readfsinfo);
    tsk_img_close(readimginfo);
    exportcount++;
    isignals->ExportUpd();
}

void SaveImagesHash()
{
    QFile thumbfile;
    thumbfile.setFileName(wombatvariable.tmpmntpath + "thumbs/thumbpathlist");
    thumbfile.open(QIODevice::WriteOnly | QIODevice::Text);
    if(thumbfile.isOpen())
    {
        QHashIterator<QString, QString> i(imageshash);
        while(i.hasNext())
        {
            i.next();
            thumbfile.write(i.key().toStdString().c_str());
            thumbfile.write("|");
            thumbfile.write(i.value().toStdString().c_str());
            thumbfile.write(",");
        }
    }
    thumbfile.close();
}

void LoadImagesHash()
{
    QFile thumbfile;
    QString tmpstr = "";
    thumbfile.setFileName(wombatvariable.tmpmntpath + "thumbs/thumbpathlist");
    thumbfile.open(QIODevice::ReadOnly);
    if(thumbfile.isOpen())
        tmpstr = thumbfile.readLine();
    thumbfile.close();
    for(int i = 0; i < tmpstr.split(",", QString::SkipEmptyParts).count(); i++)
        imageshash.insert(tmpstr.split(",", QString::SkipEmptyParts).at(i).split("|").at(0), tmpstr.split(",", QString::SkipEmptyParts).at(i).split("|").at(1));
}

void GenerateHash(QString itemid)
{
    // NEED TO IMPLEMENT NTFS AND ADS USING WHETHER IT HAS THE :
    if(itemid.split("-").count() == 5)
    {
        int hashtype = 1;
        // given itemid, open file stat, file prop
        TSK_IMG_INFO* readimginfo = NULL;
        TSK_FS_INFO* readfsinfo = NULL;
        TSK_FS_FILE* readfileinfo = NULL;
        QString tmpstr = "";
        QStringList tmplist;
        tmplist.clear();
        QDir eviddir = QDir(wombatvariable.tmpmntpath);
        std::vector<std::string> pathvector;
        const TSK_TCHAR** imagepartspath;
        pathvector.clear();
        //qDebug() << "itemid:" << itemid;
        qint64 curaddress = itemid.split("-f").at(1).split("-a").at(0).split(":").at(0).toLongLong();
        //qDebug() << "curaddress:" << curaddress;
        QStringList evidfiles = eviddir.entryList(QStringList("*." + itemid.split("-").at(0)), QDir::NoSymLinks | QDir::Dirs);
        QString evidencename = evidfiles.at(0).split(".e").first();
        QFile evidfile(wombatvariable.tmpmntpath + evidencename + "." + itemid.split("-").at(0) + "/stat");
        evidfile.open(QIODevice::ReadOnly | QIODevice::Text);
        if(evidfile.isOpen())
            tmpstr = evidfile.readLine();
        evidfile.close();
        int partcount = tmpstr.split(",").at(3).split("|").size();
        for(int i=0; i < partcount; i++)
            pathvector.push_back(tmpstr.split(",").at(3).split("|").at(i).toStdString());
        imagepartspath = (const char**)malloc(pathvector.size()*sizeof(char*));
        for(uint i=0; i < pathvector.size(); i++)
            imagepartspath[i] = pathvector[i].c_str();
        readimginfo = tsk_img_open(partcount, imagepartspath, TSK_IMG_TYPE_DETECT, 0);
        if(readimginfo == NULL)
        {
            qDebug() << tsk_error_get_errstr();
            //LogMessage("Image opening error");
        }
        free(imagepartspath);
        tmpstr = "";
        QFile partfile(wombatvariable.tmpmntpath + evidencename + "." + itemid.split("-").at(0) + "/" + itemid.split("-").at(1) + "/" + itemid.split("-").at(2) + "/stat");
        partfile.open(QIODevice::ReadOnly | QIODevice::Text);
        if(partfile.isOpen())
            tmpstr = partfile.readLine();
        partfile.close();
        //qDebug() << "part tmpstr:" << tmpstr;
        tmplist = tmpstr.split(",");
        if(tmplist.count() > 0)
        {
            readfsinfo = tsk_fs_open_img(readimginfo, tmpstr.split(",").at(4).toLongLong(), TSK_FS_TYPE_DETECT);
            if(readfsinfo != NULL)
                readfileinfo = tsk_fs_file_open_meta(readfsinfo, NULL, curaddress);
        }
        QString hashstr = "";
        char* hashdata = new char[0];
        ssize_t hashdatalen = 0;
        if(readfileinfo != NULL)
        {
            if(itemid.contains(":")) // ADS file
            {
                qint64 adssize = 0;
                TSK_OFF_T curmftentrystart = 0;
                NTFS_INFO* ntfsinfo = (NTFS_INFO*)readfileinfo->fs_info;
                int recordsize = 0;
                if(ntfsinfo->fs->mft_rsize_c > 0)
                    recordsize = ntfsinfo->fs->mft_rsize_c * ntfsinfo->fs->csize * tsk_getu16(readfileinfo->fs_info->endian, ntfsinfo->fs->ssize);
                else
                    recordsize = 1 << -ntfsinfo->fs->mft_rsize_c;
                if(readfileinfo->meta != NULL)
                    curmftentrystart = tsk_getu16(readfileinfo->fs_info->endian, ntfsinfo->fs->ssize) * ntfsinfo->fs->csize * tsk_getu64(readfileinfo->fs_info->endian, ntfsinfo->fs->mft_clust) + recordsize * readfileinfo->meta->addr + 20;
                else
                    curmftentrystart = tsk_getu16(readfileinfo->fs_info->endian, ntfsinfo->fs->ssize) * ntfsinfo->fs->csize * tsk_getu64(readfileinfo->fs_info->endian, ntfsinfo->fs->mft_clust) + recordsize + 20;
                char startoffset[2];
                tsk_fs_read(readfileinfo->fs_info, curmftentrystart, startoffset, 2);
                uint16_t teststart = startoffset[1] * 256 + startoffset[0];
                adssize = (qint64)teststart;
                int cnt, i;
                cnt = tsk_fs_file_attr_getsize(readfileinfo);
                for(i = 0; i < cnt; i++)
                {
                    char type[512];
                    const TSK_FS_ATTR* fsattr = tsk_fs_file_attr_get_idx(readfileinfo, i);
                    adssize += 24;
                    adssize += (qint64)fsattr->size;
                    if(ntfs_attrname_lookup(readfileinfo->fs_info, fsattr->type, type, 512) == 0)
                    {
                        if(QString::compare(QString(type), "$DATA", Qt::CaseSensitive) == 0)
                        {
                            if(QString::compare(QString(fsattr->name), "") != 0 && QString::compare(QString(fsattr->name), "$I30", Qt::CaseSensitive) != 0)
                            {
                                hashdata = new char[fsattr->size];
                                hashdatalen = tsk_fs_attr_read(fsattr, 0, hashdata, fsattr->size, TSK_FS_FILE_READ_FLAG_NONE);
                            }
                        }
                    }
                }
            }
            else // regular file
            {
                hashdata = new char[readfileinfo->meta->size];
                hashdatalen = tsk_fs_file_read(readfileinfo, 0, hashdata, readfileinfo->meta->size, TSK_FS_FILE_READ_FLAG_NONE);
            }
        }
        tsk_fs_file_close(readfileinfo);
        tsk_fs_close(readfsinfo);
        tsk_img_close(readimginfo);
        readfileinfo = NULL;
        readfsinfo = NULL;
        readimginfo = NULL;
        QCryptographicHash tmphash((QCryptographicHash::Algorithm)hashsum);
        QByteArray hasharray = QByteArray::fromRawData(hashdata, hashdatalen);
        QDir filedir = QDir(wombatvariable.tmpmntpath + evidencename + "." + itemid.split("-").at(0) + "/" + itemid.split("-").at(1) + "/" + itemid.split("-").at(2));
        QStringList filefiles;
        QFile filefile;
        if(itemid.contains(":"))
        {
            filefiles = filedir.entryList(QStringList(itemid.split("-").at(3).split(":").first() + "-" + itemid.split("-").at(3).split(":").last() + ".a*.stat"), QDir::NoSymLinks | QDir::Files);
            filefile.setFileName(wombatvariable.tmpmntpath + evidencename + "." + itemid.split("-").at(0) + "/" + itemid.split("-").at(1) + "/" + itemid.split("-").at(2) + "/" + filefiles.at(0));
        }
        else
        {
            filefiles = filedir.entryList(QStringList(itemid.split("-").at(3) + ".a*.stat"), QDir::NoSymLinks | QDir::Files);
            filefile.setFileName(wombatvariable.tmpmntpath + evidencename + "." + itemid.split("-").at(0) + "/" + itemid.split("-").at(1) + "/" + itemid.split("-").at(2) + "/" + filefiles.at(0));
        }
        filefile.open(QIODevice::ReadOnly | QIODevice::Text);
        if(filefile.isOpen())
            tmpstr = filefile.readLine();
        filefile.close();
        if(tmpstr.split(",").count() > 0)
        {
            //qDebug() << "tmpstr:" << tmpstr;
            tmplist.clear();
            tmplist = tmpstr.split(",");
            if(hashdatalen > 0)
                tmplist[13] = QString(tmphash.hash(hasharray, (QCryptographicHash::Algorithm)hashsum).toHex()).toUpper();
            else
            {
                if(hashsum == 1)
                    tmplist[13] = QString("d41d8cd98f00b204e9800998ecf8427e").toUpper(); // MD5 zero file
                else if(hashsum == 2)
                    tmplist[13] = QString("da39a3ee5e6b4b0d3255bfef95601890afd80709").toUpper(); // SHA1 zero file
                else if(hashsum == 4)
                    tmplist[13] = QString("e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855").toUpper(); // SHA256 zero file
            }
            hashstr = tmplist.at(13);
            tmpstr = "";
            for(int i=0; i < tmplist.count() - 1; i++)
                tmpstr += tmplist.at(i) + ",";
            tmpstr += tmplist.last();
            filefile.open(QIODevice::WriteOnly | QIODevice::Text);
            if(filefile.isOpen())
                filefile.write(tmpstr.toStdString().c_str());
            filefile.close();
        }
        treenodemodel->UpdateNode(itemid, 7, hashstr);
        QString hashheader = "";
        //delete[] hashdata;
        if(hashsum == 1) // MD5
            hashtype = 1;
        else if(hashsum == 2) // SHA1
            hashtype = 2;
        else if(hashsum == 4) // SHA256
            hashtype = 3;
        dighashcount++;
        isignals->DigUpd(hashtype, dighashcount);
    }
}

void GenerateThumbnails(QString thumbid)
{
    if(thumbid.split("-").count() == 5)
    {
        TSK_IMG_INFO* readimginfo = NULL;
        TSK_FS_INFO* readfsinfo = NULL;
        TSK_FS_FILE* readfileinfo = NULL;
        QString tmpstr = "";
        QString filestr = "";
        QDir eviddir = QDir(wombatvariable.tmpmntpath);
        std::vector<std::string> pathvector;
        const TSK_TCHAR** imagepartspath;
        pathvector.clear();
        //qDebug() << "thumbid:" << thumbid;
        QString estring = thumbid.split("-", QString::SkipEmptyParts).at(0);
        QString vstring = thumbid.split("-", QString::SkipEmptyParts).at(1);
        QString pstring = thumbid.split("-", QString::SkipEmptyParts).at(2);
        QString fstring = thumbid.split("-", QString::SkipEmptyParts).at(3);
        if(fstring.contains(":"))
            fstring = thumbid.split("-").at(3).split(":").first() + "-" + thumbid.split("-").at(3).split(":").last();
        QString astring = thumbid.split("-", QString::SkipEmptyParts).at(4);
        qint64 curaddress = thumbid.split("-f").at(1).split("-a").at(0).split(":").at(0).toLongLong();
        //qDebug() << "curaddress:" << curaddress;
        QStringList evidfiles = eviddir.entryList(QStringList("*." + estring), QDir::NoSymLinks | QDir::Dirs);
        QString evidencename = evidfiles.at(0).split(".e").first();
        QFile evidfile(wombatvariable.tmpmntpath + evidencename + "." + estring + "/stat");
        evidfile.open(QIODevice::ReadOnly | QIODevice::Text);
        if(evidfile.isOpen())
            tmpstr = evidfile.readLine();
        evidfile.close();
        int partcount = tmpstr.split(",").at(3).split("|").size();
        for(int i=0; i < partcount; i++)
            pathvector.push_back(tmpstr.split(",").at(3).split("|").at(i).toStdString());
        imagepartspath = (const char**)malloc(pathvector.size()*sizeof(char*));
        for(uint i=0; i < pathvector.size(); i++)
            imagepartspath[i] = pathvector[i].c_str();
        readimginfo = tsk_img_open(partcount, imagepartspath, TSK_IMG_TYPE_DETECT, 0);
        if(readimginfo == NULL)
        {
            qDebug() << tsk_error_get_errstr();
            //LogMessage("Image opening error");
        }
        free(imagepartspath);
        tmpstr = "";
        //qDebug() << "wombatvariable.evidencename:" << wombatvariable.evidencename;
        QFile partfile(wombatvariable.tmpmntpath + evidencename + "." + estring + "/" + vstring + "/" + pstring + "/stat");
        //qDebug() << "partfile:" << partfile.fileName();
        partfile.open(QIODevice::ReadOnly | QIODevice::Text);
        if(partfile.isOpen())
            tmpstr = partfile.readLine();
        partfile.close();
        //qDebug() << "tmpstr:" << tmpstr;
        if(tmpstr.count() > 0)
        {
            readfsinfo = tsk_fs_open_img(readimginfo, tmpstr.split(",").at(4).toLongLong(), TSK_FS_TYPE_DETECT);
            if(readfsinfo != NULL)
                readfileinfo = tsk_fs_file_open_meta(readfsinfo, NULL, curaddress);
        }
        char* imgbuf = NULL;
        ssize_t imglen = 0;
        if(readfileinfo != NULL)
        {
            if(thumbid.contains(":")) // ADS
            {
                qint64 adssize = 0;
                TSK_OFF_T curmftentrystart = 0;
                NTFS_INFO* ntfsinfo = (NTFS_INFO*)readfileinfo->fs_info;
                int recordsize = 0;
                if(ntfsinfo->fs->mft_rsize_c > 0)
                    recordsize = ntfsinfo->fs->mft_rsize_c * ntfsinfo->fs->csize * tsk_getu16(readfileinfo->fs_info->endian, ntfsinfo->fs->ssize);
                else
                    recordsize = 1 << -ntfsinfo->fs->mft_rsize_c;
                if(readfileinfo->meta != NULL)
                    curmftentrystart = tsk_getu16(readfileinfo->fs_info->endian, ntfsinfo->fs->ssize) * ntfsinfo->fs->csize * tsk_getu64(readfileinfo->fs_info->endian, ntfsinfo->fs->mft_clust) + recordsize * readfileinfo->meta->addr + 20;
                else
                    curmftentrystart = tsk_getu16(readfileinfo->fs_info->endian, ntfsinfo->fs->ssize) * ntfsinfo->fs->csize * tsk_getu64(readfileinfo->fs_info->endian, ntfsinfo->fs->mft_clust) + recordsize + 20;
                char startoffset[2];
                tsk_fs_read(readfileinfo->fs_info, curmftentrystart, startoffset, 2);
                uint16_t teststart = startoffset[1] * 256 + startoffset[0];
                adssize = (qint64)teststart;
                int cnt, i;
                cnt = tsk_fs_file_attr_getsize(readfileinfo);
                for(i = 0; i < cnt; i++)
                {
                    char type[512];
                    const TSK_FS_ATTR* fsattr = tsk_fs_file_attr_get_idx(readfileinfo, i);
                    adssize += 24;
                    adssize += (qint64)fsattr->size;
                    if(ntfs_attrname_lookup(readfileinfo->fs_info, fsattr->type, type, 512) == 0)
                    {
                        if(QString::compare(QString(type), "$DATA", Qt::CaseSensitive) == 0)
                        {
                            if(QString::compare(QString(fsattr->name), "") != 0 && QString::compare(QString(fsattr->name), "$I30", Qt::CaseSensitive) != 0)
                            {
                                imgbuf = new char[fsattr->size];
                                imglen = tsk_fs_attr_read(fsattr, 0, imgbuf, fsattr->size, TSK_FS_FILE_READ_FLAG_NONE);
                            }
                        }
                    }
                }
            }
            else // regular file
            {
                imgbuf = new char[readfileinfo->meta->size];
                //imgbuf = reinterpret_cast<char*>(malloc(readfileinfo->meta->size));
                imglen = tsk_fs_file_read(readfileinfo, 0, imgbuf, readfileinfo->meta->size, TSK_FS_FILE_READ_FLAG_NONE);
            }
        }
        tsk_fs_file_close(readfileinfo);
        tsk_fs_close(readfsinfo);
        tsk_img_close(readimginfo);
        readfileinfo = NULL;
        readfsinfo = NULL;
        readimginfo = NULL;

        QFile filefile(wombatvariable.tmpmntpath + evidencename + "." + estring + "/" + vstring + "/" + pstring + "/" + fstring + "." + astring + ".stat");
        //qDebug() << "id:filename" << thumbid << filefile.fileName().split("mntpt/").at(1);
        filefile.open(QIODevice::ReadOnly | QIODevice::Text);
        if(filefile.isOpen())
            filestr = filefile.readLine();
        filefile.close();
        if(filestr.count() > 1)
        {
            QString filemime = filestr.split(",").at(10);
            QString filecat = filemime.split("/").first();
            if(filecat.contains("image"))
            {
                QByteArray ba;
                QByteArray ba2;
                ba.append(filestr.split(",").at(0));
                ba2.append(filestr.split(",").at(3));
                QString fullpath = QString(QByteArray::fromBase64(ba2)) + QString(QByteArray::fromBase64(ba));
                ba.clear();
                ba.append(fullpath);
                imageshash.insert(filestr.split(",", QString::SkipEmptyParts).at(12), QString(ba.toBase64()));
                QImage fileimage;
                QImage thumbimage;
                QImageWriter writer(wombatvariable.tmpmntpath + "thumbs/" + thumbid + ".jpg");
                if(imglen > 0)
                {
                    bool imageloaded = fileimage.loadFromData(QByteArray::fromRawData(imgbuf, imglen));
                    if(imageloaded)
                    {
                        thumbimage = fileimage.scaled(QSize(thumbsize, thumbsize), Qt::KeepAspectRatio, Qt::FastTransformation);
                        writer.write(thumbimage);
                    }
                    else
                    {
                        fileimage.load(":/missingimage");
                        thumbimage = fileimage.scaled(QSize(thumbsize, thumbsize), Qt::KeepAspectRatio, Qt::FastTransformation);
                        writer.write(thumbimage);
                    }
                }
            }
        }
        delete[] imgbuf;
        //free(imgbuf);
        digimgthumbcount++;
        isignals->DigUpd(0, digimgthumbcount);
    }
}

int SegmentDigits(int number)
{
    if(number < 10)
        return 1;
    int count = 0;
    while(number > 0)
    {
        number /= 10;
        count++;
    }
    return count;
}

void PopulateTreeModel(QString evidstring)
{
    AddEvidenceVariable addevidvar;
    AddEvidenceVariable* aevar = &addevidvar;
    QDir eviddir = QDir(wombatvariable.tmpmntpath);
    QStringList evidlist = eviddir.entryList(QStringList(evidstring.split("/").last() + ".e*"), QDir::NoSymLinks | QDir::Dirs);
    QString evidid = "." + evidlist.first().split(".").last();
    addevidvar.evidcnt = evidlist.first().split(".").last().mid(1).toInt();
    QString tmpstr = "";
    QString evidencename = evidlist.first().split(evidid).first();
    //qDebug() << "evidencename:" << evidencename;
    QFile evidfile(wombatvariable.tmpmntpath + evidlist.first() + "/stat");
    evidfile.open(QIODevice::ReadOnly | QIODevice::Text);
    if(evidfile.isOpen())
        tmpstr = evidfile.readLine();
    else
        qDebug() << "evidence file:" << evidfile.fileName() << "failed to open.";
    evidfile.close();
    std::vector<std::string> pathvector;
    pathvector.clear();
    pathvector.push_back(tmpstr.split(",").at(3).toStdString());
    TSK_IMG_INFO* imginfo = NULL;
    TSK_VS_INFO* vsinfo = NULL;
    const TSK_VS_PART_INFO* partinfo = NULL;
    TSK_FS_INFO* fsinfo = NULL;
    const TSK_TCHAR** images;
    images = (const char**)malloc(pathvector.size()*sizeof(char*));
    images[0] = pathvector[0].c_str();
    imginfo = tsk_img_open(1, images, TSK_IMG_TYPE_DETECT, 0);
    if(imginfo == NULL)
    {
        qWarning() << "Evidence image access failed";
        //LogMessage("Evidence image access failed");
        errorcount++;
    }
    else
        qInfo() << "Evidence Image successfully opened";
    free(images);
    //int imgtype = imginfo->itype;
    QString evidencepath = wombatvariable.tmpmntpath + evidencename + evidid + "/";
    QList<QVariant> nodedata;
    nodedata.clear();
    nodedata << evidencename << "0" << QString::number(imginfo->size) << "0" << "0" << "0" << "0" << "0" << "0" << "0" << tmpstr.split(",").at(6) << evidid.mid(1);
    mutex.lock();
    treenodemodel->AddNode(nodedata, "-1", -1, -1);
    mutex.unlock();
    vsinfo = tsk_vs_open(imginfo, 0, TSK_VS_TYPE_DETECT);
    QString volname = "Dummy Volume (NO PART)";
    if(vsinfo != NULL)
        volname = QString::fromUtf8(tsk_vs_type_todesc(vsinfo->vstype));
    QDir voldir = QDir(evidencepath);
    QStringList vollist = voldir.entryList(QStringList("v*"), QDir::NoSymLinks | QDir::Dirs);
    QString partitionpath = "";
    for(int i=0; i < vollist.count(); i++)
    {
        addevidvar.volcnt = vollist.at(i).mid(1).toInt();
        QString volumepath = evidencepath + vollist.at(i) + "/";
        nodedata.clear();
        nodedata << volname << "0" << QString::number(imginfo->size) << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << QString(evidid.mid(1) + "-" + vollist.at(i));
        mutex.lock();
        treenodemodel->AddNode(nodedata, evidid.mid(1), -1, 0);
        mutex.unlock();
        if(vsinfo == NULL) // No volume so a single file system is all there is in the image
        {
            addevidvar.partint = 0;
            fsinfo = tsk_fs_open_img(imginfo, 0, TSK_FS_TYPE_DETECT);
            if(fsinfo == NULL)
            {
                nodedata.clear();
                nodedata << QString("NON-RECOGNIZED FS") << "0" << QString::number(imginfo->size) << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << QString(evidid.mid(1) + "-" + vollist.at(i) + "-p0");
                mutex.lock();
                treenodemodel->AddNode(nodedata, QString(evidid.mid(1) + "-" + vollist.at(i)), -1, 0);
                mutex.unlock();
            }
            else
            {
                partitionpath = volumepath + "p0/";
                addevidvar.partitionpath = partitionpath;
                nodedata.clear();
                nodedata << QString(GetFileSystemLabel(fsinfo) + " (" + QString(tsk_fs_type_toname(fsinfo->ftype)).toUpper() + ")") << "0" << "0" << QString::number(fsinfo->block_size * fsinfo->block_count) << "0" << "0" << "0" << "0" << "0" << "0" << "0" << QString(evidid.mid(1) + "-" + vollist.at(i) + "-p0");
                mutex.lock();
                treenodemodel->AddNode(nodedata, QString(evidid.mid(1) + "-" + vollist.at(i)), -1, 0);
                mutex.unlock();
                uint8_t walkreturn;
                int walkflags = TSK_FS_DIR_WALK_FLAG_ALLOC | TSK_FS_DIR_WALK_FLAG_UNALLOC | TSK_FS_DIR_WALK_FLAG_RECURSE;
                walkreturn = tsk_fs_dir_walk(fsinfo, fsinfo->root_inum, (TSK_FS_DIR_WALK_FLAG_ENUM)walkflags, TreeEntries, (void*)aevar);
                if(walkreturn == 1)
                {
                    qWarning() << "Issues with traversing the file structure were encountered";
                    //LogMessage("Issues with traversing the file structure were encountered");
                    errorcount++;
                }

            }
        }
        else
        {
            QDir partdir = QDir(volumepath);
            QStringList partlist = partdir.entryList(QStringList("p*"), QDir::NoSymLinks | QDir::Dirs);
            for(int j=0; j < partlist.count(); j++)
            {
                addevidvar.partint = j;
                partitionpath = volumepath + "p" + QString::number(j) + "/";
                addevidvar.partitionpath = partitionpath;
                partinfo = tsk_vs_part_get(vsinfo, j);
                if(partinfo->flags == 0x02 || partinfo->flags == 0x04) // unallocated partition or meta entry
                {
                    nodedata.clear();
                    nodedata << QString(partinfo->desc) << "0" << QString::number(partinfo->len * vsinfo->block_size) << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << QString(evidid.mid(1) + "-" + vollist.at(i) + "-p" + QString::number(j) + "/");
                    mutex.lock();
                    treenodemodel->AddNode(nodedata, QString(evidid.mid(1) + "-" + vollist.at(i)), -1, 0);
                    mutex.unlock();
                }
                else if(partinfo->flags == 0x01) // allocated partition
                {
                    fsinfo = tsk_fs_open_vol(partinfo, TSK_FS_TYPE_DETECT);
                    if(fsinfo != NULL)
                    {
                        nodedata.clear();
                        nodedata << QString(GetFileSystemLabel(fsinfo) + " (" + QString(tsk_fs_type_toname(fsinfo->ftype)).toUpper() + ")") << "0" << QString::number(fsinfo->block_size * fsinfo->block_count) << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << QString(evidid.mid(1) + "-" + vollist.at(i) + "-p" + QString::number(j));
                        mutex.lock();
                        treenodemodel->AddNode(nodedata, QString(evidid.mid(1) + "-" + vollist.at(i)), -1, 0);
                        mutex.unlock();
                        uint8_t walkreturn;
                        int walkflags = TSK_FS_DIR_WALK_FLAG_ALLOC | TSK_FS_DIR_WALK_FLAG_UNALLOC | TSK_FS_DIR_WALK_FLAG_RECURSE;
                        walkreturn = tsk_fs_dir_walk(fsinfo, fsinfo->root_inum, (TSK_FS_DIR_WALK_FLAG_ENUM)walkflags, TreeEntries, (void*)aevar);
                        if(walkreturn == 1)
                        {
                            qWarning() << "Issues with traversing the file structure were encountered";
                            //LogMessage("Issues with traversing the file structure were encountered");
                            errorcount++;
                        }
                    }
                    else
                    {
                        nodedata.clear();
                        nodedata << QString(QString(partinfo->desc) + QString(" (NON-RECOGNIZED FS)")) << "0" << QString::number(partinfo->len * vsinfo->block_size) << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << QString(evidid.mid(1) + "-" + vollist.at(i) + "-p" + QString::number(j));
                        mutex.lock();
                        treenodemodel->AddNode(nodedata, QString(evidid.mid(1) + "-" + vollist.at(j)), -1, 0);
                        mutex.unlock();
                    }
                }
            }
        }
    }
}

void InitializeEvidenceStructure(QString evidname)
{
    // REPLACE ALL THE GLOBAL VARIABLES WITH LOCAL ONES...
    AddEvidenceVariable addevidvar;
    AddEvidenceVariable* aevar = &addevidvar;
    int evidcnt = 0;
    int volcnt = 0;
    int partint = 0;
    QDir eviddir = QDir(wombatvariable.tmpmntpath);
    QStringList evidfiles = eviddir.entryList(QStringList(QString(evidname.split("/").last() + ".e*")), QDir::NoSymLinks | QDir::Dirs);
    //qDebug() << evidfiles.count() << evidfiles;
    //evidcnt = evidfiles.count();
    evidcnt = evidfiles.at(0).split(".e").last().toInt();
    //qDebug() << "evidcnt:" << evidcnt;
    addevidvar.evidcnt = evidcnt;
    //qDebug() << evidfiles.count() << evidfiles;
    std::vector<std::string> fullpathvector;
    QString evidencename = evidname.split("/").last();
    //TSK_IMG_TYPE_ENUM imgtype;
    fullpathvector.clear();
    fullpathvector.push_back(evidname.toStdString());
    //qDebug() << "fullpathvector:" << QString::fromStdString(fullpathvector[0]) << "evidencename:" << evidencename;
    TSK_IMG_INFO* readimginfo = NULL;
    TSK_VS_INFO* readvsinfo = NULL;
    const TSK_VS_PART_INFO* readpartinfo = NULL;
    TSK_FS_INFO* readfsinfo = NULL;
    TSK_FS_FILE* readfileinfo = NULL;
    const TSK_TCHAR** images;
    images = (const char**)malloc(fullpathvector.size()*sizeof(char*));
    for(uint i=0; i < fullpathvector.size(); i++)
    {
        images[i] = fullpathvector[i].c_str();
    }
    readimginfo = tsk_img_open(1, images, TSK_IMG_TYPE_DETECT, 0);
    if(readimginfo == NULL)
    {
        qWarning() << "Evidence Image access failed";
        tsk_error_print(stderr);
        //LogMessage("Evidence Image acess failed");
        errorcount++;
    }
    free(images);
    //wombatvariable.segmentcount = wombatvariable.fullpathvector.size(); // number of segments for xmount call (TSK 4.2)
    QString evidencepath = wombatvariable.tmpmntpath + evidencename + ".e" + QString::number(evidcnt) + "/";
    //qDebug() << "evidencepath:" << evidencepath;
    //QString evidencepath = wombatvariable.tmpmntpath + evidencename + ".e" + QString::number(evidcnt) + "/";
    //(new QDir())->mkpath(evidencepath);
    QFile evidfile(evidencepath + "stat");
    evidfile.open(QIODevice::Append | QIODevice::Text);
    QTextStream out(&evidfile);
    out << (int)readimginfo->itype << "," << (qint64)readimginfo->size << "," << (int)readimginfo->sector_size << ",";
    //for(unsigned int i=0; i < wombatvariable.itemcount; i++)
    //{
    //    if(i > 0 && i < wombatvariable.itemcount - 2)
    //        out << "|";
        out << QString::fromStdString(fullpathvector[0]);
    //}
    out << "," << 1 << ",e" + QString::number(evidcnt) << ",0";
    //out << "," << wombatvariable.itemcount << ",e" + QString::number(evidcnt);
    out.flush();
    evidfile.close();
    //treefile.open(QIODevice::Append | QIODevice::Text);
    QStringList treeout;
    treeout << evidencename << "0" << QString::number(readimginfo->size) << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << QString("e" + QString::number(evidcnt)); // NAME IN FIRST COLUMN
    QList<QVariant> nodedata;
    nodedata.clear();
    for(int i=0; i < treeout.count(); i++)
        nodedata << treeout.at(i);
    mutex.lock();
    treenodemodel->AddNode(nodedata,  "-1", -1, -1);
    mutex.unlock();
    // Write Evidence Properties Here...
    WriteEvidenceProperties(readimginfo, evidencepath, QString::fromStdString(fullpathvector[0]));
    readvsinfo = tsk_vs_open(readimginfo, 0, TSK_VS_TYPE_DETECT);
    QString volname = "Dummy Volume (NO PART)";
    int voltype = 240;
    int volsectorsize = (int)readimginfo->sector_size;
    qint64 voloffset = 0;
    if(readvsinfo != NULL)
    {
        voltype = (int)readvsinfo->vstype;
        volname = QString::fromUtf8(tsk_vs_type_todesc(readvsinfo->vstype));
        volsectorsize = (int)readvsinfo->block_size;
        voloffset = (qint64)readvsinfo->offset;
    }
    QDir voldir = QDir(evidencepath);
    QStringList volfiles = voldir.entryList(QStringList(QString("v*")), QDir::NoSymLinks | QDir::Dirs);
    volcnt = volfiles.count();
    addevidvar.volcnt = volcnt;
    QString volumepath = evidencepath + "v" + QString::number(volcnt) + "/";
    QDir dir;
    dir.mkpath(volumepath);
    QFile volfile(volumepath + "stat");
    volfile.open(QIODevice::Append | QIODevice::Text);
    out.setDevice(&volfile);
    out << voltype << "," << (qint64)readimginfo->size << "," << volname << "," << volsectorsize << "," << voloffset << ",e" + QString::number(evidcnt) + "-v" + QString::number(volcnt) << ",0";
    out.flush();
    volfile.close();
    treeout.clear();
    treeout << volname << "0" << QString::number(readimginfo->size) << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << QString("e" + QString::number(evidcnt) + "-v" + QString::number(volcnt)); // NAME IN FIRST COLUMN
    nodedata.clear();
    for(int i=0; i < treeout.count(); i++)
        nodedata << treeout.at(i);
    mutex.lock();
    treenodemodel->AddNode(nodedata, QString("e" + QString::number(evidcnt)), -1, 0);
    mutex.unlock();
    if(readvsinfo != NULL)
        WriteVolumeProperties(readvsinfo, volumepath);
    if(readvsinfo == NULL) // No volume, so a single file system is all there is in the image.
    {
        readfsinfo = tsk_fs_open_img(readimginfo, 0, TSK_FS_TYPE_DETECT);
        if(readfsinfo == NULL) // unrecognized fs
        {
            qDebug() << "unrecognized single file system, need to catch and describe.";
            QString partitionpath = volumepath + "p0/";
            addevidvar.partitionpath = partitionpath;
            addevidvar.partint = partint;
            dir.mkpath(partitionpath);
            QFile pfile(partitionpath + "stat");
            pfile.open(QIODevice::Append | QIODevice::Text);
            out.setDevice(&pfile);
            out << "240," << QString::number(readimginfo->size) << ",NON-RECOGNIZED FS,0,0," << (qint64)readimginfo->size/volsectorsize << "," << volsectorsize << ",0,0,0,e" << QString::number(evidcnt) + "-v0-p0,0";
            out.flush();
            pfile.close();
            treeout.clear();
            treeout << "NON-RECOGNIZED FS" << "0" << QString::number(readimginfo->size) << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << QString("e" + QString::number(evidcnt) + "-v0-p0");
            nodedata.clear();
            for(int i=0; i < treeout.count(); i++)
                nodedata << treeout.at(i);
            mutex.lock();
            treenodemodel->AddNode(nodedata, QString("e" + QString::number(evidcnt) + "-v0-p0"), -1, 0);
            mutex.unlock();
        }
        else
        {
            QString partitionpath = volumepath + "p0/";
            addevidvar.partitionpath = partitionpath;
            addevidvar.partint = partint;
            dir.mkpath(partitionpath);
            QFile pfile(partitionpath + "stat");
            pfile.open(QIODevice::Append | QIODevice::Text);
            out.setDevice(&pfile);
            out << readfsinfo->ftype << "," << (qint64)readfsinfo->block_size * (qint64)readfsinfo->block_count << "," << GetFileSystemLabel(readfsinfo) << "," << (qint64)readfsinfo->root_inum << "," << (qint64)readfsinfo->offset << "," << (qint64)readfsinfo->block_count << "," << (int)readfsinfo->block_size << ",0,0,0,e" << QString::number(evidcnt) + "-v" + QString::number(volcnt) + "-p" + QString::number(partint) << ",0";
            out.flush();
            pfile.close();
            treeout.clear();
            treeout << QString(GetFileSystemLabel(readfsinfo) + " (" + QString(tsk_fs_type_toname(readfsinfo->ftype)).toUpper() + ")") << "0" << QString::number(readfsinfo->block_size * readfsinfo->block_count) << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << QString("e" + QString::number(evidcnt) + "-v" + QString::number(volcnt) + "-p" + QString::number(partint)); // NAME IN FIRST COLUMN
            nodedata.clear();
            for(int i=0; i < treeout.count(); i++)
                nodedata << treeout.at(i);
            mutex.lock();
            treenodemodel->AddNode(nodedata, QString("e" + QString::number(evidcnt) + "-v" + QString::number(volcnt)), -1, 0);
            mutex.unlock();
            WriteFileSystemProperties(readfsinfo, partitionpath);
            uint8_t walkreturn;
            int walkflags = TSK_FS_DIR_WALK_FLAG_ALLOC | TSK_FS_DIR_WALK_FLAG_UNALLOC | TSK_FS_DIR_WALK_FLAG_RECURSE;
            walkreturn = tsk_fs_dir_walk(readfsinfo, readfsinfo->root_inum, (TSK_FS_DIR_WALK_FLAG_ENUM)walkflags, RootEntries, (void*)aevar);
            //walkreturn = tsk_fs_dir_walk(readfsinfo, readfsinfo->root_inum, (TSK_FS_DIR_WALK_FLAG_ENUM)walkflags, RootEntries, NULL);
            if(walkreturn == 1)
            {
                qWarning() << "Issues with traversing the file structure were encountered";
                tsk_error_print(stderr);
                //LogMessage("Issues with traversing the file structure were encountered");
                errorcount++;
            }
        }
    }
    else
    {
        QFile pfile;
        if(readvsinfo->part_count > 0)
        {
            //qDebug() << "readvsinfo->part_count:" << readvsinfo->part_count;
            for(uint32_t i=0; i < readvsinfo->part_count; i++)
            {
                //qDebug() << "partint:" << partint << "partcount:" << readvsinfo->part_count;
                readpartinfo = tsk_vs_part_get(readvsinfo, i);
                if(readpartinfo == NULL)
                {
                    qDebug() << "tsk_vs_part_get error:";
                    tsk_error_print(stderr);
                }
                //qDebug() << "slot num:" << readpartinfo->slot_num;
                QString partitionpath = volumepath + "p" + QString::number(partint) + "/";
                addevidvar.partitionpath = partitionpath;
                addevidvar.partint = partint;
                dir.mkpath(partitionpath);
                //(new QDir())->mkpath(partitionpath);
                pfile.setFileName(partitionpath + "stat");
                pfile.open(QIODevice::Append | QIODevice::Text);
                out.setDevice(&pfile);
                if(readpartinfo->flags == 0x02 || readpartinfo->flags == 0x04) // unallocated partition or meta entry
                {
                    out << "0," << (qint64)readpartinfo->len * readvsinfo->block_size << "," << QString(readpartinfo->desc) << ",0," << (qint64)readpartinfo->start * (int)readvsinfo->block_size << "," << (qint64)readpartinfo->len << "," << (int)readvsinfo->block_size << "," << readpartinfo->flags << "," << (qint64)readpartinfo->len << "," << (int)readvsinfo->block_size << ",e" + QString::number(evidcnt) + "-v" + QString::number(volcnt) + "-p" + QString::number(partint) << ",0";
                    out.flush();
                    pfile.close();
                    treeout.clear();
                    treeout << QString(readpartinfo->desc) << "0" << QString::number(readpartinfo->len * readvsinfo->block_size) << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << QString("e" + QString::number(evidcnt) + "-v" + QString::number(volcnt) + "-p" + QString::number(partint)); // NAME IN FIRST COLUMN
                    nodedata.clear();
                    for(int j=0; j < treeout.count(); j++)
                        nodedata << treeout.at(j);
                    mutex.lock();
                    treenodemodel->AddNode(nodedata, QString("e" + QString::number(evidcnt) + "-v" + QString::number(volcnt)), -1, 0);
                    mutex.unlock();
                }
                else if(readpartinfo->flags == 0x01) // allocated partition
                {
                    readfsinfo = tsk_fs_open_vol(readpartinfo, TSK_FS_TYPE_DETECT);
                    if(readfsinfo != NULL)
                    {
                        out << readfsinfo->ftype << "," << (qint64)readfsinfo->block_size * (qint64)readfsinfo->block_count << "," << GetFileSystemLabel(readfsinfo) << "," << (qint64)readfsinfo->root_inum << "," << (qint64)readfsinfo->offset << "," << (qint64)readfsinfo->block_count << "," << (int)readfsinfo->block_size << "," << readpartinfo->flags << "," << (qint64)readpartinfo->len << "," << (int)readfsinfo->dev_bsize << ",e" + QString::number(evidcnt) + "-v" + QString::number(volcnt) + "-p" + QString::number(partint) << ",0";
                        out.flush();
                        pfile.close();
                        treeout.clear();
                        treeout << QString(GetFileSystemLabel(readfsinfo) + " (" + QString(tsk_fs_type_toname(readfsinfo->ftype)).toUpper() + ")") << "0" << QString::number(readfsinfo->block_size * readfsinfo->block_count) << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << QString("e" + QString::number(evidcnt) + "-v" + QString::number(volcnt) + "-p" + QString::number(partint)); // NAME IN FIRST COLUMN
                        nodedata.clear();
                        for(int j=0; j < treeout.count(); j++)
                            nodedata << treeout.at(j);
                        mutex.lock();
                        treenodemodel->AddNode(nodedata, QString("e" + QString::number(evidcnt) + "-v" + QString::number(volcnt)), -1, 0);
                        mutex.unlock();
                        WriteFileSystemProperties(readfsinfo, partitionpath);
                        uint8_t walkreturn;
                        int walkflags = TSK_FS_DIR_WALK_FLAG_ALLOC | TSK_FS_DIR_WALK_FLAG_UNALLOC | TSK_FS_DIR_WALK_FLAG_RECURSE;
                        walkreturn = tsk_fs_dir_walk(readfsinfo, readfsinfo->root_inum, (TSK_FS_DIR_WALK_FLAG_ENUM)walkflags, RootEntries, (void*)aevar);
                        //walkreturn = tsk_fs_dir_walk(readfsinfo, readfsinfo->root_inum, (TSK_FS_DIR_WALK_FLAG_ENUM)walkflags, RootEntries, NULL);
                        if(walkreturn == 1)
                        {
                            qWarning() << "Issues with traversing the file structure were encountered";
                            //LogMessage("Issues with traversing the file structure were encountered");
                            errorcount++;
                        }
                    }
                    else
                    {
                        out << "0," << (qint64)readpartinfo->len * (int)readvsinfo->block_size << "," << QString(readpartinfo->desc) << " (Non-Recognized FS),0," << (qint64)readpartinfo->start * (int)readvsinfo->block_size << "," << (qint64)readpartinfo->len << "," << (int)readvsinfo->block_size << "," << readpartinfo->flags << "," << (qint64)readpartinfo->len << "," << (int)readvsinfo->block_size << ",e" << QString::number(evidcnt) + "-v" + QString::number(volcnt) + "-p" + QString::number(partint) << ",0";
                        out.flush();
                        pfile.close();
                        treeout.clear();
                        treeout << QString(QString(readpartinfo->desc) + QString(" (Non-Recognized FS)")) << "0" << QString::number(readpartinfo->len * readvsinfo->block_size) << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << QString("e" + QString::number(evidcnt) + "-v" + QString::number(volcnt) + "-p" + QString::number(partint)); // NAME IN FIRST COLUMN
                        nodedata.clear();
                        for(int j=0; j < treeout.count(); j++)
                            nodedata << treeout.at(j);
                        mutex.lock();
                        treenodemodel->AddNode(nodedata, QString("e" + QString::number(evidcnt) + "-v" + QString::number(volcnt)), -1, 0);
                        mutex.unlock();
                    }
                }
                else
                {
                    out << "0," << (qint64)readpartinfo->len * (int)readvsinfo->block_size << "," << QString(readpartinfo->desc) << " (Non-Recognized FS),0," << (qint64)readpartinfo->start * (int)readvsinfo->block_size << "," << (qint64)readpartinfo->len << "," << (int)readvsinfo->block_size << "," << readpartinfo->flags << "," << (qint64)readpartinfo->len << "," << (int)readvsinfo->block_size << ",e" << QString::number(evidcnt) + "-v" + QString::number(volcnt) + "-p" + QString::number(partint) << ",0";
                    out.flush();
                    pfile.close();
                    treeout.clear();
                    treeout << QString(QString(readpartinfo->desc) + QString(" (Non-Recognized FS)")) << "0" << QString::number(readpartinfo->len * readvsinfo->block_size) << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << QString("e" + QString::number(evidcnt) + "-v" + QString::number(volcnt) + "-p" + QString::number(partint)); // NAME IN FIRST COLUMN
                    nodedata.clear();
                    for(int j=0; j < treeout.count(); j++)
                        nodedata << treeout.at(j);
                    mutex.lock();
                    treenodemodel->AddNode(nodedata, QString("e" + QString::number(evidcnt) + "-v" + QString::number(volcnt)), -1, 0);
                    mutex.unlock();
                }
                partint++;
            }
        }
    }
    tsk_fs_file_close(readfileinfo);
    readfileinfo = NULL;
    tsk_fs_close(readfsinfo);
    readfsinfo = NULL;
    readpartinfo = NULL;
    tsk_vs_close(readvsinfo);
    readvsinfo = NULL;
    tsk_img_close(readimginfo);
    readimginfo = NULL;
}

QString GetAdsBlockList(TSK_FS_FILE* tmpfile, qint64 attrid)
{
    QString blkstring = "";
    QString* blkstr = &blkstring;
    //blockstring = "";
    if(tmpfile->fs_info->ftype == TSK_FS_TYPE_NTFS_DETECT)
    {
        tsk_fs_file_walk_type(tmpfile, TSK_FS_ATTR_TYPE_NTFS_DATA, attrid, (TSK_FS_FILE_WALK_FLAG_ENUM)(TSK_FS_FILE_WALK_FLAG_AONLY | TSK_FS_FILE_WALK_FLAG_SLACK), GetBlockAddress, (void*)blkstr);
    }
    return blkstring;
    //return blockstring;
}

QString GetBlockList(TSK_FS_FILE* tmpfile)
{
    QString blkstring = "";
    QString* blkstr = &blkstring;
    //blockstring = "";
    if(tmpfile->fs_info->ftype == TSK_FS_TYPE_HFS_DETECT || tmpfile->fs_info->ftype == TSK_FS_TYPE_ISO9660_DETECT || tmpfile->fs_info->ftype == TSK_FS_TYPE_NTFS_DETECT || tmpfile->fs_info->ftype == TSK_FS_TYPE_FAT_DETECT)
    {
        if(tmpfile->fs_info->ftype == TSK_FS_TYPE_HFS_DETECT)
        {
            tsk_fs_file_walk_type(tmpfile, TSK_FS_ATTR_TYPE_HFS_DATA, HFS_FS_ATTR_ID_DATA, (TSK_FS_FILE_WALK_FLAG_ENUM)(TSK_FS_FILE_WALK_FLAG_AONLY | TSK_FS_FILE_WALK_FLAG_SLACK), GetBlockAddress, (void*)blkstr);
            //tsk_fs_file_walk_type(tmpfile, TSK_FS_ATTR_TYPE_HFS_DATA, HFS_FS_ATTR_ID_DATA, (TSK_FS_FILE_WALK_FLAG_ENUM)(TSK_FS_FILE_WALK_FLAG_AONLY | TSK_FS_FILE_WALK_FLAG_SLACK), GetBlockAddress, NULL);
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
                blkstring += QString::number(block++) + "^^";
                //blockstring += QString::number(block++) + "^^";
                size -= tmpfile->fs_info->block_size;
            }
        }
        else if(tmpfile->fs_info->ftype == TSK_FS_TYPE_NTFS_DETECT)
        {
            int minads = 1000;
            if(tmpfile->meta != NULL)
            {
                if(tmpfile->meta->attr)
                {
                    int cnt, i;
                    cnt = tsk_fs_file_attr_getsize(tmpfile);
                    for(i = 0; i < cnt; i++)
                    {
                        const TSK_FS_ATTR* tmpattr = tsk_fs_file_attr_get_idx(tmpfile, i);
                        if(tmpattr->flags & TSK_FS_ATTR_NONRES)
                        {
                            if(tmpattr->type == TSK_FS_ATTR_TYPE_NTFS_DATA)
                            {
                                if(tmpattr->id < minads)
                                    minads = tmpattr->id;
                            }
                        }
                    }
                    for(i = 0; i < cnt; i++)
                    {
                        const TSK_FS_ATTR* tmpattr = tsk_fs_file_attr_get_idx(tmpfile, i);
                        if(tmpattr->flags & TSK_FS_ATTR_NONRES) // non resident attribute
                        {
                            if(tmpattr->type == TSK_FS_ATTR_TYPE_NTFS_DATA && tmpattr->id == minads)
                            {
                                tsk_fs_file_walk_type(tmpfile, tmpattr->type, tmpattr->id, (TSK_FS_FILE_WALK_FLAG_ENUM)(TSK_FS_FILE_WALK_FLAG_AONLY | TSK_FS_FILE_WALK_FLAG_SLACK), GetBlockAddress, (void*)blkstr);
                                //tsk_fs_file_walk_type(tmpfile, tmpattr->type, tmpattr->id, (TSK_FS_FILE_WALK_FLAG_ENUM)(TSK_FS_FILE_WALK_FLAG_AONLY | TSK_FS_FILE_WALK_FLAG_SLACK), GetBlockAddress, NULL);
                            }
                        }
                    }
                }
            }
        }
        else if(tmpfile->fs_info->ftype == TSK_FS_TYPE_FAT_DETECT)
        {
            tsk_fs_file_walk(tmpfile, (TSK_FS_FILE_WALK_FLAG_ENUM)(TSK_FS_FILE_WALK_FLAG_AONLY | TSK_FS_FILE_WALK_FLAG_SLACK), GetBlockAddress, (void*)blkstr);
            //tsk_fs_file_walk(tmpfile, (TSK_FS_FILE_WALK_FLAG_ENUM)(TSK_FS_FILE_WALK_FLAG_AONLY | TSK_FS_FILE_WALK_FLAG_SLACK), GetBlockAddress, NULL);
        }
    }
    else
    {
        tsk_fs_file_walk(tmpfile, TSK_FS_FILE_WALK_FLAG_AONLY, GetBlockAddress, (void*)blkstr);
        //tsk_fs_file_walk(tmpfile, TSK_FS_FILE_WALK_FLAG_AONLY, GetBlockAddress, NULL);
    }
    
    return blkstring;
    //return blockstring;
}

void WriteAlternateDataStreamProperties(TSK_FS_FILE* curfileinfo, QString adsname, qint64 adssize, QString attrid, AddEvidenceVariable* aevar)
{
    QString curblockstring = GetAdsBlockList(curfileinfo, attrid.toLongLong());
    //qDebug() << "ads block string:" << curblockstring;
    if(curblockstring.compare("0^^") == 0)
        curblockstring = "";
    QFile adspropfile;
    if(curfileinfo->name != NULL)
    {
        adspropfile.setFileName(aevar->partitionpath + "f" + QString::number(curfileinfo->name->meta_addr) + "-" + attrid + ".a" + QString::number(curfileinfo->name->meta_addr) + ".prop");
        adspropfile.open(QIODevice::WriteOnly | QIODevice::Text);
        QTextStream proplist(&adspropfile);
        proplist << "Alternate Data Stream (ADS)||" << QString::fromStdString(std::string(curfileinfo->name->name)) << "||Alternate data stream which contains different content from what the file's standard content is." << endl;
        proplist << "Name||" << adsname << "||Name for the NTFS parent file additional $Data attribute" << endl;
        proplist << "Parent Address||" << QString::number(curfileinfo->meta->addr) << "||NTFS address ID for the parent file" << endl;
        proplist << "Parent File Name||" << QString(curfileinfo->name->name) << "||File name of the parent file" << endl;
        proplist << "Block Address||" << curblockstring << "||List of block addresses which contain the contents of the alternate data stream" << endl;
        proplist << "Attribute ID||" << attrid << "||ID for the file's ADS attribute" << endl;
        if(curblockstring.compare("") != 0)
            proplist << "Byte Offset||" << QString::number(curblockstring.split("^^", QString::SkipEmptyParts).at(0).toLongLong()*curfileinfo->fs_info->block_size + curfileinfo->fs_info->offset) << "||Byte Offset for the first block of the file in bytes" << endl;
        else
        {
            if(curfileinfo->fs_info->ftype == TSK_FS_TYPE_NTFS_DETECT)
            {
                NTFS_INFO* ntfsinfo = (NTFS_INFO*)curfileinfo->fs_info;
                int recordsize = 0;
                if(ntfsinfo->fs->mft_rsize_c > 0)
                    recordsize = ntfsinfo->fs->mft_rsize_c * ntfsinfo->fs->csize * tsk_getu16(curfileinfo->fs_info->endian, ntfsinfo->fs->ssize);
                else
                    recordsize = 1 << -ntfsinfo->fs->mft_rsize_c;
                proplist << "Resident Offset||" << QString::number(((tsk_getu16(curfileinfo->fs_info->endian, ntfsinfo->fs->ssize) * ntfsinfo->fs->csize * tsk_getu64(curfileinfo->fs_info->endian, ntfsinfo->fs->mft_clust)) + (recordsize * curfileinfo->meta->addr)) + curfileinfo->fs_info->offset + adssize) << "||Resident offset within the MFT for the file's contents" << endl;
            }
        }
        proplist.flush();
        if(adspropfile.isOpen())
        {
            adspropfile.close();
        }
    }
}

void WriteFileProperties(TSK_FS_FILE* curfileinfo, AddEvidenceVariable* aevar)
{
    QFile filepropfile;
    if(curfileinfo->name->meta_addr == 0 && strcmp(curfileinfo->name->name, "$MFT") != 0)
        filepropfile.setFileName(aevar->partitionpath + "f*" + QString::number(orphancount) + ".a" + QString::number(curfileinfo->name->par_addr) + ".prop");
    else
        filepropfile.setFileName(aevar->partitionpath + "f" + QString::number(curfileinfo->name->meta_addr) + ".a" + QString::number(curfileinfo->name->par_addr) + ".prop");
    filepropfile.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream proplist(&filepropfile);
    if(curfileinfo->name != NULL) proplist << "Short Name||" << curfileinfo->name->shrt_name << "||Short Name for a file" << endl;
    if(curfileinfo->meta != NULL)
    {
        proplist << "Target File Name||" << QString(curfileinfo->meta->link) << "||Name of target file if this is a symbolic link" << endl;
        proplist << "Link Count||" << QString::number(curfileinfo->meta->nlink) << "||Number of files pointing to this file" << endl;
        proplist << "File Permissions||" << GetFilePermissions(curfileinfo->meta) << "||Unix Style Permissions. r - file, d - directory, l - symbolic link, c - character device, b - block device, p - named pipe, v - virtual file created by the forensic tool; r - read, w - write, x - execute, s - set id and executable, S - set id, t - sticky bit executable, T - sticky bit. format is type|user|group|other - [rdlcbpv]|rw[sSx]|rw[sSx]|rw[tTx]" << endl;
        proplist << "User ID||" << QString::number(curfileinfo->meta->uid) << "||User ID" << endl;
        proplist << "Group ID||" << QString::number(curfileinfo->meta->gid) << "||Group ID" << endl;
        proplist << "Allocation Status||";
        if((curfileinfo->meta->flags & TSK_FS_META_FLAG_ALLOC) == 1)
            proplist << "Allocated,";
        if((curfileinfo->meta->flags & TSK_FS_META_FLAG_UNALLOC) == 2)
            proplist << "Unallocated,";
        if((curfileinfo->meta->flags & TSK_FS_META_FLAG_USED) == 4)
            proplist << "Used,";
        if((curfileinfo->meta->flags & TSK_FS_META_FLAG_UNUSED) == 8)
            proplist << "Unused,";
        if((curfileinfo->meta->flags & TSK_FS_META_FLAG_COMP) == 16)
            proplist << "Compressed,";
        if((curfileinfo->meta->flags & TSK_FS_META_FLAG_ORPHAN) == 32)
            proplist << "Orphan,";
        proplist << "||allocation status for the file." << endl;
        proplist << "Attributes||";
        QString attrstr = "";
        int attrcnt = tsk_fs_file_attr_getsize(curfileinfo);
        for(int i=0; i < attrcnt; i++)
        {
            const TSK_FS_ATTR* fsattr = tsk_fs_file_attr_get_idx(curfileinfo, i);
            if(fsattr->type == 0) attrstr += "Not Found,";
            else if(fsattr->type == 1) attrstr += "Default,";
            else if(fsattr->type == 16) attrstr += "$STANDARD_INFOMRATION,";
            else if(fsattr->type == 32) attrstr += "$ATTRIBUTE_LIST,";
            else if(fsattr->type == 48) attrstr += "$FILE_NAME,";
            else if(fsattr->type == 64) attrstr += "$OBJECT_ID,";
            else if(fsattr->type == 80) attrstr += "SECURITY_DESCRIPTOR,";
            else if(fsattr->type == 96) attrstr += "$VOLUME_NAME,";
            else if(fsattr->type == 112) attrstr += "$VOLUME_INFORMATION,";
            else if(fsattr->type == 128) attrstr += "$DATA,";
            else if(fsattr->type == 144) attrstr += "$INDEX_ROOT,";
            else if(fsattr->type == 160) attrstr += "$INDEX_ALLOCATION,";
            else if(fsattr->type == 176) attrstr += "$BITMAP,";
            else if(fsattr->type == 192) attrstr += "$REPARSE_POINT,";
            else if(fsattr->type == 208) attrstr += "$EA_INFORMATION,";
            else if(fsattr->type == 224) attrstr += "$EA,";
            else if(fsattr->type == 256) attrstr += "$LOGGED_UTILITY_STREAM,";
        }
        proplist << attrstr << "||Attributes Types" << endl;
    }
    proplist << "Block Address||" << GetBlockList(curfileinfo) << "||List of block addresses which contain the contents of the file" << endl;
    if(GetBlockList(curfileinfo).compare("") == 0 || GetBlockList(curfileinfo).compare("0^^") == 0)
    {
        if(curfileinfo->fs_info->ftype == TSK_FS_TYPE_NTFS_DETECT)
        {
            NTFS_INFO* ntfsinfo = (NTFS_INFO*)curfileinfo->fs_info;

            int recordsize = 0;
            if(ntfsinfo->fs->mft_rsize_c > 0)
            {
                recordsize = ntfsinfo->fs->mft_rsize_c * ntfsinfo->fs->csize * tsk_getu16(curfileinfo->fs_info->endian, ntfsinfo->fs->ssize);
            }
            else
                recordsize = 1 << -ntfsinfo->fs->mft_rsize_c;
            if(curfileinfo->meta != NULL)
                proplist << "Resident Offset||" << QString::number(((tsk_getu16(curfileinfo->fs_info->endian, ntfsinfo->fs->ssize) * ntfsinfo->fs->csize * tsk_getu64(curfileinfo->fs_info->endian, ntfsinfo->fs->mft_clust)) + (recordsize * curfileinfo->meta->addr)) + curfileinfo->fs_info->offset) << "||" << endl;
        }
        else
            proplist << "Byte Offset||" << QString::number(curfileinfo->fs_info->offset) << "||Byte Offset for the first block of the file" << endl;
    }
    else
        proplist << "Byte Offset||" << QString::number(GetBlockList(curfileinfo).split("^^", QString::SkipEmptyParts).at(0).toLongLong()*curfileinfo->fs_info->block_size + curfileinfo->fs_info->offset) << "||Byte Offset for the first block of the file in bytes" << endl;
    proplist.flush();
    if(filepropfile.isOpen())
    {
        filepropfile.close();
    }
}

void WriteFileSystemProperties(TSK_FS_INFO* curfsinfo, QString partitionpath)
{
    FFS_INFO* ffs = NULL;
    ffs_sb1* sb1 = NULL;
    ffs_sb2* sb2 = NULL;
    FATFS_INFO* fatfs = NULL;
    FATXXFS_SB* fatsb = NULL;
    NTFS_INFO* ntfsinfo = NULL;
    EXFATFS_MASTER_BOOT_REC* exfatsb = NULL;
    HFS_INFO* hfs = NULL;
    hfs_plus_vh* hsb = NULL;
    ISO_INFO* iso = NULL;
    iso9660_pvd_node* p = NULL;
    iso9660_svd_node* s = NULL;
    YAFFSFS_INFO* yfs = NULL;
    EXT2FS_INFO* ext2fs = NULL;
    char asc[512];
    char asc128[129];
    char timebuf[128];
    QFile fspropfile(partitionpath + "prop");
    fspropfile.open(QIODevice::WriteOnly | QIODevice::Text);
    //qDebug() << "fspropfile: open";
    QTextStream proplist(&fspropfile);
    if(curfsinfo->ftype == TSK_FS_TYPE_EXT2 || curfsinfo->ftype == TSK_FS_TYPE_EXT3 || curfsinfo->ftype == TSK_FS_TYPE_EXT4 || curfsinfo->ftype == TSK_FS_TYPE_EXT_DETECT)
    {
        ext2fs = (EXT2FS_INFO*)curfsinfo;
        proplist << "File System Type||";
        if(curfsinfo->ftype == TSK_FS_TYPE_EXT2)
            proplist << "ext2";
        else if(curfsinfo->ftype == TSK_FS_TYPE_EXT3)
            proplist << "ext3";
        else if(curfsinfo->ftype == TSK_FS_TYPE_EXT4)
            proplist << "ext4";
        else
            proplist << "ext2";
        proplist << "||" << endl;
        proplist << "Inode Count||" << QString::number(tsk_getu32(curfsinfo->endian, ext2fs->fs->s_inodes_count)) << "||Number of Inodes in the file system (0x00-0x03)" << endl;
        proplist << "Block Count||" << QString::number(tsk_getu32(curfsinfo->endian, ext2fs->fs->s_inodes_count)) << "||Number of Blocks in the file system (0x04-0x07)" << endl;
        proplist << "Reserved Blocks||" << QString::number(tsk_getu32(curfsinfo->endian, ext2fs->fs->s_r_blocks_count)) << "||Number of blocks reserved to prevent the file system from filling up (0x08-0x0B)" << endl;
        proplist << "Unallocated Blocks||" << QString::number(tsk_getu32(curfsinfo->endian, ext2fs->fs->s_free_blocks_count)) << "||Number of unallocated blocks (0x0C-0x0F)" << endl;
        proplist << "Unallocated Inodes||" << QString::number(tsk_getu32(curfsinfo->endian, ext2fs->fs->s_free_inode_count)) << "||Number of unnalocated inodes (0x10-0x13)" << endl;
        proplist << "First Data Block||" << QString::number(tsk_getu32(curfsinfo->endian, ext2fs->fs->s_first_data_block)) << "||Block where block group 0 starts (0x14-0x17)" << endl;
        proplist << "Log Block Size||" << QString::number(tsk_getu32(curfsinfo->endian, ext2fs->fs->s_log_block_size)) << "||Block size saved as the number of places to shift 1,024 to the left (0x18-0x1B)" << endl;
        proplist << "Log Fragment Size||" << QString::number(tsk_getu32(curfsinfo->endian, ext2fs->fs->s_log_frag_size)) << "||Fragment size saved as the number of bits to shift 1,024 to the left (0x1C-0x1F)" << endl;
        proplist << "Blocks per Group||" << QString::number(tsk_getu32(curfsinfo->endian, ext2fs->fs->s_blocks_per_group)) << "||Number of blocks in each block group (0x20-0x23)" << endl;
        proplist << "Fragments per Group||" << QString::number(tsk_getu32(curfsinfo->endian, ext2fs->fs->s_frags_per_group)) << "||Number of fragments in each block group (0x24-0x27)" << endl;
        proplist << "Inodes per Group||" << QString::number(tsk_getu32(curfsinfo->endian, ext2fs->fs->s_inodes_per_group)) << "||Number of inodes in each block group (0x28-0x2B)" << endl;
        sprintf(asc, "%s", (tsk_getu32(curfsinfo->endian, ext2fs->fs->s_mtime) > 0) ? tsk_fs_time_to_str(tsk_getu32(curfsinfo->endian, ext2fs->fs->s_mtime), timebuf) : "empty");
        proplist << "Last Mount Time||" << QString::fromStdString(std::string(asc)) << "||Last time the file system was mounted (0x2C-0x2F)" << endl;
        sprintf(asc, "%s", (tsk_getu32(curfsinfo->endian, ext2fs->fs->s_wtime) > 0) ? tsk_fs_time_to_str(tsk_getu32(curfsinfo->endian, ext2fs->fs->s_wtime), timebuf) : "empty");
        proplist << "Last Written Time||" << QString::fromStdString(std::string(asc)) << "||Last time data was written to the file system (0x30-0x33)" << endl;
        proplist << "Current Mount Count||" << QString::number(tsk_getu16(curfsinfo->endian, ext2fs->fs->s_mnt_count)) << "||Number of times the file system has been mounted (0x34-0x35)" << endl;
        proplist << "Maximum Mount Count||" << QString::number(tsk_getu16(curfsinfo->endian, ext2fs->fs->s_max_mnt_count)) << "||Maximum number of times the file system can be mounted (0x36-0x37)" << endl;
        proplist << "Signature||" << QString::number(tsk_getu16(curfsinfo->endian, ext2fs->fs->s_magic)) << "||File system signature value should be 0xef53 (0x38-0x39)" << endl;
        proplist << "File System State||";
        if(tsk_getu16(curfsinfo->endian, ext2fs->fs->s_state) & EXT2FS_STATE_VALID)
            proplist << "Unmounted properly";
        else if(tsk_getu16(curfsinfo->endian, ext2fs->fs->s_state) & EXT2FS_STATE_ERROR)
            proplist << "Errors Detected";
        else
            proplist << "Orphan inodes were being recovered";
        proplist << "||State of the file system when it was last shut down (0x3A-0x3B)" << endl;
        proplist << "Error Handling Method||";
        if(tsk_getu16(curfsinfo->endian, ext2fs->fs->s_errors) == 1)
            proplist << "Continue";
        else if(tsk_getu16(curfsinfo->endian, ext2fs->fs->s_errors) == 2)
            proplist << "Remount as Read-Only";
        else
            proplist << "Panic";
        proplist << "||Identifies what the OS should do when it encounters a file system error (0x3C-0x3D)" << endl;
        proplist << "Minor Version||" << QString::number(tsk_getu16(curfsinfo->endian, ext2fs->fs->s_minor_rev_level)) << "||Minor Revision Level (0x3E-0x3F)" << endl;
        sprintf(asc, "%s", (tsk_getu32(curfsinfo->endian, ext2fs->fs->s_lastcheck) > 0) ? tsk_fs_time_to_str(tsk_getu32(curfsinfo->endian, ext2fs->fs->s_lastcheck), timebuf) : "empty");
        proplist << "Last Checked||" << QString::fromStdString(std::string(asc)) << "||Last time the consistency of the file system was checked (0x40-0x43)" << endl;
        proplist << "Interval b/w Checks||" << QString::number(tsk_getu32(curfsinfo->endian, ext2fs->fs->s_checkinterval)) << "||Interval between forced consistency checks (0x44-0x47)" << endl;
        proplist << "Creator OS||";
        switch(tsk_getu32(curfsinfo->endian, ext2fs->fs->s_creator_os))
        {
            case EXT2FS_OS_LINUX:
                proplist << "Linux";
                break;
            case EXT2FS_OS_HURD:
                proplist << "HURD";
                break;
            case EXT2FS_OS_MASIX:
                proplist << "MASIX";
                break;
            case EXT2FS_OS_FREEBSD:
                proplist << "FreeBSD";
                break;
            case EXT2FS_OS_LITES:
                proplist << "LITES";
                break;
            default:
                proplist << "Unknown";
                break;
        }
        proplist << "||OS that might have created the file system (0x48-0x4B)" << endl;
        proplist << "Major Version||";
        if(tsk_getu32(curfsinfo->endian, ext2fs->fs->s_rev_level) == EXT2FS_REV_ORIG)
            proplist << "Static Structure";
        else
            proplist << "Dynamic Structure";
        proplist << "||If the version is not set to dynamic, the values from bytes 84 and up might not be accurate (0x4C-0x4F)" << endl;
        proplist << "UID to Use Reserved Blocks||" << QString::number(tsk_getu16(curfsinfo->endian, ext2fs->fs->s_def_resuid)) << "||User ID that can use reserved blocks (0x50-0x51)" << endl;
        proplist << "GID to Use Reserved Blocks||" << QString::number(tsk_getu16(curfsinfo->endian, ext2fs->fs->s_def_resgid)) << "||Group ID that can use reserved blocks (0x52-0x53)" << endl;
        proplist << "First Non-Reserved Inode||" << QString::number(tsk_getu32(curfsinfo->endian, ext2fs->fs->s_first_ino)) << "||First non-reserved inode in the file system (0x54-0x57)" << endl;
        proplist << "Inode Structure Size||" << QString::number(tsk_getu16(curfsinfo->endian, ext2fs->fs->s_inode_size)) << "||Size of each inode structure (0x58-0x59)" << endl;
        proplist << "Block Group for SuperBlock||" << QString::number(tsk_getu16(curfsinfo->endian, ext2fs->fs->s_block_group_nr)) << "||Superblock is part of the block group (if backup copy) (0x5A-0x5B)" << endl;
        if(tsk_getu32(curfsinfo->endian, ext2fs->fs->s_feature_compat) & EXT2FS_FEATURE_COMPAT_DIR_PREALLOC)
            proplist << "Compatible Feature||" << "Directory Pre-allocation" << "||Pre-allocate directory blocks to reduce fragmentation. The OS can mount the file system as normal if it does not support a compatible feature (0x5C-0x5F)" << endl;
        if(tsk_getu32(curfsinfo->endian, ext2fs->fs->s_feature_compat) & EXT2FS_FEATURE_COMPAT_IMAGIC_INODES)
            proplist << "Compatible Feature||" << "IMagic Inodes" << "||AFS server inodes exist (0x5C-0x5F)" << endl;
        if(tsk_getu32(curfsinfo->endian, ext2fs->fs->s_feature_compat) & EXT2FS_FEATURE_COMPAT_HAS_JOURNAL)
            proplist << "Compatible Feature||" << "Journal" << "||File System has a journal (0x5C-0x5F)" << endl;
        if(tsk_getu32(curfsinfo->endian, ext2fs->fs->s_feature_compat) & EXT2FS_FEATURE_COMPAT_EXT_ATTR)
            proplist << "Compatible Feature||" << "Extended Attributes" << "||Inodes have extended attributes (0x5C-0x5F)" << endl;
        if(tsk_getu32(curfsinfo->endian, ext2fs->fs->s_feature_compat) & EXT2FS_FEATURE_COMPAT_RESIZE_INO)
            proplist << "Compatible Feature||" << "Resizable File System" << "||File system can resize itself for larger aptitions (0x5C-0x5F)" << endl;
        if(tsk_getu32(curfsinfo->endian, ext2fs->fs->s_feature_compat) & EXT2FS_FEATURE_COMPAT_DIR_INDEX)
            proplist << "Compatible Feature||" << "Directory Index" << "||Directories use hash index (0x5C-0x5F)" << endl;
        if(tsk_getu32(curfsinfo->endian, ext2fs->fs->s_feature_incompat) & EXT2FS_FEATURE_INCOMPAT_COMPRESSION)
            proplist << "Incompatible Feature||" << "Compression" << "||The OS should not mount the file system if it does not support this incompatible feature (0x60-0x63)" << endl;
        if(tsk_getu32(curfsinfo->endian, ext2fs->fs->s_feature_incompat) & EXT2FS_FEATURE_INCOMPAT_FILETYPE)
            proplist << "Incompatible Feature||" << "Filetype" << "||Directory entries contain a file type field (0x60-0x63)" << endl;
        if(tsk_getu32(curfsinfo->endian, ext2fs->fs->s_feature_incompat) & EXT2FS_FEATURE_INCOMPAT_RECOVER)
            proplist << "Incompatible Feature||" << "Needs Recovery" << "||The file systems needs to be recovered (0x60-0x63)" << endl;
        if(tsk_getu32(curfsinfo->endian, ext2fs->fs->s_feature_incompat) & EXT2FS_FEATURE_INCOMPAT_JOURNAL_DEV)
            proplist << "Incompatible Feature||" << "Journal Device" << "||The file system uses a journal device (0x60-0x63)" << endl;
        if(tsk_getu32(curfsinfo->endian, ext2fs->fs->s_feature_incompat) & EXT2FS_FEATURE_INCOMPAT_META_BG)
            proplist << "Incompatible Feature||" << "Meta Block Groups" << "||The file system has meta block groups (0x60-0x63)" << endl;
        if(tsk_getu32(curfsinfo->endian, ext2fs->fs->s_feature_incompat) & EXT2FS_FEATURE_INCOMPAT_EXTENTS)
            proplist << "Incompatible Feature||" << "Extents" << "||The file system uses extents (0x60-0x63)" << endl;
        if(tsk_getu32(curfsinfo->endian, ext2fs->fs->s_feature_incompat) & EXT2FS_FEATURE_INCOMPAT_64BIT)
            proplist << "Incompatible Feature||" << "64-bit" << "||The file system is 64-bit capable (0x60-0x63)" << endl;
        if(tsk_getu32(curfsinfo->endian, ext2fs->fs->s_feature_incompat) & EXT2FS_FEATURE_INCOMPAT_MMP)
            proplist << "Incompatible Feature||" << "Multiple Mount Protection" << "||The OS should not mount the file system if it does not support this incompatible feature (0x60-0x63)" << endl;
        if(tsk_getu32(curfsinfo->endian, ext2fs->fs->s_feature_incompat) & EXT2FS_FEATURE_INCOMPAT_FLEX_BG)
            proplist << "Incompatible Feature||" << "Flexible Block Groups" << "||The OS should not mount the file system if it does not support this incompatible feature (0x60-0x63)" << endl;
        if(tsk_getu32(curfsinfo->endian, ext2fs->fs->s_feature_incompat) & EXT2FS_FEATURE_INCOMPAT_EA_INODE)
            proplist << "Incompatible Feature||" << "Extended Attributes" << "||The file system supports extended attributes (0x60-0x63)" << endl;
        if(tsk_getu32(curfsinfo->endian, ext2fs->fs->s_feature_incompat) & EXT2FS_FEATURE_INCOMPAT_DIRDATA)
            proplist << "Incompatible Feature||" << "Directory Entry Data" << "||The OS should not mount the file system if it does not support this incompatible feature (0x60-0x63)" << endl;
        if(tsk_getu32(curfsinfo->endian, ext2fs->fs->s_feature_ro_compat) & EXT2FS_FEATURE_RO_COMPAT_SPARSE_SUPER)
            proplist << "Read only Feature||" << "Sparse Super" << "||Sparse superblocks and goup descriptor tables. The OS should mount the file system as read only if it does not support this read only feature (0x64-0x67)" << endl;
        if(tsk_getu32(curfsinfo->endian, ext2fs->fs->s_feature_ro_compat) & EXT2FS_FEATURE_RO_COMPAT_LARGE_FILE)
            proplist << "Read only Feature||" << "Large File" << "||The OS should mount the file system as read only if it does not support this read only feature (0x64-0x67)" << endl;
        if(tsk_getu32(curfsinfo->endian, ext2fs->fs->s_feature_ro_compat) & EXT2FS_FEATURE_RO_COMPAT_HUGE_FILE)
            proplist << "Read only Feature||" << "Huge File" << "||The OS should mount the file system as read only if it does not support this read only feature (0x64-0x67)" << endl;
        if(tsk_getu32(curfsinfo->endian, ext2fs->fs->s_feature_ro_compat) & EXT2FS_FEATURE_RO_COMPAT_BTREE_DIR)
            proplist << "Read only Feature||" << "BTree Directory" << "||The OS should mount the file system as read only if it does not support this read only feature (0x64-0x67)" << endl;
        if(tsk_getu32(curfsinfo->endian, ext2fs->fs->s_feature_ro_compat) & EXT2FS_FEATURE_RO_COMPAT_EXTRA_ISIZE)
            proplist << "Read only Feature||" << "Extra Inode Size" << "||The OS should mount the file system as read only if it does not support this read only feature (0x64-0x67)" << endl;
        sprintf(asc, "%" PRIx64 "%" PRIx64 "", tsk_getu64(curfsinfo->endian, &(ext2fs->fs)->s_uuid[8]), tsk_getu64(curfsinfo->endian, &(ext2fs->fs)->s_uuid[0]));
        proplist << "File System ID||" << QString::fromStdString(std::string(asc)) << "||File system ID. Found in the superblock at bytes (0x68-0x77)" << endl;
        proplist << "File System Label||" << QString::fromStdString(std::string(ext2fs->fs->s_volume_name)) << "||File System Label. (0x78-0x87)" << endl;
        proplist << "Last Mounted Path||" << QString::fromStdString(std::string(ext2fs->fs->s_last_mounted)) << " ||Path where the file system was last mounted (0x88-0xC7)" << endl;
        proplist << "Algorithm Usage Bitmap||" << QString::number(tsk_getu32(curfsinfo->endian, ext2fs->fs->s_algorithm_usage_bitmap)) << "||(0xC8-0xCB)" << endl;
        proplist << "Blocks Preallocated for Files||" << QString::number(ext2fs->fs->s_prealloc_blocks) << "||Number of blocks to preallocate for files (0xCC-0xCC)" << endl;
        proplist << "Blocks Preallocated for Directories||" << QString::number(ext2fs->fs->s_prealloc_dir_blocks) << "||Number of blocks to preallocate for directories (0xCD-0xCD)" << endl;
        proplist << "Unused||" << "Unused" << "||Unused bytes (0xCE-0xCF)" << endl;
        sprintf(asc, "%" PRIx64 "%" PRIx64 "", tsk_getu64(curfsinfo->endian, &(ext2fs->fs)->s_journal_uuid[8]), tsk_getu64(curfsinfo->endian, &(ext2fs->fs)->s_journal_uuid[0]));
        proplist << "Journal ID||" << QString::fromStdString(std::string(asc)) << "||Journal ID (0xD0-0xDF)" << endl;
        proplist << "Journal Inode||" << QString::number(tsk_getu32(curfsinfo->endian, ext2fs->fs->s_journal_inum)) << "||Journal Inode (0xE0-0xE3)" << endl;
        proplist << "Journal Device||" << QString::number(tsk_getu32(curfsinfo->endian, ext2fs->fs->s_journal_dev)) << "||Journal device (0xE4-0xE7)" << endl;
        proplist << "Head of Oprhan Inode List||" << QString::number(tsk_getu32(curfsinfo->endian, ext2fs->fs->s_last_orphan)) << "||Head of orphan inode list. (0xE8-0xEB)" << endl;
        proplist << "Unused||" << "Unused" << "||Unused (0xEC-0x03FF)" << endl;
    }
    else if(curfsinfo->ftype == TSK_FS_TYPE_FFS1 || curfsinfo->ftype == TSK_FS_TYPE_FFS1B || curfsinfo->ftype == TSK_FS_TYPE_FFS2 || curfsinfo->ftype == TSK_FS_TYPE_FFS_DETECT)
    {
        ffs = (FFS_INFO*)curfsinfo;
        sb1 = ffs->fs.sb1;
        sb2 = ffs->fs.sb2;
        proplist << "File System Type||";
        if(curfsinfo->ftype == TSK_FS_TYPE_FFS1 || curfsinfo->ftype == TSK_FS_TYPE_FFS1B)
            proplist << "UFS 1";
        else
            proplist << "UFS 2";
        proplist << "" << endl;
        if(curfsinfo->ftype == TSK_FS_TYPE_FFS1 || curfsinfo->ftype == TSK_FS_TYPE_FFS1B)
        {
            proplist << "Unused||" << "Unused" << "||Unused (0x00-0x07)" << endl;
            proplist << "Backup Superblock Offset||" << QString::number(tsk_gets32(curfsinfo->endian, sb1->sb_off)) << "||Offset to backup superblock in cylinder group relative to a \"base\" (0x08-0x0B)" << endl;
            proplist << "Group Descriptor Offset||" << QString::number(tsk_gets32(curfsinfo->endian, sb1->gd_off)) << "||Offset to group descriptor in cylinder group relative to a \"base\" (0x0C-0x0F)" << endl;
            proplist << "Inode Table Offset||" << QString::number(tsk_gets32(curfsinfo->endian, sb1->ino_off)) << "||Offset to inode table in cylinder group relative to a \"base\" (0x10-0x13)" << endl;
            proplist << "Data Block Offset||" << QString::number(tsk_gets32(curfsinfo->endian, sb1->dat_off)) << "||Offset to data blocks in cylinder group relative to a \"base\" (0x14-0x17)" << endl;
            proplist << "Delta Value for Staggering Offset||" << QString::number(tsk_gets32(curfsinfo->endian, sb1->cg_delta)) << "||Delta value for caluclating the staggering offset in cylinder group (0x18-0x1B)" << endl;
            proplist << "Mask for Staggering Offset||" << QString::number(tsk_gets32(curfsinfo->endian, sb1->cg_cyc_mask)) << "||Mask for calculating the staggering offset (cycle value) in cylinder group (0x1C-0x1F)" << endl;
            sprintf(asc, "%s", (tsk_getu32(curfsinfo->endian, sb1->wtime) > 0) ? tsk_fs_time_to_str(tsk_getu32(curfsinfo->endian, sb1->wtime), timebuf) : "empty");
            proplist << "Last Written Time||" << QString::fromStdString(std::string(asc)) << "||Last time data was written to the file system (0x20-0x23)" << endl;
            proplist << "Number of Fragments||" << QString::number(tsk_gets32(curfsinfo->endian, sb1->frag_num)) << "||Number of fragments in the file system (0x24-0x27)" << endl;
            proplist << "Number of Data Fragments||" << QString::number(tsk_gets32(curfsinfo->endian, sb1->data_frag_num)) << "||Number of fragments in the file system that can store file data (0x28-0x2B)" << endl;
            proplist << "Number of Cylinder Groups||" << QString::number(tsk_gets32(curfsinfo->endian, sb1->cg_num)) << "||Number of cylinder groups in the file system (0x2C-0x2F)" << endl;
            proplist << "Block Byte Size||" << QString::number(tsk_gets32(curfsinfo->endian, sb1->bsize_b)) << "||Size of a block in bytes (0x30-0x33)" << endl;
            proplist << "Fragment Size||" << QString::number(tsk_gets32(curfsinfo->endian, sb1->fsize_b)) << "||Size of a fragment in bytes (0x34-0x37)" << endl;
            proplist << "Block Framgent Size||" << QString::number(tsk_gets32(curfsinfo->endian, sb1->bsize_frag)) << "||Size of a block in fragments (0x38-0x3B)" << endl;
            proplist << "Non-Essential Values||" << "Non-Essential Values" << "||Non-Essential Values (0x3C-0x5F)" << endl;
            proplist << "Number of Bits Convert Block to Fragment||" << QString::number(tsk_gets32(curfsinfo->endian, sb1->fs_fragshift)) << "||Number of bits to convert between a block address and a fragment address (0x60-0x63)" << endl;
            proplist << "Non-Essential Values||" << "Non-Essential Values" << "||Non-Essential Values (0x64-0x77)" << endl;
            proplist << "Inodes Per Block||" << QString::number(tsk_gets32(curfsinfo->endian, sb1->fs_inopb)) << "||Number of inodes per block in the inode table (0x78-0x7B)" << endl;
            proplist << "Non-Essential Values||" << "Non-Essential Values" << "||Non-Essential Values (0x7C-0x8F)" << endl;
            sprintf(asc, "%" PRIx32 "%" PRIx32 "", tsk_getu32(curfsinfo->endian, &sb1->fs_id[4]), tsk_getu32(curfsinfo->endian, &sb1->fs_id[0]));
            proplist << "File System ID||" << QString::fromStdString(std::string(asc)) << "||File system ID (0x90-0x97)" << endl;
            proplist << "Cylinder Group Fragment Address||" << QString::number(tsk_gets32(curfsinfo->endian, sb1->cg_saddr)) << "||Fragment address of the cylinder group summary area (0x98-0x9B)" << endl;
            proplist << "Cylinder Group Summary Area Byte Size||" << QString::number(tsk_gets32(curfsinfo->endian, sb1->cg_ssize_b)) << "||Size of the cylinder group summary area in bytes (0x9C-0x9F)" << endl;
            proplist << "Cylinder Group Descriptor Byte Size||" << QString::number(tsk_gets32(curfsinfo->endian, sb1->fs_cgsize)) << "||Size of the cylinder group descriptor in bytes (0xA0-0xA3)" << endl;
            proplist << "Non-Essential Values||" << "Non-Essential Values" << "||Non-Essential Values (0xA4-0xA5)" << endl;
            proplist << "Cylinders in File System||" << QString::number(tsk_gets32(curfsinfo->endian, sb1->fs_ncyl)) << "||Number of cylinders in the file system (0xA6-0xB3)" << endl;
            proplist << "Cylinders per Cylinder Group||" << QString::number(tsk_gets32(curfsinfo->endian, sb1->fs_cpg)) << "||Number of cylinders in a cylinder group (0xB4-0xB7)" << endl;
            proplist << "Inodes per Cylinder Group||" << QString::number(tsk_gets32(curfsinfo->endian, sb1->cg_inode_num)) << "||Number of inodes in a cylinder group (0xB8-0xBB)" << endl;
            proplist << "Fragments per Cylinder Group||" << QString::number(tsk_gets32(curfsinfo->endian, sb1->cg_frag_num)) << "||Number of fragments in a cylinder group (0xBC-0xBF)" << endl;
            proplist << "Number of Directories||" << QString::number(tsk_gets32(curfsinfo->endian, sb1->cstotal.dir_num)) << "||Number of directories (0xC0-0xC3)" << endl;
            proplist << "Number of Free Blocks||" << QString::number(tsk_gets32(curfsinfo->endian, sb1->cstotal.blk_free)) << "||Number of free blocks (0xC4-0xC7)" << endl;
            proplist << "Number of Free Inodes||" << QString::number(tsk_gets32(curfsinfo->endian, sb1->cstotal.ino_free)) << "||Number of free inodes (0xC8-0xCB)" << endl;
            proplist << "Number of Free Fragments||" << QString::number(tsk_gets32(curfsinfo->endian, sb1->cstotal.frag_free)) << "||Number of free fragments (0xCC-0xCF)" << endl;
            proplist << "Super Block Modified Flag||" << QString::number(sb1->fs_fmod) << "||Super Block Modified Flag (0xD0-0xD0)" << endl;
            proplist << "Clean File System Flag||" << QString::number(sb1->fs_clean) << "||File system was clean when it was mounted (0xD1-0xD1)" << endl;
            proplist << "Read Only File System Flag||" << QString::number(sb1->fs_ronly) << "||File system was mounted read only (0xD2-0xD2)" << endl;
            if(sb1->fs_flags & FFS_SB_FLAG_UNCLEAN)
                proplist << "General Flags||" << "Unclean" << "||Set when the file system is mounted (0xD3-0xD3)" << endl;
            if(sb1->fs_flags & FFS_SB_FLAG_SOFTDEP)
                proplist << "General Flags||" << "Soft Dependencies" << "||Soft dependencies are being used (0xD3-0xD3)" << endl;
            if(sb1->fs_flags & FFS_SB_FLAG_NEEDFSCK)
                proplist << "General Flags||" << "Needs Check" << "||Needs consistency check next time the file system is mounted (0xD3-0xD3)" << endl;
            if(sb1->fs_flags & FFS_SB_FLAG_INDEXDIR)
                proplist << "General Flags||" << "Index Directories" << "||Directories are indexed using a hashtree or B-Tree (0xD3-0xD3)" << endl;
            if(sb1->fs_flags & FFS_SB_FLAG_ACL)
                proplist << "General Flags||" << "Access Control Lists" << "||Access control lists are being used (0xD3-0xD3)" << endl;
            if(sb1->fs_flags & FFS_SB_FLAG_MULTILABEL)
                proplist << "General Flags||" << "TrustedBSD MAC Multi-Label" << "||TrustedBSD Mandatory Access Control multi-labels are being used (0xD3-0xD3)" << endl;
            if(sb1->fs_flags & FFS_SB_FLAG_UPDATED)
                proplist << "General Flags||" << "Updated Flag Location" << "||Flags have been moved (0xD3-0xD3)" << endl;
            proplist << "Last Mount Point||" << QString::fromStdString(std::string(reinterpret_cast<char*>(sb1->last_mnt))) << "||Last mount point (0xD4-0x02D3)" << endl;
            proplist << "Non-Essential Values||" << "Non-Essential Values" << "||Non-Essential Values (0x02D4-0x055B)" << endl;
            proplist << "Signature||" << QString::number(tsk_gets32(curfsinfo->endian, sb1->magic)) << "||File System signature value should be 0x011954 (0x055C-0x055F)" << endl;
        }
        else // FFS2
        {
            proplist << "Unused||" << "Unused" << "||Unused (0x00-0x07)" << endl;
            proplist << "Backup Superblock Offset||" << QString::number(tsk_gets32(curfsinfo->endian, sb2->sb_off)) << "||Offset to backup superblock in cylinder group relative to a \"base\" (0x08-0x0B)" << endl;
            proplist << "Group Descriptor Offset||" << QString::number(tsk_gets32(curfsinfo->endian, sb2->gd_off)) << "||Offset to group descriptor in cylinder group relative to a \"base\" (0x0C-0x0F)" << endl;
            proplist << "Inode Table Offset||" << QString::number(tsk_gets32(curfsinfo->endian, sb2->ino_off)) << "||Offset to inode table in cylinder group relative to a \"base\" (0x10-0x13)" << endl;
            proplist << "First Data Block Offset||" << QString::number(tsk_gets32(curfsinfo->endian, sb2->dat_off)) << "||Offset to the first data block in the cylinder group relative to a \"base\" (0x14-0x17)" << endl;
            proplist << "Unused||" << "Unused" << "||Unused (0x18-0x2B)" << endl;
            proplist << "Number of Cylinder Groups||" << QString::number(tsk_gets32(curfsinfo->endian, sb2->cg_num)) << "||The Number of cylinder groups in the file system (0x2C-0x2F)" << endl;
            proplist << "Block Byte Size||" << QString::number(tsk_gets32(curfsinfo->endian, sb2->bsize_b)) << "||Size of a block in bytes (0x30-0x33)" << endl;
            proplist << "Fragment Size||" << QString::number(tsk_gets32(curfsinfo->endian, sb2->fsize_b)) << "||Size of a fragment in bytes (0x34-0x37)" << endl;
            proplist << "Block Fragment Size||" << QString::number(tsk_gets32(curfsinfo->endian, sb2->bsize_frag)) << "||Size of a block in fragments (0x38-0x3B)" << endl;
            proplist << "Unused||" << "Unused" << "||Unused (0x3C-0x5F)" << endl;
            proplist << "File System Fragment Shift||" << QString::number(tsk_gets32(curfsinfo->endian, sb2->fs_fragshift)) << "||Number of bits to convert between a block address and a fragment address (0x60-0x63)" << endl;
            proplist << "Unused||" << "Unused" << "||Unused (0x64-0x77)" << endl;
            proplist << "Inodes Per Block||" << QString::number(tsk_gets32(curfsinfo->endian, sb2->fs_inopb)) << "||Number of inodes per block in inode table (0x78-0x7B)" << endl;
            proplist << "Unused||" << "Unused" << "||Unused (0x7C-0x9B)" << endl;
            proplist << "Cylinder Group Summary Size||" << QString::number(tsk_gets32(curfsinfo->endian, sb2->cg_ssize_b)) << "||Size of cylinder group summary area in bytes (0x9C-0x9F)" << endl;
            proplist << "Cylinder Group Descriptor Size||" << QString::number(tsk_gets32(curfsinfo->endian, sb2->fs_cgsize)) << "||Size of cylinder group descriptor in bytes (0xA0-0xA3)" << endl;
            proplist << "Unused||" << "Unused" << "||Unused (0xA4-0xB7)" << endl;
            proplist << "Cylinder Group Inodes||" << QString::number(tsk_gets32(curfsinfo->endian, sb2->cg_inode_num)) << "||Inodes per cylinder group (0xB8-0xBB)" << endl;
            proplist << "Cylinder Group Fragments||" << QString::number(tsk_gets32(curfsinfo->endian, sb2->cg_frag_num)) << "||Fragments per cylinder group (0xBC-0xBF)" << endl;
            proplist << "Unused||" << "Unused" << "||Unused (0xC0-0xCF)" << endl;
            proplist << "Super Block Modified Flag||" << QString::number(sb2->fs_fmod) << "||Super block modified flag (0xD0-0xD0)" << endl;
            proplist << "Clean File System||" << QString::number(sb2->fs_clean) << "||File system was clean when it was mounted (0xD1-0xD1)" << endl;
            proplist << "Read Only Flag||" << QString::number(sb2->fs_ronly) << "||File system was mounted read only (0xD2-0xD2)" << endl;
            proplist << "Unused||" << "Unused" << "||Unused (0xD3-0xD3)" << endl;
            proplist << "Last Mount Point||" << QString::fromUtf8(reinterpret_cast<char*>(sb2->last_mnt)) << "||Last mount point (0xD4-0x02A7)" << endl;
            proplist << "Volume Name||" << QString::fromUtf8(reinterpret_cast<char*>(sb2->volname)) << "||Volume name (0x02A8-0x02C7)" << endl;
            proplist << "System UID||" << QString::fromUtf8(reinterpret_cast<char*>(sb2->swuid)) << "||System UID (0x02C8-0x02CF)" << endl;
            proplist << "Unused||" << "Unused" << "||Unused (0x02D0-0x03EF)" << endl;
            proplist << "Number of Directories||" << QString::number(tsk_getu64(curfsinfo->endian, sb2->cstotal.dir_num)) << "||Number of directories (0x03F0-0x03F7)" << endl;
            proplist << "Number of Free Blocks||" << QString::number(tsk_getu64(curfsinfo->endian, sb2->cstotal.blk_free)) << "||Number of free blocks (0x03F8-0x03FF)" << endl;
            proplist << "Number of Free Inodes||" << QString::number(tsk_getu64(curfsinfo->endian, sb2->cstotal.ino_free)) << "||Number of free inodes (0x0400-0x0407)" << endl;
            proplist << "Number of Free Fragments||" << QString::number(tsk_getu64(curfsinfo->endian, sb2->cstotal.frag_free)) << "||Number of free fragments (0x0408-0x040F)" << endl;
            proplist << "Number of Free Clusters||" << QString::number(tsk_getu64(curfsinfo->endian, sb2->cstotal.clust_free)) << "||Number of free clusters (0x0410-0x0417)" << endl;
            proplist << "Unused||" << "Unused" << "||Unused (0x0418-0x042F)" << endl;
            sprintf(asc, "%s", (tsk_getu64(curfsinfo->endian, sb1->wtime) > 0) ? tsk_fs_time_to_str(tsk_getu64(curfsinfo->endian, sb1->wtime), timebuf) : "empty");
            proplist << "Last Written Time||" << QString::fromStdString(std::string(asc)) << "||Last time data was written to the file system (0x0430-0x0437)" << endl;
            proplist << "Fragment Numbers||" << QString::number(tsk_gets64(curfsinfo->endian, sb2->frag_num)) << "||Number of fragments in the file system (0x0438-0x043F)" << endl;
            proplist << "Usable Fragment Numbers||" << QString::number(tsk_gets64(curfsinfo->endian, sb2->blk_num)) << "||Number of fragments that can store file data (0x0440-0x0447)" << endl;
            proplist << "Cylinder Group Fragment Address||" << QString::number(tsk_gets64(curfsinfo->endian, sb2->cg_saddr)) << "||Fragment address of cylinder group summary area (0x0448-0x044F)" << endl;
            proplist << "Unused||" << "Unused" << "||Unused (0x0450-0x051F)" << endl;
            int flags = tsk_getu32(curfsinfo->endian, sb2->fs_flags);
            if(flags & FFS_SB_FLAG_UNCLEAN)
                proplist << "General Flags||" << "Unclean" << "||Set when the file system is mounted (0x0520-0x0523)" << endl;
            if(flags & FFS_SB_FLAG_SOFTDEP)
                proplist << "General Flags||" << "Soft Dependencies" << "||Soft dependencies are being used (0x0520-0x0523)" << endl;
            if(flags & FFS_SB_FLAG_NEEDFSCK)
                proplist << "General Flags||" << "Needs Check" << "||Needs consistency check next time the file system is mounted (0x0520-0x0523)" << endl;
            if(flags & FFS_SB_FLAG_INDEXDIR)
                proplist << "General Flags||" << "Index Directories" << "||Directories are indexed using a hashtree or B-Tree (0x0520-0x0523)" << endl;
            if(flags & FFS_SB_FLAG_ACL)
                proplist << "General Flags||" << "Access Control Lists" << "||Access control lists are being used (0x0520-0x0523)" << endl;
            if(flags & FFS_SB_FLAG_MULTILABEL)
                proplist << "General Flags||" << "TrustedBSD MAC Multi-Label" << "||TrustedBSD Mandatory Access Control multi-labels are being used (0x0520-0x0523)" << endl;
            if(flags & FFS_SB_FLAG_UPDATED)
                proplist << "General Flags||" << "Updated Flag Location" << "||Flags have been moved (0x0520-0x0523)" << endl;
            proplist << "Unused" << "Unused||" << "||Unused (0x0524-0x055B)" << endl;
            proplist << "Signature||" << QString::number(tsk_gets32(curfsinfo->endian, sb2->magic)) << "||File system signature value should be 0x19540119 (0x055C-0x055F)" << endl;
        }
    }
    else if(curfsinfo->ftype == TSK_FS_TYPE_FAT12 || curfsinfo->ftype == TSK_FS_TYPE_FAT16 || curfsinfo->ftype == TSK_FS_TYPE_FAT32)
    {
        fatfs = (FATFS_INFO*)curfsinfo;
        fatsb = (FATXXFS_SB*)fatfs->boot_sector_buffer;
        char* data_buf;
        ssize_t cnt;
        if((data_buf = (char*)tsk_malloc(curfsinfo->block_size)) != NULL)
        {
            // print error
        }
        cnt = tsk_fs_read_block(curfsinfo, fatfs->rootsect, data_buf, curfsinfo->block_size);
        if(cnt != curfsinfo->block_size)
        {
            // print error
        }
        free(data_buf);
        proplist << "File System Type||";
        if(curfsinfo->ftype == TSK_FS_TYPE_FAT12)
            proplist << "FAT12";
        else if(curfsinfo->ftype == TSK_FS_TYPE_FAT16)
            proplist << "FAT16";
        else if(curfsinfo->ftype == TSK_FS_TYPE_FAT32)
            proplist << "FAT32";
        proplist << "||File System Type" << endl;
        proplist << "Reserved||" << "Jump Code" << "||Assembly instructions to jump to boot code (0x00-0x02)" << endl;
        proplist << "OEM Name||" << QString::fromUtf8(reinterpret_cast<char*>(fatsb->oemname)) << "||OEM name in ASCII (0x03-0x0A)" << endl;
        proplist << "Bytes per Sector||" << QString::number(tsk_getu16(curfsinfo->endian, fatsb->ssize)) << "||Sector size in bytes. Allowed values include 512, 1024, 2048, and 4096 (0x0B-0x0C)" << endl;
        proplist << "Sectors per Cluster||" << QString::number(fatsb->csize) << "||Cluster size in sectors. Allowed values are powers of 2, but the cluster size must be 32KB or smaller (0x0D-0x0D)" << endl;
        proplist << "Reserved Area Size||" << QString::number(tsk_getu16(curfsinfo->endian, fatsb->reserved)) << "||Number of reserved sectors for boot sectors (0x0E-0x0F)" << endl;
        proplist << "Number of FATs||" << QString::number(fatsb->numfat) << "||Number of File Allocation Tables (FATs). Typically two for redundancy (0x10-0x10)" << endl;
        proplist << "Number of Root Directory Entries||" << QString::number(tsk_getu16(curfsinfo->endian, fatsb->numroot)) << "||Maximum number of files in the root directory for FAT12 and FAT16. This is 0 for FAT32 and typically 512 for FAT16 (0x11-0x12)" << endl;
        proplist << "Number of Sectors in File System||" << QString::number(tsk_getu16(curfsinfo->endian, fatsb->sectors16)) << "||Maximum number of sectors in the file system. If the number of sectors is larger than can be represented in this 2-byte value, a 4-byte value exists later in the data structure and this should be 0 (0x13-0x14)" << endl;
        proplist << "Media Type||" << QString::fromUtf8(reinterpret_cast<char*>(fatsb->f2)) << "||Media type. Should be 0xF8 for fixed disks and 0xF0 for removable disks (0x13-0x13)" << endl;
        proplist << "Size of FAT||" << QString::number(tsk_getu16(curfsinfo->endian, fatsb->sectperfat16)) << "||16-bit size in sectors of each FAT for FAT12 and FAT16. For FAT32, this field is 0 (0x14-0x15)" << endl;
        proplist << "Reserved||" << "Reserved" << "||Reserved (0x16-0x1B)" << endl;
        proplist << "Number of Sectors Before Partition Start||" << QString::number(tsk_getu16(curfsinfo->endian, fatsb->prevsect)) << "||Number of sectors before the start of the file system partition (0x1C-0x1F)" << endl;
        proplist << "Number of Sectors in File System||" << QString::number(tsk_getu32(curfsinfo->endian, fatsb->sectors32)) << "||32-bit value of number of sectors in the file system. Either this value or the 16-bit value above must be 0 (0x20-0x23)" << endl;
        if(curfsinfo->ftype == TSK_FS_TYPE_FAT32)
        {
            proplist << "Size of FAT||" << QString::number(tsk_getu32(curfsinfo->endian, fatsb->a.f32.sectperfat32)) << "||32-bit size in sectors of one FAT (0x24-0x27)" << endl;
            proplist << "Defines FAT is Written||" << QString::number(tsk_getu16(curfsinfo->endian, fatsb->a.f32.ext_flag)) << "||Defines how multiple FAT structures are written to. If bit 7 is 1, only one of the FAT structures is active and its index is described in bits 0-3. Otherwise all FAT structures are mirrors of each other (0x28-0x29)" << endl;
            proplist << "Major and Minor Version||" << QString::number(tsk_getu16(curfsinfo->endian, fatsb->a.f32.fs_ver)) << "||The major and minor version number (0x2A-0x2B)" << endl;
            proplist << "Root Directory Cluster||" << QString::number(tsk_getu32(curfsinfo->endian, fatsb->a.f32.rootclust)) << "||Cluster where the root directory can be found (0x2C-0x2F)" << endl;
            proplist << "FSINFO Structure Sector||" << QString::number(tsk_getu16(curfsinfo->endian, fatsb->a.f32.fsinfo)) << "||Sector where FSINFO structure can be found (0x30-0x31)" << endl;
            proplist << "Boot Sector Backup Copy||" << QString::number(tsk_getu16(curfsinfo->endian, fatsb->a.f32.bs_backup)) << "||Sector where the backup copy of the boot sector is located, default is 6 (0x32-0x33)" << endl;
            proplist << "Reserved||" << "Reserved" << "||Reserved (0x34-0x3F)" << endl;
            proplist << "BIOS Drive Number||" << QString::number(fatsb->a.f32.drvnum) << "||BIOS INT32h drive number (0x40-0x40)" << endl;
            proplist << "Not used||" << "Not used" << "||Not used (0x41-0x42)" << endl;
            proplist << "Volume Serial Number||" << QString::number(tsk_getu32(curfsinfo->endian, fatsb->a.f32.vol_id)) << "||Volume serial number, which some versions of Windows will calculate based on the creation date and time (0x43-0x46)" << endl;
            proplist << "Volume Label||" << QString::fromUtf8(reinterpret_cast<char*>(fatsb->a.f32.vol_lab)) << "||Volume label in ASCII. The user chooses this value when creating the file system (0x47-0x51)" << endl;
            proplist << "File System Type||" << QString::fromUtf8(reinterpret_cast<char*>(fatsb->a.f32.fs_type)) << "||File system type label in ASCII. Standard values include \"FAT32\", but nothing is required (0x52-0x59)" << endl;
            proplist << "Not Used||" << "Not Used" << "||Not Used (0x005A-0x01FD)" << endl;
        }
        else
        {
            proplist << "Reserved||" << "Reserved" << "||Reserved (0x24-0x26)" << endl;
            proplist << "Volume Serial Number||" << QString::number(tsk_getu32(curfsinfo->endian, fatsb->a.f16.vol_id)) << "||Volume serial number, which some versions of Windows will calculate based on the creation date and time (0x27-0x2A)" << endl;
            proplist << "Volume Label||" << QString::fromUtf8(reinterpret_cast<char*>(fatsb->a.f16.vol_lab)) << "||Volume label in ASCII. The user chooses this value when creating the file system (0x2B-0x35)" << endl;
            proplist << "File System Type||" << QString::fromUtf8(reinterpret_cast<char*>(fatsb->a.f16.fs_type)) << "||File system type in ASCII. Standard values include \"FAT\", \"FAT12\", \"FAT16\", but nothing is required (0x36->0x3D)" << endl;
            proplist << "Reserved||" << "Reserved" << "||Reserved (0x3E-0x01FD)" << endl;
        }
        proplist << "Signature||" << QString::number(tsk_getu16(curfsinfo->endian, fatsb->magic)) << "||Signature value should be 0xAA55 (0x01FE-0x01FF)" << endl;
    }
    else if(curfsinfo->ftype == TSK_FS_TYPE_NTFS)
    {
        ntfsinfo = (NTFS_INFO*)curfsinfo;
        proplist << "Assembly Boot Code||" << "Assembly Boot Code" << "||Assembly instruction to jump to boot code (0x00-0x02)" << endl;
        proplist << "OEM Name||" << QString::fromUtf8(ntfsinfo->fs->oemname) << "||OEM Name (0x03-0x0A)" << endl;
        proplist << "Bytes per Sector||" << QString::number(tsk_getu16(curfsinfo->endian, ntfsinfo->fs->ssize)) << "||Bytes per sector (0x0B-0x0C)" << endl;
        proplist << "Sectors per Cluster||" << QString::number(ntfsinfo->fs->csize) << "||Sectors per cluster (0x0D-0x0D)" << endl;
        proplist << "Reserved Sectors||" << "Reserved Sectors" << "||Reserved and Unused Sectors (0x0E-0x27)" << endl;
        proplist << "Volume Size (sectors)||" << QString::number(tsk_getu64(curfsinfo->endian, ntfsinfo->fs->vol_size_s)) << "||Total Sectors in the file system (0x28-0x2F)" << endl;
        proplist << "MFT Starting Cluster Address||" << QString::number(tsk_getu64(curfsinfo->endian, ntfsinfo->fs->mft_clust)) << "||Starting cluster address of the Master File Table (MFT) (0x30-0x37)" << endl;
        proplist << "MFT Starting Byte Address||" << QString::number( tsk_getu64(curfsinfo->endian, ntfsinfo->fs->mft_clust) * ntfsinfo->fs->csize * tsk_getu16(curfsinfo->endian, ntfsinfo->fs->ssize) ) << "||Starting byte address of the Master File Table (MFT)" << endl;
        proplist << "MFT Mirror Starting Cluster Address||" << QString::number(tsk_getu64(curfsinfo->endian, ntfsinfo->fs->mftm_clust)) << "||Starting cluster address of the MFT Mirror (0x38-0x3F)" << endl;
        int recordsize = 0;
        if(ntfsinfo->fs->mft_rsize_c > 0)
        {
            recordsize = ntfsinfo->fs->mft_rsize_c * ntfsinfo->fs->csize * tsk_getu16(curfsinfo->endian, ntfsinfo->fs->ssize);
        }
        else
            recordsize = 1 << -ntfsinfo->fs->mft_rsize_c;
        proplist << "MFT Record Size (bytes)||" << QString::number(recordsize) << "||Size of file record in bytes (MFT Entry) (0x40-0x40)" << endl;
        proplist << "Unused||" << "Unused" << "||Unused (0x41-0x43)" << endl;
        proplist << "Size of Index Record||" << QString::number(ntfsinfo->fs->idx_rsize_c) << "||Number of clusters per index record (0x44-0x44)" << endl;
        proplist << "Unused||" << "Unused" << "||Unused (0x45-0x47)" << endl;
        proplist << "Serial Number||" << QString::number(tsk_getu64(curfsinfo->endian, ntfsinfo->fs->serial)) << "||Serial Number (0x48-0x4F)" << endl;
        proplist << "Boot Code||" << "Boot Code" << "||Boot Code (0x0050-0x00FD)" << endl;
        proplist << "Signature||" << QString::number(tsk_getu16(curfsinfo->endian, ntfsinfo->fs->magic)) << "||Signature value should be 0xAA55 (0x00FE-0x00FF)" << endl;
        TSK_FS_FILE* tmpfile = NULL;
        const TSK_FS_ATTR* tmpattr;
        if((tmpfile = tsk_fs_file_open_meta(curfsinfo, NULL, NTFS_MFT_VOL)) == NULL)
        {
            // log error here
        }
        tmpattr = tsk_fs_attrlist_get(tmpfile->meta->attr, TSK_FS_ATTR_TYPE_NTFS_VNAME);
        if((tmpattr->flags & TSK_FS_ATTR_RES) && (tmpattr->size))
        {
            UTF16* name16 = (UTF16*) tmpattr->rd.buf;
            UTF8* name8 = (UTF8*) asc;
            int retval;
            retval = tsk_UTF16toUTF8(curfsinfo->endian, (const UTF16**)&name16, (UTF16*) ((uintptr_t)name16 + (int) tmpattr->size), &name8, (UTF8*) ((uintptr_t)name8 + sizeof(asc)), TSKlenientConversion);
            if(retval != TSKconversionOK)
            {
                // log error here
                *name8 = '\0';
            }
            else if((uintptr_t)name8 >= (uintptr_t)asc + sizeof(asc))
                asc[sizeof(asc)-1] = '\0';
            else
                *name8 = '\0';
            proplist << "Volume Name||" << QString::fromStdString(std::string(asc)) << "||Volume Name from $VOLUME_NAME attribute" << endl;
            proplist << "Version||";
            if(ntfsinfo->ver == NTFS_VINFO_NT)
                proplist << "Windows NT";
            else if(ntfsinfo->ver == NTFS_VINFO_2K)
                proplist << "Windows 2000";
            else if(ntfsinfo->ver == NTFS_VINFO_XP)
                proplist << "Windows XP";
            else
                proplist << "Newer than Windows XP";
            proplist << "||Version Information" << endl;
        }
    }
    else if(curfsinfo->ftype == TSK_FS_TYPE_EXFAT)
    {
        fatfs = (FATFS_INFO*)curfsinfo;
        exfatsb = (EXFATFS_MASTER_BOOT_REC*)&(fatfs->boot_sector_buffer);
        proplist << "Jump Boot||" << "Jump Boot" << "||Jump boot should be 0xEB7690 (0x00-0x02)" << endl;
        proplist << "File System Name||" << QString::fromUtf8(reinterpret_cast<char*>(exfatsb->fs_name)) << "||File system name should be \"EXFAT \" (0x03-0x0A)" << endl;
        proplist << "Unused||" << "Unused" << "||Unused, must be zeros (0x0B-0x3F)" << endl;
        proplist << "Partition Offset||" << QString::number(tsk_getu64(curfsinfo->endian, exfatsb->partition_offset)) << "||Sector address for partition start (0x40-0x47)" << endl;
        proplist << "Volume Length (sectors)||" << QString::number(tsk_getu64(curfsinfo->endian, exfatsb->vol_len_in_sectors)) << "||Size of exFAT volume in sectors (0x48-0x4F)" << endl;
        proplist << "FAT Offset||" << QString::number(tsk_getu32(curfsinfo->endian, exfatsb->fat_offset)) << "||FAT offset in sectors (0x50-0x53)" << endl;
        proplist << "FAT Length||" << QString::number(tsk_getu32(curfsinfo->endian, exfatsb->fat_len_in_sectors)) << "||FAT length in sectors. May exceed the required space in order to align the second FAT (0x54-0x57)" << endl;
        proplist << "Cluster Heap Offset||" << QString::number(tsk_getu32(curfsinfo->endian, exfatsb->cluster_heap_offset)) << "||Sector address of the data region (0x58-0x5B)" << endl;
        proplist << "Cluster Count||" << QString::number(tsk_getu32(curfsinfo->endian, exfatsb->cluster_cnt)) << "||Number of clusters in the cluster heap (0x5C-0x5F)" << endl;
        proplist << "Root Directory Cluster||" << QString::number(tsk_getu32(curfsinfo->endian, exfatsb->root_dir_cluster)) << "||Cluster address of the root directory (0x60-0x63)" << endl;
        proplist << "Volume Serial Number||" << QString::fromUtf8(reinterpret_cast<char*>(exfatsb->vol_serial_no)) << "||Volume serial number (0x64-0x67)" << endl;
        proplist << "File System Revision||" << QString::number(exfatsb->fs_revision[1]) + "." + QString::number(exfatsb->fs_revision[0]) << "||File system revision as Major.Minor (0x68-0x69)" << endl;
        proplist << "Volume Flags||" << QString::number(tsk_getu16(curfsinfo->endian, exfatsb->vol_flags)) << "||Volume Flags (0x6A-0x6B)" << endl;
        proplist << "Bytes Per Sector||" << QString::number(exfatsb->bytes_per_sector) << "||Bytes per sector as a power of 2. Minimum 9 (512 bytes per sector), maximum 12 (4096 bytes per sector) (0x6C-0x6C)" << endl;
        proplist << "Sectors Per Cluster||" << QString::number(exfatsb->sectors_per_cluster) << "||Sectors per cluster as a power of 2. Minimum 0 (1 sector per cluster) up to a maximum 25 (0x6D-0x6D)" << endl;
        proplist << "Number of FATs||" << QString::QString::number(exfatsb->num_fats) << "||Number of FATs: 1 or 2, 2 is for TexFAT only) (0x6E-0x6E)" << endl;
        proplist << "Drive Select||" << QString::number(exfatsb->drive_select) << "||Used by INT 13h, typically 0x80 (0x6F-0x6F)" << endl;
        proplist << "Percent of the Heap in Use||" << QString::number(exfatsb->percent_of_cluster_heap_in_use) << "||0-100, percentage of allocated clusters rounded down to the integer; 0xFF, percentage is not available (0x70-0x70)" << endl;
        proplist << "Reserved||" << "Reserved" << "||Reserved (0x71-0x77)" << endl;
        proplist << "Boot Code||" << "Boot Code" << "||Boot Code (0x78-0x01FD)" << endl;
        proplist << "Signature||" << QString::number(tsk_getu16(curfsinfo->endian, exfatsb->signature)) << "||Signature value should be 0xAA55 (0x01FE-0x01FF)" << endl;
    }
    else if(curfsinfo->ftype == TSK_FS_TYPE_ISO9660) // Byte offset's aren't working out too well right now.
    {
        iso = (ISO_INFO*)curfsinfo;
        for(p = iso->pvd; p!= NULL; p = p->next)
        {
            proplist << "Unused||" << "Unused" << "||Unused (0x00-0x07)" << endl;
            proplist << "System Identifier||" << QString::fromUtf8(reinterpret_cast<char*>(p->pvd.sys_id), 32) << "||System Identifier (0x08-0x27)" << endl;
            proplist << "Volume Identifier||" << QString::fromUtf8(reinterpret_cast<char*>(p->pvd.vol_id)) << "||Volume Identifier (0x28-0x47)" << endl;
            proplist << "Unused||" << "Unused" << "||Unused should be all zeros (0x48-0x4F)" << endl;
            proplist << "Volume Space Size (LE)||" << QString::number(tsk_getu32(curfsinfo->endian, p->pvd.vs_sz_l)) << "||Volume Space Size in blocks (0x50-0x53)" << endl;
            proplist << "Volume Space Size (BE)||" << QString::number(tsk_getu32(curfsinfo->endian, p->pvd.vs_sz_m)) << "||Volume Space Size in blocks (0x54-0x57)" << endl;
            proplist << "Unused||" << "Unused" << "||Unused. All zeros (0x58-0x77)" << endl;
            proplist << "Volume Set Size (LE)||" << QString::number(tsk_getu16(curfsinfo->endian, p->pvd.vol_set_l)) << "||The size of the set in this logical volume (0x78-0x79)" << endl;
            proplist << "Volume Set Size (BE)||" << QString::number(tsk_getu16(curfsinfo->endian, p->pvd.vol_set_m)) << "||The size of the set in this logical volume (0x7A-0x7B)" << endl;
            proplist << "Volume Sequence Number (LE)||" << QString::number(tsk_getu16(curfsinfo->endian, p->pvd.vol_seq_l)) << "||The number of this disk in the volume set (0x7C-0x7D)" << endl;
            proplist << "Volume Sequence Number (BE)||" << QString::number(tsk_getu16(curfsinfo->endian, p->pvd.vol_seq_m)) << "||The number of this disk in the volume set (0x7E-0x7F)" << endl;
            proplist << "Logical Block Size (LE)||" << QString::number(tsk_getu16(curfsinfo->endian, p->pvd.blk_sz_l)) << "||The size in bytes of a logical block (0x80-0x81)" << endl;
            proplist << "Logical Block Size (BE)||" << QString::number(tsk_getu16(curfsinfo->endian, p->pvd.blk_sz_m)) << "||The size in bytes of a logical block (0x82-0x83)" << endl;
            proplist << "Path Table Size (LE)||" << QString::number(tsk_getu32(curfsinfo->endian, p->pvd.pt_size_l)) << "||The size in bytes of the path table (0x84-0x87)" << endl;
            proplist << "Path Table Size (BE)||" << QString::number(tsk_getu32(curfsinfo->endian, p->pvd.pt_size_m)) << "||The size in bytes of the path table (0x88-0x8B)" << endl;
            proplist << "Logical Block Address of Type-L Path Table||" << QString::number(tsk_getu32(curfsinfo->endian, p->pvd.pt_loc_l)) << "||LBA location of the path table (0x8C-0x8F)" << endl;
            proplist << "Logical Block Address of Optional Type-L Path Table||" << QString::number(tsk_getu32(curfsinfo->endian, p->pvd.pt_opt_loc_l)) << "||LBA location of the optional path table (0x90-0x93)" << endl;
            proplist << "Logical Block Address of the Type-M Path Table||" << QString::number(tsk_getu32(curfsinfo->endian, p->pvd.pt_loc_m)) << "||LBA location of the path table (0x94-0x97)" << endl;
            proplist << "Logical Block Address of the Optional Type-M Path Table||" << QString::number(tsk_getu32(curfsinfo->endian, p->pvd.pt_opt_loc_m)) << "||LBA location of the optional path table (0x98-0x9B)" << endl;
            proplist << "Directory Entry for Root Directory||" << QString::fromUtf8(reinterpret_cast<char*>(p->pvd.dir_rec.name)) << "||Directory Entry for Root Directory (0xB6-0xBD)" << endl;
            snprintf(asc128, 128, "%s", p->pvd.vol_setid);
            proplist << "Volume Set Identifier||" << QString::fromStdString(std::string(asc128)) << "||Identifier of the volume set of which this volume is a member (0x00BE-0x013D)" << endl;
            snprintf(asc128, 128, "%s", p->pvd.pub_id);
            proplist << "Publisher Identifier||" << QString::fromStdString(std::string(asc128)) << "||Volume publisher (0x013E-0x01BD)" << endl;
            snprintf(asc128, 128, "%s", p->pvd.prep_id);
            proplist << "Data Preparer Identifier||" << QString::fromStdString(std::string(asc128)) << "||Identifier of the person(s) who prepared the data for this volume (0x01BE-0x023D)" << endl;
            snprintf(asc128, 128, "%s", p->pvd.app_id);
            proplist << "Application Identifier||" << QString::fromStdString(std::string(asc128)) << "||Identifies how the data are recorded on this volume (0x023E-0x02BD)" << endl;
            snprintf(asc128, 38, "%s", p->pvd.copy_id);
            proplist << "Copyright File Identifier||" << QString::fromStdString(std::string(asc128)) << "||Fielname of a file in the root directory that contains copyright information for the volume set (0x02BE-0x02E3)" << endl;
            snprintf(asc128, 37, "%s", p->pvd.abs_id);
            proplist << "Abstract File Identifier||" << QString::fromStdString(std::string(asc128)) << "||Filename of a file in the root directory that contains abstract information for the volume set (0x02E4-0x0307)" << endl;
            snprintf(asc128, 37, "%s", p->pvd.bib_id);
            proplist << "Bibliographic File Identifier||" << QString::fromStdString(std::string(asc128)) << "||Filename of a file in the root directory that contains bibliographic information for this volume set (0x0308-0x032C)" << endl;
            sprintf(asc, "%c%c/%c%c/%c%c%c%c %c%c:%c%c:%c%c", ((char)(p->pvd.make_date.month[0])), (char)p->pvd.make_date.month[1], (char)p->pvd.make_date.day[0], (char)p->pvd.make_date.day[1], (char)p->pvd.make_date.year[0], (char)p->pvd.make_date.year[1], (char)p->pvd.make_date.year[2], (char)p->pvd.make_date.year[3], (char)p->pvd.make_date.hour[0], (char)p->pvd.make_date.hour[1], (char)p->pvd.make_date.min[0], (char)p->pvd.make_date.min[1], (char)p->pvd.make_date.sec[0], (char)p->pvd.make_date.sec[1]);
            proplist << "Volume Creation Date/Time||" << QString::fromStdString(std::string(asc)) + ConvertGmtHours(p->pvd.make_date.gmt_off) << "||The date and time when the volume was created (0x032D-0x033D)" << endl;
            sprintf(asc, "%c%c/%c%c/%c%c%c%c %c%c:%c%c:%c%c", (char)p->pvd.mod_date.month[0], (char)p->pvd.mod_date.month[1], (char)p->pvd.mod_date.day[0], (char)p->pvd.mod_date.day[1], (char)p->pvd.mod_date.year[0], (char)p->pvd.mod_date.year[1], (char)p->pvd.mod_date.year[2], (char)p->pvd.mod_date.year[3], (char)p->pvd.mod_date.hour[0], (char)p->pvd.mod_date.hour[1], (char)p->pvd.mod_date.min[0], (char)p->pvd.mod_date.min[1], (char)p->pvd.mod_date.sec[0], (char)p->pvd.mod_date.sec[1]);
            proplist << "Volume Modification Date/Time||" << QString::fromStdString(std::string(asc)) + ConvertGmtHours(p->pvd.mod_date.gmt_off) << "||The date and time when the volume was modified (0x033E-0x034E)" << endl;
            sprintf(asc, "%c%c/%c%c/%c%c%c%c %c%c:%c%c:%c%c", (char)p->pvd.exp_date.month[0], (char)p->pvd.exp_date.month[1], (char)p->pvd.exp_date.day[0], (char)p->pvd.exp_date.day[1], (char)p->pvd.exp_date.year[0], (char)p->pvd.exp_date.year[1], (char)p->pvd.exp_date.year[2], (char)p->pvd.exp_date.year[3], (char)p->pvd.exp_date.hour[0], (char)p->pvd.exp_date.hour[1], (char)p->pvd.exp_date.min[0], (char)p->pvd.exp_date.min[1], (char)p->pvd.exp_date.sec[0], (char)p->pvd.exp_date.sec[1]);
            proplist << "Volume Expiration Date/Time||" << QString::fromStdString(std::string(asc)) + ConvertGmtHours(p->pvd.exp_date.gmt_off) << "||The date and time after which the volume is considered to be obsolete. If not specified, then the volume is never considered to be obsolete (0x034F-0x035F)" << endl;
            sprintf(asc, "%c%c/%c%c/%c%c%c%c %c%c:%c%c:%c%c", (char)p->pvd.ef_date.month[0], (char)p->pvd.ef_date.month[1], (char)p->pvd.ef_date.day[0], (char)p->pvd.ef_date.day[1], (char)p->pvd.ef_date.year[0], (char)p->pvd.ef_date.year[1], (char)p->pvd.ef_date.year[2], (char)p->pvd.ef_date.year[3], (char)p->pvd.ef_date.hour[0], (char)p->pvd.ef_date.hour[1], (char)p->pvd.ef_date.min[0], (char)p->pvd.ef_date.min[1], (char)p->pvd.ef_date.sec[0], (char)p->pvd.ef_date.sec[1]);
            proplist << "Volume Effective Date/Time||" << QString::fromStdString(std::string(asc)) + ConvertGmtHours(p->pvd.ef_date.gmt_off) << "||The date and time after which the volume may be used. If not specified, then the volume may be used immediately (0x0360-0x0370)" << endl;
            proplist << "File Structure Version||" << QString::number(p->pvd.fs_ver) << "||The directory records and path table version, always 0x01 (0x0371-0x0371)" << endl;
            proplist << "Unused||" << "Unused" << "||Unused, always 0x00 (0x0372-0x0372)" << endl;
            snprintf(asc, 512, "%s", p->pvd.app_use);
            proplist << "Application Used||" << QString::fromStdString(std::string(asc)) << "||Application Used. Contents not defined by ISO9660 (0x0373-0x0572)" << endl;
            proplist << "Reserved by ISO||" << "Reserved" << "||Reserved by ISO (0x0573-0x07FF)" << endl;
        }
        for(s = iso->svd; s!= NULL; s = s->next)
        {
            proplist << "Volume Flags||" << QString::number(tsk_getu32(curfsinfo->endian, s->svd.flags)) << "||Unused (0x00-0x07)" << endl;
            proplist << "System Identifier||" << QString::fromUtf8(reinterpret_cast<char*>(s->svd.sys_id), 32) << "||System Identifier (0x08-0x27)" << endl;
            proplist << "Volume Identifier||" << QString::fromUtf8(reinterpret_cast<char*>(s->svd.vol_id)) << "||Volume Identifier (0x28-0x47)" << endl;
            proplist << "Unused||" << "Unused" << "||Unused should be all zeros (0x48-0x4F)" << endl;
            proplist << "Volume Space Size (LE)||" << QString::number(tsk_getu32(curfsinfo->endian, s->svd.vs_sz_l)) << "||Volume Space Size in blocks (0x50-0x53)" << endl;
            proplist << "Volume Space Size (BE)||" << QString::number(tsk_getu32(curfsinfo->endian, s->svd.vs_sz_m)) << "||Volume Space Size in blocks (0x54-0x57)" << endl;
            proplist << "Unused||" << "Unused" << "||Unused. All zeros (0x58-0x77)" << endl;
            proplist << "Volume Set Size (LE)||" << QString::number(tsk_getu16(curfsinfo->endian, s->svd.vol_set_l)) << "||The size of the set in this logical volume (0x78-0x79)" << endl;
            proplist << "Volume Set Size (BE)||" << QString::number(tsk_getu16(curfsinfo->endian, s->svd.vol_set_m)) << "||The size of the set in this logical volume (0x7A-0x7B)" << endl;
            proplist << "Volume Sequence Number (LE)||" << QString::number(tsk_getu16(curfsinfo->endian, s->svd.vol_seq_l)) << "||The number of this disk in the volume set (0x7C-0x7D)" << endl;
            proplist << "Volume Sequence Number (BE)||" << QString::number(tsk_getu16(curfsinfo->endian, s->svd.vol_seq_m)) << "||The number of this disk in the volume set (0x7E-0x7F)" << endl;
            proplist << "Logical Block Size (LE)||" << QString::number(tsk_getu16(curfsinfo->endian, s->svd.blk_sz_l)) << "||The size in bytes of a logical block (0x80-0x81)" << endl;
            proplist << "Logical Block Size (BE)||" << QString::number(tsk_getu16(curfsinfo->endian, s->svd.blk_sz_m)) << "||The size in bytes of a logical block (0x82-0x83)" << endl;
            proplist << "Path Table Size (LE)||" << QString::number(tsk_getu32(curfsinfo->endian, s->svd.pt_size_l)) << "||The size in bytes of the path table (0x84-0x87)" << endl;
            proplist << "Path Table Size (BE)||" << QString::number(tsk_getu32(curfsinfo->endian, s->svd.pt_size_m)) << "||The size in bytes of the path table (0x88-0x8B)" << endl;
            proplist << "Logical Block Address of Type-L Path Table||" << QString::number(tsk_getu32(curfsinfo->endian, s->svd.pt_loc_l)) << "||LBA location of the path table (0x8C-0x8F)" << endl;
            proplist << "Logical Block Address of Optional Type-L Path Table||" << QString::number(tsk_getu32(curfsinfo->endian, s->svd.pt_opt_loc_l)) << "||LBA location of the optional path table (0x90-0x93)" << endl;
            proplist << "Logical Block Address of the Type-M Path Table||" << QString::number(tsk_getu32(curfsinfo->endian, s->svd.pt_loc_m)) << "||LBA location of the path table (0x94-0x97)" << endl;
            proplist << "Logical Block Address of the Optional Type-M Path Table||" << QString::number(tsk_getu32(curfsinfo->endian, s->svd.pt_opt_loc_m)) << "||LBA location of the optional path table (0x98-0x9B)" << endl;
            proplist << "Directory Entry for Root Directory||" << QString::fromUtf8(reinterpret_cast<char*>(s->svd.dir_rec.name)) << "||Directory Entry for Root Directory (0xB6-0xBD)" << endl;
            snprintf(asc128, 128, "%s", s->svd.vol_setid);
            proplist << "Volume Set Identifier||" << QString::fromStdString(std::string(asc128)) << "||Identifier of the volume set of which this volume is a member (0x00BE-0x013D)" << endl;
            snprintf(asc128, 128, "%s", s->svd.pub_id);
            proplist << "Publisher Identifier||" << QString::fromStdString(std::string(asc128)) << "||Volume publisher (0x013E-0x01BD)" << endl;
            snprintf(asc128, 128, "%s", s->svd.prep_id);
            proplist << "Data Preparer Identifier||" << QString::fromStdString(std::string(asc128)) << "||Identifier of the person(s) who prepared the data for this volume (0x01BE-0x023D)" << endl;
            snprintf(asc128, 128, "%s", s->svd.app_id);
            proplist << "Application Identifier||" << QString::fromStdString(std::string(asc128)) << "||Identifies how the data are recorded on this volume (0x023E-0x02BD)" << endl;
            snprintf(asc128, 38, "%s", s->svd.copy_id);
            proplist << "Copyright File Identifier||" << QString::fromStdString(std::string(asc128)) << "||Fielname of a file in the root directory that contains copyright information for the volume set (0x02BE-0x02E3)" << endl;
            snprintf(asc128, 37, "%s", s->svd.abs_id);
            proplist << "Abstract File Identifier||" << QString::fromStdString(std::string(asc128)) << "||Filename of a file in the root directory that contains abstract information for the volume set (0x02E4-0x0307)" << endl;
            snprintf(asc128, 37, "%s", s->svd.bib_id);
            proplist << "Bibliographic File Identifier||" << QString::fromStdString(std::string(asc128)) << "||Filename of a file in the root directory that contains bibliographic information for this volume set (0x0308-0x032C)" << endl;
            sprintf(asc, "%c%c/%c%c/%c%c%c%c %c%c:%c%c:%c%c", ((char)(s->svd.make_date.month[0])), (char)s->svd.make_date.month[1], (char)s->svd.make_date.day[0], (char)s->svd.make_date.day[1], (char)s->svd.make_date.year[0], (char)s->svd.make_date.year[1], (char)s->svd.make_date.year[2], (char)s->svd.make_date.year[3], (char)s->svd.make_date.hour[0], (char)s->svd.make_date.hour[1], (char)s->svd.make_date.min[0], (char)s->svd.make_date.min[1], (char)s->svd.make_date.sec[0], (char)s->svd.make_date.sec[1]);
            proplist << "Volume Creation Date/Time||" << QString::fromStdString(std::string(asc)) + ConvertGmtHours(s->svd.make_date.gmt_off) << "||The date and time when the volume was created (0x032D-0x033D)" << endl;
            sprintf(asc, "%c%c/%c%c/%c%c%c%c %c%c:%c%c:%c%c", (char)s->svd.mod_date.month[0], (char)s->svd.mod_date.month[1], (char)s->svd.mod_date.day[0], (char)s->svd.mod_date.day[1], (char)s->svd.mod_date.year[0], (char)s->svd.mod_date.year[1], (char)s->svd.mod_date.year[2], (char)s->svd.mod_date.year[3], (char)s->svd.mod_date.hour[0], (char)s->svd.mod_date.hour[1], (char)s->svd.mod_date.min[0], (char)s->svd.mod_date.min[1], (char)s->svd.mod_date.sec[0], (char)s->svd.mod_date.sec[1]);
            proplist << "Volume Modification Date/Time||" << QString::fromStdString(std::string(asc)) + ConvertGmtHours(s->svd.mod_date.gmt_off) << "||The date and time when the volume was modified (0x033E-0x034E)" << endl;
            sprintf(asc, "%c%c/%c%c/%c%c%c%c %c%c:%c%c:%c%c", (char)s->svd.exp_date.month[0], (char)s->svd.exp_date.month[1], (char)s->svd.exp_date.day[0], (char)s->svd.exp_date.day[1], (char)s->svd.exp_date.year[0], (char)s->svd.exp_date.year[1], (char)s->svd.exp_date.year[2], (char)s->svd.exp_date.year[3], (char)s->svd.exp_date.hour[0], (char)s->svd.exp_date.hour[1], (char)s->svd.exp_date.min[0], (char)s->svd.exp_date.min[1], (char)s->svd.exp_date.sec[0], (char)s->svd.exp_date.sec[1]);
            proplist << "Volume Expiration Date/Time||" << QString::fromStdString(std::string(asc)) + ConvertGmtHours(s->svd.exp_date.gmt_off) << "||The date and time after which the volume is considered to be obsolete. If not specified, then the volume is never considered to be obsolete (0x034F-0x035F)" << endl;
            sprintf(asc, "%c%c/%c%c/%c%c%c%c %c%c:%c%c:%c%c", (char)s->svd.ef_date.month[0], (char)s->svd.ef_date.month[1], (char)s->svd.ef_date.day[0], (char)s->svd.ef_date.day[1], (char)s->svd.ef_date.year[0], (char)s->svd.ef_date.year[1], (char)s->svd.ef_date.year[2], (char)s->svd.ef_date.year[3], (char)s->svd.ef_date.hour[0], (char)s->svd.ef_date.hour[1], (char)s->svd.ef_date.min[0], (char)s->svd.ef_date.min[1], (char)s->svd.ef_date.sec[0], (char)s->svd.ef_date.sec[1]);
            proplist << "Volume Effective Date/Time||" << QString::fromStdString(std::string(asc)) + ConvertGmtHours(s->svd.ef_date.gmt_off) << "||The date and time after which the volume may be used. If not specified, then the volume may be used immediately (0x0360-0x0370)" << endl;
            proplist << "File Structure Version||" << QString::fromUtf8(reinterpret_cast<char*>(s->svd.fs_ver)) << "||The directory records and path table version, always 0x01 (0x0371-0x0371)" << endl;
            proplist << "Unused||" << "Unused" << "||Unused, always 0x00 (0x0372-0x0372)" << endl;
            snprintf(asc, 512, "%s", s->svd.app_use);
            proplist << "Application Used||" << QString::fromStdString(std::string(asc)) << "||Application Used. Contents not defined by ISO9660 (0x0373-0x0572)" << endl;
            proplist << "Reserved by ISO||" << "Reserved" << "||Reserved by ISO (0x0573-0x07FF)" << endl;
        }
    }
    else if(curfsinfo->ftype == TSK_FS_TYPE_YAFFS2) // YAFFS2 file system info
    {
        yfs = (YAFFSFS_INFO*)curfsinfo;
        unsigned int objcnt;
        uint32_t objfirst, objlast, vercnt, verfirst, verlast;
        proplist << "Page Size||" << QString::number(yfs->page_size) << "||Page Size" << endl;
        proplist << "Spare Size||" << QString::number(yfs->spare_size) << "||Spare Size" << endl;
        proplist << "Spare Offset Sequence Number||" << QString::number(yfs->spare_seq_offset) << "||Spare Offset Sequence Number" << endl;
        proplist << "Spare Offset Object ID||" << QString::number(yfs->spare_obj_id_offset) << "||Spare offset object id" << endl;
        proplist << "Spare Offset Chunk ID||" << QString::number(yfs->spare_chunk_id_offset) << "||Spare offset chunk id" << endl;
        proplist << "Spare Offset Number of Bytes||" << QString::number(yfs->spare_nbytes_offset) << "||Spare offset number of bytes" << endl;
        yaffscache_objects_stats(yfs, &objcnt, &objfirst, &objlast, &vercnt, &verfirst, &verlast);
        proplist << "Number of Allocated Objects||" << QString::number(objcnt) << "||Number of Allocated Objects" << endl;
        proplist << "Object ID Range||" << QString::number(objfirst) + " - " + QString::number(objlast) << "||Object id range" << endl;
        proplist << "Number of Total Object Versions||" << QString::number(vercnt) << "||Number of total object versions" << endl;
        proplist << "Object Version Range||" << QString::number(verfirst) + " - " + QString::number(verlast) << "||Object version range" << endl;
    }
    else if(curfsinfo->ftype == TSK_FS_TYPE_HFS) // hfs file system info (last of the tsk defined ones)
    {
        hfs = (HFS_INFO*)curfsinfo;
        hsb = hfs->fs;
        time_t mactime;
        sprintf(asc, "0x%X%X %c%c", hsb->signature[0], hsb->signature[1], hsb->signature[0], hsb->signature[1]);
        proplist << "Signature||" << QString::fromStdString(std::string(asc)) << "||Signature value 0x4244 for \"BD\", 0x482B for \"H+\" and 0x4858 for \"HX\" (0x00-0x01)" << endl;
        proplist << "Version||" << QString::number(tsk_getu16(curfsinfo->endian, hsb->version)) << "||Version number. 4 for HFS+, 5 for HFSX (0x02-0x03)" << endl;
        proplist << "Volume Attributes||";
        if(tsk_getu32(curfsinfo->endian, hsb->attr) & HFS_VH_ATTR_UNMOUNTED)
            proplist << "Volume was Unmounted Properly";
        else
            proplist << "Volume was Unmounted Improperly";
        proplist << "||Attribute values, bits 0-7,14,16-31 are reserved. Bit 8 determines if the volume was unmounted properly (0x04-0x07)" << endl;
        proplist << "Volume Attributes||";
        if(tsk_getu32(curfsinfo->endian, hsb->attr) & HFS_VH_ATTR_BADBLOCKS)
            proplist << "Volume has Bad Blocks";
        else
            proplist << "Volume does not have Bad Blocks";
        proplist << "||Attribute values, bits 0-7,14,16-31 are reserved. Bit 9 determines if there are bad blocks for the volume (0x04-0x07)" << endl;
        proplist << "Volume Attributes||";
        if(tsk_getu32(curfsinfo->endian, hsb->attr) & HFS_VH_ATTR_NOCACHE)
            proplist << "Volume is not Cached";
        else
            proplist << "Volume is Cached";
        proplist << "||Attribute values, bits 0-7,14,16-31 are reserved. Bit 10 determines if the volume should not be cached (0x04-0x07)" << endl;
        proplist << "Volume Attributes||";
        if(tsk_getu32(curfsinfo->endian, hsb->attr) & HFS_VH_ATTR_INCONSISTENT)
            proplist << "Volume is Inconsistent";
        else
            proplist << "Volume is Consistent";
        proplist << "||Attribute values, bits 0-7,14,16-31 are reserved. Bit 11 determines if the volume was unmounted properly (0x04-0x07)" << endl;
        proplist << "Volume Attributes||";
        if(tsk_getu32(curfsinfo->endian, hsb->attr) & HFS_VH_ATTR_CNIDS_REUSED)
            proplist << "CNIDs are Being Reused";
        else
            proplist << "CNIDs are not Being Reused";
        proplist << "||Attribute values, bits 0-7,14,16-31 are reserved. Bit 12 determines if the CNIDs have wrapped around past the maximum value and are being reused (0x04-0x07)" << endl;
        proplist << "Volume Attributes||";
        if(tsk_getu32(curfsinfo->endian, hsb->attr) & HFS_VH_ATTR_JOURNALED)
            proplist << "Journaling Enabled";
        else
            proplist << "Journaling Disabled";
        proplist << "||Attribute values, bits 0-7,14,16-31 are reserved. Bit 13 determines if journaling is enabled for the volume (0x04-0x07)" << endl;
        proplist << "Volume Attributes||";
        if(tsk_getu32(curfsinfo->endian, hsb->attr) & HFS_VH_ATTR_SOFTWARE_LOCK)
            proplist << "Volume should be Write-Protected in Software";
        else
            proplist << "Volume is not Write-Protected in Software";
        proplist << "||Attribute values, bits 0-7,14,16-31 are reserved. Bit 15 determines if the volume should be write-protected (0x04-0x07)" << endl;
        sprintf(asc, "0x%X%X%X%X %c%c%c%c", hsb->last_mnt_ver[0], hsb->last_mnt_ver[1], hsb->last_mnt_ver[2], hsb->last_mnt_ver[3], hsb->last_mnt_ver[0], hsb->last_mnt_ver[1], hsb->last_mnt_ver[2], hsb->last_mnt_ver[3]);
        proplist << "Last Mounted By||" << QString::fromStdString(std::string(asc)) << "||\"10.0\" for Mac OS X, \"HFSJ\" for journaled HFS+ on Mac OS X, \"FSK!\" for failed journal replay, \"fsck\" for fsck_hfs and \"8.10\" for Mac OS 8.1-9.2.2 (0x08-0x0B)" << endl;
        proplist << "Journal Info Block||" << QString::number(tsk_getu32(curfsinfo->endian, hsb->jinfo_blk)) << "||Journal information block (0x0C-0x0F)" << endl;
        mactime = hfs_convert_2_unix_time(tsk_getu32(curfsinfo->endian, hsb->cr_date));
        sprintf(asc, "%s", tsk_fs_time_to_str(mktime(gmtime(&mactime)), timebuf));
        proplist << "Volume Creation TimeStamp||" << QString::fromStdString(std::string(asc)) << "||MACTIME32 converted to UTC from Local Time (0x10-0x13)" << endl;
        sprintf(asc, "%s", tsk_fs_time_to_str(hfs_convert_2_unix_time(tsk_getu32(curfsinfo->endian, hsb->m_date)), timebuf));
        proplist << "Volume Last Modified TimeStamp||" << QString::fromStdString(std::string(asc)) << "||MACTIME32 converted to UTC (0x14-0x17)" << endl;
        sprintf(asc, "%s", tsk_fs_time_to_str(hfs_convert_2_unix_time(tsk_getu32(curfsinfo->endian, hsb->bkup_date)), timebuf));
        proplist << "Volume Backup TimeStamp||" << QString::fromStdString(std::string(asc)) << "||MACTIME32 converted to UTC (0x18-0x1B)" << endl;
        sprintf(asc, "%s", tsk_fs_time_to_str(hfs_convert_2_unix_time(tsk_getu32(curfsinfo->endian, hsb->chk_date)), timebuf));
        proplist << "Volume Checked TimeStamp||" << QString::fromStdString(std::string(asc)) << "||MACTIME32 converted to UTC (0x1C-0x1F)" << endl;
        proplist << "File Count||" << QString::number(tsk_getu32(curfsinfo->endian, hsb->file_cnt)) << "||Number of files on volume, not including special files (0x20-0x23)" << endl;
        proplist << "Folder Count||" << QString::number(tsk_getu32(curfsinfo->endian, hsb->fldr_cnt)) << "||Number of folders on volume, not including the root directory (0x24-0x27)" << endl;
        proplist << "Allocation Block Size||" << QString::number(tsk_getu32(curfsinfo->endian, hsb->blk_sz)) << "||Allocation block size in bytes (0x28-0x2B)" << endl;
        proplist << "Allocation Block Total||" << QString::number(tsk_getu32(curfsinfo->endian, hsb->blk_cnt)) << "||Number of blocks on disk (0x2C-0x2F)" << endl;
        proplist << "Allocation Block Free||" << QString::number(tsk_getu32(curfsinfo->endian, hsb->free_blks)) << "||Unused block count (0x30-0x33)" << endl;
        proplist << "Next Allocation||" << QString::number(tsk_getu32(curfsinfo->endian, hsb->next_alloc)) << "||Block address to start the allocation search (0x34-0x37)" << endl;
        proplist << "Resource Clump Size||" << QString::number(tsk_getu32(curfsinfo->endian, hsb->rsrc_clmp_sz)) << "||Default clump size for resource forks in bytes (0x38-0x3B)" << endl;
        proplist << "Data Clump Size||" << QString::number(tsk_getu32(curfsinfo->endian, hsb->data_clmp_sz)) << "||Default clump size for data forks in bytes (0x3C-0x3F)" << endl;
        proplist << "Next Catalog ID||" << QString::number(tsk_getu32(curfsinfo->endian, hsb->next_cat_id)) << "||Next catalog node id for allocation (0x40-0x43)" << endl;
        proplist << "Write Count||" << QString::number(tsk_getu32(curfsinfo->endian, hsb->write_cnt)) << "||Write count incremented each time it is mounted and modified (0x44-0x47)" << endl;
        proplist << "Encoding Bitmap||" << QString::number(tsk_getu64(curfsinfo->endian, hsb->enc_bmp)) << "||Encoding bitmap identifies which encodings were used in the file system (0x48-0x4F)" << endl;
        proplist << "Bootable Folder ID||" << QString::number(tsk_getu32(curfsinfo->endian, hsb->finder_info[HFS_VH_FI_BOOT])) << "||Bootable folder id (0x50-0x53)" << endl;
        proplist << "Startup App ID||" << QString::number(tsk_getu32(curfsinfo->endian, hsb->finder_info[HFS_VH_FI_START])) << "||Startup app id (0x54-0x57)" << endl;
        proplist << "Startup Open Folder ID||" << QString::number(tsk_getu32(curfsinfo->endian, hsb->finder_info[HFS_VH_FI_OPEN])) << "||Startup open folder id (0x58-0x5B)" << endl;
        proplist << "Mac OS 8/9 Blessed System Folder ID||" << QString::number(tsk_getu32(curfsinfo->endian, hsb->finder_info[HFS_VH_FI_BOOT9])) << "||Mac OS 8/9 blessed system folder id (0x5C-0x5F)" << endl;
        proplist << "Reserved||" << "Reserved" << "||Reserved (0x60-0x63)" << endl;
        proplist << "Mac OS X Blessed System Folder ID||" << QString::number(tsk_getu32(curfsinfo->endian, hsb->finder_info[HFS_VH_FI_BOOTX])) << "||Mac OS X blessed system folder id (0x64-0x67)" << endl;
        sprintf(asc, "%08" PRIx32 "%08" PRIx32, tsk_getu32(curfsinfo->endian, hsb->finder_info[HFS_VH_FI_ID1]), tsk_getu32(curfsinfo->endian, hsb->finder_info[HFS_VH_FI_ID2]));
        proplist << "Volume Identifier||" << QString::fromStdString(std::string(asc)) << "||Volume identifier (0x068-0x6F)" << endl;
        proplist << "Fork Data Allocation File||" << "" << "||Location and size of allocation bitmap files (0x70-0xBF)" << endl;
        proplist << "Fork Data Extents File||" << "" << "||Location and size of extents file (0x00C0-0x010F)" << endl;
        proplist << "Fork Data Catalog File||" << "" << "||Location and size of catalog file (0x0110-0x015F)" << endl;
        proplist << "Fork Data Attributes File||" << "" << "||Location and size of attributes file (0x0160-0x01AF)" << endl;
        proplist << "Fork Data Startup File||" << "" << "||Location and size of startup file (0x01B0-0x01FF)" << endl;
    }
    proplist << "Endian Ordering||";
    if(curfsinfo->endian == TSK_UNKNOWN_ENDIAN)
        proplist << "Endianness is unknown";
    else if(curfsinfo->endian == TSK_LIT_ENDIAN)
        proplist << "Data is in Little Endian";
    else if(curfsinfo->endian == TSK_BIG_ENDIAN)
        proplist << "Data is in Big Endian";
    else
        proplist << "Endianness is unknown";
    proplist << "||Identifies the endian ordering of the data being read." << endl;
    proplist.flush();
    fspropfile.close();
    //qDebug() << "fspropfile: close";
}

//void WriteVolumeProperties(TSK_VS_INFO* curvolinfo)
void WriteVolumeProperties(TSK_VS_INFO* curvolinfo, QString volumepath)
{
    char asc[512];
    QFile vpropfile(volumepath + "prop");
    vpropfile.open(QIODevice::WriteOnly | QIODevice::Text);
    //qDebug() << "vpropfile: open";
    QTextStream proplist(&vpropfile);
    mac_part* macpart = NULL;
    bsd_disklabel* bsdpart = NULL;
    sun_dlabel_sparc* sunsparcpart = NULL;
    sun_dlabel_i386* sunx86part = NULL;
    gpt_head* gptpart = NULL;

    if(curvolinfo->vstype == TSK_VS_TYPE_DOS)
        proplist << "Signature||" << "0xAA55" << "||Signature Value should be 0xAA55 (0x1FE-0x1FF)" << endl;
    else if(curvolinfo->vstype == TSK_VS_TYPE_BSD)
    {
        char* sect_buf;
        ssize_t cnt;
        TSK_ENDIAN_ENUM endian = curvolinfo->endian;
        sect_buf = (char*)tsk_malloc(curvolinfo->block_size);
        bsdpart = (bsd_disklabel*) sect_buf;
        cnt = tsk_vs_read_block(curvolinfo, BSD_PART_SOFFSET, sect_buf, curvolinfo->block_size);
        if(cnt != curvolinfo->block_size)
        {
            // print error here
        }
        free(sect_buf);
        proplist << "Signature||" << QString::number(tsk_getu32(endian, bsdpart->magic)) << "||Signature value should be 0x82564557 (0x00-0x03)" << endl;
        proplist << "Drive Type||" << QString::number(tsk_getu16(endian, bsdpart->type)) << "||Drive type (0x04-0x05)" << endl;
        proplist << "Drive Subtype||" << QString::number(tsk_getu16(endian, bsdpart->sub_type)) << "||Drive subtype (0x06-0x07)" << endl;
        proplist << "Drive Type Name||" << QString::fromUtf8(reinterpret_cast<char*>(bsdpart->type_name)) << "||The Drive type name (0x08-0x17)" << endl;
        proplist << "Pack Identifier Name||" << QString::fromUtf8(reinterpret_cast<char*>(bsdpart->packname)) << "||The Pack identifier name (0x18-0x27)" << endl;
        proplist << "Sector Size (bytes)||" << QString::number(tsk_getu32(endian, bsdpart->sec_size)) << "||Size of a sector in bytes (0x28-0x2B)" << endl;
        proplist << "Sectors per Track||" << QString::number(tsk_getu32(endian, bsdpart->sec_per_tr)) << "||Number of Sectors per track (0x2C-0x2F)" << endl;
        proplist << "Tracks per Cylinder||" << QString::number(tsk_getu32(endian, bsdpart->tr_per_cyl)) << "||Number of tracks per cylinder (0x30-0x33)" << endl;
        proplist << "Cylinders per Unit||" << QString::number(tsk_getu32(endian, bsdpart->cyl_per_unit)) << "||Number of cylinders per unit (0x34-0x37)" << endl;
        proplist << "Sectors per Cylinder||" << QString::number(tsk_getu32(endian, bsdpart->sec_per_cyl)) << "||Number of sectors per cylinder (0x38-0x3B)" << endl;
        proplist << "Sectors per Unit||" << QString::number(tsk_getu32(endian, bsdpart->sec_per_unit)) << "||Number of sectors per unit (0x3C-0x3F)" << endl;
        proplist << "Spare Sectors per Track||" << QString::number(tsk_getu16(endian, bsdpart->spare_per_tr)) << "||Number of spare sectors per track (0x40-0x41)" << endl;
        proplist << "Spare Sectors per Cylinder||" << QString::number(tsk_getu16(endian, bsdpart->spare_per_cyl)) << "||Number of spare sectors per cylinder (0x42-0x43)" << endl;
        proplist << "Alternate Cylinders Per Unit||" << QString::number(tsk_getu32(endian, bsdpart->alt_per_unit)) << "||Number of alternate cylinders per unit (0x44-0x47)" << endl;
        proplist << "Rotational Disk Speed||" << QString::number(tsk_getu16(endian, bsdpart->rpm)) << "||Rotational Speed of Disk (0x48-0x49)" << endl;
        proplist << "Hardware Sector Interleave||" << QString::number(tsk_getu16(endian, bsdpart->interleave)) << "||Hardware sector interleave (0x4A-0x4B)" << endl;
        proplist << "Track Skew||" << QString::number(tsk_getu16(endian, bsdpart->trackskew)) << "||Track skew (0x4C-0x4D)" << endl;
        proplist << "Cylinder Skew||" << QString::number(tsk_getu16(endian, bsdpart->cylskew)) << "||Cylinder skew (0x4E-0x4F)" << endl;
        proplist << "Head Switch Time||" << QString::number(tsk_getu32(endian, bsdpart->headswitch)) << "||Head switch time in microseconds (0x50-0x53)" << endl;
        proplist << "Track-to-Track Seek Time||" << QString::number(tsk_getu32(endian, bsdpart->track_seek)) << "||Track-to-Track seek time in microseconds (0x54-0x57)" << endl;
        proplist << "Flags||" << QString::number(tsk_getu32(endian, bsdpart->flags)) << "||Flags (0x58-0x5B)" << endl;
        proplist << "Drive Specific Data||" << QString::fromUtf8(reinterpret_cast<char*>(bsdpart->drivedata)) << "||Drive Specific Data (0x5C-0x6F)" << endl;
        proplist << "Reserved||" << "Reserved" << "||Reserved (0x70-0x83)" << endl;
        proplist << "Signature||" << QString::number(tsk_getu32(endian, bsdpart->magic2)) << "||Signature value should be 0x82564557 (0x84-0x87)" << endl;
        proplist << "Checksum||" << QString::number(tsk_getu16(endian, bsdpart->checksum)) << "||Checksum (0x88-0x89)" << endl;
        proplist << "Number of Partitions||" << QString::number(tsk_getu16(endian, bsdpart->num_parts)) << "||Number of partitions (0x8A-0x8B)" << endl;
        proplist << "Boot Area Size||" << QString::number(tsk_getu32(endian, bsdpart->bootarea_size)) << "||Size of boot area (0x8C-0x8F)" << endl;
        proplist << "Super Block Maximum Size||" << QString::number(tsk_getu32(endian, bsdpart->sb_size)) << "||Maximum size of the file system boot super block (0x90-0x93)" << endl;
        proplist << "Unused||" << "Unused" << "||Unused (0x0194-0x01FF)" << endl;
    }
    else if(curvolinfo->vstype == TSK_VS_TYPE_SUN)
    {
        char* buf;
        ssize_t cnt;
        TSK_ENDIAN_ENUM endian = curvolinfo->endian;
        buf = (char*)tsk_malloc(curvolinfo->block_size);
        cnt = tsk_vs_read_block(curvolinfo, SUN_SPARC_PART_SOFFSET, buf, curvolinfo->block_size);
        if(cnt != curvolinfo->block_size)
        {
            // print error here.
        }
        sunsparcpart = (sun_dlabel_sparc*)buf;
        sunx86part = (sun_dlabel_i386*)buf;
        if(tsk_vs_guessu16(curvolinfo, sunsparcpart->magic, SUN_MAGIC) == 0)
        {
            if(tsk_getu32(endian, sunsparcpart->sanity) == SUN_SANITY) // populate sparc
            {
                proplist << "ASCII Label||" << QString::fromUtf8(reinterpret_cast<char*>(sunsparcpart->asciilabel)) << "||ASCII label (0x00-0x7F)" << endl;
                proplist << "Version||" << QString::number(tsk_getu32(endian, sunsparcpart->version)) << "||Version (0x80-0x83)" << endl;
                proplist << "Volume Name||" << QString::fromUtf8(reinterpret_cast<char*>(sunsparcpart->vol_name)) << "||Volume Name (0x84-0x8C)" << endl;
                proplist << "Number of Partitions||" << QString::number(tsk_getu16(endian, sunsparcpart->num_parts)) << "||Number of partitions (0x8D-0x8E)" << endl;
                proplist << "Signature Sanity||" << QString::number(tsk_getu32(endian, sunsparcpart->sanity)) << "||Signature Value Sanity should be 0x600DDEEE (0xBC-0xBF)" << endl;
                proplist << "Writing Sectors to Skip||" << QString::number(tsk_getu16(endian, sunsparcpart->write_reinstruct)) << "||Sectors to skip, writing (0x0106-0x0107)" << endl;
                proplist << "Reading Sectors to Skip||" << QString::number(tsk_getu16(endian, sunsparcpart->read_reinstruct)) << "||Sectors to skip, reading (0x0108-0x0109)" << endl;
                proplist << "Reserved||" << "Reserved" << "||Reserved (0x010A-0x01A3)" << endl;
                proplist << "Disk Speed||" << QString::number(tsk_getu16(endian, sunsparcpart->rpm)) << "||Disk Speed (0x01A4-0x01A5)" << endl;
                proplist << "Physical Cylinder Count||" << QString::number(tsk_getu16(endian, sunsparcpart->num_ph_cyl)) << "||Number of physical cylinders (0x01A6-0x01A7)" << endl;
                proplist << "Alternates per Cylinder||" << QString::number(tsk_getu16(endian, sunsparcpart->alt_per_cyl)) << "||Alternates per cylinder (0x01A8-0x01A9)" << endl;
                proplist << "Reserved||" << "Reserved" << "||Reserved (0x01AA-0x01AD)" << endl;
                proplist << "Interleave||" << QString::number(tsk_getu16(endian, sunsparcpart->interleave)) << "||Interleave (0x01AE-0x01AF)" << endl;
                proplist << "Data Cylinder Count||" << QString::number(tsk_getu16(endian, sunsparcpart->num_cyl)) << "||Number of data cylinders (0x01B0-0x01B1)" << endl;
                proplist << "Alternate Cylinder Count||" << QString::number(tsk_getu16(endian, sunsparcpart->num_alt_cyl)) << "||Number of alternate cylinders (0x01B2-0x01B3)" << endl;
                proplist << "Number of Heads||" << QString::number(tsk_getu16(endian, sunsparcpart->num_head)) << "||Number of heads (0x01B4-0x01B5)" << endl;
                proplist << "Sectors per Track Count||" << QString::number(tsk_getu16(endian, sunsparcpart->sec_per_tr)) << "||Number of sectors per track (0x01B6-0x01B7)" << endl;
                proplist << "Reserved||" << "Reserved" << "||Reserved (0x01B8-0x01BB)" << endl;
                proplist << "Signature||" << QString::number(tsk_getu16(endian, sunsparcpart->magic)) << "||Signature value should be 0xDABE (0x01FC-0x01FD)" << endl;
                proplist << "Checksum||" << QString::number(tsk_getu16(endian, sunsparcpart->checksum)) << "||Checksum (0x01FE-0x01FF)" << endl;
            }
            else if(tsk_getu32(endian, sunx86part->sanity) == SUN_SANITY) // populate i386
            {
                proplist << "Signature Sanity||" << QString::number(tsk_getu32(endian, sunx86part->sanity)) << "||Signature Value Sanity should be 0x600DDEEE (0x0C-0x0F)" << endl;
                proplist << "Version||" << QString::number(tsk_getu32(endian, sunx86part->version)) << "||Version (0x10-0x13)" << endl;
                proplist << "Volume Name||" << QString::fromUtf8(reinterpret_cast<char*>(sunx86part->vol_name)) << "||Volume Name (0x14-0x1B)" << endl;
                proplist << "Sector Size||" << QString::number(tsk_getu16(endian, sunx86part->sec_size)) << "||Sector sie (0x1C-0x1D)" << endl;
                proplist << "Number of Partitions||" << QString::number(tsk_getu16(endian, sunx86part->num_parts)) << "||Number of partitions (0x1E-0x1F)" << endl;
                proplist << "Volume Label||" << QString::fromUtf8(reinterpret_cast<char*>(sunx86part->asciilabel)) << "||Volume label (0x0148-0x01C7)" << endl;
                proplist << "Physical Cylinder Count||" << QString::number(tsk_getu32(endian, sunx86part->num_ph_cyl)) << "||Number of physical cylinders (0x01C8-0x01CB)" << endl;
                proplist << "Data Cylinder Count||" << QString::number(tsk_getu32(endian, sunx86part->num_cyl)) << "||Number of data cylinders (0x01CC-0x01CF)" << endl;
                proplist << "Alternate Cylinder Count||" << QString::number(tsk_getu16(endian, sunx86part->num_alt_cyl)) << "||Number of alternate cylinders (0x01D0-0x01D1)" << endl;
                proplist << "Cylinder offset||" << QString::number(tsk_getu16(endian, sunx86part->cyl_offset)) << "||Cylinder offset (0x01D2-0x01D3)" << endl;
                proplist << "Number of Heads||" << QString::number(tsk_getu32(endian, sunx86part->num_head)) << "||Number of heads (0x01D4-0x01D7)" << endl;
                proplist << "Sectors per Track Count||" << QString::number(tsk_getu32(endian, sunx86part->sec_per_tr)) << "||Number of sectors per track (0x01D8-0x01DB)" << endl;
                proplist << "Interleave||" << QString::number(tsk_getu16(endian, sunx86part->interleave)) << "||Interleave (0x01DC-0x01DD)" << endl;
                proplist << "Skew||" << QString::number(tsk_getu16(endian, sunx86part->skew)) << "||Skew (0x01DE-0x01DF)" << endl;
                proplist << "Alternates per Cylinder||" << QString::number(tsk_getu16(endian, sunx86part->alt_per_cyl)) << "||Number of alternates per cylinder (0x01E0-0x01E1)" << endl;
                proplist << "Disk Speed||" << QString::number(tsk_getu16(endian, sunx86part->rpm)) << "||Disk Speed (0x01E2-0x01E3)" << endl;
                proplist << "Writing Sectors to Skip||" << QString::number(tsk_getu16(endian, sunx86part->write_reinstruct)) << "||Sectors to skip, writing (0x01E4-0x01E5)" << endl;
                proplist << "Reading Sectors to Skip||" << QString::number(tsk_getu16(endian, sunx86part->read_reinstruct)) << "||Sectors to skip, reading (0x01E6-0x01E7)" << endl;
                proplist << "Reserved||" << "Reserved" << "||Reserved (0x01E8-0x01EF)" << endl;
                proplist << "Reserved||" << "Reserved" << "||Reserved (0x01F0-0x0x01FB)" << endl;
                proplist << "Signature||" << QString::number(tsk_getu16(endian, sunx86part->magic)) << "||Signature value should be 0xDABE (0x01FC-0x01FD)" << endl;
                proplist << "Checksum||" << QString::number(tsk_getu16(endian, sunx86part->checksum)) << "||Checksum (0x01FE-0x01FF)" << endl;
            }
            free(buf);
        }
    }
    else if(curvolinfo->vstype == TSK_VS_TYPE_MAC)
    {
        TSK_ENDIAN_ENUM endian = curvolinfo->endian;
        macpart = (mac_part*)curvolinfo;
        proplist << "Signature||" << "0x504D" << "||Signature Value should be 0x504D (0x00-0x01)" << endl;
        proplist << "Reserved||" << "Reserved" << "||Reserved (0x02-0x03)" << endl;
        proplist << "Number of Partitions||" << QString::number(tsk_getu32(endian, macpart->pmap_size)) << "||Total Number of Partitions (0x08-0x0B)" << endl;
    }
    else if(curvolinfo->vstype == TSK_VS_TYPE_GPT)
    {
        char* sect_buf;
        ssize_t cnt;
        TSK_ENDIAN_ENUM endian = curvolinfo->endian;
        sect_buf = (char*)tsk_malloc(curvolinfo->block_size);
        gptpart = (gpt_head*)sect_buf;
        cnt = tsk_vs_read_block(curvolinfo, GPT_PART_SOFFSET + 1, sect_buf, curvolinfo->block_size);
        if(cnt != curvolinfo->block_size)
        {
            // print error here
        }
        free(sect_buf);
        proplist << "Signature||" << QString::number(tsk_getu64(endian, gptpart->signature)) << "||Signature Value should be EFI PART, 0x5452415020494645 (0x00-0x07)" << endl;
        proplist << "Version||" << QString::number(tsk_getu32(endian, gptpart->version)) << "||Version (0x08-0x0B)" << endl;
        proplist << "GPT Header Size (bytes)||" << QString::number(tsk_getu32(endian, gptpart->head_size_b)) << "||Size of GPT header in bytes (0x0C-0x0F)" << endl;
        proplist << "GPT Header Checksum||" << QString::number(tsk_getu32(endian, gptpart->head_crc)) << "||CRC32 checksum of GPT header (0x10-0x13)" << endl;
        proplist << "Reserved||" << "Reserved" << "||Reserved (0x14-0x17)" << endl;
        proplist << "Current GPT Header LBA||" << QString::number(tsk_getu64(endian, gptpart->head_lba)) << "||Logical Block Address of the current GPT header structure (0x18-0x1F)" << endl;
        proplist << "Other GPT Header LBA||" << QString::number(tsk_getu64(endian, gptpart->head2_lba)) << "||Logical Block Address of the other GPT header structure (0x20-0x27)" << endl;
        proplist << "Partition Area Start LBA||" << QString::number(tsk_getu64(endian, gptpart->partarea_start)) << "||Logical Block Address of the start of the partition area (0x28-0x2F)" << endl;
        proplist << "Partition End Area LBA||" << QString::number(tsk_getu64(endian, gptpart->partarea_end)) << "||Logical Block Address of the end of the partition area (0x30-0x37)" << endl;
        sprintf(asc, "%" PRIx64 "%" PRIx64 "", tsk_getu64(endian, &(gptpart->guid[8])), tsk_getu64(endian, &(gptpart->guid[0])));
        proplist << "Disk GUID||" << QString::fromStdString(std::string(asc)) << "||Disk GUID (0x38-0x47)" << endl;
        proplist << "LBA of Start of the Partition Table||" << QString::number(tsk_getu64(endian, gptpart->tab_start_lba)) << "||Logical Block Address of the start of the partition table (0x48-0x4F)" << endl;
        proplist << "Number of Partition Table Entries||" << QString::number(tsk_getu32(endian, gptpart->tab_num_ent)) << "||Number of entries in the partition table (0x50-0x53)" << endl;
        proplist << "Partition Table Entry Size||" << QString::number(tsk_getu32(endian, gptpart->tab_size_b)) << "||Size of each entry in the partition table (0x54-0x57)" << endl;
        proplist << "Partition Table Checksum||" << QString::number(tsk_getu32(endian, gptpart->tab_crc)) << "||CRC32 checksum of the partition table (0x58-0x5B)" << endl;
        proplist << "Reserved||" << "Reserved" << "||Reserved (0x5C-0x01FF)" << endl;
    }
    proplist.flush();
    vpropfile.close();
    //qDebug() << "vpropfile: close";
}

//void WriteEvidenceProperties(TSK_IMG_INFO* curimginfo)
void WriteEvidenceProperties(TSK_IMG_INFO* curimginfo, QString evidencepath, QString imgfullpath)
{
    QFile epropfile(evidencepath + "prop");
    epropfile.open(QIODevice::WriteOnly | QIODevice::Text);
    //qDebug() << "epropfile: open";
    QTextStream proplist(&epropfile);
    IMG_EWF_INFO* ewfinfo = NULL;
    uint8_t* ewfvalue = (uint8_t*)malloc(sizeof(uint8_t)*64);
    uint8_t uvalue8bit = 0;
    int8_t value8bit = 0;
    uint32_t value32bit = 0;
    uint64_t value64bit = 0;
    size64_t size64bit = 0;
    libewf_error_t* ewferror = NULL;
    proplist << QString("File Format||") << QString(tsk_img_type_todesc((TSK_IMG_TYPE_ENUM)curimginfo->itype)) << QString("||File Format the evidence data is stored in. Usually it is either a raw image (.dd/.001) or an embedded image (.E01/.AFF). A raw image contains only the data from the evidence. The embedded image contains other descriptive information from the acquisition.") << endl;
    proplist << QString("Sector Size||") << QString(QString::number(curimginfo->sector_size) + " bytes||") << QString("Sector size of the device. A Sector is a subdivision of a disk where data is stored. It is the smallest value used to divide the disk.") << endl;
    proplist << QString("Sector Count||") << QString(QString::number((int)((float)curimginfo->size/(float)curimginfo->sector_size)) + " sectors||") << QString("The number of sectors in the disk.") << endl;
    proplist << QString("Image Path||") << imgfullpath << QString("||Location where the evidence image is stored and read from.") << endl;
    if(TSK_IMG_TYPE_ISAFF(curimginfo->itype)) // its AFF
    {
    }
    else if(TSK_IMG_TYPE_ISEWF(curimginfo->itype)) // its EWF
    {
        ewfinfo = (IMG_EWF_INFO*)curimginfo;
        if(libewf_handle_get_utf8_header_value_case_number(ewfinfo->handle, ewfvalue, 64, &ewferror) == 1)
            proplist << "Case Number||" << QString::fromUtf8(reinterpret_cast<char*>(ewfvalue)) << "||The case number the image is associated" << endl;
        else
            libewf_error_fprint(ewferror, stdout);
        if(libewf_handle_get_utf8_header_value_description(ewfinfo->handle, ewfvalue, 64, &ewferror) == 1)
            proplist << "Description||" << QString::fromUtf8(reinterpret_cast<char*>(ewfvalue)) << "||Description of the acquisition and or evidence item" << endl;
        else
            libewf_error_fprint(ewferror, stdout);
        if(libewf_handle_get_utf8_header_value_examiner_name(ewfinfo->handle, ewfvalue, 64, &ewferror) == 1)
            proplist << "Examiner Name||" << QString::fromUtf8(reinterpret_cast<char*>(ewfvalue)) << "||Name of the examiner who acquired the image" << endl;
        else
            libewf_error_fprint(ewferror, stdout);
        if(libewf_handle_get_utf8_header_value_evidence_number(ewfinfo->handle, ewfvalue, 64, &ewferror) == 1)
            proplist << "Evidence Number||" << QString::fromUtf8(reinterpret_cast<char*>(ewfvalue)) << "||Unique number identifying the evidence item" << endl;
        else
            libewf_error_fprint(ewferror, stdout);
        if(libewf_handle_get_utf8_header_value_notes(ewfinfo->handle, ewfvalue, 255, &ewferror) == 1)
            proplist << "Notes||" << QString::fromUtf8(reinterpret_cast<char*>(ewfvalue)) << "||Any notes related to the acquisition" << endl;
        else
            libewf_error_fprint(ewferror, stdout);
        if(libewf_handle_get_utf8_header_value_acquiry_date(ewfinfo->handle, ewfvalue, 64, &ewferror) == 1)
            proplist << "Acquisition Date||" << QString::fromUtf8(reinterpret_cast<char*>(ewfvalue)) << "||Date the acquisition was made" << endl;
        else
            libewf_error_fprint(ewferror, stdout);
        if(libewf_handle_get_utf8_header_value_system_date(ewfinfo->handle, ewfvalue, 64, &ewferror) == 1)
            proplist << "System Date||" << QString::fromUtf8(reinterpret_cast<char*>(ewfvalue)) << "||Date for the system acquiring the image" << endl;
        else
            libewf_error_fprint(ewferror, stdout);
        if(libewf_handle_get_utf8_header_value_acquiry_operating_system(ewfinfo->handle, ewfvalue, 64, &ewferror) == 1)
            proplist << "Aquisition OS||" << QString::fromUtf8(reinterpret_cast<char*>(ewfvalue)) << "||Operating System acquiring the image" << endl;
        else
            libewf_error_fprint(ewferror, stdout);
        if(libewf_handle_get_utf8_header_value_acquiry_software_version(ewfinfo->handle, ewfvalue, 64, &ewferror) == 1)
            proplist << "Software Version Used||" << QString::fromUtf8(reinterpret_cast<char*>(ewfvalue)) << "||Version of the software used to acquire the image" << endl;
        else
            libewf_error_fprint(ewferror, stdout);
        if(libewf_handle_get_utf8_header_value_password(ewfinfo->handle, ewfvalue, 64, &ewferror) == 1)
            proplist << "Password||" << QString::fromUtf8(reinterpret_cast<char*>(ewfvalue)) << "||Password to protect the image" << endl;
        else
            libewf_error_fprint(ewferror, stdout);
        if(libewf_handle_get_utf8_header_value_model(ewfinfo->handle, ewfvalue, 64, &ewferror) == 1)
            proplist << "Model||" << QString::fromUtf8(reinterpret_cast<char*>(ewfvalue)) << "||Model of the drive acquired" << endl;
        else
            libewf_error_fprint(ewferror, stdout);
        if(libewf_handle_get_utf8_header_value_serial_number(ewfinfo->handle, ewfvalue, 64, &ewferror) == 1)
            proplist << "Serial Number||" << QString::fromUtf8(reinterpret_cast<char*>(ewfvalue)) << "||Serial number of the drive acquired" << endl;
        else
            libewf_error_fprint(ewferror, stdout);
        if(libewf_handle_get_sectors_per_chunk(ewfinfo->handle, &value32bit, &ewferror) == 1)
            proplist << QString("Sectors Per Chunk||") << QString::number(value32bit) << "||Number of sectors in a image evidence chunk" << endl;
        else
            libewf_error_fprint(ewferror, stdout);
        if(libewf_handle_get_format(ewfinfo->handle, &uvalue8bit, &ewferror) == 1)
        {
            proplist << QString("File Format||");
            switch(uvalue8bit)
            {
                case LIBEWF_FORMAT_EWF:
                    proplist << QString("Original EWF") << "||Format used to store the evidence image" << endl;
                    break;
                case LIBEWF_FORMAT_SMART:
                    proplist << QString("SMART") << "||Format used to store the evidence image" << endl;
                    break;
                case LIBEWF_FORMAT_FTK:
                    proplist << QString("FTK Imager") << "||Format used to store the evidence image" << endl;
                    break;
                case LIBEWF_FORMAT_ENCASE1:
                    proplist << QString("EnCase 1") << "||Format used to store the evidence image" << endl;
                    break;
                case LIBEWF_FORMAT_ENCASE2:
                    proplist << QString("EnCase 2") << "||Format used to store the evidence image" << endl;
                    break;
                case LIBEWF_FORMAT_ENCASE3:
                    proplist << QString("EnCase 3") << "||Format used to store the evidence image" << endl;
                    break;
                case LIBEWF_FORMAT_ENCASE4:
                    proplist << QString("EnCase 4") << "||Format used to store the evidence image" << endl;
                    break;
                case LIBEWF_FORMAT_ENCASE5:
                    proplist << QString("EnCase 5") << "||Format used to store the evidence image" << endl;
                    break;
                case LIBEWF_FORMAT_ENCASE6:
                    proplist << QString("EnCase 6") << "||Format used to store the evidence image" << endl;
                    break;
                case LIBEWF_FORMAT_LINEN5:
                    proplist << QString("Linen 5") << "||Format used to store the evidence image" << endl;
                    break;
                case LIBEWF_FORMAT_LINEN6:
                    proplist << QString("Linen 6") << "||Format used to store the evidence image" << endl;
                    break;
                case LIBEWF_FORMAT_EWFX:
                    proplist << QString("EWFX (extended ewf)") << QString("||Extended EWF Format used to store the evidence image") << endl;
                    break;
                case LIBEWF_FORMAT_LOGICAL_ENCASE5:
                    proplist << QString("LEF EnCase 5") << QString("||Logical Evidence File EnCase 5 Format used to store the evidence image") << endl;
                    break;
                case LIBEWF_FORMAT_LOGICAL_ENCASE6:
                    proplist << QString("LEF EnCase 6") << QString("||Logical Evidence File EnCase 6 Format used to store the evidence image") << endl;
                    break;
                case LIBEWF_FORMAT_UNKNOWN:
                    proplist << QString("Unknown Format") << "||Format used to store the evidence image" << endl;
                    break;
            }
        }
        else
            libewf_error_fprint(ewferror, stdout);
        if(libewf_handle_get_error_granularity(ewfinfo->handle, &value32bit, &ewferror) == 1)
            proplist << QString("Error Granularity||") << QString::number(value32bit) << "||Error block size" << endl;
        else
            libewf_error_fprint(ewferror, stdout);
        proplist << "Compression Method||" << "Deflate" << "||Method used to compress the image" << endl;
        if(libewf_handle_get_compression_values(ewfinfo->handle, &value8bit, &uvalue8bit, &ewferror) == 1)
        {
            proplist << "Compression Level||";
            if(value8bit == LIBEWF_COMPRESSION_NONE)
                proplist << "No Compression";
            else if(value8bit == LIBEWF_COMPRESSION_FAST)
                proplist << "Good (Fast) Compression";
            else if(value8bit == LIBEWF_COMPRESSION_BEST)
                proplist << "Best Compression";
            else
                proplist << "Unknown Compression";
            proplist << "||The more compression, the slower the acquisition but the smaller the file size" << endl;
        }
        else
            libewf_error_fprint(ewferror, stdout);
        if(libewf_handle_get_media_type(ewfinfo->handle, &uvalue8bit, &ewferror) == 1)
        {
            proplist << "Media Type||";
            if(uvalue8bit == LIBEWF_MEDIA_TYPE_REMOVABLE)
                proplist << "Removable Disk";
            else if(uvalue8bit == LIBEWF_MEDIA_TYPE_FIXED)
                proplist << "Fixed Disk";
            else if(uvalue8bit == LIBEWF_MEDIA_TYPE_SINGLE_FILES)
                proplist << "Single Files";
            else if(uvalue8bit == LIBEWF_MEDIA_TYPE_OPTICAL)
                proplist << "Optical Disk (CD/DVD/BD)";
            else if(uvalue8bit == LIBEWF_MEDIA_TYPE_MEMORY)
                proplist << "Memory (RAM)";
            else
                proplist << "Unknown";
            proplist << "||Media type of the original evidence" << endl;
        }
        else
            libewf_error_fprint(ewferror, stdout);
        if(libewf_handle_get_media_flags(ewfinfo->handle, &uvalue8bit, &ewferror) == 1)
        {
            if(uvalue8bit == LIBEWF_MEDIA_FLAG_PHYSICAL)
                proplist << "Media Flag||" << "Physical" << "||Directly connected disk" << endl;
            if(uvalue8bit == LIBEWF_MEDIA_FLAG_FASTBLOC)
                proplist << "Media Flag||" << "Fastbloc Write Blocked" << "||Write blocked disk using Fastbloc" << endl;
            if(uvalue8bit == LIBEWF_MEDIA_FLAG_TABLEAU)
                proplist << "Media Flag||" << "Tableau Write Blocked" << "||Write blocked disk using Tableau" << endl;
        }
        else
            libewf_error_fprint(ewferror, stdout);
        if(libewf_handle_get_bytes_per_sector(ewfinfo->handle, &value32bit, &ewferror) == 1)
            proplist << "Bytes Per Sector||" << QString::number(value32bit) << "||Number of bytes in a sector" << endl;
        else
            libewf_error_fprint(ewferror, stdout);
        if(libewf_handle_get_number_of_sectors(ewfinfo->handle, &value64bit, &ewferror) == 1)
            proplist << "Number of Sectors||" << QString::number(value64bit) << "||Number of total sectors in the original media" << endl;
        else
            libewf_error_fprint(ewferror, stdout);
        if(libewf_handle_get_chunk_size(ewfinfo->handle, &value32bit, &ewferror) == 1)
            proplist << "Chunk Size||" << QString::number(value32bit) << "||The size of an image chunk" << endl;
        else
            libewf_error_fprint(ewferror, stdout);
        if(libewf_handle_get_media_size(ewfinfo->handle, &size64bit, &ewferror) == 1)
            proplist << "Media Size||" << QString::number(size64bit) << "||The size of the media" << endl;
        else
            libewf_error_fprint(ewferror, stdout);
        if(libewf_handle_get_utf8_hash_value_md5(ewfinfo->handle, ewfvalue, 64, &ewferror) == 1)
            proplist << "MD5||" << QString::fromUtf8(reinterpret_cast<char*>(ewfvalue)) << "||The MD5 hash algorithm of the uncompressed image stored as a 128-bit value" << endl;
        else
            libewf_error_fprint(ewferror, stdout);
        if(libewf_handle_get_utf8_hash_value_sha1(ewfinfo->handle, ewfvalue, 64, &ewferror) == 1)
            proplist << "SHA1||" << QString::fromUtf8(reinterpret_cast<char*>(ewfvalue)) << "||The SHA1 hash algorithm of the uncompressed image stored as a 160-bit value" << endl;
        else
            libewf_error_fprint(ewferror, stdout);
        free(ewfvalue);
    }
    else if(TSK_IMG_TYPE_ISRAW(curimginfo->itype)) // is raw
    {
        // nothing i want to add for raw right now.
    }
    else // not supported...
    {
        // log error about unsupported image type.
    } 
    proplist.flush();
    epropfile.close();
    //qDebug() << "epropfile: close";
}

QString GetFileSystemLabel(TSK_FS_INFO* curinfo)
{
    char asc[512];
    if(curinfo != NULL)
    {
        if(curinfo->ftype == TSK_FS_TYPE_EXT2 || curinfo->ftype == TSK_FS_TYPE_EXT3 || curinfo->ftype == TSK_FS_TYPE_EXT4 || curinfo->ftype == TSK_FS_TYPE_EXT_DETECT)
        {
            return QString::fromStdString(std::string(((EXT2FS_INFO*)curinfo)->fs->s_volume_name));
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
            return QString::fromStdString(std::string(asc));
        }
        else if(curinfo->ftype == TSK_FS_TYPE_FAT32)
        {
            sprintf(asc, "%c%c%c%c%c%c%c%c%c%c%c", ((FATXXFS_SB*)((FATFS_INFO*)curinfo)->boot_sector_buffer)->a.f32.vol_lab[0], ((FATXXFS_SB*)((FATFS_INFO*)curinfo)->boot_sector_buffer)->a.f32.vol_lab[1], ((FATXXFS_SB*)((FATFS_INFO*)curinfo)->boot_sector_buffer)->a.f32.vol_lab[2], ((FATXXFS_SB*)((FATFS_INFO*)curinfo)->boot_sector_buffer)->a.f32.vol_lab[3], ((FATXXFS_SB*)((FATFS_INFO*)curinfo)->boot_sector_buffer)->a.f32.vol_lab[4], ((FATXXFS_SB*)((FATFS_INFO*)curinfo)->boot_sector_buffer)->a.f32.vol_lab[5], ((FATXXFS_SB*)((FATFS_INFO*)curinfo)->boot_sector_buffer)->a.f32.vol_lab[6], ((FATXXFS_SB*)((FATFS_INFO*)curinfo)->boot_sector_buffer)->a.f32.vol_lab[7], ((FATXXFS_SB*)((FATFS_INFO*)curinfo)->boot_sector_buffer)->a.f32.vol_lab[8], ((FATXXFS_SB*)((FATFS_INFO*)curinfo)->boot_sector_buffer)->a.f32.vol_lab[9], ((FATXXFS_SB*)((FATFS_INFO*)curinfo)->boot_sector_buffer)->a.f32.vol_lab[10]);
            return QString::fromStdString(std::string(asc));
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
                    return QString::fromStdString(std::string(asc));
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
            iso9660_pvd_node* p;
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
            HFS_INFO* hfs = NULL;
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
            return QString::fromStdString(std::string(asc));
        }
        return "";
    }
    return "";
}

QString ConvertGmtHours(int gmtvar)
{
    int tmpmin = gmtvar - 48;
    int gmthr = tmpmin / 4;
    int gmtmin = (tmpmin % 4)*15;
    QString tmpstring = " GMT ";
    if(abs(gmthr) < 10)
        tmpstring += "0";
    tmpstring += QString::number(gmthr) + QString::number(gmtmin);
    if(gmtmin == 0)
        tmpstring += "0";

    return tmpstring;

}
