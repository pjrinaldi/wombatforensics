#ifndef WOMBATFORENSICS_H
#define WOMBATFORENSICS_H

#include "wombatinclude.h"
#include "wombatvariable.h"
#include "wombatdatabase.h"
#include "wombatframework.h"
#include "wombatfunctions.h"
#include "ui_wombatforensics.h"
#include "progresswindow.h"
#include "exportdialog.h"
#include "globals.h"
#include "checkableproxymodel.h"

/*
class TreeProxy : public QSortFilterProxyModel
{
    QModelIndex mapToSource(const QModelIndex &proxyIndex) const
    {
        return proxyIndex;
    };
    QModelIndex mapFromSource(const QModelIndex &sourceIndex) const
    {
        return sourceIndex;
    };
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const
    {
        //qDebug() << QSortFilterProxyModel::data(index, role); // object id
        return QSortFilterProxyModel::data(index, role);
    };
    bool setData(const QModelIndex &index, const QVariant &value, int role)
    {
        return QSortFilterProxyModel::setData(index, value, role);
    };
};*/
/*
class TreeProxy : public QAbstractProxyModel
{
public:
    TreeProxy() : QAbstractProxyModel() {};
    QModelIndex mapToSource(const QModelIndex &proxyIndex) const
    {
    };
    QModelIndex mapFromSource(const QModelIndex &sourceIndex) const
    {
    };
    QModelIndex index(int row, int column, const QModelIndex &parentIndex = QModelIndex()) const
    {
    };
    QModelIndex parent(const QModelIndex &child) const
    {
    };
    int rowCount(const QModelIndex &parent = QModelIndex()) const
    {
    };
    int columnCount(const QModelIndex &parent = QModelIndex()) const
    {
    };
    QVariant data(const QModelIndex &proxyIndex, int role = Qt::DisplayRole) const
    {
        QVariant value = QAbstractProxyModel::data(proxyIndex, role);
        /*
        QVariant value = QSqlQueryModel::data(index, role);
        if(value.isValid() && role == Qt::DisplayRole)
        {
            if(index.column() >= 6 && index.column() <= 9)
            {
                char buf[128];
                QString tmpstr = QString(tsk_fs_time_to_str(value.toInt(), buf));

                return tmpstr;
            }
        }
        */
//        return value;
//    };
    //QVariant headerData(int 
//};
    /*
    QModelIndex mapToSource(const QModelIndex &proxyIndex) const
    {
        return QModelIndex();
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
        return QModelIndex();
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
        if(parent.isValid()
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
        return QModelIndex();
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
};*/



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
    //ProgressWindow* wombatprogresswindow;
    ExportDialog* exportdialog;
    CheckableProxyModel* checkableproxy;
    //TreeProxy* treeproxy;

signals:
    void LogVariable(WombatVariable* wombatVariable);
private slots:
    void AddEvidence();
    void RemEvidence();
    void ExportEvidence();
    void on_actionNew_Case_triggered();
    void on_actionOpen_Case_triggered();
    void on_actionView_Progress_triggered(bool checked);
    void UpdateProgress(int count, int processcount);
    void UpdateMessageTable();
    void SelectionChanged(const QItemSelection &selitem, const QItemSelection &deselitem);
    void CurrentChanged(const QModelIndex &selindex, const QModelIndex &previndex);
    void dirTreeView_selectionChanged(const QModelIndex &index);
    void HideProgressWindow(bool checkstate);
    void DisplayError(QString errorNumber, QString errorType, QString errorValue);
    void PopulateProgressWindow(WombatVariable* wvariable);
    void ResizeColumns(void);
    void OpenParentImage(int imgid);
    void OpenParentFileSystem(void);
    void OpenFileSystemFile(void);
    void ResizeViewColumns(const QModelIndex &index)
    {
        ResizeColumns();
    }
    void FileExport(FileExportData* exportdata);
    void setScrollBarRange(off_t low, off_t high);
    void setScrollBarValue(off_t pos);
    void setOffsetLabel(off_t pos);
    void UpdateSelectValue(const QString &txt);
    void ViewGroupTriggered(QAction* curaction);
    void LoadComplete(bool isok);
    void InitializeQueryModel(void);
    void UpdateTree(void);

