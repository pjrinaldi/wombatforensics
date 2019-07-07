#include "videoviewer.h"

// Copyright 2013-2019 Pasquale J. Rinaldi, Jr.
// Distrubted under the terms of the GNU General Public License version 2

VideoViewer::VideoViewer(QWidget* parent) : QDialog(parent), ui(new Ui::VideoViewer)
{
    ui->setupUi(this);
    vplayer = new QMediaPlayer;
    videowidget = new QVideoWidget;
    vplayer->setVideoOutput(videowidget);
    ui->horizontalLayout->addWidget(videowidget);
    ui->pushButton->setText("Pause");
    ui->horizontalSlider->setEnabled(false);
    connect(ui->horizontalSlider, SIGNAL(sliderMoved(int)), this, SLOT(Seek(int)));
    connect(vplayer, SIGNAL(positionChanged(qint64)), this, SLOT(UpdateSlider(qint64)));
    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(PlayPause()));
    connect(vplayer, SIGNAL(durationChanged(qint64)), this, SLOT(SetDuration(qint64)));
    tskptr = &tskobj;
    tskptr->readimginfo = NULL;
    tskptr->readfsinfo = NULL;
    tskptr->readfileinfo = NULL;
}

VideoViewer::~VideoViewer()
{
    delete ui;
    this->close();
}

void VideoViewer::Seek(int pos)
{
    vplayer->setPosition(pos*1000LL);
}

void VideoViewer::PlayPause()
{
    if(vplayer->state() == QMediaPlayer::PlayingState)
    {
        ui->pushButton->setText("Play");
        vplayer->pause();
    }
    else
    {
        ui->pushButton->setText("Pause");
        vplayer->play();
    }
}

void VideoViewer::UpdateSlider(qint64 pos)
{
    ui->horizontalSlider->setValue(int(pos/1000LL));
    ui->label->setText(QTime(0, 0, 0).addMSecs(pos).toString("HH:mm:ss"));
}

void VideoViewer::SetDuration(qint64 pos)
{
    ui->horizontalSlider->setRange(0, int(pos/1000LL));
    ui->label3->setText(QTime(0, 0, 0).addMSecs(pos).toString("HH:mm:ss"));
}

void VideoViewer::GetVideo(const QModelIndex &index)
{
    QDir eviddir = QDir(wombatvariable.tmpmntpath);
    QStringList evidfiles = eviddir.entryList(QStringList("*." + index.sibling(index.row(), 11).data().toString().split("-").at(0)), QDir::NoSymLinks | QDir::Dirs);
    QString evidencename = evidfiles.at(0).split(".e").first();
    QString tmpstr = "";
    QStringList evidlist;
    evidlist.clear();
    std::vector<std::string> pathvector;
    pathvector.clear();
    QString estring = index.sibling(index.row(), 11).data().toString().split("-", QString::SkipEmptyParts).at(0);
    QString vstring = index.sibling(index.row(), 11).data().toString().split("-", QString::SkipEmptyParts).at(1);
    QString pstring = index.sibling(index.row(), 11).data().toString().split("-", QString::SkipEmptyParts).at(2);
    QString fstring = index.sibling(index.row(), 11).data().toString().split("-", QString::SkipEmptyParts).at(3);
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
    if(tskptr->readfileinfo->meta != NULL)
    {
        char* ibuffer = new char[tskptr->readfileinfo->meta->size];
        ssize_t imglen = tsk_fs_file_read(tskptr->readfileinfo, 0, ibuffer, tskptr->readfileinfo->meta->size, TSK_FS_FILE_READ_FLAG_NONE);
        QByteArray filedata = QByteArray::fromRawData(ibuffer, imglen);
        QBuffer* filebuf = new QBuffer(vplayer);
        filebuf->setData(filedata);
        filebuf->open(QIODevice::ReadOnly);
        vplayer->setMedia(QMediaContent(), filebuf);
    }
}

void VideoViewer::ShowVideo(const QModelIndex &index)
{
    this->show();
    ui->label_2->setVisible(true);
    this->setWindowTitle(QString("Video Viewer - ") + QString(index.sibling(index.row(), 1).data().toString()));
    curobjaddr = index.sibling(index.row(), 11).data().toString().split("-f").at(1).toLongLong();
    GetVideo(index);
    vplayer->play();
    ui->label_2->setVisible(false);
}
void VideoViewer::mousePressEvent(QMouseEvent* e)
{
    if(e->type() == QEvent::MouseButtonPress)
    {
        vplayer->stop();
        this->close();
    }
}
