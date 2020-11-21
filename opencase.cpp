#include "opencase.h"

// Copyright 2013-2020 Pasquale J. Rinaldi, Jr.
// Distrubted under the terms of the GNU General Public License version 2

/*
void PopulateTskTree(QString evidstring)
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
    //qDebug() << "tmpstr:" << tmpstr;
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
                nodedata << QString(GetFileSystemLabel(fsinfo) + " (" + QString(tsk_fs_type_toname(fsinfo->ftype)).toUpper() + ")") << "0" << QString::number(fsinfo->block_size * fsinfo->block_count) << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << QString(evidid.mid(1) + "-" + vollist.at(i) + "-p0");
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
                        treenodemodel->AddNode(nodedata, QString(evidid.mid(1) + "-" + vollist.at(i)), -1, 0);
                        mutex.unlock();
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
                char* magicbuffer = new char[0];
                magicbuffer = new char[1024];
                QByteArray tmparray;
                tmparray.clear();
                tsk_fs_file_read(tmpfile, 0, magicbuffer, 1024, TSK_FS_FILE_READ_FLAG_NONE);
                tmparray = QByteArray::fromRawData(magicbuffer, 1024);
                QMimeDatabase mimedb;
                QMimeType mimetype = mimedb.mimeTypeForData(tmparray);
                QString mimestr = GenerateCategorySignature(mimetype);
                /*
                if(!mimetype.name().contains("application/octet-stream") && mimetype.name().contains("application/"))
                {
                    qDebug() << mimetype.name() << "mimetype aliases:" << mimetype.aliases();
                    //qDebug() << mimetype.name() << "mimetype parents:" << mimetype.parentMimeTypes();
                    qDebug() << mimetype.name() << "mimetype comment:" << mimetype.comment();
                }
                */
/*                delete[] magicbuffer;
                QString tmpfilestr = "";
                /*
                QFile hshfile;
                if(tmpfile->name->meta_addr == 0 && strcmp(tmpfile->name->name, "$MFT") != 0)
                    hshfile.setFileName(partitionpath + "f" + QString::number(orphancount) + ".a" + QString::number(tmpfile->name->par_addr) + ".stat");
                else
                    hshfile.setFileName(partitionpath + "f" + QString::number(tmpfile->name->meta_addr) + ".a" + QString::number(tmpfile->name->par_addr) + ".stat");
                hshfile.open(QIODevice::ReadOnly | QIODevice::Text);
                if(hshfile.isOpen())
                    tmpfilestr = hshfile.readLine();
                hshfile.close();
                */
/*                if(tmpfilestr.split(",").count() > 12)
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
                treeout << mimestr.split("/").at(0) << mimestr.split("/").at(1); // category << signature
                if(tmpfilestr.split(",").count() >= 16)
                    treeout << tmpfilestr.split(",").at(15); // bookmark value
                else
                    treeout << "0"; // default no bookmark value
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
                    char* startoffset = new char[0];
                    startoffset = new char[2];
                    char* type = new char[0];
                    tsk_fs_read(tmpfile->fs_info, curmftentrystart, startoffset, 2);
                    uint16_t teststart = startoffset[1] * 256 + startoffset[0];
                    adssize = (qint64)teststart;
                    int cnt, i;
                    cnt = tsk_fs_file_attr_getsize(tmpfile);
                    for(i = 0; i < cnt; i++)
                    {
                        type = new char[512];
                        const TSK_FS_ATTR* fsattr = tsk_fs_file_attr_get_idx(tmpfile, i);
                        adssize += 24;
                        adssize += (qint64)fsattr->size;
                        if(ntfs_attrname_lookup(tmpfile->fs_info, fsattr->type, type, 512) == 0)
                        {
                            if(QString::compare(QString(type), "$DATA", Qt::CaseSensitive) == 0)
                            {
                                if(QString::compare(QString(fsattr->name), "") != 0 && QString::compare(QString(fsattr->name), "$I30", Qt::CaseSensitive) != 0)
                                {
                                    //char fbuf[fsattr->size];
                                    char* fbuf = new char[fsattr->size];
                                    ssize_t flen = tsk_fs_attr_read(fsattr, 0, fbuf, fsattr->size, TSK_FS_FILE_READ_FLAG_NONE);
                                    QByteArray fdata = QByteArray::fromRawData(fbuf, flen);
                                    QMimeDatabase adsmimedb;
                                    QMimeType adsmimetype = mimedb.mimeTypeForData(fdata);
                                    QString mimestr = GenerateCategorySignature(adsmimetype);
                                    delete[] fbuf;
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

                                    treeout << adsba.toBase64() << ba2.toBase64() << QString::number(fsattr->size) << "0" << "0" << "0" << "0" << hshstr << mimestr.split("/").at(0) << mimestr.split("/").at(1) << "0" << QString("e" + QString::number(((AddEvidenceVariable*)tmpptr)->evidcnt) + "-v" + QString::number(((AddEvidenceVariable*)tmpptr)->volcnt) + "-p" + QString::number(((AddEvidenceVariable*)tmpptr)->partint) + "-f" + QString::number(tmpfile->name->meta_addr) + ":" + QString::number(fsattr->id) + "-a" + QString::number(tmpfile->name->meta_addr)) << "10" << "0"; // NAME IN FIRST COLUMN
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
                    delete[] type;
                    delete[] startoffset;
                }
            }
            return TSK_WALK_CONT;
        }
        return TSK_WALK_CONT;
    }
    return TSK_WALK_CONT;
}
*/

