/********************************************************************************
** Form generated from reading UI file 'pathfilter.ui'
**
** Created by: Qt User Interface Compiler version 5.9.1
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
            PathFilter->setObjectName(QStringLiteral("PathFilter"));
        PathFilter->setWindowModality(Qt::ApplicationModal);
        PathFilter->resize(339, 75);
        QFont font;
        font.setPointSize(8);
        PathFilter->setFont(font);
        PathFilter->setAutoFillBackground(true);
        gridLayout = new QGridLayout(PathFilter);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setHorizontalSpacing(0);
        checkBox = new QCheckBox(PathFilter);
        checkBox->setObjectName(QStringLiteral("checkBox"));

        gridLayout->addWidget(checkBox, 0, 0, 1, 1);

        lineEdit = new QLineEdit(PathFilter);
        lineEdit->setObjectName(QStringLiteral("lineEdit"));
        lineEdit->setEnabled(false);

        gridLayout->addWidget(lineEdit, 0, 1, 1, 1);

        pushButton = new QPushButton(PathFilter);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setAutoFillBackground(false);
        pushButton->setFlat(true);

        gridLayout->addWidget(pushButton, 1, 1, 1, 1);


        retranslateUi(PathFilter);
        QObject::connect(checkBox, SIGNAL(toggled(bool)), lineEdit, SLOT(setEnabled(bool)));

        pushButton->setDefault(true);


        QMetaObject::connectSlotsByName(PathFilter);
    } // setupUi

    void retranslateUi(QWidget *PathFilter)
    {
        PathFilter->setWindowTitle(QApplication::translate("PathFilter", "Filter", Q_NULLPTR));
        checkBox->setText(QApplication::translate("PathFilter", "Show Items where path contains", Q_NULLPTR));
        pushButton->setText(QApplication::translate("PathFilter", "Apply", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class PathFilter: public Ui_PathFilter {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PATHFILTER_H
