#ifndef FILEVIEWER_H
#define FILEVIEWER_H

// Copyright 2015 Pasquale J. Rinaldi, Jr.
// Distrubted under the terms of the GNU General Public License version 2

#include "wombatinclude.h"
#include "ui_fileviewer.h"

namespace Ui {
    class FileViewer;
}
    
class FileViewer : public QMainWindow
{
    Q_OBJECT
    
public:
    FileViewer(QWidget* parent = 0);
    ~FileViewer();
    void UpdateHexView(void);

public slots:

private slots:

    void SetOffsetLabel(off_t pos);
    void UpdateSelectValue(const QString &txt);
    void HideClicked();

signals:
    void HideFileViewer(bool checkstate);

protected:
    void closeEvent(QCloseEvent* event);
private:
    Ui::FileViewer* ui;

    QLabel* selectedoffset;
    QLabel* selectedhex;
    QFile hexfile;
};

Q_DECLARE_METATYPE(FileViewer*);

#endif // FILEVIWER_H
