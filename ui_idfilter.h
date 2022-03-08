/********************************************************************************
** Form generated from reading UI file 'idfilter.ui'
**
** Created by: Qt User Interface Compiler version 5.15.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_IDFILTER_H
#define UI_IDFILTER_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>

QT_BEGIN_NAMESPACE

class Ui_IdFilter
{
public:
    QGridLayout *gridLayout;
    QCheckBox *echeckBox;
    QLabel *idlabel;
    QSpinBox *fspinBox;
    QSpinBox *espinBox;
    QCheckBox *fcheckBox;
    QCheckBox *vcheckBox;
    QSpinBox *pspinBox;
    QLabel *label;
    QCheckBox *pcheckBox;
    QSpinBox *vspinBox;
    QPushButton *pushButton;
    QPushButton *pushButton_2;

    void setupUi(QDialog *IdFilter)
    {
        if (IdFilter->objectName().isEmpty())
            IdFilter->setObjectName(QString::fromUtf8("IdFilter"));
        IdFilter->resize(146, 243);
        gridLayout = new QGridLayout(IdFilter);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        echeckBox = new QCheckBox(IdFilter);
        echeckBox->setObjectName(QString::fromUtf8("echeckBox"));
        QFont font;
        font.setPointSize(8);
        echeckBox->setFont(font);

        gridLayout->addWidget(echeckBox, 2, 0, 1, 1);

        idlabel = new QLabel(IdFilter);
        idlabel->setObjectName(QString::fromUtf8("idlabel"));
        QFont font1;
        font1.setFamily(QString::fromUtf8("Ubuntu"));
        font1.setPointSize(8);
        font1.setBold(true);
        font1.setWeight(75);
        idlabel->setFont(font1);
        idlabel->setStyleSheet(QString::fromUtf8("padding-top: 5px; padding-bottom: 5px;"));
        idlabel->setTextFormat(Qt::AutoText);
        idlabel->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(idlabel, 1, 0, 1, 2);

        fspinBox = new QSpinBox(IdFilter);
        fspinBox->setObjectName(QString::fromUtf8("fspinBox"));
        fspinBox->setEnabled(false);
        fspinBox->setFont(font);
        fspinBox->setMaximum(1000000000);

        gridLayout->addWidget(fspinBox, 5, 1, 1, 1);

        espinBox = new QSpinBox(IdFilter);
        espinBox->setObjectName(QString::fromUtf8("espinBox"));
        espinBox->setEnabled(false);
        espinBox->setFont(font);
        espinBox->setMaximum(1000000000);
        espinBox->setValue(0);

        gridLayout->addWidget(espinBox, 2, 1, 1, 1);

        fcheckBox = new QCheckBox(IdFilter);
        fcheckBox->setObjectName(QString::fromUtf8("fcheckBox"));

        gridLayout->addWidget(fcheckBox, 5, 0, 1, 1);

        vcheckBox = new QCheckBox(IdFilter);
        vcheckBox->setObjectName(QString::fromUtf8("vcheckBox"));
        vcheckBox->setFont(font);

        gridLayout->addWidget(vcheckBox, 3, 0, 1, 1);

        pspinBox = new QSpinBox(IdFilter);
        pspinBox->setObjectName(QString::fromUtf8("pspinBox"));
        pspinBox->setEnabled(false);
        pspinBox->setFont(font);
        pspinBox->setMaximum(1000000000);

        gridLayout->addWidget(pspinBox, 4, 1, 1, 1);

        label = new QLabel(IdFilter);
        label->setObjectName(QString::fromUtf8("label"));
        label->setFont(font);
        label->setStyleSheet(QString::fromUtf8(""));
        label->setFrameShape(QFrame::NoFrame);
        label->setTextFormat(Qt::PlainText);

        gridLayout->addWidget(label, 0, 0, 1, 2);

        pcheckBox = new QCheckBox(IdFilter);
        pcheckBox->setObjectName(QString::fromUtf8("pcheckBox"));

        gridLayout->addWidget(pcheckBox, 4, 0, 1, 1);

        vspinBox = new QSpinBox(IdFilter);
        vspinBox->setObjectName(QString::fromUtf8("vspinBox"));
        vspinBox->setEnabled(false);
        vspinBox->setFont(font);
        vspinBox->setMaximum(1000000000);

        gridLayout->addWidget(vspinBox, 3, 1, 1, 1);

        pushButton = new QPushButton(IdFilter);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        QFont font2;
        font2.setPointSize(8);
        font2.setStrikeOut(false);
        pushButton->setFont(font2);
        pushButton->setCursor(QCursor(Qt::ArrowCursor));
        pushButton->setStyleSheet(QString::fromUtf8(""));
        pushButton->setFlat(false);

        gridLayout->addWidget(pushButton, 6, 1, 1, 1);

        pushButton_2 = new QPushButton(IdFilter);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));
        pushButton_2->setStyleSheet(QString::fromUtf8(""));
        pushButton_2->setFlat(false);

        gridLayout->addWidget(pushButton_2, 7, 1, 1, 1);


        retranslateUi(IdFilter);

        pushButton->setDefault(true);


        QMetaObject::connectSlotsByName(IdFilter);
    } // setupUi

    void retranslateUi(QDialog *IdFilter)
    {
        IdFilter->setWindowTitle(QCoreApplication::translate("IdFilter", "ID Filter", nullptr));
        echeckBox->setText(QString());
        idlabel->setText(QCoreApplication::translate("IdFilter", "TextLabel", nullptr));
        fspinBox->setPrefix(QCoreApplication::translate("IdFilter", "f", nullptr));
        espinBox->setSuffix(QString());
        espinBox->setPrefix(QCoreApplication::translate("IdFilter", "e", nullptr));
        fcheckBox->setText(QString());
        vcheckBox->setText(QString());
        pspinBox->setPrefix(QCoreApplication::translate("IdFilter", "p", nullptr));
        label->setText(QCoreApplication::translate("IdFilter", "Show ID's that contain:", nullptr));
        pcheckBox->setText(QString());
        vspinBox->setPrefix(QCoreApplication::translate("IdFilter", "v", nullptr));
        pushButton->setText(QCoreApplication::translate("IdFilter", "Apply", nullptr));
        pushButton_2->setText(QCoreApplication::translate("IdFilter", "Cancel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class IdFilter: public Ui_IdFilter {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_IDFILTER_H
