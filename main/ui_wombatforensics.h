/********************************************************************************
** Form generated from reading UI file 'wombatforensics.ui'
**
** Created by: Qt User Interface Compiler version 5.1.0
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
#include <QtWidgets/QToolBar>
#include <QtWidgets/QTreeView>
#include <QtWidgets/QWidget>

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
    QWidget *centralwidget;
    QHBoxLayout *horizontalLayout;
    QSplitter *splitter;
    QStackedWidget *viewerStack;
    QWidget *hexPage;
    QWidget *textPage;
    QWidget *omniPage;
    QHBoxLayout *horizontalLayout_2;
    QStackedWidget *stackedWidget;
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
    QToolBar *analysisToolBar;

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
        actionView_Progress = new QAction(WombatForensics);
        actionView_Progress->setObjectName(QStringLiteral("actionView_Progress"));
        actionView_Progress->setCheckable(true);
        QIcon icon2;
        icon2.addFile(QStringLiteral(":/progresswindow/window"), QSize(), QIcon::Normal, QIcon::Off);
        actionView_Progress->setIcon(icon2);
        actionExit = new QAction(WombatForensics);
        actionExit->setObjectName(QStringLiteral("actionExit"));
        actionAdd_Evidence = new QAction(WombatForensics);
        actionAdd_Evidence->setObjectName(QStringLiteral("actionAdd_Evidence"));
        QIcon icon3;
        icon3.addFile(QStringLiteral(":/basic/addevidence"), QSize(), QIcon::Normal, QIcon::Off);
        actionAdd_Evidence->setIcon(icon3);
        actionRemove_Evidence = new QAction(WombatForensics);
        actionRemove_Evidence->setObjectName(QStringLiteral("actionRemove_Evidence"));
        QIcon icon4;
        icon4.addFile(QStringLiteral(":/basic/remevidence"), QSize(), QIcon::Normal, QIcon::Off);
        actionRemove_Evidence->setIcon(icon4);
        actionExport_Evidence = new QAction(WombatForensics);
        actionExport_Evidence->setObjectName(QStringLiteral("actionExport_Evidence"));
        QIcon icon5;
        icon5.addFile(QStringLiteral(":/basic/export"), QSize(), QIcon::Normal, QIcon::Off);
        actionExport_Evidence->setIcon(icon5);
        actionManage_OmniViewer = new QAction(WombatForensics);
        actionManage_OmniViewer->setObjectName(QStringLiteral("actionManage_OmniViewer"));
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
        viewerStack = new QStackedWidget(splitter);
        viewerStack->setObjectName(QStringLiteral("viewerStack"));
        sizePolicy.setHeightForWidth(viewerStack->sizePolicy().hasHeightForWidth());
        viewerStack->setSizePolicy(sizePolicy);
        hexPage = new QWidget();
        hexPage->setObjectName(QStringLiteral("hexPage"));
        sizePolicy.setHeightForWidth(hexPage->sizePolicy().hasHeightForWidth());
        hexPage->setSizePolicy(sizePolicy);
        viewerStack->addWidget(hexPage);
        textPage = new QWidget();
        textPage->setObjectName(QStringLiteral("textPage"));
        sizePolicy.setHeightForWidth(textPage->sizePolicy().hasHeightForWidth());
        textPage->setSizePolicy(sizePolicy);
        viewerStack->addWidget(textPage);
        omniPage = new QWidget();
        omniPage->setObjectName(QStringLiteral("omniPage"));
        sizePolicy.setHeightForWidth(omniPage->sizePolicy().hasHeightForWidth());
        omniPage->setSizePolicy(sizePolicy);
        horizontalLayout_2 = new QHBoxLayout(omniPage);
        horizontalLayout_2->setSpacing(0);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        stackedWidget = new QStackedWidget(omniPage);
        stackedWidget->setObjectName(QStringLiteral("stackedWidget"));
        sizePolicy.setHeightForWidth(stackedWidget->sizePolicy().hasHeightForWidth());
        stackedWidget->setSizePolicy(sizePolicy);
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

        stackedWidget->addWidget(webpage);
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
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 782, 142));
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

        stackedWidget->addWidget(picpage);
        vidpage = new QWidget();
        vidpage->setObjectName(QStringLiteral("vidpage"));
        sizePolicy.setHeightForWidth(vidpage->sizePolicy().hasHeightForWidth());
        vidpage->setSizePolicy(sizePolicy);
        stackedWidget->addWidget(vidpage);

        horizontalLayout_2->addWidget(stackedWidget);

        viewerStack->addWidget(omniPage);
        splitter->addWidget(viewerStack);
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
        mainMenubar->setGeometry(QRect(0, 0, 800, 19));
        mainMenubar->setAcceptDrops(true);
        menuFile = new QMenu(mainMenubar);
        menuFile->setObjectName(QStringLiteral("menuFile"));
        menuEvidence = new QMenu(mainMenubar);
        menuEvidence->setObjectName(QStringLiteral("menuEvidence"));
        menuAction = new QMenu(mainMenubar);
        menuAction->setObjectName(QStringLiteral("menuAction"));
        menuSettings = new QMenu(mainMenubar);
        menuSettings->setObjectName(QStringLiteral("menuSettings"));
        WombatForensics->setMenuBar(mainMenubar);
        analysisToolBar = new QToolBar(WombatForensics);
        analysisToolBar->setObjectName(QStringLiteral("analysisToolBar"));
        analysisToolBar->setMovable(false);
        analysisToolBar->setAllowedAreas(Qt::NoToolBarArea);
        analysisToolBar->setFloatable(false);
        WombatForensics->addToolBar(Qt::TopToolBarArea, analysisToolBar);

        mainMenubar->addAction(menuFile->menuAction());
        mainMenubar->addAction(menuEvidence->menuAction());
        mainMenubar->addAction(menuAction->menuAction());
        mainMenubar->addAction(menuSettings->menuAction());
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
        actionOpen_Case->setText(QApplication::translate("WombatForensics", "Open Case", 0));
        actionView_Progress->setText(QApplication::translate("WombatForensics", "View Progress...", 0));
        actionExit->setText(QApplication::translate("WombatForensics", "Exit", 0));
        actionAdd_Evidence->setText(QApplication::translate("WombatForensics", "Add Evidence", 0));
        actionRemove_Evidence->setText(QApplication::translate("WombatForensics", "Remove Evidence", 0));
        actionExport_Evidence->setText(QApplication::translate("WombatForensics", "Export Evidence", 0));
        actionManage_OmniViewer->setText(QApplication::translate("WombatForensics", "Manage OmniViewer", 0));
        picViewer->setText(QString());
        menuFile->setTitle(QApplication::translate("WombatForensics", "File", 0));
        menuEvidence->setTitle(QApplication::translate("WombatForensics", "Evidence", 0));
        menuAction->setTitle(QApplication::translate("WombatForensics", "Action", 0));
        menuSettings->setTitle(QApplication::translate("WombatForensics", "Settings", 0));
        analysisToolBar->setWindowTitle(QApplication::translate("WombatForensics", "toolBar", 0));
    } // retranslateUi

};

namespace Ui {
    class WombatForensics: public Ui_WombatForensics {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WOMBATFORENSICS_H
