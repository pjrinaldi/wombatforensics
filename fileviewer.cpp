#include "fileviewer.h"

// Copyright 2015 Pasquale J. Rinaldi, Jr.
// Distrubted under the terms of the GNU General Public License version 2

FileViewer::FileViewer(QWidget* parent) : QMainWindow(parent), ui(new Ui::FileViewer)
{
    ui->setupUi(this);
    connect(ui->filehexview, SIGNAL(currentAddressChanged(qint64)), this, SLOT(SetOffsetLabel(qint64)));
    connect(ui->filehexview, SIGNAL(selectionChanged()), this, SLOT(UpdateSelectValue()));
    this->statusBar()->setSizeGripEnabled(true);
    jumpto = new QLineEdit();
    jumplabel = new QLabel("Jump To:");
    jumpbutton = new QPushButton("Go");
    jumpto->setPlaceholderText("Enter Hex Offset");
    ui->toolBar->addWidget(jumplabel);
    ui->toolBar->addWidget(jumpto);
    ui->toolBar->addWidget(jumpbutton);
    selectedoffset = new QLabel(this);
    selectedoffset->setText("Offset: 00");
    selectedhex = new QLabel(this);
    selectedhex->setText("Length: 0");
    this->statusBar()->addWidget(selectedoffset, 0);
    this->statusBar()->addWidget(selectedhex, 0);
    connect(jumpbutton, SIGNAL(clicked()), this, SLOT(JumpHex()));
}

FileViewer::~FileViewer()
{
    this->close();
}

void FileViewer::HideClicked()
{
    this->hide();
    emit HideFileViewer(false);
}

void FileViewer::closeEvent(QCloseEvent* event)
{
    emit HideFileViewer(false);
    event->accept();
}

void FileViewer::SetOffsetLabel(qint64 pos)
{
    QString label;
    label = "Offset: ";
    char buffer[64];
    #if _LARGEFILE_SOURCE
    sprintf(buffer,"0x%llx",pos);
    #else
    sprintf(buffer,"0x%x",pos);
    #endif
    label += buffer;
    selectedoffset->setText(label);
}

void FileViewer::UpdateSelectValue()
{
    QByteArray selectionbytes = ui->filehexview->selectionToByteArray();
    QString tmptext = "Length: " + QString::number(selectionbytes.size());
    selectedhex->setText(tmptext);
}

