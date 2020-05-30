#ifndef GLOBALS_H
#define GLOBALS_H

#include "wombatinclude.h"
#include "wombatvariable.h"

// Copyright 2013-2020 Pasquale J. Rinaldi, Jr.
// Distrubted under the terms of the GNU General Public License version 2

extern WombatVariable wombatvariable; // contains useful application variables
extern QFile logfile; // controls the logfile
extern QFile viewerfile; // controls the viewer file and its values for external viewers
extern QFile settingsfile; // controls the settings
extern QFile previewfile; // controls the preview html file for the analysis results
extern QFile bookmarkfile; // controls the tags
extern QDir thumbdir; // controls the thumbs/ directory
extern QTextEdit* msglog; // holds the msglog variable to write messages to the log
extern qint64 filesfound; // holds the number of files found for all the evidence.
extern qint64 fileschecked; // holds humber of files checked for a case across all evidence
extern qint64 totalcount; // used for dig deeper and export dialogs
extern qint64 totalchecked; // used for dig deeper and export dialogs
extern qint64 exportcount; // used for exporting files
extern qint64 dighashcount; // used for hashing files with dig deeper
extern qint64 digimgthumbcount; // used for generating image thumbnails
extern qint64 digvidthumbcount; // used for generating video thumbnails
extern qint64 digarchivecount; // used for expanding archives
extern qint64 errorcount; // export error count variable
extern qint64 jumpoffset; // stores offset hex value when the user wants to jump to that offset
extern qint64 filejumpoffset; // stores offset hex value when the user wants to jump to that offset in the file hexviewer
extern qint64 orphancount; // keeps track of unique id # for orphan files
extern qint64 carvedcount; // keeps track of unique id # for carved files
extern int ecount; // evidence count variable
extern int thumbsize; // thumbnail size settings variable
extern int vidcount; // number of video thumbnails to generate
extern int autosave; // minutes between automatically saving current state
extern QByteArray reporttimezone; // selected timezone to format the report in...
extern int mftrecordsize; // contains the size of an mft record, usually always 1024
extern int hashsum; // type of hash used: either md5, sha1, sha256
extern int idcol; // contains the position for the ID column
extern QStringList propertylist; // contains properties list values
extern QStringList exportlist; // contains list of files to be exported
extern QStringList digfilelist; // contains list of files to dig
extern QStringList listeditems; // contains checked or all list of files for a case
extern QStringList existingevidence; // contains the list of existing evidence
extern QStringList newevidence; // contains the list of new evidence items
extern QStringList partitionlist; // contains the list of partitions for file carving
extern QString exportpath; // path to export files to
extern bool originalpath; // whether to include original file path or not
extern bool hashash; // whether we are calculating the hash for the item
extern bool hasvid; // whether we are calculating the vid thumbnail for the item
extern bool hasimg; // whether we are calculating the img thumbnail for the item
extern bool hasarchive; // whether we are calculating the archive for the item
extern bool isclosing; // whether the application is closing and threads should not write back to the application
extern QString genthmbpath; // global path to place thumbs for each operationgal call of generate thumbnails (mntpt/thumbs or rep/thmb)
extern QString casepath; // case path settings variable
extern QString reportpath; // report path settings variable
extern QString hexstring; // tmp hex file filename with path
extern QString hfsreserveinfo; // hfsreserveinfo file which contains the info for catalog, attribute, allocation, extents, and startup
extern QHash<QString, bool> checkhash; // hash value list for check boxes.
extern QHash<QString, QString> imageshash; // list of thumbnail ids, paths
extern QHash<QString, QString> taggedhash; // list of tagged files: [id], tag
extern QHash<QString, QString> hashlist; // list of file hashes: ids, hash
extern QHash<QString, QString> mftblockhash; // list of mft blockstrings for each partition for each evidence item
extern QHash<QString, QString> passwordhash; // list of passwords for encrypted pool volume's/fs's
extern QMutex mutex; // mutex so my code will work when multithreaded (mainly for adding treeview nodes)

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
    qint64 maxid;
    qint64 minid;
    bool namebool;
    QString namefilter;
    bool pathbool;
    QString pathfilter;
    bool maxsizebool;
    bool minsizebool;
    qint64 maxsize;
    qint64 minsize;
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
    bool tagbool;
    QString tag;
};

