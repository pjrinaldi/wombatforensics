/********************************************************************************
** Form generated from reading UI file 'textviewer.ui'
**
** Created by: Qt User Interface Compiler version 5.15.8
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TEXTVIEWER_H
#define UI_TEXTVIEWER_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_TextViewer
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QComboBox *comboBox;
    QSpacerItem *horizontalSpacer;
    QPlainTextEdit *textEdit;

    void setupUi(QDialog *TextViewer)
    {
        if (TextViewer->objectName().isEmpty())
            TextViewer->setObjectName(QString::fromUtf8("TextViewer"));
        TextViewer->setWindowModality(Qt::NonModal);
        TextViewer->resize(640, 480);
        verticalLayout = new QVBoxLayout(TextViewer);
        verticalLayout->setSpacing(3);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label = new QLabel(TextViewer);
        label->setObjectName(QString::fromUtf8("label"));
        label->setEnabled(false);

        horizontalLayout->addWidget(label);

        comboBox = new QComboBox(TextViewer);
        comboBox->setObjectName(QString::fromUtf8("comboBox"));
        comboBox->setEnabled(false);

        horizontalLayout->addWidget(comboBox);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);


        verticalLayout->addLayout(horizontalLayout);

        textEdit = new QPlainTextEdit(TextViewer);
        textEdit->setObjectName(QString::fromUtf8("textEdit"));
        textEdit->setUndoRedoEnabled(false);
        textEdit->setReadOnly(true);

        verticalLayout->addWidget(textEdit);


        retranslateUi(TextViewer);

        QMetaObject::connectSlotsByName(TextViewer);
    } // setupUi

    void retranslateUi(QDialog *TextViewer)
    {
        TextViewer->setWindowTitle(QCoreApplication::translate("TextViewer", "Dialog", nullptr));
        label->setText(QCoreApplication::translate("TextViewer", "Encoding", nullptr));
        comboBox->setCurrentText(QString());
    } // retranslateUi

};

namespace Ui {
    class TextViewer: public Ui_TextViewer {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TEXTVIEWER_H
