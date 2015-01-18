/********************************************************************************
** Form generated from reading UI file 'idfilter.ui'
**
** Created by: Qt User Interface Compiler version 5.4.0
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
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>

QT_BEGIN_NAMESPACE

class Ui_IdFilter
{
public:
    QGridLayout *gridLayout;
    QCheckBox *morecheckBox;
    QSpinBox *morespinBox;
    QCheckBox *lesscheckBox;
    QSpinBox *lessspinBox;
    QPushButton *pushButton;

    void setupUi(QFrame *IdFilter)
    {
        if (IdFilter->objectName().isEmpty())
            IdFilter->setObjectName(QStringLiteral("IdFilter"));
        IdFilter->setWindowModality(Qt::WindowModal);
        IdFilter->resize(207, 107);
        IdFilter->setAutoFillBackground(true);
        IdFilter->setFrameShape(QFrame::StyledPanel);
        IdFilter->setFrameShadow(QFrame::Raised);
        gridLayout = new QGridLayout(IdFilter);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setSizeConstraint(QLayout::SetFixedSize);
        morecheckBox = new QCheckBox(IdFilter);
        morecheckBox->setObjectName(QStringLiteral("morecheckBox"));
        QFont font;
        font.setPointSize(8);
        morecheckBox->setFont(font);

        gridLayout->addWidget(morecheckBox, 0, 0, 1, 1);

        morespinBox = new QSpinBox(IdFilter);
        morespinBox->setObjectName(QStringLiteral("morespinBox"));
        morespinBox->setFont(font);

        gridLayout->addWidget(morespinBox, 0, 1, 1, 1);

        lesscheckBox = new QCheckBox(IdFilter);
        lesscheckBox->setObjectName(QStringLiteral("lesscheckBox"));
        lesscheckBox->setFont(font);

        gridLayout->addWidget(lesscheckBox, 1, 0, 1, 1);

        lessspinBox = new QSpinBox(IdFilter);
        lessspinBox->setObjectName(QStringLiteral("lessspinBox"));
        lessspinBox->setFont(font);

        gridLayout->addWidget(lessspinBox, 1, 1, 1, 1);

        pushButton = new QPushButton(IdFilter);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        QFont font1;
        font1.setPointSize(8);
        font1.setStrikeOut(false);
        pushButton->setFont(font1);
        pushButton->setCursor(QCursor(Qt::ArrowCursor));
        pushButton->setDefault(true);
        pushButton->setFlat(true);

        gridLayout->addWidget(pushButton, 2, 1, 1, 1);


        retranslateUi(IdFilter);

        QMetaObject::connectSlotsByName(IdFilter);
    } // setupUi

    void retranslateUi(QFrame *IdFilter)
    {
        IdFilter->setWindowTitle(QApplication::translate("IdFilter", "Frame", 0));
        morecheckBox->setText(QApplication::translate("IdFilter", "Show ID's >", 0));
        lesscheckBox->setText(QApplication::translate("IdFilter", "Show ID's < ", 0));
        pushButton->setText(QApplication::translate("IdFilter", "Apply", 0));
    } // retranslateUi

};

namespace Ui {
    class IdFilter: public Ui_IdFilter {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_IDFILTER_H
