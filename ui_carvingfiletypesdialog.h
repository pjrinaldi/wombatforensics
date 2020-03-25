/********************************************************************************
** Form generated from reading UI file 'carvingfiletypesdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.14.1
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
    QSpacerItem *horizontalSpacer;
    QPushButton *savepushbutton;
    QPushButton *pushButton;
    QPlainTextEdit *filetypeseditor;
    QTableWidget *filetypestablewidget;

    void setupUi(QDialog *CarvingFileTypesDialog)
    {
        if (CarvingFileTypesDialog->objectName().isEmpty())
            CarvingFileTypesDialog->setObjectName(QString::fromUtf8("CarvingFileTypesDialog"));
        CarvingFileTypesDialog->resize(787, 473);
        gridLayout_2 = new QGridLayout(CarvingFileTypesDialog);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_2->addItem(horizontalSpacer, 0, 0, 1, 1);

        savepushbutton = new QPushButton(CarvingFileTypesDialog);
        savepushbutton->setObjectName(QString::fromUtf8("savepushbutton"));

        gridLayout_2->addWidget(savepushbutton, 0, 2, 1, 1);

        pushButton = new QPushButton(CarvingFileTypesDialog);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));

        gridLayout_2->addWidget(pushButton, 0, 3, 1, 1);

        filetypeseditor = new QPlainTextEdit(CarvingFileTypesDialog);
        filetypeseditor->setObjectName(QString::fromUtf8("filetypeseditor"));
        filetypeseditor->setLineWrapMode(QPlainTextEdit::WidgetWidth);

        gridLayout_2->addWidget(filetypeseditor, 1, 0, 1, 4);

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
        QTableWidgetItem *__qtablewidgetitem6 = new QTableWidgetItem();
        filetypestablewidget->setItem(0, 0, __qtablewidgetitem6);
        QTableWidgetItem *__qtablewidgetitem7 = new QTableWidgetItem();
        filetypestablewidget->setItem(0, 1, __qtablewidgetitem7);
        QTableWidgetItem *__qtablewidgetitem8 = new QTableWidgetItem();
        filetypestablewidget->setItem(0, 2, __qtablewidgetitem8);
        QTableWidgetItem *__qtablewidgetitem9 = new QTableWidgetItem();
        filetypestablewidget->setItem(0, 3, __qtablewidgetitem9);
        QTableWidgetItem *__qtablewidgetitem10 = new QTableWidgetItem();
        filetypestablewidget->setItem(0, 4, __qtablewidgetitem10);
        QTableWidgetItem *__qtablewidgetitem11 = new QTableWidgetItem();
        filetypestablewidget->setItem(0, 5, __qtablewidgetitem11);
        QTableWidgetItem *__qtablewidgetitem12 = new QTableWidgetItem();
        filetypestablewidget->setItem(1, 0, __qtablewidgetitem12);
        QTableWidgetItem *__qtablewidgetitem13 = new QTableWidgetItem();
        filetypestablewidget->setItem(1, 1, __qtablewidgetitem13);
        QTableWidgetItem *__qtablewidgetitem14 = new QTableWidgetItem();
        filetypestablewidget->setItem(1, 2, __qtablewidgetitem14);
        QTableWidgetItem *__qtablewidgetitem15 = new QTableWidgetItem();
        filetypestablewidget->setItem(1, 3, __qtablewidgetitem15);
        QTableWidgetItem *__qtablewidgetitem16 = new QTableWidgetItem();
        filetypestablewidget->setItem(1, 4, __qtablewidgetitem16);
        QTableWidgetItem *__qtablewidgetitem17 = new QTableWidgetItem();
        filetypestablewidget->setItem(1, 5, __qtablewidgetitem17);
        QTableWidgetItem *__qtablewidgetitem18 = new QTableWidgetItem();
        filetypestablewidget->setItem(2, 0, __qtablewidgetitem18);
        QTableWidgetItem *__qtablewidgetitem19 = new QTableWidgetItem();
        filetypestablewidget->setItem(2, 1, __qtablewidgetitem19);
        QTableWidgetItem *__qtablewidgetitem20 = new QTableWidgetItem();
        filetypestablewidget->setItem(2, 2, __qtablewidgetitem20);
        QTableWidgetItem *__qtablewidgetitem21 = new QTableWidgetItem();
        filetypestablewidget->setItem(2, 3, __qtablewidgetitem21);
        QTableWidgetItem *__qtablewidgetitem22 = new QTableWidgetItem();
        filetypestablewidget->setItem(2, 4, __qtablewidgetitem22);
        QTableWidgetItem *__qtablewidgetitem23 = new QTableWidgetItem();
        filetypestablewidget->setItem(2, 5, __qtablewidgetitem23);
        filetypestablewidget->setObjectName(QString::fromUtf8("filetypestablewidget"));
        filetypestablewidget->setProperty("showDropIndicator", QVariant(false));
        filetypestablewidget->setDragDropOverwriteMode(false);
        filetypestablewidget->setAlternatingRowColors(true);
        filetypestablewidget->setGridStyle(Qt::SolidLine);
        filetypestablewidget->setRowCount(3);
        filetypestablewidget->setColumnCount(6);
        filetypestablewidget->horizontalHeader()->setCascadingSectionResizes(false);
        filetypestablewidget->horizontalHeader()->setDefaultSectionSize(100);
        filetypestablewidget->horizontalHeader()->setStretchLastSection(true);
        filetypestablewidget->verticalHeader()->setVisible(false);

        gridLayout_2->addWidget(filetypestablewidget, 2, 0, 1, 4);


        retranslateUi(CarvingFileTypesDialog);

        QMetaObject::connectSlotsByName(CarvingFileTypesDialog);
    } // setupUi

    void retranslateUi(QDialog *CarvingFileTypesDialog)
    {
        CarvingFileTypesDialog->setWindowTitle(QCoreApplication::translate("CarvingFileTypesDialog", "Carving File Types Editor", nullptr));
        savepushbutton->setText(QCoreApplication::translate("CarvingFileTypesDialog", "Save", nullptr));
        pushButton->setText(QCoreApplication::translate("CarvingFileTypesDialog", "Save && Close", nullptr));
        filetypeseditor->setDocumentTitle(QString());
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

        const bool __sortingEnabled = filetypestablewidget->isSortingEnabled();
        filetypestablewidget->setSortingEnabled(false);
        QTableWidgetItem *___qtablewidgetitem6 = filetypestablewidget->item(0, 0);
        ___qtablewidgetitem6->setText(QCoreApplication::translate("CarvingFileTypesDialog", "Pictures", nullptr));
        QTableWidgetItem *___qtablewidgetitem7 = filetypestablewidget->item(0, 1);
        ___qtablewidgetitem7->setText(QCoreApplication::translate("CarvingFileTypesDialog", "Generic JPEG", nullptr));
        QTableWidgetItem *___qtablewidgetitem8 = filetypestablewidget->item(0, 2);
        ___qtablewidgetitem8->setText(QCoreApplication::translate("CarvingFileTypesDialog", "FFD8FFE0????4A464946", nullptr));
        QTableWidgetItem *___qtablewidgetitem9 = filetypestablewidget->item(0, 3);
        ___qtablewidgetitem9->setText(QCoreApplication::translate("CarvingFileTypesDialog", "FFD9", nullptr));
        QTableWidgetItem *___qtablewidgetitem10 = filetypestablewidget->item(0, 4);
        ___qtablewidgetitem10->setText(QCoreApplication::translate("CarvingFileTypesDialog", "JPG", nullptr));
        QTableWidgetItem *___qtablewidgetitem11 = filetypestablewidget->item(0, 5);
        ___qtablewidgetitem11->setText(QCoreApplication::translate("CarvingFileTypesDialog", "2500000", nullptr));
        QTableWidgetItem *___qtablewidgetitem12 = filetypestablewidget->item(1, 0);
        ___qtablewidgetitem12->setText(QCoreApplication::translate("CarvingFileTypesDialog", "Pictures", nullptr));
        QTableWidgetItem *___qtablewidgetitem13 = filetypestablewidget->item(1, 1);
        ___qtablewidgetitem13->setText(QCoreApplication::translate("CarvingFileTypesDialog", "Exif JPEG", nullptr));
        QTableWidgetItem *___qtablewidgetitem14 = filetypestablewidget->item(1, 2);
        ___qtablewidgetitem14->setText(QCoreApplication::translate("CarvingFileTypesDialog", "FFD8FFE1????45786966", nullptr));
        QTableWidgetItem *___qtablewidgetitem15 = filetypestablewidget->item(1, 3);
        ___qtablewidgetitem15->setText(QCoreApplication::translate("CarvingFileTypesDialog", "FFD9", nullptr));
        QTableWidgetItem *___qtablewidgetitem16 = filetypestablewidget->item(1, 4);
        ___qtablewidgetitem16->setText(QCoreApplication::translate("CarvingFileTypesDialog", "JPG", nullptr));
        QTableWidgetItem *___qtablewidgetitem17 = filetypestablewidget->item(1, 5);
        ___qtablewidgetitem17->setText(QCoreApplication::translate("CarvingFileTypesDialog", "2500000", nullptr));
        QTableWidgetItem *___qtablewidgetitem18 = filetypestablewidget->item(2, 0);
        ___qtablewidgetitem18->setText(QCoreApplication::translate("CarvingFileTypesDialog", "Pictures", nullptr));
        QTableWidgetItem *___qtablewidgetitem19 = filetypestablewidget->item(2, 1);
        ___qtablewidgetitem19->setText(QCoreApplication::translate("CarvingFileTypesDialog", "SPIFF JPEG", nullptr));
        QTableWidgetItem *___qtablewidgetitem20 = filetypestablewidget->item(2, 2);
        ___qtablewidgetitem20->setText(QCoreApplication::translate("CarvingFileTypesDialog", "FFD8FFE8????5350494646", nullptr));
        QTableWidgetItem *___qtablewidgetitem21 = filetypestablewidget->item(2, 3);
        ___qtablewidgetitem21->setText(QCoreApplication::translate("CarvingFileTypesDialog", "FFD9", nullptr));
        QTableWidgetItem *___qtablewidgetitem22 = filetypestablewidget->item(2, 4);
        ___qtablewidgetitem22->setText(QCoreApplication::translate("CarvingFileTypesDialog", "JPG", nullptr));
        QTableWidgetItem *___qtablewidgetitem23 = filetypestablewidget->item(2, 5);
        ___qtablewidgetitem23->setText(QCoreApplication::translate("CarvingFileTypesDialog", "2500000", nullptr));
        filetypestablewidget->setSortingEnabled(__sortingEnabled);

    } // retranslateUi

};

namespace Ui {
    class CarvingFileTypesDialog: public Ui_CarvingFileTypesDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CARVINGFILETYPESDIALOG_H
