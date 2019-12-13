/********************************************************************************
** Form generated from reading UI file 'digstatus.ui'
**
** Created by: Qt User Interface Compiler version 5.13.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIGSTATUS_H
#define UI_DIGSTATUS_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_DigStatus
{
public:
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout;
    QLabel *imgthumblabel;
    QSpacerItem *horizontalSpacer;
    QToolButton *imgthumbbutton;
    QHBoxLayout *horizontalLayout_2;
    QLabel *vidthumblabel;
    QSpacerItem *horizontalSpacer_2;
    QToolButton *vidthumbbutton;
    QHBoxLayout *horizontalLayout_3;
    QLabel *hashlabel;
    QSpacerItem *horizontalSpacer_3;
    QToolButton *hashbutton;
    QSpacerItem *verticalSpacer;

    void setupUi(QDialog *DigStatus)
    {
        if (DigStatus->objectName().isEmpty())
            DigStatus->setObjectName(QString::fromUtf8("DigStatus"));
        DigStatus->setWindowModality(Qt::NonModal);
        DigStatus->resize(320, 294);
        DigStatus->setSizeGripEnabled(true);
        DigStatus->setModal(false);
        verticalLayout_2 = new QVBoxLayout(DigStatus);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        imgthumblabel = new QLabel(DigStatus);
        imgthumblabel->setObjectName(QString::fromUtf8("imgthumblabel"));

        horizontalLayout->addWidget(imgthumblabel);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        imgthumbbutton = new QToolButton(DigStatus);
        imgthumbbutton->setObjectName(QString::fromUtf8("imgthumbbutton"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/cancelthread"), QSize(), QIcon::Normal, QIcon::Off);
        imgthumbbutton->setIcon(icon);

        horizontalLayout->addWidget(imgthumbbutton);


        verticalLayout_2->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        vidthumblabel = new QLabel(DigStatus);
        vidthumblabel->setObjectName(QString::fromUtf8("vidthumblabel"));

        horizontalLayout_2->addWidget(vidthumblabel);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);

        vidthumbbutton = new QToolButton(DigStatus);
        vidthumbbutton->setObjectName(QString::fromUtf8("vidthumbbutton"));
        vidthumbbutton->setIcon(icon);

        horizontalLayout_2->addWidget(vidthumbbutton);


        verticalLayout_2->addLayout(horizontalLayout_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        hashlabel = new QLabel(DigStatus);
        hashlabel->setObjectName(QString::fromUtf8("hashlabel"));

        horizontalLayout_3->addWidget(hashlabel);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_3);

        hashbutton = new QToolButton(DigStatus);
        hashbutton->setObjectName(QString::fromUtf8("hashbutton"));
        hashbutton->setIcon(icon);

        horizontalLayout_3->addWidget(hashbutton);


        verticalLayout_2->addLayout(horizontalLayout_3);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer);


        retranslateUi(DigStatus);

        QMetaObject::connectSlotsByName(DigStatus);
    } // setupUi

    void retranslateUi(QDialog *DigStatus)
    {
        DigStatus->setWindowTitle(QCoreApplication::translate("DigStatus", "Dig Status", nullptr));
        imgthumblabel->setText(QCoreApplication::translate("DigStatus", "Generating Image Thumbnail: x of y z%", nullptr));
        imgthumbbutton->setText(QCoreApplication::translate("DigStatus", "...", nullptr));
        vidthumblabel->setText(QCoreApplication::translate("DigStatus", "Generating Video Thumbnail: x of y z%", nullptr));
        vidthumbbutton->setText(QCoreApplication::translate("DigStatus", "...", nullptr));
        hashlabel->setText(QCoreApplication::translate("DigStatus", "XXX Hashing: a of b c%", nullptr));
        hashbutton->setText(QCoreApplication::translate("DigStatus", "...", nullptr));
    } // retranslateUi

};

namespace Ui {
    class DigStatus: public Ui_DigStatus {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIGSTATUS_H
