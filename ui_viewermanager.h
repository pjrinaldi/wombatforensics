/********************************************************************************
** Form generated from reading UI file 'viewermanager.ui'
**
** Created by: Qt User Interface Compiler version 5.11.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_VIEWERMANAGER_H
#define UI_VIEWERMANAGER_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_ViewerManager
{
public:
    QVBoxLayout *verticalLayout;
    QFrame *frame;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QLineEdit *lineEdit;
    QPushButton *browsebutton;
    QPushButton *addbutton;
    QLabel *label_2;
    QListWidget *listWidget;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer;
    QPushButton *removebutton;

    void setupUi(QDialog *ViewerManager)
    {
        if (ViewerManager->objectName().isEmpty())
            ViewerManager->setObjectName(QStringLiteral("ViewerManager"));
        ViewerManager->setWindowModality(Qt::ApplicationModal);
        ViewerManager->resize(660, 506);
        ViewerManager->setModal(true);
        verticalLayout = new QVBoxLayout(ViewerManager);
        verticalLayout->setSpacing(5);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        frame = new QFrame(ViewerManager);
        frame->setObjectName(QStringLiteral("frame"));
        frame->setMaximumSize(QSize(16777215, 40));
        frame->setFrameShape(QFrame::NoFrame);
        frame->setFrameShadow(QFrame::Plain);
        horizontalLayout = new QHBoxLayout(frame);
        horizontalLayout->setSpacing(5);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(5, 5, 5, 5);
        label = new QLabel(frame);
        label->setObjectName(QStringLiteral("label"));
        label->setFrameShadow(QFrame::Plain);

        horizontalLayout->addWidget(label);

        lineEdit = new QLineEdit(frame);
        lineEdit->setObjectName(QStringLiteral("lineEdit"));
        lineEdit->setReadOnly(true);

        horizontalLayout->addWidget(lineEdit);

        browsebutton = new QPushButton(frame);
        browsebutton->setObjectName(QStringLiteral("browsebutton"));

        horizontalLayout->addWidget(browsebutton);

        addbutton = new QPushButton(frame);
        addbutton->setObjectName(QStringLiteral("addbutton"));
        addbutton->setEnabled(false);

        horizontalLayout->addWidget(addbutton);


        verticalLayout->addWidget(frame);

        label_2 = new QLabel(ViewerManager);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setFrameShape(QFrame::NoFrame);
        label_2->setTextInteractionFlags(Qt::NoTextInteraction);

        verticalLayout->addWidget(label_2);

        listWidget = new QListWidget(ViewerManager);
        listWidget->setObjectName(QStringLiteral("listWidget"));
        listWidget->setAutoFillBackground(false);
        listWidget->setEditTriggers(QAbstractItemView::SelectedClicked);
        listWidget->setProperty("showDropIndicator", QVariant(false));
        listWidget->setAlternatingRowColors(true);

        verticalLayout->addWidget(listWidget);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        removebutton = new QPushButton(ViewerManager);
        removebutton->setObjectName(QStringLiteral("removebutton"));
        removebutton->setEnabled(false);

        horizontalLayout_2->addWidget(removebutton);


        verticalLayout->addLayout(horizontalLayout_2);


        retranslateUi(ViewerManager);

        QMetaObject::connectSlotsByName(ViewerManager);
    } // setupUi

    void retranslateUi(QDialog *ViewerManager)
    {
        ViewerManager->setWindowTitle(QApplication::translate("ViewerManager", "Viewer Manager", nullptr));
        label->setText(QApplication::translate("ViewerManager", "Path To Viewer:", nullptr));
        browsebutton->setText(QApplication::translate("ViewerManager", "Browse", nullptr));
        addbutton->setText(QApplication::translate("ViewerManager", "Add", nullptr));
        label_2->setText(QApplication::translate("ViewerManager", "Existing Viewers", nullptr));
        removebutton->setText(QApplication::translate("ViewerManager", "Remove Selected", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ViewerManager: public Ui_ViewerManager {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VIEWERMANAGER_H
