#ifndef WOMBATFORENSICS_H
#define WOMBATFORENSICS_H

// Copyright 2015 Pasquale J. Rinaldi, Jr.
// Distrubted under the terms of the GNU General Public License version 2

#include "wombatinclude.h"
#include "wombatvariable.h"
#include "wombatdatabase.h"
#include "wombatframework.h"
#include "wombatfunctions.h"
#include "ui_wombatforensics.h"
#include "propertieswindow.h"
#include "exportdialog.h"
#include "fileviewer.h"
#include "globals.h"
#include "imgreader.h"
#include "imghexviewer.h"
#include "filterviews.h"
#include "imageviewer.h"
#include "videoviewer.h"
#include "viewermanager.h"
#include "textviewer.h"
#include "htmlviewer.h"
#include "messageviewer.h"
#include "byteconverter.h"
#include "digdeeperdialog.h"
#include "aboutbox.h"

class WombatSlider : public QSlider
{
    Q_OBJECT

public:
    WombatSlider(QWidget* parent = 0) : QSlider(parent) {};
signals:
    void ShowJumpFilter(void);

protected:
    void mousePressEvent(QMouseEvent* event)
    {
        if(event->button() == Qt::RightButton)
        {
            emit ShowJumpFilter();
            event->accept();
        }
        else
        {
            event->accept();
            QSlider::mousePressEvent(event);
        }
    };
};
/*
class TableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    TableModel(QObject* parent = 0) : QAbstractTableModel(parent)
    {
        headerdata << "ID" << "Name" << "Full Path" << "Size (bytes)" << "Object Type" << "Address" << "Created" << "Accessed" << "Modified" << "Status Changed" << "MD5" << "Parent ID" << "Item Type" << "Parent Image ID" << "Parent FS ID" << "Flags" << "File Signature" << "File Category" << "Checked" << "MFT Attribute ID";
        QList<QVariant> emptyset;
        dummynode = 0;
        emptyset << "" << "" << "" << "" << "" << "" << "" << "" << "" << "" << "" << "" << "" << "" << "" << "" << "" << "" << "" << "";
    };

    ~TableModel()
    {
    };

    int rowCount(const QModelIndex &parent = QModelIndex()) const
    {
        if(parent.isValid())
            return 0;
        return 5;
    };

    int columnCount(const QModelIndex &parent = QModelIndex()) const
    {
        if(parent.isValid())
            return 0;
        else
            return 20;
    };

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const
    {
        return QVariant();
    };

    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const
    {
        return QVariant();
    };

    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole)
    {
        return false;
    };

    Qt::ItemFlags flags(const QModelIndex &index) const
    {
        Qt::ItemFlags flags = QAbstractTableModel::flags(index);

        if(!index.isValid())
            return Qt::NoItemFlags;
        if(index == QModelIndex())
            return Qt::NoItemFlags;
        if(index.column() == 0)
        {
            flags |= Qt::ItemIsUserCheckable;
            if(index.model()->hasChildren(index))
                flags |= Qt::ItemIsTristate;
        }
        
        return flags;
    };

    bool canFetchMore(const QModelIndex &parent = QModelIndex()) const
    {
        if(parent.isValid())
            return false;
        else
            return false;
    };

    void fetchMore(const QModelIndex &parent = QModelIndex())
    {
        if(parent.isValid())
            qDebug() << "its broken";
    };

    Node* NodeFromIndex(const QModelIndex &index) const
    {
        if(index.isValid())
            return static_cast<Node*>(index.internalPointer());
        else
            return rootnode;
    };

    QStringList headerdata;
};
*/
class TreeModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    TreeModel(QObject* parent = 0) : QAbstractItemModel(parent)
    {
        headerdata << "ID" << "Name" << "Full Path" << "Size (bytes)" << "Object Type" << "Address" << "Created (UTC)" << "Accessed (UTC)" << "Modified (UTC)" << "Status Changed (UTC)" << "MD5 Hash" << "Parent ID" << "Item Type" << "Parent Image ID" << "Parent FS ID" << "Flags" << "File Signature" << "File Category" << "Checked" << "MFT Attribute ID";
        rootnode = 0;
        QList<QVariant> emptyset;
        emptyset << "" << "" << "" << "" << "" << "" << "" << "" << "" << "" << "" << "" << "" << "" << "" << "" << "" << "" << "" << "";
        rootnode = new Node(emptyset);
        rootnode->parent = 0;
        rootnode->childcount = 0;
    };

    ~TreeModel()
    {
        delete rootnode;
    };

    enum MyDataRoles
    {
        IndexPtrRole = Qt::UserRole + 1
    };

    QModelIndex index(int row, int col, const QModelIndex &parent) const
    {
        if(!rootnode || row < 0 || col < 0)
            return QModelIndex();
        Node* parentnode = NodeFromIndex(parent);
        Node* childnode = parentnode->children.value(row);
        if(!childnode)
            return QModelIndex();
        return createIndex(row, col, childnode);
    };

    QModelIndex parent(const QModelIndex &child) const
    {
        Node* node = NodeFromIndex(child);
        if(!node)
            return QModelIndex();
        Node* parentnode = node->parent;
        if(!parentnode)
            return QModelIndex();
        Node* grandparentnode = parentnode->parent;
        if(!grandparentnode)
            return QModelIndex();
        int row = grandparentnode->children.indexOf(parentnode);
        return createIndex(row, 0, parentnode);
    };

    int rowCount(const QModelIndex &parent) const
    {
        if(parent == QModelIndex())
            return rootnode->childcount;
        if(parent.column() > 0)
            return 0;
        Node* parentnode = rootnode; 
        if(parent.isValid())
            parentnode = NodeFromIndex(parent);
        return parentnode->childcount;
    };

    int columnCount(const QModelIndex &parent) const
    {
        return NodeFromIndex(parent)->nodevalues.count();
    };

    QVariant data(const QModelIndex &index, int role) const
    {
        if(index == QModelIndex())
            return QVariant();
        Node* node = rootnode; 
        if(index.isValid())
            node = NodeFromIndex(index);
        if(role == Qt::CheckStateRole)
        {
            if(index.column() == 0)
                return QVariant(GetCheckState(node));
            else
                return QVariant();
        }
        if(role == Qt::ForegroundRole)
        {
            if(filtervalues.maxidbool && filtervalues.minidbool == false)
            {
                if(node->nodevalues.at(0).toULongLong() <= filtervalues.maxid)
                    return QColor(Qt::lightGray);
            }
            if(filtervalues.minidbool && filtervalues.maxidbool == false)
            {
                if(node->nodevalues.at(0).toULongLong() >= filtervalues.minid)
                    return QColor(Qt::lightGray);
            }
            if(filtervalues.maxidbool && filtervalues.minidbool)
            {
                if(node->nodevalues.at(0).toULongLong() >= filtervalues.minid || node->nodevalues.at(0).toULongLong() <= filtervalues.maxid)
                    return QColor(Qt::lightGray);
            }
            if(filtervalues.namebool)
            {
                if(node->nodevalues.at(1).toString().contains(filtervalues.namefilter) == false)
                    return QColor(Qt::lightGray);
            }
            if(filtervalues.pathbool)
            {
                if(node->nodevalues.at(2).toString().contains(filtervalues.pathfilter) == false)
                    return QColor(Qt::lightGray);
            }
            if(filtervalues.maxsizebool && filtervalues.minsizebool == false)
            {
                if(node->nodevalues.at(3).toULongLong() <= filtervalues.maxsize)
                    return QColor(Qt::lightGray);
            }
            if(filtervalues.minsizebool && filtervalues.maxsizebool == false)
            {
                if(node->nodevalues.at(3).toULongLong() >= filtervalues.minsize)
                    return QColor(Qt::lightGray);
            }
            if(filtervalues.maxsizebool && filtervalues.minsizebool)
            {
                if(node->nodevalues.at(3).toULongLong() >= filtervalues.minsize || node->nodevalues.at(3).toULongLong() <= filtervalues.maxsize)
                    return QColor(Qt::lightGray);
            }
            if(node->nodevalues.at(4).toInt() == 5)
            {
                if(filtervalues.maxcreatebool && filtervalues.mincreatebool == false)
                {
                    if(node->nodevalues.at(6).toInt() <= filtervalues.maxcreate)
                        return QColor(Qt::lightGray);
                }
                if(filtervalues.maxcreatebool == false && filtervalues.mincreatebool)
                {
                    if(node->nodevalues.at(6).toInt() >= filtervalues.mincreate)
                        return QColor(Qt::lightGray);
                }
                if(filtervalues.maxcreatebool && filtervalues.mincreatebool)
                {
                    if(node->nodevalues.at(6).toInt() >= filtervalues.mincreate || node->nodevalues.at(6).toInt() <= filtervalues.maxcreate)
                        return QColor(Qt::lightGray);
                }
                if(filtervalues.maxaccessbool && filtervalues.minaccessbool == false)
                {
                    if(node->nodevalues.at(7).toInt() <= filtervalues.maxaccess)
                        return QColor(Qt::lightGray);
                }
                if(filtervalues.maxaccessbool == false && filtervalues.minaccessbool)
                {
                    if(node->nodevalues.at(7).toInt() >= filtervalues.minaccess)
                        return QColor(Qt::lightGray);
                }
                if(filtervalues.maxaccessbool && filtervalues.minaccessbool)
                {
                    if(node->nodevalues.at(7).toInt() >= filtervalues.minaccess || node->nodevalues.at(7).toInt() <= filtervalues.maxaccess)
                        return QColor(Qt::lightGray);
                }
                if(filtervalues.maxmodifybool && filtervalues.minmodifybool == false)
                {
                    if(node->nodevalues.at(8).toInt() <= filtervalues.maxmodify)
                        return QColor(Qt::lightGray);
                }
                if(filtervalues.maxmodifybool == false && filtervalues.minmodifybool)
                {
                    if(node->nodevalues.at(8).toInt() >= filtervalues.minmodify)
                        return QColor(Qt::lightGray);
                }
                if(filtervalues.maxmodifybool && filtervalues.minmodifybool)
                {
                    if(node->nodevalues.at(8).toInt() >= filtervalues.minmodify || node->nodevalues.at(8).toInt() <= filtervalues.maxmodify)
                        return QColor(Qt::lightGray);
                }
                if(filtervalues.maxchangebool && filtervalues.minchangebool == false)
                {
                    if(node->nodevalues.at(9).toInt() <= filtervalues.maxchange)
                        return QColor(Qt::lightGray);
                }
                if(filtervalues.maxchangebool == false && filtervalues.minchangebool)
                {
                    if(node->nodevalues.at(9).toInt() >= filtervalues.minchange)
                        return QColor(Qt::lightGray);
                }
                if(filtervalues.maxchangebool && filtervalues.minchangebool)
                {
                    if(node->nodevalues.at(9).toInt() >= filtervalues.minchange || node->nodevalues.at(9).toInt() <= filtervalues.maxchange)
                        return QColor(Qt::lightGray);
                }
                if(filtervalues.filecategorybool && filtervalues.filetypebool == false)
                {
                    if(node->nodevalues.at(16).toString().contains(filtervalues.filecategory) == false)
                        return QColor(Qt::lightGray);
                }
                if(filtervalues.filecategorybool == false && filtervalues.filetypebool)
                {
                    if(node->nodevalues.at(16).toString().contains(filtervalues.filetype) == false)
                        return QColor(Qt::lightGray);
                }
                if(filtervalues.filecategorybool && filtervalues.filetypebool)
                {
                    if(node->nodevalues.at(16).toString().contains(filtervalues.filecategory) == false || node->nodevalues.at(16).toString().contains(filtervalues.filetype) == false)
                        return QColor(Qt::lightGray);
                }
                if(filtervalues.filegroupbool)
                {
                    if(node->nodevalues.at(17).toString().contains(filtervalues.filegroup) == false)
                            return QColor(Qt::lightGray);
                }
                if(filtervalues.hashbool)
                {
                    for(int i=0; i < filtervalues.hashidlist.count(); i++)
                    {
                        if(node->nodevalues.at(0).toULongLong() == filtervalues.hashidlist.at(i))
                            return QColor(Qt::lightGray);
                    }
                }
            }
        }
        if(role == Qt::DisplayRole)
        {
            if(index.column() == 2)
            {
                if(node->nodevalues.at(4).toInt() == 1)
                    return QString("");
                else
                    return node->nodevalues.at(index.column());
            }
            else if(index.column() >= 6 && index.column() <= 9)
            {
                char buf[128];
                QString tmpstr = QString(TskTimeToStringUTC(node->nodevalues.at(index.column()).toInt(), buf));
                return tmpstr;
            }
            else
                return node->nodevalues.at(index.column());
        }
        if(role == Qt::DecorationRole)
        {
            int nodetype = node->nodevalues.at(4).toInt();
            QString nodename = node->nodevalues.at(1).toString();
            if(index.column() == 0)
            {
                if(nodetype == 1)
                    return QIcon(QPixmap(QString(":/basic/treeimg")));
                else if(nodetype == 2)
                    return QIcon(QPixmap(QString(":/basic/treevol")));
                else if(nodetype == 3)
                    return QIcon(QPixmap(QString(":/basic/treepart")));
                else if(nodetype == 4)
                    return QIcon(QPixmap(QString(":/basic/treefs")));
                else if(nodetype == 5)
                {
                    int itemtype = node->nodevalues.at(12).toInt();
                    if(itemtype == 5)
                    {
                        if(nodename.compare("AttrDef") == 0 || nodename.compare("$BadClus") == 0 || nodename.compare("$Bitmap") == 0 || nodename.compare("$Boot") == 0 || nodename.compare("$ObjId") == 0 || nodename.compare("$Quota") == 0 || nodename.compare("$Reparse") == 0 || nodename.compare("$LogFile") == 0 || nodename.compare("$MFT") == 0 || nodename.compare("$MFTMirr") == 0 || nodename.compare("$Secure") == 0 || nodename.compare("$UpCase") == 0 || nodename.compare("$Volume") == 0)
                            return QIcon(QPixmap(QString(":/basic/virtualfile")));
                        else
                            return QIcon(QPixmap(QString(":/basic/treefile")));
                    }
                    else if(itemtype == 3)
                    {
                        if(nodename.compare("$OrphanFiles") == 0 || nodename.compare("$Extend") == 0)
                            return QIcon(QPixmap(QString(":/basic/virtualfolder")));
                        else
                            return QIcon(QPixmap(QString(":/basic/treefolder")));
                    }
                    else if(itemtype == 10)
                        return QIcon(QPixmap(QString(":/basic/virtualfile")));
                    return QIcon(QPixmap(QString(":/basic/treefile")));
                }
                else if(nodetype == 6)
                    return QIcon(QPixmap(QString(":/basic/virtualfile")));
                return QVariant();
            }
            
            return QVariant();
        }

        return QVariant();
    };

    bool setData(const QModelIndex &index, const QVariant &value, int role)
    {
        if(index.column() == 0 && role == Qt::CheckStateRole)
        {
            Qt::CheckState state = static_cast<Qt::CheckState>(value.toInt());
            return SetCheckState(index, state);
        }
        if(role == Qt::DisplayRole)
        {
            if(value.toInt() == -15)
            {
                Node* parentnode = NodeFromIndex(index);
                QSqlQuery childupdatequery(fcasedb);
                childupdatequery.prepare("SELECT objectid, name, fullpath, size, objecttype, address, crtime, atime, mtime, ctime, md5, parentid, type, parimgid, parfsid, flags, filemime, filesignature, checked, mftattrid FROM data WHERE objectid = ?;");
                for(int i=0; i < parentnode->children.count(); i++)
                {
                    childupdatequery.addBindValue(parentnode->children.at(i)->nodevalues.at(0).toULongLong());
                    childupdatequery.exec();
                    childupdatequery.next();
                    for(int j=0; j < childupdatequery.record().count(); j++)
                    {
                        parentnode->children.at(i)->nodevalues[j] = childupdatequery.value(j);
                    }
                }
                childupdatequery.finish();
                emit dataChanged(index.child(0, 0), index.child(parentnode->children.count() - 1, 0));
                return true;
            }
            else
            {
                Node* currentnode = NodeFromIndex(index);
                QSqlQuery updatequery(fcasedb);
                updatequery.prepare("SELECT objectid, name, fullpath, size, objecttype, address, crtime, atime, mtime, ctime, md5, parentid, type, parimgid, parfsid, flags, filemime, filesignature, checked, mftattrid FROM data WHERE objectid = ?;");
                updatequery.addBindValue(currentnode->nodevalues.at(0).toULongLong());
                updatequery.exec();
                updatequery.next();
                for(int i=0; i < updatequery.record().count(); i++)
                {
                    currentnode->nodevalues[i] = updatequery.value(i);
                }
                updatequery.finish();
                emit dataChanged(index, index);
                return true;
            }
        }
        return false;
    };

    void dataChanged(const QModelIndex &topleftindex, const QModelIndex &botrightindex, const QVector<int> &roles = QVector<int>())
    {
        foreach(int role, roles)
        {
        if(role == Qt::DisplayRole)
        {
            Node* startnode = NodeFromIndex(topleftindex);
            Node* endnode = NodeFromIndex(botrightindex);
            QSqlQuery updatequery(fcasedb);
            updatequery.prepare("SELECT objectid, name, fullpath, size, objecttype, address, crtime, atime, mtime, ctime, md5, parentid, type, parimgid, parfsid, flags, filemime, filesignature, checked, mftattrid FROM data WHERE objectid == ? OR objectid == ?;");
            updatequery.addBindValue(startnode->nodevalues.at(0).toULongLong());
            updatequery.exec();
            updatequery.next();
            startnode->nodevalues[10] = updatequery.value(10).toString();
            updatequery.addBindValue(endnode->nodevalues.at(0).toULongLong());
            updatequery.exec();
            updatequery.next();
            endnode->nodevalues[10] = updatequery.value(10).toString();
            updatequery.finish();
        }
        }
    };

    QVariant headerData(int section, Qt::Orientation orientation, int role) const
    {
        if(orientation == Qt::Horizontal && role == Qt::DisplayRole)
        {
            if(section >= 0)
                return headerdata.at(section);
        }
        if(role == Qt::DecorationRole)
        {
            if(section == 0 && (filtervalues.maxidbool || filtervalues.minidbool))
            {
                return QIcon(QPixmap(QString(":/basic/filterimg")));
            }
            if(section == 1 && filtervalues.namebool)
            {
                return QIcon(QPixmap(QString(":/basic/filterimg")));
            }
            if(section == 2 && filtervalues.pathbool)
                return QIcon(QPixmap(QString(":/basic/filterimg")));
            if(section == 3 && (filtervalues.maxsizebool || filtervalues.minsizebool))
                return QIcon(QPixmap(QString(":/basic/filterimg")));
            if(section == 6 && (filtervalues.maxcreatebool || filtervalues.mincreatebool))
                return QIcon(QPixmap(QString(":/basic/filterimg")));
            if(section == 7 && (filtervalues.maxaccessbool || filtervalues.minaccessbool))
                return QIcon(QPixmap(QString(":/basic/filterimg")));
            if(section == 8 && (filtervalues.maxmodifybool || filtervalues.minmodifybool))
                return QIcon(QPixmap(QString(":/basic/filterimg")));
            if(section == 9 && (filtervalues.maxchangebool || filtervalues.minchangebool))
                return QIcon(QPixmap(QString(":/basic/filterimg")));
            if(section == 16 && (filtervalues.filecategorybool || filtervalues.filetypebool))
                return QIcon(QPixmap(QString(":/basic/filterimg")));
            if(section == 10 && filtervalues.hashbool)
                return QIcon(QPixmap(QString(":/basic/filterimg")));
            if(section == 17 && filtervalues.filegroupbool)
                return QIcon(QPixmap(QString(":/basic/filterimg")));
        }
        return QVariant();
    };

    Qt::ItemFlags flags(const QModelIndex &index) const
    {
        Qt::ItemFlags flags = QAbstractItemModel::flags(index);

        if(!index.isValid())
            return Qt::NoItemFlags;
        if(index == QModelIndex())
            return Qt::NoItemFlags;
        if(index.column() == 0)
        {
            flags |= Qt::ItemIsUserCheckable;
            if(index.model()->hasChildren(index))
                flags |= Qt::ItemIsTristate;
        }
        
        return flags;
    };

    bool hasChildren(const QModelIndex &parent = QModelIndex()) const
    {
        if(parent == QModelIndex())
            return true;
        parentnode = rootnode;
        if(parent.isValid())
            parentnode = NodeFromIndex(parent);
        if(rowCount(parent) > 0)
        {
            return true;
        }
        return false;
    };


    bool canFetchMore(const QModelIndex &parent = QModelIndex()) const
    {
        if(parent == QModelIndex())
            return false;
        Node* parentnode = NodeFromIndex(parent);
        if(parentnode == rootnode)
            return false;
        if((unsigned long long)parentnode->children.count() < parentnode->childcount && parentnode->haschildren == true)
            return true;
        return false;
    };

    void fetchMore(const QModelIndex &parent = QModelIndex())
    {
        qDebug() << "parentnode->children.count():" << parentnode->children.count();
        qDebug() << "parentnode->childcount:" << parentnode->childcount;
        Node* parentnode = NodeFromIndex(parent);
        QList<QVariant> fetchvalues;
        fetchvalues.clear();
        if(parentnode->haschildren == true)
        {
            QSqlQuery morequery(fcasedb);
            QString morestring = "SELECT objectid, name, fullpath, size, objecttype, address, crtime, atime, mtime, ctime, md5, parentid, type, parimgid, parfsid, flags, filemime, filesignature, checked, mftattrid FROM data WHERE (objecttype = 5 OR objecttype = 6)";
            if(parentnode->nodevalues.at(4).toInt() == 4)
            {
                morestring += " AND parfsid = ?";
                morequery.prepare(morestring);
                morequery.addBindValue(parentnode->nodevalues.at(0).toULongLong());
            }
            else
            {
                morestring += " AND parentid = ? AND parimgid = ?";
                morequery.prepare(morestring);
                morequery.addBindValue(parentnode->nodevalues.at(5).toULongLong());
                morequery.addBindValue(parentnode->nodevalues.at(13).toULongLong());
            }
            //morequery.prepare("SELECT objectid, name, fullpath, size, objecttype, address, crtime, atime, mtime, ctime, md5, parentid, type, parimgid, parfsid, flags, filemime, filesignature, checked, mftattrid FROM data WHERE (objecttype = 5 OR objecttype = 6) AND parentid = ? AND parimgid = ?");
            //morequery.addBindValue(parentnode->nodevalues.at(5).toULongLong());
            //morequery.addBindValue(parentnode->nodevalues.at(13).toULongLong());
            //beginInsertRows(parent, 0, 0);
            if(morequery.exec())
            {
                while(morequery.next())
                {
                    fetchvalues.clear();
                    for(int i=0; i < morequery.record().count(); i++)
                        fetchvalues.append(morequery.value(i));
                    Node* curchild = new Node(fetchvalues);
                    curchild->parent = parentnode;
                    if(QString(".").compare(curchild->nodevalues.at(1).toString()) == 0 || QString("..").compare(curchild->nodevalues.at(1).toString()) == 0)
                    {
                        curchild->childcount = 0;
                        curchild->haschildren = false;
                    }
                    else
                    {
                        // CHILDCOUNT SET HERE
                        curchild->childcount = GetChildCount(5, curchild->nodevalues.at(5).toULongLong(), parentnode->nodevalues.at(13).toULongLong());
                        curchild->haschildren = curchild->HasChildren();
                    }
                    if(morequery.value(18).toInt() == 0)
                        curchild->checkstate = 0;
                    else if(morequery.value(18).toInt() == 1)
                        curchild->checkstate = 1;
                    else
                        curchild->checkstate = 2;
                    parentnode->children.append(curchild);
                }
                emit checkedNodesChanged();
                setData(parent, QVariant(-15), Qt::DisplayRole);
            }
        }
    };
    
    void GetModelCount(Node* curnode)
    {
        if(curnode->nodevalues.at(4).toInt() == 5 || curnode->nodevalues.at(4).toInt() == 6)
        {
            totalcount++;
            if(curnode->checkstate == 2)
                totalchecked++;
        }
        if(curnode->haschildren)
        {
            for(int i=0; i < curnode->children.count(); i++)
            {
                GetModelCount(curnode->children[i]);
            }
        }
    };

    void RemEvidence(unsigned long long curid)
    {
        int rownumber = rootnode->GetChildRow(curid);
        beginRemoveRows(QModelIndex(), rownumber, rownumber);
        rootnode->RemoveChild(rownumber);
        rootnode->childcount--;
        endRemoveRows();
    };

    void AddEvidence(unsigned long long curid)
    {
        int filesystemcount;
        QSqlQuery addevidquery(fcasedb);
        addevidquery.prepare("SELECT objectid, name, fullpath, size, objecttype, address, crtime, atime, mtime, ctime, md5, parentid, type, parimgid, parfsid, flags, filemime, filesignature, checked, mftattrid FROM data WHERE objectid = ? OR (objecttype < 6 AND parimgid = ?)");
        addevidquery.addBindValue(curid);
        addevidquery.addBindValue(curid);
        if(addevidquery.exec())
        {
            beginInsertRows(QModelIndex(), rootnode->childcount, rootnode->childcount);
            while(addevidquery.next())
            {
                currentnode = 0;
                colvalues.clear();
                for(int i=0; i < addevidquery.record().count(); i++)
                    colvalues.append(addevidquery.value(i));
                currentnode = new Node(colvalues);
                if(currentnode->nodevalues.at(4).toInt() == 1) // image file
                {
                    filesystemcount = 0;
                    rootnode->children.append(currentnode);
                    rootnode->childcount++;
                    rootnode->haschildren = rootnode->HasChildren();
                    currentnode->parent = rootnode;
                    currentnode->childcount = GetChildCount(1, currentnode->nodevalues.at(0).toULongLong());
                    currentnode->haschildren = currentnode->HasChildren();
                    parentnode = currentnode;
                }
                else if(currentnode->nodevalues.at(4).toInt() == 2) // volume
                {
                    currentnode->parent = parentnode;
                    parentnode->children.append(currentnode);
                    currentnode->childcount = GetChildCount(2, currentnode->nodevalues.at(0).toULongLong(), curid);
                    currentnode->haschildren = currentnode->HasChildren();
                    parentnode = currentnode;
                }
                else if(currentnode->nodevalues.at(4).toInt() == 3) // determine if its an unallocated partition space
                {
                    if(currentnode->nodevalues.at(15).toInt() == 2) // unallocated partition, add to parent as a child.
                    {
                        currentnode->parent = parentnode;
                        parentnode->children.append(currentnode); 
                    }
                }
                // THERE SHOULD ONLY BE PARTITION OBJECTS, OBJECTTYPE == 3, FOR UNALLOCATED SPACES....
                else if(currentnode->nodevalues.at(4).toInt() == 4) // filesystem
                {
                    currentnode->parent = parentnode;
                    parentnode->children.append(currentnode);
                    if(filesystemcount <= fsobjectlist.count())
                    {
                        //currentnode->childcount = GetChildCount(4, fsobjectlist.at(filesystemcount).address, curid);
                        currentnode->childcount = GetChildCount(4, fsobjectlist.at(filesystemcount).rootinum, curid);
                        filesystemcount++;
                    }
                    currentnode->haschildren = currentnode->HasChildren();
                }
                if(addevidquery.value(18).toInt() == 0)
                    currentnode->checkstate = 0;
                else if(addevidquery.value(18).toInt() == 1)
                    currentnode->checkstate = 1;
                else
                    currentnode->checkstate = 2;
            }
            QSqlQuery filequery(fcasedb);
            Node* rootdirectory = 0;
            for(int j=0; j < fsobjectlist.count(); j++)
            {
                filequery.prepare("SELECT objectid, name, fullpath, size, objecttype, address, crtime, atime, mtime, ctime, md5, parentid, type, parimgid, parfsid, flags, filemime, filesignature, checked, mftattrid FROM data WHERE (objecttype = 5 OR objecttype = 6) AND parimgid = ? AND parentid = ?)");
                filequery.addBindValue(curid);
                //filequery.addBindValue(fsobjectlist.at(j).address);
                filequery.addBindValue(fsobjectlist.at(j).rootinum);
                if(filequery.exec())
                {
                    while(filequery.next())
                    {
                        for(int i=0; i < parentnode->children.count(); i++)
                        {
                            if(filequery.value(14).toULongLong() == parentnode->children.at(i)->nodevalues.at(0).toULongLong())
                                rootdirectory = parentnode->children.at(i);
                        }
                        currentnode->parent = rootdirectory;
                        if(QString(".").compare(currentnode->nodevalues.at(1).toString()) == 0 || QString("..").compare(currentnode->nodevalues.at(1).toString()) == 0)
                        {
                            currentnode->childcount = 0;
                            currentnode->haschildren = false;
                        }
                        else
                        {
                            currentnode->childcount = GetChildCount(5, currentnode->nodevalues.at(5).toULongLong(), curid);
                            currentnode->haschildren = currentnode->HasChildren();
                        }
                        rootdirectory->children.append(currentnode);
                    }
                }
                filequery.finish();
            }
            endInsertRows();
            emit checkedNodesChanged();
        }
    };

    Node* NodeFromIndex(const QModelIndex &index) const
    {
        if(index.isValid())
            return static_cast<Node*>(index.internalPointer());
        else
            return rootnode;
    };
 
