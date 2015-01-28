#include "videoviewer.h"

VideoViewer::VideoViewer(QWidget* parent) : QDialog(parent), ui(new Ui::VideoViewer)
{
    ui->setupUi(this);
    QtAV::Widgets::registerRenderers();
    vplayer = new QtAV::AVPlayer(this);
    vout = new QtAV::VideoOutput(this);
    vplayer->setRenderer(vout);
    ui->horizontalLayout->addWidget(vout->widget());
    this->hide();
}

VideoViewer::~VideoViewer()
{
}

void VideoViewer::ShowVideo(const QModelIndex &index)
{
    this->show();
    vplayer->play("/home/pasquale/Downloads/small.mp4");
}
void VideoViewer::mousePressEvent(QMouseEvent* e)
{
    if(e->type() == QEvent::MouseButtonPress)
    {
        qDebug() << "mouse pressed, so close.";
        this->hide();
    }
}

