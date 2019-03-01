/********************************************************************************
** Form generated from reading UI file 'digstatus.ui'
**
** Created by: Qt User Interface Compiler version 5.12.0
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
    QLabel *hashlabel;
    QSpacerItem *horizontalSpacer_2;
    QToolButton *hashbutton;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_3;
    QSpacerItem *horizontalSpacer_3;
    QToolButton *toolButton_3;
    QSpacerItem *verticalSpacer;

    void setupUi(QDialog *DigStatus)
    {
        if (DigStatus->objectName().isEmpty())
            DigStatus->setObjectName(QString::fromUtf8("DigStatus"));
        DigStatus->setWindowModality(Qt::ApplicationModal);
        DigStatus->resize(320, 294);
        DigStatus->setModal(true);
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
        hashlabel = new QLabel(DigStatus);
        hashlabel->setObjectName(QString::fromUtf8("hashlabel"));

        horizontalLayout_2->addWidget(hashlabel);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);

        hashbutton = new QToolButton(DigStatus);
        hashbutton->setObjectName(QString::fromUtf8("hashbutton"));
        hashbutton->setIcon(icon);

        horizontalLayout_2->addWidget(hashbutton);


        verticalLayout_2->addLayout(horizontalLayout_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        label_3 = new QLabel(DigStatus);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        horizontalLayout_3->addWidget(label_3);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_3);

        toolButton_3 = new QToolButton(DigStatus);
        toolButton_3->setObjectName(QString::fromUtf8("toolButton_3"));
        toolButton_3->setIcon(icon);

        horizontalLayout_3->addWidget(toolButton_3);


        verticalLayout_2->addLayout(horizontalLayout_3);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer);


        retranslateUi(DigStatus);

        QMetaObject::connectSlotsByName(DigStatus);
    } // setupUi

    void retranslateUi(QDialog *DigStatus)
    {
        DigStatus->setWindowTitle(QApplication::translate("DigStatus", "Dig Status", nullptr));
        imgthumblabel->setText(QApplication::translate("DigStatus", "Generating Image Thumbnail: x of y z%", nullptr));
        imgthumbbutton->setText(QApplication::translate("DigStatus", "...", nullptr));
        hashlabel->setText(QApplication::translate("DigStatus", "XXX Hashing: a of b c%", nullptr));
        hashbutton->setText(QApplication::translate("DigStatus", "...", nullptr));
        label_3->setText(QApplication::translate("DigStatus", "TextLabel", nullptr));
        toolButton_3->setText(QApplication::translate("DigStatus", "...", nullptr));
    } // retranslateUi

};

namespace Ui {
    class DigStatus: public Ui_DigStatus {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIGSTATUS_H
