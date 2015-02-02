#ifndef VIEWERMANAGER_H
#define VIEWERMANAGAER_H

#include "wombatinclude.h"
#include "globals.h"
#include "ui_viewermanager.h"

class ViewerModel : public QAbstractListModel
{
    Q_OBJECT

public:
    ViewerModel(const QStringList extlist, QObject* parent = 0) : QAbstractListModel(parent), externalviewers(extlist)
    {
    };

    int rowCount(const QModelIndex &parent = QModelIndex()) const
    {
        if(parent.row() >= externalviewers.count())
            return 0;
        return externalviewers.count();
    };

    QVariant data(const QModelIndex& index, int role) const
    {
        if(!index.isValid())
            return QVariant();
        if(index.row() >= externalviewers.count())
            return QVariant();
        if(role == Qt::DisplayRole)
            return externalviewers.at(index.row());
        else
            return QVariant();
    };

    void RemoveSelected(QModelIndex index)
    {
        QSqlQuery remquery(fappdb);
        remquery.prepare("UPDATE externalviewers SET deleted = 1 WHERE path = ?;");
        remquery.addBindValue(index.sibling(index.row(), 0).data().toString());
        remquery.exec();
        remquery.next();
        remquery.finish();
        beginRemoveRows(QModelIndex(), index.row(), index.row());
        for(int i=0; i < externalviewers.count(); i++)
        {
            if(index.sibling(index.row(), 0).data().toString().compare(externalviewers.at(i)) == 0)
            {
                externalviewers.removeAt(i);
                break;
            }
        }
        endRemoveRows();
        externallist = externalviewers;
    };

    void AddViewer(QString viewpath)
    {
        bool viewexists = false;
        int isdeleted = 0;
        QSqlQuery existquery(fappdb);
        existquery.prepare("SELECT path, deleted FROM externalviewers;");
        existquery.exec();
        while(existquery.next())
        {
            if(viewpath.compare(existquery.value(0).toString()) == 0)
            {
                viewexists = true;
                isdeleted = existquery.value(1).toInt();
            }
        }
        existquery.finish();
        beginInsertRows(QModelIndex(), 0, 1);
        if(viewexists == true)
        {
            if(isdeleted == 1)
            {
                existquery.prepare("UPDATE externalviewers SET deleted = 0 WHERE path = ?;");
                existquery.addBindValue(viewpath);
                existquery.exec();
                existquery.next();
                existquery.finish();
            }
        }
        else
        {
            existquery.prepare("INSERT INTO externalviewers (path, deleted) VALUES(?, 0);");
            existquery.addBindValue(viewpath);
            existquery.exec();
            existquery.next();
            existquery.finish();
        }
        externalviewers.append(viewpath);
        endInsertRows();
        externallist = externalviewers;
    };
private:
    QStringList externalviewers;
};

namespace Ui
{
    class ViewerManager;
}

class ViewerManager : public QDialog
{
    Q_OBJECT

public:
    ViewerManager(QWidget* parent = 0);
    ~ViewerManager();

private slots:
    void HideClicked();
    void ShowBrowser();
    void AddViewer();
    void RemoveSelected();
    void SelectionChanged(const QItemSelection &newitem, const QItemSelection &previtem);
signals:
    void HideManagerWindow(bool checkstate);

protected:
    void closeEvent(QCloseEvent* event);
private:
    Ui::ViewerManager* ui;
    QString fileviewerpath;
    ViewerModel* viewmodel; 
    QStringList externlist;
    QModelIndex selectedindex;
};

Q_DECLARE_METATYPE(ViewerManager*);

#endif // VIDEOMANAGER_H
