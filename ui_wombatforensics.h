/********************************************************************************
** Form generated from reading UI file 'wombatforensics.ui'
**
** Created by: Qt User Interface Compiler version 5.3.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WOMBATFORENSICS_H
#define UI_WOMBATFORENSICS_H

#include <QtCore/QVariant>
#include <QtWebKitWidgets/QWebView>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QTreeView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_WombatForensics
{
public:
    QAction *actionNew_Case;
    QAction *actionOpen_Case;
    QAction *actionViewHex;
    QAction *actionViewTxt;
    QAction *actionViewOmni;
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
    QWidget *centralwidget;
    QHBoxLayout *horizontalLayout;
    QSplitter *splitter;
    QStackedWidget *viewerstack;
    QWidget *hexPage;
    QWidget *textPage;
    QWidget *webpage;
    QHBoxLayout *horizontalLayout_3;
    QWebView *webView;
    QWidget *picpage;
    QHBoxLayout *horizontalLayout_4;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    QHBoxLayout *horizontalLayout_5;
    QLabel *picViewer;
    QWidget *vidpage;
    QTreeView *dirTreeView;
    QMenuBar *mainMenubar;
    QMenu *menuFile;
    QMenu *menuEvidence;
    QMenu *menuAction;
    QMenu *menuSettings;
    QMenu *menuAbout;
    QMenu *menuBookmark_Manager;
    QMenu *menuAdd_File_to;
    QToolBar *analysisToolBar;
    QStatusBar *mainStatusBar;

    void setupUi(QMainWindow *WombatForensics)
    {
        if (WombatForensics->objectName().isEmpty())
            WombatForensics->setObjectName(QStringLiteral("WombatForensics"));
        WombatForensics->resize(800, 600);
        actionNew_Case = new QAction(WombatForensics);
        actionNew_Case->setObjectName(QStringLiteral("actionNew_Case"));
        QIcon icon;
        icon.addFile(QStringLiteral(":/newcase"), QSize(), QIcon::Normal, QIcon::Off);
        actionNew_Case->setIcon(icon);
        actionOpen_Case = new QAction(WombatForensics);
        actionOpen_Case->setObjectName(QStringLiteral("actionOpen_Case"));
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/opencase"), QSize(), QIcon::Normal, QIcon::Off);
        actionOpen_Case->setIcon(icon1);
        actionViewHex = new QAction(WombatForensics);
        actionViewHex->setObjectName(QStringLiteral("actionViewHex"));
        actionViewHex->setCheckable(true);
        actionViewHex->setChecked(true);
        QIcon icon2;
        icon2.addFile(QStringLiteral(":/bar/hexview"), QSize(), QIcon::Normal, QIcon::Off);
        actionViewHex->setIcon(icon2);
        actionViewHex->setAutoRepeat(false);
        actionViewTxt = new QAction(WombatForensics);
        actionViewTxt->setObjectName(QStringLiteral("actionViewTxt"));
        actionViewTxt->setCheckable(true);
        QIcon icon3;
        icon3.addFile(QStringLiteral(":/bar/txtview"), QSize(), QIcon::Normal, QIcon::Off);
        actionViewTxt->setIcon(icon3);
        actionViewTxt->setAutoRepeat(false);
        actionViewOmni = new QAction(WombatForensics);
        actionViewOmni->setObjectName(QStringLiteral("actionViewOmni"));
        actionViewOmni->setCheckable(true);
        QIcon icon4;
        icon4.addFile(QStringLiteral(":/bar/omniview"), QSize(), QIcon::Normal, QIcon::Off);
        actionViewOmni->setIcon(icon4);
        actionViewOmni->setAutoRepeat(false);
        actionView_Progress = new QAction(WombatForensics);
        actionView_Progress->setObjectName(QStringLiteral("actionView_Progress"));
        actionView_Progress->setCheckable(true);
        QIcon icon5;
        icon5.addFile(QStringLiteral(":/bar/window"), QSize(), QIcon::Normal, QIcon::Off);
        actionView_Progress->setIcon(icon5);
        actionExit = new QAction(WombatForensics);
        actionExit->setObjectName(QStringLiteral("actionExit"));
        actionAdd_Evidence = new QAction(WombatForensics);
        actionAdd_Evidence->setObjectName(QStringLiteral("actionAdd_Evidence"));
        QIcon icon6;
        icon6.addFile(QStringLiteral(":/basic/addevidence"), QSize(), QIcon::Normal, QIcon::Off);
        actionAdd_Evidence->setIcon(icon6);
        actionRemove_Evidence = new QAction(WombatForensics);
        actionRemove_Evidence->setObjectName(QStringLiteral("actionRemove_Evidence"));
        QIcon icon7;
        icon7.addFile(QStringLiteral(":/basic/remevidence"), QSize(), QIcon::Normal, QIcon::Off);
        actionRemove_Evidence->setIcon(icon7);
        actionExport_Evidence = new QAction(WombatForensics);
        actionExport_Evidence->setObjectName(QStringLiteral("actionExport_Evidence"));
        QIcon icon8;
        icon8.addFile(QStringLiteral(":/bar/export"), QSize(), QIcon::Normal, QIcon::Off);
        actionExport_Evidence->setIcon(icon8);
        actionManage_OmniViewer = new QAction(WombatForensics);
        actionManage_OmniViewer->setObjectName(QStringLiteral("actionManage_OmniViewer"));
        actionView_Properties = new QAction(WombatForensics);
        actionView_Properties->setObjectName(QStringLiteral("actionView_Properties"));
        actionView_Properties->setCheckable(true);
        QIcon icon9;
        icon9.addFile(QStringLiteral(":/bar/propview"), QSize(), QIcon::Normal, QIcon::Off);
        actionView_Properties->setIcon(icon9);
        actionActionAbout = new QAction(WombatForensics);
        actionActionAbout->setObjectName(QStringLiteral("actionActionAbout"));
        QIcon icon10;
        icon10.addFile(QStringLiteral(":/bar/about"), QSize(), QIcon::Normal, QIcon::Off);
        actionActionAbout->setIcon(icon10);
        actionAbout = new QAction(WombatForensics);
        actionAbout->setObjectName(QStringLiteral("actionAbout"));
        actionAbout->setIcon(icon10);
        actionHelp = new QAction(WombatForensics);
        actionHelp->setObjectName(QStringLiteral("actionHelp"));
        actionNew_Bookmark = new QAction(WombatForensics);
        actionNew_Bookmark->setObjectName(QStringLiteral("actionNew_Bookmark"));
        QIcon icon11;
        icon11.addFile(QStringLiteral(":/bar/addtonewbkmrk"), QSize(), QIcon::Normal, QIcon::Off);
        actionNew_Bookmark->setIcon(icon11);
        actionExisting_Bookmarks = new QAction(WombatForensics);
        actionExisting_Bookmarks->setObjectName(QStringLiteral("actionExisting_Bookmarks"));
        QIcon icon12;
        icon12.addFile(QStringLiteral(":/bar/addtobkmrk"), QSize(), QIcon::Normal, QIcon::Off);
        actionExisting_Bookmarks->setIcon(icon12);
        actionView_File = new QAction(WombatForensics);
        actionView_File->setObjectName(QStringLiteral("actionView_File"));
        actionView_File->setCheckable(true);
        QIcon icon13;
        icon13.addFile(QStringLiteral(":/bar/fileview"), QSize(), QIcon::Normal, QIcon::Off);
        actionView_File->setIcon(icon13);
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
        viewerstack = new QStackedWidget(splitter);
        viewerstack->setObjectName(QStringLiteral("viewerstack"));
        sizePolicy.setHeightForWidth(viewerstack->sizePolicy().hasHeightForWidth());
        viewerstack->setSizePolicy(sizePolicy);
        viewerstack->setFrameShape(QFrame::StyledPanel);
        viewerstack->setFrameShadow(QFrame::Sunken);
        hexPage = new QWidget();
        hexPage->setObjectName(QStringLiteral("hexPage"));
        sizePolicy.setHeightForWidth(hexPage->sizePolicy().hasHeightForWidth());
        hexPage->setSizePolicy(sizePolicy);
        viewerstack->addWidget(hexPage);
        textPage = new QWidget();
        textPage->setObjectName(QStringLiteral("textPage"));
        sizePolicy.setHeightForWidth(textPage->sizePolicy().hasHeightForWidth());
        textPage->setSizePolicy(sizePolicy);
        viewerstack->addWidget(textPage);
        webpage = new QWidget();
        webpage->setObjectName(QStringLiteral("webpage"));
        horizontalLayout_3 = new QHBoxLayout(webpage);
        horizontalLayout_3->setSpacing(0);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        horizontalLayout_3->setContentsMargins(0, 0, 0, 0);
        webView = new QWebView(webpage);
        webView->setObjectName(QStringLiteral("webView"));
        sizePolicy.setHeightForWidth(webView->sizePolicy().hasHeightForWidth());
        webView->setSizePolicy(sizePolicy);
        webView->setAcceptDrops(false);
        webView->setUrl(QUrl(QStringLiteral("about:blank")));

        horizontalLayout_3->addWidget(webView);

        viewerstack->addWidget(webpage);
        picpage = new QWidget();
        picpage->setObjectName(QStringLiteral("picpage"));
        sizePolicy.setHeightForWidth(picpage->sizePolicy().hasHeightForWidth());
        picpage->setSizePolicy(sizePolicy);
        horizontalLayout_4 = new QHBoxLayout(picpage);
        horizontalLayout_4->setSpacing(0);
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        horizontalLayout_4->setContentsMargins(0, 0, 0, 0);
        scrollArea = new QScrollArea(picpage);
        scrollArea->setObjectName(QStringLiteral("scrollArea"));
        scrollArea->setFrameShape(QFrame::NoFrame);
        scrollArea->setWidgetResizable(true);
        scrollArea->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QStringLiteral("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 100, 30));
        sizePolicy.setHeightForWidth(scrollAreaWidgetContents->sizePolicy().hasHeightForWidth());
        scrollAreaWidgetContents->setSizePolicy(sizePolicy);
        horizontalLayout_5 = new QHBoxLayout(scrollAreaWidgetContents);
        horizontalLayout_5->setSpacing(0);
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        horizontalLayout_5->setContentsMargins(0, 0, 0, 0);
        picViewer = new QLabel(scrollAreaWidgetContents);
        picViewer->setObjectName(QStringLiteral("picViewer"));
        sizePolicy.setHeightForWidth(picViewer->sizePolicy().hasHeightForWidth());
        picViewer->setSizePolicy(sizePolicy);
        picViewer->setTextFormat(Qt::PlainText);

        horizontalLayout_5->addWidget(picViewer);

        scrollArea->setWidget(scrollAreaWidgetContents);

        horizontalLayout_4->addWidget(scrollArea);

        viewerstack->addWidget(picpage);
        vidpage = new QWidget();
        vidpage->setObjectName(QStringLiteral("vidpage"));
        sizePolicy.setHeightForWidth(vidpage->sizePolicy().hasHeightForWidth());
        vidpage->setSizePolicy(sizePolicy);
        viewerstack->addWidget(vidpage);
        splitter->addWidget(viewerstack);
        dirTreeView = new QTreeView(splitter);
        dirTreeView->setObjectName(QStringLiteral("dirTreeView"));
        dirTreeView->setEditTriggers(QAbstractItemView::NoEditTriggers);
        dirTreeView->setProperty("showDropIndicator", QVariant(false));
        dirTreeView->setAlternatingRowColors(true);
        dirTreeView->setExpandsOnDoubleClick(false);
        splitter->addWidget(dirTreeView);

        horizontalLayout->addWidget(splitter);

        WombatForensics->setCentralWidget(centralwidget);
        mainMenubar = new QMenuBar(WombatForensics);
        mainMenubar->setObjectName(QStringLiteral("mainMenubar"));
        mainMenubar->setGeometry(QRect(0, 0, 800, 22));
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
        menuAbout->setIcon(icon10);
        menuBookmark_Manager = new QMenu(mainMenubar);
        menuBookmark_Manager->setObjectName(QStringLiteral("menuBookmark_Manager"));
        QIcon icon14;
        icon14.addFile(QStringLiteral(":/bar/bookmarkmgr"), QSize(), QIcon::Normal, QIcon::Off);
        menuBookmark_Manager->setIcon(icon14);
        menuAdd_File_to = new QMenu(menuBookmark_Manager);
        menuAdd_File_to->setObjectName(QStringLiteral("menuAdd_File_to"));
        QIcon icon15;
        icon15.addFile(QStringLiteral(":/bar/addfileto"), QSize(), QIcon::Normal, QIcon::Off);
        menuAdd_File_to->setIcon(icon15);
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
        analysisToolBar->addAction(actionNew_Case);
        analysisToolBar->addAction(actionOpen_Case);
        analysisToolBar->addSeparator();
        analysisToolBar->addAction(actionAdd_Evidence);
        analysisToolBar->addAction(actionRemove_Evidence);
        analysisToolBar->addSeparator();
        analysisToolBar->addAction(actionViewHex);
        analysisToolBar->addAction(actionViewTxt);
        analysisToolBar->addAction(actionViewOmni);
        analysisToolBar->addSeparator();
        analysisToolBar->addAction(actionView_Properties);
        analysisToolBar->addAction(actionView_File);
        analysisToolBar->addAction(actionView_Progress);
        analysisToolBar->addSeparator();
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
        WombatForensics->setWindowTitle(QApplication::translate("WombatForensics", "WombatForensics", 0));
        actionNew_Case->setText(QApplication::translate("WombatForensics", "New Case", 0));
        actionNew_Case->setShortcut(QApplication::translate("WombatForensics", "Ctrl+N", 0));
        actionOpen_Case->setText(QApplication::translate("WombatForensics", "Open Case", 0));
        actionOpen_Case->setShortcut(QApplication::translate("WombatForensics", "Ctrl+O", 0));
        actionViewHex->setText(QApplication::translate("WombatForensics", "View Hex", 0));
        actionViewHex->setShortcut(QApplication::translate("WombatForensics", "Ctrl+1", 0));
        actionViewTxt->setText(QApplication::translate("WombatForensics", "View Text", 0));
        actionViewTxt->setShortcut(QApplication::translate("WombatForensics", "Ctrl+2", 0));
        actionViewOmni->setText(QApplication::translate("WombatForensics", "View Omni", 0));
        actionViewOmni->setShortcut(QApplication::translate("WombatForensics", "Ctrl+3", 0));
        actionView_Progress->setText(QApplication::translate("WombatForensics", "View Log...", 0));
#ifndef QT_NO_TOOLTIP
        actionView_Progress->setToolTip(QApplication::translate("WombatForensics", "Log Viewer", 0));
#endif // QT_NO_TOOLTIP
        actionExit->setText(QApplication::translate("WombatForensics", "Exit", 0));
        actionAdd_Evidence->setText(QApplication::translate("WombatForensics", "Add Evidence", 0));
        actionAdd_Evidence->setShortcut(QApplication::translate("WombatForensics", "Ctrl+A", 0));
        actionRemove_Evidence->setText(QApplication::translate("WombatForensics", "Remove Evidence", 0));
        actionRemove_Evidence->setShortcut(QApplication::translate("WombatForensics", "Ctrl+D", 0));
        actionExport_Evidence->setText(QApplication::translate("WombatForensics", "Export Evidence", 0));
        actionExport_Evidence->setShortcut(QApplication::translate("WombatForensics", "Ctrl+E", 0));
        actionManage_OmniViewer->setText(QApplication::translate("WombatForensics", "Manage OmniViewer", 0));
        actionView_Properties->setText(QApplication::translate("WombatForensics", "View Properties", 0));
#ifndef QT_NO_TOOLTIP
        actionView_Properties->setToolTip(QApplication::translate("WombatForensics", "Properties Window", 0));
#endif // QT_NO_TOOLTIP
        actionView_Properties->setShortcut(QApplication::translate("WombatForensics", "Ctrl+P", 0));
        actionActionAbout->setText(QApplication::translate("WombatForensics", "actionAbout", 0));
#ifndef QT_NO_TOOLTIP
        actionActionAbout->setToolTip(QApplication::translate("WombatForensics", "About", 0));
#endif // QT_NO_TOOLTIP
        actionActionAbout->setShortcut(QApplication::translate("WombatForensics", "F1", 0));
        actionAbout->setText(QApplication::translate("WombatForensics", "About", 0));
        actionHelp->setText(QApplication::translate("WombatForensics", "Help", 0));
        actionNew_Bookmark->setText(QApplication::translate("WombatForensics", "New Bookmark", 0));
        actionExisting_Bookmarks->setText(QApplication::translate("WombatForensics", "Existing Bookmarks", 0));
        actionView_File->setText(QApplication::translate("WombatForensics", "View File", 0));
        picViewer->setText(QString());
        menuFile->setTitle(QApplication::translate("WombatForensics", "File", 0));
        menuEvidence->setTitle(QApplication::translate("WombatForensics", "Evidence", 0));
        menuAction->setTitle(QApplication::translate("WombatForensics", "Action", 0));
        menuSettings->setTitle(QApplication::translate("WombatForensics", "Settings", 0));
        menuAbout->setTitle(QApplication::translate("WombatForensics", "About", 0));
        menuBookmark_Manager->setTitle(QApplication::translate("WombatForensics", "Bookmark Manager", 0));
        menuAdd_File_to->setTitle(QApplication::translate("WombatForensics", "Add Selected File to...", 0));
        analysisToolBar->setWindowTitle(QApplication::translate("WombatForensics", "toolBar", 0));
    } // retranslateUi

};

namespace Ui {
    class WombatForensics: public Ui_WombatForensics {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WOMBATFORENSICS_H
