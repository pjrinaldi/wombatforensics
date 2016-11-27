/********************************************************************************
** Form generated from reading UI file 'cancelthread.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CANCELTHREAD_H
#define UI_CANCELTHREAD_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_CancelThread
{
public:
    QHBoxLayout *horizontalLayout;
    QPushButton *pushButton;

    void setupUi(QDialog *CancelThread)
    {
        if (CancelThread->objectName().isEmpty())
            CancelThread->setObjectName(QStringLiteral("CancelThread"));
        CancelThread->resize(266, 47);
        horizontalLayout = new QHBoxLayout(CancelThread);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        pushButton = new QPushButton(CancelThread);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(pushButton->sizePolicy().hasHeightForWidth());
        pushButton->setSizePolicy(sizePolicy);
        pushButton->setMinimumSize(QSize(30, 0));
        pushButton->setMaximumSize(QSize(160, 16777215));
        pushButton->setBaseSize(QSize(30, 0));
        QFont font;
        font.setPointSize(8);
        font.setStrikeOut(false);
        pushButton->setFont(font);
        pushButton->setCursor(QCursor(Qt::ArrowCursor));
        pushButton->setAutoDefault(true);
        pushButton->setFlat(false);

        horizontalLayout->addWidget(pushButton);


        retranslateUi(CancelThread);

        pushButton->setDefault(true);


        QMetaObject::connectSlotsByName(CancelThread);
    } // setupUi

    void retranslateUi(QDialog *CancelThread)
    {
        CancelThread->setWindowTitle(QApplication::translate("CancelThread", "Cancel Operation", 0));
        pushButton->setText(QApplication::translate("CancelThread", "Cancel Current Operation", 0));
        pushButton->setShortcut(QApplication::translate("CancelThread", "Return", 0));
    } // retranslateUi

};

namespace Ui {
    class CancelThread: public Ui_CancelThread {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CANCELTHREAD_H
