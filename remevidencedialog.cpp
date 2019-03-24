#include "remevidencedialog.h"

// Copyright 2015 Pasquale J. Rinaldi, Jr.
// Distrubted under the terms of the GNU General Public License version 2

RemEvidenceDialog::RemEvidenceDialog(QWidget* parent) : QDialog(parent), ui(new Ui::RemEvidenceDialog)
{
    ui->setupUi(this);
    connect(ui->removebutton, SIGNAL(clicked()), this, SLOT(RemoveEvidence()));
    connect(ui->cancelbutton, SIGNAL(clicked()), this, SLOT(Cancel()));

    // POPULATE EVIDENCE LIST
    for(int i=0; i < evidencelist.count(); i++)
        ui->evidencelist->addItem(evidencelist.at(i));
}

RemEvidenceDialog::~RemEvidenceDialog()
{
}

/*
void AddEvidenceDialog::SelectEvidence()
{
    QFileDialog addeviddialog(this, tr("Add Evidence Item"), QDir::homePath());
    addeviddialog.setFileMode(QFileDialog::ExistingFile);
    addeviddialog.setLabelText(QFileDialog::Accept, "Add");
    QString evidfilename = "";
    if(addeviddialog.exec())
        evidfilename = addeviddialog.selectedFiles().first();
    //QString evidfilename = QFileDialog::getOpenFileName(this, tr("Add Evidence Item"), QDir::homePath());
    // check for evidence image
    if(evidfilename.toLower().contains(".dd") || evidfilename.toLower().contains(".e01") || evidfilename.toLower().contains(".000") || evidfilename.toLower().contains(".001") || evidfilename.toLower().contains(".aff"))
    {
        // it's an evidence image so process...
        ui->evidencelist->addItem(evidfilename);
        ui->startbutton->setEnabled(true);
    }
    else if(evidfilename.isNull())
    {
        qDebug() << "cancelled by the user.";
    }
    else
    {
        QMessageBox::warning(this, tr("Unsupported Format"), tr("Unfortunately, this format is not supported YET, it should be supported by v0.2"), QMessageBox::Ok);
        qDebug() << "Unfortunately this format is not supported YET, it should be supported by v0.2.";
    }
}

void AddEvidenceDialog::UpdateButtons()
{
    if(ui->evidencelist->selectedItems().count() > 0)
        ui->removebutton->setEnabled(true);
}
*/

void RemEvidenceDialog::RemoveEvidence()
{
    for(int i=0; i < ui->evidencelist->selectedItems().count(); i++)
    {
        qDebug() << ui->evidencelist->selectedItems().at(i)->text();
    }
    //So I need to return a list of the selected items
    //then i need to loop over these in the main thread and remove the items from the tree... or i can do it here...
    //
    //qDebug() << ui->evidencelist->selectedItems();
    //qDeleteAll(ui->evidencelist->selectedItems());
    /*
    if(ui->evidencelist->count() == 0)
        ui->startbutton->setEnabled(false);
    */
}

void RemEvidenceDialog::Cancel()
{
    this->close();
}
