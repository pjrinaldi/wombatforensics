/********************************************************************************
** Form generated from reading UI file 'htmlviewer.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_HTMLVIEWER_H
#define UI_HTMLVIEWER_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QTextBrowser>

QT_BEGIN_NAMESPACE

class Ui_HtmlViewer
{
public:
    QHBoxLayout *horizontalLayout;
    QTextBrowser *textbrowser;

    void setupUi(QDialog *HtmlViewer)
    {
        if (HtmlViewer->objectName().isEmpty())
            HtmlViewer->setObjectName(QString::fromUtf8("HtmlViewer"));
        HtmlViewer->resize(640, 480);
        HtmlViewer->setSizeGripEnabled(true);
        horizontalLayout = new QHBoxLayout(HtmlViewer);
        horizontalLayout->setSpacing(0);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        textbrowser = new QTextBrowser(HtmlViewer);
        textbrowser->setObjectName(QString::fromUtf8("textbrowser"));

        horizontalLayout->addWidget(textbrowser);


        retranslateUi(HtmlViewer);

        QMetaObject::connectSlotsByName(HtmlViewer);
    } // setupUi

    void retranslateUi(QDialog *HtmlViewer)
    {
        HtmlViewer->setWindowTitle(QCoreApplication::translate("HtmlViewer", "Html Viewer", nullptr));
    } // retranslateUi

};

namespace Ui {
    class HtmlViewer: public Ui_HtmlViewer {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_HTMLVIEWER_H
