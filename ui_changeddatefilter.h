/********************************************************************************
** Form generated from reading UI file 'changeddatefilter.ui'
**
** Created by: Qt User Interface Compiler version 5.15.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CHANGEDDATEFILTER_H
#define UI_CHANGEDDATEFILTER_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDateTimeEdit>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_ChangedDateFilter
{
public:
    QGridLayout *gridLayout;
    QCheckBox *morecheckBox;
    QDateTimeEdit *moredateTimeEdit;
    QCheckBox *lesscheckBox;
    QDateTimeEdit *lessdateTimeEdit;
    QPushButton *pushButton;
    QPushButton *pushButton_2;

    void setupUi(QDialog *ChangedDateFilter)
    {
        if (ChangedDateFilter->objectName().isEmpty())
            ChangedDateFilter->setObjectName(QString::fromUtf8("ChangedDateFilter"));
        ChangedDateFilter->setWindowModality(Qt::ApplicationModal);
        ChangedDateFilter->resize(348, 105);
        gridLayout = new QGridLayout(ChangedDateFilter);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        morecheckBox = new QCheckBox(ChangedDateFilter);
        morecheckBox->setObjectName(QString::fromUtf8("morecheckBox"));

        gridLayout->addWidget(morecheckBox, 0, 0, 1, 1);

        moredateTimeEdit = new QDateTimeEdit(ChangedDateFilter);
        moredateTimeEdit->setObjectName(QString::fromUtf8("moredateTimeEdit"));
        moredateTimeEdit->setEnabled(false);
        moredateTimeEdit->setCalendarPopup(true);
        moredateTimeEdit->setTimeSpec(Qt::UTC);

        gridLayout->addWidget(moredateTimeEdit, 0, 1, 1, 1);

        lesscheckBox = new QCheckBox(ChangedDateFilter);
        lesscheckBox->setObjectName(QString::fromUtf8("lesscheckBox"));

        gridLayout->addWidget(lesscheckBox, 1, 0, 1, 1);

        lessdateTimeEdit = new QDateTimeEdit(ChangedDateFilter);
        lessdateTimeEdit->setObjectName(QString::fromUtf8("lessdateTimeEdit"));
        lessdateTimeEdit->setEnabled(false);
        lessdateTimeEdit->setCalendarPopup(true);
        lessdateTimeEdit->setTimeSpec(Qt::UTC);

        gridLayout->addWidget(lessdateTimeEdit, 1, 1, 1, 1);

        pushButton = new QPushButton(ChangedDateFilter);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setStyleSheet(QString::fromUtf8(""));
        pushButton->setFlat(false);

        gridLayout->addWidget(pushButton, 2, 1, 1, 1);

        pushButton_2 = new QPushButton(ChangedDateFilter);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));

        gridLayout->addWidget(pushButton_2, 2, 0, 1, 1);


        retranslateUi(ChangedDateFilter);

        pushButton->setDefault(true);


        QMetaObject::connectSlotsByName(ChangedDateFilter);
    } // setupUi

    void retranslateUi(QDialog *ChangedDateFilter)
    {
        ChangedDateFilter->setWindowTitle(QCoreApplication::translate("ChangedDateFilter", "Changed Date Filter", nullptr));
        morecheckBox->setText(QCoreApplication::translate("ChangedDateFilter", "Show Items with date >", nullptr));
        moredateTimeEdit->setDisplayFormat(QCoreApplication::translate("ChangedDateFilter", "MM/dd/yyyy HH:mm:ss", nullptr));
        lesscheckBox->setText(QCoreApplication::translate("ChangedDateFilter", "Show Items with date <", nullptr));
        lessdateTimeEdit->setDisplayFormat(QCoreApplication::translate("ChangedDateFilter", "MM/dd/yyyy HH:mm:ss", nullptr));
        pushButton->setText(QCoreApplication::translate("ChangedDateFilter", "Apply", nullptr));
        pushButton_2->setText(QCoreApplication::translate("ChangedDateFilter", "Cancel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ChangedDateFilter: public Ui_ChangedDateFilter {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CHANGEDDATEFILTER_H
