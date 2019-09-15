/********************************************************************************
** Form generated from reading UI file 'messageviewer.ui'
**
** Created by: Qt User Interface Compiler version 5.13.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MESSAGEVIEWER_H
#define UI_MESSAGEVIEWER_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_MessageViewer
{
public:
    QVBoxLayout *verticalLayout;
    QCheckBox *checkBox;
    QTextEdit *textEdit;

    void setupUi(QDialog *MessageViewer)
    {
        if (MessageViewer->objectName().isEmpty())
            MessageViewer->setObjectName(QString::fromUtf8("MessageViewer"));
        MessageViewer->resize(480, 210);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MessageViewer->sizePolicy().hasHeightForWidth());
        MessageViewer->setSizePolicy(sizePolicy);
        verticalLayout = new QVBoxLayout(MessageViewer);
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        checkBox = new QCheckBox(MessageViewer);
        checkBox->setObjectName(QString::fromUtf8("checkBox"));

        verticalLayout->addWidget(checkBox, 0, Qt::AlignRight);

        textEdit = new QTextEdit(MessageViewer);
        textEdit->setObjectName(QString::fromUtf8("textEdit"));
        textEdit->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        textEdit->setSizeAdjustPolicy(QAbstractScrollArea::AdjustIgnored);
        textEdit->setUndoRedoEnabled(false);
        textEdit->setLineWrapMode(QTextEdit::WidgetWidth);
        textEdit->setAcceptRichText(false);
        textEdit->setTextInteractionFlags(Qt::TextSelectableByKeyboard|Qt::TextSelectableByMouse);

        verticalLayout->addWidget(textEdit);


        retranslateUi(MessageViewer);

        QMetaObject::connectSlotsByName(MessageViewer);
    } // setupUi

    void retranslateUi(QDialog *MessageViewer)
    {
        MessageViewer->setWindowTitle(QCoreApplication::translate("MessageViewer", "Message Viewer", nullptr));
        checkBox->setText(QCoreApplication::translate("MessageViewer", "View Logged Messages", nullptr));
        textEdit->setHtml(QCoreApplication::translate("MessageViewer", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Bitstream Vera Sans'; font-size:8pt; font-weight:400; font-style:normal;\">\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><br /></p></body></html>", nullptr));
        textEdit->setPlaceholderText(QCoreApplication::translate("MessageViewer", "Messages and Errors go here...", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MessageViewer: public Ui_MessageViewer {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MESSAGEVIEWER_H
