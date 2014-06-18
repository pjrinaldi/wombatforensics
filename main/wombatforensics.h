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
#include "../modeltest/modeltest.h"


class TreeModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    TreeModel(QObject* parent = 0) : QAbstractItemModel(parent)
    {
        headerdata << "ID" << "Name" << "Full Path" << "Size (bytes)" << "Object Type" << "Address" << "Created (UTC)" << "Accessed (UTC)" << "Modified (UTC)" << "Status Changed (UTC)" << "MD5 Hash" << "Parent ID";
        rootnode = 0;
    };

    ~TreeModel()
    {
        delete rootnode;
    };

    void SetRootNode(Node* node)
    {
        delete rootnode;
        rootnode = node;
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
            return 1;
        if(parent.column() > 0)
            return 0;
        Node* parentnode = rootnode; 
        if(parent.isValid())
            parentnode = NodeFromIndex(parent);
        return parentnode->childcount;
    };

    int columnCount(const QModelIndex &parent) const
    {
        return headerdata.count();
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
        if(role == Qt::DisplayRole)
        {
            if(index.column() >= 6 && index.column() <= 9)
            {
                char buf[128];
                QString tmpstr = QString(TskTimeToStringUTC(node->nodevalues.at(index.column()).toInt(), buf));
                return tmpstr;
            }
            else
                return node->nodevalues.at(index.column());
        }

        return QVariant();
    };

    bool setData(const QModelIndex &index, const QVariant &value, int role)
    {
        if(index.column() == 0 && role == Qt::CheckStateRole)
        {
            Node* curnode = NodeFromIndex(index);
            Qt::CheckState state = static_cast<Qt::CheckState>(value.toInt());
            return SetCheckState(curnode, state);
        }
    };

    QVariant headerData(int section, Qt::Orientation orientation, int role) const
    {
        if(orientation == Qt::Horizontal && role == Qt::DisplayRole)
        {
            if(section >= 0)
                return headerdata.at(section);
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
        Node* parentnode = rootnode;
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
            morequery.prepare("SELECT objectid, name, fullpath, size, objecttype, address, crtime, atime, mtime, ctime, md5, parentid FROM data WHERE objecttype == 5 AND parentid = ?");
            morequery.addBindValue(parentnode->nodevalues.at(5).toInt());
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
                        curchild->childcount = GetChildCount(5, curchild->nodevalues.at(5).toInt());
                        curchild->haschildren = curchild->HasChildren();
                    }
                    parentnode->children.append(curchild);
                }
                endInsertRows();
            }
        }
    };

signals:
    void checkedNodesChanged();

private:
    Node* NodeFromIndex(const QModelIndex &index) const
    {
        if(index.isValid())
            return static_cast<Node*>(index.internalPointer());
        else
            return rootnode;
    };

    Qt::CheckState GetCheckState(Node* curnode) const
    {
        if(curnode->checkstate == 0) // determined by parent
        {
            return GetCheckState(curnode->parent);
        }
        else if(curnode->checkstate == 1) // determined by child
        {
            int checkcount = 0;
            for(int i=0; i < curnode->children.count(); i++)
            {
                if(curnode->children.at(i)->checkstate == 2)
                    checkcount++;
            }
            if(curnode->childcount < checkcount)
                return Qt::PartiallyChecked;
            else if(curnode->childcount == checkcount)
                return Qt::Checked;
            return Qt::Unchecked;
        }
        else if(curnode->checkstate == 2)
            return Qt::Checked;
        else if(curnode->checkstate == 3)
            return Qt::Unchecked;

        return Qt::Unchecked;
    };

    bool SetCheckState(Node* curnode, Qt::CheckState state)
    {
        qDebug() << "setcheckstate to: " << state;
        //int oldcheckstate = curnode->checkstate;
        if(curnode->haschildren == true) // parent node with children
        {
            if(state == Qt::Unchecked) // it will be set to unchecked
            {
                curnode->checkstate = 3;
                for(int i=0; i < curnode->children.count(); i++)
                    curnode->children.at(i)->checkstate = 3;
            }
            else if(state == Qt::Checked) // it will be set to checked
            {
                curnode->checkstate = 2;
                for(int i=0; i < curnode->children.count(); i++)
                {
                    curnode->children[i]->checkstate = 2;
                    bool test = SetCheckState(curnode->children[i], state);
                }
                bool test = SetCheckState(curnode->parent, state);
                //curnode->parent->checkstate = 1; // set by child
            }
            else if(state == Qt::PartiallyChecked)
            {
                curnode->checkstate = 1; // set by child.
            }
            emit checkedNodesChanged();
            return true;
        }
        else // child node with no children
        {
            if(state == Qt::Unchecked)
                curnode->checkstate = 3;
            else if(state == Qt::Checked)
            {
                curnode->checkstate = 2;
                curnode->parent->checkstate = 1;
                //return SetCheckState(curnode->parent, state);
            }
            else if(state == Qt::PartiallyChecked)
            {
                curnode->checkstate = 1;
            }
            emit checkedNodesChanged();
            return true;
        }
        // use this function to set the new checkstate as well as parents or children where appropriate.
        return false;
    };
   
    //Node* rootnode;
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
    //ProgressWindow* wombatprogresswindow;
    ExportDialog* exportdialog;
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
        if(((TreeModel*)ui->dirTreeView->model())->canFetchMore(index))
            ((TreeModel*)ui->dirTreeView->model())->fetchMore(index);
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

#endif // WOMBATFORENSICS_H
