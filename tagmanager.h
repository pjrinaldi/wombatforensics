#ifndef TAGMANAGER_H
#define TAGMANAGAER_H

// Copyright 2013-2019 Pasquale J. Rinaldi, Jr.
// Distrubted under the terms of the GNU General Public License version 2

#include "wombatinclude.h"
#include "globals.h"
#include "wombatfunctions.h"
#include "ui_tagmanager.h"

namespace Ui
{
    class TagManager;
}

class TagManager : public QDialog
{
    Q_OBJECT

public:
    TagManager(QWidget* parent = 0);
    ~TagManager();

private slots:
    void HideClicked();
    void ModifyTag();
    void RemoveTag();
    void AddTag();
    void UpdateList();
    void SelectionChanged();
signals:
    void HideManagerWindow();

private:
    Ui::TagManager* ui;
    QFile bookmarkfile;
    QStringList taglist;
    QStringList llist;
    QStringList tlist;
    QString linkstr = "";
    QString tagstr = "";
};

Q_DECLARE_METATYPE(TagManager*);

#endif // TAGMANAGER_H
