/********************************************************************************
** Form generated from reading UI file 'imageviewer.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_IMAGEVIEWER_H
#define UI_IMAGEVIEWER_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_ImageViewer
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QListWidget *listWidget;

    void setupUi(QDialog *ImageViewer)
    {
        if (ImageViewer->objectName().isEmpty())
            ImageViewer->setObjectName(QString::fromUtf8("ImageViewer"));
        ImageViewer->resize(820, 525);
        ImageViewer->setSizeGripEnabled(false);
        ImageViewer->setModal(false);
        verticalLayout = new QVBoxLayout(ImageViewer);
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(3);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(5, -1, -1, -1);
        label = new QLabel(ImageViewer);
        label->setObjectName(QString::fromUtf8("label"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy);
        label->setTextFormat(Qt::PlainText);
        label->setScaledContents(true);
        label->setMargin(5);

        horizontalLayout->addWidget(label);


        verticalLayout->addLayout(horizontalLayout);

        listWidget = new QListWidget(ImageViewer);
        listWidget->setObjectName(QString::fromUtf8("listWidget"));
        listWidget->setEditTriggers(QAbstractItemView::DoubleClicked|QAbstractItemView::SelectedClicked);
        listWidget->setProperty("showDropIndicator", QVariant(false));
        listWidget->setDragDropMode(QAbstractItemView::NoDragDrop);
        listWidget->setDefaultDropAction(Qt::IgnoreAction);
        listWidget->setHorizontalScrollMode(QAbstractItemView::ScrollPerItem);
        listWidget->setMovement(QListView::Static);
        listWidget->setFlow(QListView::LeftToRight);
        listWidget->setProperty("isWrapping", QVariant(true));
        listWidget->setResizeMode(QListView::Adjust);
        listWidget->setLayoutMode(QListView::Batched);
        listWidget->setSpacing(10);
        listWidget->setViewMode(QListView::IconMode);
        listWidget->setUniformItemSizes(false);
        listWidget->setSelectionRectVisible(false);

        verticalLayout->addWidget(listWidget);


        retranslateUi(ImageViewer);

        QMetaObject::connectSlotsByName(ImageViewer);
    } // setupUi

    void retranslateUi(QDialog *ImageViewer)
    {
        ImageViewer->setWindowTitle(QCoreApplication::translate("ImageViewer", "Image Viewer", nullptr));
        label->setText(QCoreApplication::translate("ImageViewer", "Loading...", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ImageViewer: public Ui_ImageViewer {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_IMAGEVIEWER_H
