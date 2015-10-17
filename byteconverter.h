#ifndef BYTECONVERTER_H
#define BYTECONVERTER_H

// Copyright 2015 Pasquale J. Rinaldi, Jr.
// Distrubted under the terms of the GNU General Public License version 2.1

#include "wombatinclude.h"
#include "globals.h"
#include "ui_byteconverter.h"

namespace Ui
{
    class ByteConverter;
}

class ByteConverter : public QDialog
{
    Q_OBJECT

public:
    ByteConverter(QWidget* parent = 0);
    ~ByteConverter();
    void SetText(QString txt);
public slots:
   void HideClicked();
signals:
    void HideByteConverterWindow(bool checkstate);
protected:
    void closeEvent(QCloseEvent* event);

private:
    Ui::ByteConverter* ui;
};

Q_DECLARE_METATYPE(ByteConverter*);

#endif // BYTECONVERTER_H
