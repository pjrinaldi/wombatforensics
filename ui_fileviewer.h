/********************************************************************************
** Form generated from reading UI file 'fileviewer.ui'
**
** Created by: Qt User Interface Compiler version 5.3.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FILEVIEWER_H
#define UI_FILEVIEWER_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_FileViewer
{
public:
    QAction *actionHex;
    QAction *actionText;
    QAction *actionOmni;
    QWidget *centralwidget;
    QHBoxLayout *horizontalLayout;
    QStackedWidget *stackedWidget;
    QWidget *hexpage;
    QWidget *textpage;
    QWidget *omnipage;
    QToolBar *toolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *FileViewer)
    {
        if (FileViewer->objectName().isEmpty())
            FileViewer->setObjectName(QStringLiteral("FileViewer"));
        FileViewer->setWindowModality(Qt::WindowModal);
        FileViewer->resize(640, 480);
        actionHex = new QAction(FileViewer);
        actionHex->setObjectName(QStringLiteral("actionHex"));
        actionText = new QAction(FileViewer);
        actionText->setObjectName(QStringLiteral("actionText"));
        actionOmni = new QAction(FileViewer);
        actionOmni->setObjectName(QStringLiteral("actionOmni"));
        centralwidget = new QWidget(FileViewer);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        horizontalLayout = new QHBoxLayout(centralwidget);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        stackedWidget = new QStackedWidget(centralwidget);
        stackedWidget->setObjectName(QStringLiteral("stackedWidget"));
        stackedWidget->setFrameShadow(QFrame::Sunken);
        hexpage = new QWidget();
        hexpage->setObjectName(QStringLiteral("hexpage"));
        stackedWidget->addWidget(hexpage);
        textpage = new QWidget();
        textpage->setObjectName(QStringLiteral("textpage"));
        stackedWidget->addWidget(textpage);
        omnipage = new QWidget();
        omnipage->setObjectName(QStringLiteral("omnipage"));
        stackedWidget->addWidget(omnipage);

        horizontalLayout->addWidget(stackedWidget);

        FileViewer->setCentralWidget(centralwidget);
        toolBar = new QToolBar(FileViewer);
        toolBar->setObjectName(QStringLiteral("toolBar"));
        toolBar->setAllowedAreas(Qt::TopToolBarArea);
        toolBar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
        toolBar->setFloatable(false);
        FileViewer->addToolBar(Qt::TopToolBarArea, toolBar);
        statusBar = new QStatusBar(FileViewer);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        FileViewer->setStatusBar(statusBar);

        toolBar->addAction(actionHex);
        toolBar->addAction(actionText);
        toolBar->addAction(actionOmni);

        retranslateUi(FileViewer);

        stackedWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(FileViewer);
    } // setupUi

    void retranslateUi(QMainWindow *FileViewer)
    {
        FileViewer->setWindowTitle(QApplication::translate("FileViewer", "File Viewer", 0));
        actionHex->setText(QApplication::translate("FileViewer", "Hex", 0));
        actionText->setText(QApplication::translate("FileViewer", "Text", 0));
        actionOmni->setText(QApplication::translate("FileViewer", "Omni", 0));
        toolBar->setWindowTitle(QApplication::translate("FileViewer", "toolBar", 0));
    } // retranslateUi

};

namespace Ui {
    class FileViewer: public Ui_FileViewer {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FILEVIEWER_H
