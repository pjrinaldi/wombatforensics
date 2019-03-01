#include "digstatus.h"

// Copyright 2015 Pasquale J. Rinaldi, Jr.
// Distrubted under the terms of the GNU General Public License version 2

DigStatus::DigStatus(QWidget* parent) : QDialog(parent), ui(new Ui::DigStatus)
{
    ui->setupUi(this);
    //ui->checkBox->setVisible(false);
    this->hide();
    // need to hide ui elements until i use them. also need to build in the cancel buttons with signals...
    ui->imgthumblabel->setVisible(false);
    ui->imgthumbbutton->setVisible(false);
    ui->hashlabel->setVisible(false);
    ui->hashbutton->setVisible(false);
    ui->label_3->setVisible(false);
    ui->toolButton_3->setVisible(false);
    connect(ui->imgthumbbutton, SIGNAL(clicked()), this, SLOT(CancelImgThumb()));
    connect(ui->hashbutton, SIGNAL(clicked()), this, SLOT(CancelHash()));
}

DigStatus::~DigStatus()
{
    delete ui;
    this->close();
}

void DigStatus::UpdateDigState(int digstateid, int digstatecount)
{
    if(digstateid == 0) // img thumbnail
    {
        imgthumbcount = digstatecount;
        imgthumbpercent = (imgthumbcount/imgthumbtotal) * 100;
        ui->imgthumblabel->setText("Generating Image Thumbnail: " + QString::number(imgthumbcount) + " of " + QString::number(imgthumbtotal) + " " + QString::number((int)imgthumbpercent) + "%");
    }
    else if(digstateid == 1 || digstateid == 2 || digstateid == 3) // md5 || sha1 || sha256
    {
        hashcount = digstatecount;
        hashpercent = (hashcount/hashtotal) * 100;
        if(digstateid == 1)
            hashstring = "MD5";
        else if(digstateid == 2)
            hashstring = "SHA1";
        else if(digstateid == 3)
            hashstring = "SHA256";
        hashstring += " Hashing: " + QString::number(hashcount) + " of " + QString::number(hashtotal) + " " + QString::number((int)hashpercent) + "%";
        ui->hashlabel->setText(hashstring);
    }
}

void DigStatus::SetInitialDigState(int digstateid, int digtotal)
{
    if(digstateid == 0) // img thumbnail
    {
        imgthumbcount = 0;
        imgthumbtotal = digtotal;
        imgthumbpercent = (imgthumbcount/imgthumbtotal) * 100;
        ui->imgthumblabel->setVisible(true);
        ui->imgthumbbutton->setVisible(true);
        ui->imgthumblabel->setText("Generating Image Thumbnail: " + QString::number(imgthumbcount) + " of " + QString::number(imgthumbtotal) + " " + QString::number((int)imgthumbpercent) + "%");
    }
    else if(digstateid == 1 || digstateid == 2 || digstateid == 3) // md5 || sha1 || sha256
    {
        hashcount = 0;
        hashtotal = digtotal;
        hashpercent = (hashcount/hashtotal) * 100;
        if(digstateid == 1)
            hashstring = "MD5";
        else if(digstateid == 2)
            hashstring = "SHA1";
        else if(digstateid == 3)
            hashstring = "SHA256";
        hashstring += " Hashing: " + QString::number(hashcount) + " of " + QString::number(hashtotal) + " " + QString::number((int)hashpercent) + "%";
        ui->hashlabel->setVisible(true);
        ui->hashbutton->setVisible(true);
        ui->hashlabel->setText(hashstring);
    }
}

void DigStatus::CancelImgThumb()
{
    emit CancelImgThumbThread();
}

void DigStatus::CancelHash()
{
    emit CancelHashThread();
}

/*
void ByteConverter::HideClicked()
{
    this->hide();
    emit HideByteConverterWindow(false);
}
*/
void DigStatus::closeEvent(QCloseEvent* e)
{
    //emit HideByteConverterWindow(false);
    e->accept();
}

/*
void ByteConverter::SetText(QString txt)
{
    ui->label->setText(txt);
}
*/
