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
    ~FileCarvingDialog();
    void PopulatePartitions(QStringList plist);
    void PopulateFileTypes();

signals:
    void StartCarve(QStringList plist, QStringList flist);

private slots:
    void EnableProcess();

private:
    Ui::FileCarvingDialog *ui;
};

#endif // FILECARVINGDIALOG_H
