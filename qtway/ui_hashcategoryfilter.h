/********************************************************************************
** Form generated from reading UI file 'hashcategoryfilter.ui'
**
** Created by: Qt User Interface Compiler version 5.15.8
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_HASHCATEGORYFILTER_H
#define UI_HASHCATEGORYFILTER_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>

QT_BEGIN_NAMESPACE

class Ui_hashcategoryfilter
{
public:
    QGridLayout *gridLayout;
    QCheckBox *checkBox;
    QComboBox *comboBox;

    void setupUi(QDialog *hashcategoryfilter)
    {
        if (hashcategoryfilter->objectName().isEmpty())
            hashcategoryfilter->setObjectName(QString::fromUtf8("hashcategoryfilter"));
        hashcategoryfilter->setWindowModality(Qt::ApplicationModal);
        hashcategoryfilter->resize(382, 41);
        gridLayout = new QGridLayout(hashcategoryfilter);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        checkBox = new QCheckBox(hashcategoryfilter);
        checkBox->setObjectName(QString::fromUtf8("checkBox"));

        gridLayout->addWidget(checkBox, 0, 0, 1, 1);

        comboBox = new QComboBox(hashcategoryfilter);
        comboBox->setObjectName(QString::fromUtf8("comboBox"));
        comboBox->setEditable(false);

        gridLayout->addWidget(comboBox, 0, 1, 1, 1);


        retranslateUi(hashcategoryfilter);

        QMetaObject::connectSlotsByName(hashcategoryfilter);
    } // setupUi

    void retranslateUi(QDialog *hashcategoryfilter)
    {
        hashcategoryfilter->setWindowTitle(QCoreApplication::translate("hashcategoryfilter", "Hash Category Filter", nullptr));
        checkBox->setText(QCoreApplication::translate("hashcategoryfilter", "Show Items where hash matches", nullptr));
        comboBox->setCurrentText(QString());
    } // retranslateUi

};

namespace Ui {
    class hashcategoryfilter: public Ui_hashcategoryfilter {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_HASHCATEGORYFILTER_H
