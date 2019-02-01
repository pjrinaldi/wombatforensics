/********************************************************************************
** Form generated from reading UI file 'filecategoryfilter.ui'
**
** Created by: Qt User Interface Compiler version 5.12.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FILECATEGORYFILTER_H
#define UI_FILECATEGORYFILTER_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_FileCategoryFilter
{
public:
    QGridLayout *gridLayout;
    QCheckBox *categorycheckBox;
    QPushButton *pushButton;
    QComboBox *categorycomboBox;

    void setupUi(QWidget *FileCategoryFilter)
    {
        if (FileCategoryFilter->objectName().isEmpty())
            FileCategoryFilter->setObjectName(QString::fromUtf8("FileCategoryFilter"));
        FileCategoryFilter->setWindowModality(Qt::ApplicationModal);
        FileCategoryFilter->resize(429, 79);
        QFont font;
        font.setPointSize(8);
        FileCategoryFilter->setFont(font);
        FileCategoryFilter->setAutoFillBackground(true);
        gridLayout = new QGridLayout(FileCategoryFilter);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setHorizontalSpacing(0);
        categorycheckBox = new QCheckBox(FileCategoryFilter);
        categorycheckBox->setObjectName(QString::fromUtf8("categorycheckBox"));

        gridLayout->addWidget(categorycheckBox, 0, 0, 1, 1);

        pushButton = new QPushButton(FileCategoryFilter);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setStyleSheet(QString::fromUtf8("border: 1px solid black; padding: 5px 10px 5px 10px;"));
        pushButton->setFlat(true);

        gridLayout->addWidget(pushButton, 1, 1, 1, 1);

        categorycomboBox = new QComboBox(FileCategoryFilter);
        categorycomboBox->setObjectName(QString::fromUtf8("categorycomboBox"));
        categorycomboBox->setEnabled(false);
        categorycomboBox->setEditable(false);
        categorycomboBox->setInsertPolicy(QComboBox::NoInsert);

        gridLayout->addWidget(categorycomboBox, 0, 1, 1, 1);


        retranslateUi(FileCategoryFilter);
        QObject::connect(categorycheckBox, SIGNAL(toggled(bool)), categorycomboBox, SLOT(setEnabled(bool)));

        pushButton->setDefault(true);


        QMetaObject::connectSlotsByName(FileCategoryFilter);
    } // setupUi

    void retranslateUi(QWidget *FileCategoryFilter)
    {
        FileCategoryFilter->setWindowTitle(QApplication::translate("FileCategoryFilter", "Filter", nullptr));
        categorycheckBox->setText(QApplication::translate("FileCategoryFilter", "Show Items where category matches", nullptr));
        pushButton->setText(QApplication::translate("FileCategoryFilter", "Apply", nullptr));
        categorycomboBox->setCurrentText(QString());
    } // retranslateUi

};

namespace Ui {
    class FileCategoryFilter: public Ui_FileCategoryFilter {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FILECATEGORYFILTER_H
