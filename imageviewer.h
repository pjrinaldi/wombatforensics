#ifndef IMAGEVIEWER_H
#define IMAGEVIEWER_H

#include "wombatinclude.h"
#include "globals.h"
#include "wombatfunctions.h"
#include "ui_imageviewer.h"
#include "ui_imagewindow.h"

class ImageModel : public QAbstractListModel
{
    Q_OBJECT

public:
    ImageModel(const QList<QPixmap> pixlist, QStringList ids, QObject* parent = 0) : QAbstractListModel(parent), pixmaplist(pixlist), idlist(ids)
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
        if(index.row() >= thumblist.count())
            return QVariant();
        if(role == Qt::DecorationRole)
            return pixmaplist.at(index.row());
        else if(role == Qt::UserRole)
            return idlist.at(index.row());
        else if(role == Qt::ToolTipRole)
        {
            QSqlQuery pathquery(fcasedb);
            pathquery.prepare("SELECT (fullpath || name) AS fullname FROM data WHERE objectid = ?;");
            pathquery.addBindValue(index.data(Qt::UserRole).toInt());
            pathquery.exec();
            pathquery.next();
            thumbpath = pathquery.value(0).toString();
            pathquery.finish();
            return thumbpath; 
        }
        else
            return QVariant();
    };


private:
    QList<QPixmap> pixmaplist;
    QStringList idlist;
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
    void GetImage(int objid);
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
    void UpdateGeometries();
    void GetPixmaps();
private slots:
    void OpenImageWindow(const QModelIndex &index);
    void HighlightTreeViewItem(const QModelIndex &index);
    void HideClicked();

signals:
    void HideImageWindow(bool checkstate);
    void SendObjectToTreeView(int selectedid);
private:
    Ui::ImageViewer* ui;
    ImageModel* imagemodel;
    ImageWindow* imagedialog;
    QList<QPixmap> pixmaps;
    QStringList idlist;
protected:
    void closeEvent(QCloseEvent* event);
};

Q_DECLARE_METATYPE(ImageViewer*)
#endif // IMAGEVIEWER_H
