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
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_accessdatefilter
{
public:
    QGridLayout *gridLayout;
    QCheckBox *morecheckBox;
    QCheckBox *lesscheckBox;
    QDateTimeEdit *moredateTimeEdit;
    QDateTimeEdit *lessdateTimeEdit;

    void setupUi(QWidget *accessdatefilter)
    {
        if (accessdatefilter->objectName().isEmpty())
            accessdatefilter->setObjectName(QStringLiteral("accessdatefilter"));
        accessdatefilter->setWindowModality(Qt::ApplicationModal);
        accessdatefilter->resize(311, 72);
        QFont font;
        font.setPointSize(8);
        accessdatefilter->setFont(font);
        gridLayout = new QGridLayout(accessdatefilter);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setHorizontalSpacing(0);
        morecheckBox = new QCheckBox(accessdatefilter);
        morecheckBox->setObjectName(QStringLiteral("morecheckBox"));

        gridLayout->addWidget(morecheckBox, 0, 0, 1, 1);

        lesscheckBox = new QCheckBox(accessdatefilter);
        lesscheckBox->setObjectName(QStringLiteral("lesscheckBox"));

        gridLayout->addWidget(lesscheckBox, 2, 0, 1, 1);

        moredateTimeEdit = new QDateTimeEdit(accessdatefilter);
        moredateTimeEdit->setObjectName(QStringLiteral("moredateTimeEdit"));

        gridLayout->addWidget(moredateTimeEdit, 0, 1, 1, 1);

        lessdateTimeEdit = new QDateTimeEdit(accessdatefilter);
        lessdateTimeEdit->setObjectName(QStringLiteral("lessdateTimeEdit"));

        gridLayout->addWidget(lessdateTimeEdit, 2, 1, 1, 1);


        retranslateUi(accessdatefilter);
        QObject::connect(lesscheckBox, SIGNAL(toggled(bool)), lessdateTimeEdit, SLOT(setEnabled(bool)));
        QObject::connect(morecheckBox, SIGNAL(toggled(bool)), moredateTimeEdit, SLOT(setEnabled(bool)));

        QMetaObject::connectSlotsByName(accessdatefilter);
    } // setupUi

    void retranslateUi(QWidget *accessdatefilter)
    {
        accessdatefilter->setWindowTitle(QApplication::translate("accessdatefilter", "Filter", 0));
        morecheckBox->setText(QApplication::translate("accessdatefilter", "Show Items with date >", 0));
        lesscheckBox->setText(QApplication::translate("accessdatefilter", "Show Items with date <", 0));
    } // retranslateUi

};

namespace Ui {
    class accessdatefilter: public Ui_accessdatefilter {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ACCESSEDDATEFILTER_H
