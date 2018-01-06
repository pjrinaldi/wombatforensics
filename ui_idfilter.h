/********************************************************************************
** Form generated from reading UI file 'idfilter.ui'
**
** Created by: Qt User Interface Compiler version 5.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_IDFILTER_H
#define UI_IDFILTER_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>

QT_BEGIN_NAMESPACE

class Ui_IdFilter
{
public:
    QGridLayout *gridLayout;
    QCheckBox *echeckBox;
    QSpinBox *espinBox;
    QCheckBox *vcheckBox;
    QSpinBox *vspinBox;
    QCheckBox *pcheckBox;
    QSpinBox *pspinBox;
    QCheckBox *fcheckBox;
    QSpinBox *fspinBox;
    QLabel *label;
    QPushButton *pushButton;
    QLabel *idlabel;

    void setupUi(QFrame *IdFilter)
    {
        if (IdFilter->objectName().isEmpty())
            IdFilter->setObjectName(QStringLiteral("IdFilter"));
        IdFilter->setWindowModality(Qt::WindowModal);
        IdFilter->resize(148, 208);
        IdFilter->setAutoFillBackground(true);
        IdFilter->setFrameShape(QFrame::StyledPanel);
        IdFilter->setFrameShadow(QFrame::Raised);
        gridLayout = new QGridLayout(IdFilter);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setSizeConstraint(QLayout::SetFixedSize);
        echeckBox = new QCheckBox(IdFilter);
        echeckBox->setObjectName(QStringLiteral("echeckBox"));
        QFont font;
        font.setPointSize(8);
        echeckBox->setFont(font);

        gridLayout->addWidget(echeckBox, 2, 0, 1, 1);

        espinBox = new QSpinBox(IdFilter);
        espinBox->setObjectName(QStringLiteral("espinBox"));
        espinBox->setEnabled(false);
        espinBox->setFont(font);
        espinBox->setMaximum(1000000000);
        espinBox->setValue(0);

        gridLayout->addWidget(espinBox, 2, 1, 1, 1);

        vcheckBox = new QCheckBox(IdFilter);
        vcheckBox->setObjectName(QStringLiteral("vcheckBox"));
        vcheckBox->setFont(font);

        gridLayout->addWidget(vcheckBox, 5, 0, 1, 1);

        vspinBox = new QSpinBox(IdFilter);
        vspinBox->setObjectName(QStringLiteral("vspinBox"));
        vspinBox->setEnabled(false);
        vspinBox->setFont(font);
        vspinBox->setMaximum(1000000000);

        gridLayout->addWidget(vspinBox, 5, 1, 1, 1);

        pcheckBox = new QCheckBox(IdFilter);
        pcheckBox->setObjectName(QStringLiteral("pcheckBox"));

        gridLayout->addWidget(pcheckBox, 6, 0, 1, 1);

        pspinBox = new QSpinBox(IdFilter);
        pspinBox->setObjectName(QStringLiteral("pspinBox"));
        pspinBox->setEnabled(false);
        pspinBox->setFont(font);
        pspinBox->setMaximum(1000000000);

        gridLayout->addWidget(pspinBox, 6, 1, 1, 1);

        fcheckBox = new QCheckBox(IdFilter);
        fcheckBox->setObjectName(QStringLiteral("fcheckBox"));

        gridLayout->addWidget(fcheckBox, 7, 0, 1, 1);

        fspinBox = new QSpinBox(IdFilter);
        fspinBox->setObjectName(QStringLiteral("fspinBox"));
        fspinBox->setEnabled(false);
        fspinBox->setFont(font);
        fspinBox->setMaximum(1000000000);

        gridLayout->addWidget(fspinBox, 7, 1, 1, 1);

        label = new QLabel(IdFilter);
        label->setObjectName(QStringLiteral("label"));
        label->setFont(font);
        label->setStyleSheet(QStringLiteral(""));
        label->setFrameShape(QFrame::NoFrame);
        label->setTextFormat(Qt::PlainText);

        gridLayout->addWidget(label, 0, 0, 1, 2);

        pushButton = new QPushButton(IdFilter);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        QFont font1;
        font1.setPointSize(8);
        font1.setStrikeOut(false);
        pushButton->setFont(font1);
        pushButton->setCursor(QCursor(Qt::ArrowCursor));
        pushButton->setStyleSheet(QStringLiteral("border: 1px solid black; padding-top: 5px; padding-bottom: 5px;"));
        pushButton->setFlat(true);

        gridLayout->addWidget(pushButton, 8, 1, 1, 1);

        idlabel = new QLabel(IdFilter);
        idlabel->setObjectName(QStringLiteral("idlabel"));
        QFont font2;
        font2.setFamily(QStringLiteral("Ubuntu"));
        font2.setPointSize(8);
        font2.setBold(true);
        font2.setWeight(75);
        idlabel->setFont(font2);
        idlabel->setStyleSheet(QStringLiteral("padding-top: 5px; padding-bottom: 5px;"));
        idlabel->setTextFormat(Qt::AutoText);
        idlabel->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(idlabel, 1, 0, 1, 2);


        retranslateUi(IdFilter);
        QObject::connect(echeckBox, SIGNAL(toggled(bool)), espinBox, SLOT(setEnabled(bool)));
        QObject::connect(vcheckBox, SIGNAL(toggled(bool)), vspinBox, SLOT(setEnabled(bool)));
        QObject::connect(pcheckBox, SIGNAL(toggled(bool)), pspinBox, SLOT(setEnabled(bool)));
        QObject::connect(fcheckBox, SIGNAL(toggled(bool)), fspinBox, SLOT(setEnabled(bool)));

        pushButton->setDefault(true);


        QMetaObject::connectSlotsByName(IdFilter);
    } // setupUi

    void retranslateUi(QFrame *IdFilter)
    {
        IdFilter->setWindowTitle(QApplication::translate("IdFilter", "Frame", Q_NULLPTR));
        echeckBox->setText(QString());
        espinBox->setSuffix(QString());
        espinBox->setPrefix(QApplication::translate("IdFilter", "e", Q_NULLPTR));
        vcheckBox->setText(QString());
        vspinBox->setPrefix(QApplication::translate("IdFilter", "v", Q_NULLPTR));
        pcheckBox->setText(QString());
        pspinBox->setPrefix(QApplication::translate("IdFilter", "p", Q_NULLPTR));
        fcheckBox->setText(QString());
        fspinBox->setPrefix(QApplication::translate("IdFilter", "f", Q_NULLPTR));
        label->setText(QApplication::translate("IdFilter", "Show ID's that contain:", Q_NULLPTR));
        pushButton->setText(QApplication::translate("IdFilter", "Apply", Q_NULLPTR));
        idlabel->setText(QApplication::translate("IdFilter", "TextLabel", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class IdFilter: public Ui_IdFilter {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_IDFILTER_H
