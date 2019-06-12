#include "digstatus.h"

// Copyright 2015 Pasquale J. Rinaldi, Jr.
// Distrubted under the terms of the GNU General Public License version 2

DigStatus::DigStatus(QWidget* parent) : QDialog(parent), ui(new Ui::DigStatus)
{
    ui->setupUi(this);
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
        if(digstatecount == -1)
            imgthumbcount = imgthumbtotal;
        else
            imgthumbcount = digstatecount;
        imgthumbpercent = ((float)imgthumbcount/(float)imgthumbtotal) * 100.0;
        ui->imgthumblabel->setText("Generating Image Thumbnail: " + QString::number(imgthumbcount) + " of " + QString::number(imgthumbtotal) + " " + QString::number((int)imgthumbpercent) + "%");
    }
    else if(digstateid == 1 || digstateid == 2 || digstateid == 3) // md5 || sha1 || sha256
    {
        if(digstatecount == -1)
            hashcount = hashtotal;
        else
            hashcount = digstatecount;
        hashpercent = ((float)hashcount/(float)hashtotal) * 100.0;
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
        if(imgthumbtotal != 0)
            imgthumbpercent = ((float)imgthumbcount/(float)imgthumbtotal) * 100.0;
        else
            imgthumbtotal = 0;
        ui->imgthumblabel->setVisible(true);
        ui->imgthumbbutton->setVisible(true);
        ui->imgthumblabel->setText("Generating Image Thumbnail: " + QString::number(imgthumbcount) + " of " + QString::number(imgthumbtotal) + " " + QString::number((int)imgthumbpercent) + "%");
    }
    else if(digstateid == 1 || digstateid == 2 || digstateid == 3) // md5 || sha1 || sha256
    {
        hashcount = 0;
        hashtotal = digtotal;
        if(hashtotal != 0)
            hashpercent = ((float)hashcount/(float)hashtotal) * 100.0;
        else
            hashpercent = 0;
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
    ui->imgthumblabel->setText("<s>" + ui->imgthumblabel->text() + "</s>");
    emit CancelImgThumbThread();
}

void DigStatus::CancelHash()
{
    ui->hashlabel->setText("<s>" + ui->hashlabel->text() + "</s>");
    emit CancelHashThread();
}

void DigStatus::closeEvent(QCloseEvent* e)
{
    e->accept();
}
