/********************************************************************************
** Form generated from reading UI file 'messageviewer.ui'
**
** Created by: Qt User Interface Compiler version 5.4.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MESSAGEVIEWER_H
#define UI_MESSAGEVIEWER_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
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
            MessageViewer->setObjectName(QStringLiteral("MessageViewer"));
        MessageViewer->resize(480, 210);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MessageViewer->sizePolicy().hasHeightForWidth());
        MessageViewer->setSizePolicy(sizePolicy);
        verticalLayout = new QVBoxLayout(MessageViewer);
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        checkBox = new QCheckBox(MessageViewer);
        checkBox->setObjectName(QStringLiteral("checkBox"));

        verticalLayout->addWidget(checkBox, 0, Qt::AlignRight);

        textEdit = new QTextEdit(MessageViewer);
        textEdit->setObjectName(QStringLiteral("textEdit"));
        textEdit->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        textEdit->setSizeAdjustPolicy(QAbstractScrollArea::AdjustIgnored);
        textEdit->setUndoRedoEnabled(false);
        textEdit->setAcceptRichText(false);
        textEdit->setTextInteractionFlags(Qt::TextSelectableByKeyboard|Qt::TextSelectableByMouse);

        verticalLayout->addWidget(textEdit);


        retranslateUi(MessageViewer);

        QMetaObject::connectSlotsByName(MessageViewer);
    } // setupUi

    void retranslateUi(QDialog *MessageViewer)
    {
        MessageViewer->setWindowTitle(QApplication::translate("MessageViewer", "Message Viewer", 0));
        checkBox->setText(QApplication::translate("MessageViewer", "View Logged Messages", 0));
        textEdit->setPlaceholderText(QApplication::translate("MessageViewer", "Messages and Errors go here...", 0));
    } // retranslateUi

};

namespace Ui {
    class MessageViewer: public Ui_MessageViewer {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MESSAGEVIEWER_H
