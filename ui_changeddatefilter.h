/********************************************************************************
** Form generated from reading UI file 'changeddatefilter.ui'
**
** Created by: Qt User Interface Compiler version 5.4.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CHANGEDDATEFILTER_H
#define UI_CHANGEDDATEFILTER_H

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

class Ui_changedatefilter
{
public:
    QGridLayout *gridLayout;
    QCheckBox *morecheckBox;
    QCheckBox *lesscheckBox;
    QDateTimeEdit *moredateTimeEdit;
    QDateTimeEdit *lessdateTimeEdit;

    void setupUi(QWidget *changedatefilter)
    {
        if (changedatefilter->objectName().isEmpty())
            changedatefilter->setObjectName(QStringLiteral("changedatefilter"));
        changedatefilter->setWindowModality(Qt::ApplicationModal);
        changedatefilter->resize(311, 72);
        QFont font;
        font.setPointSize(8);
        changedatefilter->setFont(font);
        gridLayout = new QGridLayout(changedatefilter);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setHorizontalSpacing(0);
        morecheckBox = new QCheckBox(changedatefilter);
        morecheckBox->setObjectName(QStringLiteral("morecheckBox"));

        gridLayout->addWidget(morecheckBox, 0, 0, 1, 1);

        lesscheckBox = new QCheckBox(changedatefilter);
        lesscheckBox->setObjectName(QStringLiteral("lesscheckBox"));

        gridLayout->addWidget(lesscheckBox, 2, 0, 1, 1);

        moredateTimeEdit = new QDateTimeEdit(changedatefilter);
        moredateTimeEdit->setObjectName(QStringLiteral("moredateTimeEdit"));

        gridLayout->addWidget(moredateTimeEdit, 0, 1, 1, 1);

        lessdateTimeEdit = new QDateTimeEdit(changedatefilter);
        lessdateTimeEdit->setObjectName(QStringLiteral("lessdateTimeEdit"));

        gridLayout->addWidget(lessdateTimeEdit, 2, 1, 1, 1);


        retranslateUi(changedatefilter);
        QObject::connect(lesscheckBox, SIGNAL(toggled(bool)), lessdateTimeEdit, SLOT(setEnabled(bool)));
        QObject::connect(morecheckBox, SIGNAL(toggled(bool)), moredateTimeEdit, SLOT(setEnabled(bool)));

        QMetaObject::connectSlotsByName(changedatefilter);
    } // setupUi

    void retranslateUi(QWidget *changedatefilter)
    {
        changedatefilter->setWindowTitle(QApplication::translate("changedatefilter", "Filter", 0));
        morecheckBox->setText(QApplication::translate("changedatefilter", "Show Items with date >", 0));
        lesscheckBox->setText(QApplication::translate("changedatefilter", "Show Items with date <", 0));
    } // retranslateUi

};

namespace Ui {
    class changedatefilter: public Ui_changedatefilter {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CHANGEDDATEFILTER_H
