#ifndef ADDEVIDENCEDIALOG_H 
#define ADDEVIDENCEDIALOG_H

// Copyright 2013-2020 Pasquale J. Rinaldi, Jr.
// Distrubted under the terms of the GNU General Public License version 2

#include "wombatinclude.h"
#include "globals.h"
#include "wombatfunctions.h"
#include "ui_addevidencedialog.h"

namespace Ui
{
    class AddEvidenceDialog;
}

class AddEvidenceDialog : public QDialog
{
    Q_OBJECT

public:
    AddEvidenceDialog(QWidget* parent = 0);
    ~AddEvidenceDialog();
private slots:
    void SelectEvidence();
    void RemoveEvidence();
    void Cancel();
    void StartProcess();
    void UpdateButtons();
private:
    Ui::AddEvidenceDialog* ui;
protected:
    void dragEnterEvent(QDragEnterEvent* e);
    void dropEvent(QDropEvent* e);
};

Q_DECLARE_METATYPE(AddEvidenceDialog*)
#endif // ADDEVIDENCEDIALOG_H
