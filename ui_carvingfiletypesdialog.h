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
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>

QT_BEGIN_NAMESPACE

class Ui_CarvingFileTypesDialog
{
public:
    QGridLayout *gridLayout_2;
    QPushButton *savepushbutton;
    QSpacerItem *horizontalSpacer;
    QPlainTextEdit *filetypeseditor;

    void setupUi(QDialog *CarvingFileTypesDialog)
    {
        if (CarvingFileTypesDialog->objectName().isEmpty())
            CarvingFileTypesDialog->setObjectName(QString::fromUtf8("CarvingFileTypesDialog"));
        CarvingFileTypesDialog->resize(727, 455);
        gridLayout_2 = new QGridLayout(CarvingFileTypesDialog);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        savepushbutton = new QPushButton(CarvingFileTypesDialog);
        savepushbutton->setObjectName(QString::fromUtf8("savepushbutton"));

        gridLayout_2->addWidget(savepushbutton, 0, 1, 1, 1);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_2->addItem(horizontalSpacer, 0, 0, 1, 1);

        filetypeseditor = new QPlainTextEdit(CarvingFileTypesDialog);
        filetypeseditor->setObjectName(QString::fromUtf8("filetypeseditor"));
        filetypeseditor->setLineWrapMode(QPlainTextEdit::WidgetWidth);

        gridLayout_2->addWidget(filetypeseditor, 1, 0, 1, 2);


        retranslateUi(CarvingFileTypesDialog);

        QMetaObject::connectSlotsByName(CarvingFileTypesDialog);
    } // setupUi

    void retranslateUi(QDialog *CarvingFileTypesDialog)
    {
        CarvingFileTypesDialog->setWindowTitle(QCoreApplication::translate("CarvingFileTypesDialog", "Carving File Types Editor", nullptr));
        savepushbutton->setText(QCoreApplication::translate("CarvingFileTypesDialog", "Save", nullptr));
        filetypeseditor->setDocumentTitle(QString());
    } // retranslateUi

};

namespace Ui {
    class CarvingFileTypesDialog: public Ui_CarvingFileTypesDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CARVINGFILETYPESDIALOG_H