protected:
    void closeEvent(QCloseEvent* event);
private:
    Ui::WombatForensics *ui;

    void SetupHexPage(void);
    void SetupToolbar(void);
    void InitializeAppStructure(void);
    void InitializeCaseStructure(void);
    void InitializeEvidenceStructure(void);
    void InitializeOpenCase(void);
    void InitializeWombatFramework(void);
    void InitializeDirModel(void);
    void UpdateViewer(void);
    void UpdateOmniValue(void);
    void LoadHexContents(void);
    void LoadTxtContents(void);
    void LoadWebContents(void);
    void LoadImgContents(void);
    void LoadVidContents(void);

    void RemoveTmpFiles(void);

    //QStandardItem* GetCurrentImageDirectoryTree(QObject *plugin, QString imageDbPath, QString imageName);
    //QThreadPool* threadpool;
    int ReturnVisibleViewerID();
    int StandardItemCheckState(QStandardItem* tmpitem, int checkcount);
    int StandardItemListCount(QStandardItem* tmpitem, int listcount);
    QVector<FileExportData> SetFileExportProperties(QStandardItem* tmpitem, FileExportData* tmpexport, QVector<FileExportData>);
    QVector<FileExportData> SetListExportProperties(QStandardItem* tmpitem, FileExportData* tmpexport, QVector<FileExportData>);
    int DetermineOmniView(QString currentSignature);
    QModelIndex curselindex;

    QFuture<void> sqlfuture;
    QFutureWatcher<void> sqlwatcher;
    //QFuture<void> openfuture;
    //QFutureWatcher<void> openwatcher;

    off_t offset() const;
    HexEditor* hexwidget;
    QActionGroup* viewgroup;
    QScrollBar* hexvsb;
    QStatusBar* hstatus;
    QLabel* selectedoffset;
    QLabel* selectedhex;
    QLabel* selectedascii;
    QLabel* selectedinteger;
    QLabel* selectedfloat;
    QLabel* selecteddouble;
    QProgressBar* mainprogress;
    QLabel* filecountlabel;

    // NEED TO REMOVE THESE ONCE I GET MY FUNCTIONS DONE AND REMOVE THE OLD STRUCTURE
    //QStandardItemModel* currenttreemodel;
    //QStandardItemModel* wombatdirmodel;
    //QStandardItemModel* wombattypmodel;
};

class FileViewSqlModel : public QSqlQueryModel
{
    Q_OBJECT

public:
    FileViewSqlModel(QObject* parent = 0) : QSqlQueryModel(parent) {};

    QVariant data(const QModelIndex &index, int role) const
    {
        QVariant value = QSqlQueryModel::data(index, role);
        if(value.isValid() && role == Qt::DisplayRole)
        {
            if(index.column() >= 6 && index.column() <= 9)
            {
                char buf[128];
                //QString tmpstr = QString(tsk_fs_time_to_str(value.toInt(), buf));
                QString tmpstr = QString(TskTimeToStringUTC(value.toInt(), buf));

                return tmpstr;
            }
        }

        return value;
    };
};

class TreeViewSqlModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit TreeViewSqlModel(QObject* parent = 0) : QAbstractItemModel(parent)
    {
        headerdata << "ID" << "Name" << "Full Path" << "Size (bytes)" << "Signature" << "Extension" << "Created (UTC)" << "Accessed (UTC)" << "Modified (UTC)" << "Status Changed (UTC)" << "MD5 Hash";
    };

    ~TreeViewSqlModel()
    {
    };

    int rowCount(const QModelIndex &parent = QModelIndex()) const
    {
        // if parent.isValid() -> not root.
        QSqlQuery rowquery(fcasedb);
        rowquery.prepare("SELECT count(objectid) AS childcount FROM data");
        //qDebug() << "ROWCOUNT " << "Parent QModelIndex: " << parent << "Parent internalID: " << parent.internalId();
        rowquery.addBindValue(parent.internalId());
        if(rowquery.exec())
        {
            rowquery.next();
            //qDebug() << "row count: " << rowquery.value(0).toInt();
            return rowquery.record().value("childcount").toInt();
        }
        else
            return 0;
    };

    QVariant data(const QModelIndex &index, int role) const
    {
        if(!index.isValid())
            return QVariant();

        if(role == Qt::DisplayRole)
        {
            QSqlQuery dataquery(fcasedb);
            dataquery.prepare("SELECT objectid, name, fullpath, size, objecttype, address, crtime, atime, mtime, ctime, md5, parentid, rootinum FROM data where objectid = ?");
            dataquery.addBindValue(index.row());
            if(dataquery.exec())
            {
                dataquery.next();
                QList<QVariant> tmplist;
                tmplist << dataquery.record().value("objectid").toInt() << dataquery.record().value("name").toString() << dataquery.record().value("fullpath").toString() << dataquery.record().value("size").toInt() << "" << "" << dataquery.record().value("crtime").toInt() << dataquery.record().value("atime").toInt() << dataquery.record().value("mtime").toInt() << dataquery.record().value("ctime").toInt() << dataquery.record().value("md5").toString();
                return tmplist;
            }
            return QVariant();
        }
        return QVariant();
    };

    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const
    {
        if(role != Qt::DisplayRole)
            return QVariant();

        if(orientation == Qt::Horizontal)
            return headerdata[section];

        return QVariant();
    };

    QModelIndex index(int row, int column, const QModelIndex &item = QModelIndex()) const
    {
        if(!hasIndex(row, column, item))
            return QModelIndex();
        
        if(!item.isValid()) // root item
        {
            return createIndex(0, 0, item.internalId());
        }

        return createIndex(row, column, item.internalId());

        //QSqlQuery indexquery(fcasedb);
        //indexquery.prepare("SELECT objectid, name, fullpath, size, crtime, atime, mtime, ctime, md5, parentid, rootinum FROM data WHERE objectid = ?");
        //indexquery.addBindValue(item.internalId());
        //if(indexquery.exec())
       // {
            //indexquery.next();
            //return createIndex(row, column, indexquery.
        //}
    };

    QModelIndex parent(const QModelIndex &item) const
    {
        if(!item.isValid())
            return QModelIndex();

        QSqlQuery parentquery(fcasedb);
        parentquery.prepare("SELECT address, parentid FROM data WHERE objectid = ?");
        parentquery.addBindValue(item.internalId());
        if(parentquery.exec())
        {
            parentquery.next();
            return createIndex(item.internalId(), 0, parentquery.value(1).toInt());
        }
        else
            return QModelIndex();
    };

    int columnCount(const QModelIndex &item = QModelIndex()) const
    {
        return headerdata.count();
    };

    Qt::ItemFlags flags(const QModelIndex &item) const
    {
        if(!item.isValid())
            return 0;

        return QAbstractItemModel::flags(item);
    };

private:
    int rootinum;
    QList<QVariant> headerdata;
};

