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
    for(int i=0; i < thumblist.count()/3; i++)
    {
        pixmaps.append(QPixmap::fromImage(MakeThumb(thumblist.at(3*i+2))));
        idlist.append(thumblist.at(3*i));
        addresslist.append(thumblist.at(3*i+1));
    }
}

void ImageViewer::UpdateGeometries()
{
    GetPixmaps();
    imagemodel = new ImageModel(pixmaps, idlist, addresslist);
    ui->listView->setModel(imagemodel);
    connect(ui->listView, SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(OpenImageWindow(const QModelIndex &)));
    connect(ui->listView, SIGNAL(clicked(const QModelIndex &)), this, SLOT(HighlightTreeViewItem(const QModelIndex &)));
}

void ImageViewer::OpenImageWindow(const QModelIndex &index)
{
    qDebug() << "open image in new window here." << index.row();
}

void ImageViewer::HighlightTreeViewItem(const QModelIndex &index)
{
    qDebug() << "select image in the treeview." << index.row();
}
