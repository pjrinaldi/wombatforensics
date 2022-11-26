/********************************************************************************
** Form generated from reading UI file 'accesseddatefilter.ui'
**
** Created by: Qt User Interface Compiler version 5.15.7
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ACCESSEDDATEFILTER_H
#define UI_ACCESSEDDATEFILTER_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDateTimeEdit>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_AccessedDateFilter
{
public:
    QGridLayout *gridLayout;
    QCheckBox *morecheckBox;
    QDateTimeEdit *moredateTimeEdit;
    QCheckBox *lesscheckBox;
    QDateTimeEdit *lessdateTimeEdit;
    QPushButton *pushButton;
    QPushButton *pushButton_2;

    void setupUi(QDialog *AccessedDateFilter)
    {
        if (AccessedDateFilter->objectName().isEmpty())
            AccessedDateFilter->setObjectName(QString::fromUtf8("AccessedDateFilter"));
        AccessedDateFilter->setWindowModality(Qt::ApplicationModal);
        AccessedDateFilter->resize(348, 105);
        gridLayout = new QGridLayout(AccessedDateFilter);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        morecheckBox = new QCheckBox(AccessedDateFilter);
        morecheckBox->setObjectName(QString::fromUtf8("morecheckBox"));

        gridLayout->addWidget(morecheckBox, 0, 0, 1, 1);

        moredateTimeEdit = new QDateTimeEdit(AccessedDateFilter);
        moredateTimeEdit->setObjectName(QString::fromUtf8("moredateTimeEdit"));
        moredateTimeEdit->setEnabled(false);
        moredateTimeEdit->setCalendarPopup(true);
        moredateTimeEdit->setTimeSpec(Qt::UTC);

        gridLayout->addWidget(moredateTimeEdit, 0, 1, 1, 1);

        lesscheckBox = new QCheckBox(AccessedDateFilter);
        lesscheckBox->setObjectName(QString::fromUtf8("lesscheckBox"));

        gridLayout->addWidget(lesscheckBox, 1, 0, 1, 1);

        lessdateTimeEdit = new QDateTimeEdit(AccessedDateFilter);
        lessdateTimeEdit->setObjectName(QString::fromUtf8("lessdateTimeEdit"));
        lessdateTimeEdit->setEnabled(false);
        lessdateTimeEdit->setCalendarPopup(true);
        lessdateTimeEdit->setTimeSpec(Qt::UTC);

        gridLayout->addWidget(lessdateTimeEdit, 1, 1, 1, 1);

        pushButton = new QPushButton(AccessedDateFilter);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setStyleSheet(QString::fromUtf8(""));
        pushButton->setFlat(false);

        gridLayout->addWidget(pushButton, 2, 1, 1, 1);

        pushButton_2 = new QPushButton(AccessedDateFilter);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));

        gridLayout->addWidget(pushButton_2, 2, 0, 1, 1);


        retranslateUi(AccessedDateFilter);

        pushButton->setDefault(true);


        QMetaObject::connectSlotsByName(AccessedDateFilter);
    } // setupUi

    void retranslateUi(QDialog *AccessedDateFilter)
    {
        AccessedDateFilter->setWindowTitle(QCoreApplication::translate("AccessedDateFilter", "Accessed Date Filter", nullptr));
        morecheckBox->setText(QCoreApplication::translate("AccessedDateFilter", "Show Items with date >", nullptr));
        moredateTimeEdit->setDisplayFormat(QCoreApplication::translate("AccessedDateFilter", "MM/dd/yyyy HH:mm:ss", nullptr));
        lesscheckBox->setText(QCoreApplication::translate("AccessedDateFilter", "Show Items with date <", nullptr));
        lessdateTimeEdit->setDisplayFormat(QCoreApplication::translate("AccessedDateFilter", "MM/dd/yyyy HH:mm:ss", nullptr));
        pushButton->setText(QCoreApplication::translate("AccessedDateFilter", "Apply", nullptr));
        pushButton_2->setText(QCoreApplication::translate("AccessedDateFilter", "Cancel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class AccessedDateFilter: public Ui_AccessedDateFilter {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ACCESSEDDATEFILTER_H
