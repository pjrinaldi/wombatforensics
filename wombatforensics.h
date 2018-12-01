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
#include "settingsdialog.h"
#define UNIX
#define UNIX_64
#include "sccex.h"
#ifdef UNIX
#define PATH_TYPE   IOTYPE_UNIXPATH
#define STRLEN      strlen
#endif

#ifndef UNUSED
#define UNUSED(x) ((x) = (x))
#endif

#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

class TreeNodeModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit TreeNodeModel(const QString &data, QObject* parent = 0) : QAbstractItemModel(parent)
    {
        QList<QVariant> zerodata;
        zerodata << "ID" << "Name" << "Full Path" << "Size (bytes)" << "Created (UTC)" << "Accessed (UTC)" << "Modified (UTC)" << "Status Changed (UTC)" << "MD5 Hash" << "File Category" << "File Signature";
        zeronode = new TreeNode(zerodata);
        AddEvidence(data.split((QString("\n"))), zeronode);
    };

    ~TreeNodeModel()
    {
        delete zeronode;
    };

    QVariant data(const QModelIndex &index, int role) const override
    {
        if(!index.isValid())
            return QVariant();
        /*
        if(role != Qt::DisplayRole)
            return QVariant();
        */
        
        TreeNode* itemnode = static_cast<TreeNode*>(index.internalPointer());
        int nodetype = 0;
        int itemtype = 0;
        QByteArray ba;
        nodetype = itemnode->Data(0).toString().split("-a").first().split("-").count();
        itemtype = itemnode->itemtype; // node type 1=file, 2=dir, 10=vir file, 11=vir dir, -1=not file (evid image, vol, part, fs)

        if(role == Qt::CheckStateRole && index.column() == 0)
            return static_cast<int>(itemnode->IsChecked() ? Qt::Checked : Qt::Unchecked);
        else if(role == Qt::ForegroundRole)
        {
            if(nodetype < 4)
                return QColor(Qt::darkBlue);
            else if(nodetype == 4)
            {
                if(itemnode->Data(0).toString().contains(filtervalues.idfilter) == false)
                    return QColor(Qt::lightGray);
                if(filtervalues.namebool)
                {
                    ba.clear();
                    ba.append(itemnode->Data(1).toString());
                    if(QString(QByteArray::fromBase64(ba)).contains(filtervalues.namefilter) == false)
                        return QColor(Qt::lightGray);
                }
                if(filtervalues.pathbool)
                {
                    ba.clear();
                    ba.append(itemnode->Data(2).toString());
                    if(QString(QByteArray::fromBase64(ba)).contains(filtervalues.pathfilter) == false)
                        return QColor(Qt::lightGray);
                }
                if(filtervalues.maxsizebool && filtervalues.minsizebool == false)
                {
                    if(itemnode->Data(3).toULongLong() <= filtervalues.maxsize)
                        return QColor(Qt::lightGray);
                }
                if(filtervalues.minsizebool && filtervalues.maxsizebool == false)
                {
                    if(itemnode->Data(3).toULongLong() >= filtervalues.minsize)
                        return QColor(Qt::lightGray);
                }
                if(filtervalues.maxsizebool && filtervalues.minsizebool)
                {
                    if(itemnode->Data(3).toULongLong() >= filtervalues.minsize || itemnode->Data(3).toULongLong() <= filtervalues.maxsize)
                        return QColor(Qt::lightGray);
                }
                if(filtervalues.maxcreatebool && filtervalues.mincreatebool == false)
                {
                    if(itemnode->Data(4).toInt() <= filtervalues.maxcreate)
                        return QColor(Qt::lightGray);
                }
                if(filtervalues.maxcreatebool == false && filtervalues.mincreatebool)
                {
                    if(itemnode->Data(4).toInt() >= filtervalues.mincreate)
                        return QColor(Qt::lightGray);
                }
                if(filtervalues.maxcreatebool && filtervalues.mincreatebool)
                {
                    if(itemnode->Data(4).toInt() >= filtervalues.mincreate || itemnode->Data(4).toInt() <= filtervalues.maxcreate)
                        return QColor(Qt::lightGray);
                }
                if(filtervalues.maxaccessbool && filtervalues.minaccessbool == false)
                {
                    if(itemnode->Data(5).toInt() <= filtervalues.maxaccess)
                        return QColor(Qt::lightGray);
                }
                if(filtervalues.maxaccessbool == false && filtervalues.minaccessbool)
                {
                    if(itemnode->Data(5).toInt() >= filtervalues.minaccess)
                        return QColor(Qt::lightGray);
                }
                if(filtervalues.maxaccessbool && filtervalues.minaccessbool)
                {
                    if(itemnode->Data(5).toInt() >= filtervalues.minaccess || itemnode->Data(5).toInt() <= filtervalues.maxaccess)
                        return QColor(Qt::lightGray);
                }
                if(filtervalues.maxmodifybool && filtervalues.minmodifybool == false)
                {
                    if(itemnode->Data(6).toInt() <= filtervalues.maxmodify)
                        return QColor(Qt::lightGray);
                }
                if(filtervalues.maxmodifybool == false && filtervalues.minmodifybool)
                {
                    if(itemnode->Data(6).toInt() >= filtervalues.minmodify)
                        return QColor(Qt::lightGray);
                }
                if(filtervalues.maxmodifybool && filtervalues.minmodifybool)
                {
                    if(itemnode->Data(6).toInt() >= filtervalues.minmodify || itemnode->Data(6).toInt() <= filtervalues.maxmodify)
                        return QColor(Qt::lightGray);
                }
                if(filtervalues.maxchangebool && filtervalues.minchangebool == false)
                {
                    if(itemnode->Data(7).toInt() <= filtervalues.maxchange)
                        return QColor(Qt::lightGray);
                }
                if(filtervalues.maxchangebool == false && filtervalues.minchangebool)
                {
                    if(itemnode->Data(7).toInt() >= filtervalues.minchange)
                        return QColor(Qt::lightGray);
                }
                if(filtervalues.maxchangebool && filtervalues.minchangebool)
                {
                    if(itemnode->Data(7).toInt() >= filtervalues.minchange || itemnode->Data(7).toInt() <= filtervalues.maxchange)
                        return QColor(Qt::lightGray);
                }
                if(filtervalues.hashbool)
                {
                    for(int i = 0; i < filtervalues.hashlist.count(); i++)
                    {
                        if(itemnode->Data(8).toString().compare(filtervalues.hashlist.at(i)) == 0)
                            return QColor(Qt::lightGray);
                    }
                }
                if(filtervalues.hashbool2)
                {
                    if(itemnode->Data(8).toString().contains(filtervalues.hashfilter, Qt::CaseInsensitive) == false)
                        return QColor(Qt::lightGray);
                }
                if(filtervalues.filegroupbool)
                {
                    if(itemnode->Data(9).toString().contains(filtervalues.filegroup) == false)
                        return QColor(Qt::lightGray);
                }
                if(filtervalues.filetypebool)
                {
                    if(itemnode->Data(10).toString().contains(filtervalues.filetype) == false)
                            return QColor(Qt::lightGray);
                }
            }
        }
        else if(role == Qt::DisplayRole)
        {
            if(index.column() == 0)
            {
                return itemnode->Data(index.column()).toString().split("-a").at(0);
            }
            else if(index.column() == 1 || index.column() == 2)
            {
                if(nodetype == 4)
                {
                    ba.clear();
                    ba.append(itemnode->Data(index.column()).toString());
                    return QByteArray::fromBase64(ba);
                }
                else
                {
                    if(itemnode->Data(index.column()).toString().compare("0") == 0)
                        return "";
                    else
                        return itemnode->Data(index.column());
                }
            }
            else if(index.column() == 3)
                return itemnode->Data(index.column());
            else if(index.column() >= 4 && index.column() <= 7)
            {
                if(itemnode->Data(index.column()).toString().compare("0") == 0)
                    return "";
                else
                {
                    char* ibuffer = new char[128];
                    QString tmpstr = QString(TskTimeToStringUTC(itemnode->Data(index.column()).toInt(), ibuffer));
                    delete[] ibuffer;
                    return tmpstr;
                }
            }
            else if(index.column() >= 8 && index.column() <= 10)
            {
                if(itemnode->Data(index.column()).toString().compare("0") != 0)
                    return itemnode->Data(index.column());
                else
                    return "";
            }
            else
                return itemnode->Data(index.column());
        }
        else if(role == Qt::DecorationRole)
        {
            ba.clear();
            ba.append(itemnode->Data(1).toString());
            QString nodename = QByteArray::fromBase64(ba);
            if(index.column() == 0)
            {
                if(nodetype == 1)
                    return QIcon(QPixmap(QString(":/basic/treeimg")));
                else if(nodetype == 2)
                    return QIcon(QPixmap(QString(":/basic/treevol")));
                else if(nodetype == 3)
                    return QIcon(QPixmap(QString(":/basic/treefs")));
                else if(nodetype == 4)
                {
                    if((itemtype == 0 && itemnode->Data(2).toString().contains("/$OrphanFiles/")) || itemtype == 1)
                    {
                        if(nodename.compare("AttrDef") == 0 || nodename.compare("$BadClus") == 0 || nodename.compare("$Bitmap") == 0 || nodename.compare("$Boot") == 0 || nodename.compare("$ObjId") == 0 || nodename.compare("$Quota") == 0 || nodename.compare("$Reparse") == 0 || nodename.compare("$LogFile") == 0 || nodename.compare("$MFT") == 0 || nodename.compare("$MFTMirr") == 0 || nodename.compare("$Secure") == 0 || nodename.compare("$UpCase") == 0 || nodename.compare("$Volume") == 0)
                            return QIcon(QPixmap(QString(":/basic/virtualfile")));
                        else
                        {
                            if(itemnode->IsDeleted()) // Unallocated and Used
                                return QIcon(QPixmap(QString(":/basic/deletedfile")));
                            else
                                return QIcon(QPixmap(QString(":/basic/treefile")));
                        }
                    }
                    else if(itemtype == 2 || itemtype == 3)
                    {
                        if(nodename.compare("$OrphanFiles") == 0 || nodename.compare("$Extend") == 0)
                            return QIcon(QPixmap(QString(":/basic/virtualfolder")));
                        else
                            return QIcon(QPixmap(QString(":/basic/treefolder")));
                    }
                    else if(itemtype == 10)
                        return QIcon(QPixmap(QString(":/basic/virtualfile")));
                    else if(itemtype == 11)
                        return QIcon(QPixmap(QString(":/basic/virtualdir")));
                    else
                        return QIcon(QPixmap(QString(":/basic/treefile")));
                }
                else
                    return QIcon(QPixmap(QString(":/basic/treefile")));
            }
        }
        else
            return QVariant();
        return QVariant();
    };

    bool setData(const QModelIndex &index, const QVariant &value, int role)
    {
        if(value.isValid())
        {
        }
        TreeNode* itemnode = static_cast<TreeNode*>(index.internalPointer());
        if(role == Qt::CheckStateRole)
        {
            if(itemnode->IsChecked())
            {
                itemnode->SetChecked(false);
                checkhash.insert(itemnode->Data(0).toString().split("-a").first(), false);
            }
            else
            {
                itemnode->SetChecked(true);
                checkhash.insert(itemnode->Data(0).toString().split("-a").first(), true);
            }
            emit dataChanged(index, index);
            emit CheckedNodesChanged();
            return true;
        }

        if(role != Qt::EditRole)
            return false;
        // if i have other SetData() items for the treenode's, use code similar to below.
        //bool result = item->SetData(index, value);
        //if(result)
        //    emit dataChanged(index, index);
        //return result;
        return false;
    };

    Qt::ItemFlags flags(const QModelIndex &index) const override
    {
        Qt::ItemFlags flags = QAbstractItemModel::flags(index);
        TreeNode* itemnode = static_cast<TreeNode*>(index.internalPointer());

        if(!index.isValid())
            return Qt::NoItemFlags;
        if(index == QModelIndex())
            return Qt::NoItemFlags;
        if(index.column() == 0 && itemnode->Data(index.column()).toString().split("-a").first().split("-").count() == 4)
            flags |= Qt::ItemIsUserCheckable | Qt::ItemIsEditable;
        
        return flags;
    };

    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override
    {
        if(orientation == Qt::Horizontal && role == Qt::DisplayRole)
        {
            if(section >= 0)
                return zeronode->Data(section);
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
            if(section == 8 && (filtervalues.hashbool || filtervalues.hashbool2))
                return QIcon(QPixmap(QString(":/basic/filterimg")));
            if(section == 9 && filtervalues.filegroupbool)
                return QIcon(QPixmap(QString(":/basic/filterimg")));
            if(section == 10 && filtervalues.filetypebool)
                return QIcon(QPixmap(QString(":/basic/filterimg")));
        }
        return QVariant();
    };

    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override
    {
        if(!hasIndex(row, column, parent))
            return QModelIndex();

        TreeNode* parentnode;
        if(!parent.isValid())
            parentnode = zeronode;
        else
            parentnode = static_cast<TreeNode*>(parent.internalPointer());

        TreeNode* childnode = parentnode->child(row);
        if(childnode)
            return createIndex(row, column, childnode);
        else
            return QModelIndex();
    };

    QModelIndex parent(const QModelIndex &index) const override
    {
        if(!index.isValid())
            return QModelIndex();

        TreeNode* childnode = static_cast<TreeNode*>(index.internalPointer());
        TreeNode* parentnode = childnode->ParentItem();

        if(parentnode == zeronode)
            return QModelIndex();

        return createIndex(parentnode->Row(), 0, parentnode);
    };

    int rowCount(const QModelIndex &parent = QModelIndex()) const override
    {
        TreeNode* parentnode;
        if(parent.column() > 0)
            return 0;

        if(!parent.isValid())
            parentnode = zeronode;
        else
            parentnode = static_cast<TreeNode*>(parent.internalPointer());

        return parentnode->ChildCount();
    };

    int columnCount(const QModelIndex &parent = QModelIndex()) const override
    {
        if(parent.isValid())
            return static_cast<TreeNode*>(parent.internalPointer())->ColumnCount();
        else
            return zeronode->ColumnCount();
    };

