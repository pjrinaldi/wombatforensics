#include "mboxviewer.h"

// Copyright 2013-2022 Pasquale J. Rinaldi, Jr.
// Distrubted under the terms of the GNU General Public License version 2

MBoxDialog::MBoxDialog(QWidget* parent) : QDialog(parent), ui(new Ui::MBoxDialog)
{
    ui->setupUi(this);
    ui->toolButton->setVisible(false);
    ui->label->setVisible(false);
    ui->mailtable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->mailtable->setHorizontalHeaderLabels({"ID", "From", "Date Time", "Subject", "Tag"});
    connect(ui->mailtable, SIGNAL(itemSelectionChanged()), this, SLOT(EmailSelected()), Qt::DirectConnection);

    QStringList taglist;
    taglist.clear();
    tagmenu = new QMenu(ui->mailtable);
    bookmarkfile.open(QIODevice::ReadOnly | QIODevice::Text);
    if(bookmarkfile.isOpen())
	taglist = QString(bookmarkfile.readLine()).split(",", Qt::SkipEmptyParts);
    bookmarkfile.close();
    for(int i=0; i < taglist.count(); i++)
    {
	QAction* tmpaction = new QAction(taglist.at(i), tagmenu);
	tmpaction->setIcon(QIcon(":/bar/addtotag"));
	tmpaction->setData(QVariant("t" + QString::number(i)));
	connect(tmpaction, SIGNAL(triggered()), this, SLOT(SetTag()));
	tagmenu->addAction(tmpaction);
    }
    tagmenu->addSeparator();
    QAction* remtagaction = new QAction("Remove Tag", tagmenu);
    remtagaction->setIcon(QIcon(":/bar/tag-rem"));
    connect(remtagaction, SIGNAL(triggered()), this, SLOT(RemoveTag()));
    tagmenu->addAction(remtagaction);
    ui->mailtable->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->mailtable, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(TagMenu(const QPoint &)), Qt::DirectConnection);
    QFile mboxtagfile;
    mboxtagfile.setFileName(wombatvariable.tmpmntpath + "mboxtags");
    if(!mboxtagfile.isOpen())
        mboxtagfile.open(QIODevice::ReadOnly | QIODevice::Text);
    mboxtaglist.clear();
    if(mboxtagfile.isOpen())
        mboxtaglist = QString(mboxtagfile.readLine()).split(",", Qt::SkipEmptyParts);
    mboxtagfile.close();
}

MBoxDialog::~MBoxDialog()
{
    delete ui;
}

void MBoxDialog::HideClicked()
{
    this->close();
}

