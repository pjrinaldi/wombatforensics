#ifndef PROPERTIESWINDOW_H
#define PROPERTIESWINDOW_H

// Copyright 2015 Pasquale J. Rinaldi, Jr.
// Distrubted under the terms of the GNU General Public License version 2

#include "wombatinclude.h"
#include "ui_propertieswindow.h"
#include "globals.h"

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

namespace Ui {
class PropertiesWindow;
}

class PropertiesWindow : public QDialog
{
    Q_OBJECT

public:
    PropertiesWindow(QWidget* parent = 0);
    void UpdateTableView(void);
    ~PropertiesWindow();


private slots:
    void SelectionChanged(const QItemSelection &selitem, const QItemSelection &deselitem);
    void HideClicked();
signals:
    void HidePropertyWindow(bool checkstate);

protected:
    void closeEvent(QCloseEvent* event);
private:
    Ui::PropertiesWindow *ui;
    PropertyModel* pmodel;
    QModelIndex selectedindex;
    QModelIndex oldselectedindex;
};

Q_DECLARE_METATYPE(PropertiesWindow*)

#endif // PROPERTIESWINDOW_H
