/********************************************************************************
** Form generated from reading UI file 'wombatforensics.ui'
**
** Created by: Qt User Interface Compiler version 5.15.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WOMBATFORENSICS_H
#define UI_WOMBATFORENSICS_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>
#include "qhexedit.h"

QT_BEGIN_NAMESPACE

class Ui_WombatForensics
{
public:
    QAction *actionNew_Case;
    QAction *actionOpen_Case;
    QAction *actionAdd_Evidence;
    QAction *actionRemove_Evidence;
    QAction *actionExport_Evidence;
    QAction *actionView_Properties;
    QAction *actionAbout;
    QAction *actionView_File;
    QAction *actionView_Image_Gallery;
    QAction *actionViewerManager;
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
    QAction *actionSettings;
    QAction *actionBookmark_Manager;
    QAction *actionJumpToHex;
    QAction *actionpreviewreport;
    QAction *actionsearchhex;
    QAction *actionpublishresults;
    QAction *actionchomp;
    QAction *actionCarve;
    QAction *actionCreateForensicImage;
    QAction *actionExportForensicImage;
    QAction *actionVerifyForensicImage;
    QAction *actionHashListManager;
    QWidget *centralwidget;
    QHBoxLayout *horizontalLayout;
    QSplitter *splitter;
    QHexEdit *hexview;
    QMenuBar *mainMenubar;
    QToolBar *analysisToolBar;
    QStatusBar *mainStatusBar;
    QToolBar *pathToolBar;

    void setupUi(QMainWindow *WombatForensics)
    {
        if (WombatForensics->objectName().isEmpty())
            WombatForensics->setObjectName(QString::fromUtf8("WombatForensics"));
        WombatForensics->resize(1641, 768);
        actionNew_Case = new QAction(WombatForensics);
        actionNew_Case->setObjectName(QString::fromUtf8("actionNew_Case"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/bar/docnew"), QSize(), QIcon::Normal, QIcon::Off);
        actionNew_Case->setIcon(icon);
        actionOpen_Case = new QAction(WombatForensics);
        actionOpen_Case->setObjectName(QString::fromUtf8("actionOpen_Case"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/bar/docopen"), QSize(), QIcon::Normal, QIcon::Off);
        actionOpen_Case->setIcon(icon1);
        actionAdd_Evidence = new QAction(WombatForensics);
        actionAdd_Evidence->setObjectName(QString::fromUtf8("actionAdd_Evidence"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/bar/add"), QSize(), QIcon::Normal, QIcon::Off);
        actionAdd_Evidence->setIcon(icon2);
        actionRemove_Evidence = new QAction(WombatForensics);
        actionRemove_Evidence->setObjectName(QString::fromUtf8("actionRemove_Evidence"));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/bar/rem"), QSize(), QIcon::Normal, QIcon::Off);
        actionRemove_Evidence->setIcon(icon3);
        actionExport_Evidence = new QAction(WombatForensics);
        actionExport_Evidence->setObjectName(QString::fromUtf8("actionExport_Evidence"));
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/eexport"), QSize(), QIcon::Normal, QIcon::Off);
        actionExport_Evidence->setIcon(icon4);
        actionView_Properties = new QAction(WombatForensics);
        actionView_Properties->setObjectName(QString::fromUtf8("actionView_Properties"));
        actionView_Properties->setCheckable(false);
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/info"), QSize(), QIcon::Normal, QIcon::Off);
        actionView_Properties->setIcon(icon5);
        actionAbout = new QAction(WombatForensics);
        actionAbout->setObjectName(QString::fromUtf8("actionAbout"));
        QIcon icon6;
        icon6.addFile(QString::fromUtf8(":/help"), QSize(), QIcon::Normal, QIcon::Off);
        actionAbout->setIcon(icon6);
        actionView_File = new QAction(WombatForensics);
        actionView_File->setObjectName(QString::fromUtf8("actionView_File"));
        actionView_File->setCheckable(false);
        QIcon icon7;
        icon7.addFile(QString::fromUtf8(":/bar/xhex"), QSize(), QIcon::Normal, QIcon::Off);
        actionView_File->setIcon(icon7);
        actionView_File->setIconVisibleInMenu(true);
        actionView_Image_Gallery = new QAction(WombatForensics);
        actionView_Image_Gallery->setObjectName(QString::fromUtf8("actionView_Image_Gallery"));
        actionView_Image_Gallery->setCheckable(true);
        QIcon icon8;
        icon8.addFile(QString::fromUtf8(":/thumb"), QSize(), QIcon::Normal, QIcon::Off);
        actionView_Image_Gallery->setIcon(icon8);
        actionViewerManager = new QAction(WombatForensics);
        actionViewerManager->setObjectName(QString::fromUtf8("actionViewerManager"));
        QIcon icon9;
        icon9.addFile(QString::fromUtf8(":/bar/setview"), QSize(), QIcon::Normal, QIcon::Off);
        actionViewerManager->setIcon(icon9);
        actionCopy_Selection_To = new QAction(WombatForensics);
        actionCopy_Selection_To->setObjectName(QString::fromUtf8("actionCopy_Selection_To"));
        actionCopy_Selection_To->setEnabled(false);
        QIcon icon10;
        icon10.addFile(QString::fromUtf8(":/ecopy"), QSize(), QIcon::Normal, QIcon::Off);
        actionCopy_Selection_To->setIcon(icon10);
        actionCopy_Selection_To->setShortcutContext(Qt::WindowShortcut);
        actionCopy_Selection_To->setAutoRepeat(true);
        actionCopy_Selection_To->setVisible(false);
        actionCopy_Selection_To->setMenuRole(QAction::TextHeuristicRole);
        actionSection = new QAction(WombatForensics);
        actionSection->setObjectName(QString::fromUtf8("actionSection"));
        QIcon icon11;
        icon11.addFile(QString::fromUtf8(":/bar/addtotag"), QSize(), QIcon::Normal, QIcon::Off);
        actionSection->setIcon(icon11);
        actionFile = new QAction(WombatForensics);
        actionFile->setObjectName(QString::fromUtf8("actionFile"));
        QIcon icon12;
        icon12.addFile(QString::fromUtf8(":/bar/carvetofile"), QSize(), QIcon::Normal, QIcon::Off);
        actionFile->setIcon(icon12);
        actionTextSection = new QAction(WombatForensics);
        actionTextSection->setObjectName(QString::fromUtf8("actionTextSection"));
        actionTextViewer = new QAction(WombatForensics);
        actionTextViewer->setObjectName(QString::fromUtf8("actionTextViewer"));
        actionTextViewer->setCheckable(true);
        actionTextViewer->setEnabled(true);
        QIcon icon13;
        icon13.addFile(QString::fromUtf8(":/bar/textencode"), QSize(), QIcon::Normal, QIcon::Off);
        actionTextViewer->setIcon(icon13);
        actionTextViewer->setVisible(true);
        actionViewMessageLog = new QAction(WombatForensics);
        actionViewMessageLog->setObjectName(QString::fromUtf8("actionViewMessageLog"));
        actionViewMessageLog->setCheckable(true);
        QIcon icon14;
        icon14.addFile(QString::fromUtf8(":/log"), QSize(), QIcon::Normal, QIcon::Off);
        actionViewMessageLog->setIcon(icon14);
        actionExpandAll = new QAction(WombatForensics);
        actionExpandAll->setObjectName(QString::fromUtf8("actionExpandAll"));
        QIcon icon15;
        icon15.addFile(QString::fromUtf8(":/bar/expandall"), QSize(), QIcon::Normal, QIcon::Off);
        actionExpandAll->setIcon(icon15);
        actionExpandAll->setVisible(true);
        actionCollapseAll = new QAction(WombatForensics);
        actionCollapseAll->setObjectName(QString::fromUtf8("actionCollapseAll"));
        QIcon icon16;
        icon16.addFile(QString::fromUtf8(":/bar/collapseall"), QSize(), QIcon::Normal, QIcon::Off);
        actionCollapseAll->setIcon(icon16);
        actionCollapseAll->setVisible(true);
        actionDigDeeper = new QAction(WombatForensics);
        actionDigDeeper->setObjectName(QString::fromUtf8("actionDigDeeper"));
        QIcon icon17;
        icon17.addFile(QString::fromUtf8(":/bar/ispade"), QSize(), QIcon::Normal, QIcon::Off);
        actionDigDeeper->setIcon(icon17);
        actionByteConverter = new QAction(WombatForensics);
        actionByteConverter->setObjectName(QString::fromUtf8("actionByteConverter"));
        actionByteConverter->setCheckable(true);
        QIcon icon18;
        icon18.addFile(QString::fromUtf8(":/bar/byteview"), QSize(), QIcon::Normal, QIcon::Off);
        actionByteConverter->setIcon(icon18);
        actionCheck = new QAction(WombatForensics);
        actionCheck->setObjectName(QString::fromUtf8("actionCheck"));
        QIcon icon19;
        icon19.addFile(QString::fromUtf8(":/echeck"), QSize(), QIcon::Normal, QIcon::Off);
        actionCheck->setIcon(icon19);
        actionExport = new QAction(WombatForensics);
        actionExport->setObjectName(QString::fromUtf8("actionExport"));
        actionExport->setIcon(icon4);
        actionSaveState = new QAction(WombatForensics);
        actionSaveState->setObjectName(QString::fromUtf8("actionSaveState"));
        QIcon icon20;
        icon20.addFile(QString::fromUtf8(":/bar/docsave"), QSize(), QIcon::Normal, QIcon::Off);
        actionSaveState->setIcon(icon20);
        actionSettings = new QAction(WombatForensics);
        actionSettings->setObjectName(QString::fromUtf8("actionSettings"));
        QIcon icon21;
        icon21.addFile(QString::fromUtf8(":/bar/settingsview"), QSize(), QIcon::Normal, QIcon::Off);
        actionSettings->setIcon(icon21);
        actionBookmark_Manager = new QAction(WombatForensics);
        actionBookmark_Manager->setObjectName(QString::fromUtf8("actionBookmark_Manager"));
        QIcon icon22;
        icon22.addFile(QString::fromUtf8(":/bar/managetags"), QSize(), QIcon::Normal, QIcon::Off);
        actionBookmark_Manager->setIcon(icon22);
        actionJumpToHex = new QAction(WombatForensics);
        actionJumpToHex->setObjectName(QString::fromUtf8("actionJumpToHex"));
        QIcon icon23;
        icon23.addFile(QString::fromUtf8(":/bar/jumptohex"), QSize(), QIcon::Normal, QIcon::Off);
        actionJumpToHex->setIcon(icon23);
        actionpreviewreport = new QAction(WombatForensics);
        actionpreviewreport->setObjectName(QString::fromUtf8("actionpreviewreport"));
        actionpreviewreport->setCheckable(true);
        QIcon icon24;
        icon24.addFile(QString::fromUtf8(":/bar/reportpreview"), QSize(), QIcon::Normal, QIcon::Off);
        actionpreviewreport->setIcon(icon24);
        actionsearchhex = new QAction(WombatForensics);
        actionsearchhex->setObjectName(QString::fromUtf8("actionsearchhex"));
        QIcon icon25;
        icon25.addFile(QString::fromUtf8(":/bar/hexsrch"), QSize(), QIcon::Normal, QIcon::Off);
        actionsearchhex->setIcon(icon25);
        actionpublishresults = new QAction(WombatForensics);
        actionpublishresults->setObjectName(QString::fromUtf8("actionpublishresults"));
        QIcon icon26;
        icon26.addFile(QString::fromUtf8(":/bar/publish"), QSize(), QIcon::Normal, QIcon::Off);
        actionpublishresults->setIcon(icon26);
        actionchomp = new QAction(WombatForensics);
        actionchomp->setObjectName(QString::fromUtf8("actionchomp"));
        QIcon icon27;
        icon27.addFile(QString::fromUtf8(":/bar/xchomppng"), QSize(), QIcon::Normal, QIcon::Off);
        actionchomp->setIcon(icon27);
        actionCarve = new QAction(WombatForensics);
        actionCarve->setObjectName(QString::fromUtf8("actionCarve"));
        actionCarve->setIcon(icon12);
        actionCreateForensicImage = new QAction(WombatForensics);
        actionCreateForensicImage->setObjectName(QString::fromUtf8("actionCreateForensicImage"));
        QIcon icon28;
        icon28.addFile(QString::fromUtf8(":/bar/newforimg"), QSize(), QIcon::Normal, QIcon::Off);
        actionCreateForensicImage->setIcon(icon28);
        actionExportForensicImage = new QAction(WombatForensics);
        actionExportForensicImage->setObjectName(QString::fromUtf8("actionExportForensicImage"));
        QIcon icon29;
        icon29.addFile(QString::fromUtf8(":/bar/exportforimg"), QSize(), QIcon::Normal, QIcon::Off);
        actionExportForensicImage->setIcon(icon29);
        actionVerifyForensicImage = new QAction(WombatForensics);
        actionVerifyForensicImage->setObjectName(QString::fromUtf8("actionVerifyForensicImage"));
        actionVerifyForensicImage->setCheckable(true);
        actionVerifyForensicImage->setChecked(false);
        QIcon icon30;
        icon30.addFile(QString::fromUtf8(":/bar/reverify"), QSize(), QIcon::Normal, QIcon::Off);
        actionVerifyForensicImage->setIcon(icon30);
        actionHashListManager = new QAction(WombatForensics);
        actionHashListManager->setObjectName(QString::fromUtf8("actionHashListManager"));
        QIcon icon31;
        icon31.addFile(QString::fromUtf8(":/bar/hashlist"), QSize(), QIcon::Normal, QIcon::Off);
        actionHashListManager->setIcon(icon31);
        centralwidget = new QWidget(WombatForensics);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        horizontalLayout = new QHBoxLayout(centralwidget);
        horizontalLayout->setSpacing(0);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        splitter = new QSplitter(centralwidget);
        splitter->setObjectName(QString::fromUtf8("splitter"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(splitter->sizePolicy().hasHeightForWidth());
        splitter->setSizePolicy(sizePolicy);
        splitter->setOrientation(Qt::Vertical);
        splitter->setChildrenCollapsible(false);
        hexview = new QHexEdit(splitter);
        hexview->setObjectName(QString::fromUtf8("hexview"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(1);
        sizePolicy1.setHeightForWidth(hexview->sizePolicy().hasHeightForWidth());
        hexview->setSizePolicy(sizePolicy1);
        hexview->setToolTipDuration(0);
        hexview->setProperty("bytesPerLine", QVariant(49));
        hexview->setProperty("hexCaps", QVariant(true));
        hexview->setProperty("dynamicBytesPerLine", QVariant(true));
        hexview->setProperty("readOnly", QVariant(true));
        splitter->addWidget(hexview);

        horizontalLayout->addWidget(splitter);

        WombatForensics->setCentralWidget(centralwidget);
        mainMenubar = new QMenuBar(WombatForensics);
        mainMenubar->setObjectName(QString::fromUtf8("mainMenubar"));
        mainMenubar->setGeometry(QRect(0, 0, 1641, 22));
        mainMenubar->setAcceptDrops(true);
        WombatForensics->setMenuBar(mainMenubar);
        analysisToolBar = new QToolBar(WombatForensics);
        analysisToolBar->setObjectName(QString::fromUtf8("analysisToolBar"));
        analysisToolBar->setLayoutDirection(Qt::LeftToRight);
        analysisToolBar->setMovable(false);
        analysisToolBar->setAllowedAreas(Qt::NoToolBarArea);
        analysisToolBar->setIconSize(QSize(24, 24));
        analysisToolBar->setToolButtonStyle(Qt::ToolButtonIconOnly);
        analysisToolBar->setFloatable(false);
        WombatForensics->addToolBar(Qt::TopToolBarArea, analysisToolBar);
        mainStatusBar = new QStatusBar(WombatForensics);
        mainStatusBar->setObjectName(QString::fromUtf8("mainStatusBar"));
        mainStatusBar->setStyleSheet(QString::fromUtf8(""));
        mainStatusBar->setSizeGripEnabled(true);
        WombatForensics->setStatusBar(mainStatusBar);
        pathToolBar = new QToolBar(WombatForensics);
        pathToolBar->setObjectName(QString::fromUtf8("pathToolBar"));
        pathToolBar->setMovable(false);
        pathToolBar->setAllowedAreas(Qt::TopToolBarArea);
        pathToolBar->setToolButtonStyle(Qt::ToolButtonFollowStyle);
        pathToolBar->setFloatable(false);
        WombatForensics->addToolBar(Qt::TopToolBarArea, pathToolBar);
        WombatForensics->insertToolBarBreak(pathToolBar);

        analysisToolBar->addAction(actionNew_Case);
        analysisToolBar->addAction(actionOpen_Case);
        analysisToolBar->addAction(actionSaveState);
        analysisToolBar->addSeparator();
        analysisToolBar->addAction(actionAdd_Evidence);
        analysisToolBar->addAction(actionRemove_Evidence);
        analysisToolBar->addSeparator();
        analysisToolBar->addAction(actionJumpToHex);
        analysisToolBar->addAction(actionsearchhex);
        analysisToolBar->addAction(actionByteConverter);
        analysisToolBar->addSeparator();
        analysisToolBar->addAction(actionSettings);
        analysisToolBar->addAction(actionViewerManager);
        analysisToolBar->addAction(actionViewMessageLog);
        analysisToolBar->addSeparator();
        analysisToolBar->addAction(actionView_Image_Gallery);
        analysisToolBar->addAction(actionDigDeeper);
        analysisToolBar->addAction(actionCopy_Selection_To);
        analysisToolBar->addAction(actionCarve);
        analysisToolBar->addAction(actionExport_Evidence);
        analysisToolBar->addAction(actionExportForensicImage);
        analysisToolBar->addSeparator();
        analysisToolBar->addAction(actionBookmark_Manager);
        analysisToolBar->addAction(actionHashListManager);
        analysisToolBar->addAction(actionpreviewreport);
        analysisToolBar->addAction(actionpublishresults);
        analysisToolBar->addSeparator();
        analysisToolBar->addAction(actionCreateForensicImage);
        analysisToolBar->addAction(actionVerifyForensicImage);

        retranslateUi(WombatForensics);
        QObject::connect(actionRemove_Evidence, SIGNAL(triggered()), WombatForensics, SLOT(RemEvidence()));
        QObject::connect(actionAdd_Evidence, SIGNAL(triggered()), WombatForensics, SLOT(AddEvidence()));
        QObject::connect(actionExport_Evidence, SIGNAL(triggered()), WombatForensics, SLOT(ExportEvidence()));

        QMetaObject::connectSlotsByName(WombatForensics);
    } // setupUi

    void retranslateUi(QMainWindow *WombatForensics)
    {
        WombatForensics->setWindowTitle(QCoreApplication::translate("WombatForensics", "WombatForensics", nullptr));
        actionNew_Case->setText(QCoreApplication::translate("WombatForensics", "New Case", nullptr));
#if QT_CONFIG(shortcut)
        actionNew_Case->setShortcut(QCoreApplication::translate("WombatForensics", "Ctrl+N", nullptr));
#endif // QT_CONFIG(shortcut)
        actionOpen_Case->setText(QCoreApplication::translate("WombatForensics", "Open Case", nullptr));
#if QT_CONFIG(shortcut)
        actionOpen_Case->setShortcut(QCoreApplication::translate("WombatForensics", "Ctrl+O", nullptr));
#endif // QT_CONFIG(shortcut)
        actionAdd_Evidence->setText(QCoreApplication::translate("WombatForensics", "Add Evidence", nullptr));
#if QT_CONFIG(shortcut)
        actionAdd_Evidence->setShortcut(QCoreApplication::translate("WombatForensics", "Ctrl+A", nullptr));
#endif // QT_CONFIG(shortcut)
        actionRemove_Evidence->setText(QCoreApplication::translate("WombatForensics", "Remove Evidence", nullptr));
#if QT_CONFIG(shortcut)
        actionRemove_Evidence->setShortcut(QCoreApplication::translate("WombatForensics", "Ctrl+D", nullptr));
#endif // QT_CONFIG(shortcut)
        actionExport_Evidence->setText(QCoreApplication::translate("WombatForensics", "Export Files", nullptr));
#if QT_CONFIG(tooltip)
        actionExport_Evidence->setToolTip(QCoreApplication::translate("WombatForensics", "Export Files to Directory", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(shortcut)
        actionExport_Evidence->setShortcut(QCoreApplication::translate("WombatForensics", "Ctrl+E", nullptr));
#endif // QT_CONFIG(shortcut)
        actionView_Properties->setText(QCoreApplication::translate("WombatForensics", "View File Properties", nullptr));
#if QT_CONFIG(tooltip)
        actionView_Properties->setToolTip(QCoreApplication::translate("WombatForensics", "Properties Window", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(shortcut)
        actionView_Properties->setShortcut(QCoreApplication::translate("WombatForensics", "Ctrl+P", nullptr));
#endif // QT_CONFIG(shortcut)
        actionAbout->setText(QCoreApplication::translate("WombatForensics", "About", nullptr));
        actionView_File->setText(QCoreApplication::translate("WombatForensics", "View File Hex", nullptr));
        actionView_Image_Gallery->setText(QCoreApplication::translate("WombatForensics", "View IMG/VID Thumbnails", nullptr));
#if QT_CONFIG(tooltip)
        actionView_Image_Gallery->setToolTip(QCoreApplication::translate("WombatForensics", "View Image and Video Thumbnails", nullptr));
#endif // QT_CONFIG(tooltip)
        actionViewerManager->setText(QCoreApplication::translate("WombatForensics", "Viewer Manager", nullptr));
        actionCopy_Selection_To->setText(QCoreApplication::translate("WombatForensics", "Copy Selection To", nullptr));
        actionSection->setText(QCoreApplication::translate("WombatForensics", "Tag Carved Section", nullptr));
#if QT_CONFIG(tooltip)
        actionSection->setToolTip(QCoreApplication::translate("WombatForensics", "Add Carved Section to Existing Tag", nullptr));
#endif // QT_CONFIG(tooltip)
        actionFile->setText(QCoreApplication::translate("WombatForensics", "Carve to a File", nullptr));
        actionTextSection->setText(QCoreApplication::translate("WombatForensics", "Copy Ascii to a Section", nullptr));
        actionTextViewer->setText(QCoreApplication::translate("WombatForensics", "Text Viewer", nullptr));
        actionViewMessageLog->setText(QCoreApplication::translate("WombatForensics", "View Message Log", nullptr));
#if QT_CONFIG(tooltip)
        actionViewMessageLog->setToolTip(QCoreApplication::translate("WombatForensics", "View Message Log", nullptr));
#endif // QT_CONFIG(tooltip)
        actionExpandAll->setText(QCoreApplication::translate("WombatForensics", "Expand All", nullptr));
#if QT_CONFIG(tooltip)
        actionExpandAll->setToolTip(QCoreApplication::translate("WombatForensics", "Expand All", nullptr));
#endif // QT_CONFIG(tooltip)
        actionCollapseAll->setText(QCoreApplication::translate("WombatForensics", "Collapse All", nullptr));
#if QT_CONFIG(tooltip)
        actionCollapseAll->setToolTip(QCoreApplication::translate("WombatForensics", "Collapse All", nullptr));
#endif // QT_CONFIG(tooltip)
        actionDigDeeper->setText(QCoreApplication::translate("WombatForensics", "Dig Deeper", nullptr));
#if QT_CONFIG(tooltip)
        actionDigDeeper->setToolTip(QCoreApplication::translate("WombatForensics", "Dig Deeper", nullptr));
#endif // QT_CONFIG(tooltip)
        actionByteConverter->setText(QCoreApplication::translate("WombatForensics", "Byte Converter", nullptr));
#if QT_CONFIG(tooltip)
        actionByteConverter->setToolTip(QCoreApplication::translate("WombatForensics", "Byte Converter", nullptr));
#endif // QT_CONFIG(tooltip)
        actionCheck->setText(QCoreApplication::translate("WombatForensics", "(Un)Check Selected", nullptr));
        actionExport->setText(QCoreApplication::translate("WombatForensics", "Export Files", nullptr));
#if QT_CONFIG(tooltip)
        actionExport->setToolTip(QCoreApplication::translate("WombatForensics", "Export Files to Directory", nullptr));
#endif // QT_CONFIG(tooltip)
        actionSaveState->setText(QCoreApplication::translate("WombatForensics", "Save State", nullptr));
#if QT_CONFIG(tooltip)
        actionSaveState->setToolTip(QCoreApplication::translate("WombatForensics", "Save State", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(shortcut)
        actionSaveState->setShortcut(QCoreApplication::translate("WombatForensics", "Ctrl+S", nullptr));
#endif // QT_CONFIG(shortcut)
        actionSettings->setText(QCoreApplication::translate("WombatForensics", "Settings", nullptr));
#if QT_CONFIG(tooltip)
        actionSettings->setToolTip(QCoreApplication::translate("WombatForensics", "Settings", nullptr));
#endif // QT_CONFIG(tooltip)
        actionBookmark_Manager->setText(QCoreApplication::translate("WombatForensics", "Manage Tags", nullptr));
#if QT_CONFIG(tooltip)
        actionBookmark_Manager->setToolTip(QCoreApplication::translate("WombatForensics", "Manage Tags", nullptr));
#endif // QT_CONFIG(tooltip)
        actionJumpToHex->setText(QCoreApplication::translate("WombatForensics", "JumpToHex", nullptr));
#if QT_CONFIG(tooltip)
        actionJumpToHex->setToolTip(QCoreApplication::translate("WombatForensics", "Jump to Hex Offset", nullptr));
#endif // QT_CONFIG(tooltip)
        actionpreviewreport->setText(QCoreApplication::translate("WombatForensics", "Preview Report", nullptr));
#if QT_CONFIG(tooltip)
        actionpreviewreport->setToolTip(QCoreApplication::translate("WombatForensics", "Preview Report", nullptr));
#endif // QT_CONFIG(tooltip)
        actionsearchhex->setText(QCoreApplication::translate("WombatForensics", "Find in Hex/Ascii", nullptr));
#if QT_CONFIG(tooltip)
        actionsearchhex->setToolTip(QCoreApplication::translate("WombatForensics", "Find in Hex/Ascii", nullptr));
#endif // QT_CONFIG(tooltip)
        actionpublishresults->setText(QCoreApplication::translate("WombatForensics", "Publish Results", nullptr));
        actionchomp->setText(QCoreApplication::translate("WombatForensics", "Play xchomp", nullptr));
#if QT_CONFIG(tooltip)
        actionchomp->setToolTip(QCoreApplication::translate("WombatForensics", "Play xchomp While  You Wait", nullptr));
#endif // QT_CONFIG(tooltip)
        actionCarve->setText(QCoreApplication::translate("WombatForensics", "File Carving", nullptr));
        actionCreateForensicImage->setText(QCoreApplication::translate("WombatForensics", "Create Forensic Image", nullptr));
#if QT_CONFIG(tooltip)
        actionCreateForensicImage->setToolTip(QCoreApplication::translate("WombatForensics", "Create Forensic Image", nullptr));
#endif // QT_CONFIG(tooltip)
        actionExportForensicImage->setText(QCoreApplication::translate("WombatForensics", "Export Files to Forensic Image", nullptr));
        actionVerifyForensicImage->setText(QCoreApplication::translate("WombatForensics", "Verify Forensic Image", nullptr));
#if QT_CONFIG(tooltip)
        actionVerifyForensicImage->setToolTip(QCoreApplication::translate("WombatForensics", "Verify Forensic Image", nullptr));
#endif // QT_CONFIG(tooltip)
        actionHashListManager->setText(QCoreApplication::translate("WombatForensics", "HashListManager", nullptr));
#if QT_CONFIG(tooltip)
        actionHashListManager->setToolTip(QCoreApplication::translate("WombatForensics", "Manage Hash Lists", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(tooltip)
        hexview->setToolTip(QString());
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(whatsthis)
        hexview->setWhatsThis(QCoreApplication::translate("WombatForensics", "QHexEdit widget allow to edit the data in hex view.", nullptr));
#endif // QT_CONFIG(whatsthis)
        analysisToolBar->setWindowTitle(QCoreApplication::translate("WombatForensics", "toolBar", nullptr));
        pathToolBar->setWindowTitle(QCoreApplication::translate("WombatForensics", "toolBar", nullptr));
    } // retranslateUi

};

namespace Ui {
    class WombatForensics: public Ui_WombatForensics {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WOMBATFORENSICS_H
