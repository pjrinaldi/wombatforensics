#ifndef WOMBATFORENSICS_H
#define WOMBATFORENSICS_H

#include "wombatinclude.h"
#include "wombatvariable.h"
#include "wombatdatabase.h"
#include "wombatframework.h"
#include "wombatfunctions.h"
#include "ui_wombatforensics.h"
#include "progresswindow.h"
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


class TreeModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    TreeModel(QObject* parent = 0) : QAbstractItemModel(parent)
    {
        headerdata << "ID" << "Name" << "Full Path" << "Size (bytes)" << "Object Type" << "Address" << "Created (UTC)" << "Accessed (UTC)" << "Modified (UTC)" << "Status Changed (UTC)" << "MD5 Hash" << "Parent ID" << "Item Type" << "Parent Image ID" << "Parent FS ID" << "Flags" << "File Signature" << "File Category";
        rootnode = 0;
        QList<QVariant> emptyset;
        emptyset << "" << "" << "" << "" << "" << "" << "" << "" << "" << "" << "" << "" << "" << "" << "" << "" << "" << "";
        rootnode = new Node(emptyset);
        rootnode->parent = 0;
        rootnode->childcount = 0;
    };

    ~TreeModel()
    {
        delete rootnode;
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
                if(node->nodevalues.at(0).toInt() <= filtervalues.maxid)
                    return QColor(Qt::lightGray);
            }
            if(filtervalues.minidbool && filtervalues.maxidbool == false)
            {
                if(node->nodevalues.at(0).toInt() >= filtervalues.minid)
                    return QColor(Qt::lightGray);
            }
            if(filtervalues.maxidbool && filtervalues.minidbool)
            {
                if(node->nodevalues.at(0).toInt() >= filtervalues.minid || node->nodevalues.at(0).toInt() <= filtervalues.maxid)
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
                if(node->nodevalues.at(3).toInt() <= filtervalues.maxsize)
                    return QColor(Qt::lightGray);
            }
            if(filtervalues.minsizebool && filtervalues.maxsizebool == false)
            {
                if(node->nodevalues.at(3).toInt() >= filtervalues.minsize)
                    return QColor(Qt::lightGray);
            }
            if(filtervalues.maxsizebool && filtervalues.minsizebool)
            {
                if(node->nodevalues.at(3).toInt() >= filtervalues.minsize || node->nodevalues.at(3).toInt() <= filtervalues.maxsize)
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
                if(filtervalues.hashbool)
                {
                    for(int i=0; i < filtervalues.hashidlist.count(); i++)
                    {
                        if(node->nodevalues.at(0).toInt() == filtervalues.hashidlist.at(i))
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
        return false;
    };

    /*
    bool setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role)
    {
        emit headerDataChanged(orientation, section, section);
        return true;
    };
    */

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
                //emit headerDataChanged(orientation, section, section);
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
        if(parentnode->children.count() < parentnode->childcount && parentnode->haschildren == true)
            return true;
        return false;
    };

    void fetchMore(const QModelIndex &parent = QModelIndex())
    {
        Node* parentnode = NodeFromIndex(parent);
        QList<QVariant> fetchvalues;
        fetchvalues.clear();
        if(parentnode->haschildren == true)
        {
            QSqlQuery morequery(fcasedb);
            morequery.prepare("SELECT objectid, name, fullpath, size, objecttype, address, crtime, atime, mtime, ctime, md5, parentid, type, parimgid, parfsid, flags, filemime, filesignature FROM data WHERE objecttype == 5 AND parentid = ? AND parimgid = ?");
            morequery.addBindValue(parentnode->nodevalues.at(5).toInt());
            morequery.addBindValue(parentnode->nodevalues.at(13).toInt());
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
                        curchild->childcount = GetChildCount(5, curchild->nodevalues.at(5).toInt(), parentnode->nodevalues.at(13).toInt());
                        curchild->haschildren = curchild->HasChildren();
                    }
                    parentnode->children.append(curchild);
                }
                endInsertRows();
            }
        }
    };
