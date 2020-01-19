/********************************************************************************
** Form generated from reading UI file 'remevidencedialog.ui'
**
** Created by: Qt User Interface Compiler version 5.14.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_REMEVIDENCEDIALOG_H
#define UI_REMEVIDENCEDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_RemEvidenceDialog
{
public:
    QVBoxLayout *verticalLayout;
    QGroupBox *groupbox;
    QVBoxLayout *verticalLayout_2;
    QListWidget *evidencelist;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *cancelbutton;
    QPushButton *removebutton;

    void setupUi(QDialog *RemEvidenceDialog)
    {
        if (RemEvidenceDialog->objectName().isEmpty())
            RemEvidenceDialog->setObjectName(QString::fromUtf8("RemEvidenceDialog"));
        RemEvidenceDialog->setWindowModality(Qt::ApplicationModal);
        RemEvidenceDialog->resize(750, 400);
        RemEvidenceDialog->setAcceptDrops(true);
        RemEvidenceDialog->setSizeGripEnabled(true);
        RemEvidenceDialog->setModal(true);
        verticalLayout = new QVBoxLayout(RemEvidenceDialog);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        groupbox = new QGroupBox(RemEvidenceDialog);
        groupbox->setObjectName(QString::fromUtf8("groupbox"));
        groupbox->setAcceptDrops(true);
        groupbox->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        groupbox->setFlat(false);
        verticalLayout_2 = new QVBoxLayout(groupbox);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        evidencelist = new QListWidget(groupbox);
        evidencelist->setObjectName(QString::fromUtf8("evidencelist"));
        evidencelist->setAcceptDrops(true);
        evidencelist->setFrameShape(QFrame::NoFrame);
        evidencelist->setEditTriggers(QAbstractItemView::NoEditTriggers);
        evidencelist->setProperty("showDropIndicator", QVariant(true));
        evidencelist->setDragDropMode(QAbstractItemView::DropOnly);
        evidencelist->setDefaultDropAction(Qt::LinkAction);
        evidencelist->setAlternatingRowColors(true);
        evidencelist->setSelectionMode(QAbstractItemView::ExtendedSelection);
        evidencelist->setUniformItemSizes(true);
        evidencelist->setWordWrap(true);

        verticalLayout_2->addWidget(evidencelist);


        verticalLayout->addWidget(groupbox);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);

        cancelbutton = new QPushButton(RemEvidenceDialog);
        cancelbutton->setObjectName(QString::fromUtf8("cancelbutton"));

        horizontalLayout_2->addWidget(cancelbutton);

        removebutton = new QPushButton(RemEvidenceDialog);
        removebutton->setObjectName(QString::fromUtf8("removebutton"));

        horizontalLayout_2->addWidget(removebutton);


        verticalLayout->addLayout(horizontalLayout_2);


        retranslateUi(RemEvidenceDialog);

        QMetaObject::connectSlotsByName(RemEvidenceDialog);
    } // setupUi

    void retranslateUi(QDialog *RemEvidenceDialog)
    {
        RemEvidenceDialog->setWindowTitle(QCoreApplication::translate("RemEvidenceDialog", "Remove Evidence", nullptr));
        groupbox->setTitle(QCoreApplication::translate("RemEvidenceDialog", "Evidence", nullptr));
        cancelbutton->setText(QCoreApplication::translate("RemEvidenceDialog", "Cancel", nullptr));
        removebutton->setText(QCoreApplication::translate("RemEvidenceDialog", "Remove", nullptr));
    } // retranslateUi

};

namespace Ui {
    class RemEvidenceDialog: public Ui_RemEvidenceDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_REMEVIDENCEDIALOG_H
