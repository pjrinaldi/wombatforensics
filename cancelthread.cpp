#include "cancelthread.h"

// Copyright 2015 Pasquale J. Rinaldi, Jr.
// Distrubted under the terms of the GNU General Public License version 2

CancelThread::CancelThread(QWidget* parent) : QDialog(parent), ui(new Ui::CancelThread)
{
    ui->setupUi(this);
    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(StopThread()));
    this->hide();
}

CancelThread::~CancelThread()
{
}

void CancelThread::HideClicked()
{
    this->hide();
}

void CancelThread::ShowCancelThread()
{
    this->show();
}
void CancelThread::StopThread()
{
    int ret = QMessageBox::question(this, tr("Cancel Current Processing"), tr("You currently processing evidence. Are you sure you want to cancel it?"), QMessageBox::Yes | QMessageBox::No);
    if(ret == QMessageBox::Yes)
    {
        emit CancelCurrentThread();
        this->hide();
    }
}
