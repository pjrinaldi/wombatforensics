#include "htmlviewer.h"

HtmlViewer::HtmlViewer(QWidget* parent) : QDialog(parent), ui(new Ui::HtmlViewer)
{
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
    curobjid = index.sibling(index.row(), 0).data().toInt();
    GetHtmlContent();
    this->show();
}

void HtmlViewer::closeEvent(QCloseEvent* e)
{
    emit HideHtmlViewerWindow(false);
    e->accept();
}

void HtmlViewer::GetHtmlContent()
{
    //this->setWindowTitle("View Html - "); // populate filename here.
    // OpenParentImage
    std::vector<std::string> pathvector;
    int imgid = 0;
    int fsid = 0;
    int fsoffset = 0;
    int address = 0;
    pathvector.clear();
    QSqlQuery pimgquery(fcasedb);
    pimgquery.prepare("SELECT parimgid, parfsid, address FROM Data WHERE objectid = ?;");
    pimgquery.addBindValue(curobjid);
    pimgquery.exec();
    pimgquery.next();
    imgid = pimgquery.value(0).toInt();
    fsid = pimgquery.value(1).toInt();
    address = pimgquery.value(2).toInt();
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
    tskptr->imagepartspath = (const char**)malloc(pathvector.size()*sizeof(char*));
    for(uint i=0; i < pathvector.size(); i++)
    {
        tskptr->imagepartspath[i] = pathvector.at(i).c_str();
    }
    tskptr->readimginfo = tsk_img_open(pathvector.size(), tskptr->imagepartspath, TSK_IMG_TYPE_DETECT, 0);
    free(tskptr->imagepartspath);
    // OpenParentFileSystem
    pimgquery.prepare("SELECT byteoffset FROM data where objectid = ?;");
    pimgquery.addBindValue(fsid);
    pimgquery.exec();
    pimgquery.next();
    fsoffset = pimgquery.value(0).toInt();
    pimgquery.finish();
    tskptr->readfsinfo = tsk_fs_open_img(tskptr->readimginfo, fsoffset, TSK_FS_TYPE_DETECT);
    // OpenFile
    tskptr->readfileinfo = tsk_fs_file_open_meta(tskptr->readfsinfo, NULL, address);
    // ReadFileToEncodedTextUsingByteArray
    if(tskptr->readfileinfo->meta != NULL)
    {
        char tbuffer[tskptr->readfileinfo->meta->size];
        ssize_t htmllen = tsk_fs_file_read(tskptr->readfileinfo, 0, tbuffer, tskptr->readfileinfo->meta->size, TSK_FS_FILE_READ_FLAG_NONE);
        htmldata = QByteArray::fromRawData(tbuffer, htmllen);
        /*
        if(tskptr->readfileinfo->meta->size > 2000000000) // 2 GB
        {
            qDebug() << "File is larger than 2GB. Export the file or use an external viewer. Otherwise showing 1st 2GB of text only.";
        }
        char tbuffer[tskptr->readfileinfo->meta->size];
        ssize_t textlen = tsk_fs_file_read(tskptr->readfileinfo, 0, tbuffer, tskptr->readfileinfo->meta->size, TSK_FS_FILE_READ_FLAG_NONE);
        txtdata = QByteArray::fromRawData(tbuffer, textlen);
        UpdateEncoding();
        */
    }
}

