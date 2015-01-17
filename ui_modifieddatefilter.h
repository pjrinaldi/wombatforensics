/********************************************************************************
** Form generated from reading UI file 'modifieddatefilter.ui'
**
** Created by: Qt User Interface Compiler version 5.4.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MODIFIEDDATEFILTER_H
#define UI_MODIFIEDDATEFILTER_H

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

class Ui_modifieddatefilter
{
public:
    QGridLayout *gridLayout;
    QCheckBox *morecheckBox;
    QCheckBox *lesscheckBox;
    QDateTimeEdit *moredateTimeEdit;
    QDateTimeEdit *lessdateTimeEdit;

    void setupUi(QWidget *modifieddatefilter)
    {
        if (modifieddatefilter->objectName().isEmpty())
            modifieddatefilter->setObjectName(QStringLiteral("modifieddatefilter"));
        modifieddatefilter->setWindowModality(Qt::ApplicationModal);
        modifieddatefilter->resize(311, 72);
        QFont font;
        font.setPointSize(8);
        modifieddatefilter->setFont(font);
        gridLayout = new QGridLayout(modifieddatefilter);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setHorizontalSpacing(0);
        morecheckBox = new QCheckBox(modifieddatefilter);
        morecheckBox->setObjectName(QStringLiteral("morecheckBox"));

        gridLayout->addWidget(morecheckBox, 0, 0, 1, 1);

        lesscheckBox = new QCheckBox(modifieddatefilter);
        lesscheckBox->setObjectName(QStringLiteral("lesscheckBox"));

        gridLayout->addWidget(lesscheckBox, 2, 0, 1, 1);

        moredateTimeEdit = new QDateTimeEdit(modifieddatefilter);
        moredateTimeEdit->setObjectName(QStringLiteral("moredateTimeEdit"));

        gridLayout->addWidget(moredateTimeEdit, 0, 1, 1, 1);

        lessdateTimeEdit = new QDateTimeEdit(modifieddatefilter);
        lessdateTimeEdit->setObjectName(QStringLiteral("lessdateTimeEdit"));

        gridLayout->addWidget(lessdateTimeEdit, 2, 1, 1, 1);


        retranslateUi(modifieddatefilter);
        QObject::connect(lesscheckBox, SIGNAL(toggled(bool)), lessdateTimeEdit, SLOT(setEnabled(bool)));
        QObject::connect(morecheckBox, SIGNAL(toggled(bool)), moredateTimeEdit, SLOT(setEnabled(bool)));

        QMetaObject::connectSlotsByName(modifieddatefilter);
    } // setupUi

    void retranslateUi(QWidget *modifieddatefilter)
    {
        modifieddatefilter->setWindowTitle(QApplication::translate("modifieddatefilter", "Filter", 0));
        morecheckBox->setText(QApplication::translate("modifieddatefilter", "Show Items with date >", 0));
        lesscheckBox->setText(QApplication::translate("modifieddatefilter", "Show Items with date <", 0));
    } // retranslateUi

};

namespace Ui {
    class modifieddatefilter: public Ui_modifieddatefilter {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MODIFIEDDATEFILTER_H
