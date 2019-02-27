#ifndef GLOBALS_H
#define GLOBALS_H

#include "wombatinclude.h"
#include "wombatvariable.h"

// Copyright 2015 Pasquale J. Rinaldi, Jr.
// Distrubted under the terms of the GNU General Public License version 2

extern WombatVariable wombatvariable;
extern QFile logfile;
extern QFile viewerfile;
extern QFile settingsfile;
extern QFile treefile;
extern QDir thumbdir;
extern QDir currentrawimagedir;
extern QTextEdit* msglog;
extern QFutureWatcher<void> thumbwatcher;
extern QFutureWatcher<void> secondwatcher;
extern unsigned long long filesfound;
extern unsigned long long fileschecked;
extern unsigned long long processphase;
extern unsigned long long totalcount;
extern unsigned long long totalchecked;
extern unsigned long long exportcount;
extern unsigned long long digcount;
extern unsigned long long errorcount;
extern unsigned long long jumpoffset;
extern unsigned long long filejumpoffset;
extern unsigned long long orphancount;
//extern int partint;
extern int ecount;
//extern int volcnt;
extern int childcount;
extern int linefactor;
extern int filelinefactor;
extern int thumbsize;
extern int mftrecordsize;
extern int hashsum;
extern QList<QVariant> colvalues;
extern QStringList propertylist;
extern QStringList thumblist;
extern QStringList thumbpathlist;
extern QStringList exportlist;
extern QStringList digfilelist;
extern QStringList listeditems;
extern QStringList evidencelist;
extern QString exportpath;
extern bool originalpath;
extern QString blockstring;
extern QString thumbpath;
extern QString hexselection;
extern QString hexstring;
extern QHash<QString, bool> checkhash;
extern QHash<QString, QString> imageshash;
extern QMutex mutex;

struct dosdate
{
    int dyear;
    char dday;
    char dmon;
};

struct dostime
{
    unsigned char dmin;
    unsigned char dhour;
    unsigned char dhund;
    unsigned char dsec;
};

struct FilterValues
{
    QString idfilter;
    unsigned long long maxid;
    unsigned long long minid;
    bool namebool;
    QString namefilter;
    bool pathbool;
    QString pathfilter;
    bool maxsizebool;
    bool minsizebool;
    unsigned long long maxsize;
    unsigned long long minsize;
    bool maxcreatebool;
    bool mincreatebool;
    int maxcreate;
    int mincreate;
    bool maxaccessbool;
    bool minaccessbool;
    int maxaccess;
    int minaccess;
    bool maxmodifybool;
    bool minmodifybool;
    int maxmodify;
    int minmodify;
    bool maxchangebool;
    bool minchangebool;
    int maxchange;
    int minchange;
    bool filecategorybool;
    bool filetypebool;
    QString filecategory;
    QString filetype;
    QString filegroup;
    bool filegroupbool;
    bool hashbool;
    bool hashbool2;
    int hashdupcnt;
    QString hashfilter;
    QStringList hashlist;
};

extern FilterValues filtervalues;
class InterfaceSignals : public QObject
{
    Q_OBJECT
public:
    InterfaceSignals() { };
    ~InterfaceSignals() { };

    void ProgUpd(void) { emit(ProgressUpdate(filesfound)); }
    void ExportUpd(void) { emit(ExportUpdate()); }
    void DigUpd(void) { emit(DigUpdate()); }

signals:
    void ProgressUpdate(unsigned long long filecount);
    void ExportUpdate(void);
    void DigUpdate(void);

};

extern InterfaceSignals* isignals;


class TreeNode
{
public:
    explicit TreeNode(const QList<QVariant> &data, TreeNode* parent = 0, int itype = -1)
    {
        parentitem = parent;
        itemdata = data;
        itemtype = itype;
    };

    ~TreeNode()
    {
        qDeleteAll(childitems);
    }

    void AppendChild(TreeNode* child)
    {
        childitems.append(child);
    };

    TreeNode* child(int row)
    {
        return childitems.value(row);
    };

    int ChildCount(void) const
    {
        return childitems.count();
    };

    int ColumnCount(void) const
    {
        return itemdata.count();
    };

    QVariant Data(int column) const
    {
        return itemdata.value(column);
    };

    int Row(void) const
    {
        if(parentitem)
            return parentitem->childitems.indexOf(const_cast<TreeNode*>(this));

        return 0;
    };

