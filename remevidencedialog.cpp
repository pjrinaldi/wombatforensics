#include "remevidencedialog.h"

// Copyright 2013-2019 Pasquale J. Rinaldi, Jr.
// Distrubted under the terms of the GNU General Public License version 2

RemEvidenceDialog::RemEvidenceDialog(QWidget* parent) : QDialog(parent), ui(new Ui::RemEvidenceDialog)
{
    ui->setupUi(this);
    connect(ui->removebutton, SIGNAL(clicked()), this, SLOT(RemoveEvidence()));
    connect(ui->cancelbutton, SIGNAL(clicked()), this, SLOT(Cancel()));

    // POPULATE EVIDENCE LIST
    qDebug() << "evidencelist:" << evidencelist;
    for(int i=0; i < evidencelist.count(); i++)
        ui->evidencelist->addItem(evidencelist.at(i));
}

RemEvidenceDialog::~RemEvidenceDialog()
{
}

void RemEvidenceDialog::RemoveEvidence()
{
    QStringList tmplist;
    tmplist.clear();
    for(int i=0; i < ui->evidencelist->selectedItems().count(); i++)
        tmplist.append(ui->evidencelist->selectedItems().at(i)->text());
    emit RemEvid(tmplist);
    this->close();
}

void RemEvidenceDialog::Cancel()
{
    this->close();
}
