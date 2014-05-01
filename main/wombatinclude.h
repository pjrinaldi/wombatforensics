#ifndef WOMBATINCLUDE_H
#define WOMBATINCLUDE_H

#include <bitset>
#include <cstdio>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <QApplication>
#include <QActionGroup>
#include <QBoxLayout>
#include <QByteArray>
#include <QDebug>
#include <QDialog>
#include <QDir>
#include <QFileDialog>
#include <QFrame>
#include <QFuture>
#include <QFutureWatcher>
#include <QInputDialog>
#include <QList>
#include <QMainWindow>
#include <QMessageBox>
#include <QObject>
#include <QSizePolicy>
#include <QtSql/QtSql>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlRecord>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QStatusBar>
#include <QString>
#include <QStringList>
#include <QThreadPool>
#include <QTreeWidgetItem>
#include <QVector>
#include <QWebElement>
#include <QWebFrame>
#include <QWebView>
#include <QtConcurrent>
#include <QtPlugin>
#include <QtWebKitWidgets>
#include <QtWidgets>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sys/stat.h>

#include "tskvariable.h"
#include "translate.hpp"
#include "hexEditor.hpp"
#include "img/tsk_img_i.h"
#include "fs/tsk_fs_i.h"

class TreeProxy : public QAbstractProxyModel
{
public:
    TreeProxy() : QAbstractProxyModel() {};
    QModelIndex mapToSource(const QModelIndex &proxyIndex) const
    {
        if(!proxyIndex.isValid()) return QModelIndex();
        int c = columnFromIndex(proxyIndex);
        int r = rowFromIndex(proxyIndex);
        return sourceModel()->index(r,c);
    };
    QModelIndex mapFromSource(const QModelIndex &sourceIndex) const
    {
        // modify this to map in a tree...
        if(!sourceIndex.isValid()) return QModelIndex();
        if(sourceIndex.column()==0)
            return createIndex(sourceIndex.row(), 0, calculateId(sourceIndex));
        return createIndex(0, 0, calculateId(sourceIndex));
    };
    int columnCount(const QModelIndex &parent = QModelIndex()) const
    {
        return 1;
    };
    int rowCount(const QModelIndex &parent = QModelIndex()) const
    {
        if(!parent.isValid())
            return qMin(0x10000, sourceModel()->rowCount());
        int c = mapToSource(parent).column();
        if(c==sourceModel()->columnCount()-1)
            return 0;
        return 1;
    };
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const
    {
        if(parent.isValid())
        {
            // if parent is valid then in the source model
            // we want to receive the same row but the next column, provided that row==0 && col==0
            // otherwise the index is not valid 
            if(row!=0 || column!=0) return QModelIndex();
            return createIndex(row, column, (int)parent.internalId()+1);
        }
        if(column!=0) return QModelIndex();
        // parent is not valid thus we can calculate the id the same way as for the source model 
        return createIndex(row, 0, calculateId(row, 0));
    };
    QModelIndex parent(const QModelIndex &child) const
    {
        if(!child.isValid())
            return QModelIndex();
        // parent of an index in the source model is the same row but previous column 
        int c = mapToSource(child).column();
        int r = mapToSource(child).row();
        if(c==0)
        {
            // if original column == 0 then there is no parent 
            return QModelIndex();
        }
        c -= 1;
        if(c==0)
        {
            return createIndex(r, 0, calculateId(r, c));
        }
        return createIndex(0, 0, calculateId(r, c));
    };
private: 
    int columnFromIndex(const QModelIndex &proxyIndex) const
    {
        quint32 id = proxyIndex.internalId();
        int c = (id & 0xffff);
        return c;
    };
    int rowFromIndex(const QModelIndex &proxyIndex) const
    {
        quint32 id = proxyIndex.internalId();
        int r = (id & 0xffff0000) >> 16;
        return r;
    };
    int calculateId(const QModelIndex &sourceIndex) const
    {
        quint32 r = sourceIndex.row();
        quint32 c = sourceIndex.column();
        return calculateId(r, c);
    };
    int calculateId(quint32 r, quint32 c) const
    {
        return (((r & 0xffff) << 16) | (c & 0xffff));
    };
};

#endif // WOMBATINCLUDE_H
