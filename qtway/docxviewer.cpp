#include "docxviewer.h"

// Copyright 2013-2022 Pasquale J. Rinaldi, Jr.
// Distributed under the terms of the GNU General Public License version 2

DocxViewer::DocxViewer(QWidget* parent) : QDialog(parent), ui(new Ui::DocxViewer)
{
    ui->setupUi(this);
    /*
    //pagehistory.clear();
    //ui->homebutton->setVisible(false);
    //connect(ui->homebutton, SIGNAL(clicked()), this, SLOT(GoHome()));
    connect(ui->backbutton, SIGNAL(clicked()), ui->textbrowser, SLOT(backward()));
    connect(ui->forbutton, SIGNAL(clicked()), ui->textbrowser, SLOT(forward()));
    connect(ui->textbrowser, SIGNAL(sourceChanged(const QUrl&)), this, SLOT(SourceChanged(const QUrl&)));
    connect(ui->textbrowser, SIGNAL(backwardAvailable(bool)), ui->backbutton, SLOT(setEnabled(bool)));
    connect(ui->textbrowser, SIGNAL(forwardAvailable(bool)), ui->forbutton, SLOT(setEnabled(bool)));
    //homepage = wombatvariable.tmpmntpath + "previewreport.html";
    //qDebug() << "homepage:" << homepage;
    */
}

DocxViewer::~DocxViewer()
{
    delete ui;
    this->close();
}

//void DocxViewer::LoadDocumentXml(QString xmlid, QString xmlname)
void DocxViewer::LoadDocumentXml(QString xmlid, QString xmlname, QString xmlcontent)
{
    QString docxstr = "<html><body style='" + ReturnCssString(0) + "'>";
    //qDebug() << "xmlid:" << xmlid << "xml name:" << xmlname;
    int err = 0;
    int zipfileid = 0;
    QString zipfilename = "";
    zip_uint64_t zipfilesize = 0;
    QString docxfilestr = wombatvariable.tmpfilepath + xmlid + "-fhex";
    zip* curzip = zip_open(docxfilestr.toStdString().c_str(), ZIP_RDONLY, &err);
    qint64 zipentrycnt = zip_get_num_entries(curzip, 0);
    struct zip_stat zipstat;
    zip_stat_init(&zipstat);
    for(int i=0; i < zipentrycnt; i++)
    {
        zip_stat_index(curzip, i, 0, &zipstat);
        //if(zipstat.name = "document.xml")
        if(zipstat.name == xmlcontent)
        {
            zipfileid = i;
            zipfilename = zipstat.name;
            zipfilesize = zipstat.size;
        }
    }
    //qDebug() << "zipfileid:" << zipfileid << "zipfilename:" << zipfilename << "zipfilesize:" << zipfilesize;
    //zip_file_t* docxml = NULL;
    zip_file_t* docxml = zip_fopen_index(curzip, zipfileid, ZIP_FL_UNCHANGED);
    char zipbuf[zipfilesize];
    //void* zipbuf = NULL;
    zip_int64_t bytesread = zip_fread(docxml, zipbuf, zipfilesize);
    err = zip_fclose(docxml);
    //qDebug() << "bytesread:" << bytesread;
    QByteArray filearray = QByteArray::fromRawData(zipbuf, zipfilesize);
    //qDebug() << "document.xml:" << filearray.left(5);
    QXmlStreamReader xmlreader;
    xmlreader.addData(filearray);
    while(!xmlreader.atEnd())
    {
        xmlreader.readNext();
        if(xmlreader.isCharacters() || xmlreader.isDTD() || xmlreader.isEntityReference() || xmlreader.isComment())
        {
            docxstr += "<p>";
            docxstr += xmlreader.text();
            //docxstr += "\n";
            docxstr += "</p>";
            //qDebug() << "Text:" << xmlreader.text();
        }
    }
    xmlreader.clear();
    docxstr += "</body></html>";
    ui->textbrowser->setHtml(docxstr);
    this->show();
}

void DocxViewer::SourceChanged(const QUrl &url)
{
    //pagehistory.append(url->
    //qDebug() << "url:" << url;
}

/*
void DocxViewer::GoHome()
{
    //homepage = wombatvariable.tmpmntpath + "previewreport.html";
    //qDebug() << "go home:" << homepage;
    ui->textbrowser->setSource(QUrl::fromLocalFile(homepage));
    //Reload();
}
*/
/*
void DocxViewer::GoBackward()
{
    ui->textbrowser->backward();
}

void DocxViewer::GoForward()
{
    ui->textbrowser->forward();
}
*/

void DocxViewer::HideClicked()
{
    this->close();
}

void DocxViewer::ShowHtml(const QModelIndex &index)
{
    //ui->homebutton->setEnabled(false);
    //QString oldfile = wombatvariable.tmpfilepath + index.sibling(index.row(), treenodemodel->GetColumnIndex("id")).data().toString() + "-fhex";
    //qDebug() << "oldfile:" << oldfile;
    //QString newfile = oldfile + ".html";
    //QFile::copy(oldfile, newfile);
    //ui->textbrowser->setSource(QUrl::fromLocalFile(oldfile));
    //ui->webView->setUrl(QUrl::fromLocalFile(newfile));
    this->show();
}

void DocxViewer::ShowArtifact(int artifactid, const QModelIndex &index)
{
    /*
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
    */
}

//void DocxViewer::ShowPrefetch(const QModelIndex

void DocxViewer::LoadHtml(QString filepath)
{
    //ui->homebutton->setEnabled(true);
    //qDebug() << "curpage:" << ui->textbrowser->source().path();
    //qDebug() << "nextpage:" << filepath;
    //qDebug() << "is backward available:" << ui->textbrowser->isBackwardAvailable();
    //ui->textbrowser->setSource(QUrl::fromLocalFile(filepath));
    /*
    // populate html here
    ui->webView->setUrl(QUrl::fromLocalFile(filepath));
    */
    //this->show();
}

void DocxViewer::Reload()
{
    //ui->textbrowser->reload();
    /*
    ui->webView->reload();
    */
}

void DocxViewer::mousePressEvent(QMouseEvent* e)
{
    //qDebug() << "url history:" << ui->textbrowser->historyUrl(
    //if(e->type() == QEvent::MouseButtonPress)
    //    isignals->ActivateReload();
}

void DocxViewer::closeEvent(QCloseEvent* e)
{
    //emit HideReportPreviewWindow(false);
    //e->accept();
}