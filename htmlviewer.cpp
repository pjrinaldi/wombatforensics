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
    ui->webView->setUrl(QUrl::fromLocalFile(newfile));
    this->show();
}

void HtmlViewer::ShowLnk(const QModelIndex &index)
{
    QString htmlstring = ParseLnkArtifact(index.sibling(index.row(), 0).data().toString(), index.sibling(index.row(), 11).data().toString());
    ui->webView->setHtml(htmlstring);
    this->show();
}

void HtmlViewer::ShowI30(const QModelIndex &index)
{
    QString htmlstring = ParseI30Artifact(index.sibling(index.row(), 0).data().toString(), index.sibling(index.row(), 11).data().toString());
    ui->webView->setHtml(htmlstring);
    this->show();
}

void HtmlViewer::LoadHtml(QString filepath)
{
    // populate html here
    ui->webView->setUrl(QUrl::fromLocalFile(filepath));
    this->show();
}

void HtmlViewer::Reload()
{
    ui->webView->reload();
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
