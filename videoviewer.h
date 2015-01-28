#ifndef VIDEOVIEWER_H
#define VIDEOVIEWER_H

#include "wombatinclude.h"
#include "ui_videviewer.h"

namespace Ui
{
    class VideoViewer;
}

class VideoViewer : public QDialog
{
    Q_OBJECT

public:
    VideoViewer(QWidget* parent = 0);
    ~VideoViewer();

private slots:
    void ShowVideo();
    
private:
    Ui::VideoViewer* ui;
    QtAV::VideoOutput* vout;
    QtAV::AVPlayer* vplayer;
protected:
    void mousePressEvent(QMouseEvent* event);
};

Q_DECLARE_METATYPE(VideoViewer*)

#endif // VIDEOVIEWER_H
