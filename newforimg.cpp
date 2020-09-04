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
    ui->sourcecombo->addItem(QString::fromStdString(std::string(tmpdevice->path)));
    while(tmpdevice != NULL)
    {
        tmpdevice = tmpdevice->next;
        if(tmpdevice != NULL)
            ui->sourcecombo->addItem(QString::fromStdString(std::string(tmpdevice->path)));
    }
    connect(ui->cancelbutton, SIGNAL(clicked()), this, SLOT(HideClicked()), Qt::DirectConnection);
    connect(ui->createbutton, SIGNAL(clicked()), this, SLOT(CreateImage()), Qt::DirectConnection);
    connect(ui->browsebutton, SIGNAL(clicked()), this, SLOT(GetFolder()), Qt::DirectConnection);
}

ForImgDialog::~ForImgDialog()
{
    delete ui;
}

void ForImgDialog::HideClicked()
{
    this->close();
}

void ForImgDialog::CreateImage()
{
    if(ui->rawradio->isChecked())
    {
	// NEED TO MOVE TO ANOTHER THREAD... AND IMPLEMENT VERIFICATION SO I CAN DO BOTH THE IMAGE AND THE SOURCE AT ONCE...
	// MODIFY VERIFY FUNCTION SO IT VERIFIES ONE ITEM ONLY, AND WE CAN STORE THE MD5 VALUE IN THE CHAR ARRAY I PASS...
	// char* imgbuf = new char[0];
	// imgbuf = new char[fsfile->meta->size];
	// delete[] imgbuf;

	unsigned long long totalbytes = 0;
	ReadBytes(ui->sourcecombo->currentText().toStdString(), QString(ui->pathedit->text() + "/" + ui->nameedit->text() + ".dd").toStdString());
	Verify(ui->sourcecombo->currentText().toStdString(), QString(ui->pathedit->text() + "/" + ui->nameedit->text() + ".dd").toStdString());
    }
    else if(ui->aff4radio->isChecked())
    {
        //qDebug() << "image type:" << "aff4 image";
    }
    else if(ui->sfsradio->isChecked())
    {
        //qDebug() << "image type:" << "sfs image";
    }
    // ensure values are valid and filled in/selected otherwise popup error and don't close...
    this->close();
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

