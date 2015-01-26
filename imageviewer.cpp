#include "imageviewer.h"

ImageViewer::ImageViewer(QWidget* parent) : QDialog(parent), ui(new Ui::ImageViewer)
{
    ui->setupUi(this);
    lw = ui->listView;
    ui->listView->setViewMode(QListView::IconMode);
    sb = ui->spinBox;
    ui->spinBox->setValue(thumbsize);
    //qDebug() << QImageReader::supportedImageFormats();
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
    for(int i=0; i < thumblist.count(); i++)
    {
        pixmaps.append(QPixmap::fromImage(MakeThumb(thumblist.at(i))));
    }
}

void ImageViewer::UpdateGeometries()
{
    GetPixmaps();
    imagemodel = new ImageModel(pixmaps);
    ui->listView->setModel(imagemodel);
}
