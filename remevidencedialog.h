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
    //void SelectEvidence();
    void RemoveEvidence();
    void Cancel();
    //void StartProcess();
    //void UpdateButtons();
    //void HideClicked();
    //void ShowAbout();
private:
    Ui::RemEvidenceDialog* ui;
protected:
    //void dragEnterEvent(QDragEnterEvent* e);
    //void dropEvent(QDropEvent* e);
    //void mousePressEvent(QMouseEvent* event);
};

Q_DECLARE_METATYPE(RemEvidenceDialog*)
#endif // ADDEVIDENCEDIALOG_H