void FileViewer::UpdateHexView(const QModelIndex curindex)
{
    //qDebug() << "fileviewer:" << curindex.sibling(curindex.row(), 10).data().toString();
    QString paridstr = curindex.parent().sibling(curindex.parent().row(), 10).data().toString().split("-f").last();
    TSK_IMG_INFO* fileheximginfo;
    TSK_FS_INFO* filehexfsinfo;
    TSK_FS_FILE* filehexfileinfo;
    std::vector<std::string> pathvector;
    const TSK_TCHAR** imagepartspath;
    pathvector.clear();
    QString estring = curindex.sibling(curindex.row(), 10).data().toString().split("-").at(0);
    QString vstring = curindex.sibling(curindex.row(), 10).data().toString().split("-").at(1);
    QString pstring = curindex.sibling(curindex.row(), 10).data().toString().split("-").at(2);
    QString fstring = curindex.sibling(curindex.row(), 10).data().toString().split("-").at(3);
    //qDebug() << "Initial string info (e-v-p-f):" << estring + vstring + pstring + fstring;
    /*
        QFile evidfile(wombatvariable.tmpmntpath + wombatvariable.evidencename + "." + estring + "/stat");
        evidfile.open(QIODevice::ReadOnly);
        tmpstr = evidfile.readLine();
        int partcount = tmpstr.split(",").at(3).split("|").size();
        evidfile.close();
        for(int i=0; i < partcount; i++)
            pathvector.push_back(tmpstr.split(",").at(3).split("|").at(i).toStdString());
        imagepartspath = (const char**)malloc(pathvector.size()*sizeof(char*));
        for(uint i=0; i < pathvector.size(); i++)
            imagepartspath[i] = pathvector[i].c_str();
        fileheximginfo = tsk_img_open(partcount, imagepartspath, TSK_IMG_TYPE_DETECT, 0);
        if(fileheximginfo == NULL)
        {
            qDebug() << tsk_error_get_errstr();
            //LogMessage("Image opening error");
        }
        free(imagepartspath);

        tmpstr = "";
        QStringList partlist;
        partlist.clear();
        QFile partfile(wombatvariable.tmpmntpath + wombatvariable.evidencename + "." + estring + "/." + vstring + "/." + pstring + "/stat");
        partfile.open(QIODevice::ReadOnly);
        tmpstr = partfile.readLine();
        partfile.close();
        partlist = tmpstr.split(",");
        // FILE SYSTEM PIECE OF FILE HEX CONTENT CODE
        filehexfsinfo = tsk_fs_open_img(fileheximginfo, partlist.at(4).toULongLong(), TSK_FS_TYPE_DETECT);
        unsigned long long fsoffset = partlist.at(4).toULongLong();
        unsigned long long rootinum = filehexfsinfo->root_inum;
        if(paridstr.contains("-"))
            paridstr = QString::number(rootinum);
        QFile partpropfile(wombatvariable.tmpmntpath + wombatvariable.evidencename + "." + estring + "/." + vstring + "/." + pstring + "/prop");
        partpropfile.open(QIODevice::ReadOnly);
        QString mftentryoffset = "";
        while(!partpropfile.atEnd())
        {
            QString tmpstring = partpropfile.readLine();
            if(tmpstring.contains("MFT Starting Byte Address"))
                mftentryoffset = tmpstring.split("||").at(1);
        }
        partpropfile.close();
        //qDebug() << "mftentryoffset:" << mftentryoffset;
        
        QStringList filelist;
        filelist.clear();
        QString tmpfilename = "";
        if(wombatvariable.selectedid.split("-").at(3).mid(1).contains(":"))
            tmpfilename = wombatvariable.selectedid.split("-").at(3).mid(1).split(":").at(0) + QString("-") + wombatvariable.selectedid.split("-").at(3).mid(1).split(":").at(1);
        else
            tmpfilename = wombatvariable.selectedid.split("-").at(3).mid(1);
        //qDebug() << "f value:" << tmpfilename;
        // INITIAL FILE SYSTEM FILE PIECE OF FILE HEX CONTENT CODE
        if(tmpfilename.contains("-"))
            filehexfileinfo = tsk_fs_file_open_meta(filehexfsinfo, NULL, tmpfilename.split("-").at(0).toInt());
        else
            filehexfileinfo = tsk_fs_file_open_meta(filehexfsinfo, NULL, tmpfilename.toInt());
        char* fhexbuf;
        ssize_t fhexlen = 0;

        QFile filefile;
        QFile filefileprop;
        if(fstring.split(":").count() > 1)
            filefile.setFileName(wombatvariable.tmpmntpath + wombatvariable.evidencename + "." + estring + "/." + vstring + "/." + pstring + "/" + fstring.split(":").first() + "-" + fstring.split(":").last() + ".a" + paridstr + ".stat");
        else
            filefile.setFileName(wombatvariable.tmpmntpath + wombatvariable.evidencename + "." + estring + "/." + vstring + "/." + pstring + "/" + fstring.split(":").first() + ".a" + paridstr + ".stat");
        filefile.open(QIODevice::ReadOnly);
        if(filefile.isOpen())
            tmpstr = filefile.readLine();
        if(fstring.split(":").count() > 1)
            filefileprop.setFileName(wombatvariable.tmpmntpath + wombatvariable.evidencename + "." + estring + "/." + vstring + "/." + pstring + "/" + fstring.split(":").first() + "-" + fstring.split(":").last() + ".a" + paridstr + ".prop");
        else
            filefileprop.setFileName(wombatvariable.tmpmntpath + wombatvariable.evidencename + "." + estring + "/." + vstring + "/." + pstring + "/" + fstring.split(":").first() + ".a" + paridstr + ".prop");
        QString blockstring;
        QString residentstring;
        QString bytestring;
        filefile.close();
        filelist = tmpstr.split(",");
        filefileprop.open(QIODevice::ReadOnly);
        while(!filefileprop.atEnd())
        {
            QString tmpstring = filefileprop.readLine();
            if(tmpstring.contains("Block Address"))
            {
                blockstring = tmpstring.split("||").at(1);
            }
            else if(tmpstring.contains("Resident Offset"))
            {
                residentstring = tmpstring.split("||").at(1);
            }
            else if(tmpstring.contains("Byte Offset"))
            {
                bytestring = tmpstring.split("||").at(1);
            }
        }
        filefileprop.close();

        //if(!ui->hexview->isEnabled())
            ui->hexview->setEnabled(true);

        if(selectedindex.sibling(selectedindex.row(), 2).data().toULongLong() == 0)
        {
            //qDebug() << "zero file";
            ui->hexview->setEnabled(false);
        }
        else
        {
        if(partlist.at(0).toInt() == TSK_FS_TYPE_NTFS_DETECT) // IF NTFS (ADS/FILE/DIR/RES/NONRES)
        {
            unsigned int curoffset = 0;
            uint8_t mftoffset[2];
            uint8_t nextattrid[2];
            uint8_t mftlen[4];
            uint8_t attrtype[4];
            uint32_t atrtype = 0;
            uint8_t namelength = 0;
            uint32_t contentlength = 0;
            uint16_t resoffset = 0;

            if(wombatvariable.selectedid.split("-").at(3).split(":").count() > 1) // IF ADS
            {
                if(filehexfileinfo->meta != NULL)
                {
                    fhexbuf = new char[filelist.at(8).toULongLong()];
                    fhexlen = tsk_fs_file_read_type(filehexfileinfo, TSK_FS_ATTR_TYPE_NTFS_DATA, wombatvariable.selectedid.split("-").at(3).split(":").at(1).toUInt(), 0, fhexbuf, filelist.at(8).toULongLong(), TSK_FS_FILE_READ_FLAG_NONE);
                    if(fhexlen == -1)
                        qDebug() << tsk_error_get_errstr();
                }
                if(blockstring.compare("") != 0 && blockstring.compare("0^^") != 0) // IF NON-RESIDENT
                {
                    ui->hexview->SetColorInformation(partlist.at(4).toULongLong(), partlist.at(6).toULongLong(), blockstring, residentstring, bytestring, selectedindex.sibling(selectedindex.row(), 2).data().toULongLong(), 0);
                    ui->hexview->setCursorPosition(bytestring.toULongLong()*2);
                }
                else // IF RESIDENT
                {
                    unsigned long long residentoffset = mftentryoffset.toULongLong() + (1024 * wombatvariable.selectedid.split("-").at(3).mid(1).split(":").at(0).toInt()) + fsoffset;
                    //qDebug() << "(resident ads) residentoffset:" << residentoffset;
                    QByteArray resbuffer = ui->hexview->dataAt(residentoffset, 1024); // MFT Entry
                    curoffset = 0;
                    //if(resbuffer.length() == 1024)
                    //{
                        mftoffset[0] = (uint8_t)resbuffer.at(20);
                        mftoffset[1] = (uint8_t)resbuffer.at(21);
                        nextattrid[0] = (uint8_t)resbuffer.at(40);
                        nextattrid[1] = (uint8_t)resbuffer.at(41);
                        curoffset += tsk_getu16(TSK_LIT_ENDIAN, mftoffset);
                        int attrcnt = tsk_getu16(TSK_LIT_ENDIAN, nextattrid);
                        for(int i = 0; i < attrcnt; i++)
                        {
                            attrtype[0] = (uint8_t)resbuffer.at(curoffset);
                            attrtype[1] = (uint8_t)resbuffer.at(curoffset + 1);
                            attrtype[2] = (uint8_t)resbuffer.at(curoffset + 2);
                            attrtype[3] = (uint8_t)resbuffer.at(curoffset + 3);
                            atrtype = tsk_getu32(TSK_LIT_ENDIAN, attrtype);
                            namelength = (uint8_t)resbuffer.at(curoffset + 9);
                            mftlen[0] = (uint8_t)resbuffer.at(curoffset + 4);
                            mftlen[1] = (uint8_t)resbuffer.at(curoffset + 5);
                            mftlen[2] = (uint8_t)resbuffer.at(curoffset + 6);
                            mftlen[3] = (uint8_t)resbuffer.at(curoffset + 7);
                            contentlength = tsk_getu32(TSK_LIT_ENDIAN, mftlen);
                            if(namelength > 0 && atrtype == 128)
                                break;
                            curoffset += contentlength;
                        }
                        mftoffset[0] = (uint8_t)resbuffer.at(curoffset + 20);
                        mftoffset[1] = (uint8_t)resbuffer.at(curoffset + 21);
                        resoffset = tsk_getu16(TSK_LIT_ENDIAN, mftoffset);
    
                        ui->hexview->SetColorInformation(partlist.at(4).toULongLong(), partlist.at(6).toULongLong(), blockstring, QString::number(residentoffset + curoffset + resoffset - fsoffset), bytestring, selectedindex.sibling(selectedindex.row(), 2).data().toULongLong(), (curoffset + resoffset));
                        ui->hexview->setCursorPosition((residentoffset + curoffset + resoffset)*2);
                    //}
                }   
            }
            else // IF NOT ADS
            {
                if(filehexfileinfo->meta != NULL)
                {
                    fhexbuf = new char[filehexfileinfo->meta->size];
                    fhexlen = tsk_fs_file_read(filehexfileinfo, 0, fhexbuf, filehexfileinfo->meta->size, TSK_FS_FILE_READ_FLAG_NONE);
                }
                if(filelist.at(1).toInt() == 3) // IF DIR
                {
                    unsigned long long residentoffset = mftentryoffset.toULongLong() + (1024 * wombatvariable.selectedid.split("-").at(3).mid(1).toInt()) + fsoffset;
                    //qDebug() << "(resident dir) residentoffset:" << residentoffset;
                    QByteArray resbuffer = ui->hexview->dataAt(residentoffset, 1024); // MFT Entry
                    //qDebug() << "resbuffer length:" << resbuffer.length();
                    //if(resbuffer.length() == 1024)
                    //{
                        curoffset = 0;
                        resoffset = 0;
                        mftoffset[0] = (uint8_t)resbuffer.at(20);
                        mftoffset[1] = (uint8_t)resbuffer.at(21);
                        nextattrid[0] = (uint8_t)resbuffer.at(40);
                        nextattrid[1] = (uint8_t)resbuffer.at(41);
                        curoffset += tsk_getu16(TSK_LIT_ENDIAN, mftoffset);
                        int attrcnt = tsk_getu16(TSK_LIT_ENDIAN, nextattrid);
                        //qDebug() << "attrcnt:" << attrcnt;
                        for(int i = 0; i < attrcnt; i++)
                        {
                            if(curoffset < resbuffer.size())
                            {
                            attrtype[0] = (uint8_t)resbuffer.at(curoffset);
                            attrtype[1] = (uint8_t)resbuffer.at(curoffset + 1);
                            attrtype[2] = (uint8_t)resbuffer.at(curoffset + 2);
                            attrtype[3] = (uint8_t)resbuffer.at(curoffset + 3);
                            atrtype = tsk_getu32(TSK_LIT_ENDIAN, attrtype);
                            mftlen[0] = (uint8_t)resbuffer.at(curoffset + 4);
                            mftlen[1] = (uint8_t)resbuffer.at(curoffset + 5);
                            mftlen[2] = (uint8_t)resbuffer.at(curoffset + 6);
                            mftlen[3] = (uint8_t)resbuffer.at(curoffset + 7);
                            contentlength = tsk_getu32(TSK_LIT_ENDIAN, mftlen);
                            if(atrtype == 144)
                                break;
                            curoffset += contentlength;
                            }
                        }
                        //qDebug() << "curoffset:" << curoffset;
                        // offset to type 144 resident attribute content
                        if(curoffset < resbuffer.size())
                        {
                        mftoffset[0] = (uint8_t)resbuffer.at(curoffset + 20);
                        mftoffset[1] = (uint8_t)resbuffer.at(curoffset + 21);
                        curoffset += tsk_getu16(TSK_LIT_ENDIAN, mftoffset);
                        }
                        ui->hexview->SetColorInformation(partlist.at(4).toULongLong(), partlist.at(6).toULongLong(), "", QString::number(residentoffset + curoffset - fsoffset), bytestring, selectedindex.sibling(selectedindex.row(), 2).data().toULongLong(), curoffset);
                        ui->hexview->setCursorPosition((residentoffset + curoffset)*2);
                    //}
                }
                else // IF FILE AND OTHER STUFF
                {
                    if(blockstring.compare("") != 0 && blockstring.compare("0^^") != 0) // IF NON-RESIDENT
                    {
                        ui->hexview->SetColorInformation(partlist.at(4).toULongLong(), partlist.at(6).toULongLong(), blockstring, residentstring, bytestring, selectedindex.sibling(selectedindex.row(), 2).data().toULongLong(), 0);
                        ui->hexview->setCursorPosition(bytestring.toULongLong()*2);
                    }
                    else // IF RESIDENT
                    {
                        unsigned long long residentoffset = mftentryoffset.toULongLong() + (1024 * wombatvariable.selectedid.split("-").at(3).mid(1).toInt()) + fsoffset;
                        //qDebug() << "(resident file) residentoffset:" << residentoffset;
                        QByteArray resbuffer = ui->hexview->dataAt(residentoffset, 1024); // MFT Entry
                        curoffset = 0;
                        //if(resbuffer.length() == 1024)
                        //{
                            mftoffset[0] = (uint8_t)resbuffer.at(20);
                            mftoffset[1] = (uint8_t)resbuffer.at(21);
                            nextattrid[0] = (uint8_t)resbuffer.at(40);
                            nextattrid[1] = (uint8_t)resbuffer.at(41);
                            curoffset += tsk_getu16(TSK_LIT_ENDIAN, mftoffset);
                            int attrcnt = tsk_getu16(TSK_LIT_ENDIAN, nextattrid);
                            //qDebug() << "attrcnt:" << attrcnt;
                            for(int i = 0; i < attrcnt; i++)
                            {
                                if(curoffset < resbuffer.size())
                                {
                                attrtype[0] = (uint8_t)resbuffer.at(curoffset);
                                attrtype[1] = (uint8_t)resbuffer.at(curoffset + 1);
                                attrtype[2] = (uint8_t)resbuffer.at(curoffset + 2);
                                attrtype[3] = (uint8_t)resbuffer.at(curoffset + 3);
                                atrtype = tsk_getu32(TSK_LIT_ENDIAN, attrtype);
                                namelength = (uint8_t)resbuffer.at(curoffset + 9);
                                mftlen[0] = (uint8_t)resbuffer.at(curoffset + 4);
                                mftlen[1] = (uint8_t)resbuffer.at(curoffset + 5);
                                mftlen[2] = (uint8_t)resbuffer.at(curoffset + 6);
                                mftlen[3] = (uint8_t)resbuffer.at(curoffset + 7);
                                contentlength = tsk_getu32(TSK_LIT_ENDIAN, mftlen);
                                if(namelength == 0 && atrtype == 128)
                                    break;
                                curoffset += contentlength;
                                }
                            }
                            //qDebug() << "curoffset:" << curoffset;
                            if(curoffset < resbuffer.size())
                            {
                            mftoffset[0] = (uint8_t)resbuffer.at(curoffset + 20);
                            mftoffset[1] = (uint8_t)resbuffer.at(curoffset + 21);
                            resoffset = tsk_getu16(TSK_LIT_ENDIAN, mftoffset);
                            }
                            ui->hexview->SetColorInformation(partlist.at(4).toULongLong(), partlist.at(6).toULongLong(), blockstring, QString::number(residentoffset + curoffset + resoffset - fsoffset), bytestring, selectedindex.sibling(selectedindex.row(), 2).data().toULongLong(), (curoffset + resoffset));
                            ui->hexview->setCursorPosition((residentoffset + curoffset + resoffset)*2);
                        //}
                    }
                }
            }
        }
        else // OTHER FILE SYSTEM
        {
            if(filehexfileinfo->meta != NULL)
            {
                fhexbuf = new char[filehexfileinfo->meta->size];
                fhexlen = tsk_fs_file_read(filehexfileinfo, 0, fhexbuf, filehexfileinfo->meta->size, TSK_FS_FILE_READ_FLAG_NONE);
            }
            ui->hexview->SetColorInformation(partlist.at(4).toULongLong(), partlist.at(6).toULongLong(), blockstring, residentstring, bytestring, selectedindex.sibling(selectedindex.row(), 2).data().toULongLong(), 0);
            ui->hexview->setCursorPosition(bytestring.toULongLong()*2);
        }
        }
        if(selectedindex.sibling(selectedindex.row(), 2).data().toULongLong() > 0)
        {
        (new QDir())->mkpath(wombatvariable.tmpfilepath);
        hexstring = wombatvariable.tmpfilepath + selectedindex.sibling(selectedindex.row(), 10).data().toString() + "-fhex";
        QFile tmpfile(hexstring);
        tmpfile.open(QIODevice::WriteOnly);
        QDataStream outbuffer(&tmpfile);
        outbuffer.writeRawData(fhexbuf, fhexlen);
        tmpfile.close();
        delete[] fhexbuf;
        tsk_fs_file_close(filehexfileinfo);
        tsk_fs_close(filehexfsinfo);
        tsk_img_close(fileheximginfo);
        }
        else
        {
            tsk_fs_file_close(filehexfileinfo);
            tsk_fs_close(filehexfsinfo);
            tsk_img_close(fileheximginfo);
        }
    }
    ui->hexview->ensureVisible();
    */
//}

    hexfile.setFileName(hexstring);
    ui->filehexview->setData(hexfile);
    this->show();
}

void FileViewer::JumpHex()
{
    //unsigned long long jumpoffset = ui->lineEdit->text().toULongLong(0, 16);
    ui->filehexview->setCursorPosition(jumpto->text().toULongLong(0, 16)*2);
    ui->filehexview->ensureVisible();
    //ui->filehexview->
}