/*
class TreeItem
{
public:
    explicit TreeItem(const QList<QVariant> &data, TreeItem* parent = 0)
    {
        parentitem = parent;
        itemdata = data;
    };
    ~TreeItem()
    {
        qDeleteAll(childitems);
    };

    void appendChild(TreeItem* child)
    {
        childitems.append(child);
    };

    TreeItem* child(int row)
    {
        return childitems.value(row);
    };
    int childCount() const
    {
        return childitems.count();
    };
    int columnCount() const
    {
        return itemdata.count();
    };
    QVariant data(int column) const
    {
        return itemdata.count();
    };
    int row() const
    {
        if(parentitem)
            return parentitem->childitems.indexOf(const_cast<TreeItem*>(this));

        return 0;
    };
    TreeItem* parent()
    {
        return parentitem;
    };

private:
    QList<TreeItem*> childitems;
    QList<QVariant> itemdata;
    TreeItem* parentitem;
};

class TreeViewSqlModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit TreeViewSqlModel(QObject* parent = 0) : QAbstractItemModel(parent)
    {
        // get root address.
        QSqlQuery rootquery(fcasedb);
        rootquery.prepare("SELECT rootinum from data where objecttype = 4");
        if(rootquery.exec())
        {
            rootquery.next();
            rootinum = rootquery.record().value("rootinum").toInt();
        }
        rootquery.finish();
        //QList<QVariant> rootdata;
        //QList<QSqlRecord> dataset;
        headerdata << "ID" << "Name" << "Full Path" << "Size (bytes)" << "Signature" << "Extension" << "Created (UTC)" << "Accessed (UTC)" << "Modified (UTC)" << "Status Changed (UTC)" << "MD5 Hash";
        SetupModelData();
        //rootitem = new TreeItem(rootdata);
        //SetupModelData(data.split(QString("\n")), rootitem);
        //treequery = QSqlQuery((fcasedb);
        //treequery.prepare("SELECT objectid, name, fullpath, size, objecttype, address, crtime, atime, mtime, ctime, md5, parentid, rootinum FROM data");
        //treequery = QSqlQuery("SELECT objectid, name, fullpath, size, objecttype, address, crtime, atime, mtime, ctime, md5, parentid, rootinum FROM data where address = ?", fcasedb);
        if(treequery.exec())
        {
            while(treequery.next())
            {
                dataset.append(treequery.record());
            }
        }
        treequery.finish();
    };
    ~TreeViewSqlModel()
    {
        //delete rootitem;
    };
    QVariant data(const QModelIndex &index, int role) const
    {
        if(!index.isValid())
            return QVariant();
        QSqlQuery dataquery(fcasedb);
        dataquery.prepare("SELECT objectid, name, fullpath, size, objecttype, address, crtime, atime, mtime, ctime, md5, parentid, rootinum FROM data WHERE address = ?");
        dataquery.addBindValue(index.internalId());
        if(dataquery.exec())
        {
            dataquery.next();
            qDebug() << "data query address|parent|rootinum: " << dataquery.value(5).toInt() << "|" << dataquery.value(11).toInt() << "|" << dataquery.value(12).toInt();
        if(role != Qt::DisplayRole)
            return QVariant();
        return dataquery.value(index.column());
        }
        //dataquery.finish();
        //treequery.addBindValue(index.internalId());
        //treequery.exec
        // places data in each column from sql query. call above query to return columns where index/address is ?"
        if(!index.isValid())
            return QVariant();

        if(role != Qt::DisplayRole)
            return QVariant();

        TreeItem* item = static_cast<TreeItem*>(index.internalPointer());

        return item->data(index.column());
    };
    Qt::ItemFlags flags(const QModelIndex &index) const
    {
        if(!index.isValid())
            return 0;
        return QAbstractItemModel::flags(index);
    };
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const
    {
        // headerdata simply calls headerdata[section].
        if(orientation == Qt::Horizontal && role == Qt::DisplayRole)
            return headerdata[section];
            //return rootitem->data(section);

        return QVariant();
    };
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const
    {
        qDebug() << "index parent.internalid: " << parent.internalId();
        if(parent.isValid()) // child
        {
            //return createIndex(row, column, parent.internalId());
        }
        QSqlQuery indexquery(fcasedb);
        return parent;
        //indexquery.prepare("SELECT addres");
        // root
        //return createIndex(row, column, rootinum);

        //treequery
        // simply calls sqlquery and uses address and parentid for it.
        // probably no need to validate stuff, since sql is valid.
        // simply calls sqlquery and get's address from it for index.
        if(parent.isValid())
        {
            //return createIndex(row, column, address);
        }
        
        //return createIndex(row, column, address);
        if(!hasIndex(row, column, parent))
            return QModelIndex();

        TreeItem *parentitem;
        if(!parent.isValid())
            parentitem = rootitem;
        else
            parentitem = static_cast<TreeItem*>(parent.internalPointer()); // parent.internalId()

        TreeItem* childitem = parentitem->child(row);
        if(childitem)
            return createIndex(row, column, childitem);
        else
            return QModelIndex();
    };
    QModelIndex parent(const QModelIndex &index) const
    {
        qDebug() << "parent index.internalid: " << index.internalId();
        // use sql to get parentid for a given index.
        // then createindex for it.
        if(index.isValid()) // child
        {
            QSqlQuery parentquery(fcasedb);
            parentquery.prepare("SELECT parentid FROM data where address = ?");
            parentquery.addBindValue(index.internalId());
            if(parentquery.exec())
            {
                parentquery.next();
                return index.parent();
                //return createIndex(index.row(), 0, parentquery.value(0).toInt());
            }

            //return createIndex(index.row(), 0, rootinum);
        }
        //return createIndex(index.row(), 0, index.internalId());
            //return createIndex(index.sibling().row(), 0, index.sibling());
        if(!index.isValid())
            return QModelIndex();

        TreeItem* childitem = static_cast<TreeItem*>(index.internalPointer());
        TreeItem* parentitem = childitem->parent();

        if(parentitem == rootitem)
            return QModelIndex();

        return createIndex(parentitem->row(), 0, parentitem);
    };
    int rowCount(const QModelIndex &parent = QModelIndex()) const
    {
        QSqlQuery rowquery(fcasedb);
        rowquery.prepare("SELECT count(objectid) AS childcount FROM data WHERE parentid = ?");
        qDebug() << "ROWCOUNT " << "Parent QModelIndex: " << parent << "Parent internalID: " << parent.internalId();
        rowquery.addBindValue(parent.internalId());
        if(rowquery.exec())
        {
            rowquery.next();
            qDebug() << "row count: " << rowquery.value(0).toInt();
            return rowquery.record().value("childcount").toInt();
        }
        else
            return 0;
        // get result count() for a given index/address
        TreeItem* parentitem;
        if(parent.column() > 0)
            return 0;

        if(!parent.isValid())
            parentitem = rootitem;
        else
            parentitem = static_cast<TreeItem*>(parent.internalPointer());
        //return parentitem->childCount();
    };
    int columnCount(const QModelIndex &parent = QModelIndex()) const
    {
        return headerdata.count();
        // column count is always the same, length of the data..
        if(parent.isValid())
            return static_cast<TreeItem*>(parent.internalPointer())->columnCount();
        else
            return rootitem->columnCount();
    };
private:
    //void SetupModelData(const QStringList &lines, TreeItem* parent)
    void SetupModelData()
    {
        treequery = QSqlQuery("SELECT objectid, name, fullpath, size, rootinum, parentid FROM data WHERE objecttype < 5", fcasedb);
        if(treequery.exec())
        {
            while(treequery.next())
            {
            }
        }
        QList<QVariant> tmpdata;
        //QList<QTreeItem*> parents;
        //QList<QTreeItem*> treebranch;
        //QHash<int, int> treehash; // address, TreeItem*.
        //int rootinum = -1;
        //tmpdata.clear();
        //parents << parent;
        QSqlQuery modelquery(fcasedb);
        QModelIndex imageindex;
        QModelIndex fsindex;
        modelquery.prepare("SELECT objectid, name, fullpath, size, objecttype, address, crtime, atime, mtime, ctime, md5, parentid, rootinum FROM data");
        if(modelquery.exec())
        {
            while(modelquery.next())
            {
                //tmpdata.clear();
                //tmpdata << modelquery.record().value("objectid").toInt() << modelquery.record().value("name").toString() << modelquery.record().value("fullpath").toString() << modelquery.record().value("size").toInt() << "" << "" << modelquery.record().value("crtime").toInt() << modelquery.record().value("atime").toInt() << modelquery.record().value("mtime").toInt() << modequery.record().value("ctime").toInt() << modelquery.record.value("md5").toString();
                if(modelquery.value(4).toInt() == 1)
                {
                    imageindex = createIndex(0, 0, (quintptr)0);
                }
                else if(modelquery.value(4).toInt() == 4)
                {
                    //parents << parents.last()->appendChild(new TreeItem*(tmpdata, parents.last()));
                    //if(modelquery.record().value("rootinum"))
                    //{
                    fsindex = createIndex(0, 0, imageindex.internalId());
                    rootinum = modelquery.value(12).toInt();
                    QModelIndex rootindex = createIndex(0, 0, fsindex.internalId());
                    qDebug() << "root inum from setupmodeldata: " << rootinum;
                        //treehash.insert(modelquery.record.value("rootinum"), new TreeItem*(tmpdata, parents.last()));
                    //}
                }
                else // file/directory with hierarchy
                {
                    createIndex(0, 0, modelquery.value(11).toInt());
                    //treehash.insert(rootinum, 
                    //treehash.insert(
                    //treebranch << parents.last();
                    //treebranch
                    //parents << parents.last().appendChild(new TreeItem*(tmpdata, parents.last()));
                }
            }
        }
        // add images...
        modelquery.prepare("SELECT objectid, name, fullpath, size FROM data WHERE objecttype = 1");
        if(modelquery.exec())
        {
            while(modelquery.next())
            {
                tmpdata << modelquery.record().value(0).toInt() << modelquery.record().value(1).toString() << modelquery.record().value(2).toString() << modelquery.record().value(3).toInt << "" << "" << "" << "" << "" << "" << "";
                parents.last()->appendChild(new TreeItem*(tmpdata, parents.last()));
            }
        }
        modelquery.finish();
        // add file systems...
        modelquery.prepare("SELECT objectid, name, fullpath, (size*blockcount) as size FROM data WHERE objecttype = 2");
        if(modelquery.exec())
        {
            while(modelquery.next())
            {
                tmpdata << modelquery.record().value(0).toInt() << modelquery.record().value(1).toString() << modelquery.record().value(2).toString() << modelquery.record().value(3).toInt() << "" << "" << "" << "" << "" << "" << "";

            }
        }
        else
        {
        }
        modelquery.finish();
        QList<TreeItem*> parents;
        QList<int> indentations;
        parents << parent;
        indentations << 0;

        int number = 0;

        while(number < lines.count())
        {
            int position = 0;
            while(position < lines[number].length())
            {
                if(lines[number].mid(position,1) != " ")
                    break;
                position++;
            }

            QString linedata = lines[number].mid(position).trimmed();

            if(!linedata.isEmpty())
            {
                // read the columnm data from the rest of the line
                QStringList columnStrings = linedata.split("\t", QString::SkipEmptyParts);
                QList<QVariant> columndata;
                for(int column = 0; column < columnStrings.count(); ++column)
                    columndata << columnStrings[column];

                if(position > indentations.last())
                {
                    // the last child of the current parent is now the new parent
                    // unless the current parent has no children
                    if(parents.last()->childCount() > 0)
                    {
                        parents << parents.last()->child(parents.last()->childCount()-1);
                        indentations << position;
                    }

                }
                else
                {
                    while(position < indentations.last() && parents.count() > 0)
                    {
                        parents.pop_back();
                        indentations.pop_back();
                    }
                }

                // append a new item to the current parent's list of children
                parents.last()->appendChild(new TreeItem(columndata, parents.last()));
            }

            ++number;
        }
    };

    //TreeItem* rootitem;
    QList<QVariant> headerdata;
    QList<QSqlRecord> dataset;
    int rootinum = 0;
    //QSqlQuery treequery(fcasedb);
    // need to reimplement index(), parent(), rowCount(), columnCount(), hasChildren(), flags(), data(), headerdata()
    // from qabstractitemmodel. index.parent use createIndex() to genereate indexes for others to use/reference
    // fetchmore() can also be implemented to when a branch in the tree model is expanded. 
};
        wombatdatabase->GetEvidenceObjects(); // get's all evidenceobjects from the db for the given case
        FileViewSqlModel* tmpmodel = new FileViewSqlModel();
        tmpmodel->setQuery("SELECT objectid, name, fullpath, size, objecttype, address, crtime, atime, mtime, ctime, md5, parentid FROM data", fcasedb);
        tmpmodel->setHeaderData(0, Qt::Horizontal, tr("ID"));
        tmpmodel->setHeaderData(1, Qt::Horizontal, tr("Name"));
        tmpmodel->setHeaderData(2, Qt::Horizontal, tr("Full Path"));
        tmpmodel->setHeaderData(3, Qt::Horizontal, tr("Size (bytes)"));
        tmpmodel->setHeaderData(4, Qt::Horizontal, tr("Signature"));
        tmpmodel->setHeaderData(5, Qt::Horizontal, tr("Extension"));
        tmpmodel->setHeaderData(6, Qt::Horizontal, tr("Created (UTC)"));
        tmpmodel->setHeaderData(7, Qt::Horizontal, tr("Accessed (UTC)"));
        tmpmodel->setHeaderData(8, Qt::Horizontal, tr("Modified (UTC)"));
        tmpmodel->setHeaderData(9, Qt::Horizontal, tr("Status Changed (UTC)"));
        tmpmodel->setHeaderData(10, Qt::Horizontal, tr("MD5 Hash"));
        tmpmodel->removeColumns(11, 1, QModelIndex());
 */ 
#endif // WOMBATFORENSICS_H
