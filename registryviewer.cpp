#include "registryviewer.h"

// Copyright 2013-2020 Pasquale J. Rinaldi, Jr.
// Distrubted under the terms of the GNU General Public License version 2

RegistryDialog::RegistryDialog(QWidget* parent) : QDialog(parent), ui(new Ui::RegistryDialog)
{
    ui->setupUi(this);
    connect(ui->treeWidget, SIGNAL(itemClicked(QTreeWidgetItem*, int)), this, SLOT(KeySelected(QTreeWidgetItem*, int)), Qt::DirectConnection);
    //connect(ui->treeWidget->selectionModel(), SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)), this, SLOT(SelectionChanged(const QItemSelection &, const QItemSelection &)), Qt::DirectConnection);
    /*
    connect(ui->cancelbutton, SIGNAL(clicked()), this, SLOT(HideClicked()));
    connect(ui->carvebutton, SIGNAL(clicked()), this, SLOT(Assign()));
    connect(ui->titlelineedit, SIGNAL(editingFinished()), this, SLOT(UpdateAssign()));
    ui->carvebutton->setEnabled(false);
    UpdateList();
    */
}

RegistryDialog::~RegistryDialog()
{
    delete ui;
}

void RegistryDialog::HideClicked()
{
    this->close();
}

void RegistryDialog::Assign()
{
    //qDebug() << ui->tagcombobox->currentText();
    //qDebug() << ui->titlelineedit->text();
    //emit TagCarved(ui->titlelineedit->text(), ui->tagcombobox->currentText());
    this->close();
}

void RegistryDialog::UpdateAssign()
{
    /*
    if(ui->titlelineedit->text().isEmpty())
        ui->carvebutton->setEnabled(false);
    else
        ui->carvebutton->setEnabled(true);
    */
}

void RegistryDialog::UpdateList()
{
    /*
    taglist.clear();
    bookmarkfile.setFileName(wombatvariable.tmpmntpath + "bookmarks");
    bookmarkfile.open(QIODevice::ReadOnly | QIODevice::Text);
    if(bookmarkfile.isOpen())
        taglist = QString(bookmarkfile.readLine()).split(",", Qt::SkipEmptyParts);
    bookmarkfile.close();
    for(int i=0; i < taglist.count(); i++)
    {
        ui->tagcombobox->addItem(taglist.at(i));
    }
    */
}

void RegistryDialog::KeySelected(QTreeWidgetItem* curitem, int itemindex)
{
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
    //qDebug() << "reverse path items:" << pathitems;
    // build path
    QString keypath = "";
    QChar sepchar = QChar(92);
    for(int i = pathitems.count() - 2; i > -1; i--)
    {
	keypath += "/" + pathitems.at(i);
	//qDebug() << "cur path item:" << pathitems.at(i);
    }
    //qDebug() << "sepchar:" << sepchar;
    //qDebug() << "keypath:" << keypath;
    keypath.replace("/", sepchar);
    //ui->plainTextEdit->setPlainText(keypath);
    //qDebug() << "keypath:" << keypath;
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
	size_t datasize = 0;
	libregf_value_get_value_data_size(curval, &datasize, &regerr);
	//qDebug() << "name size:" << namesize << "value name:" << QString::fromUtf8(reinterpret_cast<char*>(name)) << "value type:" << type << "data size:" << datasize;
	if(namesize == 0)
	{
	    ui->tableWidget->setHorizontalHeaderLabels({"Value Name", "Value"});
	    ui->tableWidget->setItem(i, 0, new QTableWidgetItem("(unnamed)"));
	    ui->tableWidget->setItem(i, 1, new QTableWidgetItem(QString::number(type, 16)));
	}
	else
	{
	    ui->tableWidget->setHorizontalHeaderLabels({"Value Name", "Value Type"});
	    ui->tableWidget->setItem(i, 0, new QTableWidgetItem(QString::fromUtf8(reinterpret_cast<char*>(name))));
	    ui->tableWidget->setItem(i, 1, new QTableWidgetItem(QString::number(type)));
	}
	libregf_value_free(&curval, &regerr);
    }
    //qDebug() << "value count:" << valuecount;
    //libregf_error_fprint(regerr, stderr);
    //size_t namesize = 0;
    //libregf_key_get_utf8_name_size(curkey, &namesize, &regerr);
    //libregf_error_fprint(regerr, stderr);
    //uint8_t name[namesize];
    //libregf_key_get_utf8_name(curkey, name, namesize, &regerr);
    //libregf_error_fprint(regerr, stderr);
    //qDebug() << "key name:" << QString::fromUtf8(reinterpret_cast<char*>(name));
    libregf_key_free(&curkey, &regerr);
    libregf_file_close(regfile, &regerr);
    libregf_file_free(&regfile, &regerr);
    libregf_error_free(&regerr);
}
void RegistryDialog::closeEvent(QCloseEvent* e)
{
    e->accept();
}

