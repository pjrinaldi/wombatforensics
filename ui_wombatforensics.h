/********************************************************************************
** Form generated from reading UI file 'wombatforensics.ui'
**
** Created by: Qt User Interface Compiler version 5.13.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WOMBATFORENSICS_H
#define UI_WOMBATFORENSICS_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
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
    QWidget *centralwidget;
    QHBoxLayout *horizontalLayout;
    QSplitter *splitter;
    QTreeView *dirTreeView;
    QHexEdit *hexview;
    QMenuBar *mainMenubar;
    QToolBar *analysisToolBar;
    QStatusBar *mainStatusBar;

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
        actionFile = new QAction(WombatForensics);
        actionFile->setObjectName(QString::fromUtf8("actionFile"));
        actionTextSection = new QAction(WombatForensics);
        actionTextSection->setObjectName(QString::fromUtf8("actionTextSection"));
        actionTextViewer = new QAction(WombatForensics);
        actionTextViewer->setObjectName(QString::fromUtf8("actionTextViewer"));
        actionTextViewer->setCheckable(true);
        actionTextViewer->setEnabled(true);
        QIcon icon11;
        icon11.addFile(QString::fromUtf8(":/bar/textencode"), QSize(), QIcon::Normal, QIcon::Off);
        actionTextViewer->setIcon(icon11);
        actionTextViewer->setVisible(true);
        actionViewMessageLog = new QAction(WombatForensics);
        actionViewMessageLog->setObjectName(QString::fromUtf8("actionViewMessageLog"));
        actionViewMessageLog->setCheckable(true);
        QIcon icon12;
        icon12.addFile(QString::fromUtf8(":/log"), QSize(), QIcon::Normal, QIcon::Off);
        actionViewMessageLog->setIcon(icon12);
        actionExpandAll = new QAction(WombatForensics);
        actionExpandAll->setObjectName(QString::fromUtf8("actionExpandAll"));
        QIcon icon13;
        icon13.addFile(QString::fromUtf8(":/bar/expandall"), QSize(), QIcon::Normal, QIcon::Off);
        actionExpandAll->setIcon(icon13);
        actionExpandAll->setVisible(true);
        actionCollapseAll = new QAction(WombatForensics);
        actionCollapseAll->setObjectName(QString::fromUtf8("actionCollapseAll"));
        QIcon icon14;
        icon14.addFile(QString::fromUtf8(":/bar/collapseall"), QSize(), QIcon::Normal, QIcon::Off);
        actionCollapseAll->setIcon(icon14);
        actionCollapseAll->setVisible(true);
        actionDigDeeper = new QAction(WombatForensics);
        actionDigDeeper->setObjectName(QString::fromUtf8("actionDigDeeper"));
        QIcon icon15;
        icon15.addFile(QString::fromUtf8(":/bar/ispade"), QSize(), QIcon::Normal, QIcon::Off);
        actionDigDeeper->setIcon(icon15);
        actionByteConverter = new QAction(WombatForensics);
        actionByteConverter->setObjectName(QString::fromUtf8("actionByteConverter"));
        actionByteConverter->setCheckable(true);
        QIcon icon16;
        icon16.addFile(QString::fromUtf8(":/bar/byteview"), QSize(), QIcon::Normal, QIcon::Off);
        actionByteConverter->setIcon(icon16);
        actionCheck = new QAction(WombatForensics);
        actionCheck->setObjectName(QString::fromUtf8("actionCheck"));
        QIcon icon17;
        icon17.addFile(QString::fromUtf8(":/echeck"), QSize(), QIcon::Normal, QIcon::Off);
        actionCheck->setIcon(icon17);
        actionExport = new QAction(WombatForensics);
        actionExport->setObjectName(QString::fromUtf8("actionExport"));
        actionExport->setIcon(icon4);
        actionSaveState = new QAction(WombatForensics);
        actionSaveState->setObjectName(QString::fromUtf8("actionSaveState"));
        QIcon icon18;
        icon18.addFile(QString::fromUtf8(":/bar/docsave"), QSize(), QIcon::Normal, QIcon::Off);
        actionSaveState->setIcon(icon18);
        actionSettings = new QAction(WombatForensics);
        actionSettings->setObjectName(QString::fromUtf8("actionSettings"));
        QIcon icon19;
        icon19.addFile(QString::fromUtf8(":/bar/settingsview"), QSize(), QIcon::Normal, QIcon::Off);
        actionSettings->setIcon(icon19);
        actionBookmark_Manager = new QAction(WombatForensics);
        actionBookmark_Manager->setObjectName(QString::fromUtf8("actionBookmark_Manager"));
        QIcon icon20;
        icon20.addFile(QString::fromUtf8(":/bar/managetags"), QSize(), QIcon::Normal, QIcon::Off);
        actionBookmark_Manager->setIcon(icon20);
        actionJumpToHex = new QAction(WombatForensics);
        actionJumpToHex->setObjectName(QString::fromUtf8("actionJumpToHex"));
        QIcon icon21;
        icon21.addFile(QString::fromUtf8(":/bar/jumptohex"), QSize(), QIcon::Normal, QIcon::Off);
        actionJumpToHex->setIcon(icon21);
        actionpreviewreport = new QAction(WombatForensics);
        actionpreviewreport->setObjectName(QString::fromUtf8("actionpreviewreport"));
        actionpreviewreport->setCheckable(true);
        QIcon icon22;
        icon22.addFile(QString::fromUtf8(":/bar/reportpreview"), QSize(), QIcon::Normal, QIcon::Off);
        actionpreviewreport->setIcon(icon22);
        actionsearchhex = new QAction(WombatForensics);
        actionsearchhex->setObjectName(QString::fromUtf8("actionsearchhex"));
        QIcon icon23;
        icon23.addFile(QString::fromUtf8(":/bar/hexsrch"), QSize(), QIcon::Normal, QIcon::Off);
        actionsearchhex->setIcon(icon23);
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
        dirTreeView = new QTreeView(splitter);
        dirTreeView->setObjectName(QString::fromUtf8("dirTreeView"));
        dirTreeView->setEditTriggers(QAbstractItemView::NoEditTriggers);
        dirTreeView->setProperty("showDropIndicator", QVariant(false));
        dirTreeView->setAlternatingRowColors(true);
        dirTreeView->setHorizontalScrollMode(QAbstractItemView::ScrollPerItem);
        dirTreeView->setUniformRowHeights(true);
        dirTreeView->setAllColumnsShowFocus(true);
        dirTreeView->setExpandsOnDoubleClick(false);
        splitter->addWidget(dirTreeView);
        hexview = new QHexEdit(splitter);
        hexview->setObjectName(QString::fromUtf8("hexview"));
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
        mainMenubar->setGeometry(QRect(0, 0, 1641, 23));
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
        analysisToolBar->addAction(actionExport_Evidence);
        analysisToolBar->addSeparator();
        analysisToolBar->addAction(actionBookmark_Manager);
        analysisToolBar->addAction(actionpreviewreport);

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
        actionExport_Evidence->setText(QCoreApplication::translate("WombatForensics", "Export Evidence", nullptr));
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
        actionView_Image_Gallery->setText(QCoreApplication::translate("WombatForensics", "View Image Gallery", nullptr));
#if QT_CONFIG(tooltip)
        actionView_Image_Gallery->setToolTip(QCoreApplication::translate("WombatForensics", "View Image Gallery", nullptr));
#endif // QT_CONFIG(tooltip)
        actionViewerManager->setText(QCoreApplication::translate("WombatForensics", "Viewer Manager", nullptr));
        actionCopy_Selection_To->setText(QCoreApplication::translate("WombatForensics", "Copy Selection To", nullptr));
        actionSection->setText(QCoreApplication::translate("WombatForensics", "Copy Hex to a Section", nullptr));
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
        actionExport->setText(QCoreApplication::translate("WombatForensics", "Export...", nullptr));
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
#if QT_CONFIG(tooltip)
        hexview->setToolTip(QString());
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(whatsthis)
        hexview->setWhatsThis(QCoreApplication::translate("WombatForensics", "QHexEdit widget allow to edit the data in hex view.", nullptr));
#endif // QT_CONFIG(whatsthis)
        analysisToolBar->setWindowTitle(QCoreApplication::translate("WombatForensics", "toolBar", nullptr));
    } // retranslateUi

};

namespace Ui {
    class WombatForensics: public Ui_WombatForensics {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WOMBATFORENSICS_H
