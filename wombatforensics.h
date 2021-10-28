#ifndef WOMBATFORENSICS_H
#define WOMBATFORENSICS_H

// Copyright 2013-2020 Pasquale J. Rinaldi, Jr.
// Distrubted under the terms of the GNU General Public License version 2

#include <QtEndian>
#include "wombatinclude.h"
#include "wombatvariable.h"
#include "wombatfunctions.h"
#include "carving.h"
#include "artifactparser.h"
#include "reporting.h"
#include "digging.h"
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
#include "addevidencedialog.h"
#include "remevidencedialog.h"
#include "tagmanager.h"
#include "searchdialog.h"
#include "carvedialog.h"
#include "filecarvingdialog.h"
#include "pdfviewer.h"
#include "artifactparser.h"
#include "digging.h"
#include "carving.h"
#include "exporting.h"
#include "newcase.h"
#include "opencase.h"
#include "registryviewer.h"
#include "newforimg.h"
#include "verevidencedialog.h"
#include "hashlistmanager.h"

class StatusLabel : public QLabel
{
    Q_OBJECT

    public:

    protected:
        void mousePressEvent(QMouseEvent* e)
        {
            emit clicked();
            e->accept();
        }

    signals:
        void clicked();
};

class PathTreeView : public QTreeView
{
    Q_OBJECT

    public:

    protected:
        void mousePressEvent(QMouseEvent* e)
        {
            QModelIndex index = this->indexAt(e->pos());
            //qDebug() << "single click:" << e->button();
            if(e->button() == Qt::LeftButton)
            {
                //qDebug() << "left clicked";
                this->setCurrentIndex(index);
            }
            else if(e->button() == Qt::MiddleButton)
            {
                TreeNode* itemcheck = static_cast<TreeNode*>(index.internalPointer());
                if(!itemcheck->IsChecked())
                {
                    itemcheck->SetChecked(true);
                    checkhash.insert(itemcheck->Data("id").toString(), true); // used to be 0
                }
                else
                {
                    itemcheck->SetChecked(false);
                    checkhash.insert(itemcheck->Data("id").toString(), false); // used to be 0
                }
                //treenodemodel->setData(index, 
		emit treenodemodel->dataChanged(index, index);
                emit treenodemodel->CheckedNodesChanged();
                //qDebug() << "call show file here ... middle button clicked";
            }
            else if(e->button() == Qt::RightButton)
            {
                //qDebug() << "right button clicked";
            }
            e->accept();
        }
        void mouseDoubleClickEvent(QMouseEvent* e)
        {
            //qDebug() << "double click:" << e->button();
            if(e->button() == Qt::LeftButton)
            {
                QModelIndex index = this->indexAt(e->pos());
                if(treenodemodel->rowCount(index.sibling(index.row(), 0)) > 0)
                    this->setRootIndex(index.sibling(index.row(), 0));
                else
                    emit LaunchFile(index);
                //qDebug() << "left clicked";
            }
            else if(e->button() == Qt::MiddleButton)
            {
            }
            else if(e->button() == Qt::RightButton)
            {
            }
            //e->ignore();
        }

