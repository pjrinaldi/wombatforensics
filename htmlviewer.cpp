#include "htmlviewer.h"

// Copyright 2013-2019 Pasquale J. Rinaldi, Jr.
// Distrubted under the terms of the GNU General Public License version 2

HtmlViewer::HtmlViewer(QWidget* parent) : QDialog(parent), ui(new Ui::HtmlViewer)
{
    ui->setupUi(this);
}

HtmlViewer::~HtmlViewer()
{
    delete ui;
    this->close();
}

void HtmlViewer::HideClicked()
{
    this->close();
}

void HtmlViewer::ShowHtml(const QModelIndex &index)
{
    QString oldfile = wombatvariable.tmpfilepath + index.sibling(index.row(), 11).data().toString() + "-fhex";
    QString newfile = oldfile + ".html";
    QFile::copy(oldfile, newfile);
    ui->textbrowser->setSource(QUrl::fromLocalFile(newfile));
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
    if(e->type() == QEvent::MouseButtonPress)
        isignals->ActivateReload();
}

void HtmlViewer::closeEvent(QCloseEvent* e)
{
    emit HideReportPreviewWindow(false);
    e->accept();
}
