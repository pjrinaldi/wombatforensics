/********************************************************************************
** Form generated from reading UI file 'addevidencedialog.ui'
**
** Created by: Qt User Interface Compiler version 5.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ADDEVIDENCEDIALOG_H
#define UI_ADDEVIDENCEDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_AddEvidenceDialog
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *addbutton;
    QGroupBox *groupbox;
    QVBoxLayout *verticalLayout_2;
    QListWidget *evidencelist;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *removebutton;
    QPushButton *cancelbutton;
    QPushButton *startbutton;

    void setupUi(QDialog *AddEvidenceDialog)
    {
        if (AddEvidenceDialog->objectName().isEmpty())
            AddEvidenceDialog->setObjectName(QStringLiteral("AddEvidenceDialog"));
        AddEvidenceDialog->setWindowModality(Qt::ApplicationModal);
        AddEvidenceDialog->resize(750, 400);
        AddEvidenceDialog->setSizeGripEnabled(true);
        AddEvidenceDialog->setModal(true);
        verticalLayout = new QVBoxLayout(AddEvidenceDialog);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        addbutton = new QPushButton(AddEvidenceDialog);
        addbutton->setObjectName(QStringLiteral("addbutton"));

        horizontalLayout->addWidget(addbutton);


        verticalLayout->addLayout(horizontalLayout);

        groupbox = new QGroupBox(AddEvidenceDialog);
        groupbox->setObjectName(QStringLiteral("groupbox"));
        groupbox->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        groupbox->setFlat(false);
        verticalLayout_2 = new QVBoxLayout(groupbox);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        evidencelist = new QListWidget(groupbox);
        evidencelist->setObjectName(QStringLiteral("evidencelist"));
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
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);

        removebutton = new QPushButton(AddEvidenceDialog);
        removebutton->setObjectName(QStringLiteral("removebutton"));

        horizontalLayout_2->addWidget(removebutton);

        cancelbutton = new QPushButton(AddEvidenceDialog);
        cancelbutton->setObjectName(QStringLiteral("cancelbutton"));

        horizontalLayout_2->addWidget(cancelbutton);

        startbutton = new QPushButton(AddEvidenceDialog);
        startbutton->setObjectName(QStringLiteral("startbutton"));

        horizontalLayout_2->addWidget(startbutton);


        verticalLayout->addLayout(horizontalLayout_2);


        retranslateUi(AddEvidenceDialog);

        QMetaObject::connectSlotsByName(AddEvidenceDialog);
    } // setupUi

    void retranslateUi(QDialog *AddEvidenceDialog)
    {
        AddEvidenceDialog->setWindowTitle(QApplication::translate("AddEvidenceDialog", "Add Evidence", Q_NULLPTR));
        addbutton->setText(QApplication::translate("AddEvidenceDialog", "Add", Q_NULLPTR));
        groupbox->setTitle(QApplication::translate("AddEvidenceDialog", "Evidence", Q_NULLPTR));
        removebutton->setText(QApplication::translate("AddEvidenceDialog", "Remove", Q_NULLPTR));
        cancelbutton->setText(QApplication::translate("AddEvidenceDialog", "Cancel", Q_NULLPTR));
        startbutton->setText(QApplication::translate("AddEvidenceDialog", "Start", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class AddEvidenceDialog: public Ui_AddEvidenceDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ADDEVIDENCEDIALOG_H
