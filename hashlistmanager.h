#ifndef HASHLISTMANAGER_H
#define HASHLISTMANAGER_H

// Copyright 2013-2019 Pasquale J. Rinaldi, Jr.
// Distrubted under the terms of the GNU General Public License version 2

#include "wombatinclude.h"
#include "globals.h"
#include "ui_hashlistmanager.h"

namespace Ui
{
    class HashListManager;
}

class HashListManager : public QDialog
{
    Q_OBJECT

public:
    HashListManager(QWidget* parent = 0);
    ~HashListManager();

private slots:
    /*
    void HideClicked();
    void ShowBrowser();
    void AddViewer();
    void RemoveSelected();
    void UpdateList();
    void SelectionChanged();
    */
signals:
    //void HideManagerWindow();

protected:
    void closeEvent(QCloseEvent* event);
private:
    Ui::HashListManager* ui;
    //QString fileviewerpath;
    //QModelIndex selectedindex;
};

Q_DECLARE_METATYPE(HashListManager*);

#endif // HASHLISTMANAGER_H
