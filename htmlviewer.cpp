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
    //curobjaddr = index.sibling(index.row(), 11).data().toString().toLongLong();
    QString oldfile = wombatvariable.tmpfilepath + index.sibling(index.row(), 11).data().toString() + "-fhex";
    QString newfile = oldfile + ".html";
    QFile::copy(oldfile, newfile);
    ui->webView->setUrl(QUrl::fromLocalFile(newfile));
    this->show();
}

void HtmlViewer::ShowLnk(const QModelIndex &index)
{
    qDebug() << "parse content here... and display as html...";
    QString lnkfile = wombatvariable.tmpfilepath + index.sibling(index.row(), 11).data().toString() + "-fhex";
    liblnk_error_t* error = NULL;
    liblnk_file_t* lnkobj = NULL;
    liblnk_file_initialize(&lnkobj, &error);
    liblnk_file_open(lnkobj, lnkfile.toStdString().c_str(), liblnk_get_access_flags_read(), &error);
    if(liblnk_check_file_signature(lnkfile.toStdString().c_str(), &error))
    {
        if(liblnk_file_link_refers_to_file(lnkobj, &error))
        {
            uint64_t crtime;
            liblnk_file_get_file_creation_time(lnkobj, &crtime, &error);
	    qDebug() << "crtime:" << crtime;
            std::string crtimestring = ConvertWindowsTimeToUnixTime(crtime);
            qDebug() << "crtimestr:" << QString::fromStdString(crtimestring);
            // something in the time function is wrong and it is using current time rather than the time from the lnk file...
            // otherwise it works.
        }
    }
    liblnk_file_close(lnkobj, &error);
    liblnk_file_free(&lnkobj, &error);
    this->show();
    //curobjaddr = index.sibling(index.row(), 11).data().toString().toULongLong()
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
