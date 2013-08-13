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
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QSpacerItem>
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
    QTreeView *analysisTreeView;
    QGroupBox *groupBox_3;
    QVBoxLayout *verticalLayout_4;
    QVBoxLayout *verticalLayout_2;
    QGridLayout *gridLayout;
    QLineEdit *lineEdit_2;
    QLineEdit *lineEdit_3;
    QLabel *label;
    QProgressBar *progressBar;
    QLineEdit *lineEdit;
    QLabel *label_2;
    QLabel *label_4;
    QLabel *label_3;
    QSpacerItem *verticalSpacer;
    QSpacerItem *verticalSpacer_2;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout_3;
    QTableWidget *tableWidget;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *ProgressWindow)
    {
        if (ProgressWindow->objectName().isEmpty())
            ProgressWindow->setObjectName(QStringLiteral("ProgressWindow"));
        ProgressWindow->resize(919, 292);
        verticalLayout = new QVBoxLayout(ProgressWindow);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        groupBox_2 = new QGroupBox(ProgressWindow);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        horizontalLayout_2 = new QHBoxLayout(groupBox_2);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        analysisTreeView = new QTreeView(groupBox_2);
        analysisTreeView->setObjectName(QStringLiteral("analysisTreeView"));
        analysisTreeView->header()->setVisible(false);

        horizontalLayout_2->addWidget(analysisTreeView);


        horizontalLayout->addWidget(groupBox_2);

        groupBox_3 = new QGroupBox(ProgressWindow);
        groupBox_3->setObjectName(QStringLiteral("groupBox_3"));
        verticalLayout_4 = new QVBoxLayout(groupBox_3);
        verticalLayout_4->setSpacing(3);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        verticalLayout_4->setContentsMargins(0, 0, 0, 0);
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setHorizontalSpacing(6);
        gridLayout->setVerticalSpacing(1);
        lineEdit_2 = new QLineEdit(groupBox_3);
        lineEdit_2->setObjectName(QStringLiteral("lineEdit_2"));
        lineEdit_2->setReadOnly(true);

        gridLayout->addWidget(lineEdit_2, 2, 1, 1, 1);

        lineEdit_3 = new QLineEdit(groupBox_3);
        lineEdit_3->setObjectName(QStringLiteral("lineEdit_3"));
        lineEdit_3->setReadOnly(true);

        gridLayout->addWidget(lineEdit_3, 3, 1, 1, 1);

        label = new QLabel(groupBox_3);
        label->setObjectName(QStringLiteral("label"));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        progressBar = new QProgressBar(groupBox_3);
        progressBar->setObjectName(QStringLiteral("progressBar"));
        progressBar->setValue(25);
        progressBar->setInvertedAppearance(false);

        gridLayout->addWidget(progressBar, 0, 1, 1, 1);

        lineEdit = new QLineEdit(groupBox_3);
        lineEdit->setObjectName(QStringLiteral("lineEdit"));
        lineEdit->setDragEnabled(false);
        lineEdit->setReadOnly(true);

        gridLayout->addWidget(lineEdit, 1, 1, 1, 1);

        label_2 = new QLabel(groupBox_3);
        label_2->setObjectName(QStringLiteral("label_2"));

        gridLayout->addWidget(label_2, 1, 0, 1, 1);

        label_4 = new QLabel(groupBox_3);
        label_4->setObjectName(QStringLiteral("label_4"));

        gridLayout->addWidget(label_4, 3, 0, 1, 1);

        label_3 = new QLabel(groupBox_3);
        label_3->setObjectName(QStringLiteral("label_3"));

        gridLayout->addWidget(label_3, 2, 0, 1, 1);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(verticalSpacer, 4, 0, 1, 1);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(verticalSpacer_2, 4, 1, 1, 1);


        verticalLayout_2->addLayout(gridLayout);


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
        ProgressWindow->setWindowTitle(QApplication::translate("ProgressWindow", "Progress Window", 0));
        groupBox_2->setTitle(QApplication::translate("ProgressWindow", "Processing Jobs", 0));
        groupBox_3->setTitle(QApplication::translate("ProgressWindow", "Job Progress", 0));
        label->setText(QApplication::translate("ProgressWindow", "Overall: ", 0));
        label_2->setText(QApplication::translate("ProgressWindow", "<html><head/><body><p>Analysis State:</p></body></html>", 0));
        label_4->setText(QApplication::translate("ProgressWindow", "Files Processed: ", 0));
        label_3->setText(QApplication::translate("ProgressWindow", "Files Found:", 0));
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
