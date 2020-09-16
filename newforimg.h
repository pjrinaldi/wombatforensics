#ifndef NEWFORIMG_H
#define NEWFORIMG_H
// Copyright 2013-2020 Pasquale J. Rinaldi, Jr.
// Distrubted under the terms of the GNU General Public License version 2

#include "wombatinclude.h"
#include "globals.h"
#include "wombatfunctions.h"
#include "ui_newforimg.h"
#include <libudev.h>
//#include <parted.h>

namespace Ui
{
    class ForImgDialog;
}

class ForImgDialog : public QDialog
{
    Q_OBJECT

public:
    ForImgDialog(QWidget* parent = 0);
    ~ForImgDialog();
    //void LoadRegistryFile(QString regid, QString regname);

private slots:
    void HideClicked();
    void CreateImage();
    void GetFolder();
    void FinishImaging();
    //void Assign();
    //void UpdateList();
    //void UpdateAssign();
    //void KeySelected(void);
    //void ValueSelected(void);
    //void TagMenu(const QPoint &point);
    //void SetTag(void);
    //void CreateNewTag(void);
    //void RemoveTag(void);
    //void DoubleClick(QTableWidgetItem* curitem);
    //void ImgHexMenu(const QPoint &point);
signals:
    //void TagCarved(QString ctitle, QString ctag);
protected:
    void closeEvent(QCloseEvent* event);

private:
    Ui::ForImgDialog* ui;
    QStringList devicelist;
    QFutureWatcher<void> imgwatcher;
    //void PopulateChildKeys(libregf_key_t* curkey, QTreeWidgetItem* curitem, libregf_error_t* curerr);
    //QString DecryptRot13(QString encstr);
    //QChar Rot13Char(QChar curchar);
    //QString regfilepath;
    //QMenu* tagmenu;
    //QFile bookmarkfile;
    //QStringList taglist;
};

Q_DECLARE_METATYPE(ForImgDialog*);

#endif // NEWFORIMG_H
