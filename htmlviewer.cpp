#include "htmlviewer.h"

// Copyright 2015 Pasquale J. Rinaldi, Jr.
// Distrubted under the terms of the GNU General Public License version 2

HtmlViewer::HtmlViewer(QWidget* parent) : QDialog(parent), ui(new Ui::HtmlViewer)
{
    ui->setupUi(this);
    tskptr = &tskobj;
    tskptr->readimginfo = NULL;
    tskptr->readfsinfo = NULL;
    tskptr->readfileinfo = NULL;
    this->hide();
}

HtmlViewer::~HtmlViewer()
{
    delete ui;
    this->close();
}

void HtmlViewer::HideClicked()
{
    this->hide();
    emit HideHtmlViewerWindow(false);
}

void HtmlViewer::ShowHtml(const QModelIndex &index)
{
    //curobjid = index.sibling(index.row(), 0).data().toULongLong();
    curobjaddr = index.sibling(index.row(), 0).data().toString().split("-f").at(1).toULongLong();
    GetHtmlContent(index);
    this->show();
}

void HtmlViewer::closeEvent(QCloseEvent* e)
{
    emit HideHtmlViewerWindow(false);
    e->accept();
}

void HtmlViewer::GetHtmlContent(const QModelIndex &index)
{
    //this->setWindowTitle("View Html - "); // populate filename here.
    // OpenParentImage
    QString tmpstr = "";
    QStringList evidlist;
    evidlist.clear();
    std::vector<std::string> pathvector;
    //unsigned long long imgid = 0;
    //unsigned long long fsid = 0;
    //unsigned long long fsoffset = 0;
    //unsigned long long address = 0;
    pathvector.clear();
    QDir eviddir = QDir(wombatvariable.tmpmntpath);
    QStringList evidfiles = eviddir.entryList(QStringList("*.evid." + index.sibling(index.row(), 0).data().toString().split("-").at(0).mid(1)), QDir::NoSymLinks | QDir::Files);
    wombatvariable.evidenceobject.name = evidfiles.at(0);
    QFile evidfile(wombatvariable.tmpmntpath + wombatvariable.evidenceobject.name.split(".evid").at(0) + ".evid." + index.sibling(index.row(), 0).data().toString().split("-").at(0).mid(1));
    evidfile.open(QIODevice::ReadOnly);
    tmpstr = evidfile.readLine();
    evidlist = tmpstr.split(",");
    tskptr->partcount = evidlist.at(3).split("|").size();
    evidfile.close();
    for(int i = 0; i < evidlist.at(3).split("|").size(); i++)
        pathvector.push_back(evidlist.at(3).split("|").at(i).toStdString());
    tskptr->imagepartspath = (const char**)malloc(pathvector.size()*sizeof(char*));
    for(int i = 0; i < pathvector.size(); i++)
        tskptr->imagepartspath[i] = pathvector[i].c_str();
    tskptr->readimginfo = tsk_img_open(tskptr->partcount, tskptr->imagepartspath, TSK_IMG_TYPE_DETECT, 0);
    if(tskptr->readimginfo == NULL)
    {
        qDebug() << tsk_error_get_errstr();
        LogMessage("Image opening error");
    }
    free(tskptr->imagepartspath);
    tmpstr = "";
    QStringList partlist;
    partlist.clear();
    QStringList partfiles = eviddir.entryList(QStringList(wombatvariable.evidenceobject.name.split(".evid").at(0) + ".part." + index.sibling(index.row(), 0).data().toString().split("-").at(2).mid(1)), QDir::NoSymLinks | QDir::Files);
    QFile partfile(wombatvariable.tmpmntpath + partfiles.at(0));
    partfile.open(QIODevice::ReadOnly);
    tmpstr = partfile.readLine();
    partfile.close();
    partlist = tmpstr.split(",");
    tskptr->readfsinfo = tsk_fs_open_img(tskptr->readimginfo, partlist.at(4).toULongLong(), TSK_FS_TYPE_DETECT);
    tskptr->readfileinfo = tsk_fs_file_open_meta(tskptr->readfsinfo, NULL, curobjaddr);

    /* NEW IMAGE CODE */
    /*
        QStringList partlist;
        partlist.clear();
        QStringList partfiles = eviddir.entryList(QStringList(wombatvariable.evidenceobject.name.split(".evid").at(0) + ".part." + wombatvariable.selectedobject.modid.split("-").at(2).mid(1)), QDir::NoSymLinks | QDir::Files);
        QFile partfile(wombatvariable.tmpmntpath + partfiles.at(0));
        partfile.open(QIODevice::ReadOnly);
        tmpstr = partfile.readLine();
        partfile.close();
        partlist = tmpstr.split(",");
        //OpenParentImage();
        QStringList filelist;
        filelist.clear();
        QStringList filefiles = eviddir.entryList(QStringList(wombatvariable.evidenceobject.name.split(".evid").at(0) + ".p" + wombatvariable.selectedobject.modid.split("-").at(2).mid(1) + ".f" + wombatvariable.selectedobject.modid.split("-").at(3).mid(1) + ".a*"), QDir::NoSymLinks | QDir::Files);
        //qDebug() << filefiles;
        QFile filefile;
        if(filefiles.count() == 1)
            filefile.setFileName(wombatvariable.tmpmntpath + filefiles.at(0));
        else
        {
            for(int i = 0; i < filefiles.count(); i++)
            {
                if(filefiles.at(i).split(".a").at(1).toInt() == selectedindex.parent().sibling(selectedindex.parent().row(), 0).data().toString().split("-f").at(1).toInt())
                {
                    qDebug() << filefiles.at(i);
                    filefile.setFileName(wombatvariable.tmpmntpath + filefiles.at(i));
                }
            }
        }
        filefile.open(QIODevice::ReadOnly);
        tmpstr = filefile.readLine();
        QFile filefileprop(filefile.fileName().split(".a").at(0) + ".prop");
        QString blockstring;
        QString residentstring;
        QString bytestring;
        //qDebug() << "File Proprety File Name:" << filefileprop.fileName();
        filefile.close();
        filelist = tmpstr.split(",");
        tskobjptr->readfsinfo = tsk_fs_open_img(tskobjptr->readimginfo, partlist.at(4).toULongLong(), TSK_FS_TYPE_DETECT);
        tskobjptr->fsoffset = tskobjptr->readfsinfo->offset;
        tskobjptr->blocksize = tskobjptr->readfsinfo->block_size;
        tskobjptr->objecttype = 5;
        tskobjptr->address = wombatvariable.selectedobject.modid.split("-").at(3).mid(1).toInt();
        tskobjptr->length = selectedindex.sibling(selectedindex.row(), 3).data().toULongLong();
        tskobjptr->offset = 0;
        filefileprop.open(QIODevice::ReadOnly);
        while(!filefileprop.atEnd())
        {
            QString tmpstring = filefileprop.readLine();
            if(tmpstring.contains("Block Address"))
            {
                blockstring = tmpstring.split("||").at(1);
            }
            else if(tmpstring.contains("Resident Offset"))
            {
                residentstring = tmpstring.split("||").at(1);
            }
            else if(tmpstring.contains("Byte Offset"))
            {
                bytestring = tmpstring.split("||").at(1);
            }
        }
        filefileprop.close();
        if(blockstring.compare("") != 0)
        {
            tskobjptr->offset = bytestring.toULongLong();
        }
        else
        {
            if(tskobjptr->readfsinfo->ftype == TSK_FS_TYPE_NTFS_DETECT)
            {
                tskobjptr->resoffset = residentstring.toULongLong();
                tskobjptr->offset = tskobjptr->resoffset + tskobjptr->fsoffset;
            }
            else
                tskobjptr->offset = tskobjptr->fsoffset;
        }
        tskobjptr->blockaddress = blockstring;
        tskobjptr->blkaddrlist = blockstring.split("^^", QString::SkipEmptyParts);


     *
     *
     */ 
    /*
    QSqlQuery pimgquery(fcasedb);
    pimgquery.prepare("SELECT parimgid, parfsid, address FROM Data WHERE objectid = ?;");
    pimgquery.addBindValue(curobjid);
    pimgquery.exec();
    pimgquery.next();
    imgid = pimgquery.value(0).toULongLong();
    fsid = pimgquery.value(1).toULongLong();
    address = pimgquery.value(2).toULongLong();
    pimgquery.finish();
    pimgquery.prepare("SELECT fullpath FROM dataruns WHERE objectid = ? ORDER BY seqnum;");
    pimgquery.addBindValue(imgid);
    if(pimgquery.exec())
    {
        while(pimgquery.next())
        {
            pathvector.push_back(pimgquery.value(0).toString().toStdString());
        }
    }
    pimgquery.finish();
    */
    /* 
    tskptr->imagepartspath = (const char**)malloc(pathvector.size()*sizeof(char*));
    for(uint i=0; i < pathvector.size(); i++)
    {
        tskptr->imagepartspath[i] = pathvector.at(i).c_str();
    }
    tskptr->readimginfo = tsk_img_open(pathvector.size(), tskptr->imagepartspath, TSK_IMG_TYPE_DETECT, 0);
    free(tskptr->imagepartspath);
    */
    // OpenParentFileSystem
    /*
    pimgquery.prepare("SELECT byteoffset FROM data where objectid = ?;");
    pimgquery.addBindValue(fsid);
    pimgquery.exec();
    pimgquery.next();
    fsoffset = pimgquery.value(0).toULongLong();
    pimgquery.finish();
    */
    //tskptr->readfsinfo = tsk_fs_open_img(tskptr->readimginfo, fsoffset, TSK_FS_TYPE_DETECT);
    // OpenFile
    //tskptr->readfileinfo = tsk_fs_file_open_meta(tskptr->readfsinfo, NULL, address);
    // ReadFileToEncodedTextUsingByteArray
    //
    if(tskptr->readfileinfo->meta != NULL)
    {
        char tbuffer[tskptr->readfileinfo->meta->size];
        ssize_t htmllen = tsk_fs_file_read(tskptr->readfileinfo, 0, tbuffer, tskptr->readfileinfo->meta->size, TSK_FS_FILE_READ_FLAG_NONE);
        htmldata = QByteArray::fromRawData(tbuffer, htmllen);
        ui->webView->setContent(htmldata, "text/html", QUrl(""));
    }
}

