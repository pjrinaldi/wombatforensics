/********************************************************************************
** Form generated from reading UI file 'sizefilter.ui'
**
** Created by: Qt User Interface Compiler version 5.13.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SIZEFILTER_H
#define UI_SIZEFILTER_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QGridLayout>
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
            SizeFilter->setObjectName(QString::fromUtf8("SizeFilter"));
        SizeFilter->setWindowModality(Qt::ApplicationModal);
        SizeFilter->resize(276, 105);
        QFont font;
        font.setPointSize(8);
        SizeFilter->setFont(font);
        SizeFilter->setAutoFillBackground(true);
        gridLayout = new QGridLayout(SizeFilter);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setHorizontalSpacing(0);
        lessspinBox = new QSpinBox(SizeFilter);
        lessspinBox->setObjectName(QString::fromUtf8("lessspinBox"));
        lessspinBox->setEnabled(false);
        lessspinBox->setMaximum(1000000000);

        gridLayout->addWidget(lessspinBox, 2, 1, 1, 1);

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

        gridLayout->addWidget(lesscheckBox, 2, 0, 1, 1);

        pushButton = new QPushButton(SizeFilter);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setStyleSheet(QString::fromUtf8("border: 1px solid black; padding: 5px 10px 5px 10px;"));
        pushButton->setFlat(true);

        gridLayout->addWidget(pushButton, 3, 1, 1, 1);


        retranslateUi(SizeFilter);
        QObject::connect(morecheckBox, SIGNAL(toggled(bool)), morespinBox, SLOT(setEnabled(bool)));
        QObject::connect(lesscheckBox, SIGNAL(toggled(bool)), lessspinBox, SLOT(setEnabled(bool)));

        pushButton->setDefault(true);


        QMetaObject::connectSlotsByName(SizeFilter);
    } // setupUi

    void retranslateUi(QWidget *SizeFilter)
    {
        SizeFilter->setWindowTitle(QCoreApplication::translate("SizeFilter", "Filter", nullptr));
        morecheckBox->setText(QCoreApplication::translate("SizeFilter", "Show Items with size >", nullptr));
        lesscheckBox->setText(QCoreApplication::translate("SizeFilter", "Show Items with size < ", nullptr));
        pushButton->setText(QCoreApplication::translate("SizeFilter", "Apply", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SizeFilter: public Ui_SizeFilter {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SIZEFILTER_H