void LoadTreeModel(QString estring)
{
    //qDebug() << "estring:" << estring;
    /*
    QDir eviddir = QDir(wombatvariable.tmpmntpath);
    QStringList evidfiles = eviddir.entryList(QStringList(estring + "-e*"), QDir::Dirs | QDir::NoSymLinks);
    qDebug() << "evidfiles:" << evidfiles;
    QString epath = wombatvariable.tmpmntpath + evidfiles.at(0) + "/";
    */
    QStringList treelist;
    treelist.clear();
    QFile treefile(wombatvariable.tmpmntpath + "treemodel");
    if(!treefile.isOpen())
        treefile.open(QIODevice::ReadOnly | QIODevice::Text);
    if(treefile.isOpen())
    {
        QTextStream in(&treefile);
        while(!in.atEnd())
        {
            treelist.append(in.readLine());
            //qDebug() << treefile.readLine();
        }
        treefile.close();
    }
    //qDebug() << "treelist count:" << treelist.count();
    for(int i=0; i < treelist.count(); i++)
    {
        QString parentstr = "";
        QStringList nodelist = treelist.at(i).split(",");
        //qDebug() << nodelist.at(11);
        QList<QVariant> nodedata;
        nodedata.clear();
        nodedata << nodelist.at(0) << nodelist.at(1) << nodelist.at(2).toLongLong() << nodelist.at(3).toInt() << nodelist.at(4).toInt() << nodelist.at(5).toInt() << nodelist.at(6).toInt() << nodelist.at(7) << nodelist.at(8) << nodelist.at(9) << nodelist.at(10) << nodelist.at(11);
        if(nodelist.at(14).isEmpty())
            parentstr = "-1";
        else
            parentstr = nodelist.at(14);
        //qDebug() << nodelist.at(11) << parentstr;
        mutex.lock();
        treenodemodel->AddNode(nodedata, parentstr, nodelist.at(12).toInt(), nodelist.at(13).toInt());
        mutex.unlock();
    }
}

