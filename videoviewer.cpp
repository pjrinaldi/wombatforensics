#include "videoviewer.h"

// Copyright 2015 Pasquale J. Rinaldi, Jr.
// Distrubted under the terms of the GNU General Public License version 2

VideoViewer::VideoViewer(QWidget* parent) : QDialog(parent), ui(new Ui::VideoViewer)
{
    ui->setupUi(this);
    vplayer = new QMediaPlayer;
    videowidget = new QVideoWidget;
    vplayer->setVideoOutput(videowidget);
    ui->horizontalLayout->addWidget(videowidget);
    //videowidget->show();
    connect(ui->horizontalSlider, SIGNAL(sliderMoved(int)), this, SLOT(Seek(int)));
    connect(vplayer, SIGNAL(positionChanged(qint64)), this, SLOT(UpdateSlider()));
    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(PlayPause()));
    //QtAV::Widgets::registerRenderers();
    //vplayer = new QtAV::AVPlayer(this);
    //vout = new QtAV::VideoOutput(this);
    //vplayer->setRenderer(vout);
    //ui->horizontalLayout->addWidget(vout->widget());
    //connect(ui->horizontalSlider, SIGNAL(sliderMoved(int)), this, SLOT(Seek(int)));
    //connect(vplayer, SIGNAL(positionChanged(qint64)), this, SLOT(UpdateSlider()));
    //connect(vplayer, SIGNAL(started()), this, SLOT(UpdateSlider()));
    //connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(PlayPause()));
    //ui->pushButton->setText("Pause");
    tskptr = &tskobj;
    tskptr->readimginfo = NULL;
    tskptr->readfsinfo = NULL;
    tskptr->readfileinfo = NULL;
    this->hide();
}

VideoViewer::~VideoViewer()
{
}

void VideoViewer::Seek(int pos)
{
    /*
    if(!vplayer->isPlaying())
        return;
    vplayer->seek(pos*1000LL);
    */
}

void VideoViewer::PlayPause()
{
    if(vplayer->state() == QMediaPlayer::PlayingState)
    {
        ui->pushButton->setText("Pause");
        vplayer->play();
        //return;
    }
    ui->pushButton->setText("Play");
    vplayer->pause();
    /*
    if(!vplayer->isPlaying())
    {
        ui->pushButton->setText("Pause");
        vplayer->play();
        return;
    }
    ui->pushButton->setText("Play");
    vplayer->pause(!vplayer->isPaused());*/
}

void VideoViewer::UpdateSlider()
{
    /*
    ui->label3->setText(QTime(0, 0, 0).addMSecs(vplayer->mediaStopPosition()).toString("HH:mm:ss"));
    ui->horizontalSlider->setRange(0, int(vplayer->duration()/1000LL));
    ui->horizontalSlider->setValue(int(vplayer->position()/1000LL));
    ui->label->setText(QTime(0, 0, 0).addMSecs(vplayer->position()).toString("HH:mm:ss"));
    */
}

//void VideoViewer::GetVideo(QString tmpfilepath, unsigned long long objectid)
void VideoViewer::GetVideo(const QModelIndex &index)
{
    // OpenParentImage
    QString tmpstr = "";
    QStringList evidlist;
    evidlist.clear();
    std::vector<std::string> pathvector;
    unsigned long long imgid = 0;
    unsigned long long fsid = 0;
    unsigned long long fsoffset = 0;
    unsigned long long address = 0;
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
        //LogMessage("Image opening error");
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
    if(tskptr->readfileinfo->meta != NULL)
    {
        char* ibuffer = new char[tskptr->readfileinfo->meta->size];
        ssize_t imglen = tsk_fs_file_read(tskptr->readfileinfo, 0, ibuffer, tskptr->readfileinfo->meta->size, TSK_FS_FILE_READ_FLAG_NONE);
        QByteArray filedata = QByteArray::fromRawData(ibuffer, imglen);
        QBuffer filebuf(&filedata);
        filebuf.open(QIODevice::ReadOnly);
        vplayer->setMedia(QMediaContent(), &filebuf);
    }
    /*
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
    // ReadFileToImageUsingByteArray
    /*
    if(tskptr->readfileinfo->meta != NULL)
    {
        char* ibuffer = new char[tskptr->readfileinfo->meta->size];
        ssize_t imglen = tsk_fs_file_read(tskptr->readfileinfo, 0, ibuffer, tskptr->readfileinfo->meta->size, TSK_FS_FILE_READ_FLAG_NONE);
        QString tmppath = tmpfilepath + QString::number(objectid) + "-tmp";
        QFile tmpfile(tmppath);
        if(tmpfile.open(QIODevice::WriteOnly))
        {
            QDataStream outbuffer(&tmpfile);
            outbuffer.writeRawData(ibuffer, imglen);
            tmpfile.close();
        }
        //vplayer->setFile(tmppath);
    }
    */
}

//void VideoViewer::ShowVideo(QString tmpfilepath, const QModelIndex &index)
void VideoViewer::ShowVideo(const QModelIndex &index)
{
    this->show();
    ui->label_2->setVisible(true);
    curobjaddr = index.sibling(index.row(), 0).data().toString().split("-f").at(1).toULongLong();
    QtConcurrent::run(this, &VideoViewer::GetVideo, index);
    vplayer->play();
    //vplayer->play();
    //QtConcurrent::run(this, &VideoViewer::GetVideo, tmpfilepath, index.sibling(index.row(), 0).data().toULongLong());
    //vplayer->play();
    ui->label_2->setVisible(false);
}
void VideoViewer::mousePressEvent(QMouseEvent* e)
{
    if(e->type() == QEvent::MouseButtonPress)
    {
        vplayer->stop();
        //vplayer->stop();
        //vplayer->unload();
        this->hide();
    }
}
