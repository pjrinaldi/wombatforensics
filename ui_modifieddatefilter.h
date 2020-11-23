/********************************************************************************
** Form generated from reading UI file 'modifieddatefilter.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MODIFIEDDATEFILTER_H
#define UI_MODIFIEDDATEFILTER_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDateTimeEdit>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ModifiedDateFilter
{
public:
    QGridLayout *gridLayout;
    QCheckBox *morecheckBox;
    QCheckBox *lesscheckBox;
    QDateTimeEdit *moredateTimeEdit;
    QDateTimeEdit *lessdateTimeEdit;
    QPushButton *pushButton;

    void setupUi(QWidget *ModifiedDateFilter)
    {
        if (ModifiedDateFilter->objectName().isEmpty())
            ModifiedDateFilter->setObjectName(QString::fromUtf8("ModifiedDateFilter"));
        ModifiedDateFilter->setWindowModality(Qt::ApplicationModal);
        ModifiedDateFilter->resize(346, 105);
        QFont font;
        font.setPointSize(8);
        ModifiedDateFilter->setFont(font);
        ModifiedDateFilter->setAutoFillBackground(true);
        gridLayout = new QGridLayout(ModifiedDateFilter);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setHorizontalSpacing(0);
        morecheckBox = new QCheckBox(ModifiedDateFilter);
        morecheckBox->setObjectName(QString::fromUtf8("morecheckBox"));

        gridLayout->addWidget(morecheckBox, 0, 0, 1, 1);

        lesscheckBox = new QCheckBox(ModifiedDateFilter);
        lesscheckBox->setObjectName(QString::fromUtf8("lesscheckBox"));

        gridLayout->addWidget(lesscheckBox, 2, 0, 1, 1);

        moredateTimeEdit = new QDateTimeEdit(ModifiedDateFilter);
        moredateTimeEdit->setObjectName(QString::fromUtf8("moredateTimeEdit"));
        moredateTimeEdit->setEnabled(false);
        moredateTimeEdit->setCalendarPopup(true);
        moredateTimeEdit->setTimeSpec(Qt::UTC);

        gridLayout->addWidget(moredateTimeEdit, 0, 1, 1, 1);

        lessdateTimeEdit = new QDateTimeEdit(ModifiedDateFilter);
        lessdateTimeEdit->setObjectName(QString::fromUtf8("lessdateTimeEdit"));
        lessdateTimeEdit->setEnabled(false);
        lessdateTimeEdit->setCalendarPopup(true);
        lessdateTimeEdit->setTimeSpec(Qt::UTC);

        gridLayout->addWidget(lessdateTimeEdit, 2, 1, 1, 1);

        pushButton = new QPushButton(ModifiedDateFilter);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setStyleSheet(QString::fromUtf8("border: 1px solid black; padding: 5px 10px 5px 10px;"));
        pushButton->setFlat(true);

        gridLayout->addWidget(pushButton, 3, 1, 1, 1);


        retranslateUi(ModifiedDateFilter);
        QObject::connect(lesscheckBox, SIGNAL(toggled(bool)), lessdateTimeEdit, SLOT(setEnabled(bool)));
        QObject::connect(morecheckBox, SIGNAL(toggled(bool)), moredateTimeEdit, SLOT(setEnabled(bool)));

        pushButton->setDefault(true);


        QMetaObject::connectSlotsByName(ModifiedDateFilter);
    } // setupUi

    void retranslateUi(QWidget *ModifiedDateFilter)
    {
        ModifiedDateFilter->setWindowTitle(QCoreApplication::translate("ModifiedDateFilter", "Filter", nullptr));
        morecheckBox->setText(QCoreApplication::translate("ModifiedDateFilter", "Show Items with date >", nullptr));
        lesscheckBox->setText(QCoreApplication::translate("ModifiedDateFilter", "Show Items with date <", nullptr));
        moredateTimeEdit->setDisplayFormat(QCoreApplication::translate("ModifiedDateFilter", "MM/dd/yyyy HH:mm:ss", nullptr));
        lessdateTimeEdit->setDisplayFormat(QCoreApplication::translate("ModifiedDateFilter", "MM/dd/yyyy HH:mm:ss", nullptr));
        pushButton->setText(QCoreApplication::translate("ModifiedDateFilter", "Apply", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ModifiedDateFilter: public Ui_ModifiedDateFilter {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MODIFIEDDATEFILTER_H
