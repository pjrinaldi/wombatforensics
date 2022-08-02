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
    this->show();
}
//docxviewer->LoadDocxFile(selectedindex.sibling(selectedindex.row(), colindex().data().toString(), selectedindex.sibling(selectedindex.row(), treenodemodel->GetColumnIndex("name")).data().toString());
/*
 *void RegistryDialog::LoadRegistryFile(QString regid, QString regname)
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
