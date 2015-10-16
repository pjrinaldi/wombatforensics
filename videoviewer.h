#ifndef VIDEOVIEWER_H
#define VIDEOVIEWER_H

#include "wombatinclude.h"
#include "globals.h"
#include "ui_videoviewer.h"

namespace Ui
{
    class VideoViewer;
}

class Thread : public QThread
{
public:
    Thread(QtAV::AVPlayer* player) : QThread(0), mplayer(player)
    {
    };
protected:
    virtual void run()
    {
        exec();
    }
    QtAV::AVPlayer* mplayer;
};

class VideoViewer : public QDialog
{
    Q_OBJECT

public:
    VideoViewer(QWidget* parent = 0);
    ~VideoViewer();

public slots:
    void ShowVideo(QString tmpfilepath, const QModelIndex &index);
    void Seek(int);
    void PlayPause();
    void UpdateSlider();
    void GetVideo(QString tmpfilepath, unsigned long long objid);
    
private:
    Ui::VideoViewer* ui;
    QtAV::VideoOutput* vout;
    QtAV::AVPlayer* vplayer;
    TskObject tskobj;
    TskObject* tskptr;
protected:
    void mousePressEvent(QMouseEvent* event);
};

Q_DECLARE_METATYPE(VideoViewer*)

#endif // VIDEOVIEWER_H
