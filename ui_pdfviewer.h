/********************************************************************************
** Form generated from reading UI file 'pdfviewer.ui'
**
** Created by: Qt User Interface Compiler version 5.15.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PDFVIEWER_H
#define UI_PDFVIEWER_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_PdfViewer
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QPushButton *prevbutton;
    QPushButton *nextbutton;
    QSpacerItem *horizontalSpacer;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label;

    void setupUi(QDialog *PdfViewer)
    {
        if (PdfViewer->objectName().isEmpty())
            PdfViewer->setObjectName(QString::fromUtf8("PdfViewer"));
        PdfViewer->resize(800, 600);
        verticalLayout = new QVBoxLayout(PdfViewer);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        prevbutton = new QPushButton(PdfViewer);
        prevbutton->setObjectName(QString::fromUtf8("prevbutton"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/bar/previous"), QSize(), QIcon::Normal, QIcon::Off);
        prevbutton->setIcon(icon);
        prevbutton->setCheckable(true);
        prevbutton->setChecked(false);

        horizontalLayout->addWidget(prevbutton);

        nextbutton = new QPushButton(PdfViewer);
        nextbutton->setObjectName(QString::fromUtf8("nextbutton"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/bar/next"), QSize(), QIcon::Normal, QIcon::Off);
        nextbutton->setIcon(icon1);

        horizontalLayout->addWidget(nextbutton);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);


        verticalLayout->addLayout(horizontalLayout);

        scrollArea = new QScrollArea(PdfViewer);
        scrollArea->setObjectName(QString::fromUtf8("scrollArea"));
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QString::fromUtf8("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 780, 548));
        horizontalLayout_2 = new QHBoxLayout(scrollAreaWidgetContents);
        horizontalLayout_2->setSpacing(0);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        label = new QLabel(scrollAreaWidgetContents);
        label->setObjectName(QString::fromUtf8("label"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy);
        label->setLineWidth(0);
        label->setScaledContents(false);
        label->setAlignment(Qt::AlignCenter);
        label->setTextInteractionFlags(Qt::NoTextInteraction);

        horizontalLayout_2->addWidget(label);

        scrollArea->setWidget(scrollAreaWidgetContents);

        verticalLayout->addWidget(scrollArea);


        retranslateUi(PdfViewer);

        QMetaObject::connectSlotsByName(PdfViewer);
    } // setupUi

    void retranslateUi(QDialog *PdfViewer)
    {
        PdfViewer->setWindowTitle(QCoreApplication::translate("PdfViewer", "View PDF", nullptr));
        prevbutton->setText(QString());
        nextbutton->setText(QString());
        label->setText(QCoreApplication::translate("PdfViewer", "Loading...", nullptr));
    } // retranslateUi

};

namespace Ui {
    class PdfViewer: public Ui_PdfViewer {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PDFVIEWER_H
