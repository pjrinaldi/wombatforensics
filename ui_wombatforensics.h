/********************************************************************************
** Form generated from reading UI file 'wombatforensics.ui'
**
** Created by: Qt User Interface Compiler version 5.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WOMBATFORENSICS_H
#define UI_WOMBATFORENSICS_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QTreeView>
#include <QtWidgets/QWidget>
#include "qhexedit.h"

QT_BEGIN_NAMESPACE

class Ui_WombatForensics
{
public:
    QAction *actionNew_Case;
    QAction *actionOpen_Case;
    QAction *actionView_Progress;
    QAction *actionExit;
    QAction *actionAdd_Evidence;
    QAction *actionRemove_Evidence;
    QAction *actionExport_Evidence;
    QAction *actionManage_OmniViewer;
    QAction *actionView_Properties;
    QAction *actionActionAbout;
    QAction *actionAbout;
    QAction *actionHelp;
    QAction *actionNew_Bookmark;
    QAction *actionExisting_Bookmarks;
    QAction *actionView_File;
    QAction *actionView_Image_Gallery;
    QAction *actionViewerManager;
    QAction *actionPlaceHolder;
    QAction *actionCopy_Selection_To;
    QAction *actionSection;
    QAction *actionFile;
    QAction *actionTextSection;
    QAction *actionTextViewer;
    QAction *actionViewMessageLog;
    QAction *actionExpandAll;
    QAction *actionCollapseAll;
    QAction *actionDigDeeper;
    QAction *actionByteConverter;
    QAction *actionCheck;
    QAction *actionExport;
    QAction *actionSaveState;
    QAction *actionHtmlViewer;
    QAction *actionMediaViewer;
    QWidget *centralwidget;
    QHBoxLayout *horizontalLayout;
    QSplitter *splitter;
    QTreeView *dirTreeView;
    QHexEdit *hexview;
    QMenuBar *mainMenubar;
    QMenu *menuFile;
    QMenu *menuEvidence;
    QMenu *menuAction;
    QMenu *menuSettings;
    QMenu *menuAbout;
    QMenu *menuBookmark_Manager;
    QMenu *menuAdd_File_to;
    QMenu *menuView;
    QMenu *menuView_With;
    QMenu *menuCopy_To;
    QToolBar *analysisToolBar;
    QStatusBar *mainStatusBar;

    void setupUi(QMainWindow *WombatForensics)
    {
        if (WombatForensics->objectName().isEmpty())
            WombatForensics->setObjectName(QStringLiteral("WombatForensics"));
        WombatForensics->resize(1366, 768);
        actionNew_Case = new QAction(WombatForensics);
        actionNew_Case->setObjectName(QStringLiteral("actionNew_Case"));
        QIcon icon;
        icon.addFile(QStringLiteral(":/bar/newcase"), QSize(), QIcon::Normal, QIcon::Off);
        actionNew_Case->setIcon(icon);
        actionOpen_Case = new QAction(WombatForensics);
        actionOpen_Case->setObjectName(QStringLiteral("actionOpen_Case"));
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/bar/opencase"), QSize(), QIcon::Normal, QIcon::Off);
        actionOpen_Case->setIcon(icon1);
        actionView_Progress = new QAction(WombatForensics);
        actionView_Progress->setObjectName(QStringLiteral("actionView_Progress"));
        actionView_Progress->setCheckable(true);
        QIcon icon2;
        icon2.addFile(QStringLiteral(":/bar/window"), QSize(), QIcon::Normal, QIcon::Off);
        actionView_Progress->setIcon(icon2);
        actionView_Progress->setVisible(false);
        actionExit = new QAction(WombatForensics);
        actionExit->setObjectName(QStringLiteral("actionExit"));
        actionAdd_Evidence = new QAction(WombatForensics);
        actionAdd_Evidence->setObjectName(QStringLiteral("actionAdd_Evidence"));
        QIcon icon3;
        icon3.addFile(QStringLiteral(":/bar/bwadd"), QSize(), QIcon::Normal, QIcon::Off);
        actionAdd_Evidence->setIcon(icon3);
        actionRemove_Evidence = new QAction(WombatForensics);
        actionRemove_Evidence->setObjectName(QStringLiteral("actionRemove_Evidence"));
        QIcon icon4;
        icon4.addFile(QStringLiteral(":/bar/bwrem"), QSize(), QIcon::Normal, QIcon::Off);
        actionRemove_Evidence->setIcon(icon4);
        actionExport_Evidence = new QAction(WombatForensics);
        actionExport_Evidence->setObjectName(QStringLiteral("actionExport_Evidence"));
        QIcon icon5;
        icon5.addFile(QStringLiteral(":/bar/export"), QSize(), QIcon::Normal, QIcon::Off);
        actionExport_Evidence->setIcon(icon5);
        actionManage_OmniViewer = new QAction(WombatForensics);
        actionManage_OmniViewer->setObjectName(QStringLiteral("actionManage_OmniViewer"));
        actionView_Properties = new QAction(WombatForensics);
        actionView_Properties->setObjectName(QStringLiteral("actionView_Properties"));
        actionView_Properties->setCheckable(true);
        QIcon icon6;
        icon6.addFile(QStringLiteral(":/bar/propview"), QSize(), QIcon::Normal, QIcon::Off);
        actionView_Properties->setIcon(icon6);
        actionActionAbout = new QAction(WombatForensics);
        actionActionAbout->setObjectName(QStringLiteral("actionActionAbout"));
        QIcon icon7;
        icon7.addFile(QStringLiteral(":/bar/about"), QSize(), QIcon::Normal, QIcon::Off);
        actionActionAbout->setIcon(icon7);
        actionAbout = new QAction(WombatForensics);
        actionAbout->setObjectName(QStringLiteral("actionAbout"));
        actionAbout->setIcon(icon7);
        actionHelp = new QAction(WombatForensics);
        actionHelp->setObjectName(QStringLiteral("actionHelp"));
        actionNew_Bookmark = new QAction(WombatForensics);
        actionNew_Bookmark->setObjectName(QStringLiteral("actionNew_Bookmark"));
        QIcon icon8;
        icon8.addFile(QStringLiteral(":/bar/addtonewbkmrk"), QSize(), QIcon::Normal, QIcon::Off);
        actionNew_Bookmark->setIcon(icon8);
        actionExisting_Bookmarks = new QAction(WombatForensics);
        actionExisting_Bookmarks->setObjectName(QStringLiteral("actionExisting_Bookmarks"));
        QIcon icon9;
        icon9.addFile(QStringLiteral(":/bar/addtobkmrk"), QSize(), QIcon::Normal, QIcon::Off);
        actionExisting_Bookmarks->setIcon(icon9);
        actionView_File = new QAction(WombatForensics);
        actionView_File->setObjectName(QStringLiteral("actionView_File"));
        actionView_File->setCheckable(true);
        QIcon icon10;
        icon10.addFile(QStringLiteral(":/bar/fileview"), QSize(), QIcon::Normal, QIcon::Off);
        actionView_File->setIcon(icon10);
        actionView_Image_Gallery = new QAction(WombatForensics);
        actionView_Image_Gallery->setObjectName(QStringLiteral("actionView_Image_Gallery"));
        actionView_Image_Gallery->setCheckable(true);
        QIcon icon11;
        icon11.addFile(QStringLiteral(":/bar/bwimageview"), QSize(), QIcon::Normal, QIcon::Off);
        actionView_Image_Gallery->setIcon(icon11);
        actionViewerManager = new QAction(WombatForensics);
        actionViewerManager->setObjectName(QStringLiteral("actionViewerManager"));
        QIcon icon12;
        icon12.addFile(QStringLiteral(":/bar/viewermanager"), QSize(), QIcon::Normal, QIcon::Off);
        actionViewerManager->setIcon(icon12);
        actionPlaceHolder = new QAction(WombatForensics);
        actionPlaceHolder->setObjectName(QStringLiteral("actionPlaceHolder"));
        actionPlaceHolder->setEnabled(false);
        actionPlaceHolder->setVisible(false);
        actionCopy_Selection_To = new QAction(WombatForensics);
        actionCopy_Selection_To->setObjectName(QStringLiteral("actionCopy_Selection_To"));
        QIcon icon13;
        icon13.addFile(QStringLiteral(":/bar/copyselect"), QSize(), QIcon::Normal, QIcon::Off);
        actionCopy_Selection_To->setIcon(icon13);
        actionCopy_Selection_To->setShortcutContext(Qt::WindowShortcut);
        actionCopy_Selection_To->setAutoRepeat(true);
        actionCopy_Selection_To->setMenuRole(QAction::TextHeuristicRole);
        actionSection = new QAction(WombatForensics);
        actionSection->setObjectName(QStringLiteral("actionSection"));
        actionFile = new QAction(WombatForensics);
        actionFile->setObjectName(QStringLiteral("actionFile"));
        actionTextSection = new QAction(WombatForensics);
        actionTextSection->setObjectName(QStringLiteral("actionTextSection"));
        actionTextViewer = new QAction(WombatForensics);
        actionTextViewer->setObjectName(QStringLiteral("actionTextViewer"));
        actionTextViewer->setCheckable(true);
        QIcon icon14;
        icon14.addFile(QStringLiteral(":/bar/textencode"), QSize(), QIcon::Normal, QIcon::Off);
        actionTextViewer->setIcon(icon14);
        actionViewMessageLog = new QAction(WombatForensics);
        actionViewMessageLog->setObjectName(QStringLiteral("actionViewMessageLog"));
        actionViewMessageLog->setCheckable(true);
        QIcon icon15;
        icon15.addFile(QStringLiteral(":/bar/logview"), QSize(), QIcon::Normal, QIcon::Off);
        actionViewMessageLog->setIcon(icon15);
        actionExpandAll = new QAction(WombatForensics);
        actionExpandAll->setObjectName(QStringLiteral("actionExpandAll"));
        QIcon icon16;
        icon16.addFile(QStringLiteral(":/bar/expandall"), QSize(), QIcon::Normal, QIcon::Off);
        actionExpandAll->setIcon(icon16);
        actionCollapseAll = new QAction(WombatForensics);
        actionCollapseAll->setObjectName(QStringLiteral("actionCollapseAll"));
        QIcon icon17;
        icon17.addFile(QStringLiteral(":/bar/collapseall"), QSize(), QIcon::Normal, QIcon::Off);
        actionCollapseAll->setIcon(icon17);
        actionDigDeeper = new QAction(WombatForensics);
        actionDigDeeper->setObjectName(QStringLiteral("actionDigDeeper"));
        QIcon icon18;
        icon18.addFile(QStringLiteral(":/bar/dig"), QSize(), QIcon::Normal, QIcon::Off);
        actionDigDeeper->setIcon(icon18);
        actionByteConverter = new QAction(WombatForensics);
        actionByteConverter->setObjectName(QStringLiteral("actionByteConverter"));
        actionByteConverter->setCheckable(true);
        QIcon icon19;
        icon19.addFile(QStringLiteral(":/bar/byteconverter"), QSize(), QIcon::Normal, QIcon::Off);
        actionByteConverter->setIcon(icon19);
        actionCheck = new QAction(WombatForensics);
        actionCheck->setObjectName(QStringLiteral("actionCheck"));
        QIcon icon20;
        icon20.addFile(QStringLiteral(":/bar/checkbox"), QSize(), QIcon::Normal, QIcon::Off);
        actionCheck->setIcon(icon20);
        actionExport = new QAction(WombatForensics);
        actionExport->setObjectName(QStringLiteral("actionExport"));
        actionExport->setIcon(icon5);
        actionSaveState = new QAction(WombatForensics);
        actionSaveState->setObjectName(QStringLiteral("actionSaveState"));
        QIcon icon21;
        icon21.addFile(QStringLiteral(":/bar/savestate"), QSize(), QIcon::Normal, QIcon::Off);
        actionSaveState->setIcon(icon21);
        actionHtmlViewer = new QAction(WombatForensics);
        actionHtmlViewer->setObjectName(QStringLiteral("actionHtmlViewer"));
        actionHtmlViewer->setCheckable(true);
        QIcon icon22;
        icon22.addFile(QStringLiteral(":/bar/htmlviewer"), QSize(), QIcon::Normal, QIcon::Off);
        actionHtmlViewer->setIcon(icon22);
        actionMediaViewer = new QAction(WombatForensics);
        actionMediaViewer->setObjectName(QStringLiteral("actionMediaViewer"));
        actionMediaViewer->setCheckable(true);
        QIcon icon23;
        icon23.addFile(QStringLiteral(":/bar/mediaviewer"), QSize(), QIcon::Normal, QIcon::Off);
        actionMediaViewer->setIcon(icon23);
        centralwidget = new QWidget(WombatForensics);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        horizontalLayout = new QHBoxLayout(centralwidget);
        horizontalLayout->setSpacing(0);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        splitter = new QSplitter(centralwidget);
        splitter->setObjectName(QStringLiteral("splitter"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(splitter->sizePolicy().hasHeightForWidth());
        splitter->setSizePolicy(sizePolicy);
        splitter->setOrientation(Qt::Vertical);
        splitter->setChildrenCollapsible(false);
        dirTreeView = new QTreeView(splitter);
        dirTreeView->setObjectName(QStringLiteral("dirTreeView"));
        dirTreeView->setEditTriggers(QAbstractItemView::NoEditTriggers);
        dirTreeView->setProperty("showDropIndicator", QVariant(false));
        dirTreeView->setAlternatingRowColors(true);
        dirTreeView->setHorizontalScrollMode(QAbstractItemView::ScrollPerItem);
        dirTreeView->setUniformRowHeights(true);
        dirTreeView->setAllColumnsShowFocus(true);
        dirTreeView->setExpandsOnDoubleClick(false);
        splitter->addWidget(dirTreeView);
        hexview = new QHexEdit(splitter);
        hexview->setObjectName(QStringLiteral("hexview"));
        hexview->setBytesPerLine(41);
        hexview->setHexCaps(true);
        hexview->setDynamicBytesPerLine(true);
        hexview->setReadOnly(true);
        splitter->addWidget(hexview);

        horizontalLayout->addWidget(splitter);

        WombatForensics->setCentralWidget(centralwidget);
        mainMenubar = new QMenuBar(WombatForensics);
        mainMenubar->setObjectName(QStringLiteral("mainMenubar"));
        mainMenubar->setGeometry(QRect(0, 0, 1366, 21));
        mainMenubar->setAcceptDrops(true);
        menuFile = new QMenu(mainMenubar);
        menuFile->setObjectName(QStringLiteral("menuFile"));
        menuEvidence = new QMenu(mainMenubar);
        menuEvidence->setObjectName(QStringLiteral("menuEvidence"));
        menuAction = new QMenu(mainMenubar);
        menuAction->setObjectName(QStringLiteral("menuAction"));
        menuSettings = new QMenu(mainMenubar);
        menuSettings->setObjectName(QStringLiteral("menuSettings"));
        menuAbout = new QMenu(mainMenubar);
        menuAbout->setObjectName(QStringLiteral("menuAbout"));
        menuAbout->setIcon(icon7);
        menuBookmark_Manager = new QMenu(mainMenubar);
        menuBookmark_Manager->setObjectName(QStringLiteral("menuBookmark_Manager"));
        QIcon icon24;
        icon24.addFile(QStringLiteral(":/bar/bookmarkmgr"), QSize(), QIcon::Normal, QIcon::Off);
        menuBookmark_Manager->setIcon(icon24);
        menuAdd_File_to = new QMenu(menuBookmark_Manager);
        menuAdd_File_to->setObjectName(QStringLiteral("menuAdd_File_to"));
        QIcon icon25;
        icon25.addFile(QStringLiteral(":/bar/addfileto"), QSize(), QIcon::Normal, QIcon::Off);
        menuAdd_File_to->setIcon(icon25);
        menuView = new QMenu(mainMenubar);
        menuView->setObjectName(QStringLiteral("menuView"));
        menuView_With = new QMenu(menuView);
        menuView_With->setObjectName(QStringLiteral("menuView_With"));
        menuCopy_To = new QMenu(menuView);
        menuCopy_To->setObjectName(QStringLiteral("menuCopy_To"));
        WombatForensics->setMenuBar(mainMenubar);
        analysisToolBar = new QToolBar(WombatForensics);
        analysisToolBar->setObjectName(QStringLiteral("analysisToolBar"));
        analysisToolBar->setLayoutDirection(Qt::LeftToRight);
        analysisToolBar->setMovable(false);
        analysisToolBar->setAllowedAreas(Qt::NoToolBarArea);
        analysisToolBar->setIconSize(QSize(16, 16));
        analysisToolBar->setFloatable(false);
        WombatForensics->addToolBar(Qt::TopToolBarArea, analysisToolBar);
        mainStatusBar = new QStatusBar(WombatForensics);
        mainStatusBar->setObjectName(QStringLiteral("mainStatusBar"));
        mainStatusBar->setStyleSheet(QStringLiteral(""));
        mainStatusBar->setSizeGripEnabled(true);
        WombatForensics->setStatusBar(mainStatusBar);

        mainMenubar->addAction(menuFile->menuAction());
        mainMenubar->addAction(menuEvidence->menuAction());
        mainMenubar->addAction(menuAction->menuAction());
        mainMenubar->addAction(menuSettings->menuAction());
        mainMenubar->addAction(menuAbout->menuAction());
        mainMenubar->addAction(menuBookmark_Manager->menuAction());
        mainMenubar->addAction(menuView->menuAction());
        menuFile->addAction(actionNew_Case);
        menuFile->addAction(actionOpen_Case);
        menuFile->addSeparator();
        menuFile->addAction(actionView_Progress);
        menuFile->addSeparator();
        menuFile->addAction(actionExit);
        menuEvidence->addAction(actionAdd_Evidence);
        menuEvidence->addAction(actionRemove_Evidence);
        menuEvidence->addSeparator();
        menuAction->addAction(actionExport_Evidence);
        menuSettings->addAction(actionManage_OmniViewer);
        menuAbout->addAction(actionAbout);
        menuAbout->addAction(actionHelp);
        menuBookmark_Manager->addAction(menuAdd_File_to->menuAction());
        menuAdd_File_to->addAction(actionNew_Bookmark);
        menuAdd_File_to->addAction(actionExisting_Bookmarks);
        menuAdd_File_to->addSeparator();
        menuView->addAction(menuView_With->menuAction());
        menuView->addAction(menuCopy_To->menuAction());
        menuView->addAction(actionCheck);
        menuView->addAction(actionExport);
        menuView_With->addAction(actionPlaceHolder);
        menuCopy_To->addAction(actionSection);
        menuCopy_To->addAction(actionTextSection);
        menuCopy_To->addAction(actionFile);
        analysisToolBar->addAction(actionNew_Case);
        analysisToolBar->addAction(actionOpen_Case);
        analysisToolBar->addAction(actionSaveState);
        analysisToolBar->addSeparator();
        analysisToolBar->addAction(actionAdd_Evidence);
        analysisToolBar->addAction(actionRemove_Evidence);
        analysisToolBar->addAction(actionCollapseAll);
        analysisToolBar->addAction(actionExpandAll);
        analysisToolBar->addSeparator();
        analysisToolBar->addAction(actionView_Properties);
        analysisToolBar->addAction(actionView_File);
        analysisToolBar->addAction(actionTextViewer);
        analysisToolBar->addAction(actionHtmlViewer);
        analysisToolBar->addAction(actionMediaViewer);
        analysisToolBar->addAction(actionView_Image_Gallery);
        analysisToolBar->addAction(actionByteConverter);
        analysisToolBar->addSeparator();
        analysisToolBar->addAction(actionViewerManager);
        analysisToolBar->addAction(actionViewMessageLog);
        analysisToolBar->addAction(actionView_Progress);
        analysisToolBar->addSeparator();
        analysisToolBar->addAction(actionDigDeeper);
        analysisToolBar->addAction(actionCopy_Selection_To);
        analysisToolBar->addAction(actionExport_Evidence);

        retranslateUi(WombatForensics);
        QObject::connect(actionExit, SIGNAL(triggered()), WombatForensics, SLOT(close()));
        QObject::connect(actionAdd_Evidence, SIGNAL(triggered()), WombatForensics, SLOT(AddEvidence()));
        QObject::connect(actionRemove_Evidence, SIGNAL(triggered()), WombatForensics, SLOT(RemEvidence()));
        QObject::connect(actionExport_Evidence, SIGNAL(triggered()), WombatForensics, SLOT(ExportEvidence()));

        QMetaObject::connectSlotsByName(WombatForensics);
    } // setupUi

    void retranslateUi(QMainWindow *WombatForensics)
    {
        WombatForensics->setWindowTitle(QApplication::translate("WombatForensics", "WombatForensics", Q_NULLPTR));
        actionNew_Case->setText(QApplication::translate("WombatForensics", "New Case", Q_NULLPTR));
#ifndef QT_NO_SHORTCUT
        actionNew_Case->setShortcut(QApplication::translate("WombatForensics", "Ctrl+N", Q_NULLPTR));
#endif // QT_NO_SHORTCUT
        actionOpen_Case->setText(QApplication::translate("WombatForensics", "Open Case", Q_NULLPTR));
#ifndef QT_NO_SHORTCUT
        actionOpen_Case->setShortcut(QApplication::translate("WombatForensics", "Ctrl+O", Q_NULLPTR));
#endif // QT_NO_SHORTCUT
        actionView_Progress->setText(QApplication::translate("WombatForensics", "View Log...", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        actionView_Progress->setToolTip(QApplication::translate("WombatForensics", "Log Viewer", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        actionExit->setText(QApplication::translate("WombatForensics", "Exit", Q_NULLPTR));
        actionAdd_Evidence->setText(QApplication::translate("WombatForensics", "Add Evidence", Q_NULLPTR));
#ifndef QT_NO_SHORTCUT
        actionAdd_Evidence->setShortcut(QApplication::translate("WombatForensics", "Ctrl+A", Q_NULLPTR));
#endif // QT_NO_SHORTCUT
        actionRemove_Evidence->setText(QApplication::translate("WombatForensics", "Remove Evidence", Q_NULLPTR));
#ifndef QT_NO_SHORTCUT
        actionRemove_Evidence->setShortcut(QApplication::translate("WombatForensics", "Ctrl+D", Q_NULLPTR));
#endif // QT_NO_SHORTCUT
        actionExport_Evidence->setText(QApplication::translate("WombatForensics", "Export Evidence", Q_NULLPTR));
#ifndef QT_NO_SHORTCUT
        actionExport_Evidence->setShortcut(QApplication::translate("WombatForensics", "Ctrl+E", Q_NULLPTR));
#endif // QT_NO_SHORTCUT
        actionManage_OmniViewer->setText(QApplication::translate("WombatForensics", "Manage OmniViewer", Q_NULLPTR));
        actionView_Properties->setText(QApplication::translate("WombatForensics", "View Properties", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        actionView_Properties->setToolTip(QApplication::translate("WombatForensics", "Properties Window", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_SHORTCUT
        actionView_Properties->setShortcut(QApplication::translate("WombatForensics", "Ctrl+P", Q_NULLPTR));
#endif // QT_NO_SHORTCUT
        actionActionAbout->setText(QApplication::translate("WombatForensics", "actionAbout", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        actionActionAbout->setToolTip(QApplication::translate("WombatForensics", "About", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_SHORTCUT
        actionActionAbout->setShortcut(QApplication::translate("WombatForensics", "F1", Q_NULLPTR));
#endif // QT_NO_SHORTCUT
        actionAbout->setText(QApplication::translate("WombatForensics", "About", Q_NULLPTR));
        actionHelp->setText(QApplication::translate("WombatForensics", "Help", Q_NULLPTR));
        actionNew_Bookmark->setText(QApplication::translate("WombatForensics", "New Bookmark", Q_NULLPTR));
        actionExisting_Bookmarks->setText(QApplication::translate("WombatForensics", "Existing Bookmarks", Q_NULLPTR));
        actionView_File->setText(QApplication::translate("WombatForensics", "View File", Q_NULLPTR));
        actionView_Image_Gallery->setText(QApplication::translate("WombatForensics", "View Image Gallery", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        actionView_Image_Gallery->setToolTip(QApplication::translate("WombatForensics", "View Image Gallery", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        actionViewerManager->setText(QApplication::translate("WombatForensics", "Viewer Manager", Q_NULLPTR));
        actionPlaceHolder->setText(QApplication::translate("WombatForensics", "PlaceHolder", Q_NULLPTR));
        actionCopy_Selection_To->setText(QApplication::translate("WombatForensics", "Copy Selection To", Q_NULLPTR));
        actionSection->setText(QApplication::translate("WombatForensics", "Copy Hex to a Section", Q_NULLPTR));
        actionFile->setText(QApplication::translate("WombatForensics", "Carve to a File", Q_NULLPTR));
        actionTextSection->setText(QApplication::translate("WombatForensics", "Copy Ascii to a Section", Q_NULLPTR));
        actionTextViewer->setText(QApplication::translate("WombatForensics", "Text Viewer", Q_NULLPTR));
        actionViewMessageLog->setText(QApplication::translate("WombatForensics", "View Message Log", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        actionViewMessageLog->setToolTip(QApplication::translate("WombatForensics", "View Message Log", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        actionExpandAll->setText(QApplication::translate("WombatForensics", "Expand All", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        actionExpandAll->setToolTip(QApplication::translate("WombatForensics", "Expand All", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        actionCollapseAll->setText(QApplication::translate("WombatForensics", "Collapse All", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        actionCollapseAll->setToolTip(QApplication::translate("WombatForensics", "Collapse All", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        actionDigDeeper->setText(QApplication::translate("WombatForensics", "Dig Deeper", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        actionDigDeeper->setToolTip(QApplication::translate("WombatForensics", "Dig Deeper", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        actionByteConverter->setText(QApplication::translate("WombatForensics", "Byte Converter", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        actionByteConverter->setToolTip(QApplication::translate("WombatForensics", "Byte Converter", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        actionCheck->setText(QApplication::translate("WombatForensics", "(Un)Check", Q_NULLPTR));
        actionExport->setText(QApplication::translate("WombatForensics", "Export...", Q_NULLPTR));
        actionSaveState->setText(QApplication::translate("WombatForensics", "SaveState", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        actionSaveState->setToolTip(QApplication::translate("WombatForensics", "Save State", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_SHORTCUT
        actionSaveState->setShortcut(QApplication::translate("WombatForensics", "Ctrl+S", Q_NULLPTR));
#endif // QT_NO_SHORTCUT
        actionHtmlViewer->setText(QApplication::translate("WombatForensics", "Html Viewer", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        actionHtmlViewer->setToolTip(QApplication::translate("WombatForensics", "HTML Viewer", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        actionMediaViewer->setText(QApplication::translate("WombatForensics", "Media Viewer", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        actionMediaViewer->setToolTip(QApplication::translate("WombatForensics", "Audio and Video Media Viewer", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_TOOLTIP
        hexview->setToolTip(QApplication::translate("WombatForensics", "QHexEditWidget", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_WHATSTHIS
        hexview->setWhatsThis(QApplication::translate("WombatForensics", "QHexEdit widget allow to edit the data in hex view.", Q_NULLPTR));
#endif // QT_NO_WHATSTHIS
        menuFile->setTitle(QApplication::translate("WombatForensics", "File", Q_NULLPTR));
        menuEvidence->setTitle(QApplication::translate("WombatForensics", "Evidence", Q_NULLPTR));
        menuAction->setTitle(QApplication::translate("WombatForensics", "Action", Q_NULLPTR));
        menuSettings->setTitle(QApplication::translate("WombatForensics", "Settings", Q_NULLPTR));
        menuAbout->setTitle(QApplication::translate("WombatForensics", "About", Q_NULLPTR));
        menuBookmark_Manager->setTitle(QApplication::translate("WombatForensics", "Bookmark Manager", Q_NULLPTR));
        menuAdd_File_to->setTitle(QApplication::translate("WombatForensics", "Add Selected File to...", Q_NULLPTR));
        menuView->setTitle(QApplication::translate("WombatForensics", "View", Q_NULLPTR));
        menuView_With->setTitle(QApplication::translate("WombatForensics", "View With", Q_NULLPTR));
        menuCopy_To->setTitle(QApplication::translate("WombatForensics", "Copy to", Q_NULLPTR));
        analysisToolBar->setWindowTitle(QApplication::translate("WombatForensics", "toolBar", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class WombatForensics: public Ui_WombatForensics {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WOMBATFORENSICS_H
