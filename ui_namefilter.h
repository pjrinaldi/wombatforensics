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
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_namefilter
{
public:
    QGridLayout *gridLayout;
    QCheckBox *checkBox;
    QLineEdit *lineEdit;

    void setupUi(QWidget *namefilter)
    {
        if (namefilter->objectName().isEmpty())
            namefilter->setObjectName(QStringLiteral("namefilter"));
        namefilter->setWindowModality(Qt::ApplicationModal);
        namefilter->resize(346, 42);
        QFont font;
        font.setPointSize(8);
        namefilter->setFont(font);
        gridLayout = new QGridLayout(namefilter);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setHorizontalSpacing(0);
        checkBox = new QCheckBox(namefilter);
        checkBox->setObjectName(QStringLiteral("checkBox"));

        gridLayout->addWidget(checkBox, 0, 0, 1, 1);

        lineEdit = new QLineEdit(namefilter);
        lineEdit->setObjectName(QStringLiteral("lineEdit"));

        gridLayout->addWidget(lineEdit, 0, 1, 1, 1);


        retranslateUi(namefilter);
        QObject::connect(checkBox, SIGNAL(toggled(bool)), lineEdit, SLOT(setEnabled(bool)));

        QMetaObject::connectSlotsByName(namefilter);
    } // setupUi

    void retranslateUi(QWidget *namefilter)
    {
        namefilter->setWindowTitle(QApplication::translate("namefilter", "Filter", 0));
        checkBox->setText(QApplication::translate("namefilter", "Show Items where name contains", 0));
    } // retranslateUi

};

namespace Ui {
    class namefilter: public Ui_namefilter {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_NAMEFILTER_H
