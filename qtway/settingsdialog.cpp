#include "settingsdialog.h"

// Copyright 2013-2020 Pasquale J. Rinaldi, Jr.
// Distrubted under the terms of the GNU General Public License version 2

SettingsDialog::SettingsDialog(QWidget* parent) : QDialog(parent), ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);
    filetypedialog = new CarvingFileTypesDialog(this);
    connect(ui->cancelbutton, SIGNAL(clicked()), this, SLOT(CancelChanges()));
    connect(ui->savebutton, SIGNAL(clicked()), this, SLOT(SaveChanges()));
    connect(ui->casepathbutton, SIGNAL(clicked()), this, SLOT(GetCaseFolder()));
    connect(ui->reportpathbutton, SIGNAL(clicked()), this, SLOT(GetReportFolder()));
    connect(ui->carvingfiletypepushbutton, SIGNAL(clicked()), filetypedialog, SLOT(ShowText()));
    zoneids.clear();
    zoneids = QTimeZone::availableTimeZoneIds();
    foreach(QByteArray id, zoneids)
        ui->timezonecombobox->addItem(id);
    LoadSettings();
    newtz = "";
    connect(ui->timezonecombobox, SIGNAL(currentTextChanged(QString)), this, SLOT(UpdateTimeZone(QString)));
    this->hide();
}

SettingsDialog::~SettingsDialog()
{
}

void SettingsDialog::SaveChanges()
{
    thumbsize = ui->thumbnailspinbox->value();
    vidcount = ui->videospinbox->value();
    autosave = ui->autosavespinbox->value();
    if(!ui->casepathlineedit->text().isEmpty())
        casepath = ui->casepathlineedit->text();
    if(casepath.endsWith("/"))
        casepath.chop(1);
    //qDebug() << "casepath:" << casepath;
    if(!ui->reportpathlineedit->text().isEmpty())
        reportpath = ui->reportpathlineedit->text();
    if(reportpath.endsWith("/"))
        reportpath.chop(1);
    QByteArray ba;
    ba.append(casepath.toUtf8());
    QByteArray ba2;
    ba2.append(reportpath.toUtf8());
    // repeat this process for other variables..
    settingsfile.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&settingsfile);
    out << "thumb:" << QString::number(ui->thumbnailspinbox->value()) << ",";
    out << "casepath:" << ba.toBase64() << ",";
    out << "reportpath:" << ba2.toBase64() << ",";
    out << "video:" << QString::number(ui->videospinbox->value()) << ",";
    out << "autosave:" << QString::number(ui->autosavespinbox->value()) << ",";
    out << "timezone:" << QByteArray::fromStdString(QString(ui->timezonecombobox->currentText()).toStdString());
    settingsfile.close();
    if(!newtz.isEmpty())
        emit UpdateTZ(newtz);
    this->hide();
}
void SettingsDialog::CancelChanges()
{
    this->hide();
}

void SettingsDialog::LoadSettings()
{
    settingsfile.open(QIODevice::ReadOnly | QIODevice::Text);
    QStringList tmplist = QString(settingsfile.readLine()).split(",", Qt::SkipEmptyParts);
    settingsfile.close();
    for(int i = 0; i < tmplist.count(); i++)
    {
        if(tmplist.at(i).split(":").at(0) == "thumb")
            ui->thumbnailspinbox->setValue(tmplist.at(i).split(":").at(1).toInt());
        else if(tmplist.at(i).split(":").at(0) == "casepath")
        {
            QByteArray ba;
            ba.append(tmplist.at(i).split(":").at(1).toUtf8());
            ui->casepathlineedit->setText(QByteArray::fromBase64(ba));
        }
        else if(tmplist.at(i).split(":").at(0) == "reportpath")
        {
            QByteArray ba;
            ba.append(tmplist.at(i).split(":").at(1).toUtf8());
            ui->reportpathlineedit->setText(QByteArray::fromBase64(ba));
        }
        else if(tmplist.at(i).split(":").at(0) == "video")
            ui->videospinbox->setValue(tmplist.at(i).split(":").at(1).toInt());
        else if(tmplist.at(i).split(":").at(0) == "autosave")
            ui->autosavespinbox->setValue(tmplist.at(i).split(":").at(1).toInt());
        else if(tmplist.at(i).split(":").at(0) == "timezone")
        {
            QByteArray tmparray = QByteArray::fromStdString(QString(tmplist.at(i).split(":").at(1)).toStdString());
            ui->timezonecombobox->setCurrentText(tmparray);
        }
    }
}

void SettingsDialog::GetCaseFolder()
{
    QString startdir = QDir::homePath();
    if(!ui->casepathlineedit->text().isEmpty())
        startdir = ui->casepathlineedit->text();
    QString casepathfolder = QFileDialog::getExistingDirectory(this, tr("Choose Case Path Directory"), startdir, QFileDialog::DontUseNativeDialog | QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks); 
    if(casepathfolder.compare("") != 0)
        ui->casepathlineedit->setText(casepathfolder);
}

void SettingsDialog::GetReportFolder()
{
    QString startdir = QDir::homePath();
    if(!ui->reportpathlineedit->text().isEmpty())
        startdir = ui->reportpathlineedit->text();
    QString reportpathfolder = QFileDialog::getExistingDirectory(this, tr("Choose Report Path Directory"), startdir, QFileDialog::DontUseNativeDialog | QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks); 
    if(reportpathfolder.compare("") != 0)
        ui->reportpathlineedit->setText(reportpathfolder);
}

void SettingsDialog::UpdateTimeZone(QString newtimezone)
{
    newtz = newtimezone;
    //emit UpdateTZ(newtimezone);
}
