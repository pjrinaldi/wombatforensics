/********************************************************************************
** Form generated from reading UI file 'registryviewer.ui'
**
** Created by: Qt User Interface Compiler version 5.15.0
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
#include <QtWidgets/QToolButton>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_RegistryDialog
{
public:
    QVBoxLayout *verticalLayout;
    QToolButton *toolButton;
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
        RegistryDialog->setWindowModality(Qt::ApplicationModal);
        RegistryDialog->resize(807, 684);
        RegistryDialog->setSizeGripEnabled(true);
        RegistryDialog->setModal(true);
        verticalLayout = new QVBoxLayout(RegistryDialog);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(1, 1, 1, 1);
        toolButton = new QToolButton(RegistryDialog);
        toolButton->setObjectName(QString::fromUtf8("toolButton"));

        verticalLayout->addWidget(toolButton);

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
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Ignored);
        sizePolicy1.setHorizontalStretch(1);
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
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy2.setHorizontalStretch(1);
        sizePolicy2.setVerticalStretch(1);
        sizePolicy2.setHeightForWidth(treeWidget->sizePolicy().hasHeightForWidth());
        treeWidget->setSizePolicy(sizePolicy2);
        splitter->addWidget(treeWidget);
        splitter_3->addWidget(splitter);
        splitter_2 = new QSplitter(splitter_3);
        splitter_2->setObjectName(QString::fromUtf8("splitter_2"));
        splitter_2->setOrientation(Qt::Vertical);
        tableWidget = new QTableWidget(splitter_2);
        if (tableWidget->columnCount() < 2)
            tableWidget->setColumnCount(2);
        tableWidget->setObjectName(QString::fromUtf8("tableWidget"));
        tableWidget->setColumnCount(2);
        splitter_2->addWidget(tableWidget);
        tableWidget->horizontalHeader()->setCascadingSectionResizes(false);
        tableWidget->horizontalHeader()->setStretchLastSection(true);
        plainTextEdit = new QPlainTextEdit(splitter_2);
        plainTextEdit->setObjectName(QString::fromUtf8("plainTextEdit"));
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
        toolButton->setText(QCoreApplication::translate("RegistryDialog", "...", nullptr));
        label->setText(QCoreApplication::translate("RegistryDialog", "TextLabel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class RegistryDialog: public Ui_RegistryDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_REGISTRYVIEWER_H