signals:
    void checkedNodesChanged();

private:
    Qt::CheckState GetCheckState(Node* curnode) const
    {
        if(curnode->checkstate == 0) // unchecked
            return Qt::Unchecked;
        else if(curnode->checkstate == 1) // partially checked
            return Qt::PartiallyChecked;
        else if(curnode->checkstate == 2) // checked
            return Qt::Checked;
        return Qt::Unchecked;
    };

    void SetParentCheckState(const QModelIndex &index)
    {
        Node* curnode = NodeFromIndex(index);
        unsigned long long checkcount = 0;
        for(int i=0; i < curnode->children.count(); i++)
        {
            if(curnode->children[i]->checkstate == 2 || curnode->children[i]->checkstate == 1)
                checkcount++;
        }
        if(curnode->childcount > checkcount && checkcount > 0)
        {
            curnode->checkstate = 1;
            checkhash[curnode->nodevalues.at(0).toULongLong()] = 1;
        }
        else if(curnode->childcount == checkcount)
        {
            curnode->checkstate = 1;
            checkhash[curnode->nodevalues.at(0).toULongLong()] = 1;
        }
        else if(checkcount == 0)
        {
            curnode->checkstate = 0;
            checkhash[curnode->nodevalues.at(0).toULongLong()] = 0;
        }
        emit dataChanged(index, index);
        emit checkedNodesChanged();
        if(curnode->parent != 0)
            SetParentCheckState(index.parent());
    };

    void SetChildCheckState(const QModelIndex &index)
    {
        Node* curnode = NodeFromIndex(index);
        for(int i=0; i < curnode->children.count(); i++)
        {
            curnode->children[i]->checkstate = curnode->checkstate;
            emit dataChanged(index.child(i, 0), index.child(i, 0));
            if(curnode->children[i]->haschildren)
                SetChildCheckState(index.child(i,0));
        }
        emit checkedNodesChanged();
    };

    bool SetCheckState(const QModelIndex &index, Qt::CheckState state)
    {
        Node* curnode = NodeFromIndex(index);
        if(state == Qt::Unchecked) // curnode is now unchecked...
        {
            curnode->checkstate = 0;
            if(curnode->haschildren)
                SetChildCheckState(index);
            checkhash[curnode->nodevalues.at(0).toULongLong()] = 0;
        }
        else if(state == Qt::PartiallyChecked) // curnode is now partially checked
        {
            curnode->checkstate = 1;
            checkhash[curnode->nodevalues.at(0).toULongLong()] = 1;
        }
        else if(state == Qt::Checked) // currentnode is now checked
        {
            curnode->checkstate = 2;
            if(curnode->haschildren)
                SetChildCheckState(index);
            checkhash[curnode->nodevalues.at(0).toULongLong()] = 2;
        }
        emit dataChanged(index, index);
        emit checkedNodesChanged();
        if(curnode->parent != 0)
            SetParentCheckState(index.parent());
        return true;
    };

    QStringList headerdata;
};

