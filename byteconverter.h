#ifndef BYTECONVERTER_H
#define BYTECONVERTER_H

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
