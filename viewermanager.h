#ifndef VIEWERMANAGER_H
#define VIEWERMANAGAER_H

#include "wombatinclude.h"
#include "globals.h"
#include "ui_viewermanager.h"

namespace Ui
{
    class ViewerManager;
}

class ViewerManager : public QDialog
{
    Q_OBJECT

public:
    ViewerManager(QWidget* parent = 0);
    ~ViewerManager();

private slots:
    void HideClicked();
signals:
    void HideManagerWindow(bool checkstate);

protected:
    void closeEvent(QCloseEvent* event);
private:
    Ui::ViewerManager* ui;
    
};

Q_DECLARE_METATYPE(ViewerManager*);

#endif // VIDEOMANAGER_H
