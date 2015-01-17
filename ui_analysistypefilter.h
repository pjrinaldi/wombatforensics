/********************************************************************************
** Form generated from reading UI file 'analysistypefilter.ui'
**
** Created by: Qt User Interface Compiler version 5.4.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ANALYSISTYPEFILTER_H
#define UI_ANALYSISTYPEFILTER_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_analysistypefilter
{
public:
    QGridLayout *gridLayout;
    QCheckBox *checkBox;
    QComboBox *comboBox;

    void setupUi(QWidget *analysistypefilter)
    {
        if (analysistypefilter->objectName().isEmpty())
            analysistypefilter->setObjectName(QStringLiteral("analysistypefilter"));
        analysistypefilter->setWindowModality(Qt::ApplicationModal);
        analysistypefilter->resize(442, 42);
        QFont font;
        font.setPointSize(8);
        analysistypefilter->setFont(font);
        gridLayout = new QGridLayout(analysistypefilter);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setHorizontalSpacing(0);
        checkBox = new QCheckBox(analysistypefilter);
        checkBox->setObjectName(QStringLiteral("checkBox"));

        gridLayout->addWidget(checkBox, 0, 0, 1, 1);

        comboBox = new QComboBox(analysistypefilter);
        comboBox->setObjectName(QStringLiteral("comboBox"));
        comboBox->setEditable(false);

        gridLayout->addWidget(comboBox, 0, 1, 1, 1);


        retranslateUi(analysistypefilter);
        QObject::connect(checkBox, SIGNAL(toggled(bool)), comboBox, SLOT(setEnabled(bool)));

        QMetaObject::connectSlotsByName(analysistypefilter);
    } // setupUi

    void retranslateUi(QWidget *analysistypefilter)
    {
        analysistypefilter->setWindowTitle(QApplication::translate("analysistypefilter", "Filter", 0));
        checkBox->setText(QApplication::translate("analysistypefilter", "Show Items where analysis type matches", 0));
        comboBox->setCurrentText(QString());
    } // retranslateUi

};

namespace Ui {
    class analysistypefilter: public Ui_analysistypefilter {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ANALYSISTYPEFILTER_H