    TreeNode* ParentItem(void)
    {
        return parentitem;
    };
    
    bool IsChecked() const
    {
        return checked;
    };

    void SetChecked(bool set)
    {
        checked = set;
    };

    bool IsDeleted() const
    {
        return deleted;
    };

    void SetDeleted(bool set)
    {
        deleted = set;
    };

    bool SetData(int column, const QVariant &value)
    {
        if(column < 0 || column >= 10)
            return false;
        itemdata[column] = value;
        return true;
    }

    int itemtype;
    bool deleted = false;
    bool checked = false;

private:
    QList<TreeNode*> childitems;
    QList<QVariant> itemdata;
    TreeNode* parentitem;
};

/*
extern TSK_IMG_INFO* readimginfo;
extern TSK_VS_INFO* readvsinfo;
extern const TSK_VS_PART_INFO* readpartinfo;
extern TSK_FS_INFO* readfsinfo;
extern TSK_FS_FILE* readfileinfo;
extern char asc[512];
extern iso9660_pvd_node* p;
extern HFS_INFO* hfs;
*/

extern TreeNode* selectednode;

class TreeNodeModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit TreeNodeModel(QObject* parent = 0) : QAbstractItemModel(parent)
    {
        QList<QVariant> zerodata;
        //zerodata << "ID" << "Name" << "Full Path" << "Size (bytes)" << "Created (UTC)" << "Accessed (UTC)" << "Modified (UTC)" << "Status Changed (UTC)" << "MD5 Hash" << "File Category" << "File Signature";
        zerodata << "Name" << "Full Path" << "Size (bytes)" << "Created (UTC)" << "Accessed (UTC)" << "Modified (UTC)" << "Status Changed (UTC)" << "MD5 Hash" << "File Category" << "File Signature" << "ID"; // NAME IN FIRST COLUMN
        zeronode = new TreeNode(zerodata);
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
        //nodetype = itemnode->Data(0).toString().split("-a").first().split("-").count();
        nodetype = itemnode->Data(10).toString().split("-a").first().split("-").count();
        itemtype = itemnode->itemtype; // node type 1=file, 2=dir, 10=vir file, 11=vir dir, -1=not file (evid image, vol, part, fs)

        if(role == Qt::CheckStateRole && index.column() == 0)
            return static_cast<int>(itemnode->IsChecked() ? Qt::Checked : Qt::Unchecked);
        else if(role == Qt::ForegroundRole)
        {
            if(nodetype < 4)
                return QColor(Qt::darkBlue);
            else if(nodetype == 4)
            {
                if(itemnode->Data(10).toString().contains(filtervalues.idfilter) == false)
                    return QColor(Qt::lightGray);
                if(filtervalues.namebool)
                {
                    ba.clear();
                    ba.append(itemnode->Data(0).toString());
                    if(QString(QByteArray::fromBase64(ba)).contains(filtervalues.namefilter) == false)
                        return QColor(Qt::lightGray);
                }
                if(filtervalues.pathbool)
                {
                    ba.clear();
                    ba.append(itemnode->Data(1).toString());
                    if(QString(QByteArray::fromBase64(ba)).contains(filtervalues.pathfilter) == false)
                        return QColor(Qt::lightGray);
                }
                if(filtervalues.maxsizebool && filtervalues.minsizebool == false)
                {
                    if(itemnode->Data(2).toULongLong() <= filtervalues.maxsize)
                        return QColor(Qt::lightGray);
                }
                if(filtervalues.minsizebool && filtervalues.maxsizebool == false)
                {
                    if(itemnode->Data(2).toULongLong() >= filtervalues.minsize)
                        return QColor(Qt::lightGray);
                }
                if(filtervalues.maxsizebool && filtervalues.minsizebool)
                {
                    if(itemnode->Data(2).toULongLong() >= filtervalues.minsize || itemnode->Data(2).toULongLong() <= filtervalues.maxsize)
                        return QColor(Qt::lightGray);
                }
                if(filtervalues.maxcreatebool && filtervalues.mincreatebool == false)
                {
                    if(itemnode->Data(3).toInt() <= filtervalues.maxcreate)
                        return QColor(Qt::lightGray);
                }
                if(filtervalues.maxcreatebool == false && filtervalues.mincreatebool)
                {
                    if(itemnode->Data(3).toInt() >= filtervalues.mincreate)
                        return QColor(Qt::lightGray);
                }
                if(filtervalues.maxcreatebool && filtervalues.mincreatebool)
                {
                    if(itemnode->Data(3).toInt() >= filtervalues.mincreate || itemnode->Data(3).toInt() <= filtervalues.maxcreate)
                        return QColor(Qt::lightGray);
                }
                if(filtervalues.maxaccessbool && filtervalues.minaccessbool == false)
                {
                    if(itemnode->Data(4).toInt() <= filtervalues.maxaccess)
                        return QColor(Qt::lightGray);
                }
                if(filtervalues.maxaccessbool == false && filtervalues.minaccessbool)
                {
                    if(itemnode->Data(4).toInt() >= filtervalues.minaccess)
                        return QColor(Qt::lightGray);
                }
                if(filtervalues.maxaccessbool && filtervalues.minaccessbool)
                {
                    if(itemnode->Data(4).toInt() >= filtervalues.minaccess || itemnode->Data(4).toInt() <= filtervalues.maxaccess)
                        return QColor(Qt::lightGray);
                }
                if(filtervalues.maxmodifybool && filtervalues.minmodifybool == false)
                {
                    if(itemnode->Data(5).toInt() <= filtervalues.maxmodify)
                        return QColor(Qt::lightGray);
                }
                if(filtervalues.maxmodifybool == false && filtervalues.minmodifybool)
                {
                    if(itemnode->Data(5).toInt() >= filtervalues.minmodify)
                        return QColor(Qt::lightGray);
                }
                if(filtervalues.maxmodifybool && filtervalues.minmodifybool)
                {
                    if(itemnode->Data(5).toInt() >= filtervalues.minmodify || itemnode->Data(5).toInt() <= filtervalues.maxmodify)
                        return QColor(Qt::lightGray);
                }
                if(filtervalues.maxchangebool && filtervalues.minchangebool == false)
                {
                    if(itemnode->Data(6).toInt() <= filtervalues.maxchange)
                        return QColor(Qt::lightGray);
                }
                if(filtervalues.maxchangebool == false && filtervalues.minchangebool)
                {
                    if(itemnode->Data(6).toInt() >= filtervalues.minchange)
                        return QColor(Qt::lightGray);
                }
                if(filtervalues.maxchangebool && filtervalues.minchangebool)
                {
                    if(itemnode->Data(6).toInt() >= filtervalues.minchange || itemnode->Data(6).toInt() <= filtervalues.maxchange)
                        return QColor(Qt::lightGray);
                }
                if(filtervalues.hashbool)
                {
                    for(int i = 0; i < filtervalues.hashlist.count(); i++)
                    {
                        if(itemnode->Data(7).toString().compare(filtervalues.hashlist.at(i)) == 0)
                            return QColor(Qt::lightGray);
                    }
                }
                if(filtervalues.hashbool2)
                {
                    if(itemnode->Data(7).toString().contains(filtervalues.hashfilter, Qt::CaseInsensitive) == false)
                        return QColor(Qt::lightGray);
                }
                if(filtervalues.filegroupbool)
                {
                    if(itemnode->Data(8).toString().contains(filtervalues.filegroup) == false)
                        return QColor(Qt::lightGray);
                }
                if(filtervalues.filetypebool)
                {
                    if(itemnode->Data(9).toString().contains(filtervalues.filetype) == false)
                            return QColor(Qt::lightGray);
                }
            }
        }
        else if(role == Qt::DisplayRole)
        {
            if(index.column() == 10) // used to be 0
            {
                return itemnode->Data(index.column()).toString().split("-a").at(0);
            }
            else if(index.column() == 0 || index.column() == 1) // used to be 1 || 2
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
            else if(index.column() == 2) // used to be 3
                return itemnode->Data(index.column());
            else if(index.column() >= 3 && index.column() <= 6) // used to be >= 4 <= 7
            {
                if(itemnode->Data(index.column()).toString().compare("0") == 0)
                    return "";
                else
                {
                    char* ibuffer = new char[128];
                    time_t tmptime = itemnode->Data(index.column()).toInt();
                    if(tmptime <= 0)
                        strncpy(ibuffer, "", 128);
                    else
                    {
                        struct tm *tmTime = gmtime(&tmptime);
                        snprintf(ibuffer, 128, "%.4d-%.2d-%.2d %.2d:%.2d:%.2d", (int) tmTime->tm_year + 1900, (int) tmTime->tm_mon + 1, (int) tmTime->tm_mday, tmTime->tm_hour, (int) tmTime->tm_min, (int) tmTime->tm_sec);//, tzname[(tmTime->tm_isdst == 0) ? 0 : 1]);
                    }
                    QString tmpstr = QString(ibuffer);
                    delete[] ibuffer;
                    return tmpstr;
                }
            }
            else if(index.column() >= 7 && index.column() <= 9) // used to be >= 8 <= 10
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
            ba.append(itemnode->Data(0).toString()); // used to be 1
            QString nodename = QByteArray::fromBase64(ba);
            //qDebug() << itemnode->Data(0).toString() << nodename << "nodetype:" << nodetype << "itemtype:" << itemtype;
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
                    if((itemtype == 0 && itemnode->Data(1).toString().contains("/$OrphanFiles/")) || itemtype == 1) // used to be (2)
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
                    {
                        if(itemnode->Data(10).toString().contains("f*")) // used to be 0
                            return QIcon(QPixmap(QString(":/basic/deletedfile")));
                        else
                            return QIcon(QPixmap(QString(":/basic/treefile")));
                    }
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
                checkhash.insert(itemnode->Data(10).toString().split("-a").first(), false); // used to be 0
            }
            else
            {
                itemnode->SetChecked(true);
                checkhash.insert(itemnode->Data(10).toString().split("-a").first(), true); // used to be 0
            }
            emit dataChanged(index, index);
            emit CheckedNodesChanged();
            return true;
        }

        if(role != Qt::EditRole)
            return false;
        if(role == Qt::EditRole)
        {
            bool result = itemnode->SetData(index.column(), value);
            if(result)
                emit dataChanged(index, index, {role});
            return result;
        }
        return false;
    };

    bool setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role)
    {
        if(role != Qt::EditRole || orientation != Qt::Horizontal)
            return false;
        bool result = zeronode->SetData(section, value);
        if(result)
            emit headerDataChanged(orientation, section, section);
        return result;
    };

    Qt::ItemFlags flags(const QModelIndex &index) const override
    {
        Qt::ItemFlags flags = QAbstractItemModel::flags(index);
        TreeNode* itemnode = static_cast<TreeNode*>(index.internalPointer());

        if(!index.isValid())
            return Qt::NoItemFlags;
        if(index == QModelIndex())
            return Qt::NoItemFlags;
        if(index.column() == 10 && itemnode->Data(index.column()).toString().split("-a").first().split("-").count() == 4) // used to be 0
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
            if(section == 10 && (!filtervalues.idfilter.isEmpty() && !filtervalues.idfilter.isNull()))
                return QIcon(QPixmap(QString(":/basic/filterimg")));
            if(section == 0 && filtervalues.namebool)
                return QIcon(QPixmap(QString(":/basic/filterimg")));
            if(section == 1 && filtervalues.pathbool)
                return QIcon(QPixmap(QString(":/basic/filterimg")));
            if(section == 2 && filtervalues.maxsizebool)
                return QIcon(QPixmap(QString(":/basic/filterimg")));
            if(section == 2 && filtervalues.minsizebool)
                return QIcon(QPixmap(QString(":/basic/filterimg")));
            if(section == 3 && (filtervalues.maxcreatebool || filtervalues.mincreatebool))
                return QIcon(QPixmap(QString(":/basic/filterimg")));
            if(section == 4 && (filtervalues.maxaccessbool || filtervalues.minaccessbool))
                return QIcon(QPixmap(QString(":/basic/filterimg")));
            if(section == 5 && (filtervalues.maxmodifybool || filtervalues.minmodifybool))
                return QIcon(QPixmap(QString(":/basic/filterimg")));
            if(section == 6 && (filtervalues.maxchangebool || filtervalues.minchangebool))
                return QIcon(QPixmap(QString(":/basic/filterimg")));
            if(section == 7 && (filtervalues.hashbool || filtervalues.hashbool2))
                return QIcon(QPixmap(QString(":/basic/filterimg")));
            if(section == 8 && filtervalues.filegroupbool)
                return QIcon(QPixmap(QString(":/basic/filterimg")));
            if(section == 9 && filtervalues.filetypebool)
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
        QString rootinum = "";
        QString tmpstr = "";
        for(int i=0; i < nodes.count(); i++)
        {
            if(nodes.at(i).length() > 0)
            {
                QDir eviddir = QDir(wombatvariable.tmpmntpath + nodes.at(i));
                QFile evidfile(wombatvariable.tmpmntpath + nodes.at(i) + "/stat");
                evidfile.open(QIODevice::ReadOnly | QIODevice::Text);
                if(evidfile.isOpen())
                    tmpstr = evidfile.readLine();
                evidfile.close();
                QList<QVariant> columndata;
                columndata.clear();
                if(tmpstr.split(",").count() > 5)
                {
                    columndata << tmpstr.split(",").at(3).split("/").last() << "0" << tmpstr.split(",").at(1) << "0" << "0" << "0" << "0" << "0" << "0" << "0" << tmpstr.split(",").at(5);
                    //columndata << tmpstr.split(",").at(5) << tmpstr.split(",").at(3).split("/").last() << "0" << tmpstr.split(",").at(1) << "0" << "0" << "0" << "0" << "0" << "0" << "0";
                    parent->AppendChild(new TreeNode(columndata, parent));
                    curid = tmpstr.split(",").at(5);
                    parents[curid] = parent->child(parent->ChildCount() - 1);
                }
                QStringList vollist = eviddir.entryList(QDir::NoSymLinks | QDir::NoDotAndDotDot | QDir::AllDirs | QDir::Hidden);
                for(int j=0; j < vollist.count(); j++)
                {
                    columndata.clear();
                    QFile volfile(wombatvariable.tmpmntpath + nodes.at(i) + "/" + vollist.at(j) + "/stat");
                    volfile.open(QIODevice::ReadOnly | QIODevice::Text);
                    if(volfile.isOpen())
                        tmpstr = volfile.readLine();
                    volfile.close();
                    if(tmpstr.split(",").count() > 5)
                    {
                        columndata << tmpstr.split(",").at(2) << "0" << tmpstr.split(",").at(1) << "0" << "0" << "0" << "0" << "0" << "0" << "0" << tmpstr.split(",").at(5);
                        //columndata << tmpstr.split(",").at(5) << tmpstr.split(",").at(2) << "0" << tmpstr.split(",").at(1) << "0" << "0" << "0" << "0" << "0" << "0" << "0";
                        parid = tmpstr.split(",").at(5).split("-").at(0);
                        curid = tmpstr.split(",").at(5);
                        parents.value(parid)->AppendChild(new TreeNode(columndata, parents.value(parid)));
                        parents[curid] = parents.value(parid)->child(parents.value(parid)->ChildCount() - 1);
                    }
                    QDir voldir = QDir(wombatvariable.tmpmntpath + nodes.at(i) + "/" + vollist.at(j));
                    QStringList partlist = voldir.entryList(QDir::NoSymLinks | QDir::NoDotAndDotDot | QDir::AllDirs | QDir::Hidden);
                    for(int k = 0; k < partlist.count(); k++)
                    {
                        columndata.clear();
                        QFile partfile(wombatvariable.tmpmntpath + nodes.at(i) + "/" + vollist.at(j) + "/" + partlist.at(k) + "/stat");
                        partfile.open(QIODevice::ReadOnly | QIODevice::Text);
                        if(partfile.isOpen())
                            tmpstr = partfile.readLine();
                        partfile.close();
                        rootinum = tmpstr.split(",").at(3);
                        if(tmpstr.split(",").count() > 10)
                        {
                            columndata << tmpstr.split(",").at(2) << "0" << tmpstr.split(",").at(1) << "0" << "0" << "0" << "0" << "0" << "0" << "0" << tmpstr.split(",").at(10);
                            //columndata << tmpstr.split(",").at(10) << tmpstr.split(",").at(2) << "0" << tmpstr.split(",").at(1) << "0" << "0" << "0" << "0" << "0" << "0" << "0";
                            parid = tmpstr.split(",").at(10).split("-p").at(0);
                            curid = tmpstr.split(",").at(10);
                            parents.value(parid)->AppendChild(new TreeNode(columndata, parents.value(parid)));
                            parents[curid] = parents.value(parid)->child(parents.value(parid)->ChildCount() - 1);
                        }
                        QDir partdir = QDir(wombatvariable.tmpmntpath + nodes.at(i) + "/" + vollist.at(j) + "/" + partlist.at(k));
                        QStringList rootlist = partdir.entryList(QDir::NoSymLinks | QDir::NoDotAndDotDot | QDir::AllDirs | QDir::Hidden);
                        for(int l = 0; l < rootlist.count(); l++)
                        {
                            columndata.clear();
                            QFile filefile(wombatvariable.tmpmntpath + nodes.at(i) + "/" + vollist.at(j) + "/" + partlist.at(k) + "/" + rootlist.at(l) + "/stat");
                            filefile.open(QIODevice::ReadOnly | QIODevice::Text);
                            if(filefile.isOpen())
                                tmpstr = filefile.readLine();
                            filefile.close();
                            if(tmpstr.split(",").count() > 12)
                            {
                                columndata << tmpstr.split(",").at(0) << tmpstr.split(",").at(3) << tmpstr.split(",").at(8) << tmpstr.split(",").at(6) << tmpstr.split(",").at(7) << tmpstr.split(",").at(4) << tmpstr.split(",").at(5) << tmpstr.split(",").at(13) << tmpstr.split(",").at(10).split("/").at(0) << tmpstr.split(",").at(10).split("/").at(1) << tmpstr.split(",").at(12);
                                //columndata << tmpstr.split(",").at(12) << tmpstr.split(",").at(0) << tmpstr.split(",").at(3) << tmpstr.split(",").at(8) << tmpstr.split(",").at(6) << tmpstr.split(",").at(7) << tmpstr.split(",").at(4) << tmpstr.split(",").at(5) << tmpstr.split(",").at(13) << tmpstr.split(",").at(10).split("/").at(0) << tmpstr.split(",").at(10).split("/").at(1);
                                if(tmpstr.split(",").at(2).toInt() == rootinum.toInt())
                                    parid = tmpstr.split(",").at(12).split("-f").at(0);
                                else
                                    parid = tmpstr.split(",").at(12).split("-f").at(0) + "-f" + tmpstr.split(",").at(2);
                                curid = tmpstr.split(",").at(12).split("-a").at(0);
                                qDebug() << "par-cur:" << parid << curid;
                                parents.value(parid)->AppendChild(new TreeNode(columndata, parents.value(parid), tmpstr.split(",").at(1).toInt()));
                                parents[curid] = parents.value(parid)->child(parents.value(parid)->ChildCount() - 1);
                                if(checkhash.contains(tmpstr.split(",").at(12).split("-a").first()))
                                    parents.value(curid)->SetChecked(true);
                                if(tmpstr.split(",").at(14).toInt() == true)
                                    parents.value(curid)->SetDeleted(true);
                            }
                        }
                    }
                }
            }
        }
    };