void PopulateTreeModel(QString evidstring)
{
    QDir eviddir = QDir(wombatvariable.tmpmntpath);
    QStringList evidlist = eviddir.entryList(QStringList(evidstring.split("/").last() + ".e*"), QDir::NoSymLinks | QDir::Dirs);
    //qDebug() << "evidlist:" << evidlist;
    QString evidid = "." + evidlist.first().split(".").last();
    //qDebug() << "evidid:" << evidid;
    QStringList elist;
    elist.clear();
    QString evidencename = evidlist.first().mid(0, evidlist.first().count() - evidid.count());
    //QString evidencename = evidlist.first().split(evidid).first();
    //qDebug() << "evidencename:" << evidencename;
    QString evidencepath = wombatvariable.tmpmntpath + evidencename + evidid + "/";
    //qDebug() << "evidencepath:" << evidencepath;
    //qDebug() << "evidencefile:" << QString(evidencepath + "stat");
    QFile evidfile(evidencepath + "stat");
    if(!evidfile.isOpen())
        evidfile.open(QIODevice::ReadOnly | QIODevice::Text);
    if(evidfile.isOpen())
        elist = QString(evidfile.readLine()).split(",");
    evidfile.close();
    //qDebug() << "elist:" << elist;
    std::vector<std::string> pathvector;
    pathvector.clear();
    if(elist.at(3).endsWith(".sfs"))
        pathvector.push_back(QString(wombatvariable.imgdatapath + elist.at(3).split("/").last() + "/" + elist.at(3).split("/").last().split(".sfs").first() + ".dd").toStdString());
    else if(elist.at(3).endsWith(".zmg"))
        pathvector.push_back(QString(wombatvariable.imgdatapath + elist.at(3).split("/").last() + "/" + elist.at(3).split("/").last().split(".zmg").first() + ".dd").toStdString());
    else if(elist.at(3).toLower().endsWith(".aff") || elist.at(3).endsWith(".000") || elist.at(3).endsWith(".001"))
    {
        if(elist.at(3).toLower().endsWith(".aff"))
            pathvector.push_back(QString(wombatvariable.imgdatapath + elist.at(3).split("/").last() + "/" + elist.at(3).split("/").last() + ".raw").toStdString());
        else if(elist.at(3).endsWith(".000"))
            pathvector.push_back(QString(wombatvariable.imgdatapath + elist.at(3).split("/").last() + "/" + elist.at(3).split("/").last() + ".raw").toStdString());
        else if(elist.at(3).endsWith(".001"))
            pathvector.push_back(QString(wombatvariable.imgdatapath + elist.at(3).split("/").last() + "/" + elist.at(3).split("/").last() + ".raw").toStdString());
    }
    else if(elist.at(3).toLower().endsWith(".e01"))
        pathvector.push_back(QString(wombatvariable.imgdatapath + elist.at(3).split("/").last() + "/" + elist.at(3).split("/").last() + ".raw").toStdString());
    else
        pathvector.push_back(elist.at(3).toStdString());
    //pathvector.push_back(elist.at(3).toStdString());
    TSK_IMG_INFO* imginfo = NULL;
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
    QList<QVariant> nodedata;
    nodedata.clear();
    nodedata << evidencename << "0" << elist.at(1) << "0" << "0" << "0" << "0" << "0" << "0" << "0" << elist.at(6) << elist.at(5);
    mutex.lock();
    treenodemodel->AddNode(nodedata, "-1", -1, -1);
    mutex.unlock();
    TSK_VS_INFO* vsinfo = NULL;
    QDir voldir = QDir(evidencepath);
    QStringList vollist = voldir.entryList(QStringList("v*"), QDir::NoSymLinks | QDir::Dirs);
    QString partitionpath = "";
    for(int i=0; i < vollist.count(); i++)
    {
        QStringList vlist;
        vlist.clear();
        QString volumepath = evidencepath + vollist.at(i) + "/";
        QFile volfile(volumepath + "stat");
        if(!volfile.isOpen())
            volfile.open(QIODevice::Text | QIODevice::ReadOnly);
        if(volfile.isOpen())
            vlist = QString(volfile.readLine()).split(",");
        volfile.close();
        nodedata.clear();
        nodedata << vlist.at(2) << "0" << vlist.at(1) << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << vlist.at(5);
        mutex.lock();
        treenodemodel->AddNode(nodedata, evidid.mid(1), -1, 0);
        mutex.unlock();
	vsinfo = NULL;
	vsinfo = tsk_vs_open(imginfo, vlist.at(4).toLongLong(), TSK_VS_TYPE_DETECT);
        QDir partdir = QDir(volumepath);
        QStringList partlist = partdir.entryList(QStringList("p*"), QDir::NoSymLinks | QDir::Dirs);
        TSK_STACK* stack;
        stack = tsk_stack_create();
	if(vsinfo == NULL)
	{
	    TSK_FS_INFO* fsinfo = NULL;
	    const TSK_POOL_INFO* poolinfo = tsk_pool_open_img_sing(imginfo, 0, TSK_POOL_TYPE_DETECT);
	    TSK_IMG_INFO* curimginfo = NULL;
	    if(poolinfo == nullptr)
            {
                partitionpath = volumepath + "p0/";
                QStringList plist;
                plist.clear();
                QFile pfile(partitionpath + "stat");
                if(!pfile.isOpen())
                    pfile.open(QIODevice::ReadOnly | QIODevice::Text);
                if(pfile.isOpen())
                    plist = QString(pfile.readLine()).split(",");
                pfile.close();
	        fsinfo = tsk_fs_open_img(imginfo, 0, TSK_FS_TYPE_DETECT);
		nodedata.clear();
		if(fsinfo != NULL) // has fs
		    nodedata << QString(plist.at(2) + "(" + QString(tsk_fs_type_toname(fsinfo->ftype)).toUpper() + ")");
		else
		    nodedata << plist.at(2);
		nodedata << "0" << plist.at(1) << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << plist.at(10);
		mutex.lock();
		treenodemodel->AddNode(nodedata, vlist.at(5), -1, 0);
		mutex.unlock();
		if(fsinfo != NULL)
		{
	    	    partitionlist.append(plist.at(10) + ": " + QString(GetFileSystemLabel(fsinfo)) + " (" + QString(tsk_fs_type_toname(fsinfo->ftype)).toUpper() + ")");
            	    ParseDir(fsinfo, stack, plist.at(3).toInt(), "/", partitionpath);
		}
		else
	    	    partitionlist.append(plist.at(10) + ": " + plist.at(2));
                // ADD ManualCarved Folders HERE
                nodedata.clear();
                nodedata << QByteArray("$Carved-Verified").toBase64() << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "Directory" << "Virtual Directory" << "0" << QString(plist.at(10) + "-pc");
                mutex.lock();
                treenodemodel->AddNode(nodedata, plist.at(10), 3, 0);
                mutex.unlock();
		nodedata.clear();
                nodedata << QByteArray("$Carved-UnVerified").toBase64() << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "Directory" << "Virtual Directory" << "0" << QString(plist.at(10) + "-uc");
                mutex.lock();
                treenodemodel->AddNode(nodedata, plist.at(10), 3, 0);
                mutex.unlock();
            }
	    else
	    {
                if(poolinfo->num_vols > 0)
                {
                    for(int k=0; k < poolinfo->num_vols; k++)
                    {
                        partitionpath = volumepath + "p" + QString::number(k) + "/";
                        QStringList plist;
                        plist.clear();
                        QFile pfile(partitionpath + "stat");
                        if(!pfile.isOpen())
                            pfile.open(QIODevice::ReadOnly | QIODevice::Text);
                        if(pfile.isOpen())
                            plist = QString(pfile.readLine()).split(",");
                        pfile.close();
                        TSK_POOL_VOLUME_INFO curpoolvol = poolinfo->vol_list[k];
                        curimginfo = poolinfo->get_img_info(poolinfo, curpoolvol.block);
                        if(curpoolvol.flags & TSK_POOL_VOLUME_FLAG_ENCRYPTED)
                            fsinfo = tsk_fs_open_img_decrypt(curimginfo, 0, TSK_FS_TYPE_APFS, passwordhash.value(evidstring.split("/", Qt::SkipEmptyParts).last() + "-v" + vlist.at(5).split("-v").last() + "-p" + QString::number(k)).toStdString().c_str());
                        else
                            fsinfo = tsk_fs_open_img(curimginfo, 0, TSK_FS_TYPE_DETECT);
                        nodedata.clear();
                        if(fsinfo != NULL)
                            nodedata << QString(plist.at(2) + "(" + QString(tsk_fs_type_toname(fsinfo->ftype)).toUpper() + ")");
                        else
                            nodedata << plist.at(2);
                        nodedata << "0" << plist.at(1) << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << plist.at(10);
                        mutex.lock();
                        treenodemodel->AddNode(nodedata, vlist.at(5), -1, 0);
                        mutex.unlock();
                        if(fsinfo != NULL)
                        {
	    	            partitionlist.append(plist.at(10) + ": " + plist.at(2) + " (" + QString(tsk_fs_type_toname(fsinfo->ftype)).toUpper() + ")");
                            ParseDir(fsinfo, stack, plist.at(3).toInt(), "/", partitionpath);
                        }
                        else
    	                    partitionlist.append(plist.at(10) + ": " + plist.at(2));
                        // ADD ManualCarved Folder HERE
                        nodedata.clear();
                        nodedata << QByteArray("$Carved-Verified").toBase64() << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "Directory" << "Virtual Directory" << "0" << QString(plist.at(10) + "-pc");
                        mutex.lock();
                        treenodemodel->AddNode(nodedata, plist.at(10), 3, 0);
                        mutex.unlock();
			nodedata.clear();
			nodedata << QByteArray("$Carved-UnVerified").toBase64() << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "Directory" << "Virtual Directory" << "0" << QString(plist.at(10) + "-uc");
			mutex.lock();
			treenodemodel->AddNode(nodedata, plist.at(10), 3, 0);
			mutex.unlock();
                        tsk_fs_close(fsinfo);
                    }
                }
                tsk_pool_close(poolinfo);
                tsk_img_close(curimginfo);
	    }
	}
	else
	{
	    // START LOOP WITH VSINFO PART'S TO MATCH THE PARTLIST VALUES..
	    for(uint j=0; j < vsinfo->part_count; j++)
	    {
		const TSK_VS_PART_INFO* partinfo = NULL;
		partinfo = tsk_vs_part_get(vsinfo, j);
		if(partlist.contains(QString("p" + QString::number(j)))) // ensure there is a directory for it...
		{
		    partitionpath = volumepath + "p" + QString::number(j) + "/";
		    QStringList plist;
		    plist.clear();
		    QFile pfile(partitionpath + "stat");
		    if(!pfile.isOpen())
			pfile.open(QIODevice::ReadOnly | QIODevice::Text);
		    if(pfile.isOpen())
			plist = QString(pfile.readLine()).split(",");
		    pfile.close();
		    TSK_FS_INFO* fsinfo = NULL;
		    const TSK_POOL_INFO* poolinfo = tsk_pool_open_sing(partinfo, TSK_POOL_TYPE_DETECT);
		    TSK_IMG_INFO* curimginfo = NULL;
		    if(poolinfo == nullptr) // no pool, just get fsinfo
		    {
			if(partinfo->flags == TSK_VS_PART_FLAG_ALLOC)
			    fsinfo = tsk_fs_open_vol(partinfo, TSK_FS_TYPE_DETECT);
			nodedata.clear();
			if(fsinfo != NULL) // has fs
			    nodedata << QString(plist.at(2) + "(" + QString(tsk_fs_type_toname(fsinfo->ftype)).toUpper() + ")");
			else
			    nodedata << plist.at(2);
			nodedata << "0" << plist.at(1) << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << plist.at(10);
			mutex.lock();
			treenodemodel->AddNode(nodedata, vlist.at(5), -1, 0);
			mutex.unlock();
			if(fsinfo != NULL)
			{
	    	    	    partitionlist.append(plist.at(10) + ": " + QString(GetFileSystemLabel(fsinfo)) + " (" + QString(tsk_fs_type_toname(fsinfo->ftype)).toUpper() + ")");
                    	    ParseDir(fsinfo, stack, plist.at(3).toInt(), "/", partitionpath);
			}
			else
	    	    	    partitionlist.append(plist.at(10) + ": " + plist.at(2));
                        // ADD ManualCarved Folder HERE
                        nodedata.clear();
                        nodedata << QByteArray("$Carved-Verified").toBase64() << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "Directory" << "Virtual Directory" << "0" << QString(plist.at(10) + "-pc");
                        mutex.lock();
                        treenodemodel->AddNode(nodedata, plist.at(10), 3, 0);
                        mutex.unlock();
			nodedata.clear();
			nodedata << QByteArray("$Carved-UnVerified").toBase64() << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "Directory" << "Virtual Directory" << "0" << QString(plist.at(10) + "-uc");
			mutex.lock();
			treenodemodel->AddNode(nodedata, plist.at(10), 3, 0);
			mutex.unlock();
		    }
		    else // has pool, loop over poolvol's
		    {
                        if(poolinfo->num_vols > 0)
                        {
                            for(int k=0; k < poolinfo->num_vols; k++)
                            {
                                int pint = j + k;
                                partitionpath = volumepath + "p" + QString::number(pint) + "/";
                                QStringList plist;
                                plist.clear();
                                QFile pfile(partitionpath + "stat");
                                if(!pfile.isOpen())
                                    pfile.open(QIODevice::ReadOnly | QIODevice::Text);
                                if(pfile.isOpen())
                                    plist = QString(pfile.readLine()).split(",");
                                pfile.close();
                                TSK_POOL_VOLUME_INFO curpoolvol = poolinfo->vol_list[k];
                                curimginfo = poolinfo->get_img_info(poolinfo, curpoolvol.block);
                                if(curpoolvol.flags & TSK_POOL_VOLUME_FLAG_ENCRYPTED)
                                    fsinfo = tsk_fs_open_img_decrypt(curimginfo, 0, TSK_FS_TYPE_APFS, passwordhash.value(evidstring.split("/", Qt::SkipEmptyParts).last() + "-v" + vlist.at(5).split("-v").last() + "-p" + QString::number(pint)).toStdString().c_str());
                                else
                                    fsinfo = tsk_fs_open_img(curimginfo, 0, TSK_FS_TYPE_DETECT);
                                nodedata.clear();
                                if(fsinfo != NULL)
                                    nodedata << QString(plist.at(2) + "(" + QString(tsk_fs_type_toname(fsinfo->ftype)).toUpper() + ")");
                                else
                                    nodedata << plist.at(2);
                                nodedata << "0" << plist.at(1) << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << plist.at(10);
                                mutex.lock();
                                treenodemodel->AddNode(nodedata, vlist.at(5), -1, 0);
                                mutex.unlock();
                                if(fsinfo != NULL)
                                {
	    	                    partitionlist.append(plist.at(10) + ": " + plist.at(2) + " (" + QString(tsk_fs_type_toname(fsinfo->ftype)).toUpper() + ")");
                                    ParseDir(fsinfo, stack, plist.at(3).toInt(), "/", partitionpath);
                                }
                                else
    	                            partitionlist.append(plist.at(10) + ": " + plist.at(2));
                                // ADD ManualCarved Folder HERE
                                nodedata.clear();
                                nodedata << QByteArray("$Carved-Verified").toBase64() << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "Directory" << "Virtual Directory" << "0" << QString(plist.at(10) + "-pc");
                                mutex.lock();
                                treenodemodel->AddNode(nodedata, plist.at(10), 3, 0);
                                mutex.unlock();
				nodedata.clear();
				nodedata << QByteArray("$Carved-UnVerified").toBase64() << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "Directory" << "Virtual Directory" << "0" << QString(plist.at(10) + "-uc");
				mutex.lock();
				treenodemodel->AddNode(nodedata, plist.at(10), 3, 0);
				mutex.unlock();
                                tsk_fs_close(fsinfo);
                            }
                        }
                        tsk_pool_close(poolinfo);
                        tsk_img_close(curimginfo);
		    }
		}
	    }
	}
        tsk_stack_free(stack);
        // ADD ManualCarved Folder HERE
        nodedata.clear();
        nodedata << "$Manual Carved" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "Directory" << "Virtual Directory" << "0" << QString(evidid.mid(1) + "-mc");
        mutex.lock();
        treenodemodel->AddNode(nodedata, evidid.mid(1), 3, 0);
        mutex.unlock();
    }
    tsk_vs_close(vsinfo);
    tsk_img_close(imginfo);
    //InitializeHashList();
    //InitializeTaggedList();
}

