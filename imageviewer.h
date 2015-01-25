#ifndef IMAGEVIEWER_H
#define IMAGEVIEWER_H

#include "wombatinclude.h"
#include "globals.h"
#include "ui_imageviewer.h"

/*
 *
class PropertyModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    PropertyModel(const QStringList plist, QObject* parent = 0) : QAbstractTableModel(parent), proplist(plist)
    {
    };

    int rowCount(const QModelIndex &parent = QModelIndex()) const
    {
        if(parent.row() >= proplist.count()/3)
            return 0;
        return ((int)proplist.count())/((int)3);
    };

    int columnCount(const QModelIndex &parent = QModelIndex()) const
    {
        if(parent.row() >= proplist.count()/3)
            return 0;
        return 3;
    }

    QVariant data(const QModelIndex &index, int role) const
    {
        if(!index.isValid())
            return QVariant();
        if(index.row() >= proplist.count()/3)
            return QVariant();
        if(role == Qt::DisplayRole)
        {
            if(index.column() == 0)
                return proplist.at(3*index.row());
            else if(index.column() == 1)
                return proplist.at(3*index.row() + 1);
            else if(index.column() == 2)
                return proplist.at(3*index.row() + 2);
            else
                return QVariant();
        }
        else
            return QVariant();
    };

    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const
    {
        QStringList headerdata;
        headerdata << "Property" << "Value" << "Description";
        if(role != Qt::DisplayRole)
            return QVariant();
        if(orientation == Qt::Horizontal)
            return headerdata.at(section);
        return QVariant();
    };

private:
    QStringList proplist;
};

 *
 */ 
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
protected:
    void closeEvent(QCloseEvent* event);
};

Q_DECLARE_METATYPE(ImageViewer*)
#endif // IMAGEVIEWER_H
