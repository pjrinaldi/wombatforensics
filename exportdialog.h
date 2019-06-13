#ifndef EXPORTDIALOG_H
#define EXPORTDIALOG_H

// Copyright 2013-2019 Pasquale J. Rinaldi, Jr.
// Distrubted under the terms of the GNU General Public License version 2

#include "wombatinclude.h"
#include "wombatvariable.h"

namespace Ui {
class ExportDialog;
}

class ExportDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit ExportDialog(QWidget *parent = 0, qint64 curcheckcount = 0, qint64 curlistcount = 0);
    ~ExportDialog();

signals:
    void StartExport(int exporttype, bool originalpath, QString exportpath);

private slots:
    void SelectDirectory(void);
    void ExportFiles(void);

private:
    Ui::ExportDialog *ui;
    qint64 checkcount;
    qint64 listcount;
    int exporttype = 0;
    bool originalpath = true;
    QString exportpath = "";
    QWidget* parentwidget;
};

#endif // EXPORTDIALOG_H
