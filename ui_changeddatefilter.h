/********************************************************************************
** Form generated from reading UI file 'changeddatefilter.ui'
**
** Created by: Qt User Interface Compiler version 5.11.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CHANGEDDATEFILTER_H
#define UI_CHANGEDDATEFILTER_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDateTimeEdit>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ChangedDateFilter
{
public:
    QGridLayout *gridLayout;
    QCheckBox *morecheckBox;
    QDateTimeEdit *lessdateTimeEdit;
    QDateTimeEdit *moredateTimeEdit;
    QCheckBox *lesscheckBox;
    QPushButton *pushButton;

    void setupUi(QWidget *ChangedDateFilter)
    {
        if (ChangedDateFilter->objectName().isEmpty())
            ChangedDateFilter->setObjectName(QStringLiteral("ChangedDateFilter"));
        ChangedDateFilter->setWindowModality(Qt::ApplicationModal);
        ChangedDateFilter->resize(343, 105);
        QFont font;
        font.setPointSize(8);
        ChangedDateFilter->setFont(font);
        ChangedDateFilter->setAutoFillBackground(true);
        gridLayout = new QGridLayout(ChangedDateFilter);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setHorizontalSpacing(0);
        morecheckBox = new QCheckBox(ChangedDateFilter);
        morecheckBox->setObjectName(QStringLiteral("morecheckBox"));

        gridLayout->addWidget(morecheckBox, 0, 0, 1, 1);

        lessdateTimeEdit = new QDateTimeEdit(ChangedDateFilter);
        lessdateTimeEdit->setObjectName(QStringLiteral("lessdateTimeEdit"));
        lessdateTimeEdit->setEnabled(false);
        lessdateTimeEdit->setCalendarPopup(true);
        lessdateTimeEdit->setTimeSpec(Qt::UTC);

        gridLayout->addWidget(lessdateTimeEdit, 2, 1, 1, 1);

        moredateTimeEdit = new QDateTimeEdit(ChangedDateFilter);
        moredateTimeEdit->setObjectName(QStringLiteral("moredateTimeEdit"));
        moredateTimeEdit->setEnabled(false);
        moredateTimeEdit->setCalendarPopup(true);
        moredateTimeEdit->setTimeSpec(Qt::UTC);

        gridLayout->addWidget(moredateTimeEdit, 0, 1, 1, 1);

        lesscheckBox = new QCheckBox(ChangedDateFilter);
        lesscheckBox->setObjectName(QStringLiteral("lesscheckBox"));

        gridLayout->addWidget(lesscheckBox, 2, 0, 1, 1);

        pushButton = new QPushButton(ChangedDateFilter);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setStyleSheet(QStringLiteral("border: 1px solid black; padding: 5px 10px 5px 10px;"));
        pushButton->setFlat(true);

        gridLayout->addWidget(pushButton, 3, 1, 1, 1);


        retranslateUi(ChangedDateFilter);
        QObject::connect(lesscheckBox, SIGNAL(toggled(bool)), lessdateTimeEdit, SLOT(setEnabled(bool)));
        QObject::connect(morecheckBox, SIGNAL(toggled(bool)), moredateTimeEdit, SLOT(setEnabled(bool)));

        pushButton->setDefault(true);


        QMetaObject::connectSlotsByName(ChangedDateFilter);
    } // setupUi

    void retranslateUi(QWidget *ChangedDateFilter)
    {
        ChangedDateFilter->setWindowTitle(QApplication::translate("ChangedDateFilter", "Filter", nullptr));
        morecheckBox->setText(QApplication::translate("ChangedDateFilter", "Show Items with date >", nullptr));
        lessdateTimeEdit->setDisplayFormat(QApplication::translate("ChangedDateFilter", "MM/dd/yyyy HH:mm:ss", nullptr));
        moredateTimeEdit->setDisplayFormat(QApplication::translate("ChangedDateFilter", "MM/dd/yyyy HH:mm:ss", nullptr));
        lesscheckBox->setText(QApplication::translate("ChangedDateFilter", "Show Items with date <", nullptr));
        pushButton->setText(QApplication::translate("ChangedDateFilter", "Apply", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ChangedDateFilter: public Ui_ChangedDateFilter {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CHANGEDDATEFILTER_H
