/********************************************************************************
** Form generated from reading UI file 'aboutbox.ui'
**
** Created by: Qt User Interface Compiler version 5.15.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ABOUTBOX_H
#define UI_ABOUTBOX_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_AboutBox
{
public:
    QHBoxLayout *horizontalLayout;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    QVBoxLayout *verticalLayout;
    QLabel *piclabel;
    QLabel *textlabel;

    void setupUi(QDialog *AboutBox)
    {
        if (AboutBox->objectName().isEmpty())
            AboutBox->setObjectName(QString::fromUtf8("AboutBox"));
        AboutBox->setWindowModality(Qt::ApplicationModal);
        AboutBox->resize(403, 443);
        AboutBox->setModal(true);
        horizontalLayout = new QHBoxLayout(AboutBox);
        horizontalLayout->setSpacing(0);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        scrollArea = new QScrollArea(AboutBox);
        scrollArea->setObjectName(QString::fromUtf8("scrollArea"));
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QString::fromUtf8("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 401, 441));
        scrollAreaWidgetContents->setCursor(QCursor(Qt::ArrowCursor));
        verticalLayout = new QVBoxLayout(scrollAreaWidgetContents);
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        piclabel = new QLabel(scrollAreaWidgetContents);
        piclabel->setObjectName(QString::fromUtf8("piclabel"));
        piclabel->setTextFormat(Qt::PlainText);
        piclabel->setPixmap(QPixmap(QString::fromUtf8(":/about")));
        piclabel->setScaledContents(false);

        verticalLayout->addWidget(piclabel);

        textlabel = new QLabel(scrollAreaWidgetContents);
        textlabel->setObjectName(QString::fromUtf8("textlabel"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(textlabel->sizePolicy().hasHeightForWidth());
        textlabel->setSizePolicy(sizePolicy);
        textlabel->setAutoFillBackground(false);
        textlabel->setStyleSheet(QString::fromUtf8("background-color: white;"));
        textlabel->setLineWidth(0);
        textlabel->setScaledContents(false);
        textlabel->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
        textlabel->setWordWrap(true);
        textlabel->setMargin(15);
        textlabel->setTextInteractionFlags(Qt::LinksAccessibleByMouse);

        verticalLayout->addWidget(textlabel);

        scrollArea->setWidget(scrollAreaWidgetContents);

        horizontalLayout->addWidget(scrollArea);


        retranslateUi(AboutBox);

        QMetaObject::connectSlotsByName(AboutBox);
    } // setupUi

    void retranslateUi(QDialog *AboutBox)
    {
        AboutBox->setWindowTitle(QCoreApplication::translate("AboutBox", "About WombatForensics", nullptr));
        piclabel->setText(QString());
        textlabel->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class AboutBox: public Ui_AboutBox {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ABOUTBOX_H
