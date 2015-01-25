#include "imageviewer.h"

ImageViewer::ImageViewer(QWidget* parent) : QDialog(parent), ui(new Ui::ImageViewer)
{
    ui->setupUi(this);
    lw = ui->listWidget;
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

void ImageViewer::UpdateGeometries()
{
    lw->repaint();
}
