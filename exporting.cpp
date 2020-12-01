#include "exporting.h"

// Copyright 2013-2020 Pasquale J. Rinaldi, Jr.
// Distrubted under the terms of the GNU General Public License version 2

void ProcessExport(QString objectid)
{
    // STILL NEED TO TAKE CARE OF CARVED AND ZIP CONTENTS
    TreeNode* curitem = NULL;
    QDir eviddir = QDir(wombatvariable.tmpmntpath);
    QStringList evidfiles = eviddir.entryList(QStringList("*-" + objectid.split("-").at(0)), QDir::NoSymLinks | QDir::Dirs);
    QString evidencename = evidfiles.at(0).split("-e").first();
    QString tmpstr = "";
    QFile estatfile(wombatvariable.tmpmntpath + evidfiles.at(0) + "/stat");
    if(!estatfile.isOpen())
        estatfile.open(QIODevice::ReadOnly | QIODevice::Text);
    if(estatfile.isOpen())
    {
        tmpstr = estatfile.readLine();
        estatfile.close();
    }
    QByteArray filecontent;
    filecontent.clear();
    QString layout = "";
    QModelIndexList indxlist = treenodemodel->match(treenodemodel->index(0, 11, QModelIndex()), Qt::DisplayRole, QVariant(objectid), -1, Qt::MatchFlags(Qt::MatchExactly | Qt::MatchRecursive));
    curitem = static_cast<TreeNode*>(indxlist.first().internalPointer());
    if(objectid.contains("-z"))
    {
        int err = 0;
        RewriteSelectedIdContent(indxlist.first().parent()); // writes aprent content to use to load zip content
        QString fnamestr = wombatvariable.tmpfilepath + objectid.split("-z").at(0) + "-fhex";
        /*
         *        int err = 0;
        RewriteSelectedIdContent(curindex.parent()); // writes parent content to use to load zip content.
        QString fnamestr = wombatvariable.tmpfilepath + curid.split("-z").at(0) + "-fhex";
        zip* curzip = zip_open(fnamestr.toStdString().c_str(), ZIP_RDONLY, &err);
        struct zip_stat zipstat;
        zip_stat_init(&zipstat);
        int zipid = curid.split("-z").at(1).toInt();
        zip_stat_index(curzip, zipid, 0, &zipstat);
        char* zipbuf = new char[zipstat.size];
        zip_file_t* curfile = NULL;
        if(zipstat.encryption_method == ZIP_EM_NONE)
            curfile = zip_fopen_index(curzip, zipid, 0);
        if(curfile != NULL)
        {
            zip_fread(curfile, zipbuf, zipstat.size);
            zip_fclose(curfile);
        }
        QFile ztmp(wombatvariable.tmpfilepath + curid + "-fhex");
        if(!ztmp.isOpen())
            ztmp.open(QIODevice::WriteOnly);
        if(ztmp.isOpen())
        {
            QDataStream zbuffer(&ztmp);
            zbuffer.writeRawData(zipbuf, zipstat.size);
            ztmp.close();
        }
        delete[] zipbuf;
        hexstring = wombatvariable.tmpfilepath + curid + "-fhex";

         */ 
    }
    else if(objectid.contains("-c"))
    {
        QFile cfile(wombatvariable.tmpmntpath + "carved/" + objectid + ".prop");
        if(!cfile.isOpen())
            cfile.open(QIODevice::ReadOnly | QIODevice::Text);
        if(cfile.isOpen())
        {
            layout = cfile.readLine();
            cfile.close();
        }
    }
    else if(objectid.split("-").count() == 3)
    {
        QFile fpropfile(wombatvariable.tmpmntpath + evidfiles.at(0) + "/" + objectid.split("-").at(1) + "/" + objectid.split("-").at(2) + ".prop");
        if(!fpropfile.isOpen())
            fpropfile.open(QIODevice::ReadOnly | QIODevice::Text);
        if(fpropfile.isOpen())
        {
            QString line = "";
            while(!fpropfile.atEnd())
            {
                line = fpropfile.readLine();
                if(line.startsWith("Layout"))
                {
                    layout = line.split("|").at(1);
                    break;
                }
            }
            fpropfile.close();
        }
    }
    QStringList layoutlist = layout.split(";", Qt::SkipEmptyParts);
    QFile efile(tmpstr.split(",", Qt::SkipEmptyParts).at(1));
    if(!efile.isOpen())
        efile.open(QIODevice::ReadOnly);
    if(efile.isOpen())
    {
        for(int i=0; i < layoutlist.count(); i++)
        {
            efile.seek(layoutlist.at(i).split(",", Qt::SkipEmptyParts).at(0).toULongLong());
            filecontent.append(efile.read(layoutlist.at(i).split(",", Qt::SkipEmptyParts).at(1).toULongLong()));
        }
        efile.close();
    }
    QString tmppath = "";
    QString tmpname = indxlist.first().sibling(indxlist.first().row(), 0).data().toString();
    if(originalpath == true)
    {
        tmppath = exportpath + indxlist.first().sibling(indxlist.first().row(), 1).data().toString();
    }
    else
        tmppath = exportpath + "/";
    if(curitem->itemtype == 3 || curitem->itemtype == 11) // directory
    {
        QDir dir;
        bool tmpdir = dir.mkpath(QString(tmppath + tmpname));
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
        if(tmpdir == true)
        {
            QFile tmpfile(tmppath + tmpname);
            if(tmpfile.open(QIODevice::WriteOnly))
            {
                tmpfile.write(filecontent);
                tmpfile.close();
            }
        }
    }
    /*
    if(objectid.contains("-c"))
    {
        QString estring = objectid.split("-").at(0);
        QDir eviddir = QDir(wombatvariable.tmpmntpath);
        QStringList evidfiles = eviddir.entryList(QStringList(QString("*." + estring)), QDir::NoSymLinks | QDir::Dirs);
        QFile evidfile(wombatvariable.tmpmntpath + evidfiles.first() + "/stat");
        QStringList evidlist;
        evidlist.clear();
        if(!evidfile.isOpen())
            evidfile.open(QIODevice::ReadOnly | QIODevice::Text);
        if(evidfile.isOpen())
        {
            evidlist = QString(evidfile.readLine()).split(",", Qt::SkipEmptyParts);
            evidfile.close();
        }
        QString rawevidencepath = wombatvariable.imgdatapath;
        if(TSK_IMG_TYPE_ISAFF((TSK_IMG_TYPE_ENUM)evidlist.at(0).toInt()))
            rawevidencepath += evidlist.at(3).split("/").last() + ".raw";
        else if(TSK_IMG_TYPE_ISEWF((TSK_IMG_TYPE_ENUM)evidlist.at(0).toInt()))
            rawevidencepath += evidlist.at(3).split("/").last() + "/ewf1";
        else if(TSK_IMG_TYPE_ISRAW((TSK_IMG_TYPE_ENUM)evidlist.at(0).toInt()))
        {
            QString imgext = evidlist.at(3).split("/").last().split("/").last();
            if(imgext.contains("000"))
                rawevidencepath += evidlist.at(3).split("/").last() + ".raw";
            else
                rawevidencepath = evidlist.at(3);
        }
        QString tmpstr;
        QFile cfile(wombatvariable.tmpmntpath + "carved/" + objectid + ".stat");
        if(!cfile.isOpen())
            cfile.open(QIODevice::ReadOnly | QIODevice::Text);
        if(cfile.isOpen())
            tmpstr = cfile.readLine();
        cfile.close();
        QByteArray carvedcontent;
        carvedcontent.clear();
        QFile rawfile(rawevidencepath);
        if(!rawfile.isOpen())
            rawfile.open(QIODevice::ReadOnly);
        if(rawfile.isOpen())
        {
            bool isseek = rawfile.seek(tmpstr.split(",").at(16).toLongLong());
            if(isseek)
                carvedcontent = rawfile.read(tmpstr.split(",").at(8).toLongLong());
            rawfile.close();
        }
        QString tmppath = exportpath + "/"; // no original path for carved files.
        QString tmpname = QByteArray::fromBase64(QByteArray(tmpstr.split(",").at(0).toStdString().c_str()));
        QDir dir;
        bool tmpdir = dir.mkpath(QDir::cleanPath(tmppath));
        if(tmpdir)
        {
            QFile tmpfile(tmppath + tmpname);
            if(!tmpfile.isOpen())
                tmpfile.open(QIODevice::WriteOnly);
            if(tmpfile.isOpen())
                tmpfile.write(carvedcontent);
            tmpfile.close();
        }
    }
    else
    {
        QString zipid = "";
        if(objectid.contains("z")) // exporting a child of a zip file
        {
            QModelIndexList indxlist = treenodemodel->match(treenodemodel->index(0, 11, QModelIndex()), Qt::DisplayRole, QVariant(objectid), -1, Qt::MatchFlags(Qt::MatchExactly | Qt::MatchRecursive));
            zipid = objectid;
            objectid = indxlist.first().parent().sibling(indxlist.first().parent().row(), 11).data().toString();
        }
        TSK_IMG_INFO* imginfo = NULL;
        std::vector<std::string> pathvector;
        pathvector.clear();
        QDir eviddir = QDir(wombatvariable.tmpmntpath);
        QString tmpstr = "";
        QStringList evidfiles = eviddir.entryList(QStringList("*." + objectid.split("-").at(0)), QDir::NoSymLinks | QDir::Dirs);
        QString evidencename = evidfiles.at(0).split(".e").first();
        QString estring = objectid.split("-", Qt::SkipEmptyParts).at(0);
        QString vstring = objectid.split("-", Qt::SkipEmptyParts).at(1);
        QString pstring = objectid.split("-", Qt::SkipEmptyParts).at(2);
        QString fstring = objectid.split("-", Qt::SkipEmptyParts).at(3);
        QString astring = objectid.split("-", Qt::SkipEmptyParts).at(4);
        QFile evidfile(wombatvariable.tmpmntpath + evidencename + "." + estring + "/stat");
        if(!evidfile.isOpen())
            evidfile.open(QIODevice::ReadOnly | QIODevice::Text);
        if(evidfile.isOpen())
            tmpstr = evidfile.readLine();
        evidfile.close();
        pathvector.push_back(tmpstr.split(",").at(3).split("|").at(0).toStdString());
        const TSK_TCHAR** images;
        images = (const char**)malloc(pathvector.size()*sizeof(char*));
        images[0] = pathvector[0].c_str();
        int partcount = tmpstr.split(",").at(3).split("|").size();
        imginfo = tsk_img_open(partcount, images, TSK_IMG_TYPE_DETECT, 0);
        if(imginfo == NULL)
        {
            qDebug() << tsk_error_get_errstr();
            //LogMessage("Image opening error");
        }
        tmpstr = "";
        QStringList partlist;
        partlist.clear();
        QFile partfile(wombatvariable.tmpmntpath + evidencename + "." + estring + "/" + vstring + "/" + pstring + "/stat");
        if(!partfile.isOpen())
            partfile.open(QIODevice::ReadOnly | QIODevice::Text);
        if(partfile.isOpen())
            tmpstr = partfile.readLine();
        partlist = tmpstr.split(",");
        tmpstr = "";
        TSK_FS_INFO* fsinfo = NULL;
        fsinfo = tsk_fs_open_img(imginfo, partlist.at(4).toULongLong(), TSK_FS_TYPE_DETECT);
        qint64 curaddr = 0;
        if(fstring.contains("a"))
            curaddr = astring.mid(1).toLongLong();
        else if(fstring.contains("d") || fstring.contains("o") || fstring.contains("z"))
            curaddr = fstring.mid(2).toLongLong();
        else
            curaddr = fstring.mid(1).toLongLong();
        char* filebuffer = new char[0];
        ssize_t bufferlength = 0;
        TSK_FS_FILE* fsfile = NULL;
        fsfile = tsk_fs_file_open_meta(fsinfo, NULL, curaddr);
        if(partlist.at(0).toInt() == TSK_FS_TYPE_NTFS_DETECT) // IF NTFS
        {
            if(fstring.contains("a")) // IF ADS
            {
                int attrid = fstring.mid(2).toInt();
                const TSK_FS_ATTR* fsattr = tsk_fs_file_attr_get_id(fsfile, attrid);
                filebuffer = new char[fsattr->size];
                bufferlength = tsk_fs_file_read_type(fsfile, TSK_FS_ATTR_TYPE_NTFS_DATA, attrid, 0, filebuffer, fsattr->size, TSK_FS_FILE_READ_FLAG_SLACK);
                fsattr = NULL;
                
            }
            else // IF NOT ADS
            {
                filebuffer = new char[fsfile->meta->size];
                bufferlength = tsk_fs_file_read(fsfile, 0, filebuffer, fsfile->meta->size, TSK_FS_FILE_READ_FLAG_SLACK);
            }
        }
        else
        {
            filebuffer = new char[fsfile->meta->size];
            bufferlength = tsk_fs_file_read(fsfile, 0, filebuffer, fsfile->meta->size, TSK_FS_FILE_READ_FLAG_SLACK);
        }
        tsk_fs_file_close(fsfile);
        fsfile = NULL;
        tsk_fs_close(fsinfo);
        fsinfo = NULL;
        tsk_img_close(imginfo);
        imginfo = NULL;
        QString tmppath = "";
        QString tmpname = "";
        TreeNode* curitem = NULL;
        if(zipid.contains("z"))
        {
            QString parstr = wombatvariable.tmpfilepath + objectid + "-fhex";
            QFile parfile(parstr);
            if(!parfile.isOpen())
                parfile.open(QIODevice::WriteOnly);
            if(parfile.isOpen())
            {
                QDataStream outbuf(&parfile);
                outbuf.writeRawData(filebuffer, bufferlength);
                parfile.close();
            }
            int err = 0;
            zip* zfile = zip_open(parstr.toStdString().c_str(), ZIP_RDONLY, &err);
            struct zip_stat zstat;
            zip_stat_init(&zstat);
            zip_stat_index(zfile, zipid.split("-").at(3).mid(2).toLongLong(), 0, &zstat);
            zip_file_t* curfile = NULL;
            if(zstat.encryption_method == ZIP_EM_NONE)
                curfile = zip_fopen_index(zfile, zipid.split("-").at(3).mid(2).toLongLong(), 0); // IF NOT ENCRYPTED
            else
            {
                // PROMPT USER FOR PASSWORD HERE....
                curfile = zip_fopen_index_encrypted(zfile, zipid.split("-").at(3).mid(2).toLongLong(), 0, "password"); // IF ENCRYPTED (PROMPT USER FOR PASSWORD)...
            }
            if(curfile != NULL)
            {
                filebuffer = new char[zstat.size];
                bufferlength = zip_fread(curfile, filebuffer, zstat.size);
                zip_fclose(curfile);
            }
            zip_close(zfile);
            QModelIndexList indxlist = treenodemodel->match(treenodemodel->index(0, 11, QModelIndex()), Qt::DisplayRole, QVariant(zipid), -1, Qt::MatchFlags(Qt::MatchExactly | Qt::MatchRecursive));
            curitem = static_cast<TreeNode*>(indxlist.first().internalPointer());
            tmppath = "";
            tmpname = indxlist.first().sibling(indxlist.first().row(), 0).data().toString().split("/").last();
            if(originalpath == true)
                tmppath = exportpath + indxlist.first().sibling(indxlist.first().row(), 1).data().toString();
            else
                tmppath = exportpath + "/";
        }
        else
        {
            QModelIndexList indxlist = treenodemodel->match(treenodemodel->index(0, 11, QModelIndex()), Qt::DisplayRole, QVariant(objectid), -1, Qt::MatchFlags(Qt::MatchExactly | Qt::MatchRecursive));
            curitem = static_cast<TreeNode*>(indxlist.first().internalPointer());
            tmppath = "";
            tmpname = indxlist.first().sibling(indxlist.first().row(), 0).data().toString();
            if(originalpath == true)
            {
                tmppath = exportpath + indxlist.first().sibling(indxlist.first().row(), 1).data().toString();
            }
            else
                tmppath = exportpath + "/";
        }
        if(curitem->itemtype == 3 || curitem->itemtype == 11) // directory
        {
            QDir dir;
            bool tmpdir = dir.mkpath(QString(tmppath + tmpname));
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
            if(tmpdir == true)
            {
                QFile tmpfile(tmppath + tmpname);
                if(tmpfile.open(QIODevice::WriteOnly))
                {
                    QDataStream outbuffer(&tmpfile);
                    outbuffer.writeRawData(filebuffer, bufferlength);
                    if(tmpfile.isOpen())
                        tmpfile.close();
                }
            }
        }
        delete filebuffer;
    }
    */
    exportcount++;
    isignals->ExportUpd();
}
