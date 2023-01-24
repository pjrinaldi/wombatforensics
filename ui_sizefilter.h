/********************************************************************************
** Form generated from reading UI file 'sizefilter.ui'
**
** Created by: Qt User Interface Compiler version 5.15.8
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SIZEFILTER_H
#define UI_SIZEFILTER_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>

QT_BEGIN_NAMESPACE

class Ui_SizeFilter
{
public:
    QGridLayout *gridLayout;
    QCheckBox *morecheckBox;
    QSpinBox *morespinBox;
    QCheckBox *lesscheckBox;
    QSpinBox *lessspinBox;
    QPushButton *pushButton;
    QPushButton *pushButton_2;

    void setupUi(QDialog *SizeFilter)
    {
        if (SizeFilter->objectName().isEmpty())
            SizeFilter->setObjectName(QString::fromUtf8("SizeFilter"));
        SizeFilter->setWindowModality(Qt::ApplicationModal);
        SizeFilter->resize(297, 105);
        gridLayout = new QGridLayout(SizeFilter);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        morecheckBox = new QCheckBox(SizeFilter);
        morecheckBox->setObjectName(QString::fromUtf8("morecheckBox"));

        gridLayout->addWidget(morecheckBox, 0, 0, 1, 1);

        morespinBox = new QSpinBox(SizeFilter);
        morespinBox->setObjectName(QString::fromUtf8("morespinBox"));
        morespinBox->setEnabled(false);
        morespinBox->setMaximum(1000000000);

        gridLayout->addWidget(morespinBox, 0, 1, 1, 1);

        lesscheckBox = new QCheckBox(SizeFilter);
        lesscheckBox->setObjectName(QString::fromUtf8("lesscheckBox"));

        gridLayout->addWidget(lesscheckBox, 1, 0, 1, 1);

        lessspinBox = new QSpinBox(SizeFilter);
        lessspinBox->setObjectName(QString::fromUtf8("lessspinBox"));
        lessspinBox->setEnabled(false);
        lessspinBox->setMaximum(1000000000);

        gridLayout->addWidget(lessspinBox, 1, 1, 1, 1);

        pushButton = new QPushButton(SizeFilter);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setStyleSheet(QString::fromUtf8(""));
        pushButton->setFlat(false);

        gridLayout->addWidget(pushButton, 2, 1, 1, 1);

        pushButton_2 = new QPushButton(SizeFilter);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));

        gridLayout->addWidget(pushButton_2, 2, 0, 1, 1);


        retranslateUi(SizeFilter);

        pushButton->setDefault(true);


        QMetaObject::connectSlotsByName(SizeFilter);
    } // setupUi

    void retranslateUi(QDialog *SizeFilter)
    {
        SizeFilter->setWindowTitle(QCoreApplication::translate("SizeFilter", "Size Filter", nullptr));
        morecheckBox->setText(QCoreApplication::translate("SizeFilter", "Show Items with size >", nullptr));
        lesscheckBox->setText(QCoreApplication::translate("SizeFilter", "Show Items with size < ", nullptr));
        pushButton->setText(QCoreApplication::translate("SizeFilter", "Apply", nullptr));
        pushButton_2->setText(QCoreApplication::translate("SizeFilter", "Cancel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SizeFilter: public Ui_SizeFilter {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SIZEFILTER_H
