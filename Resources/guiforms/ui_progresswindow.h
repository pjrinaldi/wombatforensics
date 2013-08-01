/********************************************************************************
** Form generated from reading UI file 'progresswindow.ui'
**
** Created by: Qt User Interface Compiler version 5.1.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PROGRESSWINDOW_H
#define UI_PROGRESSWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QTreeView>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_ProgressWindow
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QGroupBox *groupBox_2;
    QHBoxLayout *horizontalLayout_2;
    QTreeView *treeView;
    QGroupBox *groupBox_3;
    QVBoxLayout *verticalLayout_4;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label;
    QProgressBar *progressBar;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout_3;
    QTableWidget *tableWidget;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *ProgressWindow)
    {
        if (ProgressWindow->objectName().isEmpty())
            ProgressWindow->setObjectName(QStringLiteral("ProgressWindow"));
        ProgressWindow->resize(919, 451);
        verticalLayout = new QVBoxLayout(ProgressWindow);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        groupBox_2 = new QGroupBox(ProgressWindow);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        horizontalLayout_2 = new QHBoxLayout(groupBox_2);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        treeView = new QTreeView(groupBox_2);
        treeView->setObjectName(QStringLiteral("treeView"));

        horizontalLayout_2->addWidget(treeView);


        horizontalLayout->addWidget(groupBox_2);

        groupBox_3 = new QGroupBox(ProgressWindow);
        groupBox_3->setObjectName(QStringLiteral("groupBox_3"));
        verticalLayout_4 = new QVBoxLayout(groupBox_3);
        verticalLayout_4->setSpacing(3);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        verticalLayout_4->setContentsMargins(0, 0, 0, 0);
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        label = new QLabel(groupBox_3);
        label->setObjectName(QStringLiteral("label"));

        horizontalLayout_3->addWidget(label);

        progressBar = new QProgressBar(groupBox_3);
        progressBar->setObjectName(QStringLiteral("progressBar"));
        progressBar->setValue(24);

        horizontalLayout_3->addWidget(progressBar);


        verticalLayout_2->addLayout(horizontalLayout_3);


        verticalLayout_4->addLayout(verticalLayout_2);


        horizontalLayout->addWidget(groupBox_3);

        groupBox = new QGroupBox(ProgressWindow);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        verticalLayout_3 = new QVBoxLayout(groupBox);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(0, 0, 0, 0);
        tableWidget = new QTableWidget(groupBox);
        if (tableWidget->columnCount() < 2)
            tableWidget->setColumnCount(2);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        tableWidget->setObjectName(QStringLiteral("tableWidget"));
        tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
        tableWidget->horizontalHeader()->setVisible(true);
        tableWidget->horizontalHeader()->setCascadingSectionResizes(false);
        tableWidget->horizontalHeader()->setStretchLastSection(true);
        tableWidget->verticalHeader()->setVisible(false);

        verticalLayout_3->addWidget(tableWidget);


        horizontalLayout->addWidget(groupBox);


        verticalLayout->addLayout(horizontalLayout);

        buttonBox = new QDialogButtonBox(ProgressWindow);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Close);

        verticalLayout->addWidget(buttonBox);


        retranslateUi(ProgressWindow);
        QObject::connect(buttonBox, SIGNAL(accepted()), ProgressWindow, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), ProgressWindow, SLOT(reject()));

        QMetaObject::connectSlotsByName(ProgressWindow);
    } // setupUi

    void retranslateUi(QDialog *ProgressWindow)
    {
        ProgressWindow->setWindowTitle(QApplication::translate("ProgressWindow", "Dialog", 0));
        groupBox_2->setTitle(QApplication::translate("ProgressWindow", "Processing Jobs", 0));
        groupBox_3->setTitle(QApplication::translate("ProgressWindow", "Job Progress", 0));
        label->setText(QApplication::translate("ProgressWindow", "Overall: ", 0));
        groupBox->setTitle(QApplication::translate("ProgressWindow", "Messages", 0));
        QTableWidgetItem *___qtablewidgetitem = tableWidget->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("ProgressWindow", "Type", 0));
        QTableWidgetItem *___qtablewidgetitem1 = tableWidget->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("ProgressWindow", "Message", 0));
    } // retranslateUi

};

namespace Ui {
    class ProgressWindow: public Ui_ProgressWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PROGRESSWINDOW_H
