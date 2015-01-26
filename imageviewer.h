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
        if(parent.row() >= thumblist.count())
            return 0;
        return thumblist.count();
    };

    QVariant data(const QModelIndex& index, int role) const
    {
        if(!index.isValid())
            return QVariant();
        //if(index.row() >= thumblist.count())
        //    return QVariant();
        if(role == Qt::DecorationRole)
        {
            qDebug() << "thumblistcount:" << thumblist.count();
            qDebug() << "decoration role:" << index.row();
            return QPixmap::fromImage(MakeThumb(thumblist.at(index.row())));
        }
        else
            return QVariant();
    };

    void GetThumbnails(void)
    {
        qDebug() << "thumbnail called";
        int row = 0;
        thumblist.clear();
        QSqlQuery thumbquery(thumbdb);
        thumbquery.prepare("SELECT thumbblobk FROM thumbs;");
        if(thumbquery.exec())
        {
            beginInsertRows(QModelIndex(), row, row);
            while(thumbquery.next())
            {
                qDebug() << "row:" << row;
                row = thumblist.count();
                thumblist.insert(row, thumbquery.value(0).toString());
            }
            endInsertRows();
        }
        thumbquery.finish();
    };

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
