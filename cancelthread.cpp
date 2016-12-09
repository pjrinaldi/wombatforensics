#include "cancelthread.h"

// Copyright 2015 Pasquale J. Rinaldi, Jr.
// Distrubted under the terms of the GNU General Public License version 2

CancelThread::CancelThread(QWidget* parent) : QDialog(parent), ui(new Ui::CancelThread)
{
    ui->setupUi(this);
    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(StopThread()));
    /*
    QString abouttext = "<h3>About WombatForensics v1.0</h3>";
    abouttext += "<h5>License: GPLv2</h5>";
    abouttext += "<4>Copyright 2015 Pasquale J. Rinaldi, Jr.</h4>";
    abouttext += "<h5>Email: pjrinaldi@gmail.com&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Phone: (865) 206-8130</h5>";
    abouttext += "<p>This program incorporates modified code from the lfhex project to implement the hexviewers.";
    abouttext += "lfhex is provided under the gplv2. I also used Qt5.5, QtAV, the sleuthkit c/c++ functions, and the treepata icons.";
    abouttext += "The wombat icon was designed by Lindsey Pargman and a lot of the design implementation ideas were brainstormed with Mark Smith.</p>";
    ui->label->setText(abouttext);
    */
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
/*
void CancelThread::mousePressEvent(QMouseEvent* e)
{
    if(e->type() == QEvent::MouseButtonPress)
    {
        this->hide();
    }
}
*/
