/********************************************************************************
** Form generated from reading UI file 'sizefilter.ui'
**
** Created by: Qt User Interface Compiler version 5.4.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SIZEFILTER_H
#define UI_SIZEFILTER_H

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

class Ui_sizefilter
{
public:
    QGridLayout *gridLayout;
    QCheckBox *morecheckBox;
    QCheckBox *lesscheckBox;
    QSpinBox *morespinBox;
    QSpinBox *lessspinBox;

    void setupUi(QWidget *sizefilter)
    {
        if (sizefilter->objectName().isEmpty())
            sizefilter->setObjectName(QStringLiteral("sizefilter"));
        sizefilter->setWindowModality(Qt::ApplicationModal);
        sizefilter->resize(221, 72);
        QFont font;
        font.setPointSize(8);
        sizefilter->setFont(font);
        gridLayout = new QGridLayout(sizefilter);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setHorizontalSpacing(0);
        morecheckBox = new QCheckBox(sizefilter);
        morecheckBox->setObjectName(QStringLiteral("morecheckBox"));

        gridLayout->addWidget(morecheckBox, 0, 0, 1, 1);

        lesscheckBox = new QCheckBox(sizefilter);
        lesscheckBox->setObjectName(QStringLiteral("lesscheckBox"));

        gridLayout->addWidget(lesscheckBox, 2, 0, 1, 1);

        morespinBox = new QSpinBox(sizefilter);
        morespinBox->setObjectName(QStringLiteral("morespinBox"));

        gridLayout->addWidget(morespinBox, 0, 1, 1, 1);

        lessspinBox = new QSpinBox(sizefilter);
        lessspinBox->setObjectName(QStringLiteral("lessspinBox"));

        gridLayout->addWidget(lessspinBox, 2, 1, 1, 1);


        retranslateUi(sizefilter);
        QObject::connect(lesscheckBox, SIGNAL(toggled(bool)), lessspinBox, SLOT(setEnabled(bool)));
        QObject::connect(morecheckBox, SIGNAL(toggled(bool)), morespinBox, SLOT(setEnabled(bool)));

        QMetaObject::connectSlotsByName(sizefilter);
    } // setupUi

    void retranslateUi(QWidget *sizefilter)
    {
        sizefilter->setWindowTitle(QApplication::translate("sizefilter", "Filter", 0));
        morecheckBox->setText(QApplication::translate("sizefilter", "Show Items with size >", 0));
        lesscheckBox->setText(QApplication::translate("sizefilter", "Show Items with size < ", 0));
    } // retranslateUi

};

namespace Ui {
    class sizefilter: public Ui_sizefilter {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SIZEFILTER_H
