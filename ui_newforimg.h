/********************************************************************************
** Form generated from reading UI file 'newforimg.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_NEWFORIMG_H
#define UI_NEWFORIMG_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_ForImgDialog
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QComboBox *sourcecombo;
    QSpacerItem *horizontalSpacer;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_2;
    QRadioButton *rawradio;
    QRadioButton *aff4radio;
    QRadioButton *sfsradio;
    QRadioButton *zmgradio;
    QRadioButton *wfiradio;
    QSpacerItem *horizontalSpacer_2;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_3;
    QLineEdit *nameedit;
    QSpacerItem *horizontalSpacer_3;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_4;
    QLineEdit *pathedit;
    QPushButton *browsebutton;
    QHBoxLayout *horizontalLayout_6;
    QLabel *label_5;
    QLineEdit *caseedit;
    QSpacerItem *horizontalSpacer_4;
    QHBoxLayout *horizontalLayout_7;
    QLabel *label_6;
    QLineEdit *examineredit;
    QSpacerItem *horizontalSpacer_6;
    QHBoxLayout *horizontalLayout_8;
    QLabel *label_7;
    QLineEdit *evidnumedit;
    QSpacerItem *horizontalSpacer_7;
    QHBoxLayout *horizontalLayout_9;
    QLabel *label_8;
    QTextEdit *descriptionedit;
    QSpacerItem *verticalSpacer;
    QHBoxLayout *horizontalLayout_5;
    QSpacerItem *horizontalSpacer_5;
    QPushButton *cancelbutton;
    QPushButton *createbutton;

    void setupUi(QDialog *ForImgDialog)
    {
        if (ForImgDialog->objectName().isEmpty())
            ForImgDialog->setObjectName(QString::fromUtf8("ForImgDialog"));
        ForImgDialog->setWindowModality(Qt::ApplicationModal);
        ForImgDialog->resize(622, 426);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/bar/newforimg"), QSize(), QIcon::Normal, QIcon::Off);
        ForImgDialog->setWindowIcon(icon);
        verticalLayout = new QVBoxLayout(ForImgDialog);
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label = new QLabel(ForImgDialog);
        label->setObjectName(QString::fromUtf8("label"));
        label->setMinimumSize(QSize(150, 0));

        horizontalLayout->addWidget(label);

        sourcecombo = new QComboBox(ForImgDialog);
        sourcecombo->setObjectName(QString::fromUtf8("sourcecombo"));

        horizontalLayout->addWidget(sourcecombo);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        label_2 = new QLabel(ForImgDialog);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setMinimumSize(QSize(150, 0));

        horizontalLayout_2->addWidget(label_2);

        rawradio = new QRadioButton(ForImgDialog);
        rawradio->setObjectName(QString::fromUtf8("rawradio"));

        horizontalLayout_2->addWidget(rawradio);

        aff4radio = new QRadioButton(ForImgDialog);
        aff4radio->setObjectName(QString::fromUtf8("aff4radio"));

        horizontalLayout_2->addWidget(aff4radio);

        sfsradio = new QRadioButton(ForImgDialog);
        sfsradio->setObjectName(QString::fromUtf8("sfsradio"));

        horizontalLayout_2->addWidget(sfsradio);

        zmgradio = new QRadioButton(ForImgDialog);
        zmgradio->setObjectName(QString::fromUtf8("zmgradio"));

        horizontalLayout_2->addWidget(zmgradio);

        wfiradio = new QRadioButton(ForImgDialog);
        wfiradio->setObjectName(QString::fromUtf8("wfiradio"));

        horizontalLayout_2->addWidget(wfiradio);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);


        verticalLayout->addLayout(horizontalLayout_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        label_3 = new QLabel(ForImgDialog);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setMinimumSize(QSize(150, 0));

        horizontalLayout_3->addWidget(label_3);

        nameedit = new QLineEdit(ForImgDialog);
        nameedit->setObjectName(QString::fromUtf8("nameedit"));

        horizontalLayout_3->addWidget(nameedit);

        horizontalSpacer_3 = new QSpacerItem(100, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_3);


        verticalLayout->addLayout(horizontalLayout_3);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        label_4 = new QLabel(ForImgDialog);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setMinimumSize(QSize(150, 0));

        horizontalLayout_4->addWidget(label_4);

        pathedit = new QLineEdit(ForImgDialog);
        pathedit->setObjectName(QString::fromUtf8("pathedit"));

        horizontalLayout_4->addWidget(pathedit);

        browsebutton = new QPushButton(ForImgDialog);
        browsebutton->setObjectName(QString::fromUtf8("browsebutton"));

        horizontalLayout_4->addWidget(browsebutton);


        verticalLayout->addLayout(horizontalLayout_4);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setSpacing(6);
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        horizontalLayout_6->setContentsMargins(-1, 0, -1, 0);
        label_5 = new QLabel(ForImgDialog);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setMinimumSize(QSize(150, 0));

        horizontalLayout_6->addWidget(label_5);

        caseedit = new QLineEdit(ForImgDialog);
        caseedit->setObjectName(QString::fromUtf8("caseedit"));

        horizontalLayout_6->addWidget(caseedit);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_6->addItem(horizontalSpacer_4);


        verticalLayout->addLayout(horizontalLayout_6);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
        horizontalLayout_7->setContentsMargins(-1, 0, -1, 0);
        label_6 = new QLabel(ForImgDialog);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setMinimumSize(QSize(150, 0));

        horizontalLayout_7->addWidget(label_6);

        examineredit = new QLineEdit(ForImgDialog);
        examineredit->setObjectName(QString::fromUtf8("examineredit"));

        horizontalLayout_7->addWidget(examineredit);

        horizontalSpacer_6 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_7->addItem(horizontalSpacer_6);


        verticalLayout->addLayout(horizontalLayout_7);

        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setObjectName(QString::fromUtf8("horizontalLayout_8"));
        horizontalLayout_8->setContentsMargins(-1, 0, -1, 0);
        label_7 = new QLabel(ForImgDialog);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setMinimumSize(QSize(150, 0));

        horizontalLayout_8->addWidget(label_7);

        evidnumedit = new QLineEdit(ForImgDialog);
        evidnumedit->setObjectName(QString::fromUtf8("evidnumedit"));

        horizontalLayout_8->addWidget(evidnumedit);

        horizontalSpacer_7 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_8->addItem(horizontalSpacer_7);


        verticalLayout->addLayout(horizontalLayout_8);

        horizontalLayout_9 = new QHBoxLayout();
        horizontalLayout_9->setObjectName(QString::fromUtf8("horizontalLayout_9"));
        horizontalLayout_9->setContentsMargins(-1, 0, -1, 0);
        label_8 = new QLabel(ForImgDialog);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setMinimumSize(QSize(150, 0));
        label_8->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);

        horizontalLayout_9->addWidget(label_8);

        descriptionedit = new QTextEdit(ForImgDialog);
        descriptionedit->setObjectName(QString::fromUtf8("descriptionedit"));

        horizontalLayout_9->addWidget(descriptionedit);


        verticalLayout->addLayout(horizontalLayout_9);

        verticalSpacer = new QSpacerItem(20, 30, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setSpacing(6);
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_5);

        cancelbutton = new QPushButton(ForImgDialog);
        cancelbutton->setObjectName(QString::fromUtf8("cancelbutton"));

        horizontalLayout_5->addWidget(cancelbutton);

        createbutton = new QPushButton(ForImgDialog);
        createbutton->setObjectName(QString::fromUtf8("createbutton"));

        horizontalLayout_5->addWidget(createbutton);


        verticalLayout->addLayout(horizontalLayout_5);


        retranslateUi(ForImgDialog);

        QMetaObject::connectSlotsByName(ForImgDialog);
    } // setupUi

    void retranslateUi(QDialog *ForImgDialog)
    {
        ForImgDialog->setWindowTitle(QCoreApplication::translate("ForImgDialog", "Create Forensic Image", nullptr));
        label->setText(QCoreApplication::translate("ForImgDialog", "Select Source Device:", nullptr));
        sourcecombo->setCurrentText(QString());
        sourcecombo->setPlaceholderText(QCoreApplication::translate("ForImgDialog", "Select Source Device", nullptr));
        label_2->setText(QCoreApplication::translate("ForImgDialog", "Forensic Image Type:", nullptr));
        rawradio->setText(QCoreApplication::translate("ForImgDialog", "Raw", nullptr));
        aff4radio->setText(QCoreApplication::translate("ForImgDialog", "AFF4", nullptr));
        sfsradio->setText(QCoreApplication::translate("ForImgDialog", "Squash FS'd Raw", nullptr));
        zmgradio->setText(QCoreApplication::translate("ForImgDialog", "ZMG FS'd Raw", nullptr));
        wfiradio->setText(QCoreApplication::translate("ForImgDialog", "WFI", nullptr));
        label_3->setText(QCoreApplication::translate("ForImgDialog", "Forensic Image Name:", nullptr));
        nameedit->setPlaceholderText(QCoreApplication::translate("ForImgDialog", "Name w/o Extension", nullptr));
        label_4->setText(QCoreApplication::translate("ForImgDialog", "Destination Folder:", nullptr));
        browsebutton->setText(QCoreApplication::translate("ForImgDialog", "Browse", nullptr));
        label_5->setText(QCoreApplication::translate("ForImgDialog", "Case Number:", nullptr));
        caseedit->setInputMask(QString());
        caseedit->setPlaceholderText(QString());
        label_6->setText(QCoreApplication::translate("ForImgDialog", "Examiner:", nullptr));
        examineredit->setPlaceholderText(QString());
        label_7->setText(QCoreApplication::translate("ForImgDialog", "Evidence Number:", nullptr));
        label_8->setText(QCoreApplication::translate("ForImgDialog", "Description:", nullptr));
        cancelbutton->setText(QCoreApplication::translate("ForImgDialog", "Cancel", nullptr));
        createbutton->setText(QCoreApplication::translate("ForImgDialog", "Create Forensic Image", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ForImgDialog: public Ui_ForImgDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_NEWFORIMG_H
