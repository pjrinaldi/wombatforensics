/********************************************************************************
** Form generated from reading UI file 'mboxviewer.ui'
**
** Created by: Qt User Interface Compiler version 5.15.8
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MBOXVIEWER_H
#define UI_MBOXVIEWER_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_MBoxDialog
{
public:
    QVBoxLayout *verticalLayout;
    QToolButton *toolButton;
    QSplitter *splitter;
    QTableWidget *mailtable;
    QPlainTextEdit *mailcontent;
    QLabel *label;

    void setupUi(QDialog *MBoxDialog)
    {
        if (MBoxDialog->objectName().isEmpty())
            MBoxDialog->setObjectName(QString::fromUtf8("MBoxDialog"));
        MBoxDialog->setWindowModality(Qt::NonModal);
        MBoxDialog->resize(904, 684);
        MBoxDialog->setSizeGripEnabled(true);
        MBoxDialog->setModal(false);
        verticalLayout = new QVBoxLayout(MBoxDialog);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        toolButton = new QToolButton(MBoxDialog);
        toolButton->setObjectName(QString::fromUtf8("toolButton"));

        verticalLayout->addWidget(toolButton);

        splitter = new QSplitter(MBoxDialog);
        splitter->setObjectName(QString::fromUtf8("splitter"));
        splitter->setOrientation(Qt::Vertical);
        splitter->setOpaqueResize(true);
        mailtable = new QTableWidget(splitter);
        if (mailtable->columnCount() < 5)
            mailtable->setColumnCount(5);
        if (mailtable->rowCount() < 3)
            mailtable->setRowCount(3);
        mailtable->setObjectName(QString::fromUtf8("mailtable"));
        mailtable->setEditTriggers(QAbstractItemView::NoEditTriggers);
        mailtable->setProperty("showDropIndicator", QVariant(false));
        mailtable->setDragDropOverwriteMode(false);
        mailtable->setAlternatingRowColors(true);
        mailtable->setSelectionMode(QAbstractItemView::SingleSelection);
        mailtable->setSelectionBehavior(QAbstractItemView::SelectRows);
        mailtable->setRowCount(3);
        mailtable->setColumnCount(5);
        splitter->addWidget(mailtable);
        mailtable->horizontalHeader()->setStretchLastSection(true);
        mailtable->verticalHeader()->setVisible(false);
        mailcontent = new QPlainTextEdit(splitter);
        mailcontent->setObjectName(QString::fromUtf8("mailcontent"));
        mailcontent->setReadOnly(true);
        splitter->addWidget(mailcontent);

        verticalLayout->addWidget(splitter);

        label = new QLabel(MBoxDialog);
        label->setObjectName(QString::fromUtf8("label"));
        label->setFrameShape(QFrame::NoFrame);
        label->setFrameShadow(QFrame::Sunken);
        label->setMargin(6);
        label->setIndent(5);

        verticalLayout->addWidget(label);


        retranslateUi(MBoxDialog);

        QMetaObject::connectSlotsByName(MBoxDialog);
    } // setupUi

    void retranslateUi(QDialog *MBoxDialog)
    {
        MBoxDialog->setWindowTitle(QCoreApplication::translate("MBoxDialog", "MBox Viewer", nullptr));
        toolButton->setText(QCoreApplication::translate("MBoxDialog", "...", nullptr));
        label->setText(QCoreApplication::translate("MBoxDialog", "TextLabel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MBoxDialog: public Ui_MBoxDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MBOXVIEWER_H
