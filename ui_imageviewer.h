/********************************************************************************
** Form generated from reading UI file 'imageviewer.ui'
**
** Created by: Qt User Interface Compiler version 5.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_IMAGEVIEWER_H
#define UI_IMAGEVIEWER_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListView>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_ImageViewer
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QLabel *label;
    QSpacerItem *horizontalSpacer_2;
    QListWidget *listWidget;
    QListView *listView;

    void setupUi(QDialog *ImageViewer)
    {
        if (ImageViewer->objectName().isEmpty())
            ImageViewer->setObjectName(QStringLiteral("ImageViewer"));
        ImageViewer->resize(320, 240);
        ImageViewer->setSizeGripEnabled(false);
        ImageViewer->setModal(false);
        verticalLayout = new QVBoxLayout(ImageViewer);
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(3);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(5, -1, -1, -1);
        horizontalSpacer = new QSpacerItem(5, 20, QSizePolicy::Preferred, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        label = new QLabel(ImageViewer);
        label->setObjectName(QStringLiteral("label"));

        horizontalLayout->addWidget(label);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);


        verticalLayout->addLayout(horizontalLayout);

        listWidget = new QListWidget(ImageViewer);
        listWidget->setObjectName(QStringLiteral("listWidget"));
        listWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
        listWidget->setFlow(QListView::LeftToRight);
        listWidget->setProperty("isWrapping", QVariant(true));
        listWidget->setResizeMode(QListView::Adjust);
        listWidget->setLayoutMode(QListView::Batched);
        listWidget->setSpacing(0);
        listWidget->setViewMode(QListView::IconMode);
        listWidget->setUniformItemSizes(true);

        verticalLayout->addWidget(listWidget);

        listView = new QListView(ImageViewer);
        listView->setObjectName(QStringLiteral("listView"));
        listView->setSizeAdjustPolicy(QAbstractScrollArea::AdjustIgnored);
        listView->setEditTriggers(QAbstractItemView::NoEditTriggers);
        listView->setProperty("showDropIndicator", QVariant(false));
        listView->setMovement(QListView::Static);
        listView->setResizeMode(QListView::Adjust);
        listView->setLayoutMode(QListView::Batched);
        listView->setViewMode(QListView::IconMode);
        listView->setUniformItemSizes(true);
        listView->setBatchSize(1);

        verticalLayout->addWidget(listView);


        retranslateUi(ImageViewer);

        QMetaObject::connectSlotsByName(ImageViewer);
    } // setupUi

    void retranslateUi(QDialog *ImageViewer)
    {
        ImageViewer->setWindowTitle(QApplication::translate("ImageViewer", "Image Viewer", Q_NULLPTR));
        label->setText(QApplication::translate("ImageViewer", "Loading...", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class ImageViewer: public Ui_ImageViewer {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_IMAGEVIEWER_H
