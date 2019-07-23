#include "addevidencedialog.h"

// Copyright 2013-2019 Pasquale J. Rinaldi, Jr.
// Distrubted under the terms of the GNU General Public License version 2

AddEvidenceDialog::AddEvidenceDialog(QWidget* parent) : QDialog(parent), ui(new Ui::AddEvidenceDialog)
{
    ui->setupUi(this);
    ui->removebutton->setEnabled(false);
    ui->startbutton->setEnabled(false);
    connect(ui->addbutton, SIGNAL(clicked()), this, SLOT(SelectEvidence()));
    connect(ui->removebutton, SIGNAL(clicked()), this, SLOT(RemoveEvidence()));
    connect(ui->cancelbutton, SIGNAL(clicked()), this, SLOT(Cancel()));
    connect(ui->startbutton, SIGNAL(clicked()), this, SLOT(StartProcess()));
    connect(ui->evidencelist, SIGNAL(itemSelectionChanged()), this, SLOT(UpdateButtons()));
}

AddEvidenceDialog::~AddEvidenceDialog()
{
}

void AddEvidenceDialog::SelectEvidence()
{
    QFileDialog addeviddialog(this, tr("Add Evidence Item"), QDir::homePath());
    addeviddialog.setFileMode(QFileDialog::ExistingFile);
    addeviddialog.setLabelText(QFileDialog::Accept, "Add");
    addeviddialog.setOption(QFileDialog::DontUseNativeDialog, true);
    QString evidfilename = "";
    bool alreadyadded = false;
    if(addeviddialog.exec())
        evidfilename = addeviddialog.selectedFiles().first();
    //for(int i=0; i < evidencelist.count();i++)
    for(int i=0; i < existingevidence.count();i++)
    {
        if(existingevidence.at(i).contains(evidfilename))
            alreadyadded = true;
    }
    if(alreadyadded == false)
    {
        if(evidfilename.toLower().contains(".dd") || evidfilename.toLower().contains(".e01") || evidfilename.toLower().contains(".000") || evidfilename.toLower().contains(".001") || evidfilename.toLower().contains(".aff") || evidfilename.toLower().contains(".image"))
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
    else
        QMessageBox::warning(this, tr("Already Added"), tr("The evidence has already been added to the case, please select another item."), QMessageBox::Ok);
}

void AddEvidenceDialog::UpdateButtons()
{
    if(ui->evidencelist->selectedItems().count() > 0)
        ui->removebutton->setEnabled(true);
}

void AddEvidenceDialog::RemoveEvidence()
{
    qDeleteAll(ui->evidencelist->selectedItems());
    if(ui->evidencelist->count() == 0)
        ui->startbutton->setEnabled(false);
}

void AddEvidenceDialog::Cancel()
{
    this->close();
}

void AddEvidenceDialog::StartProcess()
{
    /*
    QStringList newevidlist;
    newevidlist.clear();
    for(int i=0; i < ui->evidencelist->count(); i++)
        newevidlist.append(ui->evidencelist->item(i)->text());
    emit SendNewEvidence(newevidlist);
    */
    this->close();
    /*
    evidencelist.clear();
    for(int i=0; i < ui->evidencelist->count(); i++)
    {
        evidencelist.append(ui->evidencelist->item(i)->text());
        this->close();
    }
    */
}

QStringList AddEvidenceDialog::SendNewEvidence()
{
    QStringList newevidlist;
    newevidlist.clear();
    for(int i=0; i < ui->evidencelist->count(); i++)
        newevidlist.append(ui->evidencelist->item(i)->text());

    return newevidlist;
}

void AddEvidenceDialog::dragEnterEvent(QDragEnterEvent* e)
{
    if(e->mimeData()->hasUrls())
        e->acceptProposedAction();
}

void AddEvidenceDialog::dropEvent(QDropEvent* e)
{
    /*
    bool alreadyadded = false;
    foreach (const QUrl &url, e->mimeData()->urls())
    {
        for(int i=0; i < evidencelist.count();i++)
        {
            if(evidencelist.at(i).contains(url.toLocalFile().split("/").last()))
                alreadyadded = true;
        }
        if(alreadyadded == false)
        {
            ui->evidencelist->addItem(url.toLocalFile());
            ui->startbutton->setEnabled(true);
            //qDebug() << "dropped filepath:" << url.toLocalFile();
        }
    }
    */
}
