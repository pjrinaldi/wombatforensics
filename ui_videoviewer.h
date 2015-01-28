/********************************************************************************
** Form generated from reading UI file 'videoviewer.ui'
**
** Created by: Qt User Interface Compiler version 5.4.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_VIDEOVIEWER_H
#define UI_VIDEOVIEWER_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>

QT_BEGIN_NAMESPACE

class Ui_VideoViewer
{
public:
    QHBoxLayout *horizontalLayout;

    void setupUi(QDialog *VideoViewer)
    {
        if (VideoViewer->objectName().isEmpty())
            VideoViewer->setObjectName(QStringLiteral("VideoViewer"));
        VideoViewer->resize(479, 332);
        horizontalLayout = new QHBoxLayout(VideoViewer);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);

        retranslateUi(VideoViewer);

        QMetaObject::connectSlotsByName(VideoViewer);
    } // setupUi

    void retranslateUi(QDialog *VideoViewer)
    {
        VideoViewer->setWindowTitle(QApplication::translate("VideoViewer", "View Video", 0));
    } // retranslateUi

};

namespace Ui {
    class VideoViewer: public Ui_VideoViewer {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VIDEOVIEWER_H
