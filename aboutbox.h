#ifndef ABOUTBOX_H 
#define ABOUTBOX_H

#include "wombatinclude.h"
#include "globals.h"
#include "wombatfunctions.h"
#include "ui_aboutbox.h"

namespace Ui
{
    class AboutBox;
}

class AboutBox : public QDialog
{
    Q_OBJECT

public:
    AboutBox(QWidget* parent = 0);
    ~AboutBox();
private slots:
    void HideClicked();
    void ShowAbout();
private:
    Ui::AboutBox* ui;
protected:
    void mousePressEvent(QMouseEvent* event);
};

Q_DECLARE_METATYPE(AboutBox*)
#endif // ABOUTBOX_H