void MBoxDialog::EmailSelected()
{
    QString mboxid = ui->mailtable->item(ui->mailtable->currentRow(), 0)->text().split("-m").at(0);
    QString layout = ui->mailtable->item(ui->mailtable->currentRow(), 0)->toolTip();
    QFile mboxfile(wombatvariable.tmpfilepath + mboxid + "-fhex");
    if(!mboxfile.isOpen())
        mboxfile.open(QIODevice::ReadOnly | QIODevice::Text);
    if(mboxfile.isOpen())
    {
        mboxfile.seek(layout.split(";").at(0).split(",").at(0).toULongLong());
        htmlvalue = mboxfile.read(layout.split(";").at(0).split(",").at(1).toULongLong());
        ui->mailcontent->setPlainText(htmlvalue);
        mboxfile.close();
    }
    /*
     *	htmlentry = "";
	htmlentry += "<td style='" + ReturnCssString(11) + "' id='" + this->windowTitle().mid(16) + "|" + ui->label->text() + "\\" + ui->tableWidget->selectedItems().first()->text() + "'>";
	htmlentry += "<table style='" + ReturnCssString(2) + "' width='300px'><tr style='" + ReturnCssString(3) + "'><th style='" + ReturnCssString(6) + "' colspan='2'>" + ui->tableWidget->selectedItems().first()->text() + "</th></tr>";
	htmlentry += "<tr style='" + ReturnCssString(12) + "'><td style='" + ReturnCssString(13) + "'>Path:</td><td style='" + ReturnCssString(14) + "'><span style='word-wrap:break-word;'>" + ui->label->text() + "</span></td></tr>";
	int valueindex = ui->tableWidget->selectedItems().first()->row();
	QString keypath = ui->label->text();
	libregf_file_t* regfile = NULL;
	libregf_error_t* regerr = NULL;
	libregf_file_initialize(&regfile, &regerr);
	libregf_file_open(regfile, regfilepath.toStdString().c_str(), LIBREGF_OPEN_READ, &regerr);
	libregf_key_t* curkey = NULL;
	libregf_file_get_key_by_utf8_path(regfile, (uint8_t*)(keypath.toUtf8().data()), keypath.toUtf8().size(), &curkey, &regerr);
	libregf_value_t* curval = NULL;
	libregf_key_get_value(curkey, valueindex, &curval, &regerr);
        uint64_t lastwritetime = 0;
        libregf_key_get_last_written_time(curkey, &lastwritetime, &regerr);
	htmlentry += "<tr style='" + ReturnCssString(5) + "'><td style='" + ReturnCssString(13) + "'>Last Modified:</td><td style='" + ReturnCssString(14) + "'>" + ConvertWindowsTimeToUnixTime(lastwritetime) + "</td></tr>";
	htmlentry += "<tr style='" + ReturnCssString(4) + "'><td style='" + ReturnCssString(13) + "'>ID:</td><td style='" + ReturnCssString(14) + "'>" + this->windowTitle().mid(16) + "</td></tr>";
        htmlentry += "<tr style='" + ReturnCssString(5) + "'><td style='" + ReturnCssString(13) + "'>&nbsp;</td><td style='" + ReturnCssString(7) + "'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href='./registry/" + this->windowTitle().mid(16) + "." + ui->label->text().replace("\\", "-") + "-" + ui->tableWidget->selectedItems().first()->text() + "'>Link</a></td></tr>";
	htmlentry += "</table></td>";
        QString valuedata = "Last Written Time:\t" + ConvertWindowsTimeToUnixTimeUTC(lastwritetime) + " UTC\n\n";
	valuedata += "Name:\t" + ui->tableWidget->selectedItems().first()->text() + "\n\n";
	if(ui->tableWidget->selectedItems().first()->text().contains("(unnamed)"))
	{
	    valuedata += "Content\n-------\n\n";
	    valuedata += "Hex:\t0x" + ui->tableWidget->selectedItems().at(1)->text() + "\n";
	    valuedata += "Integer:\t" + QString::number(ui->tableWidget->selectedItems().at(1)->text().toInt(nullptr, 16)) + "\n";
	}
	else
	{
            QString valuetype = ui->tableWidget->selectedItems().at(1)->text();
            if(valuetype.contains("REG_SZ") || valuetype.contains("REG_EXPAND_SZ"))
            {
                valuedata += "Content:\t";
                size_t strsize = 0;
                libregf_value_get_value_utf8_string_size(curval, &strsize, &regerr);
                uint8_t valstr[strsize];
                libregf_value_get_value_utf8_string(curval, valstr, strsize, &regerr);
                valuedata += QString::fromUtf8(reinterpret_cast<char*>(valstr));
            }
            else if(valuetype.contains("REG_BINARY"))
            {
                valuedata += "Content\n-------\n\n";
                if(keypath.contains("UserAssist") && (keypath.contains("{750") || keypath.contains("{F4E") || keypath.contains("{5E6")))
                {
                    valuedata += "ROT13 Decrypted Content:\t";
                    valuedata += DecryptRot13(ui->tableWidget->selectedItems().first()->text()) + "\n";
                }
                else if(keypath.contains("SAM") && ui->tableWidget->selectedItems().first()->text().count() == 1 && ui->tableWidget->selectedItems().first()->text().startsWith("F"))
                {
                    size_t datasize = 0;
                    libregf_value_get_value_data_size(curval, &datasize, &regerr);
                    uint8_t data[datasize];
                    libregf_value_get_value_data(curval, data, datasize, &regerr);
                    QByteArray farray = QByteArray::fromRawData((char*)data, datasize);
                    valuedata += "Account Expiration:\t\t";
                    if(farray.mid(32,1).toHex() == "ff")
                    {
                        valuedata += "No Expiration is Set\n";
                    }
                    else
                        valuedata += ConvertWindowsTimeToUnixTimeUTC(qFromLittleEndian<uint64_t>(farray.mid(32, 8))) + " UTC\n";
                    valuedata += "Last Logon Time:\t\t" + ConvertWindowsTimeToUnixTimeUTC(qFromLittleEndian<uint64_t>(farray.mid(8, 8))) + " UTC\n";
                    valuedata += "Last Failed Login:\t\t" + ConvertWindowsTimeToUnixTimeUTC(qFromLittleEndian<uint64_t>(farray.mid(40, 8))) + " UTC\n";
                    valuedata += "Last Time Password Changed:\t" + ConvertWindowsTimeToUnixTimeUTC(qFromLittleEndian<uint64_t>(farray.mid(24, 8))) + " UTC";
	            //QString filenamestring = QString::fromStdString(QByteArray(info2content.mid(curpos + 3, 260).toStdString().c_str(), -1).toStdString());
                }
                else if(ui->tableWidget->selectedItems().first()->text().startsWith("ShutdownTime"))
                {
                    size_t datasize = 0;
                    libregf_value_get_value_data_size(curval, &datasize, &regerr);
                    uint8_t data[datasize];
                    libregf_value_get_value_data(curval, data, datasize, &regerr);
                    QByteArray valarray = QByteArray::fromRawData((char*)data, datasize);
                    valuedata += "Shutdown Time:\t" + ConvertWindowsTimeToUnixTimeUTC(qFromLittleEndian<uint64_t>(valarray)) + " UTC";
                }
                /*
                else if(keypath.contains("SAM") && ui->tableWidget->selectedItems().first()->text().count() == 1 && ui->tableWidget->selectedItems().first()->text().startsWith("V"))
                {
                    size_t datasize = 0;
                    libregf_value_get_value_data_size(curval, &datasize, &regerr);
                    uint8_t data[datasize];
                    libregf_value_get_value_data(curval, data, datasize, &regerr);
                    QByteArray varray = QByteArray::fromRawData((char*)data, datasize);
                    valuedata += "Machine SID Location:\t" + varray.right(12).toHex();
                }
                */
/*            }
            else if(valuetype.contains("REG_DWORD"))
            {
                valuedata += "Content:\t";
                uint32_t dwordvalue = 0;
                libregf_value_get_value_32bit(curval, &dwordvalue, &regerr);
                if(ui->tableWidget->selectedItems().first()->text().toLower().contains("date"))
                    valuedata += ConvertUnixTimeToString(dwordvalue);
                else
                    valuedata += QString::number(dwordvalue);
            }
            else if(valuetype.contains("REG_DWORD_BIG_ENDIAN"))
            {
                valuedata += "Content:\t";
                uint32_t dwordvalue = 0;
                libregf_value_get_value_32bit(curval, &dwordvalue, &regerr);
                valuedata += QString::number(qFromBigEndian<uint32_t>(dwordvalue));
            }
            else if(valuetype.contains("REG_MULTI_SZ"))
            {
                valuedata += "Content\n";
                valuedata += "-------\n";
                libregf_multi_string_t* multistring = NULL;
                libregf_value_get_value_multi_string(curval, &multistring, &regerr);
                int strcnt = 0;
                libregf_multi_string_get_number_of_strings(multistring, &strcnt, &regerr);
                for(int i=0; i < strcnt; i++)
                {
                    size_t strsize = 0;
                    libregf_multi_string_get_utf8_string_size(multistring, i, &strsize, &regerr);
                    uint8_t valstr[strsize];
                    libregf_multi_string_get_utf8_string(multistring, i, valstr, strsize, &regerr);
                    valuedata += QString::fromUtf8(reinterpret_cast<char*>(valstr)) + "\n";
                }
                libregf_multi_string_free(&multistring, &regerr);
            }
            else if(valuetype.contains("REG_QWORD"))
            {
                valuedata += "Content:\t";
                uint64_t qwordvalue = 0;
                libregf_value_get_value_64bit(curval, &qwordvalue, &regerr);
                valuedata += QString::number(qwordvalue);
            }
	}
        size_t datasize = 0;
        libregf_value_get_value_data_size(curval, &datasize, &regerr);
        uint8_t data[datasize];
        libregf_value_get_value_data(curval, data, datasize, &regerr);
        QByteArray dataarray = QByteArray::fromRawData((char*)data, datasize);
        valuedata += "\n\nBinary Content\n--------------\n\n";
        int linecount = datasize / 16;
        //int remainder = datasize % 16;
        for(int i=0; i < linecount; i++)
        {
            valuedata += QString::number(i * 16, 16).rightJustified(8, '0') + "\t";
            for(int j=0; j < 16; j++)
            {
                valuedata += QString("%1").arg(data[j+i*16], 2, 16, QChar('0')).toUpper() + " ";
            }
            for(int j=0; j < 16; j++)
            {
                if(!QChar(dataarray.at(j+i*16)).isPrint())
                {
                    valuedata += ".";
                }
                else
                    valuedata += QString("%1").arg(dataarray.at(j+i*16));
            }
            valuedata += "\n";
        }
	ui->plainTextEdit->setPlainText(valuedata);

    	htmlvalue = "<html><body style='" + ReturnCssString(0) + "'>";
        /*
	QFile initfile(":/html/artifactprephtml");
	initfile.open(QIODevice::ReadOnly);
	if(initfile.isOpen())
	    htmlvalue = initfile.readAll();
	initfile.close();
        */
/*	htmlvalue += "<div style='" + ReturnCssString(1) + "'>Registry Analysis</div><br/>";
	htmlvalue += "<pre>";
	htmlvalue += valuedata;
	htmlvalue += "</pre>";
	htmlvalue += "</table></body></html>";
*/
}

