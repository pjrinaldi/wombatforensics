#ifndef WOMBATFORENSICS_H
#define WOMBATFORENSICS_H

// Copyright 2015 Pasquale J. Rinaldi, Jr.
// Distrubted under the terms of the GNU General Public License version 2

#include "wombatinclude.h"
#include "wombatvariable.h"
//#include "wombatdatabase.h"
//#include "wombatframework.h"
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
#include "cancelthread.h"

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

class TreeModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    TreeModel(QObject* parent = 0) : QAbstractItemModel(parent)
    {
        headerdata << "ID" << "Name" << "Full Path" << "Size (bytes)" << "Created (UTC)" << "Accessed (UTC)" << "Modified (UTC)" << "Status Changed (UTC)" << "MD5 Hash" << "File Signature" << "File Category";
        //headerdata << "ID" << "Name" << "Full Path" << "Size (bytes)" << "Object Type" << "Address" << "Created (UTC)" << "Accessed (UTC)" << "Modified (UTC)" << "Status Changed (UTC)" << "MD5 Hash" << "Parent ID" << "Item Type" << "Parent Image ID" << "Parent FS ID" << "Flags" << "File Signature" << "File Category" << "Checked" << "MFT Attribute ID";
        rootnode = 0;
        QList<QVariant> emptyset;
        emptyset << "" << "" << "" << "" << "" << "" << "" << "" << "" << "" << "";
        //emptyset << "" << "" << "" << "" << "" << "" << "" << "" << "" << "" << "" << "" << "" << "" << "" << "" << "" << "" << "" << "";
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
        int nodetype = 0;
        int itemtype = 0;
        /*
        QSqlQuery dataquery(fcasedb);
        dataquery.prepare("SELECT objtype, type FROM data WHERE id = ?");
        dataquery.bindValue(0, node->nodevalues.at(0).toULongLong());
        dataquery.exec();
        dataquery.next();
        int nodetype = dataquery.value(0).toInt();
        int itemtype = dataquery.value(1).toInt();
        dataquery.finish();
        */
        //headerdata << "ID" << "Name" << "Full Path" << "Size (bytes)" << "Object Type" << "Address" << "Created (UTC)" << "Accessed (UTC)" << "Modified (UTC)" << "Status Changed (UTC)" << "MD5 Hash" << "Parent ID" << "Item Type" << "Parent Image ID" << "Parent FS ID" << "Flags" << "File Signature" << "File Category" << "Checked" << "MFT Attribute ID";
        // WILL NEED TO REPLACE ALL THESE CALLS TO THE RESPECTIVE SQL QUERY RATHER THAN THE NODE SINCE I'M NOT STORING IT IN THE NODE ANYMORE...
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
            //if(node->nodevalues.at(4).toInt() == 5)
            if(nodetype == 5)
            {
                if(filtervalues.maxcreatebool && filtervalues.mincreatebool == false)
                {
                    //if(node->nodevalues.at(6).toInt() <= filtervalues.maxcreate)
                    if(node->nodevalues.at(4).toInt() <= filtervalues.maxcreate)
                        return QColor(Qt::lightGray);
                }
                if(filtervalues.maxcreatebool == false && filtervalues.mincreatebool)
                {
                    if(node->nodevalues.at(4).toInt() >= filtervalues.mincreate)
                        return QColor(Qt::lightGray);
                }
                if(filtervalues.maxcreatebool && filtervalues.mincreatebool)
                {
                    if(node->nodevalues.at(4).toInt() >= filtervalues.mincreate || node->nodevalues.at(4).toInt() <= filtervalues.maxcreate)
                        return QColor(Qt::lightGray);
                }
                if(filtervalues.maxaccessbool && filtervalues.minaccessbool == false)
                {
                    //if(node->nodevalues.at(7).toInt() <= filtervalues.maxaccess)
                    if(node->nodevalues.at(5).toInt() <= filtervalues.maxaccess)
                        return QColor(Qt::lightGray);
                }
                if(filtervalues.maxaccessbool == false && filtervalues.minaccessbool)
                {
                    if(node->nodevalues.at(5).toInt() >= filtervalues.minaccess)
                        return QColor(Qt::lightGray);
                }
                if(filtervalues.maxaccessbool && filtervalues.minaccessbool)
                {
                    if(node->nodevalues.at(5).toInt() >= filtervalues.minaccess || node->nodevalues.at(5).toInt() <= filtervalues.maxaccess)
                        return QColor(Qt::lightGray);
                }
                if(filtervalues.maxmodifybool && filtervalues.minmodifybool == false)
                {
                    //if(node->nodevalues.at(8).toInt() <= filtervalues.maxmodify)
                    if(node->nodevalues.at(6).toInt() <= filtervalues.maxmodify)
                        return QColor(Qt::lightGray);
                }
                if(filtervalues.maxmodifybool == false && filtervalues.minmodifybool)
                {
                    if(node->nodevalues.at(6).toInt() >= filtervalues.minmodify)
                        return QColor(Qt::lightGray);
                }
                if(filtervalues.maxmodifybool && filtervalues.minmodifybool)
                {
                    if(node->nodevalues.at(6).toInt() >= filtervalues.minmodify || node->nodevalues.at(6).toInt() <= filtervalues.maxmodify)
                        return QColor(Qt::lightGray);
                }
                if(filtervalues.maxchangebool && filtervalues.minchangebool == false)
                {
                    //if(node->nodevalues.at(9).toInt() <= filtervalues.maxchange)
                    if(node->nodevalues.at(7).toInt() <= filtervalues.maxchange)
                        return QColor(Qt::lightGray);
                }
                if(filtervalues.maxchangebool == false && filtervalues.minchangebool)
                {
                    if(node->nodevalues.at(7).toInt() >= filtervalues.minchange)
                        return QColor(Qt::lightGray);
                }
                if(filtervalues.maxchangebool && filtervalues.minchangebool)
                {
                    if(node->nodevalues.at(7).toInt() >= filtervalues.minchange || node->nodevalues.at(7).toInt() <= filtervalues.maxchange)
                        return QColor(Qt::lightGray);
                }
                if(filtervalues.filecategorybool && filtervalues.filetypebool == false)
                {
                    //if(node->nodevalues.at(16).toString().contains(filtervalues.filecategory) == false)
                    if(node->nodevalues.at(9).toString().contains(filtervalues.filecategory) == false)
                        return QColor(Qt::lightGray);
                }
                if(filtervalues.filecategorybool == false && filtervalues.filetypebool)
                {
                    if(node->nodevalues.at(9).toString().contains(filtervalues.filetype) == false)
                        return QColor(Qt::lightGray);
                }
                if(filtervalues.filecategorybool && filtervalues.filetypebool)
                {
                    if(node->nodevalues.at(9).toString().contains(filtervalues.filecategory) == false || node->nodevalues.at(9).toString().contains(filtervalues.filetype) == false)
                        return QColor(Qt::lightGray);
                }
                if(filtervalues.filegroupbool)
                {
                    //if(node->nodevalues.at(17).toString().contains(filtervalues.filegroup) == false)
                    if(node->nodevalues.at(10).toString().contains(filtervalues.filegroup) == false)
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
            //if(index.column() == 2)
            //{
                //if(node->nodevalues.at(4).toInt() == 1)
                //if(nodetype == 1)
                //    return QString("");
                //else
                //    return node->nodevalues.at(index.column());
            //}
            if(index.column() >= 4 && index.column() <= 7)
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
            //int nodetype = node->nodevalues.at(4).toInt();
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
                    //int itemtype = node->nodevalues.at(12).toInt();
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
            /*
            if(value.toInt() == -15)
            {
                Node* parentnode = NodeFromIndex(index);
                QSqlQuery childupdatequery(fcasedb);
                childupdatequery.prepare("SELECT id, name, fullpath, size, crtime, atime, mtime, ctime, md5, filemime, FROM data WHERE id = ?");
                //childupdatequery.prepare("SELECT objectid, name, fullpath, size, objecttype, address, crtime, atime, mtime, ctime, md5, parentid, type, parimgid, parfsid, flags, filemime, filesignature, checked, mftattrid FROM data WHERE objectid = ?;");
                for(int i=0; i < parentnode->children.count(); i++)
                {
                    childupdatequery.addBindValue(parentnode->children.at(i)->nodevalues.at(0).toULongLong());
                    childupdatequery.exec();
                    childupdatequery.next();
                    parentnode->children.at(i)->nodevalues[0] = childupdatequery.value(0);
                    parentnode->children.at(i)->nodevalues[1] = childupdatequery.value(1);
                    parentnode->children.at(i)->nodevalues[2] = childupdatequery.value(2);
                    parentnode->children.at(i)->nodevalues[3] = childupdatequery.value(3);
                    parentnode->children.at(i)->nodevalues[4] = childupdatequery.value(4);
                    parentnode->children.at(i)->nodevalues[5] = childupdatequery.value(5);
                    parentnode->children.at(i)->nodevalues[6] = childupdatequery.value(6);
                    parentnode->children.at(i)->nodevalues[7] = childupdatequery.value(7);
                    parentnode->children.at(i)->nodevalues[8] = childupdatequery.value(8);
                    parentnode->children.at(i)->nodevalues[9] = childupdatequery.value(9);
                    parentnode->children.at(i)->nodevalues[10] = QVariant(childupdatequery.value(9).toString().split("/").at(0));
                }
                childupdatequery.finish();
                emit dataChanged(index.child(0, 0), index.child(parentnode->children.count() - 1, 0));
                return true;
            }*/
            /*else
            {*/
                Node* currentnode = NodeFromIndex(index);
                /*
                QSqlQuery updatequery(fcasedb);
                updatequery.prepare("SELECT id, name, fullpath, size, crtime, atime, mtime, ctime, md5, filemime, FROM data WHERE id = ?;");
                //updatequery.prepare("SELECT objectid, name, fullpath, size, objecttype, address, crtime, atime, mtime, ctime, md5, parentid, type, parimgid, parfsid, flags, filemime, filesignature, checked, mftattrid FROM data WHERE objectid = ?;");
                updatequery.addBindValue(currentnode->nodevalues.at(0).toULongLong());
                updatequery.exec();
                updatequery.first();
                currentnode->nodevalues[0] = updatequery.value(0);
                currentnode->nodevalues[1] = updatequery.value(1);
                currentnode->nodevalues[2] = updatequery.value(2);
                currentnode->nodevalues[3] = updatequery.value(3);
                currentnode->nodevalues[4] = updatequery.value(4);
                currentnode->nodevalues[5] = updatequery.value(5);
                currentnode->nodevalues[6] = updatequery.value(6);
                currentnode->nodevalues[7] = updatequery.value(7);
                currentnode->nodevalues[8] = updatequery.value(8);
                currentnode->nodevalues[9] = updatequery.value(9);
                currentnode->nodevalues[10] = QVariant(updatequery.value(9).toString().split("/").at(0));
                updatequery.finish();
                */
                //emit dataChanged(index, index);
                return true;
            //}
        }
        return false;
    };

    void dataChanged(const QModelIndex &topleftindex, const QModelIndex &botrightindex, const QVector<int> &roles = QVector<int>())
    {
        /*
        foreach(int role, roles)
        {
        if(role == Qt::DisplayRole)
        {
            Node* startnode = NodeFromIndex(topleftindex);
            Node* endnode = NodeFromIndex(botrightindex);
            QSqlQuery updatequery(fcasedb);
            updatequery.prepare("SELECT id, name, fullpath, size, crtime, atime, mtime, ctime, md5, filemime, FROM data WHERE id = ? OR id = ?;");
            //updatequery.prepare("SELECT objectid, name, fullpath, size, objecttype, address, crtime, atime, mtime, ctime, md5, parentid, type, parimgid, parfsid, flags, filemime, filesignature, checked, mftattrid FROM data WHERE objectid == ? OR objectid == ?;");
            updatequery.addBindValue(startnode->nodevalues.at(0).toULongLong());
            updatequery.exec();
            updatequery.next();
            startnode->nodevalues[8] = updatequery.value(8).toString();
            updatequery.addBindValue(endnode->nodevalues.at(0).toULongLong());
            updatequery.exec();
            updatequery.next();
            endnode->nodevalues[8] = updatequery.value(8).toString();
            updatequery.finish();
        }
        }*/
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
            if(section == 4 && (filtervalues.maxcreatebool || filtervalues.mincreatebool))
                return QIcon(QPixmap(QString(":/basic/filterimg")));
            if(section == 5 && (filtervalues.maxaccessbool || filtervalues.minaccessbool))
                return QIcon(QPixmap(QString(":/basic/filterimg")));
            if(section == 6 && (filtervalues.maxmodifybool || filtervalues.minmodifybool))
                return QIcon(QPixmap(QString(":/basic/filterimg")));
            if(section == 7 && (filtervalues.maxchangebool || filtervalues.minchangebool))
                return QIcon(QPixmap(QString(":/basic/filterimg")));
            if(section == 9 && (filtervalues.filecategorybool || filtervalues.filetypebool))
                return QIcon(QPixmap(QString(":/basic/filterimg")));
            if(section == 8 && filtervalues.hashbool)
                return QIcon(QPixmap(QString(":/basic/filterimg")));
            if(section == 10 && filtervalues.filegroupbool)
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
        qDebug() << parent.sibling(parent.row(), 0).data().toString(); // unique id

//wombatvariable.selectedobject.id = selectedindex.sibling(selectedindex.row(), 0).data().toULongLong(); // object id
        //Node* parentnode = NodeFromIndex(parent);
        //QList<QVariant> fetchvalues;
        //fetchvalues.clear();
        if(parentnode->haschildren == true)
        {
            // NEED TO KNOW THE CURRENT PARTITION NUMBER AND THE ADDR NUMBER TO OPEN THE CURRENT PARENT FILE
            //QFile parentfile(wombatvariable.tmpmntpath + wombatvariable.evidenceobject.name + ".p" + 
            //IF I USE THE EVID-VOL-PART-INODE NUMBERS AS THE UNIQUEID, THEN I CAN KEEP IT REAL AND I CAN SPLIT IT OUT AND DETERMINE WHAT I NEED TO FIGURE OUT WHAT TO OPEN... I MAY WANT TO ADD A UNIQUE EVIDENCE ID SO I CAN GET THE IMAGE FILE NAME, OR I NEED TO GET THE ROOTINDEX SO I CAN GET ITS FILENAME OR SET A GLOBAL VARIABLE TO HOLD THAT INFORMATION.
            /*
            QSqlQuery prequery(fcasedb);
            prequery.prepare("SELECT addr, parimgid, parfsid FROM data WHERE id = ?");
            prequery.bindValue(0, parentnode->nodevalues.at(0).toULongLong());
            //qDebug() << "curid:" << parentnode->nodevalues.at(0).toULongLong();
            prequery.exec();
            prequery.first();
            unsigned long long parentaddress = prequery.value(0).toULongLong();
            unsigned long long parentimgid = prequery.value(1).toULongLong();
            unsigned long long parentfsid = prequery.value(2).toULongLong();
            if(parentfsid == 0)
                parentfsid = parentnode->nodevalues.at(0).toULongLong();
            prequery.finish();

            QSqlQuery fetchquery(fcasedb);
            fetchquery.prepare("SELECT id, name, fullpath, size, crtime, atime, mtime, ctime, md5, filemime, checked, addr, parimgid, parfsid FROM data WHERE (objtype = 5 OR objtype = 6) AND parid = ? AND parimgid = ? AND parfsid = ?;");
            //qDebug() << "parid:" << parentaddress << "parimgid:" << parentimgid << "parfsid:" << parentfsid;
            fetchquery.bindValue(0, parentaddress);
            fetchquery.bindValue(1, parentimgid);
            fetchquery.bindValue(2, parentfsid);
            if(fetchquery.exec())
            {
                beginInsertRows(parent, 0, parentnode->childcount - 1);
                while(fetchquery.next())
                {
                    fetchvalues.clear();
                    fetchvalues.append(fetchquery.value(0));
                    fetchvalues.append(fetchquery.value(1));
                    fetchvalues.append(fetchquery.value(2));
                    fetchvalues.append(fetchquery.value(3));
                    fetchvalues.append(fetchquery.value(4));
                    fetchvalues.append(fetchquery.value(5));
                    fetchvalues.append(fetchquery.value(6));
                    fetchvalues.append(fetchquery.value(7));
                    fetchvalues.append(fetchquery.value(8));
                    fetchvalues.append(fetchquery.value(9));
                    fetchvalues.append(QVariant(fetchquery.value(9).toString().split("/").at(0)));
                    Node* curchild = new Node(fetchvalues);
                    curchild->parent = parentnode;
                    if(QString(".").compare(curchild->nodevalues.at(1).toString()) == 0 || QString("..").compare(curchild->nodevalues.at(1).toString()) == 0)
                    {
                        curchild->childcount = 0;
                        curchild->haschildren = false;
                    }
                    else
                    {
                        curchild->childcount = GetChildCount(5, fetchquery.value(11).toULongLong(), fetchquery.value(12).toULongLong(), fetchquery.value(13).toULongLong());
                        curchild->haschildren = curchild->HasChildren();
                    }
                    if(fetchquery.value(10).toInt() == 0)
                        curchild->checkstate = 0;
                    else if(fetchquery.value(10).toInt() == 1)
                        curchild->checkstate = 1;
                    else
                        curchild->checkstate = 2;
                    parentnode->children.append(curchild);
                }
                endInsertRows();
                emit checkedNodesChanged();
                //setData(parent, QVariant(-15), Qt::DisplayRole);
            }
            fetchquery.finish();
            */


            /*
            QSqlQuery morequery(fcasedb);
            morequery.prepare("SELECT objectid, name, fullpath, size, objecttype, address, crtime, atime, mtime, ctime, md5, parentid, type, parimgid, parfsid, flags, filemime, filesignature, checked, mftattrid FROM data WHERE (objecttype = 5 OR objecttype = 6) AND parentid = ? AND parimgid = ? AND parfsid = ?");
            //morequery.prepare("SELECT objectid, name, fullpath, size, objecttype, address, crtime, atime, mtime, ctime, md5, parentid, type, parimgid, parfsid, flags, filemime, filesignature, checked, mftattrid FROM data WHERE (objecttype = 5 OR objecttype = 6) AND parentid = ? AND parimgid = ? AND parfsid = ?");
            morequery.addBindValue(parentnode->nodevalues.at(5).toULongLong());
            morequery.addBindValue(parentnode->nodevalues.at(13).toULongLong());
            if(parentnode->nodevalues.at(4).toInt() == 4)
                morequery.addBindValue(parentnode->nodevalues.at(0).toULongLong());
            else
                morequery.addBindValue(parentnode->nodevalues.at(14).toULongLong());
            //morequery.addBindValue(parentnode->nodevalues.at(0).toULongLong());
            if(morequery.exec())
            {
                beginInsertRows(parent, 0, parentnode->childcount - 1);
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
                        curchild->childcount = GetChildCount(5, curchild->nodevalues.at(5).toULongLong(), curchild->nodevalues.at(13).toULongLong(), curchild->nodevalues.at(14).toULongLong());
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
                endInsertRows();
                emit checkedNodesChanged();
                setData(parent, QVariant(-15), Qt::DisplayRole);

            }
            */
        }
    };
    
    void GetModelCount(Node* curnode)
    {
        //if(curnode->nodevalues.at(4).toInt() == 5 || curnode->nodevalues.at(4).toInt() == 6)
        //{
            totalcount++;
            if(curnode->checkstate == 2)
                totalchecked++;
        //}
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

    //void AddEvidence(unsigned long long curid)
    void AddEvidence()
    {
        QStringList tmplist;
        QString tmpstr = "";
        tmplist.clear();
        // NEED TO ADD THE EVID, VOLUME, PARTITIONS, THEN THE FILES...
        // APPEND EVIDENCE TO NODE TREE
        QFile evidfile(wombatvariable.tmpmntpath + wombatvariable.evidenceobject.name + ".evid");
        evidfile.open(QIODevice::ReadOnly);
        tmpstr = evidfile.readLine();
        tmplist = tmpstr.split(",");
        beginInsertRows(QModelIndex(), rootnode->childcount, rootnode->childcount);
        currentnode = 0;
        colvalues.clear();
        //colvalues.append(wombatid);                             // ID
        colvalues.append(tmplist.at(5));                        // ID
        colvalues.append(wombatvariable.evidenceobject.name);   // Name
        colvalues.append(tmplist.at(3));                        // Full Path
        colvalues.append(tmplist.at(1));                        // Size
        colvalues.append(0);                                    // Created
        colvalues.append(0);                                    // Accessed
        colvalues.append(0);                                    // Modified
        colvalues.append(0);                                    // Status Changed
        colvalues.append("");                                   // MD5
        colvalues.append("");                                   // File Signature
        colvalues.append("");                                   // File Category
        currentnode = new Node(colvalues);
        rootnode->children.append(currentnode);
        rootnode->childcount++;
        rootnode->haschildren = rootnode->HasChildren();
        currentnode->parent = rootnode;
        currentnode->childcount = GetChildCount(wombatvariable.evidenceobject.name + ".vol");
        currentnode->haschildren = currentnode->HasChildren();
        parentnode = currentnode;
        evidfile.close();
        wombatid++;
        // APPEND VOLUME TO NODE TREE
        tmpstr = "";
        currentnode = 0;
        tmplist.clear();
        colvalues.clear();
        QFile volfile(wombatvariable.tmpmntpath + wombatvariable.evidenceobject.name + ".vol");
        volfile.open(QIODevice::ReadOnly);
        tmpstr = volfile.readLine();
        volfile.close();
        tmplist = tmpstr.split(",");
        //colvalues.append(wombatid);                             // ID
        colvalues.append(tmplist.at(5));                        // ID
        colvalues.append(tmplist.at(2));                        // Name
        colvalues.append("");                                   // Full Path
        colvalues.append(tmplist.at(1));                        // Size
        colvalues.append(0);                                    // Created
        colvalues.append(0);                                    // Accessed
        colvalues.append(0);                                    // Modified
        colvalues.append(0);                                    // Status Changed
        colvalues.append("");                                   // MD5
        colvalues.append("");                                   // File Signature
        colvalues.append("");                                   // File Category
        currentnode = new Node(colvalues);
        currentnode->parent = parentnode;
        parentnode->children.append(currentnode);
        currentnode->childcount = GetChildCount(wombatvariable.evidenceobject.name + ".p?");
        currentnode->haschildren = currentnode->HasChildren();
        parentnode = currentnode;
        wombatid++;
        // APPEND PARTITION(S) TO THE VOLUME
        int partcount = currentnode->childcount;
        QFile partfile;
        for(int i = 0; i < partcount; i++)
        {
            tmpstr = "";
            currentnode = 0;
            tmplist.clear();
            colvalues.clear();
            partfile.setFileName(wombatvariable.tmpmntpath + wombatvariable.evidenceobject.name + ".p" + QString::number(i));
            partfile.open(QIODevice::ReadOnly);
            tmpstr = partfile.readLine();
            partfile.close();
            tmplist = tmpstr.split(",");
            rootinum = tmplist.at(3);
            //colvalues.append(wombatid);                             // ID
            colvalues.append(tmplist.at(10));                       // ID
            colvalues.append(tmplist.at(2));                        // Name
            colvalues.append("");                                   // Full Path
            colvalues.append(tmplist.at(1));                        // Size
            colvalues.append(0);                                    // Created
            colvalues.append(0);                                    // Accessed
            colvalues.append(0);                                    // Modified
            colvalues.append(0);                                    // Status Changed
            colvalues.append("");                                   // MD5
            colvalues.append("");                                   // File Signature
            colvalues.append("");                                   // File Category
            currentnode = new Node(colvalues);
            currentnode->parent = parentnode;
            parentnode->children.append(currentnode);
            currentnode->childcount = GetChildCount(wombatvariable.evidenceobject.name + ".p" + QString::number(i) + "*.a" + rootinum);
            //qDebug() << currentnode->childcount;
            currentnode->haschildren = currentnode->HasChildren();
            parentnode = currentnode;
            wombatid++;
            QFile filefile;
            QStringList curfiles = GetChildFiles(wombatvariable.evidenceobject.name + ".p" + QString::number(i) + "*.a" + rootinum);
            for(int j = 0; j < curfiles.count(); j++)
            {
                tmpstr = "";
                currentnode = 0;
                tmplist.clear();
                colvalues.clear();
                filefile.setFileName(wombatvariable.tmpmntpath + curfiles.at(j));
                filefile.open(QIODevice::ReadOnly);
                tmpstr = filefile.readLine();
                filefile.close();
                tmplist = tmpstr.split(",");
                //colvalues.append(wombatid);                             // ID
                colvalues.append(tmplist.at(12));                       // ID
                colvalues.append(tmplist.at(0));                        // Name
                colvalues.append(tmplist.at(3));                        // Full Path
                colvalues.append(tmplist.at(8));                        // Size
                colvalues.append(tmplist.at(4));                        // Created
                colvalues.append(tmplist.at(5));                        // Accessed
                colvalues.append(tmplist.at(6));                        // Modified
                colvalues.append(tmplist.at(7));                        // Status Changed
                colvalues.append("");                                   // MD5
                colvalues.append(tmplist.at(10));                       // File Signature
                colvalues.append(tmplist.at(10).split("/").at(0));      // File Category
                currentnode = new Node(colvalues);
                currentnode->parent = parentnode;
                parentnode->children.append(currentnode);
                currentnode->childcount = GetChildCount(wombatvariable.evidenceobject.name + ".p" + QString::number(i) + "*.a" + tmplist.at(9));
                currentnode->haschildren = currentnode->HasChildren();
                wombatid++;
            }
        }
        endInsertRows();
        // STILL NEED TO ACCOUNT FOR THE FILE CHECKS AND UPDATE ACCORDINGLY

        //emit checkedNodesChanged();
        /*
        if(addevidquery.value(16).toInt() == 0)
            currentnode->checkstate = 0;
        else if(addevidquery.value(16).toInt() == 1)
            currentnode->checkstate = 1;
        else
            currentnode->checkstate = 2;
        */
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
        //emit dataChanged(index, index);
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
            //emit dataChanged(index.child(i, 0), index.child(i, 0));
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
        //emit dataChanged(index, index);
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
    //WombatDatabase* wombatdatabase;
    //WombatVariable* wombatvarptr;
    TskObject tskobject;
    TskObject* tskobjptr;
    TskObject tskexternalobject;
    TskObject* tskexternalptr;
    //WombatFramework* wombatframework;
    //PropertiesWindow* propertywindow;
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
    CancelThread* cancelthread;

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
        if(((TreeModel*)ui->dirTreeView->model())->canFetchMore(index))
        {
            ((TreeModel*)ui->dirTreeView->model())->fetchMore(index);
        }
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
    //void InitializeEvidenceStructure(QVector<WombatVariable> wombatvarvector);
    //void InitializeEvidenceStructure(void);
    void InitializeOpenCase(void);
    void CloseCurrentCase(void);
    void UpdateProperties(void);
    void LoadHexContents(void);
    //void SecondaryProcessing(void);
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
    void CreateAppDB(void);
    void OpenAppDB(void);
    unsigned long long ReturnCaseCount(void);
    void InsertCase(void);
    void CreateThumbDB(void);
    void OpenThumbDB(void);
    void CreateCaseDB(void);
    void OpenCaseDB(void);
    void AddNewEvidence(void);
    //QString GetFileSystemLabel(TSK_FS_INFO* fsinfo);
    void RemoveTmpFiles(void);
    //void GetEvidenceObjects(void);
/*
    uint8_t hfs_cat_file_lookup(HFS_INFO* hfs, TSK_INUM_T inum, HFS_ENTRY* entry, unsigned char follow_hard_link);
    uint8_t hfs_UTF16toUTF8(TSK_FS_INFO* fs, uint8_t* uni, int ulen, char* asc, int alen, uint32_t flags);
    static uint8_t hfs_cat_get_record_offset_cb(HFS_INFO* hfs, int8_t level_type, const void* targ_data, const hfs_btree_key_cat* cur_key, TSK_OFF_T key_off, void* ptr);
    static TSK_OFF_T hfs_cat_get_record_offset(HFS_INFO* hfs, const hfs_btree_key_cat* needle);
    uint8_t hfs_cat_read_thread_record(HFS_INFO* hfs, TSK_OFF_T off, hfs_thread* thread);
    uint8_t hfs_cat_read_file_folder_record(HFS_INFO* hfs, TSK_OFF_T off, hfs_file_folder* record);
    static int hfs_cat_compare_keys(HFS_INFO* hfs, const hfs_btree_key_cat* key1, const hfs_btree_key_cat* key2);
    static uint8_t hfs_cat_traverse(HFS_INFO* hfs, const void* targ_data, TSK_HFS_BTREE_CB a_cb, void* ptr);
*/
    QModelIndex selectedindex;
    QModelIndex oldselectedindex;

    QFuture<void> sqlfuture;
    QFutureWatcher<void> sqlwatcher;
    //QFuture<void> secondfuture;
    //QFutureWatcher<void> secondwatcher;
    QFuture<void> thumbfuture;
    QFutureWatcher<void> thumbwatcher;
    QFuture<void> exportfuture;
    QFutureWatcher<void> exportwatcher;
    QFutureWatcher<void> digwatcher;
    QFuture<void> remfuture;
    QFutureWatcher<void> remwatcher;

    QFile casesfile;
    QFile settingsfile;
    QFile viewerfile;
    QFile casedatafile;
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
    //QVector<QVariantMap> jsonstorevector;
    //QVector<SecondaryProcessObject> secondprocessvector;
    QShortcut* jumpforward;
    QShortcut* jumpbackward;
    QShortcut* showitem;
    QTimer* autosavetimer;
    //unsigned long long currentcaseid;
    /*
    WombatVariable wombatvariable; // possibly need to make this global...
    TSK_IMG_INFO* readimginfo;
    TSK_VS_INFO* readvsinfo;
    const TSK_VS_PART_INFO* readpartinfo;
    TSK_FS_INFO* readfsinfo;
    TSK_FS_FILE* readfileinfo;
    char asc[512];
    iso9660_pvd_node* p;
    HFS_INFO* hfs;
    */
};

//void SecondaryProcessing(QVariantMap &jsonstore);
//void SecondaryProcessing(SecondaryProcessObject &secprocobj);

#endif // WOMBATFORENSICS_H
