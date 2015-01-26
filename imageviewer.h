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
    ImageModel(QObject* parent = 0) : QAbstractListModel(parent)
    {
    };

    int rowCount(const QModelIndex &parent = QModelIndex()) const
    {
        qDebug() << "rowcount:" << thumblist.count();
        if(parent.row() >= thumblist.count())
            return 0;
        return thumblist.count();
    };

    /*
    QModelIndex index(int row, int column = 0, const QModelIndex& parent = QModelIndex()) const
    {
    };
    */
    QVariant data(const QModelIndex& index, int role) const
    {
        if(!index.isValid())
        {
            qDebug() << "index is not valid" << index.row();
            return QVariant();
        }
        if(index.row() >= thumblist.count())
        {
            qDebug() << "row is greater than thumblist count" << index.row();
            return QVariant();
        }
        if(role == Qt::DecorationRole)
        {
            qDebug() << "thumblistcount:" << thumblist.count();
            qDebug() << "decoration role:" << index.row();
            return QPixmap::fromImage(MakeThumb(thumblist.at(index.row())));
        }
        else
        {
            qDebug() << "role is not the decoration role" << index.row();
            return QVariant();
        }
    };
/*
    void GetThumbnails(void)
    {
        qDebug() << "thumbnail called";
        int row = 0;
        thumblist.clear();
        QSqlQuery thumbquery(thumbdb);
        thumbquery.prepare("SELECT thumbblob FROM thumbs;");
        if(thumbquery.exec())
        {
            beginInsertRows(QModelIndex(), row, thumbquery.size());
            while(thumbquery.next())
            {
                qDebug() << "row:" << row;
                row = thumblist.count();
                thumblist.insert(row, thumbquery.value(0).toString());
            }
            endInsertRows();
        }
        thumbquery.finish();
    };*/

    /*
     *    thumblist.clear();
    QSqlQuery thumbquery(thumbdb);
    thumbquery.prepare("SELECT thumbblob FROM thumbs;");
    if(thumbquery.exec())
    {
        while(thumbquery.next())
        {
            thumblist.append(thumbquery.value(0).toString());
            //thumblist.append(thumbquery.values(1).toString());
        }
    }

     *
     */ 
//private:
    //QList<QPixmap> pixmaplist;
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
private slots:
    void HideClicked();

signals:
    void HideImageWindow(bool checkstate);
private:
    Ui::ImageViewer* ui;
    ImageModel* imagemodel;
protected:
    void closeEvent(QCloseEvent* event);
};

Q_DECLARE_METATYPE(ImageViewer*)
#endif // IMAGEVIEWER_H
