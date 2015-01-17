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

class Ui_hashfilter
{
public:
    QGridLayout *gridLayout;
    QCheckBox *checkBox;

    void setupUi(QWidget *hashfilter)
    {
        if (hashfilter->objectName().isEmpty())
            hashfilter->setObjectName(QStringLiteral("hashfilter"));
        hashfilter->setWindowModality(Qt::ApplicationModal);
        hashfilter->resize(181, 36);
        QFont font;
        font.setPointSize(8);
        hashfilter->setFont(font);
        gridLayout = new QGridLayout(hashfilter);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setHorizontalSpacing(0);
        checkBox = new QCheckBox(hashfilter);
        checkBox->setObjectName(QStringLiteral("checkBox"));

        gridLayout->addWidget(checkBox, 0, 0, 1, 1);


        retranslateUi(hashfilter);

        QMetaObject::connectSlotsByName(hashfilter);
    } // setupUi

    void retranslateUi(QWidget *hashfilter)
    {
        hashfilter->setWindowTitle(QApplication::translate("hashfilter", "Filter", 0));
        checkBox->setText(QApplication::translate("hashfilter", "Filter out duplicate items", 0));
    } // retranslateUi

};

namespace Ui {
    class hashfilter: public Ui_hashfilter {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_HASHFILTER_H
