#include "registryviewer.h"

// Copyright 2013-2020 Pasquale J. Rinaldi, Jr.
// Distrubted under the terms of the GNU General Public License version 2

RegistryDialog::RegistryDialog(QWidget* parent) : QDialog(parent), ui(new Ui::RegistryDialog)
{
    ui->setupUi(this);
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

void RegistryDialog::closeEvent(QCloseEvent* e)
{
    e->accept();
}

void RegistryDialog::LoadRegistryFile(QString regid)
{
    int retval = 0;
    qDebug() << "regid:" << regid;
    libregf_file_t* regfile = NULL;
    libregf_error_t* regerr = NULL;
    libregf_file_initialize(&regfile, &regerr);
    QString regfilestr = wombatvariable.tmpfilepath + regid + "-fhex";
    retval = 0;
    retval = libregf_file_open(regfile, regfilestr.toStdString().c_str(), LIBREGF_OPEN_READ, &regerr);
    qDebug() << "open registry file retval:" << retval;
    libregf_error_fprint(regerr, stderr);
    libregf_key_t* rootkey = NULL;
    retval = libregf_file_get_root_key(regfile, &rootkey, &regerr);
    qDebug() << "Get root key return value:" << retval;
    libregf_error_fprint(regerr, stderr);
    int rootsubkeycnt = 0;
    libregf_key_get_number_of_sub_keys(rootkey, &rootsubkeycnt, &regerr);
    qDebug() << "root subkey count:" << rootsubkeycnt;
    libregf_error_fprint(regerr, stderr);
    uint64_t rootfiletime = 0;
    libregf_key_get_last_written_time(rootkey, &rootfiletime, &regerr);
    qDebug() << "root filetime:" << rootfiletime << ConvertWindowsTimeToUnixTimeUTC(rootfiletime);
    size_t namesize = 0;
    uint8_t name[namesize];
    libregf_key_get_name_size(rootkey, &namesize, &regerr);
    libregf_key_get_name(rootkey, name, namesize, &regerr);
    qDebug() << "key name:" << QString::fromUtf8(reinterpret_cast<char*>(name));
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
    libregf_key_get_last_written_time(curkey, &lasttime, &regerr);
    qDebug() << "last time:" << ConvertWindowsTimeToUnixTimeUTC(lasttime);
    qDebug() << "curitemtext:" << curitem->text(0);
    // get sub key count
    // loop over sub keys...
    // populate to tree..
    // will probably need a storage method for this, so when i select on it, it display value where i need it...
    // if key has its own subkeys, then PopulateChildKeys(curkey, curitem, regerr);
    // libregf_key_free(curkey, &regerr);
}
