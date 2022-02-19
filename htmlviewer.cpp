#include "htmlviewer.h"

// Copyright 2013-2022 Pasquale J. Rinaldi, Jr.
// Distrubted under the terms of the GNU General Public License version 2

HtmlViewer::HtmlViewer(QWidget* parent) : QDialog(parent), ui(new Ui::HtmlViewer)
{
    ui->setupUi(this);
    //pagehistory.clear();
    ui->homebutton->setVisible(false);
    //connect(ui->homebutton, SIGNAL(clicked()), this, SLOT(GoHome()));
    connect(ui->backbutton, SIGNAL(clicked()), ui->textbrowser, SLOT(backward()));
    connect(ui->forbutton, SIGNAL(clicked()), ui->textbrowser, SLOT(forward()));
    connect(ui->textbrowser, SIGNAL(sourceChanged(const QUrl&)), this, SLOT(SourceChanged(const QUrl&)));
    connect(ui->textbrowser, SIGNAL(backwardAvailable(bool)), ui->backbutton, SLOT(setEnabled(bool)));
    connect(ui->textbrowser, SIGNAL(forwardAvailable(bool)), ui->forbutton, SLOT(setEnabled(bool)));
    //homepage = wombatvariable.tmpmntpath + "previewreport.html";
    //qDebug() << "homepage:" << homepage;
}

HtmlViewer::~HtmlViewer()
{
    delete ui;
    this->close();
}

void HtmlViewer::SourceChanged(const QUrl &url)
{
    //pagehistory.append(url->
    //qDebug() << "url:" << url;
}

/*
void HtmlViewer::GoHome()
{
    //homepage = wombatvariable.tmpmntpath + "previewreport.html";
    //qDebug() << "go home:" << homepage;
    ui->textbrowser->setSource(QUrl::fromLocalFile(homepage));
    //Reload();
}
*/
/*
void HtmlViewer::GoBackward()
{
    ui->textbrowser->backward();
}

void HtmlViewer::GoForward()
{
    ui->textbrowser->forward();
}
*/

void HtmlViewer::HideClicked()
{
    this->close();
}

void HtmlViewer::ShowHtml(const QModelIndex &index)
{
    //ui->homebutton->setEnabled(false);
    QString oldfile = wombatvariable.tmpfilepath + index.sibling(index.row(), treenodemodel->GetColumnIndex("id")).data().toString() + "-fhex";
    //qDebug() << "oldfile:" << oldfile;
    //QString newfile = oldfile + ".html";
    //QFile::copy(oldfile, newfile);
    ui->textbrowser->setSource(QUrl::fromLocalFile(oldfile));
    //ui->webView->setUrl(QUrl::fromLocalFile(newfile));
    this->show();
}

void HtmlViewer::ShowArtifact(int artifactid, const QModelIndex &index)
{
    int nameindex = treenodemodel->GetColumnIndex("name");
    int idindex = treenodemodel->GetColumnIndex("id");
    QString htmlstring = "";
    if(artifactid == 0) // LNK
	htmlstring = ParseLnkArtifact(index.sibling(index.row(), nameindex).data().toString(), index.sibling(index.row(), idindex).data().toString());
    else if(artifactid == 1) // I30
	htmlstring = ParseI30Artifact(index.sibling(index.row(), nameindex).data().toString(), index.sibling(index.row(), idindex).data().toString());
    else if(artifactid == 2) // INFO2
	htmlstring = ParseInfo2Artifact(index.sibling(index.row(), nameindex).data().toString(), index.sibling(index.row(), idindex).data().toString());
    else if(artifactid == 3) // $I
	htmlstring = ParseIDollarArtifact(index.sibling(index.row(), nameindex).data().toString(), index.sibling(index.row(), idindex).data().toString());
    else if(artifactid == 4) // Prefetch
	htmlstring = ParsePrefetchArtifact(index.sibling(index.row(), nameindex).data().toString(), index.sibling(index.row(), idindex).data().toString());
    else if(artifactid == 5) // Archive
        htmlstring = ParseArchiveArtifact(index.sibling(index.row(), nameindex).data().toString(), index.sibling(index.row(), idindex).data().toString());
    ui->textbrowser->setHtml(htmlstring);
    //ui->webView->setHtml(htmlstring);
    this->show();
}

//void HtmlViewer::ShowPrefetch(const QModelIndex

void HtmlViewer::LoadHtml(QString filepath)
{
    //ui->homebutton->setEnabled(true);
    //qDebug() << "curpage:" << ui->textbrowser->source().path();
    //qDebug() << "nextpage:" << filepath;
    //qDebug() << "is backward available:" << ui->textbrowser->isBackwardAvailable();
    ui->textbrowser->setSource(QUrl::fromLocalFile(filepath));
    /*
    // populate html here
    ui->webView->setUrl(QUrl::fromLocalFile(filepath));
    */
    this->show();
}

void HtmlViewer::Reload()
{
    ui->textbrowser->reload();
    /*
    ui->webView->reload();
    */
}

void HtmlViewer::mousePressEvent(QMouseEvent* e)
{
    //qDebug() << "url history:" << ui->textbrowser->historyUrl(
    if(e->type() == QEvent::MouseButtonPress)
        isignals->ActivateReload();
}

void HtmlViewer::closeEvent(QCloseEvent* e)
{
    emit HideReportPreviewWindow(false);
    e->accept();
}