extern FilterValues filtervalues;

struct EvidenceReportData
{
    int evidid;
    QString evidname;
    QString evidcontent;
};

extern QList<EvidenceReportData> evidrepdatalist;

class InterfaceSignals : public QObject
{
    Q_OBJECT
public:
    InterfaceSignals() { };
    ~InterfaceSignals() { };

    void ProgUpd(void) { emit(ProgressUpdate(filesfound)); }
    void ExportUpd(void) { emit(ExportUpdate()); }
    void DigUpd(int digid, int digcnt) { emit(DigUpdate(digid, digcnt)); }
    void ActivateReload(void) { emit(ReloadPreview()); }

signals:
    void ProgressUpdate(qint64 filecount);
    void ExportUpdate(void);
    void DigUpdate(int digid, int digcnt);
    void ReloadPreview(void);

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
        if(column < 0 || column >= 12)
            return false;
        itemdata[column] = value;
        return true;
    }

    bool RemoveChildren(int position, int count)
    {
        if(position < 0 || position + count > parentitem->childitems.count())
            return false;
        for(int i=0; i < count; ++i)
            delete parentitem->childitems.takeAt(position);

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

extern TreeNode* selectednode;

class TreeNodeModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit TreeNodeModel(QObject* parent = 0) : QAbstractItemModel(parent)
    {
        QList<QVariant> zerodata;
        zerodata << "Name" << "Full Path" << "Size (bytes)" << "Created (UTC)" << "Accessed (UTC)" << "Modified (UTC)" << "Status Changed (UTC)" << "MD5 Hash" << "File Category" << "File Signature" << "Tagged" << "ID"; // NAME IN FIRST COLUMN
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

        TreeNode* itemnode = static_cast<TreeNode*>(index.internalPointer());
        int nodetype = 0;
        int itemtype = 0;
        QByteArray ba;
        nodetype = itemnode->Data(11).toString().split("-").count();
        itemtype = itemnode->itemtype; // node type 5=file, 3=dir, 10=vir file, 11=vir dir, -1=not file (evid image, vol, part, fs), 15=carved file
        if(role == Qt::CheckStateRole && index.column() == 11)
            return static_cast<int>(itemnode->IsChecked() ? Qt::Checked : Qt::Unchecked);
        else if(role == Qt::ForegroundRole)
        {
            if(nodetype == 5 || (nodetype == 2 && itemnode->Data(11).toString().contains("-c")) || (nodetype == 2 && itemnode->Data(11).toString().contains("-mc")))
            {
                if(itemnode->Data(11).toString().contains(filtervalues.idfilter) == false)
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
                    if(itemnode->Data(2).toLongLong() <= filtervalues.maxsize)
                        return QColor(Qt::lightGray);
                }
                if(filtervalues.minsizebool && filtervalues.maxsizebool == false)
                {
                    if(itemnode->Data(2).toLongLong() >= filtervalues.minsize)
                        return QColor(Qt::lightGray);
                }
                if(filtervalues.maxsizebool && filtervalues.minsizebool)
                {
                    if(itemnode->Data(2).toLongLong() >= filtervalues.minsize || itemnode->Data(2).toLongLong() <= filtervalues.maxsize)
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
                if(filtervalues.tagbool)
                {
                    if(itemnode->Data(10).toString().compare(filtervalues.tag) != 0)
                        return QColor(Qt::lightGray);
                }
            }
            else if(nodetype < 4)
                return QColor(Qt::darkBlue);
        }
        else if(role == Qt::DisplayRole)
        {
            if(index.column() == 0 || index.column() == 1) // used to be 1 || 2
            {
                if(nodetype == 5 || nodetype == 4 || (nodetype == 2 && itemnode->Data(11).toString().contains("-c")))
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
                else // surely i can qt this up....
                {
                    QDateTime tmpdt = QDateTime::fromSecsSinceEpoch(itemnode->Data(index.column()).toInt(), QTimeZone::utc());
                    QString tmpstr = tmpdt.toString("MM/dd/yyyy hh:mm:ss AP");
                    return tmpstr;
                }
            }
            else if(index.column() >= 7 && index.column() <= 10) // used to be >= 8 <= 10 // now includes tagged column
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
            if(index.column() == 0)
            {
                if(nodetype == 1)
                    return QIcon(":/basic/treeimg");
                else if(nodetype == 2)
                {
                    if(itemnode->Data(11).toString().contains("-c"))
                        return QIcon(":/basic/carveicon");
                    else if(itemnode->Data(11).toString().contains("-mc"))
                        return QIcon(":/basic/virtualfolder");
                    else
                        return QIcon(":/basic/treevol");
                }
                else if(nodetype == 3)
                    return QIcon(":/basic/treefs");
                else if(nodetype == 4)
                {
                    if(itemtype == 3)
                        return QIcon(":/basic/virtualfolder");
                    return QIcon(":/basic/carveicon");
                    /*
                    if((itemtype == 0 && nodename.contains("$OrphanFiles")) || itemtype == 5) // used to be (2)
                    {
                        if(nodename.compare("AttrDef") == 0 || nodename.compare("$BadClus") == 0 || nodename.compare("$Bitmap") == 0 || nodename.compare("$Boot") == 0 || nodename.compare("$ObjId") == 0 || nodename.compare("$Quota") == 0 || nodename.compare("$Reparse") == 0 || nodename.compare("$LogFile") == 0 || nodename.compare("$MFT") == 0 || nodename.compare("$MFTMirr") == 0 || nodename.compare("$Secure") == 0 || nodename.compare("$UpCase") == 0 || nodename.compare("$Volume") == 0)
                            return QIcon(":/basic/virtualfile");
                        else if(nodename.compare("$OprhanFiles"))
                            return QIcon(":/basic/virtualfolder");
                        else
                        {
                            if(itemnode->IsDeleted()) // Unallocated and Used
                                return QIcon(":/basic/deletedfile");
                            else
                                return QIcon(":/basic/treefile");
                        }
                    }
                    else if(itemtype == 3)
                    {
                        if(nodename.compare("$OrphanFiles") == 0 || nodename.compare("$Extend") == 0 || nodename.compare("$Carved") == 0)
                            return QIcon(":/basic/virtualfolder");
                        else
                            return QIcon(":/basic/treefolder");
                    }
                    else if(itemtype == 10)
                        return QIcon(":/basic/virtualfile");
                    else if(itemtype == 11)
                        return QIcon(":/basic/virtualdir");
                    else if(itemtype == 15)
                        return QIcon(":/basic/carveicon");
                    else
                    {
                        if(itemnode->Data(11).toString().contains("fd") || itemnode->Data(11).toString().contains("fo")) // used to be 0
                            return QIcon(":/basic/deletedfile");
                        else
                            return QIcon(":/basic/treefile");
                    }*/
                }
                else if(nodetype == 5)
                {
                    if(nodename.startsWith("$"))
                    {
                        if(itemtype == 3)
                            return QIcon(":/basic/virtualdir");
                        else if(nodename.contains("$OrphanFiles"))
                            return QIcon(":/basic/virtualdir");
                        else if(itemtype == 5)
                            return QIcon(":/basic/virtualfile");
                        else
                            return QIcon(":/basic/treefile");
                    }
                    else if(itemtype == 3)
                        return QIcon(":/basic/treefolder");
                    else if(itemtype == 5)
                        return QIcon(":/basic/treefile");
                    else if(itemtype == 10)
                        return QIcon(":/basic/virtualfile");
                    else if(itemtype == 11)
                        return QIcon(":/basic/virtualdir");
                    else if(itemtype == 15)
                        return QIcon(":/basic/carveicon");
                    else
                    {
                        if(itemnode->Data(11).toString().contains("fd") || itemnode->Data(11).toString().contains("fo"))
                            return QIcon(":/basic/deletedfile");
                        else
                            return QIcon(":/basic/treefile");
                    }
                }
                else
                    return QIcon(":/basic/treefile");
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
                checkhash.insert(itemnode->Data(11).toString(), false); // used to be 0
            }
            else
            {
                itemnode->SetChecked(true);
                checkhash.insert(itemnode->Data(11).toString(), true); // used to be 0
            }
            //qDebug() << "checkhash:" << checkhash;
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
        if(role == Qt::EditRole)
        {
            //qDebug() << "edit role for header...";
            bool result = zeronode->SetData(section, value);
            if(result)
                emit headerDataChanged(orientation, section, section);
            return result;
        }
        return false;
    };

    bool removeRows(int position, int rows, const QModelIndex &parent = QModelIndex()) override
    {
        //qDebug() << "pos:" << position << "rows:" << rows;
        TreeNode* parentnode = static_cast<TreeNode*>(parent.internalPointer());
        bool success = true;
        beginRemoveRows(parent, position, position + rows - 1);
        success = parentnode->RemoveChildren(position, rows);
        endRemoveRows();

        return success;
    };

    Qt::ItemFlags flags(const QModelIndex &index) const override
    {
        Qt::ItemFlags flags = QAbstractItemModel::flags(index);
        TreeNode* itemnode = static_cast<TreeNode*>(index.internalPointer());

        if(!index.isValid())
            return Qt::NoItemFlags;
        if(index == QModelIndex())
            return Qt::NoItemFlags;
        if(index.column() == 11 && itemnode->Data(index.column()).toString().split("-a").first().split("-").count() == 4) // used to be 0
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
            if(section == 11 && (!filtervalues.idfilter.isEmpty() && !filtervalues.idfilter.isNull()))
                return QIcon(":/basic/filterimg");
            if(section == 0 && filtervalues.namebool)
                return QIcon(":/basic/filterimg");
            if(section == 1 && filtervalues.pathbool)
                return QIcon(":/basic/filterimg");
            if(section == 2 && filtervalues.maxsizebool)
                return QIcon(":/basic/filterimg");
            if(section == 2 && filtervalues.minsizebool)
                return QIcon(":/basic/filterimg");
            if(section == 3 && (filtervalues.maxcreatebool || filtervalues.mincreatebool))
                return QIcon(":/basic/filterimg");
            if(section == 4 && (filtervalues.maxaccessbool || filtervalues.minaccessbool))
                return QIcon(":/basic/filterimg");
            if(section == 5 && (filtervalues.maxmodifybool || filtervalues.minmodifybool))
                return QIcon(":/basic/filterimg");
            if(section == 6 && (filtervalues.maxchangebool || filtervalues.minchangebool))
                return QIcon(":/basic/filterimg");
            if(section == 7 && (filtervalues.hashbool || filtervalues.hashbool2))
                return QIcon(":/basic/filterimg");
            if(section == 8 && filtervalues.filegroupbool)
                return QIcon(":/basic/filterimg");
            if(section == 9 && filtervalues.filetypebool)
                return QIcon(":/basic/filterimg");
            if(section == 10 && filtervalues.tagbool)
                return QIcon(":/basic/filterimg");
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
//private:

public:
    void AddNode(QList<QVariant> data, QString parid, int type, int deleted)
    {
        if(parid.toInt() == -1) // evid
        {
            zeronode->AppendChild(new TreeNode(data, zeronode));
            parents[data.at(11).toString()] = zeronode->child(zeronode->ChildCount() - 1); // USED TO BE 0
        }
        else // everything else 
        {
            if(type == -1)
                parents.value(parid)->AppendChild(new TreeNode(data, parents.value(parid)));
            else
                parents.value(parid)->AppendChild(new TreeNode(data, parents.value(parid), type));
            parents[data.at(11).toString().split("-a").first()] = parents.value(parid)->child(parents.value(parid)->ChildCount() - 1); // USED TO BE 0
            if(checkhash.contains(data.at(11).toString())) // USED TO BE 0
                parents.value(data.at(11).toString().split("-a").first())->SetChecked(true); // USED TO BE 0
            if(deleted == 1)
                parents.value(data.at(11).toString().split("-a").first())->SetDeleted(true); // USED TO BE 0
        }
    };

    void UpdateNode(QString itemid, int column, QString hash)
    {
        const QVariant tmpvar(hash);
        parents.value(itemid.split("-a").first())->SetData(column, tmpvar);
    };
    
    void UpdateHeaderNode(int column, QString hash)
    {
        //qDebug() << "updatehadernode value:" << zeronode->Data(9).toString();
        zeronode->SetData(column, hash);
    };

    void RemEvidence()
    {
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
