#ifndef SETTINGSDIALOG_H 
#define SETTINGSDIALOG_H

// Copyright 2013-2019 Pasquale J. Rinaldi, Jr.
// Distrubted under the terms of the GNU General Public License version 2

#include "wombatinclude.h"
#include "globals.h"
#include "wombatfunctions.h"
#include "ui_settingsdialog.h"
#include "carvingfiletypesdialog.h"

namespace Ui
{
    class SettingsDialog;
}

class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    SettingsDialog(QWidget* parent = 0);
    ~SettingsDialog();
    void LoadSettings();
private slots:
    void CancelChanges();
    void SaveChanges();
    void GetCaseFolder();
    void GetReportFolder();
    void UpdateTimeZone(QString newtimezone);

signals:
    void HideSettingsWindow();
    void UpdateTZ(QString newtz);
private:
    Ui::SettingsDialog* ui;
    QList<QByteArray> zoneids;
    QString newtz = "";
    CarvingFileTypesDialog* filetypedialog;
    
protected:
};

Q_DECLARE_METATYPE(SettingsDialog*)
#endif // SETTINGSDIALOG_H
