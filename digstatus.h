#ifndef DIGSTATUS_H
#define DIGSTATUS_H

// Copyright 2015 Pasquale J. Rinaldi, Jr.
// Distrubted under the terms of the GNU General Public License version 2

#include "wombatinclude.h"
#include "globals.h"
#include "ui_digstatus.h"

namespace Ui
{
    class DigStatus;
}

class DigStatus : public QDialog
{
    Q_OBJECT

public:
    DigStatus(QWidget* parent = 0);
    ~DigStatus();
    //void SetText(QString txt);
    void UpdateDigState(int digstateid, int digstatecount);
    void SetIntitialDigState(int digstateid, int digtotal);
public slots:
   //void HideClicked();
signals:
    //void HideDigStatusWindow(bool checkstate);
    void CancelDigStatus(int digstateid);
protected:
    void closeEvent(QCloseEvent* event);

private:
    Ui::DigStatus* ui;
};

Q_DECLARE_METATYPE(DigStatus*);

#endif // BYTECONVERTER_H
