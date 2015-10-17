#include "exportdialog.h"
#include "ui_exportdialog.h"

// Copyright 2015 Pasquale J. Rinaldi, Jr.
// Distrubted under the terms of the GNU General Public License version 2.1

ExportDialog::ExportDialog(QWidget *parent, unsigned long long curcheckcount, unsigned long long curlistcount) :
    QDialog(parent),
    ui(new Ui::ExportDialog)
{
    ui->setupUi(this);
    parentwidget = parent;
    checkcount = curcheckcount;
    QString checktext = QString("Checked (") + QString::number(checkcount) + QString(")");
    listcount = curlistcount;
    QString listtext = QString("Currently Listed (") + QString::number(listcount) + QString(")");
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
    FileExportData exportdata;
    exportdata.exportpath = ui->exportDirectoryLineEdit->text().toStdString();
    if(ui->selectedFileRadioButton->isChecked())
        exportdata.filestatus = FileExportData::selected;
    if(ui->checkedFileRadioButton->isChecked())
        exportdata.filestatus = FileExportData::checked;
    if(ui->listedFileRadioButton->isChecked())
        exportdata.filestatus = FileExportData::listed;
    if(ui->includePathRadioButton->isChecked())
        exportdata.pathstatus = FileExportData::include;
    if(ui->excludePathRadioButton->isChecked())
        exportdata.pathstatus = FileExportData::exclude;
    emit FileExport(&exportdata);
    this->close();
}
