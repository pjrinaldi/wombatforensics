/********************************************************************************
** Form generated from reading UI file 'idfilter.ui'
**
** Created by: Qt User Interface Compiler version 5.4.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_IDFILTER_H
#define UI_IDFILTER_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_idfilter
{
public:
    QGridLayout *gridLayout;
    QCheckBox *morecheckBox;
    QCheckBox *lesscheckBox;
    QSpinBox *morespinBox;
    QSpinBox *lessspinBox;

    void setupUi(QWidget *idfilter)
    {
        if (idfilter->objectName().isEmpty())
            idfilter->setObjectName(QStringLiteral("idfilter"));
        idfilter->setWindowModality(Qt::ApplicationModal);
        idfilter->resize(159, 72);
        QFont font;
        font.setPointSize(8);
        idfilter->setFont(font);
        gridLayout = new QGridLayout(idfilter);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setHorizontalSpacing(0);
        morecheckBox = new QCheckBox(idfilter);
        morecheckBox->setObjectName(QStringLiteral("morecheckBox"));

        gridLayout->addWidget(morecheckBox, 0, 0, 1, 1);

        lesscheckBox = new QCheckBox(idfilter);
        lesscheckBox->setObjectName(QStringLiteral("lesscheckBox"));

        gridLayout->addWidget(lesscheckBox, 2, 0, 1, 1);

        morespinBox = new QSpinBox(idfilter);
        morespinBox->setObjectName(QStringLiteral("morespinBox"));

        gridLayout->addWidget(morespinBox, 0, 1, 1, 1);

        lessspinBox = new QSpinBox(idfilter);
        lessspinBox->setObjectName(QStringLiteral("lessspinBox"));

        gridLayout->addWidget(lessspinBox, 2, 1, 1, 1);


        retranslateUi(idfilter);
        QObject::connect(lesscheckBox, SIGNAL(toggled(bool)), lessspinBox, SLOT(setEnabled(bool)));
        QObject::connect(morecheckBox, SIGNAL(toggled(bool)), morespinBox, SLOT(setEnabled(bool)));

        QMetaObject::connectSlotsByName(idfilter);
    } // setupUi

    void retranslateUi(QWidget *idfilter)
    {
        idfilter->setWindowTitle(QApplication::translate("idfilter", "Filter", 0));
        morecheckBox->setText(QApplication::translate("idfilter", "Show ID's >", 0));
        lesscheckBox->setText(QApplication::translate("idfilter", "Show ID's < ", 0));
    } // retranslateUi

};

namespace Ui {
    class idfilter: public Ui_idfilter {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_IDFILTER_H
