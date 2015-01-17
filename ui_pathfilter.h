/********************************************************************************
** Form generated from reading UI file 'pathfilter.ui'
**
** Created by: Qt User Interface Compiler version 5.4.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PATHFILTER_H
#define UI_PATHFILTER_H

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

class Ui_idfilter
{
public:
    QGridLayout *gridLayout;
    QCheckBox *checkBox;
    QLineEdit *lineEdit;

    void setupUi(QWidget *idfilter)
    {
        if (idfilter->objectName().isEmpty())
            idfilter->setObjectName(QStringLiteral("idfilter"));
        idfilter->setWindowModality(Qt::ApplicationModal);
        idfilter->resize(339, 42);
        QFont font;
        font.setPointSize(8);
        idfilter->setFont(font);
        gridLayout = new QGridLayout(idfilter);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setHorizontalSpacing(0);
        checkBox = new QCheckBox(idfilter);
        checkBox->setObjectName(QStringLiteral("checkBox"));

        gridLayout->addWidget(checkBox, 0, 0, 1, 1);

        lineEdit = new QLineEdit(idfilter);
        lineEdit->setObjectName(QStringLiteral("lineEdit"));

        gridLayout->addWidget(lineEdit, 0, 1, 1, 1);


        retranslateUi(idfilter);
        QObject::connect(checkBox, SIGNAL(toggled(bool)), lineEdit, SLOT(setEnabled(bool)));

        QMetaObject::connectSlotsByName(idfilter);
    } // setupUi

    void retranslateUi(QWidget *idfilter)
    {
        idfilter->setWindowTitle(QApplication::translate("idfilter", "Filter", 0));
        checkBox->setText(QApplication::translate("idfilter", "Show Items where path contains", 0));
    } // retranslateUi

};

namespace Ui {
    class idfilter: public Ui_idfilter {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PATHFILTER_H
