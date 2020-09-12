/********************************************************************************
** Form generated from reading UI file 'pathfilter.ui'
**
** Created by: Qt User Interface Compiler version 5.15.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PATHFILTER_H
#define UI_PATHFILTER_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_PathFilter
{
public:
    QGridLayout *gridLayout;
    QCheckBox *checkBox;
    QLineEdit *lineEdit;
    QPushButton *pushButton;

    void setupUi(QWidget *PathFilter)
    {
        if (PathFilter->objectName().isEmpty())
            PathFilter->setObjectName(QString::fromUtf8("PathFilter"));
        PathFilter->setWindowModality(Qt::ApplicationModal);
        PathFilter->resize(339, 75);
        QFont font;
        font.setPointSize(8);
        PathFilter->setFont(font);
        PathFilter->setAutoFillBackground(true);
        gridLayout = new QGridLayout(PathFilter);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setHorizontalSpacing(0);
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
        pushButton->setStyleSheet(QString::fromUtf8("border: 1px solid black; padding: 5px 10px 5px 10px;"));
        pushButton->setFlat(true);

        gridLayout->addWidget(pushButton, 1, 1, 1, 1);


        retranslateUi(PathFilter);
        QObject::connect(checkBox, SIGNAL(toggled(bool)), lineEdit, SLOT(setEnabled(bool)));

        pushButton->setDefault(true);


        QMetaObject::connectSlotsByName(PathFilter);
    } // setupUi

    void retranslateUi(QWidget *PathFilter)
    {
        PathFilter->setWindowTitle(QCoreApplication::translate("PathFilter", "Filter", nullptr));
        checkBox->setText(QCoreApplication::translate("PathFilter", "Show Items where path contains", nullptr));
        pushButton->setText(QCoreApplication::translate("PathFilter", "Apply", nullptr));
    } // retranslateUi

};

namespace Ui {
    class PathFilter: public Ui_PathFilter {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PATHFILTER_H
