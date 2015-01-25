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
        if(index.row() >= thumblist.count())
            return QVariant();
        if(role == Qt::DecorationRole)
        {
            return QPixmap::fromImage(MakeThumb(thumblist.at(index.row())));
        }
        else
            return QVariant();
    };
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
