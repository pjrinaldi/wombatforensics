#include "settingsdialog.h"

// Copyright 2018 Pasquale J. Rinaldi, Jr.
// Distrubted under the terms of the GNU General Public License version 2

SettingsDialog::SettingsDialog(QWidget* parent) : QDialog(parent), ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);
    connect(ui->cancelbutton, SIGNAL(clicked()), this, SLOT(CancelChanges()));
    connect(ui->savebutton, SIGNAL(clicked()), this, SLOT(SaveChanges()));
    LoadSettings();
    this->hide();
}

SettingsDialog::~SettingsDialog()
{
}

void SettingsDialog::SaveChanges()
{
    thumbsize = ui->thumbnailspinbox->value();
    // repeat this process for other variables..
    settingsfile.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&settingsfile);
    out << "thumb:" << QString::number(ui->thumbnailspinbox->value()) << ",";
    settingsfile.close();
    this->hide();
}
void SettingsDialog::CancelChanges()
{
    this->hide();
}

void SettingsDialog::LoadSettings()
{
    settingsfile.open(QIODevice::ReadOnly | QIODevice::Text);
    QStringList tmplist = QString(settingsfile.readLine()).split(",", QString::SkipEmptyParts);
    settingsfile.close();
    for(int i = 0; i < tmplist.count(); i++)
    {
        if(tmplist.at(i).split(":").at(0) == "thumb")
            ui->thumbnailspinbox->setValue(tmplist.at(i).split(":").at(1).toInt());
    }
}
