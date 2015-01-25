#ifndef IMAGEVIEWER_H
#define IMAGEVIEWER_H

#include "wombatinclude.h"
#include "globals.h"
#include "ui_imageviewer.h"

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
    QListWidget* lw;
    QSpinBox* sb;
private slots:
    void HideClicked();

signals:
    void HideImageWindow(bool checkstate);
private:
    Ui::ImageViewer* ui;
protected:
    void closeEvent(QCloseEvent* event);
};

Q_DECLARE_METATYPE(ImageViewer*)
#endif // IMAGEVIEWER_H
