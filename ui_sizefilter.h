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
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SizeFilter
{
public:
    QGridLayout *gridLayout;
    QSpinBox *lessspinBox;
    QCheckBox *morecheckBox;
    QSpinBox *morespinBox;
    QCheckBox *lesscheckBox;
    QPushButton *pushButton;

    void setupUi(QWidget *SizeFilter)
    {
        if (SizeFilter->objectName().isEmpty())
            SizeFilter->setObjectName(QStringLiteral("SizeFilter"));
        SizeFilter->setWindowModality(Qt::ApplicationModal);
        SizeFilter->resize(261, 105);
        QFont font;
        font.setPointSize(8);
        SizeFilter->setFont(font);
        SizeFilter->setAutoFillBackground(true);
        gridLayout = new QGridLayout(SizeFilter);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setHorizontalSpacing(0);
        lessspinBox = new QSpinBox(SizeFilter);
        lessspinBox->setObjectName(QStringLiteral("lessspinBox"));

        gridLayout->addWidget(lessspinBox, 2, 1, 1, 1);

        morecheckBox = new QCheckBox(SizeFilter);
        morecheckBox->setObjectName(QStringLiteral("morecheckBox"));

        gridLayout->addWidget(morecheckBox, 0, 0, 1, 1);

        morespinBox = new QSpinBox(SizeFilter);
        morespinBox->setObjectName(QStringLiteral("morespinBox"));

        gridLayout->addWidget(morespinBox, 0, 1, 1, 1);

        lesscheckBox = new QCheckBox(SizeFilter);
        lesscheckBox->setObjectName(QStringLiteral("lesscheckBox"));

        gridLayout->addWidget(lesscheckBox, 2, 0, 1, 1);

        pushButton = new QPushButton(SizeFilter);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setDefault(true);
        pushButton->setFlat(true);

        gridLayout->addWidget(pushButton, 3, 1, 1, 1);


        retranslateUi(SizeFilter);

        QMetaObject::connectSlotsByName(SizeFilter);
    } // setupUi

    void retranslateUi(QWidget *SizeFilter)
    {
        SizeFilter->setWindowTitle(QApplication::translate("SizeFilter", "Filter", 0));
        morecheckBox->setText(QApplication::translate("SizeFilter", "Show Items with size >", 0));
        lesscheckBox->setText(QApplication::translate("SizeFilter", "Show Items with size < ", 0));
        pushButton->setText(QApplication::translate("SizeFilter", "Apply", 0));
    } // retranslateUi

};

namespace Ui {
    class SizeFilter: public Ui_SizeFilter {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SIZEFILTER_H
