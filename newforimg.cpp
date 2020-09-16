#include "newforimg.h"
#include "imagefunctions.h"

// Copyright 2013-2020 Pasquale J. Rinaldi, Jr.
// Distrubted under the terms of the GNU General Public License version 2

ForImgDialog::ForImgDialog(QWidget* parent) : QDialog(parent), ui(new Ui::ForImgDialog)
{
    ui->setupUi(this);
    devicelist.clear();
    struct udev* udev;
    struct udev_device* dev;
    struct udev_enumerate* enumerate;
    struct udev_list_entry* devices;
    struct udev_list_entry* devlistentry;
    udev = udev_new();
    enumerate = udev_enumerate_new(udev);
    udev_enumerate_add_match_subsystem(enumerate, "block");
    udev_enumerate_scan_devices(enumerate);
    devices = udev_enumerate_get_list_entry(enumerate);
    udev_list_entry_foreach(devlistentry, devices)
    {
        QString tmpdevice = "";
        const char* path;
        const char* tmp;
        unsigned long long disksize = 0;
        unsigned short int blocksize = 512;
        path = udev_list_entry_get_name(devlistentry);
        dev = udev_device_new_from_syspath(udev, path);
        if(strncmp(udev_device_get_devtype(dev), "partition", 9) != 0 && strncmp(udev_device_get_sysname(dev), "loop", 4) != 0)
        {
            tmp = udev_device_get_sysattr_value(dev, "size");
            if(tmp)
                disksize = strtoull(tmp, NULL, 10);
            tmp = udev_device_get_sysattr_value(dev, "queue/logical_block_size");
            if(tmp)
                blocksize = atoi(tmp);
            unsigned long long disktotal = disksize * blocksize;
            int diskdigitcount = int(log10(disktotal) + 1);
            if(diskdigitcount > 9)
                tmpdevice += QString::number(disktotal / 1000000000) + " GB ";
            else if(diskdigitcount < 9)
                tmpdevice += QString::number(disktotal / 1000000) + " MB ";
            tmp = udev_device_get_property_value(dev, "ID_VENDOR");
            if(tmp)
                tmpdevice += QString::fromStdString(std::string(tmp)) + " ";
            tmp = udev_device_get_property_value(dev, "ID_MODEL");
            if(tmp)
                tmpdevice += QString::fromStdString(std::string(tmp)) + " ";
            tmp = udev_device_get_property_value(dev, "ID_NAME");
            if(tmp)
                tmpdevice += QString::fromStdString(std::string(tmp)) + " ";
            tmp = udev_device_get_property_value(dev, "ID_SERIAL_SHORT");
            if(tmp)
                tmpdevice += "[" + QString::fromStdString(std::string(tmp)) + "] ";
            tmp = udev_device_get_devnode(dev);
            if(tmp)
                tmpdevice += "(" + QString::fromStdString(std::string(tmp)) + ")";
            ui->sourcecombo->addItem(tmpdevice);
        }
        udev_device_unref(dev);
    }
    udev_enumerate_unref(enumerate);
    udev_unref(udev);

    //ped_device_probe_all();
    //PedDevice* tmpdevice = ped_device_get_next(NULL);
    //ui->sourcecombo->addItem(QString::fromStdString(std::string(tmpdevice->model)) + " (" + QString::fromStdString(std::string(tmpdevice->path) + ")"));
    /*
    while(tmpdevice != NULL)
    {
        tmpdevice = tmpdevice->next;
        if(tmpdevice != NULL)
            ui->sourcecombo->addItem(QString::fromStdString(std::string(tmpdevice->model)) + " (" + QString::fromStdString(std::string(tmpdevice->path) + ")"));
    }
    */
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

