/********************************************************************************
** Form generated from reading UI file 'filecategoryfilter.ui'
**
** Created by: Qt User Interface Compiler version 5.15.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FILECATEGORYFILTER_H
#define UI_FILECATEGORYFILTER_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_FileCategoryFilter
{
public:
    QGridLayout *gridLayout;
    QCheckBox *categorycheckBox;
    QComboBox *categorycomboBox;
    QPushButton *pushButton;
    QPushButton *pushButton_2;

    void setupUi(QDialog *FileCategoryFilter)
    {
        if (FileCategoryFilter->objectName().isEmpty())
            FileCategoryFilter->setObjectName(QString::fromUtf8("FileCategoryFilter"));
        FileCategoryFilter->setWindowModality(Qt::ApplicationModal);
        FileCategoryFilter->resize(440, 74);
        gridLayout = new QGridLayout(FileCategoryFilter);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        categorycheckBox = new QCheckBox(FileCategoryFilter);
        categorycheckBox->setObjectName(QString::fromUtf8("categorycheckBox"));

        gridLayout->addWidget(categorycheckBox, 0, 0, 1, 1);

        categorycomboBox = new QComboBox(FileCategoryFilter);
        categorycomboBox->setObjectName(QString::fromUtf8("categorycomboBox"));
        categorycomboBox->setEnabled(false);
        categorycomboBox->setEditable(false);
        categorycomboBox->setInsertPolicy(QComboBox::NoInsert);

        gridLayout->addWidget(categorycomboBox, 0, 1, 1, 1);

        pushButton = new QPushButton(FileCategoryFilter);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setStyleSheet(QString::fromUtf8(""));
        pushButton->setFlat(false);

        gridLayout->addWidget(pushButton, 1, 1, 1, 1);

        pushButton_2 = new QPushButton(FileCategoryFilter);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));

        gridLayout->addWidget(pushButton_2, 1, 0, 1, 1);


        retranslateUi(FileCategoryFilter);

        pushButton->setDefault(true);


        QMetaObject::connectSlotsByName(FileCategoryFilter);
    } // setupUi

    void retranslateUi(QDialog *FileCategoryFilter)
    {
        FileCategoryFilter->setWindowTitle(QCoreApplication::translate("FileCategoryFilter", "File Category Filter", nullptr));
        categorycheckBox->setText(QCoreApplication::translate("FileCategoryFilter", "Show Items where category matches", nullptr));
        categorycomboBox->setCurrentText(QString());
        pushButton->setText(QCoreApplication::translate("FileCategoryFilter", "Apply", nullptr));
        pushButton_2->setText(QCoreApplication::translate("FileCategoryFilter", "Cancel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class FileCategoryFilter: public Ui_FileCategoryFilter {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FILECATEGORYFILTER_H
