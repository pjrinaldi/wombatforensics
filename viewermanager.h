#ifndef VIEWERMANAGER_H
#define VIEWERMANAGAER_H

// Copyright 2015 Pasquale J. Rinaldi, Jr.
// Distrubted under the terms of the GNU General Public License version 2

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
    void ShowBrowser();
    void AddViewer();
    void RemoveSelected();
    void UpdateList();
    void SelectionChanged();
signals:
    void HideManagerWindow();

protected:
    void closeEvent(QCloseEvent* event);
private:
    Ui::ViewerManager* ui;
    QString fileviewerpath;
    QModelIndex selectedindex;
};

Q_DECLARE_METATYPE(ViewerManager*);

#endif // VIDEOMANAGER_H