/*
    void sort(int col, Qt::SortOrder order = Qt::AscendingOrder)
    {
        // i won't be using sorting, so i can probably delete this.
        if(col == 3) // file size sort
        {
            qDebug() << "sorting would work now if i could figure it out.";
        }
    };*/
    
    void GetModelCount(Node* curnode)
    {
        if(curnode->nodevalues.at(4).toInt() == 5)
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

    void RemEvidence(int curid)
    {
        int rownumber = rootnode->GetChildRow(curid);
        beginRemoveRows(QModelIndex(), rownumber, rownumber);
        rootnode->RemoveChild(rownumber);
        rootnode->childcount--;
        endRemoveRows();
    };

    void AddEvidence(int curid)
    {
        int filesystemcount;
        QSqlQuery addevidquery(fcasedb);
        addevidquery.prepare("SELECT objectid, name, fullpath, size, objecttype, address, crtime, atime, mtime, ctime, md5, parentid, type, parimgid, parfsid, flags, filemime, filesignature FROM data WHERE objectid = ? OR (objecttype < 5 AND parimgid = ?)");
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
                    currentnode->childcount = GetChildCount(1, currentnode->nodevalues.at(0).toInt());
                    currentnode->haschildren = currentnode->HasChildren();
                    parentnode = currentnode;
                }
                else if(currentnode->nodevalues.at(4).toInt() == 2) // volume
                {
                    currentnode->parent = parentnode;
                    parentnode->children.append(currentnode);
                    currentnode->childcount = GetChildCount(2, currentnode->nodevalues.at(0).toInt(), curid);
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
                        currentnode->childcount = GetChildCount(4, fsobjectlist.at(filesystemcount).rootinum, curid);
                        filesystemcount++;
                    }
                    currentnode->haschildren = currentnode->HasChildren();
                }
            }
            QSqlQuery filequery(fcasedb);
            Node* rootdirectory = 0;
            for(int j=0; j < fsobjectlist.count(); j++)
            {
                filequery.prepare("SELECT objectid, name, fullpath, size, objecttype, address, crtime, atime, mtime, ctime, md5, parentid, type, parimgid, parfsid, flags, filemime, filesignature FROM data WHERE objecttype = 5 AND parimgid = ? AND parentid = ?)");
                filequery.addBindValue(curid);
                filequery.addBindValue(fsobjectlist.at(j).rootinum);
                if(filequery.exec())
                {
                    while(filequery.next())
                    {
                        for(int i=0; i < parentnode->children.count(); i++)
                        {
                            if(filequery.value(14).toInt() == parentnode->children.at(i)->nodevalues.at(0).toInt())
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
                            currentnode->childcount = GetChildCount(5, currentnode->nodevalues.at(5).toInt(), curid);
                            currentnode->haschildren = currentnode->HasChildren();
                        }
                        rootdirectory->children.append(currentnode);
                    }
                }
                filequery.finish();
            }
           endInsertRows();
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
        int checkcount = 0;
        for(int i=0; i < curnode->children.count(); i++)
        {
            if(curnode->children[i]->checkstate == 2 || curnode->children[i]->checkstate == 1)
                checkcount++;
        }
        if(curnode->childcount > checkcount && checkcount > 0)
            curnode->checkstate = 1;
        else if(curnode->childcount == checkcount)
            curnode->checkstate = 2;
        else if(checkcount == 0)
            curnode->checkstate = 0;
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
        }
        else if(state == Qt::PartiallyChecked) // curnode is now partially checked
            curnode->checkstate = 1;
        else if(state == Qt::Checked) // currentnode is now checked
        {
            curnode->checkstate = 2;
            if(curnode->haschildren)
                SetChildCheckState(index);
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
    WombatFramework* wombatframework;
    PropertiesWindow* propertywindow;
    ExportDialog* exportdialog;
    FileViewer* fileviewer;
    TreeModel* treemodel;
    QMenu* treemenu;
    IdFilter* idfilterview;
    NameFilter* namefilterview;
    PathFilter* pathfilterview;
    SizeFilter* sizefilterview;
    CreatedDateFilter* createfilterview;
    AccessedDateFilter* accessfilterview;
    ModifiedDateFilter* modifyfilterview;
    ChangedDateFilter* changefilterview;
    FileTypeFilter* filetypefilterview;
    HashFilter* hashfilterview;
    ImageViewer* imagewindow;
    VideoViewer* videowindow;
    ViewerManager* viewmanage;

signals:
    //void LogVariable(WombatVariable* wombatVariable);

private slots:
    void AddEvidence();
    void RemEvidence();
    void ExportEvidence();
    void on_actionNew_Case_triggered();
    void on_actionOpen_Case_triggered();
    void on_actionView_Properties_triggered(bool checked);
    void on_actionView_Progress_triggered(bool checked);
    void on_actionView_File_triggered(bool checked);
    void on_actionView_Image_Gallery_triggered(bool checked);
    void on_actionViewerManager_triggered(bool checked);
    void UpdateProgress(int count, int processcount);
    void SelectionChanged(const QItemSelection &selitem, const QItemSelection &deselitem);
    void HideProgressWindow(bool checkstate);
    void HidePropertyWindow(bool checkstate);
    void HideFileViewer(bool checkstate);
    void HideImageWindow(bool checkstate);
    void HideViewerManager(bool checkstate);
    void DisplayError(QString errorNumber, QString errorType, QString errorValue);
    void ResizeColumns(void);
    void OpenParentImage(int imgid);
    void OpenParentFileSystem(int fsid);
    void OpenFileSystemFile(void);
    void ResizeViewColumns(const QModelIndex &index)
    {
        if(index.isValid())
            ResizeColumns();
    };
    void ExpandCollapseResize(const QModelIndex &index)
    {
        if(((TreeModel*)ui->dirTreeView->model())->canFetchMore(index))
            ((TreeModel*)ui->dirTreeView->model())->fetchMore(index);
        ResizeViewColumns(index);
    };
    void FileExport(FileExportData* exportdata);
    void setScrollBarRange(off_t low, off_t high);
    void setScrollBarValue(off_t pos);
    void SetOffsetLabel(off_t pos);
    void UpdateSelectValue(const QString &txt);
    //void LoadComplete(bool isok);
    void InitializeQueryModel(void);
    void FinishExport(void);
    void FinishRemoval(void);
    void StatusUpdate(QString tmptext)
    {
        statuslabel->setText(tmptext);
    };
    void SetStepValues(int singlestep, int pagestep);
    void TreeContextMenu(const QPoint &point);
    void SetFilter(int headercolumn);
    void FilterApplied()
    {
        UpdateFilterCount();
        emit ui->dirTreeView->header()->geometriesChanged();
    };
    void NextItem();
    void PreviousItem();
    void UpdateThumbnails(int tsize);
    void SetSelectedFromImageViewer(int selectedid);
    void ShowFile(const QModelIndex &index);

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
    //void LoadTxtContents(void);
    //void LoadWebContents(void);
    //void LoadImgContents(void);
    //void LoadVidContents(void);
    void OpenEvidenceStructure(void);
    void ExportFiles(FileExportData* exportdata);
    void GetExportData(Node* curnode, FileExportData* exportdata);
    void ProcessExport(TskObject curobject, std::string fullpath, std::string name);
    void UpdateFilterCount(void);

    void RemoveTmpFiles(void);

    QModelIndex selectedindex;
    QModelIndex oldselectedindex;

    QFuture<void> sqlfuture;
    QFutureWatcher<void> sqlwatcher;
    QFuture<void> exportfuture;
    QFutureWatcher<void> exportwatcher;
    QFuture<void> remfuture;
    QFutureWatcher<void> remwatcher;

    off_t offset() const;
    ImageHexViewer* hexwidget;
    QScrollBar* hexvsb;
    QLabel* selectedoffset;
    QLabel* selectedhex;
    QLabel* filecountlabel;
    QLabel* filtercountlabel;
    QLabel* processcountlabel;
    QLabel* statuslabel;
    QFrame* vline1;
    QFrame* vline2;
    QVector<FileExportData> exportfilelist;
    QShortcut* jumpforward;
    QShortcut* jumpbackward;
};

#endif // WOMBATFORENSICS_H