signals:
    void CheckedNodesChanged();
private:
    void AddEvidence(const QStringList &nodes, TreeNode* parent)
    {
        // parent is the zero item...
        QString parid;
        QString curid;
        unsigned long long fpar;
        int nodecount = 0;
        QString tmpstr = "";
        for(int i=0; i < nodes.count(); i++)
        {
            if(nodes.at(i).length() > 0)
            {
                /*
                QDirIterator it(wombatvariable.tmpmntpath + nodes.at(i), QDirIterator::Subdirectories);
                while (it.hasNext())
                {
                    qDebug() << it.next();
                }
                */
    //QDir eviddir = QDir(wombatvariable.tmpmntpath);
    //QStringList files = eviddir.entryList(QStringList(filefilter), QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot | QDir::NoSymLinks);
                QDir eviddir = QDir(wombatvariable.tmpmntpath + nodes.at(i));
                QFile evidfile(wombatvariable.tmpmntpath + nodes.at(i) + "/stat");
                evidfile.open(QIODevice::ReadOnly | QIODevice::Text);
                tmpstr = evidfile.readLine();
                evidfile.close();
                QList<QVariant> columndata;
                columndata.clear();
                columndata << tmpstr.split(",").at(5) << tmpstr.split(",").at(3).split("/").last() << "0" << tmpstr.split(",").at(1) << "0" << "0" << "0" << "0" << "0" << "0" << "0";
                parent->AppendChild(new TreeNode(columndata, parent));
                curid = tmpstr.split(",").at(5);
                parents[curid] = parent->child(parent->ChildCount() - 1);
                //QStringList statlist = eviddir.entryList(QDir::Files | QDir::NoSymLinks | QDir::NoDotAndDotDot | QDir::Hidden, QDir::DirsLast);
                QStringList vollist = eviddir.entryList(QDir::NoSymLinks | QDir::NoDotAndDotDot | QDir::AllDirs | QDir::Hidden);
                for(int j=0; j < vollist.count(); j++)
                {
                    columndata.clear();
                    QFile volfile(wombatvariable.tmpmntpath + nodes.at(i) + "/" + vollist.at(j) + "/stat");
                    volfile.open(QIODevice::ReadOnly | QIODevice::Text);
                    tmpstr = volfile.readLine();
                    volfile.close();
                    columndata << tmpstr.split(",").at(5) << tmpstr.split(",").at(2) << "0" << tmpstr.split(",").at(1) << "0" << "0" << "0" << "0" << "0" << "0" << "0";
                    parid = tmpstr.split(",").at(5).split("-").at(0);
                    curid = tmpstr.split(",").at(5);
                    parents.value(parid)->AppendChild(new TreeNode(columndata, parents.value(parid)));
                    parents[curid] = parents.value(parid)->child(parents.value(parid)->ChildCount() - 1);
                    QDir voldir = QDir(wombatvariable.tmpmntpath + nodes.at(i) + "/" + vollist.at(j));
                    QStringList partlist = voldir.entryList(QDir::NoSymLinks | QDir::NoDotAndDotDot | QDir::AllDirs | QDir::Hidden);
                    for(int k = 0; k < partlist.count(); k++)
                    {
                        columndata.clear();
                        QFile partfile(wombatvariable.tmpmntpath + nodes.at(i) + "/" + vollist.at(j) + "/" + partlist.at(k) + "/stat");
                        partfile.open(QIODevice::ReadOnly | QIODevice::Text);
                        tmpstr = partfile.readLine();
                        partfile.close();
                        columndata << tmpstr.split(",").at(10) << tmpstr.split(",").at(2) << "0" << tmpstr.split(",").at(1) << "0" << "0" << "0" << "0" << "0" << "0" << "0";
                        parid = tmpstr.split(",").at(10).split("-p").at(0);
                        curid = tmpstr.split(",").at(10);
                        parents.value(parid)->AppendChild(new TreeNode(columndata, parents.value(parid)));
                        parents[curid] = parents.value(parid)->child(parents.value(parid)->ChildCount() - 1);
                        QDir partdir = QDir(wombatvariable.tmpmntpath + nodes.at(i) + "/" + vollist.at(j) + "/" + partlist.at(k));
                        QStringList rootlist = partdir.entryList(QDir::NoSymLinks | QDir::NoDotAndDotDot | QDir::AllDirs | QDir::Hidden);
                        for(int l = 0; l < rootlist.count(); l++)
                        {
                            columndata.clear();
                            QFile filefile(wombatvariable.tmpmntpath + nodes.at(i) + "/" + vollist.at(j) + "/" + partlist.at(k) + "/" + rootlist.at(l) + "/stat");
                            filefile.open(QIODevice::ReadOnly | QIODevice::Text);
                            tmpstr = filefile.readLine();
                            filefile.close();
                            columndata << tmpstr.split(",").at(12) << tmpstr.split(",").at(0) << tmpstr.split(",").at(3) << tmpstr.split(",").at(8) << tmpstr.split(",").at(6) << tmpstr.split(",").at(7) << tmpstr.split(",").at(4) << tmpstr.split(",").at(5) << tmpstr.split(",").at(13) << tmpstr.split(",").at(10).split("/").at(0) << tmpstr.split(",").at(10).split("/").at(1);
                            parid = tmpstr.split(",").at(12).split("-f").at(0);
                            curid = tmpstr.split(",").at(12);
                            parents.value(parid)->AppendChild(new TreeNode(columndata, parents.value(parid), tmpstr.split(",").at(1).toInt()));
                            parents[curid] = parents.value(parid)->child(parents.value(parid)->ChildCount() - 1);
                            if(checkhash.contains(tmpstr.split(",").at(12).split("-a").first()))
                                parents.value(curid)->SetChecked(true);
                            if(tmpstr.split(",").at(14).toInt() == true)
                                parents.value(curid)->SetDeleted(true);
                            FileRecurse(wombatvariable.tmpmntpath + nodes.at(i) + "/" + vollist.at(j) + "/" + partlist.at(k) + "/" + rootlist.at(l));
                            //FileRecurse(newpath);
                            // NEED TO RECURSE HERE FOR EACH FILE FOLDER TO CHECK IF IT HAS CHILDREN...
                            // CALL SOMETHING LIKE FileRecurse(path) WHICH WOULD GET CHILDREN, CALL THE DATA, ETC AND THEN CALL RECURSE
                            /*
                             *
            if(columnstrings.at(0).split("-").count() == 5) // files
            {
                fpar = columnstrings.at(0).split("-").at(4).mid(1).toULongLong();
                curid = columnstrings.at(0);
                parid = columnstrings.at(0).split("-f").at(0);
                QFile rootinumfile(wombatvariable.volumepath + ".p" + columnstrings.at(0).split("-").at(2).mid(1) + "/stat");
                //QFile rootinumfile(wombatvariable.tmpmntpath + wombatvariable.evidencename + ".part." + columnstrings.at(0).split("-").at(2).mid(1));
                rootinumfile.open(QIODevice::ReadOnly);
                unsigned long long rootinum = QString(rootinumfile.readLine()).split(",").at(3).toULongLong();
                rootinumfile.close();
                if(fpar != rootinum)
                {
                    QString parkey = parid + "-f" + QString::number(fpar) + "-a";
                    QHashIterator<QString, TreeNode*> i(parents);
                    while(i.hasNext())
                    {
                        i.next();
                        if(i.key().contains(parkey))
                            parid = i.key();
                    }
                }
                parents.value(parid)->AppendChild(new TreeNode(columndata, parents.value(parid), columnstrings.at(11).toInt()));
                parents[curid] = parents.value(parid)->child(parents.value(parid)->ChildCount() - 1);
                if(checkhash.contains(columnstrings.at(0).split("-a").first()))
                    parents.value(curid)->SetChecked(true);
                if(columnstrings.at(12).toInt() == true)
                    parents.value(curid)->SetDeleted(true);

                             *
                             */ 
                        }
                    }
                }
            }
        }
        /*
        while(nodecount < nodes.count() - 1)
        {
            QStringList columnstrings = nodes.at(nodecount).split(",", QString::SkipEmptyParts);
            QList<QVariant> columndata;
            for(int i = 0; i < 11; i++)
                columndata << columnstrings.at(i);
            if(columnstrings.at(0).split("-").count() == 1) // evidence image
            {
                curid = columnstrings.at(0).split("-").at(0);
                parent->AppendChild(new TreeNode(columndata, parent));
                parents[curid] = parent->child(parent->ChildCount() - 1);
            }
            nodecount++;
        }
        nodecount = 0;
        while(nodecount < nodes.count() - 1)
        {
            QStringList columnstrings = nodes.at(nodecount).split(",", QString::SkipEmptyParts);
            QList<QVariant> columndata;
            for(int i = 0; i < 11; i++)
                columndata << columnstrings.at(i);
            if(columnstrings.at(0).split("-").count() == 2) // volume
            {
                parid = columnstrings.at(0).split("-").at(0);
                curid = columnstrings.at(0);
                parents.value(parid)->AppendChild(new TreeNode(columndata, parents.value(parid)));
                parents[curid] = parents.value(parid)->child(parents.value(parid)->ChildCount() - 1);
            }
            nodecount++;
        }
        nodecount = 0;
        while(nodecount < nodes.count() - 1)
        {
            QStringList columnstrings = nodes.at(nodecount).split(",", QString::SkipEmptyParts);
            QList<QVariant> columndata;
            for(int i = 0; i < 11; i++)
                columndata << columnstrings.at(i);
            if(columnstrings.at(0).split("-").count() == 3) // partition
            {
                parid = columnstrings.at(0).split("-p").at(0);
                curid = columnstrings.at(0);
                parents.value(parid)->AppendChild(new TreeNode(columndata, parents.value(parid)));
                parents[curid] = parents.value(parid)->child(parents.value(parid)->ChildCount() - 1);
            }
            nodecount++;
        }
        nodecount = 0;
        while(nodecount < nodes.count() - 1)
        {
            QStringList columnstrings = nodes.at(nodecount).split(",", QString::SkipEmptyParts);
            QList<QVariant> columndata;
            for(int i = 0; i < 11; i++)
                columndata << columnstrings.at(i);
            if(columnstrings.at(0).split("-").count() == 5) // files
            {
                fpar = columnstrings.at(0).split("-").at(4).mid(1).toULongLong();
                curid = columnstrings.at(0);
                parid = columnstrings.at(0).split("-f").at(0);
                QFile rootinumfile(wombatvariable.volumepath + ".p" + columnstrings.at(0).split("-").at(2).mid(1) + "/stat");
                //QFile rootinumfile(wombatvariable.tmpmntpath + wombatvariable.evidencename + ".part." + columnstrings.at(0).split("-").at(2).mid(1));
                rootinumfile.open(QIODevice::ReadOnly);
                unsigned long long rootinum = QString(rootinumfile.readLine()).split(",").at(3).toULongLong();
                rootinumfile.close();
                if(fpar != rootinum)
                {
                    QString parkey = parid + "-f" + QString::number(fpar) + "-a";
                    QHashIterator<QString, TreeNode*> i(parents);
                    while(i.hasNext())
                    {
                        i.next();
                        if(i.key().contains(parkey))
                            parid = i.key();
                    }
                }
                parents.value(parid)->AppendChild(new TreeNode(columndata, parents.value(parid), columnstrings.at(11).toInt()));
                parents[curid] = parents.value(parid)->child(parents.value(parid)->ChildCount() - 1);
                if(checkhash.contains(columnstrings.at(0).split("-a").first()))
                    parents.value(curid)->SetChecked(true);
                if(columnstrings.at(12).toInt() == true)
                    parents.value(curid)->SetDeleted(true);
            }
            nodecount++;
        }*/
    };
    void FileRecurse(QString path)
    {
        QString parid;
        QString curid;
        QString tmpstr = "";
        unsigned long long fpar;
        QList<QVariant> columndata;
        QDir filedir = QDir(path);
        QStringList subfiles = filedir.entryList(QDir::NoSymLinks | QDir::NoDotAndDotDot | QDir::AllDirs | QDir::Hidden);
        for(int i=0; i < subfiles.count(); i++)
        {
            //qDebug() << subfiles.at(i);
            columndata.clear();
            QFile subfile(path + "/" + subfiles.at(i) + "/stat");
            subfile.open(QIODevice::ReadOnly | QIODevice::Text);
            tmpstr = subfile.readLine();
            subfile.close();
            columndata << tmpstr.split(",").at(12) << tmpstr.split(",").at(0) << tmpstr.split(",").at(3) << tmpstr.split(",").at(8) << tmpstr.split(",").at(6) << tmpstr.split(",").at(7) << tmpstr.split(",").at(4) << tmpstr.split(",").at(5) << tmpstr.split(",").at(13) << tmpstr.split(",").at(10).split("/").at(0) << tmpstr.split(",").at(10).split("/").at(1);
            fpar = path.split("/").last().split("f").last().toULongLong();
            parid = tmpstr.split(",").at(12).split("-f").at(0) + "-f" + QString::number(fpar) + "-a";
            QHashIterator<QString, TreeNode*> j(parents);
            while(j.hasNext())
            {
                j.next();
                if(j.key().contains(parid))
                {
                    parid = j.key();
                    break;
                }
            }
            //parid = tmpstr.split(",").at(12).split("-f").at(0);
            curid = tmpstr.split(",").at(12);
            parents.value(parid)->AppendChild(new TreeNode(columndata, parents.value(parid), tmpstr.split(",").at(1).toInt()));
            parents[curid] = parents.value(parid)->child(parents.value(parid)->ChildCount() - 1);
            if(checkhash.contains(tmpstr.split(",").at(12).split("-a").first()))
                parents.value(curid)->SetChecked(true);
            if(tmpstr.split(",").at(14).toInt() == true)
                parents.value(curid)->SetDeleted(true);
            FileRecurse(path + subfiles.at(i));
        }
        /*
         *  columndata.clear();
            QFile filefile(wombatvariable.tmpmntpath + nodes.at(i) + "/" + vollist.at(j) + "/" + partlist.at(k) + "/" + rootlist.at(l) + "/stat");
            filefile.open(QIODevice::ReadOnly | QIODevice::Text);
            tmpstr = filefile.readLine();
            filefile.close();
            columndata << tmpstr.split(",").at(12) << tmpstr.split(",").at(0) << tmpstr.split(",").at(3) << tmpstr.split(",").at(8) << tmpstr.split(",").at(6) << tmpstr.split(",").at(7) << tmpstr.split(",").at(4) << tmpstr.split(",").at(5) << tmpstr.split(",").at(13) << tmpstr.split(",").at(10).split("/").at(0) << tmpstr.split(",").at(10).split("/").at(1);
            parid = tmpstr.split(",").at(12).split("-f").at(0);
            curid = tmpstr.split(",").at(12);
            parents.value(parid)->AppendChild(new TreeNode(columndata, parents.value(parid), tmpstr.split(",").at(1).toInt()));
            parents[curid] = parents.value(parid)->child(parents.value(parid)->ChildCount() - 1);
            if(checkhash.contains(tmpstr.split(",").at(12).split("-a").first()))
                parents.value(curid)->SetChecked(true);
            if(tmpstr.split(",").at(14).toInt() == true)
                parents.value(curid)->SetDeleted(true);
            QDir partdir = QDir(wombatvariable.tmpmntpath + nodes.at(i) + "/" + vollist.at(j) + "/" + partlist.at(k));
            QStringList rootlist = partdir.entryList(QDir::NoSymLinks | QDir::NoDotAndDotDot | QDir::AllDirs | QDir::Hidden);
         *
         */ 

    };
