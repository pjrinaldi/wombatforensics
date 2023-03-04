#ifndef IMAGEVIEWER_H
#define IMAGEVIEWER_H

// Copyright 2013-2019 Pasquale J. Rinaldi, Jr.
// Distrubted under the terms of the GNU General Public License version 2

#include "wombatinclude.h"
#include "globals.h"
#include "wombatfunctions.h"
#include "ui_imageviewer.h"
#include "ui_imagewindow.h"
#include "videoviewer.h"

namespace Ui
{
    class ImageViewer;
    class ImageWindow;
}

class ImageWindow : public QDialog
{
    Q_OBJECT

public:
    ImageWindow(QWidget* parent = 0);
    ~ImageWindow();
    void GetImage(QString objid);
private slots:
    void ShowImage();
    void SwitchScale(bool swsc);

private:
    Ui::ImageWindow* ui;
protected:
    void mousePressEvent(QMouseEvent* event);
};

class ImageViewer : public QDialog
{
    Q_OBJECT

public:
    ImageViewer(QWidget* parent = 0);
    ~ImageViewer();
    void LoadThumbnails(void);
public slots:
    void OpenImageWindow(QListWidgetItem* item);
private slots:
    void HighlightTreeViewItem(QListWidgetItem* item);
    void HideClicked();

signals:
    void HideImageWindow(bool checkstate);
    void SendObjectToTreeView(QString selectedid);
private:
    Ui::ImageViewer* ui;
    ImageWindow* imagedialog;
    VideoViewer* videowindow;
protected:
    void closeEvent(QCloseEvent* event);
};

Q_DECLARE_METATYPE(ImageViewer*)
#endif // IMAGEVIEWER_H
