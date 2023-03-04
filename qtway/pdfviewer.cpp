#include "pdfviewer.h"

// Copyright 2013-2020 Pasquale J. Rinaldi, Jr.
// Distrubted under the terms of the GNU General Public License version 2

PdfViewer::PdfViewer(QWidget* parent) : QDialog(parent), ui(new Ui::PdfViewer)
{
    ui->setupUi(this);
    pdfdoc = NULL;
    pdfpage = NULL;
    ui->pagelineedit->setText("0");
    ui->maxlineedit->setText("0");
    connect(ui->nextbutton, SIGNAL(clicked()), this, SLOT(NextPage()));
    connect(ui->prevbutton, SIGNAL(clicked()), this, SLOT(PrevPage()));
}

PdfViewer::~PdfViewer()
{
    delete pdfpage;
    delete pdfdoc;
    this->close();
}
void PdfViewer::mousePressEvent(QMouseEvent* e)
{
    if(e->type() == QEvent::MouseButtonPress)
    {
	this->close();
    }
}

void PdfViewer::ShowPdf(QString objectid)
{
    QString pdffilename = wombatvariable.tmpfilepath + objectid + "-fhex";
    pdfdoc = Poppler::Document::load(pdffilename);
    if(pdfdoc != NULL)
    {
    //if(!document || document == 0) { error };
    pagecount = pdfdoc->numPages();
    pdfpage = pdfdoc->page(0); // load initial page
    if(pdfpage != NULL)
    {
    pagenumber = 0;
    ui->pagelineedit->setText("1");
    ui->maxlineedit->setText(QString::number(pagecount));
    if(pagenumber == 0)
	ui->prevbutton->setEnabled(false);
    if(pagenumber == pagecount - 1)
	ui->nextbutton->setEnabled(false);
    QImage curimage = pdfpage->renderToImage();
    ui->label->setPixmap(QPixmap::fromImage(curimage));
    }
    else
	ui->label->setText("Invalid PDF and can't be displayed.");
    }
    else
	ui->label->setText("Invald PDF and can't be displayed.");
    this->show();
}

void PdfViewer::NextPage()
{
    if(pagenumber < pagecount - 1)
    {
	pdfpage = pdfdoc->page(pagenumber + 1);
	pagenumber++;
	QImage curimage = pdfpage->renderToImage();
	ui->label->setPixmap(QPixmap::fromImage(curimage));
	ui->pagelineedit->setText(QString::number(pagenumber + 1));
	ui->prevbutton->setEnabled(true);
	if(pagenumber == pagecount - 1)
	    ui->nextbutton->setEnabled(false);
    }
}

void PdfViewer::PrevPage()
{
    if(pagenumber > 0)
    {
	pdfpage = pdfdoc->page(pagenumber - 1);
	pagenumber--;
	QImage curimage = pdfpage->renderToImage();
	ui->label->setPixmap(QPixmap::fromImage(curimage));
	ui->pagelineedit->setText(QString::number(pagenumber + 1));
	ui->nextbutton->setEnabled(true);
	if(pagenumber == 0)
	    ui->prevbutton->setEnabled(false);
    }
}

/*
ImageWindow::ImageWindow(QWidget* parent) : QDialog(parent), ui(new Ui::ImageWindow)
{
    ui->setupUi(this);
    connect(ui->pushButton, SIGNAL(clicked(bool)), this, SLOT(SwitchScale(bool)));
}

void ImageWindow::SwitchScale(bool switchscale)
{
    ui->label->setScaledContents(switchscale);
}
ImageWindow::~ImageWindow()
{
    this->close();
}

void ImageWindow::ShowImage()
{
    this->show();
}

void ImageWindow::mousePressEvent(QMouseEvent* e)
{
    if(e->type() == QEvent::MouseButtonPress)
    {
        ui->label->clear();
        this->close();
    }
}

void ImageWindow::GetImage(QString objectid)
{
    QImage fileimage;
    bool imageloaded = fileimage.load(wombatvariable.tmpfilepath + objectid + "-fhex");
    if(imageloaded)
        ui->label->setPixmap(QPixmap::fromImage(fileimage));
    else
        ui->label->setPixmap(QPixmap::fromImage(QImage(":/bar/missingthumb")));
}

ImageViewer::ImageViewer(QWidget* parent) : QDialog(parent), ui(new Ui::ImageViewer)
{
    ui->setupUi(this);
    setWindowTitle("Image and Video Thumbnail Viewer");
    connect(ui->listWidget, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(OpenImageWindow(QListWidgetItem*)));
    connect(ui->listWidget, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(HighlightTreeViewItem(QListWidgetItem*)));
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

void ImageViewer::LoadThumbnails()
{
    ui->listWidget->clear();
    ui->listWidget->setIconSize(QSize(thumbsize * (100 / vidcount), thumbsize+20));
    QByteArray ba;
    ba.clear();
    QDir tdir = QDir(wombatvariable.tmpmntpath + "thumbs/");
    QStringList jpgfiles = tdir.entryList(QStringList("*.png"), QDir::NoSymLinks | QDir::Files);
    for(int i = 0; i < jpgfiles.count(); i++)
    {
        QListWidgetItem* tmpitem = new QListWidgetItem(QIcon(wombatvariable.tmpmntpath + "thumbs/" + jpgfiles.at(i)), jpgfiles.at(i).split(".").at(0), ui->listWidget);
        ba.clear();
        ba.append(imageshash.value(jpgfiles.at(i).split(".").at(0)));
        tmpitem->setToolTip(QString(QByteArray::fromBase64(ba)));
    }
    ui->label->setText("Thumbnail Count: " + QString::number(jpgfiles.count()));
}

void ImageViewer::OpenImageWindow(QListWidgetItem* item)
{
    ui->label->setText("Loading...");
    imagedialog = new ImageWindow();
    imagedialog->setModal(false);
    imagedialog->setAttribute(Qt::WA_DeleteOnClose);
    imagedialog->setWindowTitle(item->text() + " Image Viewer");
    QImage iconimage(QString(wombatvariable.tmpmntpath + "thumbs/" + item->text() + ".png"));
    if(iconimage.width() == thumbsize)
    {
        imagedialog->GetImage(item->text());
        imagedialog->show();
    }
    else if(iconimage.width() > thumbsize)
    {
        QModelIndexList indexlist = treenodemodel->match(treenodemodel->index(0, 11, QModelIndex()), Qt::DisplayRole, QVariant(item->text()), -1, Qt::MatchFlags(Qt::MatchExactly | Qt::MatchRecursive));
        if(indexlist.count() > 0)
        {
            videowindow = new VideoViewer();
            videowindow->setAttribute(Qt::WA_DeleteOnClose);
            videowindow->setWindowIcon(QIcon(":/vid"));
            videowindow->setWindowTitle(item->text() + "Video Viewer");
            videowindow->setWindowTitle(indexlist.first().sibling(indexlist.first().row(), 11).data().toString() + " Video Viewer");
            videowindow->ShowVideo(indexlist.first());
        }
    }
    ui->label->setText("");
}

void ImageViewer::HighlightTreeViewItem(QListWidgetItem* item)
{
    //qDebug() << "image item:" << item->text();
    emit SendObjectToTreeView(item->text());
}
*/