public:
    //void RemEvidence(QString evidid)
    void RemEvidence()
    {
        /*
        if(evidid.compare("") == 0)
        {
        }
        */
        // READALL THE MODEL DATA INTO A STRINGLIST...
        // LOOP OVER STRING ITEMS AND IF MATCH EVIDID, REMOVE FROM LIST
        // WRITE THE STRINGLIST TO THE TREEFILE.
        // this would need to remove all instances of the e# from the file and then save the file and then reload the evidence...
        // have to think on this and figure out a quick way to do this...
        /*
        beginRemoveRows(QModelIndex(), evidid, evidid);
        parents.remove(evidid);
        endRemoveRows();
        */
        beginResetModel();
        endResetModel();
    };

    void Clear()
    {
        beginResetModel();
        parents.clear();
        endResetModel();
    };
private:
    TreeNode* zeronode; //rootitem
    QHash<QString, TreeNode*> parents;
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
    TskObject tskexternalobject;
    TskObject* tskexternalptr;
    PropertiesWindow* propertywindow;
    ExportDialog* exportdialog;
    DigDeeperDialog* digdeeperdialog;
    FileViewer* fileviewer;
    TreeNodeModel* treenodemodel;
    QMenu* treemenu;
    QMenu* selectionmenu;
    QMenu* bookmarkmenu;
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
    ImageWindow* imageviewer;
    VideoViewer* videowindow;
    ViewerManager* viewmanage;
    TextViewer* textviewer;
    HtmlViewer* htmlviewer;
    MessageViewer* msgviewer;
    ByteConverter* byteviewer;
    AboutBox* aboutbox;
    SettingsDialog* settingsdialog;

