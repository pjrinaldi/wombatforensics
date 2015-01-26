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
    ImageModel(const QList<QPixmap> pixlist, QObject* parent = 0) : QAbstractListModel(parent), pixmaplist(pixlist)
    {
    };

    int rowCount(const QModelIndex &parent = QModelIndex()) const
    {
        //qDebug() << "rowcount:" << pixmaplist.count();
        //if(parent.row() >= pixmaplist.count())
        //    return 0;
        return pixmaplist.count();
    };

    QVariant data(const QModelIndex& index, int role) const
    {
        if(!index.isValid())
        {
            //qDebug() << "index is not valid" << index.row();
            return QVariant();
        }
        if(index.row() >= thumblist.count())
        {
            //qDebug() << "row is greater than thumblist count" << index.row();
            return QVariant();
        }
        if(role == Qt::DecorationRole)
        {
            //qDebug() << "decoration role:" << index.row();
            return pixmaplist.at(index.row());
            //return QPixmap::fromImage(MakeThumb(thumblist.at(index.row())));
        }
        else
        {
            //qDebug() << "role is not the decoration role" << index.row();
            return QVariant();
        }
    };

private:
    QList<QPixmap> pixmaplist;
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
    void HideClicked();

signals:
    void HideImageWindow(bool checkstate);
private:
    Ui::ImageViewer* ui;
    ImageModel* imagemodel;
    QList<QPixmap> pixmaps;
protected:
    void closeEvent(QCloseEvent* event);
};

Q_DECLARE_METATYPE(ImageViewer*)
#endif // IMAGEVIEWER_H
