#ifndef REGISTRYVIEWER_H
#define REGISTRYVIEWER_H

// Copyright 2013-2020 Pasquale J. Rinaldi, Jr.
// Distrubted under the terms of the GNU General Public License version 2

#include "wombatinclude.h"
#include "globals.h"
#include "wombatfunctions.h"
#include "ui_registryviewer.h"
#include "libregf.h"
#include "tagging.h"
#include "reporting.h"

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
    QString bodycss = "font-size: 12px; font-family: verdana, sans; font-color: #3a291a;";
    QString infotitlecss = "text-transform: uppercase; font-size: 18px; font-family: verdana, sans; font-color: #9b7d4b; font-color: #3a291a;";
    QString tdfitemcss = "padding-left: 10px; padding-top: 2.5px; padding-right: 10px; padding-bottom: 2.5px; width:150px; vertical-align: top;";
    QString tablecss = "border-collapse: collapse; border-color: #644813;";
    QString trcss = "text-transform: uppercase; font-family: verdana, sans; font-size: 12px; text-align: left;";
    QString troddcss = "text-transform: uppercase; font-family: verdana, sans; font-size: 12px; text-align: left; background-color: #d6ceb5;";
    QString trevencss = "text-transform: uppercase; font-family: verdana, sans; font-size: 12px; text-align: left; background-color: #d5e8e2;";
    QString troddvtopcss = "text-transform: uppercase; font-family: verdana, sans; font-size: 12px; text-align: left; background-color: #d6ceb5; vertical-align: top;";
    QString thcss = "background-color: #ad9f68; text-transform: uppercase; font-size: 12px; font-weight: bold; font-family: verdana, sans; padding: 5px 10px 5px 10px; border-bottom: 2px solid #3a291a;";
    QString tdcss = "padding-left: 10px; padding-top: 2.5px; padding-right: 10px; padding-bottom: 2.5px;";
    QString tdpvaluecss = "padding-left: 10px; padding-top: 2.5px; padding-right: 10px; padding-bottom: 2.5px; width:72px";
    QString tdpropcss = "padding-left: 10px; padding-top: 2.5px; padding-right: 10px; padding-bottom: 2.5px; width:188px";
};

Q_DECLARE_METATYPE(RegistryDialog*);

#endif // REGISTRYVIEWER_H
