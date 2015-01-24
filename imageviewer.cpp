#include "imageviewer.h"

ImageViewer::ImageViewer(QWidget* parent) : QDialog(parent), ui(new Ui::ImageViewer)
{
    ui->setupUi(this);
    lw = ui->listWidget;
    //qDebug() << QImageReader::supportedImageFormats();
    this->hide();
}

void ImageViewer::SetFutureWatcher(ImageWatcher* w)
{
    watcher = w;
    if(w)
    {
        connect(w, SIGNAL(resultReadyAt(int)), this, SLOT(ReadyAt(int)));
        connect(w, SIGNAL(progressRangeChanged(int, int)), ui->progressBar, SLOT(setRange(int, int)));
        connect(w, SIGNAL(progressValueChanged(int)), ui->progressBar, SLOT(setValue(int)));
        connect(w, SIGNAL(finished()), ui->progressBar, SLOT(hide()));
        connect(w, SIGNAL(started()), ui->progressBar, SLOT(show()));
    }
}

void ImageViewer::ReadyAt(int which)
{
    QFuture<QImage> f = watcher->future();
    QListWidgetItem* item = new QListWidgetItem(ui->listWidget);
    item->setData(Qt::DecorationRole, QPixmap::fromImage(f.resultAt(which)));
}

ImageViewer::~ImageViewer()
{
}
