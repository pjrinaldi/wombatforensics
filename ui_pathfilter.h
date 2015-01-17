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

class Ui_pathfilter
{
public:
    QGridLayout *gridLayout;
    QCheckBox *checkBox;
    QLineEdit *lineEdit;

    void setupUi(QWidget *pathfilter)
    {
        if (pathfilter->objectName().isEmpty())
            pathfilter->setObjectName(QStringLiteral("pathfilter"));
        pathfilter->setWindowModality(Qt::ApplicationModal);
        pathfilter->resize(339, 42);
        QFont font;
        font.setPointSize(8);
        pathfilter->setFont(font);
        gridLayout = new QGridLayout(pathfilter);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setHorizontalSpacing(0);
        checkBox = new QCheckBox(pathfilter);
        checkBox->setObjectName(QStringLiteral("checkBox"));

        gridLayout->addWidget(checkBox, 0, 0, 1, 1);

        lineEdit = new QLineEdit(pathfilter);
        lineEdit->setObjectName(QStringLiteral("lineEdit"));

        gridLayout->addWidget(lineEdit, 0, 1, 1, 1);


        retranslateUi(pathfilter);
        QObject::connect(checkBox, SIGNAL(toggled(bool)), lineEdit, SLOT(setEnabled(bool)));

        QMetaObject::connectSlotsByName(pathfilter);
    } // setupUi

    void retranslateUi(QWidget *pathfilter)
    {
        pathfilter->setWindowTitle(QApplication::translate("pathfilter", "Filter", 0));
        checkBox->setText(QApplication::translate("pathfilter", "Show Items where path contains", 0));
    } // retranslateUi

};

namespace Ui {
    class pathfilter: public Ui_pathfilter {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PATHFILTER_H
