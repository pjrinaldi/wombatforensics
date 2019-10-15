#ifndef VIDEOVIEWER_H
#define VIDEOVIEWER_H

// Copyright 2013-2019 Pasquale J. Rinaldi, Jr.
// Distrubted under the terms of the GNU General Public License version 2

#include "wombatinclude.h"
#include "globals.h"
#include "ui_videoviewer.h"
//#include "wombatfunctions.h"

namespace Ui
{
    class VideoViewer;
}

class Thread : public QThread
{
public:
protected:
    virtual void run()
    {
        exec();
    }
};

class VideoViewer : public QDialog
{
    Q_OBJECT

public:
    VideoViewer(QWidget* parent = 0);
    ~VideoViewer();

public slots:
    void ShowVideo(const QModelIndex &index);
    void Seek(int);
    void PlayPause();
    void UpdateSlider(qint64);
    void SetDuration(qint64);
    
private:
    Ui::VideoViewer* ui;
    QMediaPlayer* vplayer;
    QVideoWidget* videowidget;
    qint64 curobjaddr;
protected:
    void mousePressEvent(QMouseEvent* event);
};

Q_DECLARE_METATYPE(VideoViewer*)

#endif // VIDEOVIEWER_H
