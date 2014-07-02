#include "exportdialog.h"
#include "ui_exportdialog.h"

ExportDialog::ExportDialog(QWidget *parent, int curcheckcount, int curlistcount) :
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
        fprintf(stderr, "Set Directory: %s\n", dirname.toStdString().c_str());
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
