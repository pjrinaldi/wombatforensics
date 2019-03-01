#include "digstatus.h"

// Copyright 2015 Pasquale J. Rinaldi, Jr.
// Distrubted under the terms of the GNU General Public License version 2

DigStatus::DigStatus(QWidget* parent) : QDialog(parent), ui(new Ui::DigStatus)
{
    ui->setupUi(this);
    //ui->checkBox->setVisible(false);
    this->hide();
}

DigStatus::~DigStatus()
{
    delete ui;
    this->close();
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
