#ifndef DIGDEEPERDIALOG_H
#define DIGDEEPERDIALOG_H

// Copyright 2013-2019 Pasquale J. Rinaldi, Jr.
// Distrubted under the terms of the GNU General Public License version 2

#include "wombatinclude.h"
#include "wombatvariable.h"

namespace Ui {
class DigDeeperDialog;
}

class DigDeeperDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit DigDeeperDialog(QWidget *parent = 0, qint64 curcheckcount = 0, qint64 curlistcount = 0);
    ~DigDeeperDialog();

signals:
    void StartDig(int digtype, QVector<int> digoptions);

private slots:
    void EnableProcess(bool checked);
    void DigDeeperFiles(void);
    void ShowHashListDialog(void);

private:
    Ui::DigDeeperDialog *ui;
    qint64 checkcount;
    qint64 listcount;
    int digtype = 0;
    QVector<int> digoptions;
    QWidget* parentwidget;
};

#endif // DIGDEEPERDIALOG_H