void MBoxDialog::closeEvent(QCloseEvent* e)
{
    e->accept();
}

void MBoxDialog::LoadMBoxFile(QString mboxid, QString mboxname)
{
    mboxparentname = mboxname;
    ui->mailtable->clear();
    ui->mailcontent->setPlainText("");
    QDir mailboxdir(wombatvariable.tmpmntpath + "mailboxes/");
    QStringList mailboxfiles = mailboxdir.entryList(QStringList() << QString(mboxid + "-m*.prop"), QDir::Files);
    ui->mailtable->setHorizontalHeaderLabels({"ID", "From", "Date Time", "Subject", "Tag"});
    ui->mailtable->setRowCount(mailboxfiles.count());
    for(int i=0; i < mailboxfiles.count(); i++)
    {
        QTableWidgetItem* tmpitem = new QTableWidgetItem(QString(mailboxfiles.at(i)).remove(".prop"));
        QFile propfile(wombatvariable.tmpmntpath + "mailboxes/" + mailboxfiles.at(i));
        if(!propfile.isOpen())
            propfile.open(QIODevice::ReadOnly | QIODevice::Text);
        if(propfile.isOpen())
        {
            while(!propfile.atEnd())
            {
                QString tmpstr = propfile.readLine();
                if(tmpstr.startsWith("From|"))
                    ui->mailtable->setItem(i, 1, new QTableWidgetItem(tmpstr.split("|").at(1)));
                if(tmpstr.startsWith("Date|"))
                    ui->mailtable->setItem(i, 2, new QTableWidgetItem(tmpstr.split("|").at(1)));
                if(tmpstr.startsWith("Subject|"))
                    ui->mailtable->setItem(i, 3, new QTableWidgetItem(tmpstr.split("|").at(1)));
                if(tmpstr.startsWith("Layout|"))
                    tmpitem->setToolTip(tmpstr.split("|").at(1));
            }
            propfile.close();
        }
        ui->mailtable->setItem(i, 0, tmpitem);
        QString tagstr = "";
        for(int j=0; j < mboxtaglist.count(); j++)
        {
            if(mboxtaglist.at(j).contains(QString(mailboxfiles.at(i)).remove(".prop")))
                tagstr = mboxtaglist.at(j).split("|", Qt::SkipEmptyParts).last();
        }
        ui->mailtable->setItem(i, 4, new QTableWidgetItem(tagstr));
        /*
	QString tagstr = "";
	for(int j=0; j < registrytaglist.count(); j++)
	{
	    //qDebug() << "registry tag list at(" << j << ") :" << registrytaglist.at(j);
	    if(registrytaglist.at(j).contains(curtagvalue))
		tagstr = registrytaglist.at(j).split("|", Qt::SkipEmptyParts).last();
	}
	//qDebug() << "tagstr:" << tagstr;
	ui->tableWidget->setItem(i, 2, new QTableWidgetItem(tagstr));
        ui->tableWidget->resizeColumnToContents(0);
         */ 
        //if(tag exists, then setItem(i, 4, tagname);
	//ui->mailtable->setItem(i, 0, new QTableWidgetItem(mailboxfiles.at(i)));
    }
    this->show();
    /*
    QDir hashdir(wombatvariable.tmpmntpath + "hashlists/");
    QFileInfoList whllist = hashdir.entryInfoList(QStringList() << "*.whl", QDir::Files);
    for(int i=0; i < whllist.count(); i++)
	new QListWidgetItem(whllist.at(i).fileName(), ui->hashlistwidget);
    if(ui->hashlistwidget->count() > 0)

    ui->tableWidget->clear();
    ui->plainTextEdit->setPlainText("");
    ui->tableWidget->setRowCount(valuecount);
    for(int i=0; i < valuecount; i++)
    {
	libregf_value_t* curval = NULL;
	libregf_key_get_value(curkey, i, &curval, &regerr);
	size_t namesize = 0;
	libregf_value_get_utf8_name_size(curval, &namesize, &regerr);
	uint8_t name[namesize];
	libregf_value_get_utf8_name(curval, name, namesize, &regerr);
	uint32_t type = 0;
	libregf_value_get_value_type(curval, &type, &regerr);
	QString curtagvalue = this->windowTitle().mid(16) + "|" + keypath + "\\";
	if(namesize == 0)
	{
	    curtagvalue += "(unnamed)";
	    ui->tableWidget->setHorizontalHeaderLabels({"Value Name", "Value", "Tag"});
	    ui->tableWidget->setItem(i, 0, new QTableWidgetItem("(unnamed)"));
	    ui->tableWidget->setItem(i, 1, new QTableWidgetItem(QString::number(type, 16)));
	    //ui->tableWidget->setItem(i, 2, new QTableWidgetItem(""));
	}
	else
	{
	    curtagvalue += QString::fromUtf8(reinterpret_cast<char*>(name));
            QString valuetypestr = "";
	    ui->tableWidget->setHorizontalHeaderLabels({"Value Name", "Value Type", "Tag"});
	    ui->tableWidget->setItem(i, 0, new QTableWidgetItem(QString::fromUtf8(reinterpret_cast<char*>(name))));
            if(type == 0x00) // none
            {
            }
            else if(type == 0x01) // reg_sz
            {
                valuetypestr = "REG_SZ";
            }
            else if(type == 0x02) // reg_expand_sz
            {
                valuetypestr = "REG_EXPAND_SZ";
            }
            else if(type == 0x03) // reg_binary
            {
                valuetypestr = "REG_BINARY";
            }
            else if(type == 0x04) // reg_dword reg_dword_little_endian (4 bytes)
            {
                valuetypestr = "REG_DWORD";
            }
            else if(type == 0x05) // reg_dword_big_endian (4 bytes)
            {
                valuetypestr = "REG_DWORD_BIG_ENDIAN";
            }
            else if(type == 0x06) // reg_link
            {
                valuetypestr = "REG_LINK";
            }
            else if(type == 0x07) // reg_multi_sz
            {
                valuetypestr = "REG_MULTI_SZ";
            }
            else if(type == 0x08) // reg_resource_list
            {
                valuetypestr = "REG_RESOURCE_LIST";
            }
            else if(type == 0x09) // reg_full_resource_descriptor
            {
                valuetypestr = "REG_FULL_RESOURCE_DESCRIPTOR";
            }
            else if(type == 0x0a) // reg_resource_requirements_list
            {
                valuetypestr = "REG_RESOURCE_REQUIREMENTS_LIST";
            }
            else if(type == 0x0b) // reg_qword_little_endian (8 bytes)
            {
                valuetypestr = "REG_QWORD";
            }
            else
            {
            }
	    ui->tableWidget->setItem(i, 1, new QTableWidgetItem(valuetypestr));
	    //ui->tableWidget->setItem(i, 2, new QTableWidgetItem(""));
	}
	//qDebug() << "curtagvalue:" << curtagvalue;
	QString tagstr = "";
	for(int j=0; j < registrytaglist.count(); j++)
	{
	    //qDebug() << "registry tag list at(" << j << ") :" << registrytaglist.at(j);
	    if(registrytaglist.at(j).contains(curtagvalue))
		tagstr = registrytaglist.at(j).split("|", Qt::SkipEmptyParts).last();
	}
	//qDebug() << "tagstr:" << tagstr;
	ui->tableWidget->setItem(i, 2, new QTableWidgetItem(tagstr));
        ui->tableWidget->resizeColumnToContents(0);
        ui->tableWidget->setCurrentCell(0, 0);
	libregf_value_free(&curval, &regerr);
     */ 
}

