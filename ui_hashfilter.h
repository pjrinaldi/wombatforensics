/********************************************************************************
** Form generated from reading UI file 'hashfilter.ui'
**
** Created by: Qt User Interface Compiler version 5.4.0
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
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_idfilter
{
public:
    QGridLayout *gridLayout;
    QCheckBox *checkBox;

    void setupUi(QWidget *idfilter)
    {
        if (idfilter->objectName().isEmpty())
            idfilter->setObjectName(QStringLiteral("idfilter"));
        idfilter->setWindowModality(Qt::ApplicationModal);
        idfilter->resize(181, 36);
        QFont font;
        font.setPointSize(8);
        idfilter->setFont(font);
        gridLayout = new QGridLayout(idfilter);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setHorizontalSpacing(0);
        checkBox = new QCheckBox(idfilter);
        checkBox->setObjectName(QStringLiteral("checkBox"));

        gridLayout->addWidget(checkBox, 0, 0, 1, 1);


        retranslateUi(idfilter);

        QMetaObject::connectSlotsByName(idfilter);
    } // setupUi

    void retranslateUi(QWidget *idfilter)
    {
        idfilter->setWindowTitle(QApplication::translate("idfilter", "Filter", 0));
        checkBox->setText(QApplication::translate("idfilter", "Filter out duplicate items", 0));
    } // retranslateUi

};

namespace Ui {
    class idfilter: public Ui_idfilter {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_HASHFILTER_H
