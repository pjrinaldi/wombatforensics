/********************************************************************************
** Form generated from reading UI file 'carvingfiletypesdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CARVINGFILETYPESDIALOG_H
#define UI_CARVINGFILETYPESDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTableWidget>

QT_BEGIN_NAMESPACE

class Ui_CarvingFileTypesDialog
{
public:
    QGridLayout *gridLayout_2;
    QPushButton *savepushbutton;
    QPushButton *saveclosepushbutton;
    QSpacerItem *horizontalSpacer;
    QPushButton *addrowpushbutton;
    QPushButton *closebutton;
    QTableWidget *filetypestablewidget;
    QPlainTextEdit *filetypeseditor;

    void setupUi(QDialog *CarvingFileTypesDialog)
    {
        if (CarvingFileTypesDialog->objectName().isEmpty())
            CarvingFileTypesDialog->setObjectName(QString::fromUtf8("CarvingFileTypesDialog"));
        CarvingFileTypesDialog->resize(957, 578);
        gridLayout_2 = new QGridLayout(CarvingFileTypesDialog);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        savepushbutton = new QPushButton(CarvingFileTypesDialog);
        savepushbutton->setObjectName(QString::fromUtf8("savepushbutton"));

        gridLayout_2->addWidget(savepushbutton, 0, 4, 1, 1);

        saveclosepushbutton = new QPushButton(CarvingFileTypesDialog);
        saveclosepushbutton->setObjectName(QString::fromUtf8("saveclosepushbutton"));

        gridLayout_2->addWidget(saveclosepushbutton, 0, 5, 1, 1);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_2->addItem(horizontalSpacer, 0, 0, 1, 1);

        addrowpushbutton = new QPushButton(CarvingFileTypesDialog);
        addrowpushbutton->setObjectName(QString::fromUtf8("addrowpushbutton"));

        gridLayout_2->addWidget(addrowpushbutton, 0, 2, 1, 1);

        closebutton = new QPushButton(CarvingFileTypesDialog);
        closebutton->setObjectName(QString::fromUtf8("closebutton"));

        gridLayout_2->addWidget(closebutton, 0, 6, 1, 1);

        filetypestablewidget = new QTableWidget(CarvingFileTypesDialog);
        if (filetypestablewidget->columnCount() < 6)
            filetypestablewidget->setColumnCount(6);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        filetypestablewidget->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        filetypestablewidget->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        filetypestablewidget->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        filetypestablewidget->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        filetypestablewidget->setHorizontalHeaderItem(4, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        filetypestablewidget->setHorizontalHeaderItem(5, __qtablewidgetitem5);
        if (filetypestablewidget->rowCount() < 3)
            filetypestablewidget->setRowCount(3);
        filetypestablewidget->setObjectName(QString::fromUtf8("filetypestablewidget"));
        filetypestablewidget->setProperty("showDropIndicator", QVariant(false));
        filetypestablewidget->setDragDropOverwriteMode(false);
        filetypestablewidget->setAlternatingRowColors(true);
        filetypestablewidget->setGridStyle(Qt::SolidLine);
        filetypestablewidget->setRowCount(3);
        filetypestablewidget->setColumnCount(6);
        filetypestablewidget->horizontalHeader()->setCascadingSectionResizes(false);
        filetypestablewidget->horizontalHeader()->setMinimumSectionSize(60);
        filetypestablewidget->horizontalHeader()->setDefaultSectionSize(100);
        filetypestablewidget->horizontalHeader()->setStretchLastSection(true);
        filetypestablewidget->verticalHeader()->setVisible(false);

        gridLayout_2->addWidget(filetypestablewidget, 2, 0, 1, 7);

        filetypeseditor = new QPlainTextEdit(CarvingFileTypesDialog);
        filetypeseditor->setObjectName(QString::fromUtf8("filetypeseditor"));
        filetypeseditor->setLineWrapMode(QPlainTextEdit::WidgetWidth);

        gridLayout_2->addWidget(filetypeseditor, 1, 0, 1, 7);


        retranslateUi(CarvingFileTypesDialog);

        QMetaObject::connectSlotsByName(CarvingFileTypesDialog);
    } // setupUi

    void retranslateUi(QDialog *CarvingFileTypesDialog)
    {
        CarvingFileTypesDialog->setWindowTitle(QCoreApplication::translate("CarvingFileTypesDialog", "Carving File Types Editor", nullptr));
        savepushbutton->setText(QCoreApplication::translate("CarvingFileTypesDialog", "Save", nullptr));
        saveclosepushbutton->setText(QCoreApplication::translate("CarvingFileTypesDialog", "Save && Close", nullptr));
        addrowpushbutton->setText(QCoreApplication::translate("CarvingFileTypesDialog", "Add Row", nullptr));
        closebutton->setText(QCoreApplication::translate("CarvingFileTypesDialog", "Close", nullptr));
        QTableWidgetItem *___qtablewidgetitem = filetypestablewidget->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("CarvingFileTypesDialog", "Category", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = filetypestablewidget->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("CarvingFileTypesDialog", "Description", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = filetypestablewidget->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QCoreApplication::translate("CarvingFileTypesDialog", "Header", nullptr));
        QTableWidgetItem *___qtablewidgetitem3 = filetypestablewidget->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QCoreApplication::translate("CarvingFileTypesDialog", "Footer", nullptr));
        QTableWidgetItem *___qtablewidgetitem4 = filetypestablewidget->horizontalHeaderItem(4);
        ___qtablewidgetitem4->setText(QCoreApplication::translate("CarvingFileTypesDialog", "Extension", nullptr));
        QTableWidgetItem *___qtablewidgetitem5 = filetypestablewidget->horizontalHeaderItem(5);
        ___qtablewidgetitem5->setText(QCoreApplication::translate("CarvingFileTypesDialog", "Maximum File Size", nullptr));
        filetypeseditor->setDocumentTitle(QString());
    } // retranslateUi

};

namespace Ui {
    class CarvingFileTypesDialog: public Ui_CarvingFileTypesDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CARVINGFILETYPESDIALOG_H