namespace Ui {
class WombatForensics;
}

class WombatForensics : public QMainWindow
{
    Q_OBJECT

public:
    explicit WombatForensics(QWidget *parent = 0);
    ~WombatForensics();
    WombatDatabase* wombatdatabase;
    WombatVariable wombatvariable;
    WombatVariable* wombatvarptr;
    TskObject tskobject;
    TskObject* tskobjptr;
    TskObject tskexternalobject;
    TskObject* tskexternalptr;
    WombatFramework* wombatframework;
    PropertiesWindow* propertywindow;
    ExportDialog* exportdialog;
    DigDeeperDialog* digdeeperdialog;
    FileViewer* fileviewer;
    TreeModel* treemodel;
    QMenu* treemenu;
    QMenu* selectionmenu;
    IdFilter* idfilterview;
    JumpFilter* jumpfilterview;
    NameFilter* namefilterview;
    PathFilter* pathfilterview;
    SizeFilter* sizefilterview;
    CreatedDateFilter* createfilterview;
    AccessedDateFilter* accessfilterview;
    ModifiedDateFilter* modifyfilterview;
    ChangedDateFilter* changefilterview;
    FileTypeFilter* filetypefilterview;
    FileCategoryFilter* filecategoryfilterview;
    HashFilter* hashfilterview;
    ImageViewer* imagewindow;
    VideoViewer* videowindow;
    ViewerManager* viewmanage;
    TextViewer* textviewer;
    HtmlViewer* htmlviewer;
    MessageViewer* msgviewer;
    ByteConverter* byteviewer;
    AboutBox* aboutbox;

signals:

private slots:
    void AddEvidence();
    void RemEvidence();
    void ExportEvidence();
    void on_actionNew_Case_triggered();
    void on_actionOpen_Case_triggered();
    void on_actionSaveState_triggered();
    void on_actionCheck_triggered();
    void on_actionExport_triggered();
    void on_actionDigDeeper_triggered();
    void on_actionView_Properties_triggered(bool checked);
    void on_actionView_File_triggered(bool checked);
    void on_actionView_Image_Gallery_triggered(bool checked);
    void on_actionViewerManager_triggered();
    void on_actionTextViewer_triggered(bool checked);
    void on_actionViewMessageLog_triggered(bool checked);
    void on_actionByteConverter_triggered(bool checked);
    void on_actionCopy_Selection_To_triggered();
    void on_actionExpandAll_triggered();
    void on_actionCollapseAll_triggered();
    void on_actionAbout_triggered();
    void UpdateProgress(unsigned long long count, unsigned long long processcount);
    void SelectionChanged(const QItemSelection &selitem, const QItemSelection &deselitem);
    void HidePropertyWindow(bool checkstate);
    void HideFileViewer(bool checkstate);
    void HideImageWindow(bool checkstate);
    void HideViewerManager(void);
    void HideTextViewer(bool checkstate);
    void HideMessageViewer(bool checkstate);
    void HideByteViewer(bool checkstate);
    void ShowExternalViewer();
    void DisplayError(QString errorNumber, QString errorType, QString errorValue);
    void ResizeColumns(void);
    void OpenParentImage(unsigned long long imgid);
    void OpenParentFileSystem(unsigned long long fsid);
    void OpenFileSystemFile(void);
    void ResizeViewColumns(const QModelIndex &index)
    {
        if(index.isValid())
            ResizeColumns();
    };
    void ExpandCollapseResize(const QModelIndex &index)
    {
       /* if(((TreeModel*)ui->dirTreeView->model())->canFetchMore(index))
        {
            ((TreeModel*)ui->dirTreeView->model())->fetchMore(index);
        }*/
        ResizeViewColumns(index);
    };
    void FileExport(FileExportData* exportdata);
    void FileDig(FileDeepData* deeperdata);
    void SetOffsetLabel(off_t pos);
    void ResetSlider(void);
    void ShowRockerToolTip(int moved);
    void SkipDown(void);
    void SkipUp(void);
    void PageUp(void);
    void PageDown(void);
    void UpdateSelectValue(const QString &txt);
    void InitializeQueryModel(void);
    void UpdateDataTable(void);
    void UpdateStatus(void);
    void UpdateDigging(void);
    void FinishExport(void);
    void FinishRemoval(void);
    void FinishThumbs(void);
    void StatusUpdate(QString tmptext)
    {
        statuslabel->setText(tmptext);
    };
    void TreeContextMenu(const QPoint &point);
    void ImgHexMenu(const QPoint &point);
    void SetFilter(int headercolumn);
    void FilterApplied()
    {
        UpdateFilterCount();
        emit ui->dirTreeView->header()->geometriesChanged();
    };
    void NextItem();
    void PreviousItem();
    void ShowItem();
    void UpdateThumbnails(int tsize);
    void SetSelectedFromImageViewer(unsigned long long selectedid);
    void ShowFile(const QModelIndex &index);
    void AddSection(void);
    void AddTextSection(void);
    void CarveFile(void);
    void AutoSaveState(void);

protected:
    void closeEvent(QCloseEvent* event);
    void mouseDoubleClickEvent(QMouseEvent* event);
private:
    Ui::WombatForensics *ui;

