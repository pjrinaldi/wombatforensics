#include "aboutbox.h"

// Copyright 2013-2019 Pasquale J. Rinaldi, Jr.
// Distrubted under the terms of the GNU General Public License version 2

AboutBox::AboutBox(QWidget* parent) : QDialog(parent), ui(new Ui::AboutBox)
{
    ui->setupUi(this);
    QString abouttext = "<h3>About WombatForensics v0.1</h3>";
    abouttext += "<h5>License: GPLv2</h5>";
    abouttext += "<4>Copyright 2013-2019 Pasquale J. Rinaldi, Jr.</h4>";
    abouttext += "<h5>Email: abizmalstudios@gmail.com</h5>";
    abouttext += "<p>This program incorporates modified code from the qhexedit2 project to implement the hexviewers.";
    abouttext += "qhexedit2 is provided under the gplv2.1. I also used Qt5.12.3, the sleuthkit c/c++ functions, libtar, and some of the treepata, elementary, and tango icons. I also make use of ewfmount and affuse programs.";
    abouttext += "The wombat icon was designed by Lindsey Pargman and a lot of the design implementation ideas were brainstormed with Mark Smith.</p>";
    ui->textlabel->setText(abouttext);
    this->hide();
}

AboutBox::~AboutBox()
{
}

void AboutBox::HideClicked()
{
    this->hide();
}

void AboutBox::ShowAbout()
{
    this->show();
}

void AboutBox::mousePressEvent(QMouseEvent* e)
{
    if(e->type() == QEvent::MouseButtonPress)
    {
        this->hide();
    }
}

