/********************************************************************************
** Form generated from reading UI file 'registryviewer.ui'
**
** Created by: Qt User Interface Compiler version 5.15.8
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_REGISTRYVIEWER_H
#define UI_REGISTRYVIEWER_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_RegistryDialog
{
public:
    QVBoxLayout *verticalLayout;
    QSplitter *splitter_3;
    QSplitter *splitter;
    QTreeWidget *treeWidget;
    QSplitter *splitter_2;
    QTableWidget *tableWidget;
    QPlainTextEdit *plainTextEdit;
    QLabel *label;

    void setupUi(QDialog *RegistryDialog)
    {
        if (RegistryDialog->objectName().isEmpty())
            RegistryDialog->setObjectName(QString::fromUtf8("RegistryDialog"));
        RegistryDialog->setWindowModality(Qt::NonModal);
        RegistryDialog->resize(1400, 720);
        RegistryDialog->setSizeGripEnabled(true);
        RegistryDialog->setModal(false);
        verticalLayout = new QVBoxLayout(RegistryDialog);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(1, 1, 1, 1);
        splitter_3 = new QSplitter(RegistryDialog);
        splitter_3->setObjectName(QString::fromUtf8("splitter_3"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(1);
        sizePolicy.setHeightForWidth(splitter_3->sizePolicy().hasHeightForWidth());
        splitter_3->setSizePolicy(sizePolicy);
        splitter_3->setOrientation(Qt::Horizontal);
        splitter = new QSplitter(splitter_3);
        splitter->setObjectName(QString::fromUtf8("splitter"));
        QSizePolicy sizePolicy1(QSizePolicy::MinimumExpanding, QSizePolicy::Ignored);
        sizePolicy1.setHorizontalStretch(2);
        sizePolicy1.setVerticalStretch(1);
        sizePolicy1.setHeightForWidth(splitter->sizePolicy().hasHeightForWidth());
        splitter->setSizePolicy(sizePolicy1);
        splitter->setOrientation(Qt::Horizontal);
        splitter->setOpaqueResize(true);
        splitter->setChildrenCollapsible(true);
        treeWidget = new QTreeWidget(splitter);
        QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem();
        __qtreewidgetitem->setText(0, QString::fromUtf8("1"));
        treeWidget->setHeaderItem(__qtreewidgetitem);
        treeWidget->setObjectName(QString::fromUtf8("treeWidget"));
        QSizePolicy sizePolicy2(QSizePolicy::MinimumExpanding, QSizePolicy::Expanding);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(1);
        sizePolicy2.setHeightForWidth(treeWidget->sizePolicy().hasHeightForWidth());
        treeWidget->setSizePolicy(sizePolicy2);
        treeWidget->setSelectionBehavior(QAbstractItemView::SelectItems);
        splitter->addWidget(treeWidget);
        treeWidget->header()->setVisible(false);
        splitter_3->addWidget(splitter);
        splitter_2 = new QSplitter(splitter_3);
        splitter_2->setObjectName(QString::fromUtf8("splitter_2"));
        QSizePolicy sizePolicy3(QSizePolicy::Preferred, QSizePolicy::Expanding);
        sizePolicy3.setHorizontalStretch(2);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(splitter_2->sizePolicy().hasHeightForWidth());
        splitter_2->setSizePolicy(sizePolicy3);
        splitter_2->setOrientation(Qt::Vertical);
        tableWidget = new QTableWidget(splitter_2);
        if (tableWidget->columnCount() < 3)
            tableWidget->setColumnCount(3);
        tableWidget->setObjectName(QString::fromUtf8("tableWidget"));
        tableWidget->setAlternatingRowColors(true);
        tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
        tableWidget->setRowCount(0);
        tableWidget->setColumnCount(3);
        splitter_2->addWidget(tableWidget);
        tableWidget->horizontalHeader()->setVisible(true);
        tableWidget->horizontalHeader()->setCascadingSectionResizes(true);
        tableWidget->horizontalHeader()->setStretchLastSection(true);
        tableWidget->verticalHeader()->setVisible(false);
        plainTextEdit = new QPlainTextEdit(splitter_2);
        plainTextEdit->setObjectName(QString::fromUtf8("plainTextEdit"));
        QFont font;
        font.setFamily(QString::fromUtf8("Source Code Pro"));
        plainTextEdit->setFont(font);
        plainTextEdit->setReadOnly(true);
        splitter_2->addWidget(plainTextEdit);
        splitter_3->addWidget(splitter_2);

        verticalLayout->addWidget(splitter_3);

        label = new QLabel(RegistryDialog);
        label->setObjectName(QString::fromUtf8("label"));
        label->setFrameShape(QFrame::NoFrame);
        label->setFrameShadow(QFrame::Sunken);
        label->setMargin(6);
        label->setIndent(5);

        verticalLayout->addWidget(label);


        retranslateUi(RegistryDialog);

        QMetaObject::connectSlotsByName(RegistryDialog);
    } // setupUi

    void retranslateUi(QDialog *RegistryDialog)
    {
        RegistryDialog->setWindowTitle(QCoreApplication::translate("RegistryDialog", "Registry Viewer", nullptr));
        plainTextEdit->setPlaceholderText(QString());
        label->setText(QCoreApplication::translate("RegistryDialog", "TextLabel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class RegistryDialog: public Ui_RegistryDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_REGISTRYVIEWER_H
