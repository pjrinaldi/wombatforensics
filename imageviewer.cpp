#include "imageviewer.h"

ImageViewer::ImageViewer(QWidget* parent) : QListWidget(parent)
{
    setViewMode(IconMode);
    setResizeMode(Adjust);
    setMovement(Static);
    setVerticalScrollMode(ScrollPerPixel);
    progressbar = new QProgressBar(this);
    progressbar->setGeometry(10, 10, 200, progressbar->sizeHint().height());
    progressbar->setFormat("%v of %m (%p%)");
    //qDebug() << QImageReader::supportedImageFormats();
    this->hide();
}

void ImageViewer::SetFutureWatcher(ImageWatcher* w)
{
    watcher = w;
    if(w)
    {
        connect(w, SIGNAL(resultReadyAt(int)), this, SLOT(ReadyAt(int)));
        connect(w, SIGNAL(progressRangeChanged(int, int)), progressbar, SLOT(setRange(int, int)));
        connect(w, SIGNAL(progressValueChanged(int)), progressbar, SLOT(setValue(int)));
        connect(w, SIGNAL(finished()), progressbar, SLOT(hide()));
        connect(w, SIGNAL(started()), progressbar, SLOT(show()));
    }
}

void ImageViewer::ReadyAt(int which)
{
    QFuture<QImage> f = watcher->future();
    QListWidgetItem* item = new QListWidgetItem(this);
    item->setData(Qt::DecorationRole, QPixmap::fromImage(f.resultAt(which)));
}

ImageViewer::~ImageViewer()
{
}
