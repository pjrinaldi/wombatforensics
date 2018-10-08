/********************************************************************************
** Form generated from reading UI file 'namefilter.ui'
**
** Created by: Qt User Interface Compiler version 5.11.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_NAMEFILTER_H
#define UI_NAMEFILTER_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QGridLayout>
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
        NameFilter->setAutoFillBackground(true);
        gridLayout = new QGridLayout(NameFilter);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setHorizontalSpacing(0);
        checkBox = new QCheckBox(NameFilter);
        checkBox->setObjectName(QStringLiteral("checkBox"));

        gridLayout->addWidget(checkBox, 0, 0, 1, 1);

        lineEdit = new QLineEdit(NameFilter);
        lineEdit->setObjectName(QStringLiteral("lineEdit"));
        lineEdit->setEnabled(false);

        gridLayout->addWidget(lineEdit, 0, 1, 1, 1);

        pushButton = new QPushButton(NameFilter);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setStyleSheet(QStringLiteral("border: 1px solid black; padding: 5px 10px 5px 10px;"));
        pushButton->setFlat(true);

        gridLayout->addWidget(pushButton, 1, 1, 1, 1);


        retranslateUi(NameFilter);
        QObject::connect(checkBox, SIGNAL(toggled(bool)), lineEdit, SLOT(setEnabled(bool)));

        pushButton->setDefault(true);


        QMetaObject::connectSlotsByName(NameFilter);
    } // setupUi

    void retranslateUi(QWidget *NameFilter)
    {
        NameFilter->setWindowTitle(QApplication::translate("NameFilter", "Filter", nullptr));
        checkBox->setText(QApplication::translate("NameFilter", "Show Items where name contains", nullptr));
        lineEdit->setPlaceholderText(QString());
        pushButton->setText(QApplication::translate("NameFilter", "Apply", nullptr));
    } // retranslateUi

};

namespace Ui {
    class NameFilter: public Ui_NameFilter {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_NAMEFILTER_H
