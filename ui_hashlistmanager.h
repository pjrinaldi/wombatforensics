/********************************************************************************
** Form generated from reading UI file 'hashlistmanager.ui'
**
** Created by: Qt User Interface Compiler version 5.15.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_HASHLISTMANAGER_H
#define UI_HASHLISTMANAGER_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_HashListManager
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QHBoxLayout *horizontalLayout;
    QListWidget *hashlistwidget;
    QVBoxLayout *verticalLayout_2;
    QPushButton *importbutton;
    QPushButton *createbutton;
    QPushButton *deletebutton;
    QSpacerItem *verticalSpacer;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer;
    QPushButton *closebutton;

    void setupUi(QDialog *HashListManager)
    {
        if (HashListManager->objectName().isEmpty())
            HashListManager->setObjectName(QString::fromUtf8("HashListManager"));
        HashListManager->setWindowModality(Qt::ApplicationModal);
        HashListManager->resize(640, 480);
        verticalLayout = new QVBoxLayout(HashListManager);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        label = new QLabel(HashListManager);
        label->setObjectName(QString::fromUtf8("label"));

        verticalLayout->addWidget(label);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        hashlistwidget = new QListWidget(HashListManager);
        hashlistwidget->setObjectName(QString::fromUtf8("hashlistwidget"));

        horizontalLayout->addWidget(hashlistwidget);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        importbutton = new QPushButton(HashListManager);
        importbutton->setObjectName(QString::fromUtf8("importbutton"));

        verticalLayout_2->addWidget(importbutton);

        createbutton = new QPushButton(HashListManager);
        createbutton->setObjectName(QString::fromUtf8("createbutton"));

        verticalLayout_2->addWidget(createbutton);

        deletebutton = new QPushButton(HashListManager);
        deletebutton->setObjectName(QString::fromUtf8("deletebutton"));

        verticalLayout_2->addWidget(deletebutton);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer);


        horizontalLayout->addLayout(verticalLayout_2);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        closebutton = new QPushButton(HashListManager);
        closebutton->setObjectName(QString::fromUtf8("closebutton"));

        horizontalLayout_2->addWidget(closebutton);


        verticalLayout->addLayout(horizontalLayout_2);


        retranslateUi(HashListManager);

        QMetaObject::connectSlotsByName(HashListManager);
    } // setupUi

    void retranslateUi(QDialog *HashListManager)
    {
        HashListManager->setWindowTitle(QCoreApplication::translate("HashListManager", "Hash List Manager", nullptr));
        label->setText(QCoreApplication::translate("HashListManager", "<u>Hash Lists</u>", nullptr));
        importbutton->setText(QCoreApplication::translate("HashListManager", "Import Hash List", nullptr));
        createbutton->setText(QCoreApplication::translate("HashListManager", "Create Empty List", nullptr));
        deletebutton->setText(QCoreApplication::translate("HashListManager", "Delete Selected", nullptr));
        closebutton->setText(QCoreApplication::translate("HashListManager", "Close", nullptr));
    } // retranslateUi

};

namespace Ui {
    class HashListManager: public Ui_HashListManager {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_HASHLISTMANAGER_H
