#ifndef MBOXVIEWER_H
#define MBOXVIEWER_H

// Copyright 2013-2022 Pasquale J. Rinaldi, Jr.
// Distrubted under the terms of the GNU General Public License version 2

#include "wombatinclude.h"
#include "globals.h"
#include "wombatfunctions.h"
#include "ui_mboxviewer.h"
#include "tagging.h"
#include "reporting.h"

namespace Ui
{
    class MBoxDialog;
}

class MBoxDialog : public QDialog
{
    Q_OBJECT

public:
    MBoxDialog(QWidget* parent = 0);
    ~MBoxDialog();
    void LoadMBoxFile(QString mboxid, QString mboxname);
    
private slots:
    void HideClicked();
    void EmailSelected();
    void TagMenu(const QPoint &point);
    void SetTag(void);
    //void CreateNewTag(void);
    void RemoveTag(void);
    //void DoubleClick(QTableWidgetItem* curitem);
    
protected:
    void closeEvent(QCloseEvent* event);

private:
    Ui::MBoxDialog* ui;
    QMenu* tagmenu;
    QString htmlentry;
    QString htmlvalue;
    QStringList mboxtaglist;
};

Q_DECLARE_METATYPE(MBoxDialog*);
/*
namespace Ui
{
    class RegistryDialog;
}

class RegistryDialog : public QDialog
{
    Q_OBJECT

public:
    RegistryDialog(QWidget* parent = 0);
    ~RegistryDialog();
    void LoadRegistryFile(QString regid, QString regname);

private slots:
    void HideClicked();
    void KeySelected(void);
    void ValueSelected(void);
    void TagMenu(const QPoint &point);
    void SetTag(void);
    //void CreateNewTag(void);
    void RemoveTag(void);
    //void DoubleClick(QTableWidgetItem* curitem);
    //void ImgHexMenu(const QPoint &point);
signals:
    //void TagCarved(QString ctitle, QString ctag);
protected:
    void closeEvent(QCloseEvent* event);

private:
    Ui::RegistryDialog* ui;
    void PopulateChildKeys(libregf_key_t* curkey, QTreeWidgetItem* curitem, libregf_error_t* curerr);
    QString DecryptRot13(QString encstr);
    QChar Rot13Char(QChar curchar);
    QString regfilepath;
    QMenu* tagmenu;
    QStringList registrytaglist;
    QString htmlentry;
    QString htmlvalue;
    //QFile bookmarkfile;
    //QStringList taglist;
/*};

Q_DECLARE_METATYPE(RegistryDialog*);
*/
#endif // MBOXVIEWER_H
