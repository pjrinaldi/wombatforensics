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
#include "qhexview.h"
#include "addevidencedialog.h"
#include "digstatus.h"

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

class StatusLabel : public QLabel
{
    Q_OBJECT

    public:
        void ShowDigStatus(void);

    protected:
        void mousePressEvent(QMouseEvent* e)
        {
            emit clicked();
            e->accept();
        }

    signals:
        void clicked();
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
    QMenu* treemenu;
    QMenu* selectionmenu;
    QMenu* bookmarkmenu;
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
    AddEvidenceDialog* addevidencedialog;
    DigStatus* digstatusdialog;
    //QHexDocument* curhexdoc;
    //QHexView* hexviewer;

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
    void on_actionJumpToHex_triggered();
    void UpdateProgress(unsigned long long count);
    void UpdateExport(void);
    void UpdateDig(int digstateid, int digcount);
    void HashingFinish(void);
    void OpenUpdate(void);
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
    void ThashFinish(void);
    void ThashSaveFinish(void);
    void ThreadCancelled(void);
    void ReadXMountOut(void);
    void ReadXMountErr(void);
    void ShowDigStatus(void);
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
    void SetSelectedFromImageViewer(QString selectedid);
    void ShowFile(const QModelIndex &index);
    void AddSection(void);
    void AddTextSection(void);
    void CarveFile(void);
    void AutoSaveState(void);
    void OpenCaseMountFinished(int exitcode, QProcess::ExitStatus exitstatus);
    void PrepareEvidenceImage(void);
    void SetHexOffset(void);

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
    void GenerateHexFile(const QModelIndex index);
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
    QFuture<void> hashingfuture;
    QFutureWatcher<void> hashingwatcher;
    QFutureWatcher<void> thashwatcher;
    QFutureWatcher<void> thashsavewatcher;
    QFutureWatcher<void> exportwatcher;
    QFutureWatcher<void> digwatcher;
    QFuture<void> openfuture;
    QFutureWatcher<void> openwatcher;

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
    //QToolButton* statuslabel;
    StatusLabel* statuslabel;
    QFrame* vline1;
    QFrame* vline2;
    int exporttype;
    int digtype;
    QVector<int> digoptions;
    QShortcut* jumpforward;
    QShortcut* jumpbackward;
    QShortcut* showitem;
    QTimer* autosavetimer;
    TreeNode* actionitem;
    QWidget* cancelwidget;

    // outside in variables
    SCCERR oiiniterr;
    SCCERR oiopndocerr;
    SCCERR oiopnexperr;
    SCCERR oirunexperr;
    SCCERR oiclsexperr;
    SCCERR oiclsdocerr;
    VTHDOC oidoc;
    VTHEXPORT oiexport;
};

SCCERR ExportCallback(VTHEXPORT hExport, VTSYSPARAM dwCallbackData, VTDWORD dwCommandID, VTLPVOID pCommandData);
void SetOptionDWORD(VTHDOC target, VTDWORD optionId, VTDWORD val);
void SetOptionBOOL(VTHDOC target, VTDWORD optionId, VTBOOL val);
void SetOptionString(VTHDOC target, VTDWORD optionId, const char* val);

#endif // WOMBATFORENSICS_H
