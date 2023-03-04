#ifndef MBOXVIEWER_H
#define MBOXVIEWER_H

// Copyright 2013-2022 Pasquale J. Rinaldi, Jr.
// Distrubted under the terms of the GNU General Public License version 2

#include "wombatinclude.h"
#include "globals.h"
#include "wombatfunctions.h"
#include "ui_mboxviewer.h"
#include "tagging.h"
#include "reporting.h"

namespace Ui
{
    class MBoxDialog;
}

class MBoxDialog : public QDialog
{
    Q_OBJECT

public:
    MBoxDialog(QWidget* parent = 0);
    ~MBoxDialog();
    void LoadMBoxFile(QString mboxid, QString mboxname);
    
private slots:
    void HideClicked();
    void EmailSelected();
    void TagMenu(const QPoint &point);
    void SetTag(void);
    void RemoveTag(void);
    
protected:
    void closeEvent(QCloseEvent* event);

private:
    Ui::MBoxDialog* ui;
    QMenu* tagmenu;
    QString htmlentry;
    QString htmlvalue;
    QString mboxparentname;
    QStringList mboxtaglist;
};

Q_DECLARE_METATYPE(MBoxDialog*);

#endif // MBOXVIEWER_H
