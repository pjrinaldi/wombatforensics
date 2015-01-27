#include "imageviewer.h"

ImageWindow::ImageWindow(QWidget* parent) : QDialog(parent), ui(new Ui::ImageWindow)
{
    ui->setupUi(this);
    tskptr = &tskobj;
    tskptr->readimginfo = NULL;
    tskptr->readfsinfo = NULL;
    tskptr->readfileinfo = NULL;
    this->hide();
}

ImageWindow::~ImageWindow()
{
}

void ImageWindow::HideClicked()
{
    this->hide();
}

void ImageWindow::ShowImage()
{
    this->show();
}

void ImageWindow::mousePressEvent(QMouseEvent* e)
{
    if(e->type() == QEvent::MouseButtonPress)
    {
        qDebug() << "mouse pressed, so close.";
        ui->label->clear();
        this->hide();
    }
}

void ImageWindow::GetImage(int objectid)
{
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
}

ImageViewer::ImageViewer(QWidget* parent) : QDialog(parent), ui(new Ui::ImageViewer)
{
    ui->setupUi(this);
    lw = ui->listView;
    ui->listView->setViewMode(QListView::IconMode);
    sb = ui->spinBox;
    ui->spinBox->setValue(thumbsize);
    //qDebug() << QImageReader::supportedImageFormats();
    imagedialog = new ImageWindow();
    imagedialog->setModal(false);
    imagedialog->hide();
    this->hide();
}

void ImageViewer::HideClicked()
{
    this->hide();
    emit HideImageWindow(false);
}

void ImageViewer::closeEvent(QCloseEvent* e)
{
    emit HideImageWindow(false);
    e->accept();
}

ImageViewer::~ImageViewer()
{
}

void ImageViewer::GetPixmaps()
{
    pixmaps.clear();
    for(int i=0; i < thumblist.count()/3; i++)
    {
        pixmaps.append(QPixmap::fromImage(MakeThumb(thumblist.at(3*i+2))));
        idlist.append(thumblist.at(3*i));
        addresslist.append(thumblist.at(3*i+1));
    }
}

void ImageViewer::UpdateGeometries()
{
    GetPixmaps();
    imagemodel = new ImageModel(pixmaps, idlist, addresslist);
    ui->listView->setModel(imagemodel);
    connect(ui->listView, SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(OpenImageWindow(const QModelIndex &)));
    connect(ui->listView, SIGNAL(clicked(const QModelIndex &)), this, SLOT(HighlightTreeViewItem(const QModelIndex &)));
}

void ImageViewer::OpenImageWindow(const QModelIndex &index)
{
    imagedialog->GetImage(index.data(Qt::UserRole).toInt());
    imagedialog->show();
}

void ImageViewer::HighlightTreeViewItem(const QModelIndex &index)
{
    emit SendObjectToTreeView(index.data(Qt::UserRole).toInt()); 
}
