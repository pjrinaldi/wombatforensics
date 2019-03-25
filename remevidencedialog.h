#ifndef REMEVIDENCEDIALOG_H 
#define REMEVIDENCEDIALOG_H

// Copyright 2015 Pasquale J. Rinaldi, Jr.
// Distrubted under the terms of the GNU General Public License version 2

#include "wombatinclude.h"
#include "globals.h"
#include "wombatfunctions.h"
#include "ui_remevidencedialog.h"

namespace Ui
{
    class RemEvidenceDialog;
}

class RemEvidenceDialog : public QDialog
{
    Q_OBJECT

public:
    RemEvidenceDialog(QWidget* parent = 0);
    ~RemEvidenceDialog();
private slots:
    void RemoveEvidence();
    void Cancel();
signals:
    void RemEvid(QStringList remevidlist);
private:
    Ui::RemEvidenceDialog* ui;
protected:
};

Q_DECLARE_METATYPE(RemEvidenceDialog*)
#endif // ADDEVIDENCEDIALOG_H
