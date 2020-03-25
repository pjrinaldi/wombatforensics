#include "filecarvingdialog.h"
#include "ui_filecarvingdialog.h"

// Copyright 2013-2020 Pasquale J. Rinaldi, Jr.
// Distrubted under the terms of the GNU General Public License version 2

//FileCarvingDialog::FileCarvingDialog(QWidget *parent, qint64 curcheckcount, qint64 curlistcount) :
FileCarvingDialog::FileCarvingDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FileCarvingDialog)
{
    ui->setupUi(this);
    parentwidget = parent;
    connect(ui->processButton, SIGNAL(clicked()), this, SLOT(EnableProcess()));
    /*
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
    connect(ui->thumbnailcheckBox, SIGNAL(clicked(bool)), this, SLOT(EnableProcess(bool)));
    connect(ui->videocheckBox, SIGNAL(clicked(bool)), this, SLOT(EnableProcess(bool)));
    connect(ui->hashcheckbox, SIGNAL(clicked(bool)), this, SLOT(EnableProcess(bool)));
    connect(ui->processButton, SIGNAL(clicked()), this, SLOT(DigDeeperFiles()));
    */
}

FileCarvingDialog::~FileCarvingDialog()
{
    delete ui;
}

void FileCarvingDialog::EnableProcess(bool checked)
{
    this->close();
    //ui->processButton->setEnabled(checked);
    /*
    if(ui->hashcheckbox->isChecked())
    {
        ui->md5radiobutton->setEnabled(true);
        ui->sha1radiobutton->setEnabled(true);
        ui->sha256radiobutton->setEnabled(true);
    }
    if(ui->hashcheckbox->isChecked() || ui->thumbnailcheckBox->isChecked() || ui->videocheckBox->isChecked())
        ui->processButton->setEnabled(true);
    */
}

/*
void FileCarvingDialog::DigDeeperFiles()
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
    }
    emit StartDig(digtype, digoptions);
    this->close();
}
*/
