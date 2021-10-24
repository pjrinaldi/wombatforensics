#ifndef VEREVIDENCEDIALOG_H 
#define VEREVIDENCEDIALOG_H

// Copyright 2013-2019 Pasquale J. Rinaldi, Jr.
// Distrubted under the terms of the GNU General Public License version 2

#include "wombatinclude.h"
#include "globals.h"
#include "wombatfunctions.h"
#include "ui_verifyevidencedialog.h"
//#include "imagefunctions.h"

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
    void Show();
private slots:
    void VerifyEvidence();
    void Cancel();
    void UpdateProgress(QString pname, qint64 bytesread);
signals:
    void VerEvid(QStringList verevidlist);
    void HideVerifyWindow(bool checkstate);
private:
    Ui::VerEvidenceDialog* ui;
protected:
};

Q_DECLARE_METATYPE(VerEvidenceDialog*)
#endif // VEREVIDENCEDIALOG_H
