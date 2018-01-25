#include "exportdialog.h"
#include "ui_exportdialog.h"

// Copyright 2015 Pasquale J. Rinaldi, Jr.
// Distrubted under the terms of the GNU General Public License version 2

ExportDialog::ExportDialog(QWidget *parent, unsigned long long curcheckcount, unsigned long long curlistcount) :
    QDialog(parent),
    ui(new Ui::ExportDialog)
{
    ui->setupUi(this);
    parentwidget = parent;
    checkcount = curcheckcount;
    QString checktext = QString("Checked (") + QString::number(checkcount) + QString(")");
    listcount = curlistcount;
    QString listtext = QString("All (") + QString::number(listcount) + QString(")");
    ui->checkedFileRadioButton->setText(checktext);
    ui->listedFileRadioButton->setText(listtext);
    ui->exportButton->setEnabled(false);
    ui->selectedFileRadioButton->setChecked(true);
    if(checkcount <= 0)
        ui->checkedFileRadioButton->setEnabled(false);
    else
        ui->checkedFileRadioButton->setEnabled(true);
    if(listcount <= 0)
        ui->listedFileRadioButton->setEnabled(false);
    else
        ui->listedFileRadioButton->setEnabled(true);
    ui->includePathRadioButton->setChecked(true);
    connect(ui->browseButton, SIGNAL(clicked()), this, SLOT(SelectDirectory()));
    connect(ui->exportButton, SIGNAL(clicked()), this, SLOT(ExportFiles()));
}

ExportDialog::~ExportDialog()
{
    delete ui;
}

void ExportDialog::SelectDirectory()
{
    QString dirname = QFileDialog::getExistingDirectory(this, tr("Select Directory"), QDir::homePath(), QFileDialog::ShowDirsOnly);
    if(!dirname.isEmpty())
    {
        ui->exportDirectoryLineEdit->setText(dirname);
        ui->exportButton->setEnabled(true);
    }
}

void ExportDialog::ExportFiles()
{
    exportpath = ui->exportDirectoryLineEdit->text();
    if(ui->selectedFileRadioButton->isChecked())
        exporttype = 0;
    if(ui->checkedFileRadioButton->isChecked())
        exporttype = 1;
    if(ui->listedFileRadioButton->isChecked())
        exporttype = 2;
    if(ui->includePathRadioButton->isChecked())
        originalpath = true;
    if(ui->excludePathRadioButton->isChecked())
        originalpath = false;
    emit StartExport(exporttype, originalpath, exportpath);
    this->close();
}
