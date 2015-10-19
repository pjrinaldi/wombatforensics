#include "digdeeperdialog.h"
#include "ui_digdeeperdialog.h"

// Copyright 2015 Pasquale J. Rinaldi, Jr.
// Distrubted under the terms of the GNU General Public License version 2

DigDeeperDialog::DigDeeperDialog(QWidget *parent, unsigned long long curcheckcount, unsigned long long curlistcount) :
    QDialog(parent),
    ui(new Ui::DigDeeperDialog)
{
    ui->setupUi(this);
    parentwidget = parent;
    checkcount = curcheckcount;
    QString checktext = QString("Checked (") + QString::number(checkcount) + QString(")");
    listcount = curlistcount;
    QString listtext = QString("Currently Listed (") + QString::number(listcount) + QString(")");
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
    connect(ui->md5checkBox, SIGNAL(clicked(bool)), this, SLOT(EnableProcess(bool)));
    connect(ui->thumbnailcheckBox, SIGNAL(clicked(bool)), this, SLOT(EnableProcess(bool)));
    connect(ui->processButton, SIGNAL(clicked()), this, SLOT(DigDeeperFiles()));
}

DigDeeperDialog::~DigDeeperDialog()
{
    delete ui;
}

void DigDeeperDialog::EnableProcess(bool checked)
{
    ui->processButton->setEnabled(checked);
}

void DigDeeperDialog::DigDeeperFiles()
{
    FileDeepData exportdata;
    if(ui->selectedFileRadioButton->isChecked())
        exportdata.filestatus = FileDeepData::selected;
    if(ui->checkedFileRadioButton->isChecked())
        exportdata.filestatus = FileDeepData::checked;
    if(ui->listedFileRadioButton->isChecked())
        exportdata.filestatus = FileDeepData::listed;
    if(ui->md5checkBox->isChecked())
        exportdata.digoptions.push_back(FileDeepData::md5);
    if(ui->thumbnailcheckBox->isChecked())
        exportdata.digoptions.push_back(FileDeepData::thumb);
    emit FileDig(&exportdata);
    this->close();
}