signals:
    void CancelCurrentThread();

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
    void on_actionSettings_triggered();
    void on_actionCancel_Operation_triggered();
    void UpdateProgress(unsigned long long count);
    void UpdateExport(void);
    void UpdateDig(void);
    void UpdateCheckCount(void);
    void SelectionChanged(const QItemSelection &selitem, const QItemSelection &deselitem);
    void HideImageWindow(bool checkstate);
    void HideViewerManager(void);
    void HideSettingsWindow(void);
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
    void ExportFiles(int exporttype, bool originalpath, QString exportpath);
    void DigFiles(int digtype, QVector<int> digoptions);
    void SetOffsetLabel(qint64 pos);
    void HexSelectionChanged(void);
    void UpdateSelectValue(void);
    void UpdateDataTable(void);
    void UpdateStatus(void);
    void UpdateDigging(void);
    void FinishExport(void);
    void FinishThumbs(void);
    void ThreadCancelled(void);
    void ReadXMountOut(void);
    void ReadXMountErr(void);
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
        emit treenodemodel->layoutChanged(); // this messes with the row height and cuts off icons...
    };
    void NextItem();
    void PreviousItem();
    void ShowItem();
    //void UpdateThumbnails(int tsize);
    void SetSelectedFromImageViewer(QString selectedid);
    void ShowFile(const QModelIndex &index);
    void AddSection(void);
    void AddTextSection(void);
    void CarveFile(void);
    void AutoSaveState(void);
    void OpenCaseMountFinished(int exitcode, QProcess::ExitStatus exitstatus);
    void PrepareEvidenceImage(void);

