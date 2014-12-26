/********************************************************************************
** Form generated from reading UI file 'fileviewer.ui'
**
** Created by: Qt User Interface Compiler version 5.4.0
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
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(centralwidget->sizePolicy().hasHeightForWidth());
        centralwidget->setSizePolicy(sizePolicy);
        horizontalLayout = new QHBoxLayout(centralwidget);
        horizontalLayout->setSpacing(0);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        FileViewer->setCentralWidget(centralwidget);
        statusBar = new QStatusBar(FileViewer);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        FileViewer->setStatusBar(statusBar);

        retranslateUi(FileViewer);

        QMetaObject::connectSlotsByName(FileViewer);
    } // setupUi

    void retranslateUi(QMainWindow *FileViewer)
    {
        FileViewer->setWindowTitle(QApplication::translate("FileViewer", "File Viewer", 0));
        actionHex->setText(QApplication::translate("FileViewer", "Hex", 0));
        actionText->setText(QApplication::translate("FileViewer", "Text", 0));
        actionOmni->setText(QApplication::translate("FileViewer", "Omni", 0));
    } // retranslateUi

};

namespace Ui {
    class FileViewer: public Ui_FileViewer {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FILEVIEWER_H
