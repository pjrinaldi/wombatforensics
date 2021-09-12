#include "hashlistmanager.h"

// Copyright 2013-2020 Pasquale J. Rinaldi, Jr.
// Distrubted under the terms of the GNU General Public License version 2

HashListManager::HashListManager(QWidget* parent) : QDialog(parent), ui(new Ui::HashListManager)
{
    ui->setupUi(this);
    this->hide();
    connect(ui->closebutton, SIGNAL(clicked()), this, SLOT(HideClicked()));
    connect(ui->importbutton, SIGNAL(clicked()), this, SLOT(ImportHashList()));
    connect(ui->createbutton, SIGNAL(clicked()), this, SLOT(CreateHashList()));
    connect(ui->deletebutton, SIGNAL(clicked()), this, SLOT(DeleteSelectedList()));
    connect(ui->hashlistwidget, SIGNAL(itemSelectionChanged()), this, SLOT(SelectionChanged()));
    ui->deletebutton->setEnabled(false);
    UpdateList();
}

HashListManager::~HashListManager()
{
    delete ui;
    this->close();
}

void HashListManager::HideClicked()
{
    emit HideHashListManager();
    this->close();
}

void HashListManager::closeEvent(QCloseEvent* e)
{
    emit HideHashListManager();
    e->accept();
}

void HashListManager::ImportHashList()
{
    //qDebug() << "import hash list here.";
    QStringList files = QFileDialog::getOpenFileNames(this, "Select one or more files to import", QDir::homePath(), "Wombat Hash Lists (*.whl)");
    for(int i=0; i < files.count(); i++)
    {
        QFile tmpfile(files.at(i));
        tmpfile.copy(wombatvariable.tmpmntpath + "hashlists/" + files.at(i).split("/").last());
    }
    UpdateList();
}

void HashListManager::CreateHashList()
{
    QString emptyfilename = "";
    QInputDialog* newdialog = new QInputDialog(this);
    newdialog->setCancelButtonText("Cancel");
    newdialog->setInputMode(QInputDialog::TextInput);
    newdialog->setLabelText("Enter Hash List Name");
    newdialog->setOkButtonText("Create Empty List");
    newdialog->setTextEchoMode(QLineEdit::Normal);
    newdialog->setWindowTitle("Create Empty Wombat Hash List");
    if(newdialog->exec())
        emptyfilename = newdialog->textValue();
    if(!emptyfilename.isEmpty())
    {
        //qDebug() << "Create Empty Hash List.";
        if(!emptyfilename.endsWith(".whl"))
            emptyfilename = emptyfilename + ".whl";
        QFile tmpfile(wombatvariable.tmpmntpath + "hashlists/" + emptyfilename);
        tmpfile.open(QIODevice::WriteOnly | QIODevice::Text);
        tmpfile.close();
        ui->hashlistwidget->addItem(emptyfilename);
    }
}

void HashListManager::DeleteSelectedList()
{
    QListWidgetItem* curitem = ui->hashlistwidget->takeItem(ui->hashlistwidget->currentRow());
    QFile tmphashlistfile(wombatvariable.tmpmntpath + "hashlists/" + curitem->text());
    tmphashlistfile.remove();
    delete curitem;
}

/*
void HashListManager::ShowBrowser()
{
    fileviewerpath = QFileDialog::getOpenFileName(this, tr("Select Viewer Executable"), QDir::homePath(), "", NULL, QFileDialog::DontUseNativeDialog);
    if(!fileviewerpath.isNull())
    {
        ui->lineEdit->setText(fileviewerpath);
        ui->addbutton->setEnabled(true);
    }
}
*/

void HashListManager::SelectionChanged()
{
    ui->deletebutton->setEnabled(true);
}

void HashListManager::UpdateList()
{
    ui->hashlistwidget->clear();
    QDir hashdir(wombatvariable.tmpmntpath + "hashlists/");
    QFileInfoList whllist = hashdir.entryInfoList(QStringList() << "*.whl", QDir::Files);
    for(int i=0; i < whllist.count(); i++)
        ui->hashlistwidget->addItem(whllist.at(i).fileName());
}
