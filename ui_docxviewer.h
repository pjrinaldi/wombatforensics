/********************************************************************************
** Form generated from reading UI file 'docxviewer.ui'
**
** Created by: Qt User Interface Compiler version 5.15.7
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DOCXVIEWER_H
#define UI_DOCXVIEWER_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_DocxViewer
{
public:
    QVBoxLayout *verticalLayout;
    QTextBrowser *textbrowser;

    void setupUi(QDialog *DocxViewer)
    {
        if (DocxViewer->objectName().isEmpty())
            DocxViewer->setObjectName(QString::fromUtf8("DocxViewer"));
        DocxViewer->resize(640, 480);
        DocxViewer->setSizeGripEnabled(true);
        verticalLayout = new QVBoxLayout(DocxViewer);
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        textbrowser = new QTextBrowser(DocxViewer);
        textbrowser->setObjectName(QString::fromUtf8("textbrowser"));
        textbrowser->setUndoRedoEnabled(false);
        textbrowser->setAcceptRichText(false);

        verticalLayout->addWidget(textbrowser);


        retranslateUi(DocxViewer);

        QMetaObject::connectSlotsByName(DocxViewer);
    } // setupUi

    void retranslateUi(QDialog *DocxViewer)
    {
        DocxViewer->setWindowTitle(QCoreApplication::translate("DocxViewer", "Docx Viewer", nullptr));
    } // retranslateUi

};

namespace Ui {
    class DocxViewer: public Ui_DocxViewer {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DOCXVIEWER_H
