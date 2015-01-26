#ifndef IMAGEVIEWER_H
#define IMAGEVIEWER_H

#include "wombatinclude.h"
#include "globals.h"
#include "wombatfunctions.h"
#include "ui_imageviewer.h"

class ImageModel : public QAbstractListModel
{
    Q_OBJECT

public:
    ImageModel(const QList<QPixmap> pixlist, QStringList ids, QStringList addlist, QObject* parent = 0) : QAbstractListModel(parent), pixmaplist(pixlist), idlist(ids), addresslist(addlist)
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
        {
            return QVariant();
        }
        if(index.row() >= thumblist.count())
        {
            return QVariant();
        }
        if(role == Qt::DecorationRole)
        {
            return pixmaplist.at(index.row());
        }
        else if(role == Qt::UserRole)
            return idlist.at(index.row());
        else if(role == Qt::UserRole + 1)
            return addresslist.at(index.row());
        else
        {
            return QVariant();
        }
    };


private:
    QList<QPixmap> pixmaplist;
    QStringList idlist;
    QStringList addresslist;
};

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
private:
    Ui::ImageViewer* ui;
    ImageModel* imagemodel;
    QList<QPixmap> pixmaps;
    QStringList idlist;
    QStringList addresslist;
protected:
    void closeEvent(QCloseEvent* event);
};

Q_DECLARE_METATYPE(ImageViewer*)
#endif // IMAGEVIEWER_H
