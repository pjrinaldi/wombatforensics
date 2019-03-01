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
}

DigStatus::~DigStatus()
{
    delete ui;
    this->close();
}

void DigStatus::UpdateDigState(int digstateid, int digstatecount)
{
}

void DigStatus::SetIntitialDigState(int digstateid, int digtotal)
{
    if(digstateid == 0) // img thumbnail
    {
        hashcount = 0;
        hashtotal = digtotal;
        hashpercent = (hashcount/hashtotal) * 100;
        ui->imgthumblabel->setVisible(true);
        ui->imgthumbbutton->setVisible(true);
        ui->imgthumblabel->setText("Generating Image Thumbnail: " + QString::number(hashcount) + " of " + QString::number(hashtotal) + " " + QString::number((int)hashpercent) + "%");
    }
    else if(digstateid == 1) // md5
    {
        ui->hashlabel->setVisible(true);
        ui->hashbutton->setVisible(true);
    }
    else if(digstateid == 2) // sha1
    {
        ui->hashlabel->setVisible(true);
        ui->hashbutton->setVisible(true);
    }
    else if(digstateid == 3) // sha256
    {
        ui->hashlabel->setVisible(true);
        ui->hashbutton->setVisible(true);
    }
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
