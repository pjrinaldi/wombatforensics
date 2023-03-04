#include "exporting.h"

// Copyright 2013-2020 Pasquale J. Rinaldi, Jr.
// Distrubted under the terms of the GNU General Public License version 2

void ProcessExport(QString objectid)
{
    // STILL NEED TO TAKE CARE OF CARVED AND ZIP CONTENTS
    TreeNode* curitem = NULL;
    QDir eviddir = QDir(wombatvariable.tmpmntpath);
    QStringList evidfiles = eviddir.entryList(QStringList("*-" + objectid.split("-").at(0)), QDir::NoSymLinks | QDir::Dirs);
    /*
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
    */
    QByteArray filecontent;
    filecontent.clear();
    QString layout = "";
    QModelIndexList indxlist = treenodemodel->match(treenodemodel->index(0, treenodemodel->GetColumnIndex("id"), QModelIndex()), Qt::DisplayRole, QVariant(objectid), -1, Qt::MatchFlags(Qt::MatchExactly | Qt::MatchRecursive));
    curitem = static_cast<TreeNode*>(indxlist.first().internalPointer());
    if(objectid.contains("-z"))
    {
        int err = 0;
        RewriteSelectedIdContent(indxlist.first().parent()); // writes aprent content to use to load zip content
        QString fnamestr = wombatvariable.tmpfilepath + objectid.split("-z").at(0) + "-fhex";
        zip* curzip = zip_open(fnamestr.toStdString().c_str(), ZIP_RDONLY, &err);
        struct zip_stat zipstat;
        zip_stat_init(&zipstat);
        int zipid = objectid.split("-z").at(1).toInt();
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
        filecontent.append(zipbuf, zipstat.size);
        delete[] zipbuf;
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
    if(!objectid.contains("-z"))
    {
        QStringList layoutlist = layout.split(";", Qt::SkipEmptyParts);
        ForImg* curimg = NULL;
        for(int i=0; i < existingforimglist.count(); i++)
        {
            curimg = existingforimglist.at(i);
            if(curimg->MountPath().endsWith(objectid.split("-").at(0)))
            {
                break;
                //qDebug() << curimg->ImgPath() << curimg->MountPath();
            }
        }
        if(curimg != NULL)
        {
            //QString tmpstr = "";
            for(int i=0; i < layoutlist.count(); i++)
            {
                filecontent.append(curimg->ReadContent(layoutlist.at(i).split(",", Qt::SkipEmptyParts).at(0).toULongLong(), layoutlist.at(i).split(",", Qt::SkipEmptyParts).at(1).toULongLong()));
            }
        }

        /*
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
        */
    }
    QString tmppath = "";
    QString tmpname = "";
    if(objectid.contains("-z"))
        tmpname = indxlist.first().sibling(indxlist.first().row(), 0).data().toString().split("/").last();
    else
        tmpname = indxlist.first().sibling(indxlist.first().row(), 0).data().toString();
    if(originalpath == true)
    {
        if(objectid.contains("-z"))
            tmppath = exportpath + indxlist.first().sibling(indxlist.first().row(), 1).data().toString() + indxlist.first().sibling(indxlist.first().row(), 0).data().toString().split("/").first() + "/";
        else
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
    exportcount++;
    isignals->ExportUpd();
}
