#ifndef GLOBALS_H
#define GLOBALS_H

#include "wombatinclude.h"
#include "wombatvariable.h"
#include "imagefunctions.h"
//#include "ewfimage.h"

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

//extern QStringList existingevidence; // contains the list of existing evidence
//extern QStringList newevidence; // contains the list of new evidence items

extern QStringList partitionlist; // contains the list of partitions for file carving
//extern QStringList carvedtypeslist; // contains the user selected list of file carving types
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
//extern EwfImage* testimage;
//extern QList<ForensicImage*> newevid;
//extern QList<ForensicImage*> existingevid;
extern QList<ForImg*> newforimglist;
extern QList<ForImg*> existingforimglist;

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
    void CarveUpd(QString pid, int carvecnt) { emit(CarveUpdate(pid, carvecnt)); }
    void StatUp(QString status) { emit(StatUpdate(status)); }
    //void ImageUpd(void) { emit(ImageUpdate()); }
    void VerUp(qint64 size) { emit(VerifyUpdate(size)); }

signals:
    void ProgressUpdate(qint64 filecount);
    void ExportUpdate(void);
    void DigUpdate(int digid, int digcnt);
    void ReloadPreview(void);
    void CarveUpdate(QString pid, int carvecnt);
    void StatUpdate(QString status);
    void VerifyUpdate(qint64 size);
    //void ImageUpdate(void);

};

extern InterfaceSignals* isignals;


