/********************************************************************************
** Form generated from reading UI file 'accesseddatefilter.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ACCESSEDDATEFILTER_H
#define UI_ACCESSEDDATEFILTER_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDateTimeEdit>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_AccessedDateFilter
{
public:
    QGridLayout *gridLayout;
    QCheckBox *lesscheckBox;
    QDateTimeEdit *lessdateTimeEdit;
    QDateTimeEdit *moredateTimeEdit;
    QCheckBox *morecheckBox;
    QPushButton *pushButton;

    void setupUi(QWidget *AccessedDateFilter)
    {
        if (AccessedDateFilter->objectName().isEmpty())
            AccessedDateFilter->setObjectName(QString::fromUtf8("AccessedDateFilter"));
        AccessedDateFilter->setWindowModality(Qt::ApplicationModal);
        AccessedDateFilter->resize(333, 105);
        QFont font;
        font.setPointSize(8);
        AccessedDateFilter->setFont(font);
        AccessedDateFilter->setAutoFillBackground(true);
        gridLayout = new QGridLayout(AccessedDateFilter);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setHorizontalSpacing(0);
        lesscheckBox = new QCheckBox(AccessedDateFilter);
        lesscheckBox->setObjectName(QString::fromUtf8("lesscheckBox"));

        gridLayout->addWidget(lesscheckBox, 2, 0, 1, 1);

        lessdateTimeEdit = new QDateTimeEdit(AccessedDateFilter);
        lessdateTimeEdit->setObjectName(QString::fromUtf8("lessdateTimeEdit"));
        lessdateTimeEdit->setEnabled(false);
        lessdateTimeEdit->setCalendarPopup(true);
        lessdateTimeEdit->setTimeSpec(Qt::UTC);

        gridLayout->addWidget(lessdateTimeEdit, 2, 1, 1, 1);

        moredateTimeEdit = new QDateTimeEdit(AccessedDateFilter);
        moredateTimeEdit->setObjectName(QString::fromUtf8("moredateTimeEdit"));
        moredateTimeEdit->setEnabled(false);
        moredateTimeEdit->setCalendarPopup(true);
        moredateTimeEdit->setTimeSpec(Qt::UTC);

        gridLayout->addWidget(moredateTimeEdit, 0, 1, 1, 1);

        morecheckBox = new QCheckBox(AccessedDateFilter);
        morecheckBox->setObjectName(QString::fromUtf8("morecheckBox"));

        gridLayout->addWidget(morecheckBox, 0, 0, 1, 1);

        pushButton = new QPushButton(AccessedDateFilter);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setStyleSheet(QString::fromUtf8("border: 1px solid black; padding: 5px 10px 5px 10px;"));
        pushButton->setFlat(true);

        gridLayout->addWidget(pushButton, 3, 1, 1, 1);


        retranslateUi(AccessedDateFilter);
        QObject::connect(lesscheckBox, SIGNAL(toggled(bool)), lessdateTimeEdit, SLOT(setEnabled(bool)));
        QObject::connect(morecheckBox, SIGNAL(toggled(bool)), moredateTimeEdit, SLOT(setEnabled(bool)));

        pushButton->setDefault(true);


        QMetaObject::connectSlotsByName(AccessedDateFilter);
    } // setupUi

    void retranslateUi(QWidget *AccessedDateFilter)
    {
        AccessedDateFilter->setWindowTitle(QCoreApplication::translate("AccessedDateFilter", "Filter", nullptr));
        lesscheckBox->setText(QCoreApplication::translate("AccessedDateFilter", "Show Items with date <", nullptr));
        lessdateTimeEdit->setDisplayFormat(QCoreApplication::translate("AccessedDateFilter", "MM/dd/yyyy HH:mm:ss", nullptr));
        moredateTimeEdit->setDisplayFormat(QCoreApplication::translate("AccessedDateFilter", "MM/dd/yyyy HH:mm:ss", nullptr));
        morecheckBox->setText(QCoreApplication::translate("AccessedDateFilter", "Show Items with date >", nullptr));
        pushButton->setText(QCoreApplication::translate("AccessedDateFilter", "Apply", nullptr));
    } // retranslateUi

};

namespace Ui {
    class AccessedDateFilter: public Ui_AccessedDateFilter {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ACCESSEDDATEFILTER_H