    signals:
        void LaunchFile(const QModelIndex &index);
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
    PropertiesWindow* propertywindow;
    ExportDialog* exportdialog;
    DigDeeperDialog* digdeeperdialog;
    FileViewer* fileviewer;
    SearchDialog* searchdialog;
    QMenu* viewmenu;
    QMenu* treemenu;
    QMenu* selectionmenu;
    QMenu* bookmarkmenu;
    QMenu* tagcheckedmenu;
    QMenu* hashlistmenu;
    QMenu* hashcheckedmenu;
    QMenu* headermenu;
    QAction* remtagaction; // remove selected tag
    QAction* remtagaction1; // remove checked tag
    QAction* remcheckedaction; // remove all checked tags
    IdFilter* idfilterview;
    JumpHex* jumpfilterview;
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
    TagFilter* tagfilterview;
    ImageViewer* imagewindow;
    ImageWindow* imageviewer;
    VideoViewer* videowindow;
    ViewerManager* viewmanage;
    TextViewer* textviewer;
    HtmlViewer* htmlviewer;
    HtmlViewer* previewreport;
    MessageViewer* msgviewer;
    ByteConverter* byteviewer;
    AboutBox* aboutbox;
    SettingsDialog* settingsdialog;
    AddEvidenceDialog* addevidencedialog;
    RemEvidenceDialog* remevidencedialog;
    TagManager* tagmanage;
    CarveDialog* carvedialog;
    FileCarvingDialog* filecarvedialog;
    PdfViewer* pdfviewer;
    RegistryDialog* regviewer;
    ForImgDialog* forimgdialog;
    VerEvidenceDialog* verevidencedialog;
    HashListManager* hashlistmanager;

signals:
    void CancelCurrentThread();
    //void DigType(int dtype);

private slots:
    void AddEvidence();
    void RemEvidence();
    void VerEvidence(bool checked);
    //void VerEvidence();
    void RemoveEvidence(QStringList remlist);
    void VerifyEvidence(QStringList verlist);
    void VerProgChange(int progval);
    void ExportEvidence();
    void on_actionNew_Case_triggered();
    void on_actionOpen_Case_triggered();
    void on_actionSaveState_triggered();
    void on_actionCheck_triggered();
    void on_actionExport_triggered();
    void on_actionDigDeeper_triggered();
    void on_actionCarve_triggered();
    void on_actionView_Properties_triggered(bool checked);
    void on_actionView_File_triggered(bool checked);
    void on_actionView_Image_Gallery_triggered(bool checked);
    void on_actionViewerManager_triggered();
    void on_actionBookmark_Manager_triggered();
    void on_actionpreviewreport_triggered(bool checked);
    void on_actionTextViewer_triggered(bool checked);
    void on_actionViewMessageLog_triggered(bool checked);
    void on_actionByteConverter_triggered(bool checked);
    void on_actionExpandAll_triggered();
    void on_actionCollapseAll_triggered();
    void on_actionAbout_triggered();
    void on_actionSettings_triggered();
    void on_actionJumpToHex_triggered();
    void on_actionHashListManager_triggered();
    void UpdateProgress(qint64 count);
    void UpdateExport(void);
    void UpdateDig(int digstateid, int digcount);
    //void UpdateCarve(QString partid, int carvecount);
    void OpenUpdate(void);
    void UpdateCheckCount(void);
    //void SelectionChanged(const QItemSelection &selitem, const QItemSelection &deselitem);
    void PathSelectionChanged(const QItemSelection &selitem, const QItemSelection &deselitem);
    void HideImageWindow(bool checkstate);
    void HideViewerManager(void);
    void HideHashListManager(void);
    void HideTagManager(void);
    void HideSettingsWindow(void);
    void HideMessageViewer(bool checkstate);
    void HideByteViewer(bool checkstate);
    void HidePreviewReport(bool checkstate);
    void ShowExternalViewer();
    void ShowForImgDialog();
    void HideVerifyWindow(bool checkstate);
    void DisplayError(QString errorNumber, QString errorType, QString errorValue);
    void ResizeColumns(void);
    void ResizeViewColumns(const QModelIndex &index)
    {
        if(index.isValid())
            ResizeColumns();
    };
    void ExportFiles(int exporttype, bool originalpath, QString exportpath);
    //void ExportToForensicImage(int exporttype, bool originalpath, QString exportpath);
    void ExportForensicImage(void);
    //void VerifyForensicImage(void);
    void DigFiles(int digtype, QVector<int> digoptions);
    void GetHashComparisons(QStringList whlfiles);
    void StartCarving(QStringList plist, QStringList flist);
    void SetOffsetLabel(qint64 pos);
    void HexSelectionChanged(void);
    void UpdateSelectValue(void);
    void UpdateDataTable(void);
    void UpdateStatus(void);
    void FinishExport(void);
    void FinishCarve(void);
    void FinishDigging(void);
    void FinishPreDigging(void);
    void ThashFinish(void);
    void ThashSaveFinish(void);
    void ThreadCancelled(void);
    void ReadXMountOut(void);
    void ReadXMountErr(void);
    void StatusUpdate(QString tmptext)
    {
        statuslabel->setText(tmptext);
    };
    void TreeContextMenu(const QPoint &point);
    void HeaderContextMenu(const QPoint &point);
    void ImgHexMenu(const QPoint &point);
    void SetFilter(int headercolumn);
    void FilterApplied()
    {
        UpdateFilterCount();
        emit pathtreeview->header()->geometriesChanged();
        emit treenodemodel->layoutChanged(); // this messes with the row height and cuts off icons...
    };
    void NextItem();
    void PreviousItem();
    void ShowItem();
    void SetSelectedFromImageViewer(QString selectedid);
    void ShowFile(const QModelIndex &index);
    void AddSection(void);
    //void AddTextSection(void);
    void CarveFile(void);
    void PublishResults(void);
    void AutoSaveState(void);
    //void RotateDig(void);
    //void RotateCarve(void);
    void OpenCaseMountFinished(int exitcode, QProcess::ExitStatus exitstatus);
    void PrepareEvidenceImage(void);
    void SetHexOffset(void);
    void CreateNewTag(void);
    void CreateEmptyHashList(void);
    void AddExistingHashList(void);
    void SetBookmark(void);
    void RemoveTag(void);
    void UnCheckChecked(void);
    void ReadSettings(void);
    void ReadBookmarks(void);
    void ReadHashLists(void);
    void ShowSearchDialog(void);
    void TagSection(QString ctitle, QString ctag);
    void LaunchChomp(void);
    void UpdateTimeZone(QString newtz);
    void FinishWombatCaseFile(void);
    void FinishVerify(void);
    void ShowHideColumn(void);
    void VerifyUpdate(qint64 size);

