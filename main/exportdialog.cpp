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
    QString listtext = QString("Listed (") + QString::number(listcount) + QString(")");
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
    fprintf(stderr, "Export Files: \n");
    // send the choices back-> which set of files (selected,checked,listed), export path string, whether to include original path (include, exclude)
    this->close();
}
