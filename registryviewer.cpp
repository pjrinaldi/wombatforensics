#include "registryviewer.h"

// Copyright 2013-2020 Pasquale J. Rinaldi, Jr.
// Distrubted under the terms of the GNU General Public License version 2

RegistryDialog::RegistryDialog(QWidget* parent) : QDialog(parent), ui(new Ui::RegistryDialog)
{
    ui->setupUi(this);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->label->setText("");
    connect(ui->treeWidget, SIGNAL(itemSelectionChanged()), this, SLOT(KeySelected()), Qt::DirectConnection);
    connect(ui->tableWidget, SIGNAL(itemSelectionChanged()), this, SLOT(ValueSelected()), Qt::DirectConnection);
    QStringList taglist;
    taglist.clear();
    tagmenu = new QMenu(ui->tableWidget);
    bookmarkfile.open(QIODevice::ReadOnly | QIODevice::Text);
    if(bookmarkfile.isOpen())
	taglist = QString(bookmarkfile.readLine()).split(",", Qt::SkipEmptyParts);
    bookmarkfile.close();
    /*
    QAction* newtagaction = new QAction("New Tag", tagmenu);
    newtagaction->setIcon(QIcon(":/bar/newtag"));
    connect(newtagaction, SIGNAL(triggered()), this, SLOT(CreateNewTag()));
    tagmenu->addAction(newtagaction);
    tagmenu->addSeparator();
    */
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
    ui->tableWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->tableWidget, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(TagMenu(const QPoint &)), Qt::DirectConnection);
    QFile registryfile;
    registryfile.setFileName(wombatvariable.tmpmntpath + "registry");
    registryfile.open(QIODevice::ReadOnly | QIODevice::Text);
    registrytaglist.clear();
    if(registryfile.isOpen())
	registrytaglist = QString(registryfile.readLine()).split(",", Qt::SkipEmptyParts);
    registryfile.close();
}

RegistryDialog::~RegistryDialog()
{
    delete ui;
}

void RegistryDialog::HideClicked()
{
    this->close();
}

/*
void RegistryDialog::CreateNewTag()
{
    qDebug() << "create new tag";
}
*/

void RegistryDialog::SetTag()
{
    QString curtag = "";
    QString regstring = "";
    QAction* tagaction = qobject_cast<QAction*>(sender());
    regstring += this->windowTitle().mid(16) + "|"; // file id
    regstring += ui->label->text() + "\\"; // key
    regstring += ui->tableWidget->selectedItems().first()->text() + "|";
    if(!ui->tableWidget->selectedItems().last()->text().isEmpty())
	curtag = regstring + ui->tableWidget->selectedItems().last()->text();
    regstring += tagaction->iconText();
    QString idkeyvalue = this->windowTitle().mid(16) + "|" + ui->label->text() + "\\" + ui->tableWidget->selectedItems().first()->text();
    ui->tableWidget->selectedItems().last()->setText(tagaction->iconText());
    //qDebug() << "curtag to remove:" << curtag;
    if(!curtag.isEmpty())
	RemTag("registry", curtag);
    AddTag("registry", regstring); // add htmlentry and htmlvalue to this function...
    RemoveFileItem(idkeyvalue);
    AddFileItem(tagaction->iconText(), htmlentry);
    CreateArtifactFile("registry", idkeyvalue, htmlvalue);
    // ADD TO PREVIEW REPORT
    //RemoveFileItem(curindex.sibling(curindex.row(), 11).data().toString());
    //AddFileItem(tagname, filestr);
    //CreateArtifactFile("registry", curtag, htmlvalue);
}

void RegistryDialog::RemoveTag()
{
    //qDebug() << "remove tag";
    QString regstring = "";
    QAction* tagaction = qobject_cast<QAction*>(sender());
    regstring += this->windowTitle().mid(16) + "|"; // file id
    regstring += ui->label->text() + "\\"; // key
    regstring += ui->tableWidget->selectedItems().first()->text() + "|";
    regstring += tagaction->iconText() + ",";
    QString idkeyvalue = this->windowTitle().mid(16) + "|" + ui->label->text() + "\\" + ui->tableWidget->selectedItems().first()->text();
    ui->tableWidget->selectedItems().last()->setText("");
    RemTag("registry", tagaction->iconText());
    // REMOVE FROM PREVIEW REPORT
    RemoveFileItem(idkeyvalue);
    //RemoveFileItem(selectedindex.sibling(selectedindex.row(), 11).data().toString());
}