protected:
    void closeEvent(QCloseEvent* event);
    void mouseDoubleClickEvent(QMouseEvent* event);
private:
    Ui::WombatForensics *ui;

    void SetupHexPage(void);
    void CheckWombatConfiguration(void);
    void InitializeAppStructure(void);
    void InitializeCaseStructure(void);
    void InitializeOpenCase(void);
    void CloseCurrentCase(void);
    void UpdateProperties(void);
    void LoadHexContents(void);
    void StartThumbnails(void);
    QStringList GetFileLists(int filelisttype);
    void ProcessDig(QString curid);
    void UpdateFilterCount(void);
    void SaveState(void);
    void RemoveTmpFiles(void);
    void UpdateCheckState(void);
    void InitializeCheckState(void);
    void UpdateSelectedState(QString id);
    void ReadSettings(void);
    static void PopulateModel(const QModelIndex index);
    QString InitializeSelectedState(void);
    QModelIndex selectedindex;
    QModelIndex oldselectedindex;

    QFuture<void> sqlfuture;
    QFutureWatcher<void> sqlwatcher;
    QFuture<void> thumbfuture;
    QFutureWatcher<void> thumbwatcher;
    QFutureWatcher<void> exportwatcher;
    QFutureWatcher<void> digwatcher;

    QProcess* xmntprocess;

    QFile casesfile;
    QFile casedatafile;
    off_t offset() const;
    QFile testfile;
    QPushButton* lineup;
    QPushButton* linedown;
    QPushButton* pageup;
    QPushButton* pagedown;
    QLabel* selectedoffset;
    QLabel* selectedhex;
    QLabel* filecountlabel;
    QLabel* filtercountlabel;
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
    TreeNode* actionitem;
    QStringList listeditems;
    QWidget* cancelwidget;

    // outside in variables
    SCCERR oiiniterr;
    SCCERR oierr;
    VTHDOC oidoc;
    VTHEXPORT oiexport;
};

SCCERR ExportCallback(VTHEXPORT hExport, VTSYSPARAM dwCallbackData, VTDWORD dwCommandID, VTLPVOID pCommandData);
void SetOptionDWORD(VTHDOC target, VTDWORD optionId, VTDWORD val);
void SetOptionBOOL(VTHDOC target, VTDWORD optionId, VTBOOL val);
void SetOptionString(VTHDOC target, VTDWORD optionId, const char* val);

#endif // WOMBATFORENSICS_H
