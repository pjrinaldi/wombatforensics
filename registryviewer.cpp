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
