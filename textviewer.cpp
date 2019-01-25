#include "textviewer.h"

// Copyright 2015 Pasquale J. Rinaldi, Jr.
// Distrubted under the terms of the GNU General Public License version 2

TextViewer::TextViewer(QWidget* parent) : QDialog(parent), ui(new Ui::TextViewer)
{
    ui->setupUi(this);
    tskptr = &tskobj;
    tskptr->readimginfo = NULL;
    tskptr->readfsinfo = NULL;
    tskptr->readfileinfo = NULL;
    FindCodecs();
    ui->comboBox->clear();
    foreach(QTextCodec* codec, codecs)
        ui->comboBox->addItem(codec->name(), codec->mibEnum());
    //this->hide();
    connect(ui->comboBox, SIGNAL(activated(int)), this, SLOT(UpdateEncoding(int)));
}

TextViewer::~TextViewer()
{
    delete ui;
    this->close();
}

void TextViewer::HideClicked()
{
    //this->hide();
    //emit HideTextViewerWindow(false);
}

void TextViewer::ShowText(const QModelIndex &index)
{
    curindex = index;
    curobjaddr = index.sibling(index.row(), 0).data().toString().split("-f").at(1).toULongLong();
    //GetTextContent(index);
    UpdateEncoding(0);
    this->setWindowTitle(QString("Text Viewer - ") + QString(index.sibling(index.row(), 1).data().toString()));
    this->show();
}

void TextViewer::closeEvent(QCloseEvent* e)
{
    emit HideTextViewerWindow(false);
    e->accept();
}

void TextViewer::FindCodecs()
{
    QMap<QString, QTextCodec*> codecmap;
    QRegExp iso8859regex("ISO[- ]8859-([0-9]+).*");

    foreach(int mib, QTextCodec::availableMibs())
    {
        QTextCodec* codec = QTextCodec::codecForMib(mib);

        QString sortkey = codec->name().toUpper();
        int rank;

        if(sortkey.startsWith("UTF-8"))
            rank = 1;
        else if(sortkey.startsWith("UTF-16"))
            rank = 2;
        else if(iso8859regex.exactMatch(sortkey))
        {
            if(iso8859regex.cap(1).size() == 1)
                rank = 3;
            else
                rank = 4;
        }
        else
            rank = 5;
        sortkey.prepend(QChar('0' + rank));
        codecmap.insert(sortkey, codec);
    }
    codecs = codecmap.values();
}

void TextViewer::GetTextContent(const QModelIndex &index)
{
    QString tmpstr = "";
    QStringList evidlist;
    evidlist.clear();
    std::vector<std::string> pathvector;
    pathvector.clear();
    QString estring = index.sibling(index.row(), 0).data().toString().split("-", QString::SkipEmptyParts).at(0);
    QString vstring = index.sibling(index.row(), 0).data().toString().split("-", QString::SkipEmptyParts).at(1);
    QString pstring = index.sibling(index.row(), 0).data().toString().split("-", QString::SkipEmptyParts).at(2);
    QString fstring = index.sibling(index.row(), 0).data().toString().split("-", QString::SkipEmptyParts).at(3);
    //wombatvariable.selectedid = selectedindex.sibling(selectedindex.row(), 0).data().toString(); // mod object id
    //QDir eviddir = QDir(wombatvariable.tmpmntpath);
    //QStringList evidfiles = eviddir.entryList(QStringList("*.evid." + index.sibling(index.row(), 0).data().toString().split("-").at(0).mid(1)), QDir::NoSymLinks | QDir::Files);
    //wombatvariable.evidencename = evidfiles.at(0);
    QFile evidfile(wombatvariable.tmpmntpath + wombatvariable.evidencename + "." + estring + "/stat");
    //QFile evidfile(wombatvariable.tmpmntpath + wombatvariable.evidencename.split(".evid").at(0) + ".evid." + index.sibling(index.row(), 0).data().toString().split("-").at(0).mid(1));
    evidfile.open(QIODevice::ReadOnly);
    tmpstr = evidfile.readLine();
    evidlist = tmpstr.split(",");
    tskptr->partcount = evidlist.at(3).split("|").size();
    evidfile.close();
    for(int i = 0; i < evidlist.at(3).split("|").size(); i++)
        pathvector.push_back(evidlist.at(3).split("|").at(i).toStdString());
    tskptr->imagepartspath = (const char**)malloc(pathvector.size()*sizeof(char*));
    for(uint i = 0; i < pathvector.size(); i++)
        tskptr->imagepartspath[i] = pathvector[i].c_str();
    tskptr->readimginfo = tsk_img_open(tskptr->partcount, tskptr->imagepartspath, TSK_IMG_TYPE_DETECT, 0);
    if(tskptr->readimginfo == NULL)
    {
        qDebug() << tsk_error_get_errstr();
        //LogMessage("Image opening error");
    }
    free(tskptr->imagepartspath);
    tmpstr = "";
    QStringList partlist;
    partlist.clear();
    //QStringList partfiles = eviddir.entryList(QStringList(wombatvariable.evidencename.split(".evid").at(0) + ".part." + index.sibling(index.row(), 0).data().toString().split("-").at(2).mid(1)), QDir::NoSymLinks | QDir::Files);
    QFile partfile(wombatvariable.tmpmntpath + wombatvariable.evidencename + "." + estring + "/." + vstring + "/." + pstring + "/stat");
    //QFile partfile(wombatvariable.tmpmntpath + partfiles.at(0));
    partfile.open(QIODevice::ReadOnly);
    tmpstr = partfile.readLine();
    partfile.close();
    partlist = tmpstr.split(",");
    tskptr->readfsinfo = tsk_fs_open_img(tskptr->readimginfo, partlist.at(4).toULongLong(), TSK_FS_TYPE_DETECT);
    tskptr->readfileinfo = tsk_fs_file_open_meta(tskptr->readfsinfo, NULL, curobjaddr);
    if(tskptr->readfileinfo->meta != NULL)
    {
        if(tskptr->readfileinfo->meta->size > 2000000000) // 2 GB
        {
            qDebug() << "File is larger than 2GB. Export the file or use an external viewer. Otherwise showing 1st 2GB of text only.";
            // should show an alert, then display 1st 2gb of text and display right click menu???
        }
        char tbuffer[tskptr->readfileinfo->meta->size];
        // TEXTVIEWER FAILS FOR AN ADS WITH THE BELOW COMMAND
        ssize_t textlen = tsk_fs_file_read(tskptr->readfileinfo, 0, tbuffer, tskptr->readfileinfo->meta->size, TSK_FS_FILE_READ_FLAG_NONE);
        txtdata = QByteArray::fromRawData(tbuffer, textlen);
        //UpdateEncoding(0);
    }
}

void TextViewer::UpdateEncoding(int unused)
{
    if(unused < 0)
    {
        // remove warning
    }
    GetTextContent(curindex);
    int mib = ui->comboBox->itemData(ui->comboBox->currentIndex()).toInt();
    qDebug() << "combobox number:" << ui->comboBox->currentIndex();
    QTextCodec* codec = QTextCodec::codecForMib(mib);
    /*
    QTextStream in(&txtdata);
    in.setAutoDetectUnicode(false);
    in.setCodec(codec);
    decodedstring = in.readAll();
    */
    QTextCodec::ConverterState state;
    decodedstring = codec->toUnicode(txtdata.constData(), txtdata.size(), &state);
    ui->textEdit->setPlainText(decodedstring);
}