class TreeNode
{
public:
    //explicit TreeNode(const QList<QVariant> &data, TreeNode* parent = 0, int itype = -1)
    explicit TreeNode(const QHash<QString, QVariant> &data, TreeNode* parent = 0, int itype = -1)
    {
        parentitem = parent;
	itemdata.clear();
	// if i want to make the data work regardless of if the data has columns, i would have to break the below itemdata into a bunch of:
	// if(data.contains("match")) { itemdata.insert("match", data.value("match")); } else { itemdata.insert("match", "0"); }
        //columnorder << "id" << "name" << "path" << "size" << "create" << "access" << "modify" << "status" << "cat" << "sig" << "tag" << "hash" << "match";
	if(data.contains("id"))
	    itemdata.insert("id", data.value("id"));
	else
	    itemdata.insert("id", "0");
	if(data.contains("name"))
	    itemdata.insert("name", data.value("name"));
	else
	    itemdata.insert("name", "");
	if(data.contains("path"))
	    itemdata.insert("path", data.value("path"));
	else
	    itemdata.insert("path", "");
	if(data.contains("size"))
	    itemdata.insert("size", data.value("size"));
	else
	    itemdata.insert("size", 0);
	if(data.contains("create"))
	    itemdata.insert("create", data.value("create"));
	else
	    itemdata.insert("create", "0");
	if(data.contains("access"))
	    itemdata.insert("access", data.value("access"));
	else
	    itemdata.insert("access", "0");
	if(data.contains("modify"))
	    itemdata.insert("modify", data.value("modify"));
	else
	    itemdata.insert("modify", "0");
	if(data.contains("status"))
	    itemdata.insert("status", data.value("status"));
	else
	    itemdata.insert("status", "0");
	if(data.contains("cat"))
	    itemdata.insert("cat", data.value("cat"));
	else
	    itemdata.insert("cat", "0");
	if(data.contains("sig"))
	    itemdata.insert("sig", data.value("sig"));
	else
	    itemdata.insert("sig", "0");
	if(data.contains("tag"))
	    itemdata.insert("tag", data.value("tag"));
	else
	    itemdata.insert("tag", "");
	if(data.contains("hash"))
	    itemdata.insert("hash", data.value("hash"));
	else
	    itemdata.insert("hash", "");
	if(data.contains("match"))
	    itemdata.insert("match", data.value("match"));
	else
	    itemdata.insert("match", "");
        //itemdata = data;
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

    //QVariant Data(int column) const
    QVariant Data(QString column) const
    {
	if(itemdata.contains(column))
	    return itemdata.value(column);
	else
	    return QVariant("");
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

    //bool SetData(int column, const QVariant &value)
    bool SetData(QString column, const QVariant &value)
    {
        //if(column < 0 || column >= 12)
        //    return false;
        itemdata.insert(column, value);
        //itemdata[column] = value;
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
    QHash<QString, QVariant> itemdata;
    //QList<QVariant> itemdata;
    TreeNode* parentitem;
};

extern TreeNode* selectednode;

class TreeNodeModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit TreeNodeModel(QObject* parent = 0) : QAbstractItemModel(parent)
    {
        QHash<QString, QVariant> zerodata;
        zerodata.insert("name", "Name");
        zerodata.insert("path", "Full Path");
        zerodata.insert("size", "Size (bytes)");
        zerodata.insert("create", "Created (UTC)");
        zerodata.insert("access", "Accessed (UTC)");
        zerodata.insert("modify", "Modified (UTC)");
        zerodata.insert("status", "Status Changed (UTC)");
        zerodata.insert("hash", "BLAKE3 Hash");
        zerodata.insert("cat", "Category");
        zerodata.insert("sig", "Signature");
        zerodata.insert("tag", "Tagged");
        zerodata.insert("id", "ID");
        zerodata.insert("match", "Hash Match");
        zeronode = new TreeNode(zerodata);
        columnorder << "id" << "name" << "path" << "size" << "create" << "access" << "modify" << "status" << "cat" << "sig" << "tag" << "hash" << "match";
        /*
        QList<QVariant> zerodata;
        zerodata << "Name" << "Full Path" << "Size (bytes)" << "Created (UTC)" << "Accessed (UTC)" << "Modified (UTC)" << "Status Changed (UTC)" << "BLAKE3 Hash" << "File Category" << "File Signature" << "Tagged" << "ID" << "Hash Match"; // NAME IN FIRST COLUMN
        zeronode = new TreeNode(zerodata);
        */
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
        nodetype = itemnode->Data("id").toString().split("-").count();
        itemtype = itemnode->itemtype; // node type 5=file, 3=dir, 2=del-dir, 4=del-file, 10=vir file, 11=vir dir, -1=not file (evid image, vol, part, fs), 15=carved file
        if(role == Qt::CheckStateRole && index.column() == 0)
            return static_cast<int>(itemnode->IsChecked() ? Qt::Checked : Qt::Unchecked);
        else if(role == Qt::ForegroundRole)
        {
            if(nodetype == 3 || (nodetype == 2 && itemnode->Data("id").toString().contains("-c")) || (nodetype == 2 && itemnode->Data("id").toString().contains("-mc"))) // used to be 5
            {
                if(itemnode->Data("id").toString().contains(filtervalues.idfilter) == false)
                    return QColor(Qt::lightGray);
                if(filtervalues.namebool)
                {
                    ba.clear();
                    ba.append(itemnode->Data("name").toString().toUtf8());
                    if(QString(QByteArray::fromBase64(ba)).contains(filtervalues.namefilter) == false)
                        return QColor(Qt::lightGray);
                }
                if(filtervalues.pathbool)
                {
                    ba.clear();
                    ba.append(itemnode->Data("path").toString().toUtf8());
                    if(QString(QByteArray::fromBase64(ba)).contains(filtervalues.pathfilter) == false)
                        return QColor(Qt::lightGray);
                }
                if(filtervalues.maxsizebool && filtervalues.minsizebool == false)
                {
                    if(itemnode->Data("size").toLongLong() <= filtervalues.maxsize)
                        return QColor(Qt::lightGray);
                }
                if(filtervalues.minsizebool && filtervalues.maxsizebool == false)
                {
                    if(itemnode->Data("size").toLongLong() >= filtervalues.minsize)
                        return QColor(Qt::lightGray);
                }
                if(filtervalues.maxsizebool && filtervalues.minsizebool)
                {
                    if(itemnode->Data("size").toLongLong() >= filtervalues.minsize || itemnode->Data("size").toLongLong() <= filtervalues.maxsize)
                        return QColor(Qt::lightGray);
                }
                if(filtervalues.maxcreatebool && filtervalues.mincreatebool == false)
                {
                    if(itemnode->Data("create").toInt() <= filtervalues.maxcreate)
                        return QColor(Qt::lightGray);
                }
                if(filtervalues.maxcreatebool == false && filtervalues.mincreatebool)
                {
                    if(itemnode->Data("create").toInt() >= filtervalues.mincreate)
                        return QColor(Qt::lightGray);
                }
                if(filtervalues.maxcreatebool && filtervalues.mincreatebool)
                {
                    if(itemnode->Data("create").toInt() >= filtervalues.mincreate || itemnode->Data("create").toInt() <= filtervalues.maxcreate)
                        return QColor(Qt::lightGray);
                }
                if(filtervalues.maxaccessbool && filtervalues.minaccessbool == false)
                {
                    if(itemnode->Data("access").toInt() <= filtervalues.maxaccess)
                        return QColor(Qt::lightGray);
                }
                if(filtervalues.maxaccessbool == false && filtervalues.minaccessbool)
                {
                    if(itemnode->Data("access").toInt() >= filtervalues.minaccess)
                        return QColor(Qt::lightGray);
                }
                if(filtervalues.maxaccessbool && filtervalues.minaccessbool)
                {
                    if(itemnode->Data("access").toInt() >= filtervalues.minaccess || itemnode->Data("access").toInt() <= filtervalues.maxaccess)
                        return QColor(Qt::lightGray);
                }
                if(filtervalues.maxmodifybool && filtervalues.minmodifybool == false)
                {
                    if(itemnode->Data("modify").toInt() <= filtervalues.maxmodify)
                        return QColor(Qt::lightGray);
                }
                if(filtervalues.maxmodifybool == false && filtervalues.minmodifybool)
                {
                    if(itemnode->Data("modify").toInt() >= filtervalues.minmodify)
                        return QColor(Qt::lightGray);
                }
                if(filtervalues.maxmodifybool && filtervalues.minmodifybool)
                {
                    if(itemnode->Data("modify").toInt() >= filtervalues.minmodify || itemnode->Data("modify").toInt() <= filtervalues.maxmodify)
                        return QColor(Qt::lightGray);
                }
                if(filtervalues.maxchangebool && filtervalues.minchangebool == false)
                {
                    if(itemnode->Data("status").toInt() <= filtervalues.maxchange)
                        return QColor(Qt::lightGray);
                }
                if(filtervalues.maxchangebool == false && filtervalues.minchangebool)
                {
                    if(itemnode->Data("status").toInt() >= filtervalues.minchange)
                        return QColor(Qt::lightGray);
                }
                if(filtervalues.maxchangebool && filtervalues.minchangebool)
                {
                    if(itemnode->Data("status").toInt() >= filtervalues.minchange || itemnode->Data("status").toInt() <= filtervalues.maxchange)
                        return QColor(Qt::lightGray);
                }
                if(filtervalues.hashbool)
                {
                    for(int i = 0; i < filtervalues.hashlist.count(); i++)
                    {
                        if(itemnode->Data("hash").toString().compare(filtervalues.hashlist.at(i)) == 0)
                            return QColor(Qt::lightGray);
                    }
                }
                if(filtervalues.hashbool2)
                {
                    if(itemnode->Data("hash").toString().contains(filtervalues.hashfilter, Qt::CaseInsensitive) == false)
                        return QColor(Qt::lightGray);
                }
                if(filtervalues.filegroupbool)
                {
                    if(itemnode->Data("cat").toString().contains(filtervalues.filegroup) == false)
                        return QColor(Qt::lightGray);
                }
                if(filtervalues.filetypebool)
                {
                    if(itemnode->Data("sig").toString().contains(filtervalues.filetype) == false)
                            return QColor(Qt::lightGray);
                }
                if(filtervalues.tagbool)
                {
                    if(itemnode->Data("tag").toString().compare(filtervalues.tag) != 0)
                        return QColor(Qt::lightGray);
                }
            }
            else if(nodetype < 3) // used to be 4
                return QColor(Qt::darkBlue);
        }
        else if(role == Qt::DisplayRole)
        {
            if(columnorder.at(index.column()) == "name" || columnorder.at(index.column()) == "path") // used to be 1 || 2
            {
                if(nodetype == 3 || nodetype == 4 || (nodetype == 2 && itemnode->Data("id").toString().contains("-c"))) // used to be 5
                {
                    ba.clear();
                    ba.append(itemnode->Data(columnorder.at(index.column())).toString().toUtf8());
                    return QByteArray::fromBase64(ba);
                }
                else
                {
                    if(itemnode->Data(columnorder.at(index.column())).toString().compare("0") == 0)
                        return "";
                    else
                        return itemnode->Data(columnorder.at(index.column()));
                }
            }
            else if(columnorder.at(index.column()) == "size") // used to be 3
	    {
		QLocale::setDefault(QLocale(QLocale::English, QLocale::UnitedStates));
		return QString("%L1").arg(itemnode->Data(columnorder.at(index.column())).toLongLong());
                //return itemnode->Data(index.column());
	    }
            //else if(index.column() >= 3 && index.column() <= 6) // used to be >= 4 <= 7
            else if(columnorder.at(index.column()) == "create" || columnorder.at(index.column()) == "access" || columnorder.at(index.column()) == "modify" || columnorder.at(index.column()) == "status") // used to be >= 4 <= 7
            {
                if(itemnode->Data(columnorder.at(index.column())).toString().compare("0") == 0)
                    return "";
                else // surely i can qt this up....
                {
                    QDateTime tmpdt = QDateTime::fromSecsSinceEpoch(itemnode->Data(columnorder.at(index.column())).toInt(), QTimeZone::utc());
                    QString tmpstr = tmpdt.toString("MM/dd/yyyy hh:mm:ss AP");
                    return tmpstr;
                }
            }
            //else if(index.column() >= 7 && index.column() <= 10) // used to be >= 8 <= 10 // now includes tagged column
            /*
            {
                if(itemnode->Data(columnorder.at(index.column())).toString().compare("0") != 0)
                    return itemnode->Data(columnorder.at(index.column()));
                else
                    return "";
            }
            */
            else
            {
                if(itemnode->Data(columnorder.at(index.column())).toString().compare("0") != 0)
                    return itemnode->Data(columnorder.at(index.column()));
                else
                    return "";
            }
        }
        else if(role == Qt::DecorationRole)
        {
            //ba.clear();
            //ba.append(itemnode->Data(0).toString().toUtf8()); // used to be 1
            //QString nodename = QByteArray::fromBase64(ba);
            //if(columnorder.at(index.column()) == "id")
            if(index.column() == 0)
            {
                if(nodetype == 1)
                    return QIcon(":/basic/treeimg");
                else if(nodetype == 2)
                {
                    if(itemnode->Data("id").toString().endsWith("-cm"))
                        return QIcon(":/basic/virtualfolder");
                    else if(itemnode->Data("id").toString().contains("-c"))
                        return QIcon(":/basic/carveicon");
                    else if(itemnode->Data("id").toString().contains("-d"))
                        return QIcon(":/basic/treefolder");
                    else if(itemnode->Data("id").toString().contains("-f"))
                        return QIcon(":/basic/treefile");
                    else
                        return QIcon(":/basic/treevol");
                }
                /*
                else if(nodetype == 3)
                    return QIcon(":/basic/treefs");
                else if(nodetype == 4)
                {
                    if(itemtype == 3)
                        return QIcon(":/basic/virtualfolder");
                    return QIcon(":/basic/carveicon");
                }
                */
                else if(nodetype == 3) // used to be 5
                {
                    /*
                    if(nodename.startsWith("$"))
                    {
                        if(itemtype == 3)
                            return QIcon(":/basic/virtualdir");
                        else if(itemtype == 5)
                            return QIcon(":/basic/virtualfile");
                        else
                            return QIcon(":/basic/treefile");
                    }
                    */
                    //else if(nodename.contains("$OrphanFile"))
                    //    return QIcon(":/basic/virtualdir");
                    if(itemtype == 3)
                        return QIcon(":/basic/treefolder");
		    else if(itemtype == 2)
			return QIcon(":/basic/deletedfolder");
                    else if(itemtype == 5)
                        return QIcon(":/basic/treefile");
                    else if(itemtype == 10)
                        return QIcon(":/basic/virtualfile");
                    else if(itemtype == 11)
                        return QIcon(":/basic/virtualfolder");
                    else if(itemtype == 15)
                        return QIcon(":/basic/carveicon");
		    else if(itemtype == 4)
			return QIcon(":/basic/deletedfile");
                    else if(itemtype == 13)
                        return QIcon(":/basic/encryptedfile");
                    /*
                    else
                    {
                        if(itemnode->Data(11).toString().contains("fd") || itemnode->Data(11).toString().contains("fo"))
                            return QIcon(":/basic/deletedfile");
                        else
                            return QIcon(":/basic/treefile");
                    }
                    */
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
                checkhash.insert(itemnode->Data("id").toString(), false); // used to be 0
            }
            else
            {
                itemnode->SetChecked(true);
                checkhash.insert(itemnode->Data("id").toString(), true); // used to be 0
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
            /*
            bool result = itemnode->SetData(index.column(), value);
            if(result)
                emit dataChanged(index, index, {role});
            return result;
            */
            return false;
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
            bool result = zeronode->SetData(columnorder.at(section), value);
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
        //if(index.column() == 11 && itemnode->Data(index.column()).toString().split("-a").first().split("-").count() == 4) // used to be 0
        if(index.column() == 0 && itemnode->Data(columnorder.at(index.column())).toString().split("-").count() == 3) // used to be 0
            flags |= Qt::ItemIsUserCheckable | Qt::ItemIsEditable;
        
        return flags;
    };

    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override
    {
        if(orientation == Qt::Horizontal && role == Qt::DisplayRole)
        {
            if(section >= 0)
                return zeronode->Data(columnorder.at(section));
        }
        if(role == Qt::DecorationRole)
        {
            if(columnorder.at(section) == "id" && (!filtervalues.idfilter.isEmpty() && !filtervalues.idfilter.isNull()))
                return QIcon(":/basic/filterimg");
            if(columnorder.at(section) == "name" && filtervalues.namebool)
                return QIcon(":/basic/filterimg");
            if(columnorder.at(section) == "path" && filtervalues.pathbool)
                return QIcon(":/basic/filterimg");
            if(columnorder.at(section) == "size" && filtervalues.maxsizebool)
                return QIcon(":/basic/filterimg");
            if(columnorder.at(section) == "size" && filtervalues.minsizebool)
                return QIcon(":/basic/filterimg");
            if(columnorder.at(section) == "create" && (filtervalues.maxcreatebool || filtervalues.mincreatebool))
                return QIcon(":/basic/filterimg");
            if(columnorder.at(section) == "access" && (filtervalues.maxaccessbool || filtervalues.minaccessbool))
                return QIcon(":/basic/filterimg");
            if(columnorder.at(section) == "modify" && (filtervalues.maxmodifybool || filtervalues.minmodifybool))
                return QIcon(":/basic/filterimg");
            if(columnorder.at(section) == "status" && (filtervalues.maxchangebool || filtervalues.minchangebool))
                return QIcon(":/basic/filterimg");
            if(columnorder.at(section) == "hash" && (filtervalues.hashbool || filtervalues.hashbool2))
                return QIcon(":/basic/filterimg");
            if(columnorder.at(section) == "cat" && filtervalues.filegroupbool)
                return QIcon(":/basic/filterimg");
            if(columnorder.at(section) == "sig" && filtervalues.filetypebool)
                return QIcon(":/basic/filterimg");
            if(columnorder.at(section) == "tag" && filtervalues.tagbool)
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
    void AddNode(QHash<QString, QVariant> data, QString parid, int type, int deleted)
    {
        if(parid.toInt() == -1) // evid
        {
            zeronode->AppendChild(new TreeNode(data, zeronode));
            parents[data.value("id").toString()] = zeronode->child(zeronode->ChildCount() - 1); // USED TO BE 0
        }
        else // everything else 
        {
            if(type == -1)
                parents.value(parid)->AppendChild(new TreeNode(data, parents.value(parid)));
            else
            {
                //qDebug() << "parid:" << parid;
                parents.value(parid)->AppendChild(new TreeNode(data, parents.value(parid), type));
            }
            parents[data.value("id").toString()] = parents.value(parid)->child(parents.value(parid)->ChildCount() - 1); // USED TO BE 0
            //parents[data.at("id").toString().split("-a").first()] = parents.value(parid)->child(parents.value(parid)->ChildCount() - 1); // USED TO BE 0
            if(checkhash.contains(data.value("id").toString())) // USED TO BE 0
            {
                parents.value(data.value("id").toString())->SetChecked(true);
                //parents.value(data.at(11).toString().split("-a").first())->SetChecked(true); // USED TO BE 0
            }
            if(deleted == 1)
            {
                //parents.value(data.at(11).toString().split("-a").first())->SetDeleted(true); // USED TO BE 0
                parents.value(data.value("id").toString())->SetDeleted(true); // USED TO BE 0
            }
        }
    };

    bool ParentNodeExists(QString parid)
    {
        if(parents.contains(parid))
            return true;
        return false;
    };

    //void UpdateNode(QString itemid, int column, QString hash)
    void UpdateNode(QString itemid, QString column, QString hash)
    {
        const QVariant tmpvar(hash);
        //parents.value(itemid.split("-a").first())->SetData(column, tmpvar);
        parents.value(itemid)->SetData(column, tmpvar);
    };
    
    /*
    void UpdateHeaderNode(int column, QString hash)
    {
        //qDebug() << "updatehadernode value:" << zeronode->Data(9).toString();
        zeronode->SetData(column, hash);
    };
    */

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

    int GetColumnIndex(QString colkey)
    {
        for(int i=0; i < columnorder.count(); i++)
        {
            //qDebug() << "i:" << i << "columnorder:" << columnorder;
            if(columnorder.at(i) == colkey)
                return i;
        }
    };

    QStringList ReturnColumnOrder()
    {
        return columnorder;
    };

    void SetColumnOrder(QStringList colorder)
    {
	columnorder.clear();
	columnorder.append(colorder);
    }

private:
    TreeNode* zeronode; //rootitem
    QStringList columnorder;
    QHash<QString, TreeNode*> parents;
};

extern TreeNodeModel* treenodemodel;

#endif
