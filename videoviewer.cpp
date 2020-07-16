#include "videoviewer.h"

// Copyright 2013-2019 Pasquale J. Rinaldi, Jr.
// Distrubted under the terms of the GNU General Public License version 2

using namespace QtAV;

VideoViewer::VideoViewer(QWidget* parent) : QDialog(parent), ui(new Ui::VideoViewer)
{
    ui->setupUi(this);
    vunit = 1000;
    vplayer = new AVPlayer(this);
    voutput = new VideoOutput(this);
    vplayer->setRenderer(voutput);
    //vplayer->setAsyncLoad(true);
    ui->horizontalLayout->addWidget(voutput->widget());
    connect(ui->horizontalSlider, SIGNAL(sliderMoved(int)), this, SLOT(Seek(int)));
    connect(ui->horizontalSlider, SIGNAL(sliderPressed()), this, SLOT(SeekBySlider()));
    connect(vplayer, SIGNAL(positionChanged(qint64)), this, SLOT(UpdateSlider(qint64)));
    connect(vplayer, SIGNAL(started()), this, SLOT(UpdateSlider()));
    connect(vplayer, SIGNAL(notifyIntervalChanged()), this, SLOT(UpdateSliderUnit()));
    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(PlayPause()));
    ui->label2->setVisible(false);
    ui->label3->setVisible(false);
    //connect(vplayer, SIGNAL(durationChanged(qint64)), this, SLOT(SetDuration(qint64)));
    //ui->pushButton->setText("Pause");
    /*
    vplayer = new QMediaPlayer;
    videowidget = new QVideoWidget;
    vplayer->setVideoOutput(videowidget);
    ui->horizontalLayout->addWidget(videowidget);
    ui->horizontalSlider->setEnabled(false);
    connect(ui->horizontalSlider, SIGNAL(sliderMoved(int)), this, SLOT(Seek(int)));
    connect(vplayer, SIGNAL(positionChanged(qint64)), this, SLOT(UpdateSlider(qint64)));
    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(PlayPause()));
    */
}

VideoViewer::~VideoViewer()
{
    delete ui;
    this->close();
}

bool VideoViewer::LoadFile(QString filename)
{
    qDebug() << "filename:" << filename;
    vplayer->setFile(filename);
    bool tmpbool = vplayer->load();
    if(tmpbool)
        return true;
    return false;
}

void VideoViewer::Seek(int pos)
{
    if(!vplayer->isPlaying())
        return;
    vplayer->seek(qint64(pos*vunit));
    //vplayer->setPosition(pos*1000LL);
}

void VideoViewer::SeekBySlider()
{
    Seek(ui->horizontalSlider->value());
}

void VideoViewer::PlayPause()
{
    if(!vplayer->isPlaying())
    {
        vplayer->play();
        return;
    }
    vplayer->pause(!vplayer->isPaused());
}

void VideoViewer::UpdateSlider(qint64 pos)
{
    ui->horizontalSlider->setRange(0, int(vplayer->duration()/vunit));
    ui->horizontalSlider->setValue(int(pos/vunit));
    //ui->horizontalSlider->setValue(int(pos/1000LL));
    ui->label->setText(QTime(0, 0, 0).addMSecs(pos).toString("HH:mm:ss"));
    //ui->label3->setText(QTime(0, 0, 0).addMSecs(int(vplayer->mediaStopPosition()/vunit)).toString("HH:mm:ss"));
    //ui->label3->setText(QTime(0, 0, 0).addMSecs(pos).toString("HH:mm:ss"));
}

void VideoViewer::UpdateSlider()
{
    UpdateSlider(vplayer->position());
}

void VideoViewer::UpdateSliderUnit()
{
    vunit = vplayer->notifyInterval();
    UpdateSlider();
}

/*
void VideoViewer::SetDuration(qint64 pos)
{
    //ui->horizontalSlider->setRange(0, int(pos/1000LL));
    //ui->label3->setText(QTime(0, 0, 0).addMSecs(pos).toString("HH:mm:ss"));
}
*/

void VideoViewer::ShowVideo(const QModelIndex &index)
{
    this->show();
    ui->label_2->setVisible(true);
    this->setWindowTitle(QString("Video Viewer - ") + QString(index.sibling(index.row(), 11).data().toString()));
    curobjaddr = index.sibling(index.row(), 11).data().toString().toLongLong();
    vplayer->play(QString(wombatvariable.tmpfilepath + index.sibling(index.row(), 11).data().toString() + "-fhex"));
    ui->label_2->setVisible(false);
    //ui->label3->setText(QTime(0, 0, 0).addMSecs(vplayer->mediaStopPosition()/vunit).toString("HH:mm:ss"));
}
void VideoViewer::mousePressEvent(QMouseEvent* e)
{
    if(e->type() == QEvent::MouseButtonPress)
    {
        vplayer->stop();
        this->close();
    }
}
