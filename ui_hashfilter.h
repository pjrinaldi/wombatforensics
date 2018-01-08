/********************************************************************************
** Form generated from reading UI file 'hashfilter.ui'
**
** Created by: Qt User Interface Compiler version 5.9.1
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
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_HashFilter
{
public:
    QGridLayout *gridLayout;
    QCheckBox *checkBox;
    QCheckBox *checkBox_2;
    QLineEdit *lineEdit;
    QPushButton *pushButton;

    void setupUi(QWidget *HashFilter)
    {
        if (HashFilter->objectName().isEmpty())
            HashFilter->setObjectName(QStringLiteral("HashFilter"));
        HashFilter->setWindowModality(Qt::ApplicationModal);
        HashFilter->resize(331, 97);
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

        checkBox_2 = new QCheckBox(HashFilter);
        checkBox_2->setObjectName(QStringLiteral("checkBox_2"));

        gridLayout->addWidget(checkBox_2, 2, 0, 1, 1);

        lineEdit = new QLineEdit(HashFilter);
        lineEdit->setObjectName(QStringLiteral("lineEdit"));
        lineEdit->setEnabled(false);

        gridLayout->addWidget(lineEdit, 2, 1, 1, 1);

        pushButton = new QPushButton(HashFilter);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setStyleSheet(QStringLiteral("border: 1px solid black; padding-top: 5px; padding-bottom: 5px; padding-left: 10px; padding-right: 10px;"));
        pushButton->setFlat(true);

        gridLayout->addWidget(pushButton, 3, 1, 1, 1);


        retranslateUi(HashFilter);
        QObject::connect(checkBox_2, SIGNAL(toggled(bool)), lineEdit, SLOT(setEnabled(bool)));

        pushButton->setDefault(true);


        QMetaObject::connectSlotsByName(HashFilter);
    } // setupUi

    void retranslateUi(QWidget *HashFilter)
    {
        HashFilter->setWindowTitle(QApplication::translate("HashFilter", "Filter", Q_NULLPTR));
        checkBox->setText(QApplication::translate("HashFilter", "Filter out duplicate items", Q_NULLPTR));
        checkBox_2->setText(QApplication::translate("HashFilter", "Filter hashes that contain:", Q_NULLPTR));
        pushButton->setText(QApplication::translate("HashFilter", "Apply", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class HashFilter: public Ui_HashFilter {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_HASHFILTER_H
