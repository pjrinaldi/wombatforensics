#ifndef FILEVIEWER_H
#define FILEVIEWER_H

// Copyright 2013-2019 Pasquale J. Rinaldi, Jr.
// Distrubted under the terms of the GNU General Public License version 2

#include "wombatinclude.h"
#include "ui_fileviewer.h"
#include "searchdialog.h"

namespace Ui {
    class FileViewer;
}
    
class FileViewer : public QMainWindow
{
    Q_OBJECT
    
public:
    FileViewer(QWidget* parent = 0);
    ~FileViewer();
    void UpdateHexView();
    void BypassColor(bool bypass);

public slots:

private slots:

    void SetOffsetLabel(qint64 pos);
    void UpdateSelectValue(void);
    void HideClicked();
    void JumpHex();
    void ShowSearch();

signals:
    void HideFileViewer(bool checkstate);

protected:
    void closeEvent(QCloseEvent* event);
private:
    Ui::FileViewer* ui;

    QLabel* selectedoffset;
    QLabel* selectedhex;
    QFile hexfile;
    QLineEdit* jumpto;
    QLabel* jumplabel;
    QPushButton* jumpbutton;
    QAction* hexbutton;
    SearchDialog* searchdialog;
};

Q_DECLARE_METATYPE(FileViewer*);

#endif // FILEVIWER_H
