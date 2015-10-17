#ifndef DIGDEEPERDIALOG_H
#define DIGDEEPERDIALOG_H

// Copyright 2015 Pasquale J. Rinaldi, Jr.
// Distrubted under the terms of the GNU General Public License version 2.1

#include "wombatinclude.h"
#include "wombatvariable.h"

namespace Ui {
class DigDeeperDialog;
}

class DigDeeperDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit DigDeeperDialog(QWidget *parent = 0, unsigned long long curcheckcount = 0, unsigned long long curlistcount = 0);
    ~DigDeeperDialog();

signals:
    void FileDig(FileDeepData* deeperdata);

private slots:
    void EnableProcess(bool checked);
    void DigDeeperFiles(void);

private:
    Ui::DigDeeperDialog *ui;
    unsigned long long checkcount;
    unsigned long long listcount;
    QWidget* parentwidget;
};

#endif // DIGDEEPERDIALOG_H
