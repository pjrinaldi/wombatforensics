#ifndef FILEVIEWER_H
#define FILEVIEWER_H

// Copyright 2015 Pasquale J. Rinaldi, Jr.
// Distrubted under the terms of the GNU General Public License version 2

#include "wombatinclude.h"
//#include "filehexviewer.h"
#include "ui_fileviewer.h"
//#include "filterviews.h"

/*
class FileSlider : public QSlider
{
    Q_OBJECT

public:
    FileSlider(QWidget* parent = 0) : QSlider(parent) {};
signals:
    void ShowJumpFilter(void);

protected:
    void mousePressEvent(QMouseEvent* event)
    {
        if(event->button() == Qt::RightButton)
        {
            emit ShowJumpFilter();
            event->accept();
        }
        else
        {
            event->accept();
            QSlider::mousePressEvent(event);
        }
    };
};
*/

namespace Ui {
    class FileViewer;
}
    
class FileViewer : public QMainWindow
{
    Q_OBJECT
    
public:
    //FileViewer(QWidget* parent = 0, TskObject* tskobject = NULL);
    FileViewer(QWidget* parent = 0);
    ~FileViewer();
    UpdateHexView(void);
    //FileHexViewer* filehexview;
    //FileReader* filereader;
    //vector<uchar*> filedata;
    //FileJumpFilter* jumpfilterview;

public slots:

private slots:

    /*
    void SetScrollBarRange(off_t low, off_t high);
    */
    void SetOffsetLabel(off_t pos);
    /*
    void ResetSlider(void);
    void ShowRockerToolTip(int moved);
    void SkipDown(void);
    void SkipUp(void);
    void PageUp(void);
    void PageDown(void);
    */
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
    //FileSlider* hexrocker;
    //QPushButton* lineup;
    //QPushButton* linedown;
    //QPushButton* pageup;
    //QPushButton* pagedown;
    //TskObject* tskptr;
};

Q_DECLARE_METATYPE(FileViewer*);

#endif // FILEVIWER_H
