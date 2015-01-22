#ifndef IMAGEVIEWER_H
#define IMAGEVIEWER_H

#include "wombatinclude.h"

class ImageViewer : public QWidget
{
    Q_OBJECT

public:
    ImageViewer(QWidget* parent = 0);
    ~ImageViewer();
};
#endif // IMAGEVIEWER_H
