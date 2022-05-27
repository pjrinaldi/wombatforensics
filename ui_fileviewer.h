/********************************************************************************
** Form generated from reading UI file 'fileviewer.ui'
**
** Created by: Qt User Interface Compiler version 5.15.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FILEVIEWER_H
#define UI_FILEVIEWER_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>
#include "qhexedit.h"

QT_BEGIN_NAMESPACE

class Ui_FileViewer
{
public:
    QAction *actionHex;
    QAction *actionText;
    QAction *actionOmni;
    QWidget *centralwidget;
    QHBoxLayout *horizontalLayout;
    QHexEdit *filehexview;
    QStatusBar *statusBar;
    QToolBar *toolBar;

    void setupUi(QMainWindow *FileViewer)
    {
        if (FileViewer->objectName().isEmpty())
            FileViewer->setObjectName(QString::fromUtf8("FileViewer"));
        FileViewer->setWindowModality(Qt::NonModal);
        FileViewer->resize(640, 480);
        actionHex = new QAction(FileViewer);
        actionHex->setObjectName(QString::fromUtf8("actionHex"));
        actionText = new QAction(FileViewer);
        actionText->setObjectName(QString::fromUtf8("actionText"));
        actionOmni = new QAction(FileViewer);
        actionOmni->setObjectName(QString::fromUtf8("actionOmni"));
        centralwidget = new QWidget(FileViewer);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(centralwidget->sizePolicy().hasHeightForWidth());
        centralwidget->setSizePolicy(sizePolicy);
        horizontalLayout = new QHBoxLayout(centralwidget);
        horizontalLayout->setSpacing(0);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        filehexview = new QHexEdit(centralwidget);
        filehexview->setObjectName(QString::fromUtf8("filehexview"));
        filehexview->setHexCaps(true);
        filehexview->setDynamicBytesPerLine(true);
        filehexview->setOverwriteMode(false);
        filehexview->setReadOnly(true);

        horizontalLayout->addWidget(filehexview);

        FileViewer->setCentralWidget(centralwidget);
        statusBar = new QStatusBar(FileViewer);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        FileViewer->setStatusBar(statusBar);
        toolBar = new QToolBar(FileViewer);
        toolBar->setObjectName(QString::fromUtf8("toolBar"));
        toolBar->setMovable(false);
        toolBar->setIconSize(QSize(32, 32));
        toolBar->setFloatable(false);
        FileViewer->addToolBar(Qt::TopToolBarArea, toolBar);

        retranslateUi(FileViewer);

        QMetaObject::connectSlotsByName(FileViewer);
    } // setupUi

    void retranslateUi(QMainWindow *FileViewer)
    {
        FileViewer->setWindowTitle(QCoreApplication::translate("FileViewer", "File Viewer", nullptr));
        actionHex->setText(QCoreApplication::translate("FileViewer", "Hex", nullptr));
        actionText->setText(QCoreApplication::translate("FileViewer", "Text", nullptr));
        actionOmni->setText(QCoreApplication::translate("FileViewer", "Omni", nullptr));
#if QT_CONFIG(tooltip)
        filehexview->setToolTip(QCoreApplication::translate("FileViewer", "QHexEditWidget", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(whatsthis)
        filehexview->setWhatsThis(QCoreApplication::translate("FileViewer", "QHexEdit widget allow to edit the data in hex view.", nullptr));
#endif // QT_CONFIG(whatsthis)
        toolBar->setWindowTitle(QCoreApplication::translate("FileViewer", "toolBar", nullptr));
    } // retranslateUi

};

namespace Ui {
    class FileViewer: public Ui_FileViewer {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FILEVIEWER_H
