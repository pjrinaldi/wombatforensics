#ifndef CARVEDIALOG_H
#define CARVEDIALOG_H

// Copyright 2013-2019 Pasquale J. Rinaldi, Jr.
// Distrubted under the terms of the GNU General Public License version 2

#include "wombatinclude.h"
#include "globals.h"
#include "wombatfunctions.h"
#include "ui_carvedialog.h"

namespace Ui
{
    class CarveDialog;
}

class CarveDialog : public QDialog
{
    Q_OBJECT

public:
    CarveDialog(QWidget* parent = 0);
    ~CarveDialog();

private slots:
    void HideClicked();
    void Assign();
    void UpdateList();
    void UpdateAssign();
signals:
    void TagCarved(QString ctitle, QString ctag);
protected:
    void closeEvent(QCloseEvent* event);

private:
    Ui::CarveDialog* ui;
    QFile bookmarkfile;
    QStringList taglist;
};

Q_DECLARE_METATYPE(CarveDialog*);

#endif // CARVEDIALOG_H
