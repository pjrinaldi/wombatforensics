#ifndef PROGRESSWINDOW_H
#define PROGRESSWINDOW_H

#include "main/interfaces.h"
#include <QDialog>
#include <QtPlugin>
#include <QObject>

namespace Ui {
class ProgressWindow;
}

class ProgressWindow : public QDialog, public ProgressWindowInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "wombat.ProgressWindowInterface" FILE "progresswindow.json")
    Q_INTERFACES(ProgressWindowInterface)
    
public:
    explicit ProgressWindow(QWidget *parent = 0);
    ~ProgressWindow();
    
private:
    Ui::ProgressWindow *ui;
};

#endif // PROGRESSWINDOW_H