    void SetupHexPage(void);
    void InitializeAppStructure(void);
    void InitializeCaseStructure(void);
    void InitializeEvidenceStructure(void);
    void InitializeOpenCase(void);
    void CloseCurrentCase(void);
    void UpdateProperties(void);
    void LoadHexContents(void);
    void SecondaryProcessing(void);
    void OpenEvidenceStructure(void);
    void StartThumbnails(void);
    void ExportFiles(FileExportData* exportdata);
    void DigFiles(FileDeepData* deepdata);
    void GetExportData(Node* curnode, FileExportData* exportdata);
    void GetDigData(Node* curnode, FileDeepData* deepdata);
    void ProcessDig(TskObject curobject, unsigned long long objectid, std::vector<FileDeepData::DigOptions> digoptions);
    void ProcessExport(TskObject curobject, std::string fullpath, std::string name);
    void UpdateFilterCount(void);
    void SaveState(void);

    void RemoveTmpFiles(void);

    QModelIndex selectedindex;
    QModelIndex oldselectedindex;

    QFuture<void> sqlfuture;
    QFutureWatcher<void> sqlwatcher;
    QFuture<void> secondfuture;
    QFutureWatcher<void> secondwatcher;
    QFuture<void> thumbfuture;
    QFutureWatcher<void> thumbwatcher;
    QFuture<void> exportfuture;
    QFutureWatcher<void> exportwatcher;
    QFutureWatcher<void> digwatcher;
    QFuture<void> remfuture;
    QFutureWatcher<void> remwatcher;

    off_t offset() const;
    ImageHexViewer* hexwidget;
    WombatSlider* hexrocker;
    QPushButton* lineup;
    QPushButton* linedown;
    QPushButton* pageup;
    QPushButton* pagedown;
    QLabel* selectedoffset;
    QLabel* selectedhex;
    QLabel* filecountlabel;
    QLabel* filtercountlabel;
    QLabel* processcountlabel;
    QLabel* statuslabel;
    QFrame* vline1;
    QFrame* vline2;
    QVector<FileExportData> exportfilelist;
    QVector<FileDeepData> digfilelist;
    QShortcut* jumpforward;
    QShortcut* jumpbackward;
    QShortcut* showitem;
    QTimer* autosavetimer;
};

#endif // WOMBATFORENSICS_H
