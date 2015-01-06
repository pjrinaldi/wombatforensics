#ifndef FILEVIEWER_H
#define FILEVIEWER_H

#include "wombatinclude.h"
#include "filehexviewer.h"
#include "ui_fileviewer.h"

namespace Ui {
    class FileViewer;
}
    
class FileViewer : public QMainWindow
{
    Q_OBJECT
    
public:
    FileViewer(QWidget* parent = 0, TskObject* tskobject = NULL);
    ~FileViewer();
    FileHexViewer* filehexview;
    FileReader* filereader;
    vector<uchar*> filedata;

public slots:

private slots:
    void SetScrollBarRange(off_t low, off_t high);
    void setScrollBarValue(off_t pos);
    void SetOffsetLabel(off_t pos);
    void UpdateSelectValue(const QString &txt);
    void SetStepValues(int singlestep, int pagestep);
    void HideClicked();

signals:
    void HideFileViewer(bool checkstate);

protected:
    void closeEvent(QCloseEvent* event);
private:
    Ui::FileViewer* ui;

    QLabel* selectedoffset;
    QLabel* selectedhex;
    QScrollBar* filehexvsb;
    TskObject* tskptr;
};

Q_DECLARE_METATYPE(FileViewer*);

#endif // FILEVIWER_H
