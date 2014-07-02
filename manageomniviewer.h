#ifndef MANAGEOMNIVIEWER_H
#define MANAGEOMNIVIEWER_H

#include <QDialog>

namespace Ui {
class ManageOmniViewer;
}

class ManageOmniViewer : public QDialog
{
    Q_OBJECT
    
public:
    explicit ManageOmniViewer(QWidget *parent = 0);
    ~ManageOmniViewer();
    
private:
    Ui::ManageOmniViewer *ui;
};

#endif // MANAGEOMNIVIEWER_H
