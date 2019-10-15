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

void VideoViewer::ShowVideo(const QModelIndex &index)
{
    this->show();
    ui->label_2->setVisible(true);
    this->setWindowTitle(QString("Video Viewer - ") + QString(index.sibling(index.row(), 11).data().toString()));
    curobjaddr = index.sibling(index.row(), 11).data().toString().toLongLong();
    vplayer->setMedia(QUrl::fromLocalFile(wombatvariable.tmpfilepath + index.sibling(index.row(), 11).data().toString() + "-fhex"));
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
