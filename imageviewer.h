#ifndef IMAGEVIEWER_H
#define IMAGEVIEWER_H

#include "wombatinclude.h"

typedef QFutureWatcher<QImage> ImageWatcher;

class ImageViewer : public QListWidget
{
    Q_OBJECT

public:
    ImageViewer(QWidget* parent = 0);
    ~ImageViewer();
    void SetFutureWatcher(ImageWatcher* w);
private slots:
    void ReadyAt(int which);
protected:
    ImageWatcher* watcher;
    QProgressBar* progressbar;
};

#endif // IMAGEVIEWER_H