void MBoxDialog::TagMenu(const QPoint &pt)
{
    // when i need the current value for the right click, i can use a class variable defined in .h so i can access it in the SetTag and CreateNewTag right click menu options...
    //QTableWidgetItem* curitem = ui->tableWidget->itemAt(pt);
    //qDebug() << "cur item:" << ui->tableWidget->item(curitem->row(), 0)->text();
    tagmenu->exec(ui->mailtable->mapToGlobal(pt));
}

void MBoxDialog::SetTag()
{
    // FOR MBOX, I NEED THE FILE ID, MAIL ID, AND MAIL MESSAGE
    QString curtag = "";
    QString mboxstring = "";
    QAction* tagaction = qobject_cast<QAction*>(sender());
    //mboxstring += this->windowTitle().mid(12) + "|"; // file id
    mboxstring += ui->mailtable->item(ui->mailtable->currentRow(), 0)->text() + "|";
    if(ui->mailtable->item(ui->mailtable->currentRow(), 4) != NULL)
        curtag = mboxstring + ui->mailtable->item(ui->mailtable->currentRow(), 4)->text();
    //mboxstring += ui->label->text() + "\\"; // key
    //QString mboxid = ui->mailtable->item(ui->mailtable->currentRow(), 0)->text().split("-m").at(0);
    //mboxstring += ui->mailtable->selectedItems().first()->text() + "|"; // mbox email id
    mboxstring += tagaction->iconText(); // tag name
    //qDebug() << "mboxstring:" << mboxstring;
    //qDebug() << "curtag:" << curtag;
    //QString idkeyvalue = this->windowTitle().mid(12) + "|" + ui->mailtable->selectedItems().first()->text();
    //qDebug() << "idkeyvalue:" << idkeyvalue;
    ui->mailtable->setItem(ui->mailtable->currentRow(), 4, new QTableWidgetItem(tagaction->iconText()));
    //qDebug() << "curtag to remove:" << curtag;
    if(!curtag.isEmpty())
	RemTag("mbox", curtag);
    AddTag("mbox", mboxstring); // this adds the id and tag to the mboxtags file // add htmlentry and htmlvalue to this function...
    //RemoveArtifactFile("mbox", idkeyvalue);
    htmlentry = "";
    htmlentry += "<td style='" + ReturnCssString(11) + "' id='" + ui->mailtable->item(ui->mailtable->currentRow(), 0)->text() + "'>";
    htmlentry += "<table style='" + ReturnCssString(2) + "' width=300px'><tr style='" + ReturnCssString(3) + "'><th style='" + ReturnCssString(6) + "' colspan='2'>Email Message " + ui->mailtable->item(ui->mailtable->currentRow(), 0)->text().split("-m").at(1) + "</th></tr>";
    htmlentry += "<tr style='" + ReturnCssString(12) + "'><td style='" + ReturnCssString(13) + "'>From:</td><td style='" + ReturnCssString(14) + "'><span style='word-wrap:break-word;'>" + ui->mailtable->item(ui->mailtable->currentRow(), 1)->text() + "</span></td></tr>";
    htmlentry += "<tr style='" + ReturnCssString(12) + "'><td style='" + ReturnCssString(13) + "'>Date:</td><td style='" + ReturnCssString(14) + "'><span style='word-wrap:break-word;'>" + ui->mailtable->item(ui->mailtable->currentRow(), 2)->text() + "</span></td></tr>";
    htmlentry += "<tr style='" + ReturnCssString(12) + "'><td style='" + ReturnCssString(13) + "'>Subject:</td><td style='" + ReturnCssString(14) + "'><span style='word-wrap:break-word;'>" + ui->mailtable->item(ui->mailtable->currentRow(), 3)->text() + "</span></td></tr>";
    htmlentry += "<tr style='" + ReturnCssString(12) + "'><td style='" + ReturnCssString(13) + "'>Mbox Id:</td><td style='" + ReturnCssString(14) + "'><span style='word-wrap:break-word;'>" + ui->mailtable->item(ui->mailtable->currentRow(), 0)->text() + "</span></td></tr>";
    htmlentry += "<tr style='" + ReturnCssString(12) + "'><td style='" + ReturnCssString(13) + "'>Mbox File Name:</td><td style='" + ReturnCssString(14) + "'><span style='word-wrap:break-word;'>" + mboxparentname + "</span></td></tr>";
    htmlentry += "<tr style='" + ReturnCssString(5) + "'><td style='" + ReturnCssString(13) + "'>&nbsp;</td><td style='" + ReturnCssString(7) + "'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href='./mbox/" + ui->mailtable->item(ui->mailtable->currentRow(), 0)->text() + "'>Link</a></td></tr>";
    htmlentry += "</table></td>";

    QString htmlcontent = "";
    htmlcontent = "<html><body style='" + ReturnCssString(0) + "'>";
    htmlcontent += "<div style='" + ReturnCssString(1) + "'>MBox Email Analysis</div><br/>";
    htmlcontent += "<pre>";
    htmlcontent += htmlvalue;
    htmlcontent += "</pre>";
    htmlcontent += "</body></html>";

    AddFileItem(tagaction->iconText(), htmlentry);
    CreateArtifactFile("mbox", ui->mailtable->item(ui->mailtable->currentRow(), 0)->text(), htmlcontent);
    // ADD TO PREVIEW REPORT
    //RemoveFileItem(curindex.sibling(curindex.row(), 11).data().toString());
    //AddFileItem(tagname, filestr);
    //CreateArtifactFile("registry", curtag, htmlvalue);
}

