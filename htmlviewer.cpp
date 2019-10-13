#include "htmlviewer.h"

// Copyright 2013-2019 Pasquale J. Rinaldi, Jr.
// Distrubted under the terms of the GNU General Public License version 2

HtmlViewer::HtmlViewer(QWidget* parent) : QDialog(parent), ui(new Ui::HtmlViewer)
{
    ui->setupUi(this);
    tskptr = &tskobj;
    tskptr->readimginfo = NULL;
    tskptr->readfsinfo = NULL;
    tskptr->readfileinfo = NULL;
}

HtmlViewer::~HtmlViewer()
{
    delete ui;
    this->close();
}

void HtmlViewer::HideClicked()
{
    this->close();
}

void HtmlViewer::ShowHtml(const QModelIndex &index)
{
    curobjaddr = index.sibling(index.row(), 11).data().toString().split("-f").at(1).toLongLong();
    QString oldfile = wombatvariable.tmpfilepath + index.sibling(index.row(), 11).data().toString() + "-fhex";
    QString newfile = oldfile + ".html";
    QFile::copy(oldfile, newfile);
    ui->webView->setUrl(QUrl::fromLocalFile(newfile));
    //GetHtmlContent(index);
    this->show();
}

void HtmlViewer::LoadHtml(QString filepath)
{
    // populate html here
    ui->webView->setUrl(QUrl::fromLocalFile(filepath));
    this->show();
}

void HtmlViewer::Reload()
{
    ui->webView->reload();
}

void HtmlViewer::mousePressEvent(QMouseEvent* e)
{
    if(e->type() == QEvent::MouseButtonPress)
        isignals->ActivateReload();
}

void HtmlViewer::closeEvent(QCloseEvent* e)
{
    emit HideReportPreviewWindow(false);
    e->accept();
}

void HtmlViewer::GetHtmlContent(const QModelIndex &index)
{
    //QString thumbid = index.sibling(index.row(), 11).data().toString();
    char* ibuffer = new char[0];
    ssize_t imglen = 0;
    QString tmpstr = "";
    QStringList evidlist;
    evidlist.clear();
    std::vector<std::string> pathvector;
    pathvector.clear();
    QDir eviddir = QDir(wombatvariable.tmpmntpath);
    QStringList evidfiles = eviddir.entryList(QStringList("*." + index.sibling(index.row(), 11).data().toString().split("-").at(0)), QDir::NoSymLinks | QDir::Dirs);
    QString evidencename = evidfiles.at(0).split(".e").first();
    QString thumbid = index.sibling(index.row(), 11).data().toString();
    QString estring = index.sibling(index.row(), 11).data().toString().split("-", QString::SkipEmptyParts).at(0);
    QString vstring = index.sibling(index.row(), 11).data().toString().split("-", QString::SkipEmptyParts).at(1);
    QString pstring = index.sibling(index.row(), 11).data().toString().split("-", QString::SkipEmptyParts).at(2);
    QString fstring = index.sibling(index.row(), 11).data().toString().split("-", QString::SkipEmptyParts).at(3);
    if(fstring.contains(":") == true)
        fstring = fstring.split(":").first() + "-" + fstring.split(":").last();
    QFile evidfile(wombatvariable.tmpmntpath + evidencename + "." + estring + "/stat");
    evidfile.open(QIODevice::ReadOnly);
    tmpstr = evidfile.readLine();
    evidlist = tmpstr.split(",");
    tskptr->partcount = evidlist.at(3).split("|").size();
    evidfile.close();
    for(int i = 0; i < evidlist.at(3).split("|").size(); i++)
        pathvector.push_back(evidlist.at(3).split("|").at(i).toStdString());
    tskptr->imagepartspath = (const char**)malloc(pathvector.size()*sizeof(char*));
    for(uint i = 0; i < pathvector.size(); i++)
        tskptr->imagepartspath[i] = pathvector[i].c_str();
    tskptr->readimginfo = tsk_img_open(tskptr->partcount, tskptr->imagepartspath, TSK_IMG_TYPE_DETECT, 0);
    if(tskptr->readimginfo == NULL)
    {
        qDebug() << tsk_error_get_errstr();
        //LogMessage("Image opening error");
    }
    free(tskptr->imagepartspath);
    tmpstr = "";
    QStringList partlist;
    partlist.clear();
    QFile partfile(wombatvariable.tmpmntpath + evidencename + "." + estring + "/" + vstring + "/" + pstring + "/stat");
    partfile.open(QIODevice::ReadOnly);
    tmpstr = partfile.readLine();
    partfile.close();
    partlist = tmpstr.split(",");
    tskptr->readfsinfo = tsk_fs_open_img(tskptr->readimginfo, partlist.at(4).toLongLong(), TSK_FS_TYPE_DETECT);
    tskptr->readfileinfo = tsk_fs_file_open_meta(tskptr->readfsinfo, NULL, curobjaddr);
    QDir filedir = QDir(wombatvariable.tmpmntpath + evidencename + "." + estring + "/" + vstring + "/" + pstring);
    QStringList filefiles = filedir.entryList(QStringList(fstring + ".a*.stat"), QDir::NoSymLinks | QDir::Files);
    QFile filefile(wombatvariable.tmpmntpath + evidencename + "." + estring + "/" + vstring + "/" + pstring + "/" + filefiles.at(0));
    filefile.open(QIODevice::ReadOnly);
    tmpstr = filefile.readLine();
    if(filefile.isOpen())
        filefile.close();
    if(tskptr->readfileinfo->meta != NULL)
    {
        if(partlist.at(0).toInt() == TSK_FS_TYPE_NTFS_DETECT) // IF NTFS (ADS/FILE/DIR/RES/NONRES)
        {
            if(thumbid.split("-").at(3).split(":").count() > 1) // IF ADS
            {
                ibuffer = new char[tmpstr.split(",").at(8).toULongLong()];
                imglen = tsk_fs_file_read_type(tskptr->readfileinfo, TSK_FS_ATTR_TYPE_NTFS_DATA, thumbid.split("-").at(3).split(":").at(1).toInt(), 0, ibuffer, tmpstr.split(",").at(8).toULongLong(), TSK_FS_FILE_READ_FLAG_SLACK);
                if(imglen == -1)
                    qDebug() << tsk_error_get_errstr();
            }
            else // IF NOT ADS
            {
                ibuffer = new char[tskptr->readfileinfo->meta->size];
                imglen = tsk_fs_file_read(tskptr->readfileinfo, 0, ibuffer, tskptr->readfileinfo->meta->size, TSK_FS_FILE_READ_FLAG_SLACK);
            }
        }
        else // OTHER FILE SYSTEM
        {
            ibuffer = new char[tskptr->readfileinfo->meta->size];
            imglen = tsk_fs_file_read(tskptr->readfileinfo, 0, ibuffer, tskptr->readfileinfo->meta->size, TSK_FS_FILE_READ_FLAG_SLACK);
        }
        htmldata = QByteArray::fromRawData(ibuffer, imglen);
        ui->webView->setContent(htmldata, "text/html", QUrl(""));
    }
    //imglen = PopulateFileBuffer(thumbid, &ibuffer);
    //htmldata = QByteArray::fromRawData(ibuffer, imglen);
    //ui->webView->setContent(htmldata, "text/html", QUrl(""));
    delete[] ibuffer;
}