void RegistryDialog::ValueSelected(void)
{
    if(ui->tableWidget->selectedItems().count() > 0)
    {
        //QTimeZone tmpzone = QTimeZone(reporttimezone);
	/*
        QString filestr = "<td class='fitem' id='" + curindex.sibling(curindex.row(), 11).data().toString() + "'>";
        filestr += "<table width='300px'><tr><th colspan='2'>" + curindex.sibling(curindex.row(), 0).data().toString() + "</th></tr>";
        filestr += "<tr class='odd vtop'><td class='pvalue'>File Path:</td><td class='property'><span style='word-wrap:break-word;'>" + curindex.sibling(curindex.row(), 1).data().toString() + "</span></td></tr>";
        filestr += "<tr class='even'><td class='pvalue'>File Size:</td><td class='property'>" + curindex.sibling(curindex.row(), 2).data().toString() + " bytes</td></tr>";
        if(!curindex.sibling(curindex.row(), 3).data().toString().isEmpty())
            filestr += "<tr class='odd'><td class='pvalue'>Created:</td><td class='property'>" + QDateTime::fromSecsSinceEpoch(QDateTime::fromString(curindex.sibling(curindex.row(), 3).data().toString(), "MM/dd/yyyy hh:mm:ss AP").toSecsSinceEpoch(), tmpzone).toString("MM/dd/yyyy hh:mm:ss AP") + "</td></tr>";
        filestr += "<tr class='even'><td class='pvalue'>ID:</td><td class='property'>" + curindex.sibling(curindex.row(), 11).data().toString() + "</td></tr>";
        filestr += "<tr class='odd'><td class='pvalue'>&nbsp;</td><td class='lvalue'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href='javascript:void(0)' onclick='ShowContent(\"./files/" + curindex.sibling(curindex.row(), 11).data().toString() + "\")'>Link</a></td></tr>";
        filestr += "</table></td>";
        RemoveFileItem(curindex.sibling(curindex.row(), 11).data().toString());
        AddFileItem(tagname, filestr);
	*/

	htmlentry = "";
	htmlentry += "<td class='fitem' id='" + this->windowTitle().mid(16) + "|" + ui->label->text() + "\\" + ui->tableWidget->selectedItems().first()->text() + "'>";
	htmlentry += "<table width='300px'><tr><th colspan='2'>" + ui->tableWidget->selectedItems().first()->text() + "</th></tr>";
	htmlentry += "<tr class='odd vtop'><td class='pvalue'>Path:</td><td class='property'><span style='word-wrap:break-word;'>" + ui->label->text() + "</span></td></tr>";
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
	htmlentry += "<tr class='even'><td class='pvalue'>Last Modified:</td><td class=;property'>" + ConvertWindowsTimeToUnixTime(lastwritetime) + "</td></tr>";
	htmlentry += "<tr class='odd'><td class='pvalue'>ID:</td><td class='property'>" + this->windowTitle().mid(16) + "</td></tr>";
        htmlentry += "<tr class='even'><td class='pvalue'>&nbsp;</td><td class='lvalue'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href='javascript:void(0)' onclick='ShowContent(\"./registry/" + this->windowTitle().mid(16) + "." + ui->label->text().replace("\\", "-") + "-" + ui->tableWidget->selectedItems().first()->text() + "\")'>Link</a></td></tr>";
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
            }
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

    	htmlvalue = "";
	QFile initfile(":/html/artifactprephtml");
	initfile.open(QIODevice::ReadOnly);
	if(initfile.isOpen())
	    htmlvalue = initfile.readAll();
	initfile.close();
	htmlvalue += "<div id='infotitle'>Registry Analysis</div><br/>";
	htmlvalue += "<pre>";
	htmlvalue += valuedata;
	htmlvalue += "</pre>";
	htmlvalue += "</table></body></html>";

        libregf_value_free(&curval, &regerr);
        libregf_key_free(&curkey, &regerr);
        libregf_file_close(regfile, &regerr);
        libregf_file_free(&regfile, &regerr);
        libregf_error_free(&regerr);
    }
}

