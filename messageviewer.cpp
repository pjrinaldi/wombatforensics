#include "messageviewer.h"

MessageViewer::MessageViewer(QWidget* parent) : QDialog(parent), ui(new Ui::MessageViewer)
{
    ui->setupUi(this);
    msglog = ui->textEdit;
    //tskptr = &tskobj;
    //tskptr->readimginfo = NULL;
    //tskptr->readfsinfo = NULL;
    //tskptr->readfileinfo = NULL;
    //FindCodecs();
    //ui->comboBox->clear();
    //foreach(QTextCodec* codec, codecs)
    //    ui->comboBox->addItem(codec->name(), codec->mibEnum());
    this->hide();
    connect(ui->checkBox, SIGNAL(stateChanged(int)), this, SLOT(UpdateMessages(int)));
    //connect(ui->comboBox, SIGNAL(activated(int)), this, SLOT(GetTextContent()));
    //connect(ui->comboBox, SIGNAL(activated(int)), this, SLOT(UpdateEncoding()));
}

MessageViewer::~MessageViewer()
{
    delete ui;
    this->close();
}

void MessageViewer::HideClicked()
{
    this->hide();
    emit HideMessageViewerWindow(false);
}

void MessageViewer::UpdateMessages(int chkstate)
{
}
/*
void MessageViewer::ShowText(const QModelIndex &index)
{
    curobjid = index.sibling(index.row(), 0).data().toInt();
    GetTextContent();
    this->show();
}
*/
void MessageViewer::closeEvent(QCloseEvent* e)
{
    emit HideMessageViewerWindow(false);
    e->accept();
}
/*
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

void TextViewer::GetTextContent()
{
    //this->setWindowTitle("View Text - "); // populate filename here.
    // OpenParentImage
    std::vector<std::string> pathvector;
    int imgid = 0;
    int fsid = 0;
    int fsoffset = 0;
    int address = 0;
    pathvector.clear();
    QSqlQuery pimgquery(fcasedb);
    pimgquery.prepare("SELECT parimgid, parfsid, address FROM Data WHERE objectid = ?;");
    pimgquery.addBindValue(curobjid);
    pimgquery.exec();
    pimgquery.next();
    imgid = pimgquery.value(0).toInt();
    fsid = pimgquery.value(1).toInt();
    address = pimgquery.value(2).toInt();
    pimgquery.finish();
    pimgquery.prepare("SELECT fullpath FROM dataruns WHERE objectid = ? ORDER BY seqnum;");
    pimgquery.addBindValue(imgid);
    if(pimgquery.exec())
    {
        while(pimgquery.next())
        {
            pathvector.push_back(pimgquery.value(0).toString().toStdString());
        }
    }
    pimgquery.finish();
    tskptr->imagepartspath = (const char**)malloc(pathvector.size()*sizeof(char*));
    for(uint i=0; i < pathvector.size(); i++)
    {
        tskptr->imagepartspath[i] = pathvector.at(i).c_str();
    }
    tskptr->readimginfo = tsk_img_open(pathvector.size(), tskptr->imagepartspath, TSK_IMG_TYPE_DETECT, 0);
    free(tskptr->imagepartspath);
    // OpenParentFileSystem
    pimgquery.prepare("SELECT byteoffset FROM data where objectid = ?;");
    pimgquery.addBindValue(fsid);
    pimgquery.exec();
    pimgquery.next();
    fsoffset = pimgquery.value(0).toInt();
    pimgquery.finish();
    tskptr->readfsinfo = tsk_fs_open_img(tskptr->readimginfo, fsoffset, TSK_FS_TYPE_DETECT);
    // OpenFile
    tskptr->readfileinfo = tsk_fs_file_open_meta(tskptr->readfsinfo, NULL, address);
    // ReadFileToEncodedTextUsingByteArray
    if(tskptr->readfileinfo->meta != NULL)
    {
        if(tskptr->readfileinfo->meta->size > 2000000000) // 2 GB
        {
            qDebug() << "File is larger than 2GB. Export the file or use an external viewer. Otherwise showing 1st 2GB of text only.";
        }
        char tbuffer[tskptr->readfileinfo->meta->size];
        ssize_t textlen = tsk_fs_file_read(tskptr->readfileinfo, 0, tbuffer, tskptr->readfileinfo->meta->size, TSK_FS_FILE_READ_FLAG_NONE);
        txtdata = QByteArray::fromRawData(tbuffer, textlen);
        UpdateEncoding();
    }
}

void TextViewer::UpdateEncoding()
{
    int mib = ui->comboBox->itemData(ui->comboBox->currentIndex()).toInt();
    QTextCodec* codec = QTextCodec::codecForMib(mib);

    QTextStream in(&txtdata);
    in.setAutoDetectUnicode(false);
    in.setCodec(codec);
    decodedstring = in.readAll();
    ui->textEdit->setPlainText(decodedstring);
}
*/
