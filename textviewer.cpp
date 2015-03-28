#include "textviewer.h"

TextViewer::TextViewer(QWidget* parent) : QDialog(parent), ui(new Ui::TextViewer)
{
    ui->setupUi(this);
    tskptr = &tskobj;
    tskptr->readimginfo = NULL;
    tskptr->readfsinfo = NULL;
    tskptr->readfileinfo = NULL;
    this->hide();
}

TextViewer::~TextViewer()
{
    delete ui;
    this->close();
}

void TextViewer::HideClicked()
{
    this->hide();
    emit HideTextViewerWindow(false);
}

void TextViewer::ShowText(const QModelIndex &index)
{
    GetTextContent(index.sibling(index.row(), 0).data().toInt());
    this->show();
}

void TextViewer::closeEvent(QCloseEvent* e)
{
    emit HideTextViewerWindow(false);
    e->accept();
}

void TextViewer::GetTextContent(int objectid)
{
    this->setWindowTitle("View Text - "); // populate filename here.
    // OpenParentImage
    std::vector<std::string> pathvector;
    int imgid = 0;
    int fsid = 0;
    int fsoffset = 0;
    int address = 0;
    pathvector.clear();
    QSqlQuery pimgquery(fcasedb);
    pimgquery.prepare("SELECT parimgid, parfsid, address FROM Data WHERE objectid = ?;");
    pimgquery.addBindValue(objectid);
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
    /*
    // ReadFileToImageUsingByteArray
    if(tskptr->readfileinfo->meta != NULL)
    {
        QImage tmpimage;
        QByteArray iba;
        QBuffer ibuff(&iba);
        char ibuffer[tskptr->readfileinfo->meta->size];
        ssize_t imglen = tsk_fs_file_read(tskptr->readfileinfo, 0, ibuffer, tskptr->readfileinfo->meta->size, TSK_FS_FILE_READ_FLAG_NONE);
        bool isloaded = tmpimage.loadFromData(QByteArray::fromRawData(ibuffer, imglen));
        if(isloaded)
            ui->label->setPixmap(QPixmap::fromImage(tmpimage));
        else
            ui->label->setPixmap(QPixmap::fromImage(QImage(":/bar/missingthumb")));
    }
    */
}
