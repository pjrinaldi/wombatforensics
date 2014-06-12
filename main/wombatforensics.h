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
#include "../modeltest/modeltest.h"


class TreeModel : public QAbstractItemModel
{
public:
    TreeModel(QObject* parent = 0) : QAbstractItemModel(parent)
    {
        headerdata << "ID" << "Name" << "Full Path" << "Size (bytes)" << "Object Type" << "Address" << "Created (UTC)" << "Accessed (UTC)" << "Modified (UTC)" << "Status Changed (UTC)" << "MD5 Hash" << "Parent ID";
        rootnode = 0;
        currentrowcount = 0;
    };

    ~TreeModel()
    {
        delete rootnode;
    };

    void SetRootNode(Node* node)
    {
        delete rootnode;
        rootnode = node;
        beginResetModel();
        fetchMore(index(0, 0, QModelIndex()));
        //QModelIndex rootindex = IndexFromNode(node, 0);
        //rowCount(QModelIndex());
        endResetModel();
        //QAbstractItemModel::reset();
        //fetchMore(QModelIndex());
        //fetchMore(rootnode);
        //beginResetModel();
        //currentrowcount = 0;
        //totalcount = node->children.count();
        //totalcount = sqlquery.count() // total record count returned from sql...
        //endResetModel();
        //QAbstractItemModel::reset();
    };

    QModelIndex index(int row, int col, const QModelIndex &parent) const
    {
        if(!rootnode || row < 0 || col < 0)
            return QModelIndex();
        Node* parentnode = NodeFromIndex(parent);
        //qDebug() << "parentid: " << parentnode->nodevalues.at(0).toInt();
        //if(parentnode == rootnode)
        //{
        //  call sql function here to get the data for root...
        //}
        Node* childnode = parentnode->children.value(row);
        qDebug() << "index (" << row << ", " << col << ")";
        //qDebug() << "index (" << row << ", " << col << ")" << childnode->nodevalues.at(0);
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
        qDebug() << "parent (" << row << ", 0) " << parentnode->nodevalues.at(0);
        return createIndex(row, 0, parentnode);
    };

    int rowCount(const QModelIndex &parent) const
    {
        if(parent.column() > 0)
            return 0;
        Node* parentnode = NodeFromIndex(parent);
        if(!parentnode)
            return 0;
        qDebug() << "return rowcount for parent." << parentnode->children.count();
        return parentnode->children.count();
    };

    int columnCount(const QModelIndex &parent) const
    {
        return headerdata.count();
    };

    QVariant data(const QModelIndex &index, int role) const
    {
        if(index == QModelIndex())
            return QVariant();
        if(role != Qt::DisplayRole)
            return QVariant();
        Node* node = NodeFromIndex(index);
        if(!node)
            return QVariant();
        if(index.column() >= 6 && index.column() <= 9)
        {
            char buf[128];
            QString tmpstr = QString(TskTimeToStringUTC(node->nodevalues.at(index.column()).toInt(), buf));
            return tmpstr;
        }
        qDebug() << "return data from nodevalues." << node->nodevalues.at(0).toInt();
        return node->nodevalues.at(index.column());
    };

    QVariant headerData(int section, Qt::Orientation orientation, int role) const
    {
        if(orientation == Qt::Horizontal && role == Qt::DisplayRole)
        {
            return headerdata.at(section);
        }
        return QVariant();
    };

    bool hasChildren(const QModelIndex &parent = QModelIndex()) const
    {
        if(parent == QModelIndex())
            return false;
        Node* parentnode = NodeFromIndex(parent);
        if(!parent.isValid())
            return (parentnode->children.count() > 0);
        return false;

        //if(rowCount(parent) > 0)
        //if(parentnode->children.count() > 0)
            //return true;
        //return false;
    };

    bool canFetchMore(const QModelIndex &parent) const
    {
        qDebug() << "can fetch more called.";
        if(parent != QModelIndex())
        {
            Node* parentnode = NodeFromIndex(parent);
            if(parentnode->haschildren && parentnode->children.count() <= 0)
                return true;
        }

        return false;
    };

