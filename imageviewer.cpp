#include "imageviewer.h"

ImageViewer::ImageViewer(QWidget* parent) : QDialog(parent), ui(new Ui::ImageViewer)
{
    ui->setupUi(this);
    lw = ui->listView;
    ui->listView->setViewMode(QListView::IconMode);
    sb = ui->spinBox;
    ui->spinBox->setValue(thumbsize);
    imagemodel = new ImageModel();
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

void ImageViewer::UpdateGeometries()
{
    //imagemodel->GetThumbnails();
    ui->listView->setModel(imagemodel);
}
