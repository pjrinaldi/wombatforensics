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

class Ui_filetypefilter
{
public:
    QGridLayout *gridLayout;
    QCheckBox *categorycheckBox;
    QComboBox *categorycomboBox;
    QCheckBox *typecheckBox;
    QComboBox *typecomboBox;

    void setupUi(QWidget *filetypefilter)
    {
        if (filetypefilter->objectName().isEmpty())
            filetypefilter->setObjectName(QStringLiteral("filetypefilter"));
        filetypefilter->setWindowModality(Qt::ApplicationModal);
        filetypefilter->resize(429, 72);
        QFont font;
        font.setPointSize(8);
        filetypefilter->setFont(font);
        gridLayout = new QGridLayout(filetypefilter);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setHorizontalSpacing(0);
        categorycheckBox = new QCheckBox(filetypefilter);
        categorycheckBox->setObjectName(QStringLiteral("categorycheckBox"));

        gridLayout->addWidget(categorycheckBox, 0, 0, 1, 1);

        categorycomboBox = new QComboBox(filetypefilter);
        categorycomboBox->setObjectName(QStringLiteral("categorycomboBox"));
        categorycomboBox->setEditable(false);

        gridLayout->addWidget(categorycomboBox, 0, 1, 1, 1);

        typecheckBox = new QCheckBox(filetypefilter);
        typecheckBox->setObjectName(QStringLiteral("typecheckBox"));

        gridLayout->addWidget(typecheckBox, 1, 0, 1, 1);

        typecomboBox = new QComboBox(filetypefilter);
        typecomboBox->setObjectName(QStringLiteral("typecomboBox"));

        gridLayout->addWidget(typecomboBox, 1, 1, 1, 1);


        retranslateUi(filetypefilter);
        QObject::connect(categorycheckBox, SIGNAL(toggled(bool)), categorycomboBox, SLOT(setEnabled(bool)));
        QObject::connect(typecheckBox, SIGNAL(toggled(bool)), typecomboBox, SLOT(setEnabled(bool)));

        QMetaObject::connectSlotsByName(filetypefilter);
    } // setupUi

    void retranslateUi(QWidget *filetypefilter)
    {
        filetypefilter->setWindowTitle(QApplication::translate("filetypefilter", "Filter", 0));
        categorycheckBox->setText(QApplication::translate("filetypefilter", "Show Items where category matches", 0));
        categorycomboBox->setCurrentText(QString());
        typecheckBox->setText(QApplication::translate("filetypefilter", "Show Items where type matches", 0));
    } // retranslateUi

};

namespace Ui {
    class filetypefilter: public Ui_filetypefilter {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FILETYPEFILTER_H