    void fetchMore(const QModelIndex &parent) const
    {
        int parentid = 5; // if this works, i'll need to figure out how to get the rootinum in here.
        //parentnode = rootnode;
        if(parent != QModelIndex())
            parentid = parentnode->nodevalues.at(5).toInt();

        Node* parentnode;
        Node* currentchild;
        QList<QSqlRecord> recordlist;
        QList<QVariant> fetchvalues;
        qDebug() << "fetchmore called.";
        if(parent == QModelIndex())
        {
            parentnode = rootnode;
        }
        else
            parentnode = NodeFromIndex(parent);
        QSqlQuery fetchchildrenquery(fcasedb);
        fetchchildrenquery.prepare("SELECT objectid, name, fullpath, size, objecttype, address, crtime, atime, mtime, ctime, md5, parentid FROM data WHERE objecttype == 5 AND parentid = ?");
        fetchchildrenquery.addBindValue(parentid);
        if(fetchchildrenquery.exec())
        {
            recordlist.clear();
            while(fetchchildrenquery.next())
            {
                recordlist.append(fetchchildrenquery.record());
            }
            for(int i=0; i < recordlist.count(); i++)
            {
                colvalues.clear();
                for(int j=0; j < recordlist[i].count(); j++)
                {
                    colvalues.append(recordlist[i].value(j));
                }
                currentchild = new Node(colvalues);
                parentnode->children.append(currentchild);
                currentchild->parent = parentnode;
                QSqlQuery childcountquery(fcasedb);
                childcountquery.prepare("SELECT COUNT(objectid) as children FROM data WHERE parentid = ?");
                childcountquery.addBindValue(currentchild->nodevalues.at(5).toInt());
                if(childcountquery.exec())
                {
                    childcountquery.next();
                    currentchild->childcount = childcountquery.value(0).toInt();
                    if(currentchild->childcount > 0)
                        currentchild->haschildren = true;
                }
            }
        }


        //if(parentnode->fetchedchildren)
        //    return;
        //parentnode->fetchedchildren = true;
       /*
        *    wombatptr->bindvalues.clear();
    wombatptr->bindvalues.append(wombatptr->currentrootinum);
    wombatptr->sqlrecords.clear();
    wombatptr->sqlrecords = GetSqlResults("SELECT objectid, name, fullpath, size, objecttype, address, crtime, atime, mtime, ctime, md5, parentid FROM data WHERE objecttype < 5 OR (objecttype == 5 AND parentid = ?)", wombatptr->bindvalues);
    for(int i=0; i < wombatptr->sqlrecords.count(); i++)
    {
        colvalues.clear();
        for(int j=0; j < wombatptr->sqlrecords[i].count(); j++)
        {
            colvalues.append(wombatptr->sqlrecords[i].value(j));
        }
        currentnode = new Node(colvalues);
        if(colvalues.at(4).toInt() < 5) // not file or directory
        {
            if(colvalues.at(4).toInt() == 1) // image node
            {
                dummynode = new Node(colvalues);
                dummynode->children.append(currentnode);
                currentnode->parent = dummynode;
                parentnode = currentnode;
            }
            else // volume or partition or filesystem
            {
                parentnode->children.append(currentnode);
                currentnode->parent = parentnode;
                parentnode = currentnode;
            }
        }
        else // its a file or directory at the rootinum level...
        {
            parentnode->children.append(currentnode);
            currentnode->parent = parentnode;
            QSqlQuery childcountquery(wombatptr->casedb);
            childcountquery.prepare("SELECT COUNT(objectid) as children FROM data WHERE parentid = ?");
            childcountquery.addBindValue(currentnode->nodevalues.at(5).toInt());
            if(childcountquery.exec())
            {
                childcountquery.next();
                currentnode->childcount = childcountquery.value(0).toInt();
                if(currentnode->childcount > 0)
                    currentnode->haschildren = true;
            }
        }

        *
        */ 
        
        // CALL SQL QUERY HERE...
        qDebug() << "get fetched children now...";
            
        //Node* parentnode = NodeFromIndex(parent);
        //rowCount(parent);
        //beginInsertRows(parent, 0, parentnode->children.count());
        /*
        //beginInsertRows(parent, 0, rowcount-1);
        //endInsertRows();
        Node* parentnode = NodeFromIndex(parent);
        if(parentnode->HasChildren())
        {
            int remainder = parentnode->children.count() - currentrowcount;
            int fetchcount = qMin(100, remainder);

            //beginInsertRows(parent, currentrowcount, currentrowcount+fetchcount-1);
            //currentrowcount += fetchcount;
            //QAbstractItemModel::endInsertRows();

            //emit NumberPopulated(fetchcount); // used to update a log.
        }*/
    };

    void Refresh(const QModelIndex &parent)
    {
        Node* parentnode;
        if(!parent.isValid())
            parentnode = rootnode;
        parentnode = NodeFromIndex(parent);
        if(parentnode->children.isEmpty())
        {
            parentnode->fetchedchildren = false;
            fetchMore(parent);
        }
    };

private:
    Node* NodeFromIndex(const QModelIndex &index) const
    {
        if(index.isValid())
            return static_cast<Node*>(index.internalPointer());
        else
            return rootnode;
    };

    QModelIndex IndexFromNode(const Node* node, int col) const
    {
        int row = 0;
        bool found = false;
        Node* parentnode = node->parent;
        if(!parentnode)
            parentnode = rootnode;
        for(int i=0; i < parentnode->children.count(); i++)
        {
            if(node == parentnode->children.at(i))
            {
                row = i;
                found = true;
            }
        }
        if(!found)
            return QModelIndex();

        return createIndex(row, col, parentnode->children.at(row));
    };

    Node* rootnode;
    QStringList headerdata;
    int currentrowcount;
    int totalcount;
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
    //ProgressWindow* wombatprogresswindow;
    ExportDialog* exportdialog;
    CheckableProxyModel* checkableproxy;
    TreeModel* treemodel;

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
    //void CurrentChanged(const QModelIndex &selindex, const QModelIndex &previndex);
    //void dirTreeView_selectionChanged(const QModelIndex &index);
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
    };
    void ExpandCollapseResize(const QModelIndex &index)
    {
        //((TreeModel*)index.model())->Refresh(index);
        if(((TreeModel*)index.model())->canFetchMore(index))
            ((TreeModel*)index.model())->fetchMore(index);
        //ui->dirTreeView->resizeColumnToContents(index.column());
        ResizeViewColumns(index);
    };
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

/*
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
*/

#endif // WOMBATFORENSICS_H
