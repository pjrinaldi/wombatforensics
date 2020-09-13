/********************************************************************************
** Form generated from reading UI file 'verifyevidencedialog.ui'
**
** Created by: Qt User Interface Compiler version 5.15.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_VERIFYEVIDENCEDIALOG_H
#define UI_VERIFYEVIDENCEDIALOG_H

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

class Ui_VerEvidenceDialog
{
public:
    QVBoxLayout *verticalLayout;
    QGroupBox *groupbox;
    QVBoxLayout *verticalLayout_2;
    QListWidget *evidencelist;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *cancelbutton;
    QPushButton *verifybutton;

    void setupUi(QDialog *VerEvidenceDialog)
    {
        if (VerEvidenceDialog->objectName().isEmpty())
            VerEvidenceDialog->setObjectName(QString::fromUtf8("VerEvidenceDialog"));
        VerEvidenceDialog->setWindowModality(Qt::ApplicationModal);
        VerEvidenceDialog->resize(750, 400);
        VerEvidenceDialog->setAcceptDrops(true);
        VerEvidenceDialog->setSizeGripEnabled(true);
        VerEvidenceDialog->setModal(true);
        verticalLayout = new QVBoxLayout(VerEvidenceDialog);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        groupbox = new QGroupBox(VerEvidenceDialog);
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

        cancelbutton = new QPushButton(VerEvidenceDialog);
        cancelbutton->setObjectName(QString::fromUtf8("cancelbutton"));

        horizontalLayout_2->addWidget(cancelbutton);

        verifybutton = new QPushButton(VerEvidenceDialog);
        verifybutton->setObjectName(QString::fromUtf8("verifybutton"));

        horizontalLayout_2->addWidget(verifybutton);


        verticalLayout->addLayout(horizontalLayout_2);


        retranslateUi(VerEvidenceDialog);

        QMetaObject::connectSlotsByName(VerEvidenceDialog);
    } // setupUi

    void retranslateUi(QDialog *VerEvidenceDialog)
    {
        VerEvidenceDialog->setWindowTitle(QCoreApplication::translate("VerEvidenceDialog", "Verify Evidence", nullptr));
        groupbox->setTitle(QCoreApplication::translate("VerEvidenceDialog", "Evidence", nullptr));
        cancelbutton->setText(QCoreApplication::translate("VerEvidenceDialog", "Cancel", nullptr));
        verifybutton->setText(QCoreApplication::translate("VerEvidenceDialog", "Verify", nullptr));
    } // retranslateUi

};

namespace Ui {
    class VerEvidenceDialog: public Ui_VerEvidenceDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VERIFYEVIDENCEDIALOG_H
