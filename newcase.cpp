#include "newcase.h"

// Copyright 2013-2020 Pasquale J. Rinaldi, Jr.
// Distrubted under the terms of the GNU General Public License version 2

void InitializeEvidenceStructure(QString evidname)
{
    qDebug() << "evidname:" << evidname;
    int evidcnt = 0;
    int volcnt = 0;
    int partint = 0;
    QDir eviddir = QDir(wombatvariable.tmpmntpath);
    QStringList evidfiles = eviddir.entryList(QStringList(QString(evidname.split("/").last() + ".e*")), QDir::NoSymLinks | QDir::Dirs);
    evidcnt = evidfiles.at(0).split(".e").last().toInt();
    QString evidencename = evidname.split("/").last();
    const TSK_TCHAR** images;
    std::vector<std::string> fullpathvector;
    fullpathvector.clear();
    if(evidname.endsWith(".sfs"))
        fullpathvector.push_back(QString(wombatvariable.imgdatapath + evidencename.split(".sfs").first() + ".dd").toStdString());
    else if(evidname.endsWith(".zmg"))
        fullpathvector.push_back(QString(wombatvariable.imgdatapath + evidencename.split("/").last().split(".zmg").first() + ".dd").toStdString());
    else
        fullpathvector.push_back(evidname.toStdString());
    images = (const char**)malloc(fullpathvector.size()*sizeof(char*));
    for(uint i=0; i < fullpathvector.size(); i++)
    {
        images[i] = fullpathvector[i].c_str();
    }
    TSK_IMG_INFO* imginfo = NULL;
    imginfo = tsk_img_open(1, images, TSK_IMG_TYPE_DETECT, 0);
    free(images);
    QString evidencepath = wombatvariable.tmpmntpath + evidencename + ".e" + QString::number(evidcnt) + "/";
    QTextStream out;
    QFile evidfile(evidencepath + "stat");
    if(!evidfile.isOpen())
        evidfile.open(QIODevice::Append | QIODevice::Text);
    if(evidfile.isOpen())
    {
        out.setDevice(&evidfile);
        out << QString::number(imginfo->itype) << "," << QString::number(imginfo->size) << "," << QString::number(imginfo->sector_size) << ",";
        out << evidname << "," << QString::number(1) << ",e" + QString::number(evidcnt) << ",0";
        out.flush();
        evidfile.close();
    }
    QStringList treeout;
    treeout << evidencename << "0" << QString::number(imginfo->size) << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << QString("e" + QString::number(evidcnt)); // NAME IN FIRST COLUMN
    QList<QVariant> nodedata;
    nodedata.clear();
    for(int i=0; i < treeout.count(); i++)
        nodedata << treeout.at(i);
    mutex.lock();
    treenodemodel->AddNode(nodedata,  "-1", -1, -1);
    mutex.unlock();
    // Write Evidence Properties Here...
    if(imginfo != NULL)
        WriteEvidenceProperties(imginfo, evidencepath, evidname);
    TSK_VS_INFO* vsinfo = NULL;
    vsinfo = tsk_vs_open(imginfo, 0, TSK_VS_TYPE_DETECT);
    QString volname = "Dummy Volume (NO PART)";
    int voltype = 240;
    int volsectorsize = (int)imginfo->sector_size;
    qint64 voloffset = 0;
    if(vsinfo != NULL)
    {
        voltype = (int)vsinfo->vstype;
        volname = QString::fromUtf8(tsk_vs_type_todesc(vsinfo->vstype));
        volsectorsize = (int)vsinfo->block_size;
        voloffset = (qint64)vsinfo->offset;
    }
    QDir voldir = QDir(evidencepath);
    QStringList volfiles = voldir.entryList(QStringList(QString("v*")), QDir::NoSymLinks | QDir::Dirs);
    volcnt = volfiles.count();
    QString volumepath = evidencepath + "v" + QString::number(volcnt) + "/";
    QDir dir;
    dir.mkpath(volumepath);
    QFile volfile(volumepath + "stat");
    if(!volfile.isOpen())
        volfile.open(QIODevice::Append | QIODevice::Text);
    if(volfile.isOpen())
    {
        out.setDevice(&volfile);
        out << voltype << "," << (qint64)imginfo->size << "," << volname << "," << volsectorsize << "," << voloffset << ",e" + QString::number(evidcnt) + "-v" + QString::number(volcnt) << ",0";
        out.flush();
        volfile.close();
    }
    treeout.clear();
    treeout << volname << "0" << QString::number(imginfo->size) << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << QString("e" + QString::number(evidcnt) + "-v" + QString::number(volcnt)); // NAME IN FIRST COLUMN
    nodedata.clear();
    for(int i=0; i < treeout.count(); i++)
        nodedata << treeout.at(i);
    QString reportstring = "";
    mutex.lock();
    reportstring = "<div id='e" + QString::number(evidcnt) + "'><table width='98%'>";
    reportstring += "<tr><th colspan='2'>Evidence Item (E" + QString::number(evidcnt) + "):" + evidname + "</th></tr>";
    reportstring += "<tr class='odd vtop'><td>Image Size:</td><td>" + QString::number(imginfo->size) + " bytes</td></tr>";
    reportstring += "<tr class='even vtop'><td>Sector Size:</td><td>" + QString::number(imginfo->sector_size) + " bytes</td></tr>";
    reportstring += "<tr class='odd vtop'><td>Volume (V" + QString::number(volcnt) + "):</td><td>" + volname + "</td></tr>";
    treenodemodel->AddNode(nodedata, QString("e" + QString::number(evidcnt)), -1, 0);
    mutex.unlock();
    // ADD ManualCarved Folder HERE
    treeout.clear();
    treeout << "$Manual Carved" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "Directory" << "Virtual Directory" << "0" << QString("e" + QString::number(evidcnt) + "-mc");
    nodedata.clear();
    for(int i=0; i < treeout.count(); i++)
        nodedata << treeout.at(i);
    mutex.lock();
    treenodemodel->AddNode(nodedata, QString("e" + QString::number(evidcnt)), 3, 0);
    mutex.unlock();
    const TSK_POOL_INFO* poolinfo = nullptr;
    TSK_IMG_INFO* curimginfo = NULL;
    TSK_FS_INFO* fsinfo = NULL;
    if(vsinfo != NULL)
        WriteVolumeProperties(vsinfo, volumepath);
    if(vsinfo == NULL) // no volume, so a single file system is all there is in the image
    {
        QString pooldesc = "";
        poolinfo = tsk_pool_open_img_sing(imginfo, 0, TSK_POOL_TYPE_APFS);
        if(poolinfo != nullptr) // contains a pool...
        {
            if(poolinfo->num_vols > 0)
            {
                for(int i=0; i < poolinfo->num_vols; i++)
                {
                    TSK_POOL_VOLUME_INFO curpoolvol = poolinfo->vol_list[i];
                    pooldesc = curpoolvol.desc;
                    curimginfo = poolinfo->get_img_info(poolinfo, curpoolvol.block);
                    if(curpoolvol.flags & TSK_POOL_VOLUME_FLAG_ENCRYPTED)
                    {
                        // CURRENTLY TSK DOESN'T WORK FOR ENCRYPTED APFS VOLUMES...
                        fsinfo = tsk_fs_open_img_decrypt(curimginfo, 0, TSK_FS_TYPE_APFS, passwordhash.value(evidname.split("/").last() + "-v" + QString::number(volcnt) + "-p" + QString::number(partint)).toStdString().c_str());
                        //fsinfo = tsk_fs_open_vol_decrypt(partinfo, TSK_FS_TYPE_APFS, "apfspassword");
                        //fsinfo = tsk_fs_open_pool_decrypt(poolinfo, curpoolvol.block, TSK_FS_TYPE_APFS_DETECT, "apfspassword");
                    }
                    else
                    {
                        fsinfo = tsk_fs_open_img(curimginfo, 0, TSK_FS_TYPE_APFS_DETECT);
                        //fsinfo = tsk_fs_open_pool(poolinfo, curpoolvol.block, TSK_FS_TYPE_APFS_DETECT);
                    }
                    if(fsinfo == NULL) // unrecognized filesystem
                    {
                        QString partitionpath = volumepath + "p" + QString::number(partint) + "/";
                        dir.mkpath(partitionpath);
                        QFile pfile(partitionpath + "stat");
                        pfile.open(QIODevice::Append | QIODevice::Text);
                        out.setDevice(&pfile);
                        out << "240," << QString::number(imginfo->size) << ",NON-RECOGNIZED FS,0,0," << (qint64)imginfo->size/volsectorsize << "," << volsectorsize << ",0,0,0,e" << QString::number(evidcnt) + "-v0-p" + QString::number(partint) + ",0";
                        out.flush();
                        pfile.close();
                        treeout.clear();
                        treeout << "NON-RECOGNIZED FS" << "0" << QString::number(imginfo->size) << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << QString("e" + QString::number(evidcnt) + "-v0-p" + QString::number(partint));
                        nodedata.clear();
                        for(int j=0; j < treeout.count(); j++)
                            nodedata << treeout.at(i);
                        mutex.lock();
                        treenodemodel->AddNode(nodedata, QString("e" + QString::number(evidcnt) + "-v0"), -1, 0);
                        mutex.unlock();
                        // ADD ManualCarved Folder HERE
                        treeout.clear();
                        treeout << QByteArray("$Carved-Verified").toBase64() << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "Directory" << "Virtual Directory" << "0" << QString("e" + QString::number(evidcnt) + "-v0-p" + QString::number(partint) + "-pc");
                        nodedata.clear();
                        for(int i=0; i < treeout.count(); i++)
                            nodedata << treeout.at(i);
                        mutex.lock();
                        treenodemodel->AddNode(nodedata, QString("e" + QString::number(evidcnt) + "-v0-p" + QString::number(partint)), 3, 0);
                        mutex.unlock();
			nodedata.clear();
			nodedata << QByteArray("$Carved-UnVerified").toBase64() << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "Directory" << "Virtual Directory" << "0" << QString("e" + QString::number(evidcnt) + "-v0-p" + QString::number(partint) + "-uc");
			mutex.lock();
			treenodemodel->AddNode(nodedata, QString("e" + QString::number(evidcnt) + "-v0-p" + QString::number(partint)), 3, 0);
			mutex.unlock();
                        reportstring += "<tr class='even vtop'><td>Partition (P" + QString::number(partint) + "):</td><td>NON RECOGNIZED FS</td></tr>";
                        partitionlist.append("e" + QString::number(evidcnt) + "-v0-p" + QString::number(partint) + ": NON-RECOGNIZED FS");
                    }
                    else
                    {
                        QString partitionpath = volumepath + "p" + QString::number(partint) + "/";
                        dir.mkpath(partitionpath);
                        QFile pfile(partitionpath + "stat");
                        pfile.open(QIODevice::Append | QIODevice::Text);
                        out.setDevice(&pfile);
                        out << fsinfo->ftype << "," << (qint64)fsinfo->block_size * (qint64)fsinfo->block_count << ",";
                        if(poolinfo == nullptr)
                            out << GetFileSystemLabel(fsinfo);
                        else
                            out << pooldesc;
                        out << "," << (qint64)fsinfo->root_inum << "," << (qint64)fsinfo->offset << "," << (qint64)fsinfo->block_count << "," << (int)fsinfo->block_size << ",0,0,0,e" << QString::number(evidcnt) + "-v" + QString::number(volcnt) + "-p" + QString::number(partint) << ",0";
                        out.flush();
                        pfile.close();
                        QString fsdesc = "";
                        if(poolinfo == nullptr)
                            fsdesc = GetFileSystemLabel(fsinfo);
                        else
                            fsdesc = pooldesc;
                        treeout.clear();
                        treeout << QString(fsdesc + " (" + QString(tsk_fs_type_toname(fsinfo->ftype)).toUpper() + ")") << "0" << QString::number(fsinfo->block_size * fsinfo->block_count) << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << QString("e" + QString::number(evidcnt) + "-v" + QString::number(volcnt) + "-p" + QString::number(partint)); // NAME IN FIRST COLUMN
                        nodedata.clear();
                        for(int j=0; j < treeout.count(); j++)
                            nodedata << treeout.at(j);
                        mutex.lock();
                        treenodemodel->AddNode(nodedata, QString("e" + QString::number(evidcnt) + "-v" + QString::number(volcnt)), -1, 0);
                        mutex.unlock();
                        reportstring += "<tr class='even vtop'><td>Partition (P0):</td><td>" + fsdesc + " (" + QString(tsk_fs_type_toname(fsinfo->ftype)).toUpper() + ")</td></tr>";
                        WriteFileSystemProperties(fsinfo, partitionpath);
                        TSK_STACK* stack = NULL;
                        stack = tsk_stack_create();
                        ProcessDir(fsinfo, stack, fsinfo->root_inum, "/", evidcnt, volcnt, partint, partitionpath);
                        tsk_fs_close(fsinfo);
                        tsk_stack_free(stack);
                        // ADD ManualCarved Folder HERE
                        treeout.clear();
                        treeout << QByteArray("$Carved-Verified").toBase64() << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "Directory" << "Virtual Directory" << "0" << QString("e" + QString::number(evidcnt) + "-v" + QString::number(volcnt) + "-p" + QString::number(partint) + "-pc");
                        nodedata.clear();
                        for(int i=0; i < treeout.count(); i++)
                            nodedata << treeout.at(i);
                        mutex.lock();
                        treenodemodel->AddNode(nodedata, QString("e" + QString::number(evidcnt) + "-v" + QString::number(volcnt) + "-p" + QString::number(partint)), 3, 0);
                        mutex.unlock();
			nodedata.clear();
			nodedata << QByteArray("$Carved-UnVerified").toBase64() << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "Directory" << "Virtual Directory" << "0" << QString("e" + QString::number(evidcnt) + "-v" + QString::number(volcnt) + "-p" + QString::number(partint) + "-uc");
			mutex.lock();
			treenodemodel->AddNode(nodedata, QString("e" + QString::number(evidcnt) + "-v" + QString::number(volcnt) + "-p" + QString::number(partint)), 3, 0);
			mutex.unlock();
                        partitionlist.append("e" + QString::number(evidcnt) + "-v" + QString::number(volcnt) + "-p" + QString::number(partint) + ": " + fsdesc + " (" + QString(tsk_fs_type_toname(fsinfo->ftype)).toUpper() + ")");
                    }
                    partint++;
                }
            }
        }
        else
        {
            fsinfo = tsk_fs_open_img(imginfo, 0, TSK_FS_TYPE_DETECT);
            if(fsinfo == NULL) // unrecognized filesystem
            {
                QString partitionpath = volumepath + "p0/";
                dir.mkpath(partitionpath);
                QFile pfile(partitionpath + "stat");
                pfile.open(QIODevice::Append | QIODevice::Text);
                out.setDevice(&pfile);
                out << "240," << QString::number(imginfo->size) << ",NON-RECOGNIZED FS,0,0," << (qint64)imginfo->size/volsectorsize << "," << volsectorsize << ",0,0,0,e" << QString::number(evidcnt) + "-v0-p0,0";
                out.flush();
                pfile.close();
                treeout.clear();
                treeout << "NON-RECOGNIZED FS" << "0" << QString::number(imginfo->size) << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << QString("e" + QString::number(evidcnt) + "-v0-p0");
                nodedata.clear();
                for(int i=0; i < treeout.count(); i++)
                    nodedata << treeout.at(i);
                mutex.lock();
                treenodemodel->AddNode(nodedata, QString("e" + QString::number(evidcnt) + "-v0"), -1, 0);
                mutex.unlock();
                // ADD ManualCarved Folder HERE
                treeout.clear();
                treeout << QByteArray("$Carved-Verified").toBase64() << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "Directory" << "Virtual Directory" << "0" << QString("e" + QString::number(evidcnt) + "-v0-p0-pc");
                nodedata.clear();
                for(int i=0; i < treeout.count(); i++)
                    nodedata << treeout.at(i);
                mutex.lock();
                treenodemodel->AddNode(nodedata, QString("e" + QString::number(evidcnt) + "-v0-p0"), 3, 0);
                mutex.unlock();
		nodedata.clear();
		nodedata << QByteArray("$Carved-UnVerified").toBase64() << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "Directory" << "Virtual Directory" << "0" << QString("e" + QString::number(evidcnt) + "-v0-p0-uc");
		mutex.lock();
		treenodemodel->AddNode(nodedata, QString("e" + QString::number(evidcnt) + "-v0-p0"), 3, 0);
		mutex.unlock();
                reportstring += "<tr class='even vtop'><td>Partition (P0):</td><td>NON RECOGNIZED FS</td></tr>";
	        partitionlist.append("e" + QString::number(evidcnt) + "-v0-p0: NON-RECOGNIZED FS");
            }
            else
            {
                QString partitionpath = volumepath + "p0/";
                dir.mkpath(partitionpath);
                QFile pfile(partitionpath + "stat");
                pfile.open(QIODevice::Append | QIODevice::Text);
                out.setDevice(&pfile);
                out << fsinfo->ftype << "," << (qint64)fsinfo->block_size * (qint64)fsinfo->block_count << ",";
                if(poolinfo == nullptr)
                    out << GetFileSystemLabel(fsinfo);
                else
                    out << pooldesc;
                out << "," << (qint64)fsinfo->root_inum << "," << (qint64)fsinfo->offset << "," << (qint64)fsinfo->block_count << "," << (int)fsinfo->block_size << ",0,0,0,e" << QString::number(evidcnt) + "-v" + QString::number(volcnt) + "-p" + QString::number(partint) << ",0";
                out.flush();
                pfile.close();
                QString fsdesc = "";
                if(poolinfo == nullptr)
                    fsdesc = GetFileSystemLabel(fsinfo);
                else
                    fsdesc = pooldesc;
                treeout.clear();
                treeout << QString(fsdesc + " (" + QString(tsk_fs_type_toname(fsinfo->ftype)).toUpper() + ")") << "0" << QString::number(fsinfo->block_size * fsinfo->block_count) << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << QString("e" + QString::number(evidcnt) + "-v" + QString::number(volcnt) + "-p" + QString::number(partint)); // NAME IN FIRST COLUMN
                nodedata.clear();
                for(int i=0; i < treeout.count(); i++)
                    nodedata << treeout.at(i);
                mutex.lock();
                treenodemodel->AddNode(nodedata, QString("e" + QString::number(evidcnt) + "-v" + QString::number(volcnt)), -1, 0);
                mutex.unlock();
                reportstring += "<tr class='even vtop'><td>Partition (P0):</td><td>" + fsdesc + " (" + QString(tsk_fs_type_toname(fsinfo->ftype)).toUpper() + ")</td></tr>";
                WriteFileSystemProperties(fsinfo, partitionpath);
                TSK_STACK* stack = NULL;
                stack = tsk_stack_create();
                ProcessDir(fsinfo, stack, fsinfo->root_inum, "/", evidcnt, volcnt, partint, partitionpath);
                tsk_fs_close(fsinfo);
                tsk_stack_free(stack);
                // ADD ManualCarved Folder HERE
                treeout.clear();
                treeout << QByteArray("$Carved-Verified").toBase64() << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "Directory" << "Virtual Directory" << "0" << QString("e" + QString::number(evidcnt) + "-v" + QString::number(volcnt) + "-p" + QString::number(partint) + "-pc");
                nodedata.clear();
                for(int i=0; i < treeout.count(); i++)
                    nodedata << treeout.at(i);
                mutex.lock();
                treenodemodel->AddNode(nodedata, QString("e" + QString::number(evidcnt) + "-v" + QString::number(volcnt) + "-p" + QString::number(partint)), 3, 0);
                mutex.unlock();
		nodedata.clear();
		nodedata << QByteArray("$Carved-UnVerified").toBase64() << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "Directory" << "Virtual Directory" << "0" << QString("e" + QString::number(evidcnt) + "-v" + QString::number(volcnt) + "-p" + QString::number(partint) + "-uc");
		mutex.lock();
		treenodemodel->AddNode(nodedata, QString("e" + QString::number(evidcnt) + "-v" + QString::number(volcnt) + "-p" + QString::number(partint)), 3, 0);
		mutex.unlock();
        	partitionlist.append("e" + QString::number(evidcnt) + "-v" + QString::number(volcnt) + "-p" + QString::number(partint) + ": " + fsdesc + " (" + QString(tsk_fs_type_toname(fsinfo->ftype)).toUpper() + ")");
            }
        }
    }
    else
    {
        QFile pfile;
        const TSK_VS_PART_INFO* partinfo = NULL;
        if(vsinfo->part_count > 0)
        {
            for(uint32_t i=0; i < vsinfo->part_count; i++)
            {
                partinfo = tsk_vs_part_get(vsinfo, i);
                if(partinfo == NULL)
                {
                    qDebug() << "tsk_vs_part_get error:";
                    tsk_error_print(stderr);
                }
                QString partitionpath = volumepath + "p" + QString::number(partint) + "/";
                dir.mkpath(partitionpath);
                pfile.setFileName(partitionpath + "stat");
                pfile.open(QIODevice::Append | QIODevice::Text);
                out.setDevice(&pfile);
                if(partinfo->flags == 0x02 || partinfo->flags == 0x04) // unallocated partition or meta entry
                {
                    out << "0," << (qint64)partinfo->len * vsinfo->block_size << "," << QString(partinfo->desc) << ",0," << (qint64)partinfo->start * (int)vsinfo->block_size << "," << (qint64)partinfo->len << "," << (int)vsinfo->block_size << "," << partinfo->flags << "," << (qint64)partinfo->len << "," << (int)vsinfo->block_size << ",e" + QString::number(evidcnt) + "-v" + QString::number(volcnt) + "-p" + QString::number(partint) << ",0";
                    out.flush();
                    pfile.close();
                    treeout.clear();
                    treeout << QString(partinfo->desc) << "0" << QString::number(partinfo->len * vsinfo->block_size) << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << QString("e" + QString::number(evidcnt) + "-v" + QString::number(volcnt) + "-p" + QString::number(partint)); // NAME IN FIRST COLUMN
                    nodedata.clear();
                    for(int j=0; j < treeout.count(); j++)
                        nodedata << treeout.at(j);
                    mutex.lock();
                    treenodemodel->AddNode(nodedata, QString("e" + QString::number(evidcnt) + "-v" + QString::number(volcnt)), -1, 0);
                    mutex.unlock();
                    reportstring += "<tr class='even vtop'><td>Partition (P" + QString::number(partint) + "):</td><td>" + QString(partinfo->desc) + "</td></tr>";
		    partitionlist.append("e" + QString::number(evidcnt) + "-v" + QString::number(volcnt) + "-p" + QString::number(partint) + ": " + QString(partinfo->desc));
                }
                else if(partinfo->flags == 0x01) // allocated partition
                {
                    QString pooldesc = "";
                    //qDebug() << "partinfo start:" << partinfo->start;
                    TSK_FS_INFO* fsinfo = NULL;
                    TSK_STACK* stack = NULL;
                    stack = tsk_stack_create();
                    const TSK_POOL_INFO* poolinfo = nullptr;
                    poolinfo = tsk_pool_open_sing(partinfo, TSK_POOL_TYPE_DETECT);
                    TSK_IMG_INFO* curimginfo = NULL;
		    //FILE* hfile = NULL;
		    //char* pstatcontent = NULL;
		    //size_t pstatsize;
		    //hfile = open_memstream(&pstatcontent, &pstatsize);
		    //poolinfo->poolstat(poolinfo, hfile);
		    //fclose(hfile);
		    // NOW I HAVE A CHAR* OF POOLSTAT OUTPUT. I NEED TO PARSE THIS AND STORE THAT IN THE POOL/VOL AND FS  PROPERTIES...
		    //qDebug() << "mem stream:" << pstatcontent;
		    //free(pstatcontent);
                    //poolinfo = nullptr;
                    poolinfo = tsk_pool_open_sing(partinfo, TSK_POOL_TYPE_DETECT);
                    if(poolinfo == nullptr)
                    {
                        fsinfo = tsk_fs_open_vol(partinfo, TSK_FS_TYPE_DETECT);
                        if(fsinfo != NULL)
                        {
                            out << fsinfo->ftype << "," << (qint64)fsinfo->block_size * (qint64)fsinfo->block_count << ",";
                            if(poolinfo == nullptr)
                                out << GetFileSystemLabel(fsinfo);
                            else
                                out << pooldesc;
                            out << "," << (qint64)fsinfo->root_inum << "," << (qint64)fsinfo->offset << "," << (qint64)fsinfo->block_count << "," << (int)fsinfo->block_size << "," << partinfo->flags << "," << (qint64)partinfo->len << "," << (int)fsinfo->dev_bsize << ",e" + QString::number(evidcnt) + "-v" + QString::number(volcnt) + "-p" + QString::number(partint) << ",0";
                            out.flush();
                            pfile.close();
                            treeout.clear();
                            QString fsdesc = "";
                            if(poolinfo == nullptr)
                                fsdesc = GetFileSystemLabel(fsinfo);
                            else
                                fsdesc = pooldesc;
                            treeout << QString(fsdesc + " (" + QString(tsk_fs_type_toname(fsinfo->ftype)).toUpper() + ")") << "0" << QString::number(fsinfo->block_size * fsinfo->block_count) << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << QString("e" + QString::number(evidcnt) + "-v" + QString::number(volcnt) + "-p" + QString::number(partint)); // NAME IN FIRST COLUMN
                            nodedata.clear();
                            for(int j=0; j < treeout.count(); j++)
                                nodedata << treeout.at(j);
                            mutex.lock();
                            treenodemodel->AddNode(nodedata, QString("e" + QString::number(evidcnt) + "-v" + QString::number(volcnt)), -1, 0);
                            mutex.unlock();
                            reportstring += "<tr class='even vtop'><td>Partition (P" + QString::number(partint) + "):</td><td>" + fsdesc + " (" + QString(tsk_fs_type_toname(fsinfo->ftype)).toUpper() + ")</td></tr>";
	    		    partitionlist.append("e" + QString::number(evidcnt) + "-v" + QString::number(volcnt) + "-p" + QString::number(partint) + ": " + fsdesc + " (" + QString(tsk_fs_type_toname(fsinfo->ftype)).toUpper() + ")");
                            WriteFileSystemProperties(fsinfo, partitionpath);
                            ProcessDir(fsinfo, stack, fsinfo->root_inum, "/", evidcnt, volcnt, partint, partitionpath);
                            // ADD ManualCarved Folder HERE
                            treeout.clear();
                            treeout << QByteArray("$Carved-Verified").toBase64() << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "Directory" << "Virtual Directory" << "0" << QString("e" + QString::number(evidcnt) + "-v" + QString::number(volcnt) + "-p" + QString::number(partint) + "-pc");
                            nodedata.clear();
                            for(int i=0; i < treeout.count(); i++)
                                nodedata << treeout.at(i);
                            mutex.lock();
                            treenodemodel->AddNode(nodedata, QString("e" + QString::number(evidcnt) + "-v" + QString::number(volcnt) + "-p" + QString::number(partint)), 3, 0);
                            mutex.unlock();
			    nodedata.clear();
			    nodedata << QByteArray("$Carved-UnVerified").toBase64() << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "Directory" << "Virtual Directory" << "0" << QString("e" + QString::number(evidcnt) + "-v" + QString::number(volcnt) + "-p" + QString::number(partint) + "-uc");
			    mutex.lock();
			    treenodemodel->AddNode(nodedata, QString("e" + QString::number(evidcnt) + "-v" + QString::number(volcnt) + "-p" + QString::number(partint)), 3, 0);
			    mutex.unlock();
                        }
                        else
                        {
                            out << "0," << (qint64)partinfo->len * (int)vsinfo->block_size << "," << QString(partinfo->desc) << " (Non-Recognized FS),0," << (qint64)partinfo->start * (int)vsinfo->block_size << "," << (qint64)partinfo->len << "," << (int)vsinfo->block_size << "," << partinfo->flags << "," << (qint64)partinfo->len << "," << (int)vsinfo->block_size << ",e" << QString::number(evidcnt) + "-v" + QString::number(volcnt) + "-p" + QString::number(partint) << ",0";
                            out.flush();
                            pfile.close();
                            treeout.clear();
                            treeout << QString(QString(partinfo->desc) + QString(" (Non-Recognized FS)")) << "0" << QString::number(partinfo->len * vsinfo->block_size) << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << QString("e" + QString::number(evidcnt) + "-v" + QString::number(volcnt) + "-p" + QString::number(partint)); // NAME IN FIRST COLUMN
                            nodedata.clear();
                            for(int j=0; j < treeout.count(); j++)
                                nodedata << treeout.at(j);
                            mutex.lock();
                            treenodemodel->AddNode(nodedata, QString("e" + QString::number(evidcnt) + "-v" + QString::number(volcnt)), -1, 0);
                            mutex.unlock();
                            // ADD ManualCarved Folder HERE
                            treeout.clear();
                            treeout << QByteArray("$Carved-Verified").toBase64() << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "Directory" << "Virtual Directory" << "0" << QString("e" + QString::number(evidcnt) + "-v" + QString::number(volcnt) + "-p" + QString::number(partint) + "-pc");
                            nodedata.clear();
                            for(int i=0; i < treeout.count(); i++)
                                nodedata << treeout.at(i);
                            mutex.lock();
                            treenodemodel->AddNode(nodedata, QString("e" + QString::number(evidcnt) + "-v" + QString::number(volcnt) + "-p" + QString::number(partint)), 3, 0);
                            mutex.unlock();
			    nodedata.clear();
			    nodedata << QByteArray("$Carved-UnVerified").toBase64() << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "Directory" << "Virtual Directory" << "0" << QString("e" + QString::number(evidcnt) + "-v" + QString::number(volcnt) + "-p" + QString::number(partint) + "-uc");
			    mutex.lock();
			    treenodemodel->AddNode(nodedata, QString("e" + QString::number(evidcnt) + "-v" + QString::number(volcnt) + "-p" + QString::number(partint)), 3, 0);
			    mutex.unlock();
                            reportstring += "<tr class='even vtop'><td>Partition (P" + QString::number(partint) + "):</td><td>" + QString(partinfo->desc) + " (NON-RECOGNIZED FS)</td></tr>";
    			    partitionlist.append("e" + QString::number(evidcnt) + "-v" + QString::number(volcnt) + "-p" + QString::number(partint) + ": " + QString(partinfo->desc) + "(NON-RECOGNIZED FS)");
                        }
                    }
                    else
                    {
                        if(poolinfo->num_vols > 0)
                        {
                            if(pfile.isOpen())
                                pfile.close();
                            for(int k=0; k < poolinfo->num_vols; k++)
                            {
				int pint = partint + k;
                                //int pint = j + k;
                                TSK_POOL_VOLUME_INFO curpoolvol = poolinfo->vol_list[k];
                                pooldesc = curpoolvol.desc;
                                curimginfo = poolinfo->get_img_info(poolinfo, curpoolvol.block);
                                if(curpoolvol.flags & TSK_POOL_VOLUME_FLAG_ENCRYPTED)
                                {
                                    // CURRENTLY TSK DOESN'T WORK FOR ENCRYPTED APFS VOLUMES...
                                    fsinfo = tsk_fs_open_img_decrypt(curimginfo, partinfo->start * curimginfo->sector_size, TSK_FS_TYPE_APFS, passwordhash.value(evidname.split("/").last() + "-v" + QString::number(volcnt) + "-p" + QString::number(pint)).toStdString().c_str());
                                    //fsinfo = tsk_fs_open_img_decrypt(curimginfo, 0, TSK_FS_TYPE_APFS, passwordhash.value(evidname + "-v" + QString::number(volcnt) + "-p" + QString::number(partint));
                                    //fsinfo = tsk_fs_open_vol_decrypt(partinfo, TSK_FS_TYPE_APFS, "apfspassword");
                                    //fsinfo = tsk_fs_open_pool_decrypt(poolinfo, curpoolvol.block, TSK_FS_TYPE_APFS_DETECT, "encrypted");
                                }
                                else
                                {
                                    fsinfo = tsk_fs_open_img(curimginfo, partinfo->start * curimginfo->sector_size, TSK_FS_TYPE_APFS_DETECT);
                                    //fsinfo = tsk_fs_open_pool(poolinfo, curpoolvol.block, TSK_FS_TYPE_APFS_DETECT);
                                }
                                QString partitionpath = volumepath + "p" + QString::number(pint) + "/";
                                dir.mkpath(partitionpath);
                                pfile.setFileName(partitionpath + "stat");
                                pfile.open(QIODevice::Append | QIODevice::Text);
                                out.setDevice(&pfile);
                                if(fsinfo != NULL)
                                {
                                    out << fsinfo->ftype << "," << (qint64)fsinfo->block_size * (qint64)fsinfo->block_count << ",";
                                    if(poolinfo == nullptr)
                                        out << GetFileSystemLabel(fsinfo);
                                    else
                                        out << pooldesc;
                                    out << "," << (qint64)fsinfo->root_inum << "," << (qint64)fsinfo->offset << "," << (qint64)fsinfo->block_count << "," << (int)fsinfo->block_size << "," << partinfo->flags << "," << (qint64)partinfo->len << "," << (int)fsinfo->dev_bsize << ",e" + QString::number(evidcnt) + "-v" + QString::number(volcnt) + "-p" + QString::number(pint) << ",0";
                                    out.flush();
                                    pfile.close();
                                    treeout.clear();
                                    QString fsdesc = "";
                                    if(poolinfo == nullptr)
                                        fsdesc = GetFileSystemLabel(fsinfo);
                                    else
                                        fsdesc = pooldesc;
                                    treeout << QString(fsdesc + " (" + QString(tsk_fs_type_toname(fsinfo->ftype)).toUpper() + ")") << "0" << QString::number(fsinfo->block_size * fsinfo->block_count) << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << QString("e" + QString::number(evidcnt) + "-v" + QString::number(volcnt) + "-p" + QString::number(pint)); // NAME IN FIRST COLUMN
                                    nodedata.clear();
                                    for(int j=0; j < treeout.count(); j++)
                                        nodedata << treeout.at(j);
                                    mutex.lock();
                                    treenodemodel->AddNode(nodedata, QString("e" + QString::number(evidcnt) + "-v" + QString::number(volcnt)), -1, 0);
                                    mutex.unlock();
                                    reportstring += "<tr class='even vtop'><td>Partition (P" + QString::number(pint) + "):</td><td>" + fsdesc + " (" + QString(tsk_fs_type_toname(fsinfo->ftype)).toUpper() + ")</td></tr>";
	                	    partitionlist.append("e" + QString::number(evidcnt) + "-v" + QString::number(volcnt) + "-p" + QString::number(pint) + ": " + fsdesc + " (" + QString(tsk_fs_type_toname(fsinfo->ftype)).toUpper() + ")");
                                    WriteFileSystemProperties(fsinfo, partitionpath);
                                    ProcessDir(fsinfo, stack, fsinfo->root_inum, "/", evidcnt, volcnt, pint, partitionpath);
                                    // ADD ManualCarved Folder HERE
                                    treeout.clear();
                                    treeout << QByteArray("$Carved-Verified").toBase64() << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "Directory" << "Virtual Directory" << "0" << QString("e" + QString::number(evidcnt) + "-v" + QString::number(volcnt) + "-p" + QString::number(partint) + "-pc");
                                    nodedata.clear();
                                    for(int i=0; i < treeout.count(); i++)
                                        nodedata << treeout.at(i);
                                    mutex.lock();
                                    treenodemodel->AddNode(nodedata, QString("e" + QString::number(evidcnt) + "-v" + QString::number(volcnt) + "-p" + QString::number(partint)), 3, 0);
                                    mutex.unlock();
				    nodedata.clear();
				    nodedata << QByteArray("$Carved-UnVerified").toBase64() << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "Directory" << "Virtual Directory" << "0" << QString("e" + QString::number(evidcnt) + "-v" + QString::number(volcnt) + "-p" + QString::number(partint) + "-uc");
				    mutex.lock();
				    treenodemodel->AddNode(nodedata, QString("e" + QString::number(evidcnt) + "-v" + QString::number(volcnt) + "-p" + QString::number(partint)), 3, 0);
				    mutex.unlock();

                                }
                                else
                                {
                                    out << "0," << (qint64)partinfo->len * (int)vsinfo->block_size << "," << QString(partinfo->desc) << " (Non-Recognized FS),0," << (qint64)partinfo->start * (int)vsinfo->block_size << "," << (qint64)partinfo->len << "," << (int)vsinfo->block_size << "," << partinfo->flags << "," << (qint64)partinfo->len << "," << (int)vsinfo->block_size << ",e" << QString::number(evidcnt) + "-v" + QString::number(volcnt) + "-p" + QString::number(partint) << ",0";
                                    out.flush();
                                    pfile.close();
                                    treeout.clear();
                                    treeout << QString(QString(partinfo->desc) + QString(" (Non-Recognized FS)")) << "0" << QString::number(partinfo->len * vsinfo->block_size) << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << QString("e" + QString::number(evidcnt) + "-v" + QString::number(volcnt) + "-p" + QString::number(partint)); // NAME IN FIRST COLUMN
                                    nodedata.clear();
                                    for(int j=0; j < treeout.count(); j++)
                                        nodedata << treeout.at(j);
                                    mutex.lock();
                                    treenodemodel->AddNode(nodedata, QString("e" + QString::number(evidcnt) + "-v" + QString::number(volcnt)), -1, 0);
                                    mutex.unlock();
                                    // ADD ManualCarved Folder HERE
                                    treeout.clear();
                                    treeout << QByteArray("$Carved-Verified").toBase64() << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "Directory" << "Virtual Directory" << "0" << QString("e" + QString::number(evidcnt) + "-v" + QString::number(volcnt) + "-p" + QString::number(partint) + "-pc");
                                    nodedata.clear();
                                    for(int i=0; i < treeout.count(); i++)
                                        nodedata << treeout.at(i);
                                    mutex.lock();
                                    treenodemodel->AddNode(nodedata, QString("e" + QString::number(evidcnt) + "-v" + QString::number(volcnt) + "-p" + QString::number(partint)), 3, 0);
                                    mutex.unlock();
                                    reportstring += "<tr class='even vtop'><td>Partition (P" + QString::number(partint) + "):</td><td>" + QString(partinfo->desc) + " (NON-RECOGNIZED FS)</td></tr>";
			            partitionlist.append("e" + QString::number(evidcnt) + "-v" + QString::number(volcnt) + "-p" + QString::number(partint) + ": " + QString(partinfo->desc) + "(NON-RECOGNIZED FS)");
                                }
                            }
                        }
                    }
                    tsk_stack_free(stack);
                    tsk_fs_close(fsinfo);
		    tsk_pool_close(poolinfo);
		    tsk_img_close(curimginfo);
                }
                else
                {
                    out << "0," << (qint64)partinfo->len * (int)vsinfo->block_size << "," << QString(partinfo->desc) << " (Non-Recognized FS),0," << (qint64)partinfo->start * (int)vsinfo->block_size << "," << (qint64)partinfo->len << "," << (int)vsinfo->block_size << "," << partinfo->flags << "," << (qint64)partinfo->len << "," << (int)vsinfo->block_size << ",e" << QString::number(evidcnt) + "-v" + QString::number(volcnt) + "-p" + QString::number(partint) << ",0";
                    out.flush();
                    pfile.close();
                    treeout.clear();
                    treeout << QString(QString(partinfo->desc) + QString(" (Non-Recognized FS)")) << "0" << QString::number(partinfo->len * vsinfo->block_size) << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << QString("e" + QString::number(evidcnt) + "-v" + QString::number(volcnt) + "-p" + QString::number(partint)); // NAME IN FIRST COLUMN
                    nodedata.clear();
                    for(int j=0; j < treeout.count(); j++)
                        nodedata << treeout.at(j);
                    mutex.lock();
                    treenodemodel->AddNode(nodedata, QString("e" + QString::number(evidcnt) + "-v" + QString::number(volcnt)), -1, 0);
                    reportstring += "<tr class='even vtop'><td>Partition (P" + QString::number(partint) + "):</td><td>" + QString(partinfo->desc) + " (NON-RECOGNIZED FS)</td></tr>";
                    mutex.unlock();
                    // ADD ManualCarved Folder HERE
                    treeout.clear();
                    treeout << QByteArray("$Carved-Verified").toBase64() << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "Directory" << "Virtual Directory" << "0" << QString("e" + QString::number(evidcnt) + "-v" + QString::number(volcnt) + "-p" + QString::number(partint) + "-pc");
                    nodedata.clear();
                    for(int i=0; i < treeout.count(); i++)
                        nodedata << treeout.at(i);
                    mutex.lock();
                    treenodemodel->AddNode(nodedata, QString("e" + QString::number(evidcnt) + "-v" + QString::number(volcnt) + "-p" + QString::number(partint)), 3, 0);
                    mutex.unlock();
		    nodedata.clear();
		    nodedata << QByteArray("$Carved-UnVerified").toBase64() << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "Directory" << "Virtual Directory" << "0" << QString("e" + QString::number(evidcnt) + "-v" + QString::number(volcnt) + "-p" + QString::number(partint) + "-uc");
		    mutex.lock();
		    treenodemodel->AddNode(nodedata, QString("e" + QString::number(evidcnt) + "-v" + QString::number(volcnt) + "-p" + QString::number(partint)), 3, 0);
		    mutex.unlock();
		    partitionlist.append("e" + QString::number(evidcnt) + "-v" + QString::number(volcnt) + "-p" + QString::number(partint) + ": " + QString(partinfo->desc) + " (NON-RECOGNIZED FS)]");
                }
                partint++;
            }
        }
        partinfo = NULL;
    }
    mutex.lock();
    reportstring += "</table></div><br/>\n";
    EvidenceReportData tmpdata;
    tmpdata.evidid = evidcnt;
    tmpdata.evidname = evidencename;
    tmpdata.evidcontent = reportstring;
    evidrepdatalist.append(tmpdata);
    mutex.unlock();
    tsk_vs_close(vsinfo);
    tsk_img_close(imginfo);
}

