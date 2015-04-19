#include "byteconverter.h"

ByteConverter::ByteConverter(QWidget* parent) : QDialog(parent), ui(new Ui::ByteConverter)
{
    ui->setupUi(this);
    this->hide();
}

ByteConverter::~ByteConverter()
{
    delete ui;
    this->close();
}

ByteConverter::HideClicked()
{
    this->hide();
    emit HideByteConverterWindow(false);
}

void ByteConverter::closeEvent(QCloseEvent* e)
{
    emit HideByteConverterWindow(false);
    e->accept();
}
