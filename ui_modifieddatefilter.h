/********************************************************************************
** Form generated from reading UI file 'modifieddatefilter.ui'
**
** Created by: Qt User Interface Compiler version 5.15.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MODIFIEDDATEFILTER_H
#define UI_MODIFIEDDATEFILTER_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDateTimeEdit>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_ModifiedDateFilter
{
public:
    QGridLayout *gridLayout;
    QCheckBox *morecheckBox;
    QDateTimeEdit *moredateTimeEdit;
    QCheckBox *lesscheckBox;
    QDateTimeEdit *lessdateTimeEdit;
    QPushButton *pushButton;
    QPushButton *pushButton_2;

    void setupUi(QDialog *ModifiedDateFilter)
    {
        if (ModifiedDateFilter->objectName().isEmpty())
            ModifiedDateFilter->setObjectName(QString::fromUtf8("ModifiedDateFilter"));
        ModifiedDateFilter->resize(348, 105);
        gridLayout = new QGridLayout(ModifiedDateFilter);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        morecheckBox = new QCheckBox(ModifiedDateFilter);
        morecheckBox->setObjectName(QString::fromUtf8("morecheckBox"));

        gridLayout->addWidget(morecheckBox, 0, 0, 1, 1);

        moredateTimeEdit = new QDateTimeEdit(ModifiedDateFilter);
        moredateTimeEdit->setObjectName(QString::fromUtf8("moredateTimeEdit"));
        moredateTimeEdit->setEnabled(false);
        moredateTimeEdit->setCalendarPopup(true);
        moredateTimeEdit->setTimeSpec(Qt::UTC);

        gridLayout->addWidget(moredateTimeEdit, 0, 1, 1, 1);

        lesscheckBox = new QCheckBox(ModifiedDateFilter);
        lesscheckBox->setObjectName(QString::fromUtf8("lesscheckBox"));

        gridLayout->addWidget(lesscheckBox, 1, 0, 1, 1);

        lessdateTimeEdit = new QDateTimeEdit(ModifiedDateFilter);
        lessdateTimeEdit->setObjectName(QString::fromUtf8("lessdateTimeEdit"));
        lessdateTimeEdit->setEnabled(false);
        lessdateTimeEdit->setCalendarPopup(true);
        lessdateTimeEdit->setTimeSpec(Qt::UTC);

        gridLayout->addWidget(lessdateTimeEdit, 1, 1, 1, 1);

        pushButton = new QPushButton(ModifiedDateFilter);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setStyleSheet(QString::fromUtf8(""));
        pushButton->setFlat(false);

        gridLayout->addWidget(pushButton, 2, 1, 1, 1);

        pushButton_2 = new QPushButton(ModifiedDateFilter);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));

        gridLayout->addWidget(pushButton_2, 2, 0, 1, 1);


        retranslateUi(ModifiedDateFilter);

        pushButton->setDefault(true);


        QMetaObject::connectSlotsByName(ModifiedDateFilter);
    } // setupUi

    void retranslateUi(QDialog *ModifiedDateFilter)
    {
        ModifiedDateFilter->setWindowTitle(QCoreApplication::translate("ModifiedDateFilter", "Modified Date Filter", nullptr));
        morecheckBox->setText(QCoreApplication::translate("ModifiedDateFilter", "Show Items with date >", nullptr));
        moredateTimeEdit->setDisplayFormat(QCoreApplication::translate("ModifiedDateFilter", "MM/dd/yyyy HH:mm:ss", nullptr));
        lesscheckBox->setText(QCoreApplication::translate("ModifiedDateFilter", "Show Items with date <", nullptr));
        lessdateTimeEdit->setDisplayFormat(QCoreApplication::translate("ModifiedDateFilter", "MM/dd/yyyy HH:mm:ss", nullptr));
        pushButton->setText(QCoreApplication::translate("ModifiedDateFilter", "Apply", nullptr));
        pushButton_2->setText(QCoreApplication::translate("ModifiedDateFilter", "Cancel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ModifiedDateFilter: public Ui_ModifiedDateFilter {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MODIFIEDDATEFILTER_H
