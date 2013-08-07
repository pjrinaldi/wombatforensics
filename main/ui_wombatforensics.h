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
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QColumnView>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QToolBox>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_WombatForensics
{
public:
    QAction *actionNew_Case;
    QAction *actionOpen_Case;
    QAction *actionNew_Case_2;
    QAction *actionOpen_Case_2;
    QAction *actionExit;
    QAction *actionSettings;
    QWidget *centralWidget;
    QHBoxLayout *horizontalLayout_2;
    QHBoxLayout *horizontalLayout;
    QToolBox *toolBox;
    QWidget *placeholderPage;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout_3;
    QVBoxLayout *verticalLayout;
    QTabWidget *fileViewTabWidget;
    QColumnView *fileColumnView;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuEvidence;
    QMenu *menuSettings;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *WombatForensics)
    {
        if (WombatForensics->objectName().isEmpty())
            WombatForensics->setObjectName(QStringLiteral("WombatForensics"));
        WombatForensics->resize(704, 580);
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(WombatForensics->sizePolicy().hasHeightForWidth());
        WombatForensics->setSizePolicy(sizePolicy);
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
        actionNew_Case_2 = new QAction(WombatForensics);
        actionNew_Case_2->setObjectName(QStringLiteral("actionNew_Case_2"));
        actionNew_Case_2->setIcon(icon);
        actionOpen_Case_2 = new QAction(WombatForensics);
        actionOpen_Case_2->setObjectName(QStringLiteral("actionOpen_Case_2"));
        actionOpen_Case_2->setIcon(icon1);
        actionExit = new QAction(WombatForensics);
        actionExit->setObjectName(QStringLiteral("actionExit"));
        actionSettings = new QAction(WombatForensics);
        actionSettings->setObjectName(QStringLiteral("actionSettings"));
        centralWidget = new QWidget(WombatForensics);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(1);
        sizePolicy1.setVerticalStretch(1);
        sizePolicy1.setHeightForWidth(centralWidget->sizePolicy().hasHeightForWidth());
        centralWidget->setSizePolicy(sizePolicy1);
        horizontalLayout_2 = new QHBoxLayout(centralWidget);
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setSizeConstraint(QLayout::SetNoConstraint);
        toolBox = new QToolBox(centralWidget);
        toolBox->setObjectName(QStringLiteral("toolBox"));
        QSizePolicy sizePolicy2(QSizePolicy::Fixed, QSizePolicy::Expanding);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(toolBox->sizePolicy().hasHeightForWidth());
        toolBox->setSizePolicy(sizePolicy2);
        toolBox->setMinimumSize(QSize(170, 0));
        placeholderPage = new QWidget();
        placeholderPage->setObjectName(QStringLiteral("placeholderPage"));
        placeholderPage->setGeometry(QRect(0, 0, 170, 452));
        sizePolicy.setHeightForWidth(placeholderPage->sizePolicy().hasHeightForWidth());
        placeholderPage->setSizePolicy(sizePolicy);
        verticalLayout_2 = new QVBoxLayout(placeholderPage);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(1, 2, 1, 2);
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(0);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));

        verticalLayout_2->addLayout(horizontalLayout_3);

        //toolBox->addItem(placeholderPage, QStringLiteral(""));

        horizontalLayout->addWidget(toolBox);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setSizeConstraint(QLayout::SetNoConstraint);
        fileViewTabWidget = new QTabWidget(centralWidget);
        fileViewTabWidget->setObjectName(QStringLiteral("fileViewTabWidget"));
        fileViewTabWidget->setMinimumSize(QSize(450, 0));
        fileViewTabWidget->setTabPosition(QTabWidget::North);

        verticalLayout->addWidget(fileViewTabWidget);

        fileColumnView = new QColumnView(centralWidget);
        fileColumnView->setObjectName(QStringLiteral("fileColumnView"));
        fileColumnView->setMinimumSize(QSize(450, 0));
        fileColumnView->setAutoFillBackground(false);
        fileColumnView->setStyleSheet(QStringLiteral("font: 8pt \"DejaVu Sans Mono\";"));
        fileColumnView->setAlternatingRowColors(true);

        verticalLayout->addWidget(fileColumnView);


        horizontalLayout->addLayout(verticalLayout);


        horizontalLayout_2->addLayout(horizontalLayout);

        WombatForensics->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(WombatForensics);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 704, 19));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QStringLiteral("menuFile"));
        menuEvidence = new QMenu(menuBar);
        menuEvidence->setObjectName(QStringLiteral("menuEvidence"));
        menuEvidence->setEnabled(false);
        menuSettings = new QMenu(menuBar);
        menuSettings->setObjectName(QStringLiteral("menuSettings"));
        WombatForensics->setMenuBar(menuBar);
        mainToolBar = new QToolBar(WombatForensics);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        mainToolBar->setMovable(false);
        mainToolBar->setAllowedAreas(Qt::TopToolBarArea);
        mainToolBar->setFloatable(false);
        WombatForensics->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(WombatForensics);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        WombatForensics->setStatusBar(statusBar);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuEvidence->menuAction());
        menuBar->addAction(menuSettings->menuAction());
        menuFile->addAction(actionNew_Case_2);
        menuFile->addAction(actionOpen_Case_2);
        menuFile->addSeparator();
        menuFile->addAction(actionExit);
        mainToolBar->addAction(actionNew_Case);
        mainToolBar->addAction(actionOpen_Case);
        mainToolBar->addSeparator();

        retranslateUi(WombatForensics);

        toolBox->setCurrentIndex(0);
        toolBox->layout()->setSpacing(6);
        fileViewTabWidget->setCurrentIndex(-1);


        QMetaObject::connectSlotsByName(WombatForensics);
    } // setupUi

    void retranslateUi(QMainWindow *WombatForensics)
    {
        WombatForensics->setWindowTitle(QApplication::translate("WombatForensics", "Wombat Forensics", 0));
        actionNew_Case->setText(QApplication::translate("WombatForensics", "New Case", 0));
#ifndef QT_NO_TOOLTIP
        actionNew_Case->setToolTip(QApplication::translate("WombatForensics", "Create a New Case", 0));
#endif // QT_NO_TOOLTIP
        actionOpen_Case->setText(QApplication::translate("WombatForensics", "Open Case", 0));
#ifndef QT_NO_TOOLTIP
        actionOpen_Case->setToolTip(QApplication::translate("WombatForensics", "Open an Existing Case", 0));
#endif // QT_NO_TOOLTIP
        actionNew_Case_2->setText(QApplication::translate("WombatForensics", "New Case", 0));
        actionOpen_Case_2->setText(QApplication::translate("WombatForensics", "Open Case", 0));
        actionExit->setText(QApplication::translate("WombatForensics", "Exit", 0));
        actionSettings->setText(QApplication::translate("WombatForensics", "Settings", 0));
        toolBox->setItemText(toolBox->indexOf(placeholderPage), QString());
        menuFile->setTitle(QApplication::translate("WombatForensics", "File", 0));
        menuEvidence->setTitle(QApplication::translate("WombatForensics", "Evidence", 0));
        menuSettings->setTitle(QApplication::translate("WombatForensics", "Settings", 0));
    } // retranslateUi

};

namespace Ui {
    class WombatForensics: public Ui_WombatForensics {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WOMBATFORENSICS_H
