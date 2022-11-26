/********************************************************************************
** Form generated from reading UI file 'namefilter.ui'
**
** Created by: Qt User Interface Compiler version 5.15.7
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_NAMEFILTER_H
#define UI_NAMEFILTER_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_NameFilter
{
public:
    QGridLayout *gridLayout;
    QCheckBox *checkBox;
    QLineEdit *lineEdit;
    QPushButton *pushButton;
    QPushButton *pushButton_2;

    void setupUi(QDialog *NameFilter)
    {
        if (NameFilter->objectName().isEmpty())
            NameFilter->setObjectName(QString::fromUtf8("NameFilter"));
        NameFilter->resize(377, 70);
        gridLayout = new QGridLayout(NameFilter);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        checkBox = new QCheckBox(NameFilter);
        checkBox->setObjectName(QString::fromUtf8("checkBox"));

        gridLayout->addWidget(checkBox, 0, 0, 1, 1);

        lineEdit = new QLineEdit(NameFilter);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));
        lineEdit->setEnabled(false);

        gridLayout->addWidget(lineEdit, 0, 1, 1, 1);

        pushButton = new QPushButton(NameFilter);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setStyleSheet(QString::fromUtf8(""));
        pushButton->setFlat(false);

        gridLayout->addWidget(pushButton, 1, 1, 1, 1);

        pushButton_2 = new QPushButton(NameFilter);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));

        gridLayout->addWidget(pushButton_2, 1, 0, 1, 1);


        retranslateUi(NameFilter);

        pushButton->setDefault(true);


        QMetaObject::connectSlotsByName(NameFilter);
    } // setupUi

    void retranslateUi(QDialog *NameFilter)
    {
        NameFilter->setWindowTitle(QCoreApplication::translate("NameFilter", "Name Filter", nullptr));
        checkBox->setText(QCoreApplication::translate("NameFilter", "Show Items where name contains", nullptr));
        lineEdit->setPlaceholderText(QString());
        pushButton->setText(QCoreApplication::translate("NameFilter", "Apply", nullptr));
        pushButton_2->setText(QCoreApplication::translate("NameFilter", "Cancel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class NameFilter: public Ui_NameFilter {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_NAMEFILTER_H
