/********************************************************************************
** Form generated from reading UI file 'hashfilter.ui'
**
** Created by: Qt User Interface Compiler version 5.4.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_HASHFILTER_H
#define UI_HASHFILTER_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_HashFilter
{
public:
    QGridLayout *gridLayout;
    QCheckBox *checkBox;
    QPushButton *pushButton;

    void setupUi(QWidget *HashFilter)
    {
        if (HashFilter->objectName().isEmpty())
            HashFilter->setObjectName(QStringLiteral("HashFilter"));
        HashFilter->setWindowModality(Qt::ApplicationModal);
        HashFilter->resize(266, 45);
        QFont font;
        font.setPointSize(8);
        HashFilter->setFont(font);
        HashFilter->setAutoFillBackground(true);
        gridLayout = new QGridLayout(HashFilter);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setSizeConstraint(QLayout::SetFixedSize);
        gridLayout->setHorizontalSpacing(0);
        checkBox = new QCheckBox(HashFilter);
        checkBox->setObjectName(QStringLiteral("checkBox"));

        gridLayout->addWidget(checkBox, 0, 0, 1, 1);

        pushButton = new QPushButton(HashFilter);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setDefault(true);
        pushButton->setFlat(true);

        gridLayout->addWidget(pushButton, 0, 1, 1, 1);


        retranslateUi(HashFilter);

        QMetaObject::connectSlotsByName(HashFilter);
    } // setupUi

    void retranslateUi(QWidget *HashFilter)
    {
        HashFilter->setWindowTitle(QApplication::translate("HashFilter", "Filter", 0));
        checkBox->setText(QApplication::translate("HashFilter", "Filter out duplicate items", 0));
        pushButton->setText(QApplication::translate("HashFilter", "Apply", 0));
    } // retranslateUi

};

namespace Ui {
    class HashFilter: public Ui_HashFilter {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_HASHFILTER_H
