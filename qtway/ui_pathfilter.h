/********************************************************************************
** Form generated from reading UI file 'pathfilter.ui'
**
** Created by: Qt User Interface Compiler version 5.15.8
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PATHFILTER_H
#define UI_PATHFILTER_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_PathFilter
{
public:
    QGridLayout *gridLayout;
    QCheckBox *checkBox;
    QLineEdit *lineEdit;
    QPushButton *pushButton;
    QPushButton *pushButton_2;

    void setupUi(QDialog *PathFilter)
    {
        if (PathFilter->objectName().isEmpty())
            PathFilter->setObjectName(QString::fromUtf8("PathFilter"));
        PathFilter->setWindowModality(Qt::ApplicationModal);
        PathFilter->resize(370, 74);
        gridLayout = new QGridLayout(PathFilter);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        checkBox = new QCheckBox(PathFilter);
        checkBox->setObjectName(QString::fromUtf8("checkBox"));

        gridLayout->addWidget(checkBox, 0, 0, 1, 1);

        lineEdit = new QLineEdit(PathFilter);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));
        lineEdit->setEnabled(false);

        gridLayout->addWidget(lineEdit, 0, 1, 1, 1);

        pushButton = new QPushButton(PathFilter);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setAutoFillBackground(false);
        pushButton->setStyleSheet(QString::fromUtf8(""));
        pushButton->setFlat(false);

        gridLayout->addWidget(pushButton, 1, 1, 1, 1);

        pushButton_2 = new QPushButton(PathFilter);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));

        gridLayout->addWidget(pushButton_2, 1, 0, 1, 1);


        retranslateUi(PathFilter);

        pushButton->setDefault(true);


        QMetaObject::connectSlotsByName(PathFilter);
    } // setupUi

    void retranslateUi(QDialog *PathFilter)
    {
        PathFilter->setWindowTitle(QCoreApplication::translate("PathFilter", "Path Filter", nullptr));
        checkBox->setText(QCoreApplication::translate("PathFilter", "Show Items where path contains", nullptr));
        pushButton->setText(QCoreApplication::translate("PathFilter", "Apply", nullptr));
        pushButton_2->setText(QCoreApplication::translate("PathFilter", "Cancel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class PathFilter: public Ui_PathFilter {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PATHFILTER_H
