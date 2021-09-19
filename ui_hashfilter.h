/********************************************************************************
** Form generated from reading UI file 'hashfilter.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_HASHFILTER_H
#define UI_HASHFILTER_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_HashFilter
{
public:
    QGridLayout *gridLayout;
    QCheckBox *checkBox;
    QCheckBox *checkBox_2;
    QLineEdit *lineEdit;
    QPushButton *pushButton;
    QPushButton *pushButton_2;

    void setupUi(QDialog *HashFilter)
    {
        if (HashFilter->objectName().isEmpty())
            HashFilter->setObjectName(QString::fromUtf8("HashFilter"));
        HashFilter->setWindowModality(Qt::ApplicationModal);
        HashFilter->resize(331, 101);
        gridLayout = new QGridLayout(HashFilter);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        checkBox = new QCheckBox(HashFilter);
        checkBox->setObjectName(QString::fromUtf8("checkBox"));

        gridLayout->addWidget(checkBox, 0, 0, 1, 1);

        checkBox_2 = new QCheckBox(HashFilter);
        checkBox_2->setObjectName(QString::fromUtf8("checkBox_2"));

        gridLayout->addWidget(checkBox_2, 1, 0, 1, 1);

        lineEdit = new QLineEdit(HashFilter);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));
        lineEdit->setEnabled(false);

        gridLayout->addWidget(lineEdit, 1, 1, 1, 1);

        pushButton = new QPushButton(HashFilter);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setStyleSheet(QString::fromUtf8(""));
        pushButton->setFlat(false);

        gridLayout->addWidget(pushButton, 2, 1, 1, 1);

        pushButton_2 = new QPushButton(HashFilter);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));

        gridLayout->addWidget(pushButton_2, 2, 0, 1, 1);


        retranslateUi(HashFilter);

        pushButton->setDefault(true);


        QMetaObject::connectSlotsByName(HashFilter);
    } // setupUi

    void retranslateUi(QDialog *HashFilter)
    {
        HashFilter->setWindowTitle(QCoreApplication::translate("HashFilter", "Hash Filter", nullptr));
        checkBox->setText(QCoreApplication::translate("HashFilter", "Filter out duplicate items", nullptr));
        checkBox_2->setText(QCoreApplication::translate("HashFilter", "Filter hashes that contain:", nullptr));
        pushButton->setText(QCoreApplication::translate("HashFilter", "Apply", nullptr));
        pushButton_2->setText(QCoreApplication::translate("HashFilter", "Cancel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class HashFilter: public Ui_HashFilter {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_HASHFILTER_H
