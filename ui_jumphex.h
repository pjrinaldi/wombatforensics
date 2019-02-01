/********************************************************************************
** Form generated from reading UI file 'jumphex.ui'
**
** Created by: Qt User Interface Compiler version 5.12.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_JUMPHEX_H
#define UI_JUMPHEX_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_JumpHex
{
public:
    QWidget *horizontalLayoutWidget;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QLineEdit *lineEdit;
    QPushButton *pushButton;
    QSpacerItem *horizontalSpacer;

    void setupUi(QDialog *JumpHex)
    {
        if (JumpHex->objectName().isEmpty())
            JumpHex->setObjectName(QString::fromUtf8("JumpHex"));
        JumpHex->setWindowModality(Qt::NonModal);
        JumpHex->resize(258, 72);
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(JumpHex->sizePolicy().hasHeightForWidth());
        JumpHex->setSizePolicy(sizePolicy);
        JumpHex->setModal(false);
        horizontalLayoutWidget = new QWidget(JumpHex);
        horizontalLayoutWidget->setObjectName(QString::fromUtf8("horizontalLayoutWidget"));
        horizontalLayoutWidget->setGeometry(QRect(10, 10, 241, 51));
        horizontalLayout = new QHBoxLayout(horizontalLayoutWidget);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        label = new QLabel(horizontalLayoutWidget);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout->addWidget(label);

        lineEdit = new QLineEdit(horizontalLayoutWidget);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));

        horizontalLayout->addWidget(lineEdit);

        pushButton = new QPushButton(horizontalLayoutWidget);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setFlat(true);

        horizontalLayout->addWidget(pushButton);

        horizontalSpacer = new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);


        retranslateUi(JumpHex);

        QMetaObject::connectSlotsByName(JumpHex);
    } // setupUi

    void retranslateUi(QDialog *JumpHex)
    {
        JumpHex->setWindowTitle(QApplication::translate("JumpHex", "Dialog", nullptr));
        label->setText(QApplication::translate("JumpHex", "Jump To:", nullptr));
        lineEdit->setPlaceholderText(QApplication::translate("JumpHex", "Enter Hex Offset", nullptr));
        pushButton->setText(QApplication::translate("JumpHex", "Go", nullptr));
    } // retranslateUi

};

namespace Ui {
    class JumpHex: public Ui_JumpHex {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_JUMPHEX_H
