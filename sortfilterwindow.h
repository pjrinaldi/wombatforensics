#ifndef SORTFILTERWINDOW_H
#define SORTFILTERWINDOW_H

#include "wombatinclude.h"
#include "globals.h"
#include "ui_sortfilterwindow.h"

namespace Ui {
    class sortfilter;
}

class SortFilterWindow : public QDialog
{
    Q_OBJECT

public:
    SortFilterWindow(QWidget* parent = 0);
    ~SortFilterWindow();
    void ShowQuery(void);

protected:
    void closeEvent(QCloseEvent* evt);
private:
    Ui::sortfilter* ui;
    QSqlQueryModel* model;
};

Q_DECLARE_METATYPE(SortFilterWindow*);

#endif // SORTFILTERWINDOW_H
