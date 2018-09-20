#ifndef IMAGEVIEWER_H
#define IMAGEVIEWER_H

// Copyright 2015 Pasquale J. Rinaldi, Jr.
// Distrubted under the terms of the GNU General Public License version 2

#include "wombatinclude.h"
#include "globals.h"
#include "wombatfunctions.h"
#include "ui_imageviewer.h"
#include "ui_imagewindow.h"

class ImageModel : public QAbstractListModel
{
    Q_OBJECT

public:
    ImageModel(const QList<QPixmap> pixlist, QStringList ids, QStringList paths, QObject* parent = 0) : QAbstractListModel(parent), pixmaplist(pixlist), idlist(ids), pathlist(paths)
    {
    };

    int rowCount(const QModelIndex &parent = QModelIndex()) const
    {
        if(parent.row() >= pixmaplist.count())
            return 0;
        return pixmaplist.count();
    };

    QVariant data(const QModelIndex& index, int role) const
    {
        if(!index.isValid())
            return QVariant();
        if(index.row() >= pixmaplist.count())
            return QVariant();
        if(role == Qt::DecorationRole)
            return pixmaplist.at(index.row());
        else if(role == Qt::UserRole)
            return idlist.at(index.row());
        else if(role == Qt::ToolTipRole)
        {
            return QString(pathlist.at(index.row()));
        }
        else
            return QVariant();
    };


private:
    QList<QPixmap> pixmaplist;
    QStringList idlist;
    QStringList pathlist;
};

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
    void HideClicked();
    void ShowImage();
private:
    Ui::ImageWindow* ui;
    TskObject tskobj;
    TskObject* tskptr;
protected:
    void mousePressEvent(QMouseEvent* event);
};

class ImageViewer : public QDialog
{
    Q_OBJECT

public:
    ImageViewer(QWidget* parent = 0);
    ~ImageViewer();
    QListView* lw;
    QSpinBox* sb;
    void UpdateGeometries(void);
    //void UpdateThumbSize(void);
    void GetPixmaps(void);
    void ShowImage(const QModelIndex &index);
public slots:
    void OpenImageWindow(const QModelIndex &index);
private slots:
    void HighlightTreeViewItem(const QModelIndex &index);
    void HideClicked();
    //void SetModel();

signals:
    void HideImageWindow(bool checkstate);
    void SendObjectToTreeView(QString selectedid);
private:
    Ui::ImageViewer* ui;
    ImageModel* imagemodel;
    ImageWindow* imagedialog;
    QList<QPixmap> pixmaps;
    QStringList idlist;
    QFileSystemModel* filemodel;
    int pixcount;
protected:
    void closeEvent(QCloseEvent* event);
};

Q_DECLARE_METATYPE(ImageViewer*)
#endif // IMAGEVIEWER_H
