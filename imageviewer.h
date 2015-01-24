#ifndef IMAGEVIEWER_H
#define IMAGEVIEWER_H

#include "wombatinclude.h"
#include "ui_imageviewer.h"

typedef QFutureWatcher<QImage> ImageWatcher;

namespace Ui
{
    class ImageViewer;
}

class ImageViewer : public QDialog
{
    Q_OBJECT

public:
    ImageViewer(QWidget* parent = 0);
    ~ImageViewer();
    void SetFutureWatcher(ImageWatcher* w);
    QListWidget* lw;
private slots:
    void ReadyAt(int which);

private:
    Ui::ImageViewer* ui;
protected:
    ImageWatcher* watcher;
    QProgressBar* progressbar;
};

#endif // IMAGEVIEWER_H
