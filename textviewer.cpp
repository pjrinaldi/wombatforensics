#include "textviewer.h"

// Copyright 2013-2019 Pasquale J. Rinaldi, Jr.
// Distrubted under the terms of the GNU General Public License version 2

TextViewer::TextViewer(QWidget* parent) : QDialog(parent), ui(new Ui::TextViewer)
{
    ui->setupUi(this);
    tskptr = &tskobj;
    tskptr->readimginfo = NULL;
    tskptr->readfsinfo = NULL;
    tskptr->readfileinfo = NULL;
    decodedstring = "default";
    qDebug() << "decodedstring at start:" << decodedstring;
    //FindCodecs();
    ui->comboBox->clear();
    //foreach(QTextCodec* codec, codecs)
    //    ui->comboBox->addItem(codec->name(), codec->mibEnum());
    //connect(ui->comboBox, SIGNAL(activated(int)), this, SLOT(UpdateEncoding(int)));
}

TextViewer::~TextViewer()
{
    delete ui;
    this->close();
}

void TextViewer::HideClicked()
{
}

void TextViewer::ShowText(const QModelIndex &index)
{
    //curindex = index;
    //curobjaddr = index.sibling(index.row(), 10).data().toString().split("-f").at(1).toLongLong();
    //UpdateEncoding(0);
    GetTextContent(index);
    ui->textEdit->setPlainText(decodedstring);
    this->setWindowTitle(QString("Text Viewer - ") + QString(index.sibling(index.row(), 10).data().toString()));
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
    QDir eviddir = QDir(wombatvariable.tmpmntpath);
    QStringList evidfiles = eviddir.entryList(QStringList("*." + index.sibling(index.row(), 10).data().toString().split("-").at(0)), QDir::NoSymLinks | QDir::Dirs);
    QString evidencename = evidfiles.at(0).split(".e").first();
    QString tmpstr = "";
    QStringList evidlist;
    evidlist.clear();
    std::vector<std::string> pathvector;
    pathvector.clear();
    QString paridstr = index.parent().sibling(index.parent().row(), 10).data().toString().split("-f").last();
    QString estring = index.sibling(index.row(), 10).data().toString().split("-", QString::SkipEmptyParts).at(0);
    QString vstring = index.sibling(index.row(), 10).data().toString().split("-", QString::SkipEmptyParts).at(1);
    QString pstring = index.sibling(index.row(), 10).data().toString().split("-", QString::SkipEmptyParts).at(2);
    QString fstring = index.sibling(index.row(), 10).data().toString().split("-", QString::SkipEmptyParts).at(3);
    QFile evidfile(wombatvariable.tmpmntpath + evidencename + "." + estring + "/stat");
    evidfile.open(QIODevice::ReadOnly);
    if(evidfile.isOpen())
        tmpstr = evidfile.readLine();
    evidfile.close();
    evidlist = tmpstr.split(",");
    tskptr->partcount = evidlist.at(3).split("|").size();
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
    QFile partfile(wombatvariable.tmpmntpath + evidencename + "." + estring + "/" + vstring + "/" + pstring + "/stat");
    partfile.open(QIODevice::ReadOnly);
    tmpstr = partfile.readLine();
    partfile.close();
    partlist = tmpstr.split(",");
    tskptr->readfsinfo = tsk_fs_open_img(tskptr->readimginfo, partlist.at(4).toLongLong(), TSK_FS_TYPE_DETECT);
    QString tmpfilename = "";
    if(fstring.mid(1).contains(":"))
        tmpfilename = fstring.mid(1).split(":").at(0) + QString("-") + fstring.mid(1).split(":").at(1);
    else
        tmpfilename = fstring.mid(1);
    qDebug() << "f value:" << tmpfilename;
    if(tmpfilename.contains("-"))
        tskptr->readfileinfo = tsk_fs_file_open_meta(tskptr->readfsinfo, NULL, tmpfilename.split("-").at(0).toInt());
    else
        tskptr->readfileinfo = tsk_fs_file_open_meta(tskptr->readfsinfo, NULL, tmpfilename.toInt());
    if(tskptr->readfileinfo->meta != NULL)
    {
        /*
        if(tskptr->readfileinfo->meta->size > 2000000000) // 2 GB
        {
            qDebug() << "File is larger than 2GB. Export the file or use an external viewer. Otherwise showing 1st 2GB of text only.";
            // should show an alert, then display 1st 2gb of text and display right click menu???
        }
        */
        QFile filefile;
        if(fstring.split(":").count() > 1)
            filefile.setFileName(wombatvariable.tmpmntpath + evidencename + "." + estring + "/" + vstring + "/" + pstring + "/" + fstring.split(":").first() + "-" + fstring.split(":").last() + ".a" + paridstr + ".stat");
        else
            filefile.setFileName(wombatvariable.tmpmntpath + evidencename + "." + estring + "/" + vstring + "/" + pstring + "/" + fstring.split(":").first() + ".a" + paridstr + ".stat");
        filefile.open(QIODevice::ReadOnly | QIODevice::Text);
        if(filefile.isOpen())
            tmpstr = filefile.readLine();
        filefile.close();
        QStringList filelist;
        filelist = tmpstr.split(",");
        char* tbuffer;
        ssize_t textlen = 0;
        if(partlist.at(0).toInt() == TSK_FS_TYPE_NTFS_DETECT) // IF NTFS (ADS/FILE/DIR/RES/NONRES
        {
            if(fstring.split(":").count() > 1) // IF ADS
            {
                if(tskptr->readfileinfo->meta != NULL)
                {
                    tbuffer = new char[filelist.at(8).toULongLong()];
                    textlen = tsk_fs_file_read_type(tskptr->readfileinfo, TSK_FS_ATTR_TYPE_NTFS_DATA, fstring.split(":").at(1).toInt(), 0, tbuffer, filelist.at(8).toULongLong(), TSK_FS_FILE_READ_FLAG_NONE);
                    if(textlen == -1)
                        qDebug() << tsk_error_get_errstr();
                }
            }
            else // IF NOT ADS
            {
                tbuffer = new char[tskptr->readfileinfo->meta->size];
                textlen = tsk_fs_file_read(tskptr->readfileinfo, 0, tbuffer, tskptr->readfileinfo->meta->size, TSK_FS_FILE_READ_FLAG_NONE);
            }
        }
        else // OTHER FILE SYSTEM
        {
            if(tskptr->readfileinfo->meta != NULL)
            {
                tbuffer = new char[tskptr->readfileinfo->meta->size];
                textlen = tsk_fs_file_read(tskptr->readfileinfo, 0, tbuffer, tskptr->readfileinfo->meta->size, TSK_FS_FILE_READ_FLAG_NONE);
            }
        }
        decodedstring = QString(tbuffer);
        delete[] tbuffer;
        tsk_fs_file_close(tskptr->readfileinfo);
        tsk_fs_close(tskptr->readfsinfo);
        tsk_img_close(tskptr->readimginfo);
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
    //int mib = ui->comboBox->itemData(ui->comboBox->currentIndex()).toInt();
    //qDebug() << "combobox number:" << ui->comboBox->currentIndex();
    //QTextCodec* codec = QTextCodec::codecForMib(mib);
    //QTextCodec::ConverterState state;
    //decodedstring = codec->toUnicode(txtdata.constData(), txtdata.size(), &state);
    //ui->textEdit->setPlainText(decodedstring);
    //ui->textEdit->setPlainText(QString(txtdata));
}
