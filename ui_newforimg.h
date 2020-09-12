/********************************************************************************
** Form generated from reading UI file 'newforimg.ui'
**
** Created by: Qt User Interface Compiler version 5.15.1
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
    QSpacerItem *horizontalSpacer_2;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_3;
    QLineEdit *nameedit;
    QSpacerItem *horizontalSpacer_3;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_4;
    QLineEdit *pathedit;
    QPushButton *browsebutton;
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
        ForImgDialog->resize(570, 260);
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

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);


        verticalLayout->addLayout(horizontalLayout_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        label_3 = new QLabel(ForImgDialog);
        label_3->setObjectName(QString::fromUtf8("label_3"));

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

        horizontalLayout_4->addWidget(label_4);

        pathedit = new QLineEdit(ForImgDialog);
        pathedit->setObjectName(QString::fromUtf8("pathedit"));

        horizontalLayout_4->addWidget(pathedit);

        browsebutton = new QPushButton(ForImgDialog);
        browsebutton->setObjectName(QString::fromUtf8("browsebutton"));

        horizontalLayout_4->addWidget(browsebutton);


        verticalLayout->addLayout(horizontalLayout_4);

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
        sourcecombo->setCurrentText(QCoreApplication::translate("ForImgDialog", "Select Source Device", nullptr));
        sourcecombo->setPlaceholderText(QCoreApplication::translate("ForImgDialog", "Select Source Device", nullptr));
        label_2->setText(QCoreApplication::translate("ForImgDialog", "Forensic Image Type:", nullptr));
        rawradio->setText(QCoreApplication::translate("ForImgDialog", "Raw", nullptr));
        aff4radio->setText(QCoreApplication::translate("ForImgDialog", "AFF4", nullptr));
        sfsradio->setText(QCoreApplication::translate("ForImgDialog", "Squash FS'd Raw", nullptr));
        label_3->setText(QCoreApplication::translate("ForImgDialog", "Forensic Image Name:", nullptr));
        nameedit->setPlaceholderText(QCoreApplication::translate("ForImgDialog", "Name w/o Extension", nullptr));
        label_4->setText(QCoreApplication::translate("ForImgDialog", "Destination Folder:", nullptr));
        browsebutton->setText(QCoreApplication::translate("ForImgDialog", "Browse", nullptr));
        cancelbutton->setText(QCoreApplication::translate("ForImgDialog", "Cancel", nullptr));
        createbutton->setText(QCoreApplication::translate("ForImgDialog", "Create Forensic Image", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ForImgDialog: public Ui_ForImgDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_NEWFORIMG_H
