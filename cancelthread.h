#ifndef CANCELTHREAD_H 
#define CANCELTHREAD_H

// Copyright 2015 Pasquale J. Rinaldi, Jr.
// Distrubted under the terms of the GNU General Public License version 2

#include "wombatinclude.h"
#include "globals.h"
#include "wombatfunctions.h"
#include "ui_cancelthread.h"

namespace Ui
{
    class CancelThread;
}

class CancelThread : public QDialog
{
    Q_OBJECT

public:
    CancelThread(QWidget* parent = 0);
    ~CancelThread();
private slots:
    void HideClicked();
    void ShowCancelThread();
    void StopThread();

signals:
    void CancelCurrentThread();
private:
    Ui::CancelThread* ui;
protected:
};

Q_DECLARE_METATYPE(CancelThread*)
#endif // ABOUTBOX_H