void RegistryDialog::KeySelected(void)
{
    int itemindex = 0;
    QTreeWidgetItem* curitem = ui->treeWidget->selectedItems().first();
    bool toplevel = false;
    QStringList pathitems;
    pathitems.clear();
    pathitems.append(curitem->text(itemindex));
    QTreeWidgetItem* parent;
    QTreeWidgetItem* child;
    child = curitem;
    while(toplevel == false)
    {
	parent = child->parent();
	if(parent == nullptr)
	    toplevel = true;
	else
	{
	    pathitems.append(parent->text(itemindex));
	    child = parent;
	}
    }
    // build path
    QString keypath = "";
    QChar sepchar = QChar(92);
    for(int i = pathitems.count() - 2; i > -1; i--)
    {
	keypath += "/" + pathitems.at(i);
    }
    keypath.replace("/", sepchar);
    // attempt to open by path...
    ui->label->setText(keypath);
    libregf_file_t* regfile = NULL;
    libregf_error_t* regerr = NULL;
    libregf_file_initialize(&regfile, &regerr);
    libregf_file_open(regfile, regfilepath.toStdString().c_str(), LIBREGF_OPEN_READ, &regerr);
    libregf_key_t* curkey = NULL;
    libregf_file_get_key_by_utf8_path(regfile, (uint8_t*)(keypath.toUtf8().data()), keypath.toUtf8().size(), &curkey, &regerr);
    // valid key, get values...
    int valuecount = 0;
    libregf_key_get_number_of_values(curkey, &valuecount, &regerr);
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
    }
    libregf_key_free(&curkey, &regerr);
    libregf_file_close(regfile, &regerr);
    libregf_file_free(&regfile, &regerr);
    libregf_error_free(&regerr);
}
void RegistryDialog::closeEvent(QCloseEvent* e)
{
    e->accept();
}

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

void RegistryDialog::PopulateChildKeys(libregf_key_t* curkey, QTreeWidgetItem* curitem, libregf_error_t* regerr)
{
    int subkeycount = 0;
    libregf_key_get_number_of_sub_keys(curkey, &subkeycount, &regerr);
    if(subkeycount > 0)
    {
	for(int i=0; i < subkeycount; i++)
	{
	    libregf_key_t* cursubkey = NULL;
	    libregf_key_get_sub_key(curkey, i, &cursubkey, &regerr);
	    size_t namesize = 0;
	    libregf_key_get_utf8_name_size(cursubkey, &namesize, &regerr);
	    uint8_t name[namesize];
	    libregf_key_get_utf8_name(cursubkey, name, namesize, &regerr);
	    QTreeWidgetItem* subitem = new QTreeWidgetItem(curitem);
	    subitem->setText(0, QString::fromUtf8(reinterpret_cast<char*>(name)));
	    curitem->addChild(subitem);
	    int subsubkeycount = 0;
	    libregf_key_get_number_of_sub_keys(cursubkey, &subsubkeycount, &regerr);
	    if(subsubkeycount > 0)
	    {
		PopulateChildKeys(cursubkey, subitem, regerr);
	    }
	    libregf_key_free(&cursubkey, &regerr);
	}
    }
}

QString RegistryDialog::DecryptRot13(QString encstr)
{
    QString decstr = "";
    int i = 0;
    int strlength = 0;
    strlength = encstr.count();
    decstr = encstr;
    for(i = 0; i < strlength; i++)
    {
        decstr[i] = Rot13Char(decstr.at(i));
    }
    return decstr;
}

QChar RegistryDialog::Rot13Char(QChar curchar)
{
    QChar rot13char;
    if('0' <= curchar && curchar <= '4')
        rot13char = QChar(curchar.unicode() + 5);
    else if('5' <= curchar && curchar <= '9')
        rot13char = QChar(curchar.unicode() - 5);
    else if('A' <= curchar && curchar <= 'M')
        rot13char = QChar(curchar.unicode() + 13);
    else if('N' <= curchar && curchar <= 'Z')
        rot13char = QChar(curchar.unicode() - 13);
    else if('a' <= curchar && curchar <= 'm')
        rot13char = QChar(curchar.unicode() + 13);
    else if('n' <= curchar && curchar <= 'z')
        rot13char = QChar(curchar.unicode() - 13);
    else
        rot13char = curchar;
    return rot13char;
}

/*
void RegistryDialog::DoubleClick(QTableWidgetItem* curitem)
{
    qDebug() << "Double click...";
}
*/

void RegistryDialog::TagMenu(const QPoint &pt)
{
    // when i need the current value for the right click, i can use a class variable defined in .h so i can access it in the SetTag and CreateNewTag right click menu options...
    //QTableWidgetItem* curitem = ui->tableWidget->itemAt(pt);
    //qDebug() << "cur item:" << ui->tableWidget->item(curitem->row(), 0)->text();
    tagmenu->exec(ui->tableWidget->mapToGlobal(pt));
}
/*
void WombatForensics::TreeContextMenu(const QPoint &pt)
{
    QModelIndex index = ui->dirTreeView->indexAt(pt);
    if(index.isValid())
    {
        actionitem = static_cast<TreeNode*>(index.internalPointer());
        if(!actionitem->IsChecked())
        {
            ui->actionCheck->setText("Check Selected");
            ui->actionCheck->setIcon(QIcon(":/echeck"));
        }
        else
        {
            ui->actionCheck->setText("UnCheck Selected");
            ui->actionCheck->setIcon(QIcon(":/remcheck"));
        }
        treemenu->exec(ui->dirTreeView->mapToGlobal(pt));
    }
}
*/
