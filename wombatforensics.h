#ifndef WOMBATFORENSICS_H
#define WOMBATFORENSICS_H

// Copyright 2015 Pasquale J. Rinaldi, Jr.
// Distrubted under the terms of the GNU General Public License version 2

#include "wombatinclude.h"
#include "wombatvariable.h"
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
        rootnode = 0;
        QList<QVariant> emptyset;
        emptyset << "" << "" << "" << "" << "" << "" << "" << "" << "" << "" << "";
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
        nodetype = node->nodevalues.at(0).toString().split("-").count();
        itemtype = node->nodetype;
        if(role == Qt::ForegroundRole)
        {
            if(nodetype == 4)
            {
                if(node->nodevalues.at(0).toString().contains(filtervalues.idfilter) == false)
                    return QColor(Qt::lightGray);
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
                if(filtervalues.maxcreatebool && filtervalues.mincreatebool == false)
                {
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
                    if(node->nodevalues.at(10).toString().contains(filtervalues.filegroup) == false)
                            return QColor(Qt::lightGray);
                }
                if(filtervalues.hashbool)
                {
                    for(int i = 0; i < filtervalues.hashlist.count(); i++)
                    {
                        if(node->nodevalues.at(8).toString().compare(filtervalues.hashlist.at(i)) == 0)
                            return QColor(Qt::lightGray);
                    }
                }
                if(filtervalues.hashbool2)
                {
                    if(node->nodevalues.at(8).toString().contains(filtervalues.hashfilter, Qt::CaseInsensitive) == false)
                        return QColor(Qt::lightGray);
                }
            }
            else if(nodetype < 4)
                return QColor(Qt::darkBlue);
        }
        if(role == Qt::DisplayRole)
        {
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
            QString nodename = node->nodevalues.at(1).toString();
            if(index.column() == 0)
            {
                if(nodetype == 1)
                    return QIcon(QPixmap(QString(":/basic/treeimg")));
                else if(nodetype == 2)
                    return QIcon(QPixmap(QString(":/basic/treevol")));
                else if(nodetype == 7)
                    return QIcon(QPixmap(QString(":/basic/treepart")));
                else if(nodetype == 3)
                    return QIcon(QPixmap(QString(":/basic/treefs")));
                else if(nodetype == 4)
                {
                    if(itemtype == 5)
                    {
                        QString estring = node->nodevalues.at(0).toString().split("-", QString::SkipEmptyParts).at(0);
                        QString pstring = node->nodevalues.at(0).toString().split("-", QString::SkipEmptyParts).at(2);
                        QString fstring = node->nodevalues.at(0).toString().split("-", QString::SkipEmptyParts).at(3);
                        QString tmpstr = "";
                        QDir eviddir = QDir(wombatvariable.tmpmntpath);
                        QStringList evidfiles = eviddir.entryList(QStringList("*.evid." + estring.mid(1)), QDir::NoSymLinks | QDir::Files);
                        QFile propfile(wombatvariable.tmpmntpath + evidfiles.at(0).split(".evid").at(0) + "." + pstring + "." + fstring + ".prop");
                        propfile.open(QIODevice::ReadOnly | QIODevice::Text);
                        QTextStream in(&propfile);
                        QString line = "";
                        while(!in.atEnd())
                        {
                            line = in.readLine();
                            if(line.contains("Allocation Status||"))
                            {
                                tmpstr = line.split("||").at(1);
                                //qDebug() << line << line.split("||").at(1);
                                break;
                            }
                        }
                        propfile.close();
                        //return line.split("||").at(1);
                        //QString tmpstr = FindProperty("Allocation Status", node->nodevalues.at(0).toString());
                        QStringList tmplist = tmpstr.split(",", QString::SkipEmptyParts);
                        if(nodename.compare("AttrDef") == 0 || nodename.compare("$BadClus") == 0 || nodename.compare("$Bitmap") == 0 || nodename.compare("$Boot") == 0 || nodename.compare("$ObjId") == 0 || nodename.compare("$Quota") == 0 || nodename.compare("$Reparse") == 0 || nodename.compare("$LogFile") == 0 || nodename.compare("$MFT") == 0 || nodename.compare("$MFTMirr") == 0 || nodename.compare("$Secure") == 0 || nodename.compare("$UpCase") == 0 || nodename.compare("$Volume") == 0)
                            return QIcon(QPixmap(QString(":/basic/virtualfile")));
                        else if(tmplist.at(0).contains("Unallocated") && tmplist.at(1).contains("Used"))
                            return QIcon(QPixmap(QString(":/basic/deletedfile")));
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
                    else if(itemtype == 0 && node->nodevalues.at(2).toString().contains("/$OrphanFiles/"))
                    {
                        QString estring = node->nodevalues.at(0).toString().split("-", QString::SkipEmptyParts).at(0);
                        QString pstring = node->nodevalues.at(0).toString().split("-", QString::SkipEmptyParts).at(2);
                        QString fstring = node->nodevalues.at(0).toString().split("-", QString::SkipEmptyParts).at(3);
                        QString tmpstr = "";
                        QDir eviddir = QDir(wombatvariable.tmpmntpath);
                        QStringList evidfiles = eviddir.entryList(QStringList("*.evid." + estring.mid(1)), QDir::NoSymLinks | QDir::Files);
                        QFile propfile(wombatvariable.tmpmntpath + evidfiles.at(0).split(".evid").at(0) + "." + pstring + "." + fstring + ".prop");
                        propfile.open(QIODevice::ReadOnly | QIODevice::Text);
                        QTextStream in(&propfile);
                        QString line = "";
                        while(!in.atEnd())
                        {
                            line = in.readLine();
                            if(line.contains("Allocation Status||"))
                            {
                                tmpstr = line.split("||").at(1);
                                break;
                            }
                        }
                        propfile.close();
                        QStringList tmplist = tmpstr.split(",", QString::SkipEmptyParts);
                        if(tmplist.at(0).contains("Unallocated") && tmplist.at(1).contains("Used"))
                            return QIcon(QPixmap(QString(":/basic/deletedfile")));
                        else
                            return QIcon(QPixmap(QString(":/basic/treefile")));
                    }
                    else
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
            SetCheckState(index, state);
            emit dataChanged(index, index);
            return true;
        }
        if(role == Qt::DisplayRole)
        {
            //Node* currentnode = NodeFromIndex(index);
            return true;
        }
        return false;
    };

    void dataChanged(const QModelIndex &topleftindex, const QModelIndex &botrightindex, const QVector<int> &roles = QVector<int>())
    {
        if(topleftindex == botrightindex)
        {
        }
        if(roles.count() < 0)
        {
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
            if(section == 0 && (!filtervalues.idfilter.isEmpty() && !filtervalues.idfilter.isNull()))
                return QIcon(QPixmap(QString(":/basic/filterimg")));
            if(section == 1 && filtervalues.namebool)
                return QIcon(QPixmap(QString(":/basic/filterimg")));
            if(section == 2 && filtervalues.pathbool)
                return QIcon(QPixmap(QString(":/basic/filterimg")));
            if(section == 3 && filtervalues.maxsizebool)
                return QIcon(QPixmap(QString(":/basic/filterimg")));
            if(section == 3 && filtervalues.minsizebool)
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
            if(section == 8 && (filtervalues.hashbool || filtervalues.hashbool2))
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
        if(index.column() == 0 && index.sibling(index.row(), 0).data().toString().split("-").count() == 4)
            flags |= Qt::ItemIsUserCheckable;
        
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
        QStringList tmplist;
        QString tmpstr = "";
        unsigned long long parentaddr = parent.sibling(parent.row(), 0).data().toString().split("-").last().mid(1).toULongLong();
        int curpart = parent.sibling(parent.row(), 0).data().toString().split("-").at(2).mid(1).toInt();
        parentnode = NodeFromIndex(parent);
        QDir eviddir = QDir(wombatvariable.tmpmntpath);
        QStringList evidfiles = eviddir.entryList(QStringList("*.evid.*"), QDir::NoSymLinks | QDir::Files);
        for(int j = 0; j < evidfiles.count(); j++)
        {
            QStringList curfiles = GetChildFiles(evidfiles.at(j).split(".evid").at(0) + ".p" + QString::number(curpart) + "*.a" + QString::number(parentaddr));
            if(curfiles.count() > 0)
            {
                QFile childfile;
                beginInsertRows(parent, 0, parentnode->childcount - 1);
                for(int i = 0; i < curfiles.count(); i++)
                {
                    tmpstr = "";
                    currentnode = 0;
                    tmplist.clear();
                    colvalues.clear();
                    childfile.setFileName(wombatvariable.tmpmntpath + curfiles.at(i));
                    childfile.open(QIODevice::ReadOnly);
                    tmpstr = childfile.readLine();
                    childfile.close();
                    tmplist = tmpstr.split(",");
                    colvalues.append(tmplist.at(12).split("-a").at(0));     // ID
                    QByteArray ba;
                    ba.append(tmplist.at(0));
                    colvalues.append(QByteArray::fromBase64(ba));           // Name
                    colvalues.append(tmplist.at(3));                        // Full Path
                    colvalues.append(tmplist.at(8));                        // Size
                    colvalues.append(tmplist.at(4));                        // Created
                    colvalues.append(tmplist.at(5));                        // Accessed
                    colvalues.append(tmplist.at(6));                        // Modified
                    colvalues.append(tmplist.at(7));                        // Status Changed
                    if(tmplist.at(13).compare("0") == 0)
                        colvalues.append("");                               // MD5
                    else
                        colvalues.append(tmplist.at(13));                   // MD5
                    colvalues.append(tmplist.at(10));                       // File Signature
                    colvalues.append(tmplist.at(10).split("/").at(0));      // File Category
                    currentnode = new Node(colvalues);
                    currentnode->parent = parentnode;
                    currentnode->nodetype = tmplist.at(1).toInt();          // node type 5=file, 3=dir, 10=vir file, 11=vir dir
                    parentnode->children.append(currentnode);
                    currentnode->childcount = GetChildCount(wombatvariable.evidencename.split(".evid").at(0) + ".p" + QString::number(curpart) + "*.a" + tmplist.at(9));
                    currentnode->haschildren = currentnode->HasChildren();
                }
                endInsertRows();
            }
        }
    };

    void GetModelCount(Node* curnode)
    {
        if(curnode->nodevalues.at(0).toString().split("-").count() == 4)
        {
            totalcount++;
            if(curnode->checkstate == true)
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

    void RemEvidence(QString curid)
    {
        int rownumber = rootnode->GetChildRow(curid);
        beginRemoveRows(QModelIndex(), rownumber, rownumber);
        rootnode->RemoveChild(rownumber);
        rootnode->childcount--;
        endRemoveRows();
    };

    void AddEvidence(int evidcount)
    {
        QStringList tmplist;
        QString tmpstr = "";
        tmplist.clear();
        QFile evidfile(wombatvariable.tmpmntpath + wombatvariable.evidencename + ".evid." + QString::number(evidcount));
        evidfile.open(QIODevice::ReadOnly);
        tmpstr = evidfile.readLine();
        tmplist = tmpstr.split(",");
        evidfile.close();
        beginInsertRows(QModelIndex(), rootnode->childcount, rootnode->childcount);
        currentnode = 0;
        colvalues.clear();
        colvalues.append(tmplist.at(5));                        // ID
        colvalues.append(wombatvariable.evidencename);   // Name
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
        currentnode->childcount = GetChildCount(wombatvariable.evidencename + ".vol");
        currentnode->haschildren = currentnode->HasChildren();
        if(checkhash.contains(tmplist.at(5)))
            currentnode->checkstate = checkhash.value(tmplist.at(5));
        else
            currentnode->checkstate = false;
        parentnode = currentnode;
        //wombatid++;
        // APPEND VOLUME TO NODE TREE
        tmpstr = "";
        currentnode = 0;
        tmplist.clear();
        colvalues.clear();
        QFile volfile(wombatvariable.tmpmntpath + wombatvariable.evidencename + ".vol");
        volfile.open(QIODevice::ReadOnly);
        tmpstr = volfile.readLine();
        volfile.close();
        tmplist = tmpstr.split(",");
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
        volnode = currentnode;
        currentnode->parent = parentnode;
        parentnode->children.append(currentnode);
        currentnode->childcount = GetChildCount(wombatvariable.evidencename + ".part.*");
        currentnode->haschildren = currentnode->HasChildren();
        if(checkhash.contains(tmplist.at(5)))
            currentnode->checkstate = checkhash.value(tmplist.at(5));
        else
            currentnode->checkstate = false;
        parentnode = currentnode;
        //wombatid++;
        int partcount = currentnode->childcount;
        QFile partfile;
        for(int i = 0; i < partcount; i++)
        {
            parentnode = volnode;
            tmpstr = "";
            currentnode = 0;
            tmplist.clear();
            colvalues.clear();
            partfile.setFileName(wombatvariable.tmpmntpath + wombatvariable.evidencename + ".part." + QString::number(i));
            partfile.open(QIODevice::ReadOnly);
            tmpstr = partfile.readLine();
            partfile.close();
            tmplist = tmpstr.split(",");
            rootinum = tmplist.at(3);
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
            partnode = currentnode;
            currentnode->parent = parentnode;
            parentnode->children.append(currentnode);
            currentnode->childcount = GetChildCount(wombatvariable.evidencename + ".p" + QString::number(i) + "*.a" + rootinum);
            currentnode->haschildren = currentnode->HasChildren();
            if(checkhash.contains(tmplist.at(10)))
                currentnode->checkstate = checkhash.value(tmplist.at(10));
            else
                currentnode->checkstate = false;
            parentnode = currentnode;
            //wombatid++;
            QFile filefile;
            QStringList curfiles = GetChildFiles(wombatvariable.evidencename + ".p" + QString::number(i) + "*.a" + rootinum);
            for(int j = 0; j < curfiles.count(); j++)
            {
                parentnode = partnode;
                tmpstr = "";
                currentnode = 0;
                tmplist.clear();
                colvalues.clear();
                filefile.setFileName(wombatvariable.tmpmntpath + curfiles.at(j));
                filefile.open(QIODevice::ReadOnly);
                tmpstr = filefile.readLine();
                filefile.close();
                tmplist = tmpstr.split(",");
                colvalues.append(tmplist.at(12).split("-a").at(0));     // ID
                QByteArray ba;
                ba.append(tmplist.at(0));
                colvalues.append(QByteArray::fromBase64(ba));           // Name
                colvalues.append(tmplist.at(3));                        // Full Path
                colvalues.append(tmplist.at(8));                        // Size
                colvalues.append(tmplist.at(4));                        // Created
                colvalues.append(tmplist.at(5));                        // Accessed
                colvalues.append(tmplist.at(6));                        // Modified
                colvalues.append(tmplist.at(7));                        // Status Changed
                if(tmplist.at(13).compare("0") == 0)
                    colvalues.append("");                               // MD5
                else
                    colvalues.append(tmplist.at(13));                   // MD5
                colvalues.append(tmplist.at(10));                       // File Signature
                colvalues.append(tmplist.at(10).split("/").at(0));      // File Category
                currentnode = new Node(colvalues);
                currentnode->parent = parentnode;
                currentnode->nodetype = tmplist.at(1).toInt();
                parentnode->children.append(currentnode);
                currentnode->childcount = GetChildCount(wombatvariable.evidencename + ".p" + QString::number(i) + "*.a" + tmplist.at(9));
                currentnode->haschildren = currentnode->HasChildren();
                if(checkhash.contains(tmplist.at(12).split("-a").at(0)))
                    currentnode->checkstate = checkhash.value(tmplist.at(12).split("-a").at(0));
                else
                    currentnode->checkstate = 0;
                if(currentnode->HasChildren())
                {
                    PopulateChildren(currentnode, i, tmplist.at(9));
                    /*
                QFile subfile;
                QStringList subfiles = GetChildFiles(wombatvariable.evidencename + ".p" + QString::number(i) + "*.a" + tmplist.at(9));
                for(k = 0; k < subfiles.count(); k++)
                {
                    parentnode = currentnode;
                    tmpstr = "";
                    currentnode = 0;
                    tmplist.clear();
                    colvalues.clear();
                    subfile.setFileName(wombatvariable.tmpmntpath + subfiles.at(k));
                    subfile.open(QIODevice::ReadOnly);
                    tmpstr = subfile.readLine();
                    subfile.close();
                    tmplist = tmpstr.split(",");
                    colvalues.append(tmplist.at(12).split("-a").at(0));     // ID
                    QByteArray ba;
                    ba.append(tmplist.at(0));
                    colvalues.append(QByteArray::fromBase64(ba));           // Name
                    colvalues.append(tmplist.at(3));                        // Full Path
                    colvalues.append(tmplist.at(8));                        // Size
                    colvalues.append(tmplist.at(4));                        // Created
                    colvalues.append(tmplist.at(5));                        // Accessed
                    colvalues.append(tmplist.at(6));                        // Modified
                    colvalues.append(tmplist.at(7));                        // Status Changed
                    if(tmplist.at(13).compare("0") == 0)
                        colvalues.append("");                               // MD5
                    else
                        colvalues.append(tmplist.at(13));                   // MD5
                    colvalues.append(tmplist.at(10));                       // File Signature
                    colvalues.append(tmplist.at(10).split("/").at(0));      // File Category
                    currentnode = new Node(colvalues);
                    currentnode->parent = parentnode;
                    currentnode->nodetype = tmplist.at(1).toInt();
                    parentnode->children.append(currentnode);
                    currentnode->childcount = GetChildCount(wombatvariable.evidencename + ".p" + QString::number(i) + "*.a" + tmplist.at(9));
                    currentnode->haschildren = currentnode->HasChildren();
                    if(checkhash.contains(tmplist.at(12).split("-a").at(0)))
                        currentnode->checkstate = checkhash.value(tmplist.at(12).split("-a").at(0));
                    else
                        currentnode->checkstate = 0;
                    if(currentnode->haschildren())
                    {
                        PopulateChildren(currentnode);
                    }
                }
                */
                }
                //wombatid++;
            }
        }
        endInsertRows();
    };

    void PopulateChildren(Node* curnode, int i, QString parentaddr)
    {
        QString tmpstr = "";
        QStringList tmplist;
        QFile subfile;
        QStringList subfiles = GetChildFiles(wombatvariable.evidencename + ".p" + QString::number(i) + "*.a" + parentaddr);
        for(int k = 0; k < subfiles.count(); k++)
        {
            parentnode = curnode;
            tmpstr = "";
            currentnode = 0;
            tmplist.clear();
            colvalues.clear();
            subfile.setFileName(wombatvariable.tmpmntpath + subfiles.at(k));
            subfile.open(QIODevice::ReadOnly);
            tmpstr = subfile.readLine();
            subfile.close();
            tmplist = tmpstr.split(",");
            colvalues.append(tmplist.at(12).split("-a").at(0));     // ID
            QByteArray ba;
            ba.append(tmplist.at(0));
            colvalues.append(QByteArray::fromBase64(ba));           // Name
            colvalues.append(tmplist.at(3));                        // Full Path
            colvalues.append(tmplist.at(8));                        // Size
            colvalues.append(tmplist.at(4));                        // Created
            colvalues.append(tmplist.at(5));                        // Accessed
            colvalues.append(tmplist.at(6));                        // Modified
            colvalues.append(tmplist.at(7));                        // Status Changed
            if(tmplist.at(13).compare("0") == 0)
                colvalues.append("");                               // MD5
            else
                colvalues.append(tmplist.at(13));                   // MD5
            colvalues.append(tmplist.at(10));                       // File Signature
            colvalues.append(tmplist.at(10).split("/").at(0));      // File Category
            currentnode = new Node(colvalues);
            currentnode->parent = parentnode;
            currentnode->nodetype = tmplist.at(1).toInt();
            parentnode->children.append(currentnode);
            currentnode->childcount = GetChildCount(wombatvariable.evidencename + ".p" + QString::number(i) + "*.a" + tmplist.at(9));
            currentnode->haschildren = currentnode->HasChildren();
            if(checkhash.contains(tmplist.at(12).split("-a").at(0)))
                currentnode->checkstate = checkhash.value(tmplist.at(12).split("-a").at(0));
            else
                currentnode->checkstate = 0;
            if(currentnode->HasChildren())
                PopulateChildren(currentnode, i, tmplist.at(9));
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
        if(curnode->checkstate == false) // unchecked
            return Qt::Unchecked;
        else
            return Qt::Checked;
    };
    
    bool SetCheckState(const QModelIndex &index, Qt::CheckState state)
    {
        Node* curnode = NodeFromIndex(index);
        if(state == Qt::Unchecked) // curnode is now unchecked...
        {
            curnode->checkstate = false;
            checkhash.insert(curnode->nodevalues.at(0).toString(), false);

        }
        else if(state == Qt::Checked) // currentnode is now checked
        {
            curnode->checkstate = true;
            checkhash.insert(curnode->nodevalues.at(0).toString(), true);
        }
        emit dataChanged(index, index);
        emit checkedNodesChanged();
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
    TskObject tskobject;
    TskObject* tskobjptr;
    TskObject tskexternalobject;
    TskObject* tskexternalptr;
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
    void UpdateProgress(unsigned long long count);
    void UpdateExport(void);
    void UpdateDig(void);
    void UpdateCheckCount(void);
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
    void ResizeViewColumns(const QModelIndex &index)
    {
        if(index.isValid())
            ResizeColumns();
    };
    // WILL PROBABLY NOT NEED
    void ExpandCollapseResize(const QModelIndex &index)
    {
        QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
        /*
        if(((TreeModel*)ui->dirTreeView->model())->canFetchMore(index))
        {
            ((TreeModel*)ui->dirTreeView->model())->fetchMore(index);
        }
        */
        ResizeViewColumns(index);
        QApplication::restoreOverrideCursor();
    };
    void ExportFiles(int exporttype, bool originalpath, QString exportpath);
    void DigFiles(int digtype, QVector<int> digoptions);
    void SetOffsetLabel(off_t pos);
    void ResetSlider(void);
    void ShowRockerToolTip(int moved);
    void SkipDown(void);
    void SkipUp(void);
    void PageUp(void);
    void PageDown(void);
    void UpdateSelectValue(const QString &txt);
    void UpdateDataTable(void);
    void UpdateStatus(void);
    void UpdateListed(void);
    void UpdateListed(const QModelIndex &index);
    void UpdateDigging(void);
    void FinishExport(void);
    void FinishThumbs(void);
    //void FinishHex(void);
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
    void SetSelectedFromImageViewer(QString selectedid);
    void ShowFile(const QModelIndex &index);
    void AddSection(void);
    void AddTextSection(void);
    void CarveFile(void);
    void AutoSaveState(void);
    void OpenCaseMountFinished(int exitcode, QProcess::ExitStatus exitstatus);

protected:
    void closeEvent(QCloseEvent* event);
    void mouseDoubleClickEvent(QMouseEvent* event);
private:
    Ui::WombatForensics *ui;

    void SetupHexPage(void);
    void InitializeAppStructure(void);
    void InitializeCaseStructure(void);
    void InitializeOpenCase(void);
    void CloseCurrentCase(void);
    void UpdateProperties(void);
    void LoadHexContents(void);
    void StartThumbnails(void);
    void GetExportList(Node* curnode, int exporttype);
    void ReturnListedCount(Node* curnode);
    void GetDigList(Node* curnode, int digtype);
    //void ProcessExport(QString curid);
    void ProcessDig(QString curid);
    void UpdateFilterCount(void);
    void SaveState(void);
    void RemoveTmpFiles(void);
    void UpdateCheckState(void);
    void InitializeCheckState(void);
    void UpdateSelectedState(QString id);
    QString InitializeSelectedState(void);
    QModelIndex selectedindex;
    QModelIndex oldselectedindex;

    QFuture<void> sqlfuture;
    QFutureWatcher<void> sqlwatcher;
    QFuture<void> hexfuture;
    QFutureWatcher<void> hexwatcher;
    QFuture<void> thumbfuture;
    QFutureWatcher<void> thumbwatcher;
    QFutureWatcher<void> exportwatcher;
    QFutureWatcher<void> digwatcher;

    QFile casesfile;
    QFile settingsfile;
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
    QLabel* checkedcountlabel;
    QLabel* statuslabel;
    QFrame* vline1;
    QFrame* vline2;
    //QString exportpath;
    int exporttype;
    //bool originalpath;
    //QStringList exportlist;
    int digtype;
    QVector<int> digoptions;
    //QStringList digfilelist;
    QShortcut* jumpforward;
    QShortcut* jumpbackward;
    QShortcut* showitem;
    QTimer* autosavetimer;
};

#endif // WOMBATFORENSICS_H
