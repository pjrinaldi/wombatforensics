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
    for(int i=0; i < existingforimglist.count(); i++)
	ui->evidencelist->addItem(existingforimglist.at(i)->ImgPath());
    /*
    for(int i=0; i < existingevidence.count(); i++)
        ui->evidencelist->addItem(existingevidence.at(i));
    */
}

VerEvidenceDialog::~VerEvidenceDialog()
{
}

void VerEvidenceDialog::VerifyEvidence()
{
    QStringList tmplist;
    tmplist.clear();
    for(int i=0; i < ui->evidencelist->selectedItems().count(); i++)
    {
        // ADD PROGRESS BARS HERE
        // will have to be like the tmp menu's, which i can access by some value maybe
        //ui->progressgroupbox
        tmplist.append(ui->evidencelist->selectedItems().at(i)->text());
    }
    emit VerEvid(tmplist);
    //this->hide();
    //emit HideVerifyWindow(false);
}

void VerEvidenceDialog::Cancel()
{
    this->hide();
    emit HideVerifyWindow(false);
    //this->close();
}

void VerEvidenceDialog::Show()
{
    ui->evidencelist->clear();
    for(int i=0; i < existingforimglist.count(); i++)
	ui->evidencelist->addItem(existingforimglist.at(i)->ImgPath());
    this->show();
}
