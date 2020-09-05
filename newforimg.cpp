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

void ForImgDialog::CreateImage()
{
    // NEED TO CHECK VALUES PRIOR TO LAUNCHING THIS AND POPUP A DIALOG WITH WHAT IS MISSING...
    if(ui->sourcecombo->currentText().contains("Select Source Drive") || ui->pathedit->text().isEmpty() || ui->nameedit->text().isEmpty() || (!ui->aff4radio->isChecked() && !ui->rawradio->isChecked( ) && !ui->sfsradio->isChecked()))
    {
        QMessageBox::critical(this, "Can't Create Image", "All Required Fields haven't been filled out.", QMessageBox::Ok);
        //int ret = QMessageBox::warning(this, tr("My Application"), tr("The document has been modified.\nDo you want to save your changes?"), QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel, QMessageBox::Save);
        qDebug() << "there are issues with the fields not filled out...";
    }
    else
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
            //std::string imgmd5 = Verify(ui->sourcecombo->currentText().toStdString());
            Verify(ui->sourcecombo->currentText().toStdString(), QString(ui->pathedit->text() + "/" + ui->nameedit->text() + ".dd").toStdString());
        }
        else if(ui->aff4radio->isChecked())
        {
            ReadBytes(ui->sourcecombo->currentText().toStdString(), QString(ui->pathedit->text() + "/" + ui->nameedit->text() + ".dd").toStdString());
            Verify(ui->sourcecombo->currentText().toStdString(), QString(ui->pathedit->text() + "/" + ui->nameedit->text() + ".dd").toStdString());
            QString aff4cmd = QDir::homePath() + "/.local/share/wombatforensics/linpmem";
            aff4cmd += " -i " + ui->pathedit->text() + "/" + ui->nameedit->text() + ".dd -o " + ui->pathedit->text() + "/" + ui->nameedit->text() + ".aff4";
            system(aff4cmd.toStdString().c_str());
            std::remove(QString(ui->pathedit->text() + "/" + ui->nameedit->text() + ".dd").toStdString().c_str());
            // AFF4 HAS NO FUSE RIGHT NOW, SO I WOULD EITHER NEED TO MOUNT IT OR EXPORT THE DD FILE
            /*
            QString xchompstr = QDir::homePath();
            xchompstr += "/.local/share/wombatforensics/xchomp";
            QProcess* process = new QProcess(this);
            process->startDetached(xchompstr, QStringList());
            */
            //qDebug() << "image type:" << "aff4 image";
        }
        else if(ui->sfsradio->isChecked())
        {
            ReadBytes(ui->sourcecombo->currentText().toStdString(), QString(ui->pathedit->text() + "/" + ui->nameedit->text() + ".dd").toStdString());
            Verify(ui->sourcecombo->currentText().toStdString(), QString(ui->pathedit->text() + "/" + ui->nameedit->text() + ".dd").toStdString());
            QString sqshcmd = "mksquashfs " + ui->pathedit->text() + "/" + ui->nameedit->text() + ".dd " + ui->pathedit->text() + "/" + ui->nameedit->text() + ".sfs";
            //QString sqshcmd = "mksquashfs " + ui->pathedit->text() + "/" + ui->nameedit->text() + ".dd " + ui->pathedit->text() + "/" + ui->nameedit->text() + ".log " + ui->pathedit->text() + "/" + ui->nameedit->text() + ".sfs";
            //qDebug() << "squashcmd:" << sqshcmd;
            // add image and log to squashfs
            system(sqshcmd.toStdString().c_str());
            // WOULD NEED TO CAPTURE OUTPUT...IN A FILENAME.log WHICH I HAVEN'T CREATED YET... WILL WORK ON LATER...

            std::remove(QString(ui->pathedit->text() + "/" + ui->nameedit->text() + ".dd").toStdString().c_str());
            std::remove(QString(ui->pathedit->text() + "/" + ui->nameedit->text() + ".log").toStdString().c_str());
            printf("Squashfs'd Forensic Image Finished Successfully.\n");
            //int fd = open("destfilepath", O_CREAT | O_TRUNC | O_RDWR, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
            //qDebug() << "image type:" << "sfs image";
            // SFS WOULD BE UNSQUASHFS TO GET RAW .DD FILE IN A TMP LOCATION AND THEN PARSE IT...
            // OR I COULD MOUNT SQUASHFS TO A TMP LOCATION AND THEN PARSE IT THAT WAY
        }
        // ensure values are valid and filled in/selected otherwise popup error and don't close...
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

