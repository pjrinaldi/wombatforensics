/********************************************************************************
** Form generated from reading UI file 'filecarvingdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.14.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FILECARVINGDIALOG_H
#define UI_FILECARVINGDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_FileCarvingDialog
{
public:
    QVBoxLayout *verticalLayout_2;
    QVBoxLayout *verticalLayout;
    QGroupBox *groupBox_2;
    QVBoxLayout *verticalLayout_3;
    QHBoxLayout *horizontalLayout_2;
    QListWidget *partitionlist;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout_4;
    QTreeWidget *filetypetree;
    QSpacerItem *verticalSpacer;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *cancelButton;
    QPushButton *processButton;

    void setupUi(QDialog *FileCarvingDialog)
    {
        if (FileCarvingDialog->objectName().isEmpty())
            FileCarvingDialog->setObjectName(QString::fromUtf8("FileCarvingDialog"));
        FileCarvingDialog->setWindowModality(Qt::ApplicationModal);
        FileCarvingDialog->resize(730, 599);
        FileCarvingDialog->setModal(true);
        verticalLayout_2 = new QVBoxLayout(FileCarvingDialog);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(9, 9, 9, 9);
        groupBox_2 = new QGroupBox(FileCarvingDialog);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(1);
        sizePolicy.setHeightForWidth(groupBox_2->sizePolicy().hasHeightForWidth());
        groupBox_2->setSizePolicy(sizePolicy);
        verticalLayout_3 = new QVBoxLayout(groupBox_2);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        partitionlist = new QListWidget(groupBox_2);
        partitionlist->setObjectName(QString::fromUtf8("partitionlist"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(1);
        sizePolicy1.setHeightForWidth(partitionlist->sizePolicy().hasHeightForWidth());
        partitionlist->setSizePolicy(sizePolicy1);
        partitionlist->setProperty("showDropIndicator", QVariant(false));
        partitionlist->setAlternatingRowColors(true);
        partitionlist->setSelectionMode(QAbstractItemView::MultiSelection);

        horizontalLayout_2->addWidget(partitionlist);


        verticalLayout_3->addLayout(horizontalLayout_2);


        verticalLayout->addWidget(groupBox_2);

        groupBox = new QGroupBox(FileCarvingDialog);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        sizePolicy.setHeightForWidth(groupBox->sizePolicy().hasHeightForWidth());
        groupBox->setSizePolicy(sizePolicy);
        verticalLayout_4 = new QVBoxLayout(groupBox);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        verticalLayout_4->setContentsMargins(0, 0, 0, 0);
        filetypetree = new QTreeWidget(groupBox);
        QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem();
        __qtreewidgetitem->setText(0, QString::fromUtf8("1"));
        filetypetree->setHeaderItem(__qtreewidgetitem);
        filetypetree->setObjectName(QString::fromUtf8("filetypetree"));
        sizePolicy1.setHeightForWidth(filetypetree->sizePolicy().hasHeightForWidth());
        filetypetree->setSizePolicy(sizePolicy1);
        filetypetree->setProperty("showDropIndicator", QVariant(false));
        filetypetree->setSelectionMode(QAbstractItemView::SingleSelection);
        filetypetree->setUniformRowHeights(true);
        filetypetree->header()->setVisible(false);

        verticalLayout_4->addWidget(filetypetree);


        verticalLayout->addWidget(groupBox);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(-1, -1, 0, -1);
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        cancelButton = new QPushButton(FileCarvingDialog);
        cancelButton->setObjectName(QString::fromUtf8("cancelButton"));

        horizontalLayout->addWidget(cancelButton);

        processButton = new QPushButton(FileCarvingDialog);
        processButton->setObjectName(QString::fromUtf8("processButton"));
        QSizePolicy sizePolicy2(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(processButton->sizePolicy().hasHeightForWidth());
        processButton->setSizePolicy(sizePolicy2);

        horizontalLayout->addWidget(processButton);


        verticalLayout->addLayout(horizontalLayout);


        verticalLayout_2->addLayout(verticalLayout);


        retranslateUi(FileCarvingDialog);
        QObject::connect(cancelButton, SIGNAL(clicked()), FileCarvingDialog, SLOT(close()));

        QMetaObject::connectSlotsByName(FileCarvingDialog);
    } // setupUi

    void retranslateUi(QDialog *FileCarvingDialog)
    {
        FileCarvingDialog->setWindowTitle(QCoreApplication::translate("FileCarvingDialog", "File Carving", nullptr));
        groupBox_2->setTitle(QCoreApplication::translate("FileCarvingDialog", "Carve the Following Partitions(s)", nullptr));
        groupBox->setTitle(QCoreApplication::translate("FileCarvingDialog", "For the Following File Types", nullptr));
        cancelButton->setText(QCoreApplication::translate("FileCarvingDialog", "Cancel", nullptr));
        processButton->setText(QCoreApplication::translate("FileCarvingDialog", "Carve", nullptr));
    } // retranslateUi

};

namespace Ui {
    class FileCarvingDialog: public Ui_FileCarvingDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FILECARVINGDIALOG_H
