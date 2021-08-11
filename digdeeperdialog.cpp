#include "digdeeperdialog.h"
#include "ui_digdeeperdialog.h"

// Copyright 2013-2019 Pasquale J. Rinaldi, Jr.
// Distrubted under the terms of the GNU General Public License version 2

DigDeeperDialog::DigDeeperDialog(QWidget *parent, qint64 curcheckcount, qint64 curlistcount) :
    QDialog(parent),
    ui(new Ui::DigDeeperDialog)
{
    ui->setupUi(this);
    parentwidget = parent;
    checkcount = curcheckcount;
    QString checktext = QString("Checked (") + QString::number(checkcount) + QString(")");
    listcount = curlistcount;
    QString listtext = QString("All (") + QString::number(listcount) + QString(")");
    ui->checkedFileRadioButton->setText(checktext);
    ui->listedFileRadioButton->setText(listtext);
    ui->processButton->setEnabled(false);
    ui->selectedFileRadioButton->setChecked(true);
    if(checkcount <= 0)
        ui->checkedFileRadioButton->setEnabled(false);
    else
        ui->checkedFileRadioButton->setEnabled(true);
    if(listcount <= 0)
        ui->listedFileRadioButton->setEnabled(false);
    else
        ui->listedFileRadioButton->setEnabled(true);
    ui->md5radiobutton->setEnabled(false);
    ui->sha1radiobutton->setEnabled(false);
    ui->sha256radiobutton->setEnabled(false);
    ui->blake3radiobutton->setEnabled(false);
    connect(ui->thumbnailcheckBox, SIGNAL(clicked(bool)), this, SLOT(EnableProcess(bool)));
    connect(ui->videocheckBox, SIGNAL(clicked(bool)), this, SLOT(EnableProcess(bool)));
    connect(ui->hashcheckbox, SIGNAL(clicked(bool)), this, SLOT(EnableProcess(bool)));
    connect(ui->expandarchivescheckbox, SIGNAL(clicked(bool)), this, SLOT(EnableProcess(bool)));
    connect(ui->processButton, SIGNAL(clicked()), this, SLOT(DigDeeperFiles()));
}

DigDeeperDialog::~DigDeeperDialog()
{
    delete ui;
}

void DigDeeperDialog::EnableProcess(bool checked)
{
    ui->processButton->setEnabled(checked);
    if(ui->hashcheckbox->isChecked())
    {
        ui->md5radiobutton->setEnabled(true);
        ui->sha1radiobutton->setEnabled(true);
        ui->sha256radiobutton->setEnabled(true);
        ui->blake3radiobutton->setEnabled(true);
    }
    if(ui->hashcheckbox->isChecked() || ui->thumbnailcheckBox->isChecked() || ui->videocheckBox->isChecked() || ui->expandarchivescheckbox->isChecked())
        ui->processButton->setEnabled(true);
}

void DigDeeperDialog::DigDeeperFiles()
{
    if(ui->selectedFileRadioButton->isChecked())
        digtype = 0;
    if(ui->checkedFileRadioButton->isChecked())
        digtype = 1;
    if(ui->listedFileRadioButton->isChecked())
        digtype = 2;
    if(ui->thumbnailcheckBox->isChecked() && !ui->videocheckBox->isChecked())
        digoptions.append(0);
    else if(!ui->thumbnailcheckBox->isChecked() && ui->videocheckBox->isChecked())
        digoptions.append(4);
    else if(ui->thumbnailcheckBox->isChecked() && ui->videocheckBox->isChecked())
        digoptions.append(5);
    if(ui->hashcheckbox->isChecked())
    {
        if(ui->md5radiobutton->isChecked())
            digoptions.append(1);
        else if(ui->sha1radiobutton->isChecked())
            digoptions.append(2);
        else if(ui->sha256radiobutton->isChecked())
            digoptions.append(3);
        else if(ui->blake3radiobutton->isChecked())
            digoptions.append(7);
    }
    if(ui->expandarchivescheckbox->isChecked())
        digoptions.append(6);
    emit StartDig(digtype, digoptions);
    this->close();
}
