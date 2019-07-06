#include "tagmanager.h"

// Copyright 2013-2019 Pasquale J. Rinaldi, Jr.
// Distrubted under the terms of the GNU General Public License version 2

TagManager::TagManager(QWidget* parent) : QDialog(parent), ui(new Ui::TagManager)
{
    ui->setupUi(this);
    connect(ui->newbutton, SIGNAL(clicked()), this, SLOT(AddTag()));
    connect(ui->modifybutton, SIGNAL(clicked()), this, SLOT(ModifyTag()));
    connect(ui->removebutton, SIGNAL(clicked()), this, SLOT(RemoveTag()));
    connect(ui->listWidget, SIGNAL(itemSelectionChanged()), this, SLOT(SelectionChanged()));
    ui->removebutton->setEnabled(false);
    UpdateList();
}

TagManager::~TagManager()
{
    delete ui;
    //this->close();
}

void TagManager::HideClicked()
{
    //this->hide();
    emit HideManagerWindow();
    this->close();
}

void TagManager::AddTag()
{
}

void TagManager::ModifyTag()
{
}

void TagManager::RemoveTag()
{
}

/*
void TagManager::ShowBrowser()
{
    fileviewerpath = QFileDialog::getOpenFileName(this, tr("Select Viewer Executable"), QDir::homePath(), "", NULL, QFileDialog::DontUseNativeDialog);
    if(!fileviewerpath.isNull())
    {
        ui->lineEdit->setText(fileviewerpath);
        ui->addbutton->setEnabled(true);
    }
}

void TagManager::AddViewer()
{
    viewerfile.open(QIODevice::Append);
    viewerfile.write(QString(fileviewerpath + ",").toStdString().c_str());
    viewerfile.close();
    ui->lineEdit->setText("");
    ui->addbutton->setEnabled(false);
    UpdateList();
}

void TagManager::RemoveSelected()
{
    QString selectedviewer = ui->listWidget->currentItem()->text();
    QString tmpstring;
    viewerfile.open(QIODevice::ReadOnly);
    QStringList tmplist = QString(viewerfile.readLine()).split(",", QString::SkipEmptyParts);
    viewerfile.close();
    for(int i=0; i < tmplist.count(); i++)
    {
        if(tmplist.at(i).contains(selectedviewer))
            tmplist.removeAt(i);
    }
    tmplist.removeDuplicates();
    for(int i=0; i < tmplist.count(); i++)
    {
        tmpstring.append(tmplist.at(i));
        tmpstring.append(",");
    }
    viewerfile.open(QIODevice::WriteOnly);
    viewerfile.write(tmpstring.toStdString().c_str());
    viewerfile.close();
    ui->removebutton->setEnabled(false);
    UpdateList();
}
*/

void TagManager::SelectionChanged()
{
    ui->removebutton->setEnabled(true);
}
void TagManager::UpdateList()
{
    QString debugstr;
    QStringList itemlist;
    itemlist.clear();
    ui->listWidget->clear();
    bookmarkfile.setFileName(wombatvariable.tmpmntpath + "bookmarks");
    bookmarkfile.open(QIODevice::ReadOnly | QIODevice::Text);
    if(bookmarkfile.isOpen())
        itemlist = QString(bookmarkfile.readLine()).split(",", QString::SkipEmptyParts);
    bookmarkfile.close();
    itemlist.removeDuplicates();
    for(int i=0; i < itemlist.count(); i++)
    {
        new QListWidgetItem(itemlist.at(i), ui->listWidget);
    }
}