void ProcessDir(TSK_FS_INFO* fsinfo, TSK_STACK* stack, TSK_INUM_T dirinum, const char* path, int eint, int vint, int pint, QString partpath)
{
    TSK_FS_DIR* fsdir = NULL;
    fsdir = tsk_fs_dir_open_meta(fsinfo, dirinum);
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
		int charsize = 1024;
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
                    parentstr = "e" + QString::number(eint) + "-v" + QString::number(vint) + "-p" + QString::number(pint);
                else
                    parentstr = "e" + QString::number(eint) + "-v" + QString::number(vint) + "-p" + QString::number(pint) + "-f" + QString::number(fsfile->name->par_addr);
                if(fsfile->meta != NULL)
		{
		    if(fsfile->meta->size < 1024)
			charsize = fsfile->meta->size;
                    treeout << QString::number(fsfile->meta->size) << QString::number(fsfile->meta->crtime) << QString::number(fsfile->meta->mtime) << QString::number(fsfile->meta->atime) << QString::number(fsfile->meta->ctime); // SIZE, 4-DATES - 2, 3, 4, 5, 6
		}
                else
                {
                    treeout << "0" << "0" << "0" << "0" << "0"; // SIZE, 4-DATES - 2, 3, 4, 5, 6
                }
                treeout << "0"; // HASH - 7
		//qDebug() << "fsfile->name->type:" << pint << fsfile->name->name << fsfile->name->meta_addr << fsfile->name->type;
                if(fsfile->name->type == TSK_FS_NAME_TYPE_DIR) // DIRECTORY
                    treeout << "Directory" << "Directory"; // CAT/SIG - 8/9
                else if(fsfile->name->type == TSK_FS_NAME_TYPE_VIRT_DIR || QString(fsfile->name->name).contains("$OrphanFiles")) // VIRTUAL DIRECTORY
                    treeout << "Directory" << "Virtual Directory"; // CAT/SIG - 8,9
                else
                {
                    char* magicbuffer = new char[0];
                    magicbuffer = new char[charsize];
                    QByteArray tmparray("intro");
                    tmparray.clear();
		    mutex.lock();
                    tsk_fs_file_read(fsfile, 0, magicbuffer, charsize, TSK_FS_FILE_READ_FLAG_NONE);
		    mutex.unlock();
                    tmparray = QByteArray::fromRawData(magicbuffer, charsize);
                    QMimeDatabase mimedb;
                    //const QMimeType mimetype = mimedb.mimeTypeForData(tmparray);
                    const QMimeType mimetype = mimedb.mimeTypeForFileNameAndData(QString::fromStdString(fsfile->name->name), tmparray);
                    QString mimestr = GenerateCategorySignature(mimetype);
                    if(mimestr.contains("Unknown")) // generate further analysis
                    {
                        if(tmparray.at(0) == '\x4c' && tmparray.at(1) == '\x00' && tmparray.at(2) == '\x00' && tmparray.at(3) == '\x00' && tmparray.at(4) == '\x01' && tmparray.at(5) == '\x14' && tmparray.at(6) == '\x02' && tmparray.at(7) == '\x00') // LNK File
                            mimestr = "Windows System/Shortcut";
                        else if(strcmp(fsfile->name->name, "INFO2") == 0 && (tmparray.at(0) == 0x04 || tmparray.at(0) == 0x05))
                            mimestr = "Windows System/Recycler";
                        else if(QString::fromStdString(fsfile->name->name).startsWith("$I") && (tmparray.at(0) == 0x01 || tmparray.at(0) == 0x02))
                            mimestr = "Windows System/Recycle.Bin";
			else if(QString::fromStdString(fsfile->name->name).endsWith(".pf") && tmparray.at(4) == 0x53 && tmparray.at(5) == 0x43 && tmparray.at(6) == 0x43 && tmparray.at(7) == 0x41)
			    mimestr = "Windows System/Prefetch";
			else if(QString::fromStdString(fsfile->name->name).endsWith(".pf") && tmparray.at(0) == 0x4d && tmparray.at(1) == 0x41 && tmparray.at(2) == 0x4d)
			    mimestr = "Windows System/Prefetch";
                        else if(tmparray.at(0) == '\x72' && tmparray.at(1) == '\x65' && tmparray.at(2) == '\x67' && tmparray.at(3) == '\x66') // 72 65 67 66 | regf
                            mimestr = "Windows System/Registry";
                    }
                    //qDebug() << "mimestr:" << mimestr;
                    delete[] magicbuffer;
                    treeout << mimestr.split("/").at(0) << mimestr.split("/").at(1); // CAT/SIG - 8, 9
                }
                treeout << "0"; // TAG - 10
                // PUT ID INFO HERE FOR NAME IN FIRST COLUMN
                if(fsfile->name->meta_addr == 0 && strcmp(fsfile->name->name, "$MFT") != 0)
                    treeout << "e" + QString::number(eint) + "-v" + QString::number(vint) + "-p" + QString::number(pint) + "-fo" + QString::number(orphancount) + "-a" + QString::number(fsfile->name->par_addr); // Orphan ID - 11
                else if(fsfile->meta == NULL)
                    treeout << "e" + QString::number(eint) + "-v" + QString::number(vint) + "-p" + QString::number(pint) + "-fd" + QString::number(fsfile->name->meta_addr) + "-a" + QString::number(fsfile->name->par_addr); // Deleted Recovered ID - 11
                else
                    treeout << "e" + QString::number(eint) + "-v" + QString::number(vint) + "-p" + QString::number(pint) + "-f" + QString::number(fsfile->name->meta_addr) + "-a" + QString::number(fsfile->name->par_addr); // ID - 11
                //if(fsfile->meta != NULL)
                //    treeout << QString::number(fsfile->meta->type); // file type - 12
                //else
                if(QString(fsfile->name->name).contains("$OrphanFiles"))
                    treeout << "11"; // file type - 12
                else
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
                //if(QString(fsfile->name->name).contains("$OrphanFiles"))
                    //qDebug() << treeout;
                QList<QVariant> nodedata;
                nodedata.clear();
                for(int i=0; i < 12; i++)
                    nodedata << treeout.at(i);
                mutex.lock();
                treenodemodel->AddNode(nodedata, parentstr, treeout.at(12).toInt(), treeout.at(13).toInt());
                mutex.unlock();
                listeditems.append(treeout.at(11));
                filesfound++;
		//qDebug() << "begin write properties...";
                WriteFileProperties(fsfile, partpath);
		//qDebug() << "end write properties...";
                isignals->ProgUpd();
                if(fsfile->fs_info->ftype == TSK_FS_TYPE_NTFS_DETECT)
                {
                    char* startoffset = new char[0];
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
                        const TSK_FS_ATTR* fsattr = tsk_fs_file_attr_get_idx(fsfile, i);
                        // NEED TO IMPLEMENT THE $DATA ATTRIBUTE: RESIDENT/NON RESIDENT value for the properties file
                        adssize += 24;
                        adssize += (qint64)fsattr->size;
                        if(fsattr->type == 128 && fsattr->name != NULL)
                        {
                            bool isresident = false;
                            if(fsattr->flags & TSK_FS_ATTR_RES)
                            isresident = true;
                            char* fbuf = new char[fsattr->size];
                            ssize_t flen = tsk_fs_attr_read(fsattr, 0, fbuf, fsattr->size, TSK_FS_FILE_READ_FLAG_NONE);
                            QByteArray fdata = QByteArray::fromRawData(fbuf, flen);
                            QMimeDatabase adsmimedb;
                            QMimeType adsmimetype = adsmimedb.mimeTypeForData(fdata);
                            QString mimestr = GenerateCategorySignature(adsmimetype);
                            if(mimestr.contains("Unknown")) // generate further analysis
                                mimestr = "Windows System/Alternate Data Stream";
                            delete[] fbuf;
                            adsba.append(QString(QString(fsfile->name->name) + QString(":") + QString(fsattr->name)).toUtf8());
                            treeout.clear();
                            treeout << adsba.toBase64() << ba.toBase64() << QString::number(fsattr->size) << "0" << "0" << "0" << "0" << "0" << mimestr.split("/").at(0) << mimestr.split("/").at(1) << "0" << QString("e" + QString::number(eint) + "-v" + QString::number(vint) + "-p" + QString::number(pint) + "-fa" + QString::number(fsattr->id) + "-a" + QString::number(fsfile->name->meta_addr)) << "10" << "0"; // NAME IN FIRST COLUMN
                            nodedata.clear();
                            for(int i=0;  i < 12; i++)
                                nodedata << treeout.at(i);
                            mutex.lock();
                            treenodemodel->AddNode(nodedata, QString("e" + QString::number(eint) + "-v" + QString::number(vint) + "-p" + QString::number(pint) + "-f" + QString::number(fsfile->name->meta_addr)), treeout.at(12).toInt(), treeout.at(13).toInt());
                            mutex.unlock();
                            listeditems.append(treeout.at(11)); // UNCOMMENT ONCE I CAN CAPTURE ADS IN GEN HASH AND IMG THUMB
                            filesfound++;
                            isignals->ProgUpd();
                            WriteAlternateDataStreamProperties(fsfile, QString(fsfile->name->name) + QString(":") + QString(fsattr->name), QString::number(fsattr->id), partpath, isresident);
                        }
                    }
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
                                //path2 = std::string(path) + "/" + std::string(fsfile->name->name);
                                ProcessDir(fsinfo, stack, fsfile->name->meta_addr, path2.c_str(), eint, vint, pint, partpath);
                                tsk_stack_pop(stack);
                            }
                        }
                    }
                }
            }
            // FIX THIS FUNCTION TOO..
            if(fsfile->name->meta_addr == 0 && strcmp(fsfile->name->name, "$MFT") != 0)
                orphancount++;
            tsk_fs_file_close(fsfile);
        }
    }
    tsk_fs_dir_close(fsdir);
}

