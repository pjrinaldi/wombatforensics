/********************************************************************************
** Form generated from reading UI file 'accesseddatefilter.ui'
**
** Created by: Qt User Interface Compiler version 5.9.1
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
        lessdateTimeEdit->setEnabled(false);
        lessdateTimeEdit->setCalendarPopup(true);
        lessdateTimeEdit->setTimeSpec(Qt::UTC);

        gridLayout->addWidget(lessdateTimeEdit, 2, 1, 1, 1);

        moredateTimeEdit = new QDateTimeEdit(AccessedDateFilter);
        moredateTimeEdit->setObjectName(QStringLiteral("moredateTimeEdit"));
        moredateTimeEdit->setEnabled(false);
        moredateTimeEdit->setCalendarPopup(true);
        moredateTimeEdit->setTimeSpec(Qt::UTC);

        gridLayout->addWidget(moredateTimeEdit, 0, 1, 1, 1);

        morecheckBox = new QCheckBox(AccessedDateFilter);
        morecheckBox->setObjectName(QStringLiteral("morecheckBox"));

        gridLayout->addWidget(morecheckBox, 0, 0, 1, 1);

        pushButton = new QPushButton(AccessedDateFilter);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setStyleSheet(QStringLiteral("border: 1px solid black; padding: 5px 10px 5px 10px;"));
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
        AccessedDateFilter->setWindowTitle(QApplication::translate("AccessedDateFilter", "Filter", Q_NULLPTR));
        lesscheckBox->setText(QApplication::translate("AccessedDateFilter", "Show Items with date <", Q_NULLPTR));
        lessdateTimeEdit->setDisplayFormat(QApplication::translate("AccessedDateFilter", "MM/dd/yyyy HH:mm:ss", Q_NULLPTR));
        moredateTimeEdit->setDisplayFormat(QApplication::translate("AccessedDateFilter", "MM/dd/yyyy HH:mm:ss", Q_NULLPTR));
        morecheckBox->setText(QApplication::translate("AccessedDateFilter", "Show Items with date >", Q_NULLPTR));
        pushButton->setText(QApplication::translate("AccessedDateFilter", "Apply", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class AccessedDateFilter: public Ui_AccessedDateFilter {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ACCESSEDDATEFILTER_H
