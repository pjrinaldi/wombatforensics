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
    //this->hide();
}

HtmlViewer::~HtmlViewer()
{
    delete ui;
    this->close();
}

void HtmlViewer::HideClicked()
{
    this->close();
    //this->hide();
    //emit HideHtmlViewerWindow(false);
}

void HtmlViewer::ShowHtml(const QModelIndex &index)
{
    curobjaddr = index.sibling(index.row(), 0).data().toString().split("-f").at(1).toULongLong();
    GetHtmlContent(index);
    this->show();
}

void HtmlViewer::LoadHtml(QString filepath)
{
    // populate html here
    ui->webView->setUrl(QUrl::fromLocalFile(filepath));
    this->show();
}

void HtmlViewer::closeEvent(QCloseEvent* e)
{
    //emit HideHtmlViewerWindow(false);
    e->accept();
}

void HtmlViewer::GetHtmlContent(const QModelIndex &index)
{
    //this->setWindowTitle("View Html - "); // populate filename here.
    QString tmpstr = "";
    QStringList evidlist;
    evidlist.clear();
    std::vector<std::string> pathvector;
    pathvector.clear();
    QString estring = index.sibling(index.row(), 0).data().toString().split("-", QString::SkipEmptyParts).at(0);
    QString vstring = index.sibling(index.row(), 0).data().toString().split("-", QString::SkipEmptyParts).at(1);
    QString pstring = index.sibling(index.row(), 0).data().toString().split("-", QString::SkipEmptyParts).at(2);
    QString fstring = index.sibling(index.row(), 0).data().toString().split("-", QString::SkipEmptyParts).at(3);
    //QDir eviddir = QDir(wombatvariable.tmpmntpath);
    //QStringList evidfiles = eviddir.entryList(QStringList("*.evid." + index.sibling(index.row(), 0).data().toString().split("-").at(0).mid(1)), QDir::NoSymLinks | QDir::Files);
    //wombatvariable.evidencename = evidfiles.at(0);
    //QFile evidfile(wombatvariable.tmpmntpath + wombatvariable.evidencename.split(".evid").at(0) + ".evid." + index.sibling(index.row(), 0).data().toString().split("-").at(0).mid(1));
    QFile evidfile(wombatvariable.tmpmntpath + wombatvariable.evidencename + "." + estring + "/stat");
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
    //QStringList partfiles = eviddir.entryList(QStringList(wombatvariable.evidencename.split(".evid").at(0) + ".part." + index.sibling(index.row(), 0).data().toString().split("-").at(2).mid(1)), QDir::NoSymLinks | QDir::Files);
    //QFile partfile(wombatvariable.tmpmntpath + partfiles.at(0));
    QFile partfile(wombatvariable.tmpmntpath + wombatvariable.evidencename + "." + estring + "/." + vstring + "/." + pstring + "/stat");
    partfile.open(QIODevice::ReadOnly);
    tmpstr = partfile.readLine();
    partfile.close();
    partlist = tmpstr.split(",");
    tskptr->readfsinfo = tsk_fs_open_img(tskptr->readimginfo, partlist.at(4).toULongLong(), TSK_FS_TYPE_DETECT);
    tskptr->readfileinfo = tsk_fs_file_open_meta(tskptr->readfsinfo, NULL, curobjaddr);

    if(tskptr->readfileinfo->meta != NULL)
    {
        char tbuffer[tskptr->readfileinfo->meta->size];
        ssize_t htmllen = tsk_fs_file_read(tskptr->readfileinfo, 0, tbuffer, tskptr->readfileinfo->meta->size, TSK_FS_FILE_READ_FLAG_NONE);
        htmldata = QByteArray::fromRawData(tbuffer, htmllen);
        ui->webView->setContent(htmldata, "text/html", QUrl(""));
    }
}

