/********************************************************************************
** Form generated from reading UI file 'htmlviewer.ui'
**
** Created by: Qt User Interface Compiler version 5.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_HTMLVIEWER_H
#define UI_HTMLVIEWER_H

#include <QtCore/QVariant>
#include <QtWebEngineWidgets/QWebEngineView>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>

QT_BEGIN_NAMESPACE

class Ui_HtmlViewer
{
public:
    QHBoxLayout *horizontalLayout;
    QWebEngineView *webView;

    void setupUi(QDialog *HtmlViewer)
    {
        if (HtmlViewer->objectName().isEmpty())
            HtmlViewer->setObjectName(QStringLiteral("HtmlViewer"));
        HtmlViewer->resize(640, 480);
        HtmlViewer->setSizeGripEnabled(true);
        horizontalLayout = new QHBoxLayout(HtmlViewer);
        horizontalLayout->setSpacing(0);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        webView = new QWebEngineView(HtmlViewer);
        webView->setObjectName(QStringLiteral("webView"));
        webView->setUrl(QUrl(QStringLiteral("about:blank")));

        horizontalLayout->addWidget(webView);


        retranslateUi(HtmlViewer);

        QMetaObject::connectSlotsByName(HtmlViewer);
    } // setupUi

    void retranslateUi(QDialog *HtmlViewer)
    {
        HtmlViewer->setWindowTitle(QApplication::translate("HtmlViewer", "Html Viewer", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class HtmlViewer: public Ui_HtmlViewer {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_HTMLVIEWER_H