void RegistryDialog::LoadRegistryFile(QString regid)
{
    int retval = 0;
    //qDebug() << "regid:" << regid;
    libregf_file_t* regfile = NULL;
    libregf_error_t* regerr = NULL;
    libregf_file_initialize(&regfile, &regerr);
    QString regfilestr = wombatvariable.tmpfilepath + regid + "-fhex";
    regfilepath = regfilestr;
    retval = 0;
    retval = libregf_file_open(regfile, regfilestr.toStdString().c_str(), LIBREGF_OPEN_READ, &regerr);
    //qDebug() << "open registry file retval:" << retval;
    libregf_error_fprint(regerr, stderr);
    libregf_key_t* rootkey = NULL;
    retval = libregf_file_get_root_key(regfile, &rootkey, &regerr);
    //qDebug() << "Get root key return value:" << retval;
    libregf_error_fprint(regerr, stderr);
    int rootsubkeycnt = 0;
    libregf_key_get_number_of_sub_keys(rootkey, &rootsubkeycnt, &regerr);
    //qDebug() << "root subkey count:" << rootsubkeycnt;
    libregf_error_fprint(regerr, stderr);
    uint64_t rootfiletime = 0;
    libregf_key_get_last_written_time(rootkey, &rootfiletime, &regerr);
    //qDebug() << "root filetime:" << rootfiletime << ConvertWindowsTimeToUnixTimeUTC(rootfiletime);
    size_t namesize = 0;
    libregf_key_get_utf8_name_size(rootkey, &namesize, &regerr);
    uint8_t name[namesize];
    libregf_key_get_utf8_name(rootkey, name, namesize, &regerr);
    //qDebug() << "key name:" << QString::fromUtf8(reinterpret_cast<char*>(name));
    QTreeWidgetItem* rootitem = new QTreeWidgetItem(ui->treeWidget);
    rootitem->setText(0, QString::fromUtf8(reinterpret_cast<char*>(name)));
    ui->treeWidget->addTopLevelItem(rootitem);
    PopulateChildKeys(rootkey, rootitem, regerr);
    libregf_key_free(&rootkey, &regerr);
    libregf_file_close(regfile, &regerr);
    libregf_file_free(&regfile, &regerr);
    libregf_error_free(&regerr);
    this->show();
}

void RegistryDialog::PopulateChildKeys(libregf_key_t* curkey, QTreeWidgetItem* curitem, libregf_error_t* regerr)
{
    uint64_t lasttime = 0;
    int subkeycount = 0;
    libregf_key_get_last_written_time(curkey, &lasttime, &regerr);
    //qDebug() << "last time:" << ConvertWindowsTimeToUnixTimeUTC(lasttime);
    //qDebug() << "curitemtext:" << curitem->text(0);
    libregf_key_get_number_of_sub_keys(curkey, &subkeycount, &regerr);
    if(subkeycount > 0)
    {
	//qDebug() << "subkeycount:" << subkeycount;
	for(int i=0; i < subkeycount; i++)
	{
	    libregf_key_t* cursubkey = NULL;
	    libregf_key_get_sub_key(curkey, i, &cursubkey, &regerr);
	    size_t namesize = 0;
	    libregf_key_get_utf8_name_size(cursubkey, &namesize, &regerr);
	    uint8_t name[namesize];
	    libregf_key_get_utf8_name(cursubkey, name, namesize, &regerr);
	    //qDebug() << "subkey" << i << " name:" << QString::fromUtf8(reinterpret_cast<char*>(name));
	    QTreeWidgetItem* subitem = new QTreeWidgetItem(curitem);
	    subitem->setText(0, QString::fromUtf8(reinterpret_cast<char*>(name)));
	    curitem->addChild(subitem);
	    int subsubkeycount = 0;
	    libregf_key_get_number_of_sub_keys(cursubkey, &subsubkeycount, &regerr);
	    if(subsubkeycount > 0)
	    {
		//qDebug() << "get child keys for current subkey";
		PopulateChildKeys(cursubkey, subitem, regerr);
	    }
	    //QTreeWidgetItem* curitem = new QTreeWidgetItem(curitem);
	    //ui->treeWidget->
	    //curitem
	    libregf_key_free(&cursubkey, &regerr);
	}
    }
    // get sub key count
    // loop over sub keys...
    // populate to tree..
    // will probably need a storage method for this, so when i select on it, it display value where i need it...
    // if key has its own subkeys, then PopulateChildKeys(curkey, curitem, regerr);
    // libregf_key_free(curkey, &regerr);
}