public:
    void AddNode(QList<QVariant> data, QString parid, int type, int deleted)
    {
        //qDebug() << "id" << data.at(0).toString().split("-a").first();
        if(parid.toInt() == -1) // evid
        {
            zeronode->AppendChild(new TreeNode(data, zeronode));
            parents[data.at(10).toString()] = zeronode->child(zeronode->ChildCount() - 1); // USED TO BE 0
        }
        else // everything else 
        {
            if(type == -1)
                parents.value(parid)->AppendChild(new TreeNode(data, parents.value(parid)));
            else
                parents.value(parid)->AppendChild(new TreeNode(data, parents.value(parid), type));
            parents[data.at(10).toString().split("-a").first()] = parents.value(parid)->child(parents.value(parid)->ChildCount() - 1); // USED TO BE 0
            if(checkhash.contains(data.at(10).toString().split("-a").first())) // USED TO BE 0
                parents.value(data.at(10).toString().split("-a").first())->SetChecked(true); // USED TO BE 0
            if(deleted == 1)
                parents.value(data.at(10).toString().split("-a").first())->SetDeleted(true); // USED TO BE 0
        }
    };
    void UpdateNode(QString itemid, int column, QString hash)
    {
        const QVariant tmpvar(hash);
        parents.value(itemid.split("-a").first())->SetData(column, tmpvar);
        //parents.value(data.at(0).toString().split("-a").first())->SetData(column, hash);
    };
    
    void UpdateHeaderNode(int column, QString hash)
    {
        // SHOULD BE EITHER MD5 HASH, SHA1 HASH, SHA256 HASH
        //zeronode->SetData(column, hash);
    };

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

extern TreeNodeModel* treenodemodel;

#endif
