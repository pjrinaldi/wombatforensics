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
        ui->label->clear();
        this->hide();
    }
}

void ImageWindow::GetImage(unsigned long long objectid)
{
    this->setWindowTitle("View Image - " + thumbpath);
    // OpenParentImage
    std::vector<std::string> pathvector;
    unsigned long long imgid = 0;
    unsigned long long fsid = 0;
    unsigned long long fsoffset = 0;
    unsigned long long address = 0;
    pathvector.clear();
    QSqlQuery pimgquery(fcasedb);
    pimgquery.prepare("SELECT parimgid, parfsid, address FROM Data WHERE objectid = ?;");
    pimgquery.addBindValue(objectid);
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
    fsoffset = pimgquery.value(0).toULongLong();
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
    ui->listView->setUniformItemSizes(false);
    ui->listView->setResizeMode(QListView::Adjust);
    sb = ui->spinBox;
    ui->spinBox->setValue(thumbsize);
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
    for(int i=0; i < thumblist.count()/2; i++)
    {
        pixmaps.append(QPixmap::fromImage(MakeThumb(thumblist.at(2*i+1))));
        idlist.append(thumblist.at(2*i));
    }
}

void ImageViewer::UpdateGeometries()
{
    ui->label_2->setText("Loading...");
    QFuture<void> thumbfuture = QtConcurrent::run(this, &ImageViewer::GetPixmaps);
    thumbwatcher.setFuture(thumbfuture);
    //GetPixmaps();
    //imagemodel = new ImageModel(pixmaps, idlist);
    //ui->listView->setModel(imagemodel);
    connect(&thumbwatcher, SIGNAL(finished()), this, SLOT(SetModel()), Qt::QueuedConnection);
}

void ImageViewer::SetModel()
{
    qDebug() << pixmaps.count();
    imagemodel = new ImageModel(pixmaps, idlist);
    ui->listView->setModel(imagemodel);
    connect(ui->listView, SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(OpenImageWindow(const QModelIndex &)));
    connect(ui->listView, SIGNAL(clicked(const QModelIndex &)), this, SLOT(HighlightTreeViewItem(const QModelIndex &)));
    ui->label_2->setText(QString::number(pixmaps.count()) + " Image(s)");
}

void ImageViewer::OpenImageWindow(const QModelIndex &index)
{
    ui->label->setText("Loading...");
    QtConcurrent::run(imagedialog, &ImageWindow::GetImage, index.data(Qt::UserRole).toULongLong());
    //imagedialog->GetImage(index.data(Qt::UserRole).toULongLong());
    imagedialog->show();
    ui->label->setText("");
}

void ImageViewer::ShowImage(const QModelIndex &index)
{
    ui->label->setText("Loading...");
    QtConcurrent::run(imagedialog, &ImageWindow::GetImage, index.sibling(index.row(), 0).data().toULongLong());
    //imagedialog->GetImage(index.sibling(index.row(), 0).data().toULongLong());
    imagedialog->show();
    ui->label->setText("");
}

void ImageViewer::HighlightTreeViewItem(const QModelIndex &index)
{
    QSqlQuery pathquery(fcasedb);
    pathquery.prepare("SELECT (fullpath || name) AS fullname FROM data WHERE objectid = ?;");
    pathquery.addBindValue(index.data(Qt::UserRole).toULongLong());
    pathquery.exec();
    pathquery.next();
    thumbpath = pathquery.value(0).toString();
    pathquery.finish();

    emit SendObjectToTreeView(index.data(Qt::UserRole).toULongLong()); 
}
