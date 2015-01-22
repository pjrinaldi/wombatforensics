#include "imageviewer.h"

ImageViewer::ImageViewer(QWidget* parent) : QWidget(parent)
{
    qDebug() << QImageReader::supportedImageFormats();
}

ImageViewer::~ImageViewer()
{
}
