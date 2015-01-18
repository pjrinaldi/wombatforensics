/********************************************************************************
** Form generated from reading UI file 'namefilter.ui'
**
** Created by: Qt User Interface Compiler version 5.4.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_NAMEFILTER_H
#define UI_NAMEFILTER_H

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

class Ui_NameFilter
{
public:
    QGridLayout *gridLayout;
    QCheckBox *checkBox;
    QLineEdit *lineEdit;
    QPushButton *pushButton;

    void setupUi(QWidget *NameFilter)
    {
        if (NameFilter->objectName().isEmpty())
            NameFilter->setObjectName(QStringLiteral("NameFilter"));
        NameFilter->setWindowModality(Qt::ApplicationModal);
        NameFilter->resize(346, 75);
        QFont font;
        font.setPointSize(8);
        NameFilter->setFont(font);
        gridLayout = new QGridLayout(NameFilter);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setHorizontalSpacing(0);
        checkBox = new QCheckBox(NameFilter);
        checkBox->setObjectName(QStringLiteral("checkBox"));

        gridLayout->addWidget(checkBox, 0, 0, 1, 1);

        lineEdit = new QLineEdit(NameFilter);
        lineEdit->setObjectName(QStringLiteral("lineEdit"));

        gridLayout->addWidget(lineEdit, 0, 1, 1, 1);

        pushButton = new QPushButton(NameFilter);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setDefault(true);
        pushButton->setFlat(true);

        gridLayout->addWidget(pushButton, 1, 1, 1, 1);


        retranslateUi(NameFilter);

        QMetaObject::connectSlotsByName(NameFilter);
    } // setupUi

    void retranslateUi(QWidget *NameFilter)
    {
        NameFilter->setWindowTitle(QApplication::translate("NameFilter", "Filter", 0));
        checkBox->setText(QApplication::translate("NameFilter", "Show Items where name contains", 0));
        lineEdit->setPlaceholderText(QString());
        pushButton->setText(QApplication::translate("NameFilter", "Apply", 0));
    } // retranslateUi

};

namespace Ui {
    class NameFilter: public Ui_NameFilter {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_NAMEFILTER_H
