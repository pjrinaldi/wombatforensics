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
}

void TagManager::HideClicked()
{
    emit HideManagerWindow();
    this->close();
}

void TagManager::ModifyTag()
{
    QString selectedtag = ui->listWidget->currentItem()->text();
    int selectedtagid = ui->listWidget->currentRow();
    QString tmpstr = "";
    QString modtagname = "";
    QInputDialog* modtagdialog = new QInputDialog(this);
    modtagdialog->setCancelButtonText("Cancel");
    modtagdialog->setInputMode(QInputDialog::TextInput);
    modtagdialog->setLabelText("Modify Tag Name");
    modtagdialog->setOkButtonText("Modify");
    modtagdialog->setTextEchoMode(QLineEdit::Normal);
    modtagdialog->setWindowTitle("Modify Tag");
    modtagdialog->setTextValue(selectedtag);
    if(modtagdialog->exec())
        modtagname = modtagdialog->textValue();
    if(!modtagname.isEmpty())
    {
        ui->listWidget->currentItem()->setText(modtagname);
        for(int i=0; i < ui->listWidget->count(); i++)
            tmpstr += ((QListWidgetItem*)ui->listWidget->item(i))->text() + ",";
        bookmarkfile.open(QIODevice::WriteOnly | QIODevice::Text);
        if(bookmarkfile.isOpen())
            bookmarkfile.write(tmpstr.toStdString().c_str());
        bookmarkfile.close();
        UpdateList();
        UpdateTLinkItem(selectedtagid, selectedtag, modtagname);
        UpdateTagItem(selectedtagid, selectedtag, modtagname);
    }
}

void TagManager::RemoveTag()
{
    QString selectedtag = ui->listWidget->currentItem()->text();
    QString tmpstr = "";
    for(int i = 0; i < taglist.count(); i++)
    {
        if(taglist.at(i).contains(selectedtag))
            taglist.removeAt(i);
    }
    for(int i=0; i < taglist.count(); i++)
        tmpstr += taglist.at(i) + ",";
    bookmarkfile.open(QIODevice::WriteOnly | QIODevice::Text);
    if(bookmarkfile.isOpen())
        bookmarkfile.write(tmpstr.toStdString().c_str());
    bookmarkfile.close();
    //qDebug() << "tmpstr:" << tmpstr;
    ui->removebutton->setEnabled(false);
    UpdateList();
    RemoveTLinkItem(selectedtag);
    RemoveTagItem(selectedtag);
}

void TagManager::AddTag()
{
    QString tagname = "";
    QInputDialog* newtagdialog = new QInputDialog(this);
    newtagdialog->setCancelButtonText("Cancel");
    newtagdialog->setInputMode(QInputDialog::TextInput);
    newtagdialog->setLabelText("Enter Tag Name");
    newtagdialog->setOkButtonText("Create Tag");
    newtagdialog->setTextEchoMode(QLineEdit::Normal);
    newtagdialog->setWindowTitle("New Tag");
    if(newtagdialog->exec())
        tagname = newtagdialog->textValue();
    if(!tagname.isEmpty())
    {
        int tagid = UpdateBookmarkItems(tagname);
        emit ReadBookmarks();
        UpdateList();
        AddTLinkItem(tagid, tagname);
        AddTagItem(tagid, tagname);
    }
}

void TagManager::SelectionChanged()
{
    //qDebug() << "selectionid:" << ui->listWidget->currentRow();
    ui->removebutton->setEnabled(true);
}

void TagManager::UpdateList()
{
    ui->listWidget->clear();
    taglist.clear();
    tlist.clear();
    llist.clear();
    bookmarkfile.setFileName(wombatvariable.tmpmntpath + "bookmarks");
    bookmarkfile.open(QIODevice::ReadOnly | QIODevice::Text);
    if(bookmarkfile.isOpen())
        taglist = QString(bookmarkfile.readLine()).split(",", QString::SkipEmptyParts);
    bookmarkfile.close();
    //taglist.removeDuplicates();
    for(int i=0; i < taglist.count(); i++)
    {
        new QListWidgetItem(taglist.at(i), ui->listWidget);
    }
}

void TagManager::closeEvent(QCloseEvent* e)
{
    emit HideManagerWindow();
    e->accept();
}
