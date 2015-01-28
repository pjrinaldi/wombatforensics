#ifndef VIDEOVIEWER_H
#define VIDEOVIEWER_H

#include "wombatinclude.h"
#include "globals.h"
#include "ui_videoviewer.h"

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
    Node* NodeFromIndex(const QModelIndex &index) const
    {
        if(index.isValid())
            return static_cast<Node*>(index.internalPointer());
        else
            return rootnode;
    };

public slots:
    void ShowVideo(const QModelIndex &index);
    
private:
    Ui::VideoViewer* ui;
    QtAV::VideoOutput* vout;
    QtAV::AVPlayer* vplayer;
protected:
    void mousePressEvent(QMouseEvent* event);
};

Q_DECLARE_METATYPE(VideoViewer*)

#endif // VIDEOVIEWER_H
