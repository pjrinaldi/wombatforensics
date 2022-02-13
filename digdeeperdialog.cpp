#include "digdeeperdialog.h"
#include "ui_digdeeperdialog.h"

// Copyright 2013-2022 Pasquale J. Rinaldi, Jr.
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
    ui->hashlistwidget->setEnabled(false);
    ui->selectedFileRadioButton->setChecked(true);
    if(checkcount <= 0)
        ui->checkedFileRadioButton->setEnabled(false);
    else
        ui->checkedFileRadioButton->setEnabled(true);
    if(listcount <= 0)
        ui->listedFileRadioButton->setEnabled(false);
    else
        ui->listedFileRadioButton->setEnabled(true);
    connect(ui->thumbnailcheckBox, SIGNAL(clicked(bool)), this, SLOT(EnableProcess(bool)));
    connect(ui->videocheckBox, SIGNAL(clicked(bool)), this, SLOT(EnableProcess(bool)));
    connect(ui->hashcheckbox, SIGNAL(clicked(bool)), this, SLOT(EnableProcess(bool)));
    connect(ui->expandarchivescheckbox, SIGNAL(clicked(bool)), this, SLOT(EnableProcess(bool)));
    connect(ui->hashlistcheckbox, SIGNAL(clicked(bool)), this, SLOT(EnableProcess(bool)));
    connect(ui->emailcheckbox, SIGNAL(clicked(bool)), this, SLOT(EnableProcess(bool)));
    connect(ui->processButton, SIGNAL(clicked()), this, SLOT(DigDeeperFiles()));
    // Get Hash Lists populated
    ui->hashlistwidget->clear();
    QDir hashdir(wombatvariable.tmpmntpath + "hashlists/");
    QFileInfoList whllist = hashdir.entryInfoList(QStringList() << "*.whl", QDir::Files);
    for(int i=0; i < whllist.count(); i++)
	new QListWidgetItem(whllist.at(i).fileName(), ui->hashlistwidget);
    if(ui->hashlistwidget->count() > 0)
	ui->hashlistcheckbox->setEnabled(true);
}

DigDeeperDialog::~DigDeeperDialog()
{
    delete ui;
}

void DigDeeperDialog::EnableProcess(bool checked)
{
    ui->processButton->setEnabled(checked);
    if(ui->hashlistcheckbox->isChecked())
    {
	ui->hashlistwidget->setEnabled(true);
    }
    else
	ui->hashlistwidget->setEnabled(false);
    if(ui->hashcheckbox->isChecked() || ui->thumbnailcheckBox->isChecked() || ui->videocheckBox->isChecked() || ui->expandarchivescheckbox->isChecked() || ui->hashlistcheckbox->isChecked() || ui->emailcheckbox->isChecked())
        ui->processButton->setEnabled(true);
}

void DigDeeperDialog::DigDeeperFiles()
{
    // POSSIBLY SET DIGOPTIONS TO BE BINARY ENUM 1, 2, 4, 8, 10, 20, 40, 80, 100, 200, 400, 800, 1000, 2000, 4000, 8000, ETC.
    if(ui->selectedFileRadioButton->isChecked())
        digtype = 0;
    if(ui->checkedFileRadioButton->isChecked())
        digtype = 1;
    if(ui->listedFileRadioButton->isChecked())
        digtype = 2;
    if(ui->thumbnailcheckBox->isChecked())
        digoptions.append(0);
    if(ui->videocheckBox->isChecked())
        digoptions.append(1);
    if(ui->hashcheckbox->isChecked())
    {
	digoptions.append(2);
    }
    if(ui->hashlistcheckbox->isChecked())
    {
	digoptions.append(3);
        for(int i=0; i < ui->hashlistwidget->selectedItems().count(); i++)
            knownhashlist.append(ui->hashlistwidget->selectedItems().at(i)->text());
    }
    if(ui->expandarchivescheckbox->isChecked())
        digoptions.append(4);
    if(ui->emailcheckbox->isChecked())
        digoptions.append(5);
    emit StartDig(digtype, digoptions);
    this->close();
}
