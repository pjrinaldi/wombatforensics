#ifndef VEREVIDENCEDIALOG_H 
#define VEREVIDENCEDIALOG_H

// Copyright 2013-2019 Pasquale J. Rinaldi, Jr.
// Distrubted under the terms of the GNU General Public License version 2

#include "wombatinclude.h"
#include "globals.h"
#include "wombatfunctions.h"
#include "ui_verifyevidencedialog.h"
#include "imagefunctions.h"

namespace Ui
{
    class VerEvidenceDialog;
}

class VerEvidenceDialog : public QDialog
{
    Q_OBJECT

public:
    VerEvidenceDialog(QWidget* parent = 0);
    ~VerEvidenceDialog();
private slots:
    void VerifyEvidence();
    void Cancel();
signals:
    void VerEvid(QStringList verevidlist);
private:
    Ui::VerEvidenceDialog* ui;
protected:
};

Q_DECLARE_METATYPE(VerEvidenceDialog*)
#endif // VEREVIDENCEDIALOG_H