    void SetRootIndex(void);

protected:
    void closeEvent(QCloseEvent* event);
    void mouseDoubleClickEvent(QMouseEvent* event);
    //void mousePressEvent(QMouseEvent* event);
private:
    Ui::WombatForensics *ui;

    void SetupHexPage(void);
    void InitializeAppStructure(void);
    void InitializeCaseStructure(void);
    void InitializeOpenCase(void);
    void InitializePreviewReport(void);
    void CloseCurrentCase(void);
    void UpdateProperties(void);
    void LoadHexContents(void);
    void PopulateHexContents(void);
    void GenerateHexFile(const QModelIndex index);
    QStringList GetFileLists(int filelisttype);
    void ProcessDig(QString curid);
    void UpdateFilterCount(void);
    void SaveState(void);
    void RemoveTmpFiles(void);
    void UpdateCheckState(void);
    void InitializeCheckState(void);
    void UpdateSelectedState(QString id);
    void TagFile(QModelIndex curindex, QString tagname);
    static void PopulateModel(const QModelIndex index);
    void SaveTreeModel(void);
    void PrintTree(int level, const QModelIndex& index, QTextStream& stream);
    QString InitializeSelectedState(void);
    QModelIndex selectedindex;
    QModelIndex oldselectedindex;

    //QFuture<void> sqlfuture;
    QFutureWatcher<void> volwatcher;
    //QFutureWatcher<void> sqlwatcher;
    QFuture<void> digfuture;
    QFutureWatcher<void> digwatcher;
    QFuture<void> predigfuture;
    QFutureWatcher<void> predigwatcher;
    QFutureWatcher<void> carvewatcher;
    QFutureWatcher<void> thashwatcher;
    QFutureWatcher<void> thashsavewatcher;
    QFutureWatcher<void> exportwatcher;
    QFuture<void> openfuture;
    QFutureWatcher<void> openwatcher;
    QFutureWatcher<void> savewcfwatcher;
    //QFuture<std::string> verfuture;
    //QFutureWatcher<std::string> verifywatcher;

    QProcess* xmntprocess;

    QFile casedatafile;
    // QFile carvingfiletypesfile; maybe do or don't need here, I might get this from a resource
    off_t offset() const;
    QPushButton* lineup;
    QPushButton* linedown;
    QPushButton* pageup;
    QPushButton* pagedown;
    QLabel* selectedoffset;
    QLabel* selectedhex;
    QLabel* filecountlabel;
    QLabel* filtercountlabel;
    QLabel* checkedcountlabel;
    QLabel* digcountlabel;
    QLabel* carvestatuslabel;
    StatusLabel* statuslabel;
    PathTreeView* pathtreeview;
    QFrame* vline1;
    QFrame* vline2;
    QFrame* vline3;
    int exporttype;
    int digtype;
    int digtimercounter = 0;
    //int carvetimercounter = 0;
    bool isvidthumb = false;
    bool isimgthumb = false;
    bool isreport = false;
    QVector<int> digoptions;
    QShortcut* jumpforward;
    QShortcut* jumpbackward;
    QShortcut* showitem;
    QTimer* autosavetimer;
    //QTimer* digrotatetimer; 
    //QTimer* carverotatetimer;
    TreeNode* actionitem;
    QWidget* cancelwidget;
    qint64 digimgthumbtotal = 0;
    qint64 digvidthumbtotal = 0;
    qint64 dighashtotal = 0;
    qint64 digarchivetotal = 0;
    qint64 digtotalcount = 0;
    QString digimgcountstring = "";
    QString digvidcountstring = "";
    QString dighashcountstring = "";
    QString digarchivecountstring = "";
    QString digtotalcountstring = "";
    QString currentreportpath = "";
    QHash<QString, int>carvecounthash;
    QStringList whlcomparisonlist;
    //std::vector<struct fuse_session*> fuserlist;
    //std::vector<ForensicImage*> evidimglist;
    //std::vector<struct fuse*> fuserlist;
    //pthread_t fusethread;
};

#endif // WOMBATFORENSICS_H
