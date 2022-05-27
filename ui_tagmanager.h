/********************************************************************************
** Form generated from reading UI file 'tagmanager.ui'
**
** Created by: Qt User Interface Compiler version 5.15.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TAGMANAGER_H
#define UI_TAGMANAGER_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_TagManager
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QListWidget *listWidget;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *newbutton;
    QPushButton *modifybutton;
    QPushButton *removebutton;

    void setupUi(QDialog *TagManager)
    {
        if (TagManager->objectName().isEmpty())
            TagManager->setObjectName(QString::fromUtf8("TagManager"));
        TagManager->resize(323, 265);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/bar/managetags"), QSize(), QIcon::Normal, QIcon::Off);
        TagManager->setWindowIcon(icon);
        TagManager->setModal(true);
        verticalLayout = new QVBoxLayout(TagManager);
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(10, 5, 10, 10);
        label = new QLabel(TagManager);
        label->setObjectName(QString::fromUtf8("label"));
        label->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        label->setIndent(5);

        verticalLayout->addWidget(label);

        listWidget = new QListWidget(TagManager);
        listWidget->setObjectName(QString::fromUtf8("listWidget"));

        verticalLayout->addWidget(listWidget);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        newbutton = new QPushButton(TagManager);
        newbutton->setObjectName(QString::fromUtf8("newbutton"));

        horizontalLayout->addWidget(newbutton);

        modifybutton = new QPushButton(TagManager);
        modifybutton->setObjectName(QString::fromUtf8("modifybutton"));

        horizontalLayout->addWidget(modifybutton);

        removebutton = new QPushButton(TagManager);
        removebutton->setObjectName(QString::fromUtf8("removebutton"));

        horizontalLayout->addWidget(removebutton);


        verticalLayout->addLayout(horizontalLayout);


        retranslateUi(TagManager);

        QMetaObject::connectSlotsByName(TagManager);
    } // setupUi

    void retranslateUi(QDialog *TagManager)
    {
        TagManager->setWindowTitle(QCoreApplication::translate("TagManager", "Tag Manager", nullptr));
        label->setText(QCoreApplication::translate("TagManager", "Case Tags", nullptr));
        newbutton->setText(QCoreApplication::translate("TagManager", "New Tag", nullptr));
        modifybutton->setText(QCoreApplication::translate("TagManager", "Modify Tag", nullptr));
        removebutton->setText(QCoreApplication::translate("TagManager", "Remove Tag", nullptr));
    } // retranslateUi

};

namespace Ui {
    class TagManager: public Ui_TagManager {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TAGMANAGER_H
