#include "byteconverter.h"

// Copyright 2015 Pasquale J. Rinaldi, Jr.
// Distrubted under the terms of the GNU General Public License version 2.1

ByteConverter::ByteConverter(QWidget* parent) : QDialog(parent), ui(new Ui::ByteConverter)
{
    ui->setupUi(this);
    ui->checkBox->setVisible(false);
    this->hide();
}

ByteConverter::~ByteConverter()
{
    delete ui;
    this->close();
}

void ByteConverter::HideClicked()
{
    this->hide();
    emit HideByteConverterWindow(false);
}

void ByteConverter::closeEvent(QCloseEvent* e)
{
    emit HideByteConverterWindow(false);
    e->accept();
}

void ByteConverter::SetText(QString txt)
{
    ui->label->setText(txt);
}
