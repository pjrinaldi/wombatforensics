#ifndef CARVINGFILETYPESDIALOG_H
#define CARVINGFILETYPESDIALOG_H

// Copyright 2013-2020 Pasquale J. Rinaldi, Jr.
// Distrubted under the terms of the GNU General Public License version 2

#include "wombatinclude.h"
#include "globals.h"
#include "ui_carvingfiletypesdialog.h"
//#include "ui_textviewer.h"
#include "wombatfunctions.h"

namespace Ui
{
    class CarvingFileTypesDialog;
}

class CarvingFileTypesDialog : public QDialog
{
    Q_OBJECT
        
public:
    CarvingFileTypesDialog(QWidget* parent = 0);
    ~CarvingFileTypesDialog();
    //void ShowText(const QModelIndex &index);
    
private slots:
    void ShowText();
    void HideClicked();
    void AddRow();
    void SaveClose();
    void Save();
    //void UpdateEncoding(int unused);
signals:
    void HideCarvingFileTypesDialogWindow(bool checkstate);

protected:
    void closeEvent(QCloseEvent* event);

private:
    Ui::CarvingFileTypesDialog* ui;
    //QByteArray txtdata;
    //QByteArray tmpdata;
    //QString decodedstring;
    //qint64 curobjaddr;
    //QList<QTextCodec*> codecs;
    //void FindCodecs();
    //QModelIndex curindex;
};

Q_DECLARE_METATYPE(CarvingFileTypesDialog*);

#endif // CARVINGFILETYPESDIALOG_H