void ParseDir(TSK_FS_INFO* fsinfo, TSK_STACK* stack, TSK_INUM_T dirnum, const char* path, QString partitionpath)
{
    int pathcount = partitionpath.split("/", Qt::SkipEmptyParts).count();
    QString evalue = partitionpath.split("/", Qt::SkipEmptyParts).at(pathcount - 3).split(".").last();
    QString vvalue = partitionpath.split("/", Qt::SkipEmptyParts).at(pathcount - 2);
    QString pvalue = partitionpath.split("/", Qt::SkipEmptyParts).at(pathcount - 1);
    TSK_FS_DIR* fsdir = NULL;
    fsdir = tsk_fs_dir_open_meta(fsinfo, dirnum);
    if(fsdir != NULL)
    {
        std::string path2 = "";
        for(uint i=0; i < tsk_fs_dir_getsize(fsdir); i++)
        {
            TSK_FS_FILE* fsfile = NULL;
            fsfile = tsk_fs_dir_get(fsdir, i);
            if(fsfile->meta == NULL)
            {
                if(fsfile->name->meta_addr || (fsfile->name->flags & TSK_FS_NAME_FLAG_ALLOC))
                    fsdir->fs_info->file_add_meta(fsdir->fs_info, fsfile, fsfile->name->meta_addr);
            }
            if(fsfile != NULL && !TSK_FS_ISDOT(fsfile->name->name))
            {
                // DO MY STUFF HERE...
                QString parentstr = "";
                QStringList treeout;
                treeout.clear();
                QByteArray ba;
                ba.clear();
                ba.append(QString(fsfile->name->name).toUtf8());
                treeout << ba.toBase64(); // NAME - 0
                ba.clear();
                ba.append(QString::fromStdString(std::string(path)).toUtf8());
                treeout << ba.toBase64(); // FULL PATH - 1
                if(fsfile->name->par_addr == fsfile->fs_info->root_inum)
                    parentstr = evalue + "-" + vvalue + "-" + pvalue;
                else
                    parentstr = evalue + "-" + vvalue + "-" + pvalue + "-f" + QString::number(fsfile->name->par_addr);
                if(fsfile->meta != NULL)
                {
                    treeout << QString::number(fsfile->meta->size) << QString::number(fsfile->meta->crtime) << QString::number(fsfile->meta->mtime) << QString::number(fsfile->meta->atime) << QString::number(fsfile->meta->ctime); // SIZE, 4-DATES - 2, 3, 4, 5, 6
                }
                else
                {
                    treeout << "0" << "0" << "0" << "0" << "0"; // SIZE, 4-DATES - 2, 3, 4, 5, 6
                }
                treeout << "0"; // HASH - 7
                if(fsfile->name->type == TSK_FS_NAME_TYPE_DIR) // DIRECTORY
                    treeout << "Directory" << "Directory"; // CAT/SIG - 8/9
                else if(fsfile->name->type == TSK_FS_NAME_TYPE_VIRT_DIR || QString(fsfile->name->name).contains("$OrphanFiles")) // VIRTUAL DIRECTORY
                    treeout << "Directory" << "Virtual Directory"; // CAT/SIG - 8,9
                else
                {
                    char* magicbuffer = new char[0];
                    magicbuffer = new char[1024];
                    QByteArray tmparray("intro");
                    tmparray.clear();
                    tsk_fs_file_read(fsfile, 0, magicbuffer, 1024, TSK_FS_FILE_READ_FLAG_NONE);
                    tmparray = QByteArray::fromRawData(magicbuffer, 1024);
                    QMimeDatabase mimedb;
                    const QMimeType mimetype = mimedb.mimeTypeForFileNameAndData(QString::fromStdString(fsfile->name->name), tmparray);
                    //const QMimeType mimetype = mimedb.mimeTypeForData(tmparray);
                    QString mimestr = GenerateCategorySignature(mimetype);
                    if(mimestr.contains("Unknown")) // generate further analysis
                    {
                        if(tmparray.at(0) == '\x4c' && tmparray.at(1) == '\x00' && tmparray.at(2) == '\x00' && tmparray.at(3) == '\x00' && tmparray.at(4) == '\x01' && tmparray.at(5) == '\x14' && tmparray.at(6) == '\x02' && tmparray.at(7) == '\x00') // LNK File
                            mimestr = "Windows System/Shortcut";
                        else if(strcmp(fsfile->name->name, "INFO2") == 0 && (tmparray.at(0) == 0x04 || tmparray.at(0) == 0x05)) // INFO2 recycle bin file
                            mimestr = "Windows System/Recycler";
                        else if(QString::fromStdString(fsfile->name->name).startsWith("$I") && (tmparray.at(0) == 0x01 || tmparray.at(0) == 0x02)) // $I recycle bin file
                            mimestr = "Windows System/Recycle.Bin";
			else if(QString::fromStdString(fsfile->name->name).endsWith(".pf") && tmparray.at(4) == 0x53 && tmparray.at(5) == 0x43 && tmparray.at(6) == 0x43 && tmparray.at(7) == 0x41)
			    mimestr = "Windows System/Prefetch";
			else if(QString::fromStdString(fsfile->name->name).endsWith(".pf") && tmparray.at(0) == 0x4d && tmparray.at(1) == 0x41 && tmparray.at(2) == 0x4d)
			    mimestr = "Windows System/Prefetch";
                        else if(tmparray.at(0) == '\x72' && tmparray.at(1) == '\x65' && tmparray.at(2) == '\x67' && tmparray.at(3) == '\x66') // 72 65 67 66 | regf
                            mimestr = "Windows System/Registry";
                    }
                    delete[] magicbuffer;
                    treeout << mimestr.split("/").at(0) << mimestr.split("/").at(1); // CAT/SIG - 8, 9
                }
                treeout << "0"; // TAG - 10
                // PUT ID INFO HERE FOR NAME IN FIRST COLUMN
		QString objid = "";
                if(fsfile->name->meta_addr == 0 && strcmp(fsfile->name->name, "$MFT") != 0)
                    objid = evalue + "-" + vvalue + "-" + pvalue + "-fo" + QString::number(orphancount) + "-a" + QString::number(fsfile->name->par_addr); // Orphan ID - 11
                else if(fsfile->meta == NULL)
                    objid = evalue + "-" + vvalue + "-" + pvalue + "-fd" + QString::number(fsfile->name->meta_addr) + "-a" + QString::number(fsfile->name->par_addr); // Deleted Recovered ID - 11
                else
                    objid =  evalue + "-" + vvalue + "-" + pvalue + "-f" + QString::number(fsfile->name->meta_addr) + "-a" + QString::number(fsfile->name->par_addr); // ID - 11
		treeout << objid;
                // POPULATE MFTBLOCKLIST
                if(fsfile->name->meta_addr == 0 && strcmp(fsfile->name->name, "$MFT") == 0)
                {
		    mftblockhash.insert(objid, GetBlockList(fsfile));
                }

                //if(fsfile->meta != NULL)
                //    treeout << QString::number(fsfile->meta->type); // file type - 12
                //else
                treeout << QString::number(fsfile->name->type); // file type - 12
                if(fsfile->name->meta_addr == 0 && strcmp(fsfile->name->name, "$MFT") != 0)
                {
                    treeout << "1"; // orphan - 13
                }
                else
                {
                    if(fsfile->meta != NULL)
                    {
                        if(((fsfile->meta->flags & TSK_FS_META_FLAG_UNALLOC) == 2) && ((fsfile->meta->flags & TSK_FS_META_FLAG_USED) == 4))
                        {
                            treeout << "1"; // UNALLOC - 13
                        }
                        else
                        {
                            treeout << "0"; // ALLOC - 13
                        }
                    }
                    else
                    {
                        if((fsfile->name->flags & TSK_FS_NAME_FLAG_UNALLOC) == 0x02)
                        {
                            treeout << "1"; // UNALLOC - 13
                        }
                        else
                        {
                            treeout << "0"; // ALLOC = 13
                        }
                    }
                }
                QList<QVariant> nodedata;
                nodedata.clear();
                for(int i=0; i < 12; i++)
                    nodedata << treeout.at(i);
                mutex.lock();
                treenodemodel->AddNode(nodedata, parentstr, treeout.at(12).toInt(), treeout.at(13).toInt());
                mutex.unlock();
                listeditems.append(treeout.at(11));
                filesfound++;
                isignals->ProgUpd();
                if(fsfile->fs_info->ftype == TSK_FS_TYPE_NTFS_DETECT)
                {
                    char* startoffset = new char[0];
                    char* type = new char[0];
                    QByteArray adsba;
                    adsba.clear();
                    qint64 adssize = 0;
                    TSK_OFF_T curmftentrystart = 0;
                    NTFS_INFO* ntfsinfo = (NTFS_INFO*)fsfile->fs_info;
                    int recordsize = 0;
                    if(ntfsinfo->fs->mft_rsize_c > 0)
                        recordsize = ntfsinfo->fs->mft_rsize_c * ntfsinfo->fs->csize * tsk_getu16(fsfile->fs_info->endian, ntfsinfo->fs->ssize);
                    else
                        recordsize = 1 << -ntfsinfo->fs->mft_rsize_c;
                    if(fsfile->meta != NULL)
                        curmftentrystart = tsk_getu16(fsfile->fs_info->endian, ntfsinfo->fs->ssize) * ntfsinfo->fs->csize * tsk_getu64(fsfile->fs_info->endian, ntfsinfo->fs->mft_clust) + recordsize * fsfile->meta->addr + 20;
                    else
                        curmftentrystart = tsk_getu16(fsfile->fs_info->endian, ntfsinfo->fs->ssize) * ntfsinfo->fs->csize * tsk_getu64(fsfile->fs_info->endian, ntfsinfo->fs->mft_clust) + recordsize + 20;
                    startoffset = new char[2];
                    tsk_fs_read(fsfile->fs_info, curmftentrystart, startoffset, 2);
                    uint16_t teststart = startoffset[1] * 256 + startoffset[0];
                    adssize = (qint64)teststart;
                    int cnt, i;
                    cnt = tsk_fs_file_attr_getsize(fsfile);
                    for(i = 0; i < cnt; i++)
                    {
                        type = new char[512];
                        const TSK_FS_ATTR* fsattr = tsk_fs_file_attr_get_idx(fsfile, i);
                        adssize += 24;
                        adssize += (qint64)fsattr->size;
                        if(ntfs_attrname_lookup(fsfile->fs_info, fsattr->type, type, 512) == 0)
                        {
                            if(QString::compare(QString(type), "$DATA", Qt::CaseSensitive) == 0)
                            {
                                if(QString::compare(QString(fsattr->name), "") != 0 && QString::compare(QString(fsattr->name), "$I30", Qt::CaseSensitive) != 0)
                                {
                                    char* fbuf = new char[fsattr->size];
                                    ssize_t flen = tsk_fs_attr_read(fsattr, 0, fbuf, fsattr->size, TSK_FS_FILE_READ_FLAG_NONE);
                                    QByteArray fdata = QByteArray::fromRawData(fbuf, flen);
                                    QMimeDatabase adsmimedb;
                                    QMimeType adsmimetype = adsmimedb.mimeTypeForData(fdata);
                                    QString mimestr = GenerateCategorySignature(adsmimetype);
                                    if(mimestr.contains("Unknown"))
                                        mimestr = "Windows System/Alternate Data Stream";
                                    delete[] fbuf;
                                    adsba.append(QString(QString(fsfile->name->name) + QString(":") + QString(fsattr->name)).toUtf8());
                                    treeout.clear();
                                    treeout << adsba.toBase64() << ba.toBase64() << QString::number(fsattr->size) << "0" << "0" << "0" << "0" << "0" << mimestr.split("/").at(0) << mimestr.split("/").at(1) << "0" << QString(evalue + "-" + vvalue + "-" + pvalue + "-fa" + QString::number(fsattr->id) + "-a" + QString::number(fsfile->name->meta_addr)) << "10" << "0"; // NAME IN FIRST COLUMN
                                    nodedata.clear();
                                    for(int i=0;  i < 12; i++)
                                        nodedata << treeout.at(i);
                                    mutex.lock();
                                    treenodemodel->AddNode(nodedata, QString(evalue + "-" + vvalue + "-" + pvalue + "-f" + QString::number(fsfile->name->meta_addr)), treeout.at(12).toInt(), treeout.at(13).toInt());
                                    mutex.unlock();
                                    listeditems.append(treeout.at(11)); // UNCOMMENT ONCE I CAN CAPTURE ADS IN GEN HASH AND IMG THUMB
                                    filesfound++;
                                    isignals->ProgUpd();
                                }
                            }
                        }
                    }
                    delete[] type;
                    delete[] startoffset;
                }
                if(fsfile->name != NULL)
                {
                    if(fsfile->name->type == TSK_FS_NAME_TYPE_DIR || QString(fsfile->name->name).contains("$OrphanFiles")) // DIRECTORY
                    {
                        if(TSK_FS_ISDOT(fsfile->name->name) == 0)
                        {
                            if(tsk_stack_find(stack, fsfile->name->meta_addr) == 0) // process if it's not on stack
                            {
                                // DO MY RECURSE HERE...
                                tsk_stack_push(stack, fsfile->name->meta_addr);
                                std::string strpath(path);
                                if(strpath.find_last_of("/") == (strpath.length() - 1))
                                    path2 = std::string(path) + std::string(fsfile->name->name);
                                else
                                    path2 = std::string(path) + "/" + std::string(fsfile->name->name);
                                ParseDir(fsinfo, stack, fsfile->name->meta_addr, path2.c_str(), partitionpath);
                                tsk_stack_pop(stack);
                            }
                        }
                    }
                }
            }
            if(fsfile->name->meta_addr == 0 && strcmp(fsfile->name->name, "$MFT") != 0)
                orphancount++;
            tsk_fs_file_close(fsfile);
        }
    }
    tsk_fs_dir_close(fsdir);
}

