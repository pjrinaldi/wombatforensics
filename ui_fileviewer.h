/********************************************************************************
** Form generated from reading UI file 'fileviewer.ui'
**
** Created by: Qt User Interface Compiler version 5.9.1
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
            FileViewer->setObjectName(QStringLiteral("FileViewer"));
        FileViewer->setWindowModality(Qt::NonModal);
        FileViewer->resize(640, 480);
        actionHex = new QAction(FileViewer);
        actionHex->setObjectName(QStringLiteral("actionHex"));
        actionText = new QAction(FileViewer);
        actionText->setObjectName(QStringLiteral("actionText"));
        actionOmni = new QAction(FileViewer);
        actionOmni->setObjectName(QStringLiteral("actionOmni"));
        centralwidget = new QWidget(FileViewer);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(centralwidget->sizePolicy().hasHeightForWidth());
        centralwidget->setSizePolicy(sizePolicy);
        horizontalLayout = new QHBoxLayout(centralwidget);
        horizontalLayout->setSpacing(0);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        filehexview = new QHexEdit(centralwidget);
        filehexview->setObjectName(QStringLiteral("filehexview"));
        filehexview->setHexCaps(true);
        filehexview->setDynamicBytesPerLine(true);
        filehexview->setOverwriteMode(false);
        filehexview->setReadOnly(true);

        horizontalLayout->addWidget(filehexview);

        FileViewer->setCentralWidget(centralwidget);
        statusBar = new QStatusBar(FileViewer);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        FileViewer->setStatusBar(statusBar);
        toolBar = new QToolBar(FileViewer);
        toolBar->setObjectName(QStringLiteral("toolBar"));
        toolBar->setMovable(false);
        toolBar->setIconSize(QSize(32, 32));
        toolBar->setFloatable(false);
        FileViewer->addToolBar(Qt::TopToolBarArea, toolBar);

        retranslateUi(FileViewer);

        QMetaObject::connectSlotsByName(FileViewer);
    } // setupUi

    void retranslateUi(QMainWindow *FileViewer)
    {
        FileViewer->setWindowTitle(QApplication::translate("FileViewer", "File Viewer", Q_NULLPTR));
        actionHex->setText(QApplication::translate("FileViewer", "Hex", Q_NULLPTR));
        actionText->setText(QApplication::translate("FileViewer", "Text", Q_NULLPTR));
        actionOmni->setText(QApplication::translate("FileViewer", "Omni", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        filehexview->setToolTip(QApplication::translate("FileViewer", "QHexEditWidget", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_WHATSTHIS
        filehexview->setWhatsThis(QApplication::translate("FileViewer", "QHexEdit widget allow to edit the data in hex view.", Q_NULLPTR));
#endif // QT_NO_WHATSTHIS
        toolBar->setWindowTitle(QApplication::translate("FileViewer", "toolBar", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class FileViewer: public Ui_FileViewer {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FILEVIEWER_H