void MBoxDialog::RemoveTag()
{
    //qDebug() << "remove tag";
    QString mboxstring = "";
    QAction* tagaction = qobject_cast<QAction*>(sender());
    //mboxstring += this->windowTitle().mid(16) + "|"; // file id
    //mboxstring += ui->label->text() + "\\"; // key
    mboxstring += ui->mailtable->item(ui->mailtable->currentRow(), 0)->text() + "|";
    mboxstring += ui->mailtable->item(ui->mailtable->currentRow(), 4)->text();
    //mboxstring += tagaction->iconText() + ",";
    //qDebug() << "mboxstring:" << mboxstring;
    //mboxstring += ui->mailtable->selectedItems().first()->text() + "|";
    //mboxstring += tagaction->iconText() + ",";
    //QString idkeyvalue = this->windowTitle().mid(16) + "|" + ui->mailtable->selectedItems().first()->text();
    ui->mailtable->item(ui->mailtable->currentRow(), 4)->setText("");
    //ui->mailtable->selectedItems().last()->setText("");
    RemTag("mbox", mboxstring);
    RemoveFileItem(ui->mailtable->item(ui->mailtable->currentRow(), 0)->text());
    RemoveArtifactFile("mbox", ui->mailtable->item(ui->mailtable->currentRow(), 0)->text());
    /*
    // REMOVE FROM PREVIEW REPORT
    RemoveFileItem(idkeyvalue);
    RemoveArtifactFile("mbox", idkeyvalue);
    */
    //RemoveFileItem(selectedindex.sibling(selectedindex.row(), 11).data().toString());
}
