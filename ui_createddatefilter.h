/********************************************************************************
** Form generated from reading UI file 'createddatefilter.ui'
**
** Created by: Qt User Interface Compiler version 5.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CREATEDDATEFILTER_H
#define UI_CREATEDDATEFILTER_H

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

class Ui_CreatedDateFilter
{
public:
    QGridLayout *gridLayout;
    QCheckBox *morecheckBox;
    QCheckBox *lesscheckBox;
    QDateTimeEdit *moredateTimeEdit;
    QDateTimeEdit *lessdateTimeEdit;
    QPushButton *pushButton;

    void setupUi(QWidget *CreatedDateFilter)
    {
        if (CreatedDateFilter->objectName().isEmpty())
            CreatedDateFilter->setObjectName(QStringLiteral("CreatedDateFilter"));
        CreatedDateFilter->setWindowModality(Qt::ApplicationModal);
        CreatedDateFilter->resize(346, 105);
        QFont font;
        font.setPointSize(8);
        CreatedDateFilter->setFont(font);
        CreatedDateFilter->setAutoFillBackground(true);
        gridLayout = new QGridLayout(CreatedDateFilter);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setSizeConstraint(QLayout::SetFixedSize);
        gridLayout->setHorizontalSpacing(0);
        morecheckBox = new QCheckBox(CreatedDateFilter);
        morecheckBox->setObjectName(QStringLiteral("morecheckBox"));

        gridLayout->addWidget(morecheckBox, 0, 0, 1, 1);

        lesscheckBox = new QCheckBox(CreatedDateFilter);
        lesscheckBox->setObjectName(QStringLiteral("lesscheckBox"));

        gridLayout->addWidget(lesscheckBox, 2, 0, 1, 1);

        moredateTimeEdit = new QDateTimeEdit(CreatedDateFilter);
        moredateTimeEdit->setObjectName(QStringLiteral("moredateTimeEdit"));
        moredateTimeEdit->setEnabled(false);
        moredateTimeEdit->setCalendarPopup(true);
        moredateTimeEdit->setTimeSpec(Qt::UTC);

        gridLayout->addWidget(moredateTimeEdit, 0, 1, 1, 1);

        lessdateTimeEdit = new QDateTimeEdit(CreatedDateFilter);
        lessdateTimeEdit->setObjectName(QStringLiteral("lessdateTimeEdit"));
        lessdateTimeEdit->setEnabled(false);
        lessdateTimeEdit->setCalendarPopup(true);
        lessdateTimeEdit->setTimeSpec(Qt::UTC);

        gridLayout->addWidget(lessdateTimeEdit, 2, 1, 1, 1);

        pushButton = new QPushButton(CreatedDateFilter);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setStyleSheet(QStringLiteral("border: 1px solid black; padding: 5px 10px 5px 10px;"));
        pushButton->setFlat(true);

        gridLayout->addWidget(pushButton, 3, 1, 1, 1);


        retranslateUi(CreatedDateFilter);
        QObject::connect(morecheckBox, SIGNAL(toggled(bool)), moredateTimeEdit, SLOT(setEnabled(bool)));
        QObject::connect(lesscheckBox, SIGNAL(toggled(bool)), lessdateTimeEdit, SLOT(setEnabled(bool)));

        pushButton->setDefault(true);


        QMetaObject::connectSlotsByName(CreatedDateFilter);
    } // setupUi

    void retranslateUi(QWidget *CreatedDateFilter)
    {
        CreatedDateFilter->setWindowTitle(QApplication::translate("CreatedDateFilter", "Filter", Q_NULLPTR));
        morecheckBox->setText(QApplication::translate("CreatedDateFilter", "Show Items with date >", Q_NULLPTR));
        lesscheckBox->setText(QApplication::translate("CreatedDateFilter", "Show Items with date <", Q_NULLPTR));
        moredateTimeEdit->setDisplayFormat(QApplication::translate("CreatedDateFilter", "MM/dd/yyyy HH:mm:ss", Q_NULLPTR));
        lessdateTimeEdit->setDisplayFormat(QApplication::translate("CreatedDateFilter", "MM/dd/yyyy HH:mm:ss", Q_NULLPTR));
        pushButton->setText(QApplication::translate("CreatedDateFilter", "Apply", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class CreatedDateFilter: public Ui_CreatedDateFilter {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CREATEDDATEFILTER_H
