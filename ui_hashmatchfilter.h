/********************************************************************************
** Form generated from reading UI file 'hashmatchfilter.ui'
**
** Created by: Qt User Interface Compiler version 5.15.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_HASHMATCHFILTER_H
#define UI_HASHMATCHFILTER_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_HashMatchFilter
{
public:
    QGridLayout *gridLayout;
    QCheckBox *hashcheckbox;
    QComboBox *hashcombobox;
    QPushButton *cancelbutton;
    QPushButton *applybutton;

    void setupUi(QDialog *HashMatchFilter)
    {
        if (HashMatchFilter->objectName().isEmpty())
            HashMatchFilter->setObjectName(QString::fromUtf8("HashMatchFilter"));
        HashMatchFilter->setWindowModality(Qt::ApplicationModal);
        HashMatchFilter->resize(447, 73);
        gridLayout = new QGridLayout(HashMatchFilter);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        hashcheckbox = new QCheckBox(HashMatchFilter);
        hashcheckbox->setObjectName(QString::fromUtf8("hashcheckbox"));

        gridLayout->addWidget(hashcheckbox, 0, 0, 1, 1);

        hashcombobox = new QComboBox(HashMatchFilter);
        hashcombobox->setObjectName(QString::fromUtf8("hashcombobox"));
        hashcombobox->setEnabled(false);

        gridLayout->addWidget(hashcombobox, 0, 1, 1, 1);

        cancelbutton = new QPushButton(HashMatchFilter);
        cancelbutton->setObjectName(QString::fromUtf8("cancelbutton"));

        gridLayout->addWidget(cancelbutton, 1, 0, 1, 1);

        applybutton = new QPushButton(HashMatchFilter);
        applybutton->setObjectName(QString::fromUtf8("applybutton"));

        gridLayout->addWidget(applybutton, 1, 1, 1, 1);


        retranslateUi(HashMatchFilter);

        QMetaObject::connectSlotsByName(HashMatchFilter);
    } // setupUi

    void retranslateUi(QDialog *HashMatchFilter)
    {
        HashMatchFilter->setWindowTitle(QCoreApplication::translate("HashMatchFilter", "Dialog", nullptr));
        hashcheckbox->setText(QCoreApplication::translate("HashMatchFilter", "Show Items That Match Hash List", nullptr));
        cancelbutton->setText(QCoreApplication::translate("HashMatchFilter", "Cancel", nullptr));
        applybutton->setText(QCoreApplication::translate("HashMatchFilter", "Apply", nullptr));
    } // retranslateUi

};

namespace Ui {
    class HashMatchFilter: public Ui_HashMatchFilter {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_HASHMATCHFILTER_H
