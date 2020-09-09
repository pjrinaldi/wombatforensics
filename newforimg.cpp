#include "newforimg.h"
#include "imagefunctions.h"

// Copyright 2013-2020 Pasquale J. Rinaldi, Jr.
// Distrubted under the terms of the GNU General Public License version 2

ForImgDialog::ForImgDialog(QWidget* parent) : QDialog(parent), ui(new Ui::ForImgDialog)
{
    ui->setupUi(this);
    devicelist.clear();
    ped_device_probe_all();
    PedDevice* tmpdevice = ped_device_get_next(NULL);
    //qDebug() << "ped device model:" << QString::fromStdString(std::string(tmpdevice->model));
    ui->sourcecombo->addItem(QString::fromStdString(std::string(tmpdevice->model)) + " (" + QString::fromStdString(std::string(tmpdevice->path) + ")"));
    while(tmpdevice != NULL)
    {
        tmpdevice = tmpdevice->next;
        if(tmpdevice != NULL)
            ui->sourcecombo->addItem(QString::fromStdString(std::string(tmpdevice->model)) + " (" + QString::fromStdString(std::string(tmpdevice->path) + ")"));
            //ui->sourcecombo->addItem(QString::fromStdString(std::string(tmpdevice->path)));
    }
    connect(ui->cancelbutton, SIGNAL(clicked()), this, SLOT(HideClicked()), Qt::DirectConnection);
    connect(ui->createbutton, SIGNAL(clicked()), this, SLOT(CreateImage()), Qt::DirectConnection);
    connect(ui->browsebutton, SIGNAL(clicked()), this, SLOT(GetFolder()), Qt::DirectConnection);
    connect(&imgwatcher, SIGNAL(finished()), this, SLOT(FinishImaging()), Qt::QueuedConnection);
    ui->aff4radio->setEnabled(false); // disabled until i get a way to fuse/tsk view/extract aff4 image.
}

ForImgDialog::~ForImgDialog()
{
    delete ui;
}

void ForImgDialog::HideClicked()
{
    this->close();
}

void ForImgDialog::FinishImaging()
{
    QMessageBox::information(this, "Finished", " Forensic Imaging completed.", QMessageBox::Ok);
}

void ForImgDialog::CreateImage()
{
    // NEED TO CHECK VALUES PRIOR TO LAUNCHING THIS AND POPUP A DIALOG WITH WHAT IS MISSING...
    if(ui->sourcecombo->currentText().contains("Select Source Drive") || ui->pathedit->text().isEmpty() || ui->nameedit->text().isEmpty() || (!ui->aff4radio->isChecked() && !ui->rawradio->isChecked( ) && !ui->sfsradio->isChecked()))
    {
        QMessageBox::critical(this, "Can't Create Forensic Image", "All Required Fields haven't been filled out.", QMessageBox::Ok);
    }
    else
    {
        int radio = 0;
        if(ui->rawradio->isChecked())
            radio = 0;
        else if(ui->aff4radio->isChecked())
            radio = 1;
        else if(ui->sfsradio->isChecked())
            radio = 2;
        QFuture<void> tmpfuture = QtConcurrent::run(StartImaging, ui->sourcecombo->currentText().toStdString(), ui->pathedit->text().toStdString(), ui->nameedit->text().toStdString(), radio);
        imgwatcher.setFuture(tmpfuture);
        this->close();
    }
}

void ForImgDialog::GetFolder()
{
    QString userdir = QFileDialog::getExistingDirectory(this, tr("Select Destination"), QDir::homePath(), QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    ui->pathedit->setText(userdir);
}
void ForImgDialog::closeEvent(QCloseEvent* e)
{
    e->accept();
}

