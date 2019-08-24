#include "imageviewer.h"

// Copyright 2013-2019 Pasquale J. Rinaldi, Jr.
// Distrubted under the terms of the GNU General Public License version 2

ImageWindow::ImageWindow(QWidget* parent) : QDialog(parent), ui(new Ui::ImageWindow)
{
    ui->setupUi(this);
    tskptr = &tskobj;
    tskptr->readimginfo = NULL;
    tskptr->readfsinfo = NULL;
    tskptr->readfileinfo = NULL;
    connect(ui->pushButton, SIGNAL(clicked(bool)), this, SLOT(SwitchScale(bool)));
}

void ImageWindow::SwitchScale(bool switchscale)
{
    ui->label->setScaledContents(switchscale);
}
ImageWindow::~ImageWindow()
{
    this->close();
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
        this->close();
    }
}

void ImageWindow::GetImage(QString objectid)
{
    TSK_IMG_INFO* readimginfo;
    TSK_FS_INFO* readfsinfo;
    TSK_FS_FILE* readfileinfo;
    QString tmpstr = "";
    QDir eviddir = QDir(wombatvariable.tmpmntpath);
    std::vector<std::string> pathvector;
    const TSK_TCHAR** imagepartspath;
    pathvector.clear();
    QStringList evidfiles = eviddir.entryList(QStringList("*." + objectid.split("-").at(0)), QDir::NoSymLinks | QDir::Dirs);
    QString evidencename = evidfiles.at(0).split(".e").first();
    QString estring = objectid.split("-", QString::SkipEmptyParts).at(0);
    QString vstring = objectid.split("-", QString::SkipEmptyParts).at(1);
    QString pstring = objectid.split("-", QString::SkipEmptyParts).at(2);
    qint64 curaddress = objectid.split("-f").at(1).split("-a").at(0).toLongLong(); 
    QFile evidfile(wombatvariable.tmpmntpath + evidencename + "." + estring + "/stat");
    evidfile.open(QIODevice::ReadOnly);
    tmpstr = evidfile.readLine();
    int partcount = tmpstr.split(",").at(3).split("|").size();
    evidfile.close();
    for(int i=0; i < partcount; i++)
        pathvector.push_back(tmpstr.split(",").at(3).split("|").at(i).toStdString());
    imagepartspath = (const char**)malloc(pathvector.size()*sizeof(char*));
    for(uint i=0; i < pathvector.size(); i++)
        imagepartspath[i] = pathvector[i].c_str();
    readimginfo = tsk_img_open(partcount, imagepartspath, TSK_IMG_TYPE_DETECT, 0);
    if(readimginfo == NULL)
    {
        qDebug() << tsk_error_get_errstr();
        //LogMessage("Image opening error");
    }
    free(imagepartspath);
    tmpstr = "";
    QFile partfile(wombatvariable.tmpmntpath + evidencename + "." + estring + "/" + vstring + "/" + pstring + "/stat");
    partfile.open(QIODevice::ReadOnly);
    tmpstr = partfile.readLine();
    readfsinfo = tsk_fs_open_img(readimginfo, tmpstr.split(",").at(4).toLongLong(), TSK_FS_TYPE_DETECT);
    readfileinfo = tsk_fs_file_open_meta(readfsinfo, NULL, curaddress);
    QImage fileimage;
    char imgbuf[readfileinfo->meta->size];
    ssize_t imglen = tsk_fs_file_read(readfileinfo, 0, imgbuf, readfileinfo->meta->size, TSK_FS_FILE_READ_FLAG_NONE);
    if(readfileinfo->meta != NULL)
    {
        bool imageloaded = fileimage.loadFromData(QByteArray::fromRawData(imgbuf, imglen));
        if(imageloaded)
        {
            ui->label->setPixmap(QPixmap::fromImage(fileimage));
        }
        else
        {
            ui->label->setPixmap(QPixmap::fromImage(QImage(":/bar/missingthumb")));
        }
    }
    tsk_fs_file_close(readfileinfo);
    tsk_fs_close(readfsinfo);
    tsk_img_close(readimginfo);
}

ImageViewer::ImageViewer(QWidget* parent) : QDialog(parent), ui(new Ui::ImageViewer)
{
    ui->setupUi(this);
    setWindowTitle("Thumbnail Viewer");
    connect(ui->listWidget, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(OpenImageWindow(QListWidgetItem*)));
    connect(ui->listWidget, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(HighlightTreeViewItem(QListWidgetItem*)));
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

void ImageViewer::LoadThumbnails()
{
    ui->listWidget->clear();
    ui->listWidget->setIconSize(QSize(thumbsize * (100 / vidcount), thumbsize+20));
    QByteArray ba;
    ba.clear();
    QDir tdir = QDir(wombatvariable.tmpmntpath + "thumbs/");
    QStringList jpgfiles = tdir.entryList(QStringList("*.jpg"), QDir::NoSymLinks | QDir::Files);
    for(int i = 0; i < jpgfiles.count(); i++)
    {
        QListWidgetItem* tmpitem = new QListWidgetItem(QIcon(wombatvariable.tmpmntpath + "thumbs/" + jpgfiles.at(i)), jpgfiles.at(i).split(".").at(0), ui->listWidget);
        ba.clear();
        ba.append(imageshash.value(jpgfiles.at(i).split(".").at(0)));
        tmpitem->setToolTip(QString(QByteArray::fromBase64(ba)));
    }
    ui->label->setText("Thumbnail Count: " + QString::number(jpgfiles.count()));
}

void ImageViewer::OpenImageWindow(QListWidgetItem* item)
{
    ui->label->setText("Loading...");
    imagedialog = new ImageWindow();
    imagedialog->setModal(false);
    imagedialog->setAttribute(Qt::WA_DeleteOnClose);
    imagedialog->setWindowTitle(item->text() + " Image Viewer");
    QImage iconimage(QString(wombatvariable.tmpmntpath + "thumbs/" + item->text() + ".jpg"));
    if(iconimage.width() == thumbsize)
    {
        imagedialog->GetImage(item->text());
        imagedialog->show();
    }
    else if(iconimage.width() > thumbsize)
    {
        QModelIndexList indexlist = treenodemodel->match(treenodemodel->index(0, 11, QModelIndex()), Qt::DisplayRole, QVariant(item->text().split("-a").first()), -1, Qt::MatchFlags(Qt::MatchExactly | Qt::MatchRecursive));
        if(indexlist.count() > 0)
        {
            videowindow = new VideoViewer();
            videowindow->setAttribute(Qt::WA_DeleteOnClose);
            videowindow->setWindowIcon(QIcon(":/vid"));
            videowindow->setWindowTitle(item->text() + "Video Viewer");
            videowindow->setWindowTitle(indexlist.first().sibling(indexlist.first().row(), 11).data().toString() + " Video Viewer");
            videowindow->ShowVideo(indexlist.first());
        }
    }
    ui->label->setText("");
}

void ImageViewer::HighlightTreeViewItem(QListWidgetItem* item)
{
    //qDebug() << "image item:" << item->text();
    emit SendObjectToTreeView(item->text().split("-a").at(0));
}
