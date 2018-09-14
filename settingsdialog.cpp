#include "settingsdialog.h"

// Copyright 2015 Pasquale J. Rinaldi, Jr.
// Distrubted under the terms of the GNU General Public License version 2

SettingsDialog::SettingsDialog(QWidget* parent) : QDialog(parent), ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);
    /*
    QString abouttext = "<h3>About WombatForensics v1.0</h3>";
    abouttext += "<h5>License: GPLv2</h5>";
    abouttext += "<4>Copyright 2015 Pasquale J. Rinaldi, Jr.</h4>";
    abouttext += "<h5>Email: pjrinaldi@gmail.com&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Phone: (865) 206-8130</h5>";
    abouttext += "<p>This program incorporates modified code from the lfhex project to implement the hexviewers.";
    abouttext += "lfhex is provided under the gplv2. I also used Qt5.5, the sleuthkit c/c++ functions, and the treepata icons.";
    abouttext += "The wombat icon was designed by Lindsey Pargman and a lot of the design implementation ideas were brainstormed with Mark Smith.</p>";
    ui->label->setText(abouttext);
    */
    this->hide();
}

SettingsDialog::~SettingsDialog()
{
}

void SettingsDialog::HideClicked()
{
    this->hide();
}

void SettingsDialog::ShowAbout()
{
    this->show();
}

void SettingsDialog::mousePressEvent(QMouseEvent* e)
{
    if(e->type() == QEvent::MouseButtonPress)
    {
        this->hide();
    }
}

