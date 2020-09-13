#include "verevidencedialog.h"

// Copyright 2013-2019 Pasquale J. Rinaldi, Jr.
// Distrubted under the terms of the GNU General Public License version 2

VerEvidenceDialog::VerEvidenceDialog(QWidget* parent) : QDialog(parent), ui(new Ui::VerEvidenceDialog)
{
    ui->setupUi(this);
    connect(ui->verifybutton, SIGNAL(clicked()), this, SLOT(VerifyEvidence()));
    connect(ui->cancelbutton, SIGNAL(clicked()), this, SLOT(Cancel()));

    // POPULATE EVIDENCE LIST
    //qDebug() << "existing evidence:" << existingevidence;
    for(int i=0; i < existingevidence.count(); i++)
        ui->evidencelist->addItem(existingevidence.at(i));
}

VerEvidenceDialog::~VerEvidenceDialog()
{
}

void VerEvidenceDialog::VerifyEvidence()
{
    QStringList tmplist;
    tmplist.clear();
    for(int i=0; i < ui->evidencelist->selectedItems().count(); i++)
        tmplist.append(ui->evidencelist->selectedItems().at(i)->text());
    emit VerEvid(tmplist);
    this->close();
}

void VerEvidenceDialog::Cancel()
{
    this->close();
}
