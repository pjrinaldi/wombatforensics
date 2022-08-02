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

void DocxViewer::LoadDocumentXml(QString xmlid, QString xmlname)
{
    qDebug() << "xmlid:" << xmlid << "xml name:" << xmlname;
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
        if(zipstat.name = "document.xml")
        {
            zipfileid = i;
            zipfilename = zipstat.name;
            zipfilesize = zipstat.size;
            /*
            zip_file_t* docxml = NULL;
            docxml = zip_fopen_index(curzip, i, ZIP_FL_UNCHANGED);
            void* zipbuf = NULL;
            int64_t bytesread = zip_fread(docxml, zipbuf, zipstat.size);
            //err = zip_fseek(0)
            zip_fclose(docxml);
            if(bytesread == zipstat.size)
            {
                qDebug() << "buf has file content";
            }
            else
                qDebug() << "bytes read:" << bytesread << "zipstat.size:" << zipstat.size;
            qDebug() << "found document.xml, need to parse into plain/markdown/html";
            */
        }
    }
    qDebug() << "zipfileid:" << zipfileid << "zipfilename:" << zipfilename << "zipfilesize:" << zipfilesize;
    //zip_file_t* docxml = NULL;
    zip_file_t* docxml = zip_fopen_index(curzip, zipfileid, ZIP_FL_UNCHANGED);
    char zipbuf[zipfilesize];
    //void* zipbuf = NULL;
    zip_int64_t bytesread = zip_fread(docxml, zipbuf, zipfilesize);
    err = zip_fclose(docxml);
    //qDebug() << "bytesread:" << bytesread;
    QByteArray filearray = QByteArray::fromRawData(zipbuf, zipfilesize);
    qDebug() << "document.xml:" << filearray.left(5);
    this->show();
}
/*
    QString archivefilestr = wombatvariable.tmpfilepath + archiveid + "-fhex";
    zip* curzip = zip_open(archivefilestr.toStdString().c_str(), ZIP_RDONLY, &err);
    qint64 zipentrycount = zip_get_num_entries(curzip, 0);
    for(int i=0; i < zipentrycount; i++)
    {
        struct zip_stat zipstat;
        zip_stat_init(&zipstat);
        zip_stat_index(curzip, i, 0, &zipstat);
        htmlstr += "<tr style='";
        if(i % 2 == 0)
            htmlstr += ReturnCssString(4);
        else
            htmlstr += ReturnCssString(5);
        time_t modtime = zipstat.mtime;
        uint64_t temp = (uint64_t)modtime;
        temp = temp + EPOCH_DIFFERENCE;
        temp = temp * TICKS_PER_SECOND;
        htmlstr += "'><td style='" + ReturnCssString(7) + "'>" + QString::fromStdString(std::string(zipstat.name)) + "</td><td style='" + ReturnCssString(7) + "'>" + QString::number(zipstat.size) + "</td><td style='" + ReturnCssString(7) + "'>" + QString::number(zipstat.comp_size) + "</td><td style='" + ReturnCssString(7) + "'>" + ConvertWindowsTimeToUnixTime(temp) + "</td><td style='" + ReturnCssString(7) + "'>";
        htmlstr += "</td><td style='" + ReturnCssString(7) + "'>";
    }
    htmlstr += "</table></body></html>";

    return htmlstr;

void RegistryDialog::LoadRegistryFile(QString regid, QString regname)
{
    libregf_file_t* regfile = NULL;
    libregf_error_t* regerr = NULL;
    libregf_file_initialize(&regfile, &regerr);
    QString regfilestr = wombatvariable.tmpfilepath + regid + "-fhex";
    regfilepath = regfilestr;
    libregf_file_open(regfile, regfilestr.toStdString().c_str(), LIBREGF_OPEN_READ, &regerr);
    libregf_error_fprint(regerr, stderr);
    libregf_key_t* rootkey = NULL;
    libregf_file_get_root_key(regfile, &rootkey, &regerr);
    libregf_error_fprint(regerr, stderr);
    int rootsubkeycnt = 0;
    libregf_key_get_number_of_sub_keys(rootkey, &rootsubkeycnt, &regerr);
    libregf_error_fprint(regerr, stderr);
        QTreeWidgetItem* rootitem = new QTreeWidgetItem(ui->treeWidget);
    rootitem->setText(0, regname.toUpper());
    ui->treeWidget->addTopLevelItem(rootitem);
    PopulateChildKeys(rootkey, rootitem, regerr);
    ui->treeWidget->expandItem(rootitem);
    libregf_key_free(&rootkey, &regerr);
    libregf_file_close(regfile, &regerr);
    libregf_file_free(&regfile, &regerr);
    libregf_error_free(&regerr);
    this->show();
}

 */ 

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
