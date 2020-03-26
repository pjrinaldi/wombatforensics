#ifndef FILECARVINGDIALOG_H
#define FILECARVINGDIALOG_H

// Copyright 2013-2020 Pasquale J. Rinaldi, Jr.
// Distrubted under the terms of the GNU General Public License version 2

#include "wombatinclude.h"
#include "wombatvariable.h"

namespace Ui {
class FileCarvingDialog;
}

class FileCarvingDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit FileCarvingDialog(QWidget* parent = 0);
    //explicit FileCarvingDialog(QWidget *parent = 0, qint64 curcheckcount = 0, qint64 curlistcount = 0);
    ~FileCarvingDialog();
    void PopulatePartitions(QStringList plist);

signals:
    //void StartDig(int digtype, QVector<int> digoptions);

private slots:
    void EnableProcess(bool checked);
    //void DigDeeperFiles(void);

private:
    Ui::FileCarvingDialog *ui;
    //qint64 checkcount;
    //qint64 listcount;
    //int digtype = 0;
    //QVector<int> digoptions;
    QWidget* parentwidget;
};

#endif // FILECARVINGDIALOG_H
