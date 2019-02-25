/********************************************************************************
** Form generated from reading UI file 'jumphex.ui'
**
** Created by: Qt User Interface Compiler version 5.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_JUMPHEX_H
#define UI_JUMPHEX_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
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
            JumpHex->setObjectName(QStringLiteral("JumpHex"));
        JumpHex->setWindowModality(Qt::NonModal);
        JumpHex->resize(265, 71);
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(JumpHex->sizePolicy().hasHeightForWidth());
        JumpHex->setSizePolicy(sizePolicy);
        JumpHex->setModal(false);
        horizontalLayoutWidget = new QWidget(JumpHex);
        horizontalLayoutWidget->setObjectName(QStringLiteral("horizontalLayoutWidget"));
        horizontalLayoutWidget->setGeometry(QRect(10, 10, 247, 51));
        horizontalLayout = new QHBoxLayout(horizontalLayoutWidget);
        horizontalLayout->setSpacing(5);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        label = new QLabel(horizontalLayoutWidget);
        label->setObjectName(QStringLiteral("label"));

        horizontalLayout->addWidget(label);

        lineEdit = new QLineEdit(horizontalLayoutWidget);
        lineEdit->setObjectName(QStringLiteral("lineEdit"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(1);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(lineEdit->sizePolicy().hasHeightForWidth());
        lineEdit->setSizePolicy(sizePolicy1);
        lineEdit->setMinimumSize(QSize(110, 0));

        horizontalLayout->addWidget(lineEdit);

        pushButton = new QPushButton(horizontalLayoutWidget);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        QSizePolicy sizePolicy2(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(pushButton->sizePolicy().hasHeightForWidth());
        pushButton->setSizePolicy(sizePolicy2);
        pushButton->setMinimumSize(QSize(40, 0));
        pushButton->setFlat(true);

        horizontalLayout->addWidget(pushButton);

        horizontalSpacer = new QSpacerItem(50, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);


        retranslateUi(JumpHex);

        QMetaObject::connectSlotsByName(JumpHex);
    } // setupUi

    void retranslateUi(QDialog *JumpHex)
    {
        JumpHex->setWindowTitle(QApplication::translate("JumpHex", "Dialog", Q_NULLPTR));
        label->setText(QApplication::translate("JumpHex", "Jump To:", Q_NULLPTR));
        lineEdit->setPlaceholderText(QApplication::translate("JumpHex", "Enter Hex Offset", Q_NULLPTR));
        pushButton->setText(QApplication::translate("JumpHex", "Go", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class JumpHex: public Ui_JumpHex {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_JUMPHEX_H
