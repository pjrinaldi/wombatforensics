/********************************************************************************
** Form generated from reading UI file 'digstatus.ui'
**
** Created by: Qt User Interface Compiler version 5.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIGSTATUS_H
#define UI_DIGSTATUS_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
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
            DigStatus->setObjectName(QStringLiteral("DigStatus"));
        DigStatus->setWindowModality(Qt::NonModal);
        DigStatus->resize(320, 294);
        DigStatus->setSizeGripEnabled(true);
        DigStatus->setModal(false);
        verticalLayout_2 = new QVBoxLayout(DigStatus);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        imgthumblabel = new QLabel(DigStatus);
        imgthumblabel->setObjectName(QStringLiteral("imgthumblabel"));

        horizontalLayout->addWidget(imgthumblabel);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        imgthumbbutton = new QToolButton(DigStatus);
        imgthumbbutton->setObjectName(QStringLiteral("imgthumbbutton"));
        QIcon icon;
        icon.addFile(QStringLiteral(":/cancelthread"), QSize(), QIcon::Normal, QIcon::Off);
        imgthumbbutton->setIcon(icon);

        horizontalLayout->addWidget(imgthumbbutton);


        verticalLayout_2->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        hashlabel = new QLabel(DigStatus);
        hashlabel->setObjectName(QStringLiteral("hashlabel"));

        horizontalLayout_2->addWidget(hashlabel);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);

        hashbutton = new QToolButton(DigStatus);
        hashbutton->setObjectName(QStringLiteral("hashbutton"));
        hashbutton->setIcon(icon);

        horizontalLayout_2->addWidget(hashbutton);


        verticalLayout_2->addLayout(horizontalLayout_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        label_3 = new QLabel(DigStatus);
        label_3->setObjectName(QStringLiteral("label_3"));

        horizontalLayout_3->addWidget(label_3);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_3);

        toolButton_3 = new QToolButton(DigStatus);
        toolButton_3->setObjectName(QStringLiteral("toolButton_3"));
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
        DigStatus->setWindowTitle(QApplication::translate("DigStatus", "Dig Status", Q_NULLPTR));
        imgthumblabel->setText(QApplication::translate("DigStatus", "Generating Image Thumbnail: x of y z%", Q_NULLPTR));
        imgthumbbutton->setText(QApplication::translate("DigStatus", "...", Q_NULLPTR));
        hashlabel->setText(QApplication::translate("DigStatus", "XXX Hashing: a of b c%", Q_NULLPTR));
        hashbutton->setText(QApplication::translate("DigStatus", "...", Q_NULLPTR));
        label_3->setText(QApplication::translate("DigStatus", "TextLabel", Q_NULLPTR));
        toolButton_3->setText(QApplication::translate("DigStatus", "...", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class DigStatus: public Ui_DigStatus {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIGSTATUS_H
