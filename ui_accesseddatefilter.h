/********************************************************************************
** Form generated from reading UI file 'accesseddatefilter.ui'
**
** Created by: Qt User Interface Compiler version 5.4.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ACCESSEDDATEFILTER_H
#define UI_ACCESSEDDATEFILTER_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDateTimeEdit>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
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
            AccessedDateFilter->setObjectName(QStringLiteral("AccessedDateFilter"));
        AccessedDateFilter->setWindowModality(Qt::ApplicationModal);
        AccessedDateFilter->resize(333, 105);
        QFont font;
        font.setPointSize(8);
        AccessedDateFilter->setFont(font);
        AccessedDateFilter->setAutoFillBackground(true);
        gridLayout = new QGridLayout(AccessedDateFilter);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setHorizontalSpacing(0);
        lesscheckBox = new QCheckBox(AccessedDateFilter);
        lesscheckBox->setObjectName(QStringLiteral("lesscheckBox"));

        gridLayout->addWidget(lesscheckBox, 2, 0, 1, 1);

        lessdateTimeEdit = new QDateTimeEdit(AccessedDateFilter);
        lessdateTimeEdit->setObjectName(QStringLiteral("lessdateTimeEdit"));
        lessdateTimeEdit->setCalendarPopup(true);
        lessdateTimeEdit->setTimeSpec(Qt::UTC);

        gridLayout->addWidget(lessdateTimeEdit, 2, 1, 1, 1);

        moredateTimeEdit = new QDateTimeEdit(AccessedDateFilter);
        moredateTimeEdit->setObjectName(QStringLiteral("moredateTimeEdit"));
        moredateTimeEdit->setCalendarPopup(true);
        moredateTimeEdit->setTimeSpec(Qt::UTC);

        gridLayout->addWidget(moredateTimeEdit, 0, 1, 1, 1);

        morecheckBox = new QCheckBox(AccessedDateFilter);
        morecheckBox->setObjectName(QStringLiteral("morecheckBox"));

        gridLayout->addWidget(morecheckBox, 0, 0, 1, 1);

        pushButton = new QPushButton(AccessedDateFilter);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setDefault(true);
        pushButton->setFlat(true);

        gridLayout->addWidget(pushButton, 3, 1, 1, 1);


        retranslateUi(AccessedDateFilter);
        QObject::connect(lesscheckBox, SIGNAL(toggled(bool)), lessdateTimeEdit, SLOT(setEnabled(bool)));
        QObject::connect(morecheckBox, SIGNAL(toggled(bool)), moredateTimeEdit, SLOT(setEnabled(bool)));

        QMetaObject::connectSlotsByName(AccessedDateFilter);
    } // setupUi

    void retranslateUi(QWidget *AccessedDateFilter)
    {
        AccessedDateFilter->setWindowTitle(QApplication::translate("AccessedDateFilter", "Filter", 0));
        lesscheckBox->setText(QApplication::translate("AccessedDateFilter", "Show Items with date <", 0));
        lessdateTimeEdit->setDisplayFormat(QApplication::translate("AccessedDateFilter", "MM/dd/yyyy HH:mm:ss", 0));
        moredateTimeEdit->setDisplayFormat(QApplication::translate("AccessedDateFilter", "MM/dd/yyyy HH:mm:ss", 0));
        morecheckBox->setText(QApplication::translate("AccessedDateFilter", "Show Items with date >", 0));
        pushButton->setText(QApplication::translate("AccessedDateFilter", "Apply", 0));
    } // retranslateUi

};

namespace Ui {
    class AccessedDateFilter: public Ui_AccessedDateFilter {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ACCESSEDDATEFILTER_H
