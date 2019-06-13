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
    if(addeviddialog.exec())
        evidfilename = addeviddialog.selectedFiles().first();
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
    evidencelist.clear();
    for(int i=0; i < ui->evidencelist->count(); i++)
    {
        evidencelist.append(ui->evidencelist->item(i)->text());
        this->close();
    }
}

void AddEvidenceDialog::dragEnterEvent(QDragEnterEvent* e)
{
    if(e->mimeData()->hasUrls())
        e->acceptProposedAction();
}

void AddEvidenceDialog::dropEvent(QDropEvent* e)
{
    foreach (const QUrl &url, e->mimeData()->urls())
    {
        ui->evidencelist->addItem(url.toLocalFile());
        ui->startbutton->setEnabled(true);
        //qDebug() << "dropped filepath:" << url.toLocalFile();
    }
}
