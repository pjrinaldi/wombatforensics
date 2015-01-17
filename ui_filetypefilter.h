/********************************************************************************
** Form generated from reading UI file 'filetypefilter.ui'
**
** Created by: Qt User Interface Compiler version 5.4.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FILETYPEFILTER_H
#define UI_FILETYPEFILTER_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_idfilter
{
public:
    QGridLayout *gridLayout;
    QCheckBox *categorycheckBox;
    QComboBox *categorycomboBox;
    QCheckBox *typecheckBox;
    QComboBox *typecomboBox;

    void setupUi(QWidget *idfilter)
    {
        if (idfilter->objectName().isEmpty())
            idfilter->setObjectName(QStringLiteral("idfilter"));
        idfilter->setWindowModality(Qt::ApplicationModal);
        idfilter->resize(429, 72);
        QFont font;
        font.setPointSize(8);
        idfilter->setFont(font);
        gridLayout = new QGridLayout(idfilter);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setHorizontalSpacing(0);
        categorycheckBox = new QCheckBox(idfilter);
        categorycheckBox->setObjectName(QStringLiteral("categorycheckBox"));

        gridLayout->addWidget(categorycheckBox, 0, 0, 1, 1);

        categorycomboBox = new QComboBox(idfilter);
        categorycomboBox->setObjectName(QStringLiteral("categorycomboBox"));
        categorycomboBox->setEditable(false);

        gridLayout->addWidget(categorycomboBox, 0, 1, 1, 1);

        typecheckBox = new QCheckBox(idfilter);
        typecheckBox->setObjectName(QStringLiteral("typecheckBox"));

        gridLayout->addWidget(typecheckBox, 1, 0, 1, 1);

        typecomboBox = new QComboBox(idfilter);
        typecomboBox->setObjectName(QStringLiteral("typecomboBox"));

        gridLayout->addWidget(typecomboBox, 1, 1, 1, 1);


        retranslateUi(idfilter);
        QObject::connect(categorycheckBox, SIGNAL(toggled(bool)), categorycomboBox, SLOT(setEnabled(bool)));
        QObject::connect(typecheckBox, SIGNAL(toggled(bool)), typecomboBox, SLOT(setEnabled(bool)));

        QMetaObject::connectSlotsByName(idfilter);
    } // setupUi

    void retranslateUi(QWidget *idfilter)
    {
        idfilter->setWindowTitle(QApplication::translate("idfilter", "Filter", 0));
        categorycheckBox->setText(QApplication::translate("idfilter", "Show Items where category matches", 0));
        categorycomboBox->setCurrentText(QString());
        typecheckBox->setText(QApplication::translate("idfilter", "Show Items where type matches", 0));
    } // retranslateUi

};

namespace Ui {
    class idfilter: public Ui_idfilter {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FILETYPEFILTER_H
