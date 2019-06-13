#ifndef DIGSTATUS_H
#define DIGSTATUS_H

// Copyright 2013-2019 Pasquale J. Rinaldi, Jr.
// Distrubted under the terms of the GNU General Public License version 2

#include "wombatinclude.h"
#include "globals.h"
#include "ui_digstatus.h"

namespace Ui
{
    class DigStatus;
}

class DigStatus : public QDialog
{
    Q_OBJECT

public:
    DigStatus(QWidget* parent = 0);
    ~DigStatus();
    void UpdateDigState(int digstateid, int digstatecount);
    void SetInitialDigState(int digstateid, int digtotal);
public slots:
    void CancelHash(void);
    void CancelImgThumb(void);
signals:
    void CancelImgThumbThread(void);
    void CancelHashThread(void);
protected:
    void closeEvent(QCloseEvent* event);

private:
    Ui::DigStatus* ui;
    int hashcount, hashtotal;
    int imgthumbcount, imgthumbtotal;
    float hashpercent, imgthumbpercent;
    int digstate;
    QString hashstring;
};

Q_DECLARE_METATYPE(DigStatus*);

#endif // BYTECONVERTER_H
