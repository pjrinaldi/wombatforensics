/********************************************************************************
** Form generated from reading UI file 'createddatefilter.ui'
**
** Created by: Qt User Interface Compiler version 5.15.8
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CREATEDDATEFILTER_H
#define UI_CREATEDDATEFILTER_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDateTimeEdit>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_CreatedDateFilter
{
public:
    QGridLayout *gridLayout;
    QCheckBox *morecheckBox;
    QDateTimeEdit *moredateTimeEdit;
    QCheckBox *lesscheckBox;
    QDateTimeEdit *lessdateTimeEdit;
    QPushButton *pushButton;
    QPushButton *pushButton_2;

    void setupUi(QDialog *CreatedDateFilter)
    {
        if (CreatedDateFilter->objectName().isEmpty())
            CreatedDateFilter->setObjectName(QString::fromUtf8("CreatedDateFilter"));
        CreatedDateFilter->setWindowModality(Qt::ApplicationModal);
        CreatedDateFilter->resize(348, 105);
        gridLayout = new QGridLayout(CreatedDateFilter);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        morecheckBox = new QCheckBox(CreatedDateFilter);
        morecheckBox->setObjectName(QString::fromUtf8("morecheckBox"));

        gridLayout->addWidget(morecheckBox, 0, 0, 1, 1);

        moredateTimeEdit = new QDateTimeEdit(CreatedDateFilter);
        moredateTimeEdit->setObjectName(QString::fromUtf8("moredateTimeEdit"));
        moredateTimeEdit->setEnabled(false);
        moredateTimeEdit->setCalendarPopup(true);
        moredateTimeEdit->setTimeSpec(Qt::UTC);

        gridLayout->addWidget(moredateTimeEdit, 0, 1, 1, 1);

        lesscheckBox = new QCheckBox(CreatedDateFilter);
        lesscheckBox->setObjectName(QString::fromUtf8("lesscheckBox"));

        gridLayout->addWidget(lesscheckBox, 1, 0, 1, 1);

        lessdateTimeEdit = new QDateTimeEdit(CreatedDateFilter);
        lessdateTimeEdit->setObjectName(QString::fromUtf8("lessdateTimeEdit"));
        lessdateTimeEdit->setEnabled(false);
        lessdateTimeEdit->setCalendarPopup(true);
        lessdateTimeEdit->setTimeSpec(Qt::UTC);

        gridLayout->addWidget(lessdateTimeEdit, 1, 1, 1, 1);

        pushButton = new QPushButton(CreatedDateFilter);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setStyleSheet(QString::fromUtf8(""));
        pushButton->setFlat(false);

        gridLayout->addWidget(pushButton, 2, 1, 1, 1);

        pushButton_2 = new QPushButton(CreatedDateFilter);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));

        gridLayout->addWidget(pushButton_2, 2, 0, 1, 1);


        retranslateUi(CreatedDateFilter);

        pushButton->setDefault(true);


        QMetaObject::connectSlotsByName(CreatedDateFilter);
    } // setupUi

    void retranslateUi(QDialog *CreatedDateFilter)
    {
        CreatedDateFilter->setWindowTitle(QCoreApplication::translate("CreatedDateFilter", "Created Date Filter", nullptr));
        morecheckBox->setText(QCoreApplication::translate("CreatedDateFilter", "Show Items with date >", nullptr));
        moredateTimeEdit->setDisplayFormat(QCoreApplication::translate("CreatedDateFilter", "MM/dd/yyyy HH:mm:ss", nullptr));
        lesscheckBox->setText(QCoreApplication::translate("CreatedDateFilter", "Show Items with date <", nullptr));
        lessdateTimeEdit->setDisplayFormat(QCoreApplication::translate("CreatedDateFilter", "MM/dd/yyyy HH:mm:ss", nullptr));
        pushButton->setText(QCoreApplication::translate("CreatedDateFilter", "Apply", nullptr));
        pushButton_2->setText(QCoreApplication::translate("CreatedDateFilter", "Cancel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class CreatedDateFilter: public Ui_CreatedDateFilter {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CREATEDDATEFILTER_H
