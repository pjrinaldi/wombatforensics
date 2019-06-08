/********************************************************************************
** Form generated from reading UI file 'settingsdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.12.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SETTINGSDIALOG_H
#define UI_SETTINGSDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>

QT_BEGIN_NAMESPACE

class Ui_SettingsDialog
{
public:
    QGridLayout *gridLayout;
    QPushButton *cancelbutton;
    QPushButton *savebutton;
    QLabel *thumbnaillabel;
    QSpacerItem *verticalSpacer;
    QSpinBox *thumbnailspinbox;
    QLabel *casepathlabel;
    QLineEdit *casepathlineedit;

    void setupUi(QDialog *SettingsDialog)
    {
        if (SettingsDialog->objectName().isEmpty())
            SettingsDialog->setObjectName(QString::fromUtf8("SettingsDialog"));
        SettingsDialog->setWindowModality(Qt::ApplicationModal);
        SettingsDialog->resize(393, 115);
        SettingsDialog->setModal(true);
        gridLayout = new QGridLayout(SettingsDialog);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        cancelbutton = new QPushButton(SettingsDialog);
        cancelbutton->setObjectName(QString::fromUtf8("cancelbutton"));

        gridLayout->addWidget(cancelbutton, 3, 3, 1, 1);

        savebutton = new QPushButton(SettingsDialog);
        savebutton->setObjectName(QString::fromUtf8("savebutton"));

        gridLayout->addWidget(savebutton, 3, 4, 1, 1);

        thumbnaillabel = new QLabel(SettingsDialog);
        thumbnaillabel->setObjectName(QString::fromUtf8("thumbnaillabel"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(thumbnaillabel->sizePolicy().hasHeightForWidth());
        thumbnaillabel->setSizePolicy(sizePolicy);
        thumbnaillabel->setScaledContents(false);

        gridLayout->addWidget(thumbnaillabel, 0, 0, 1, 1);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(verticalSpacer, 2, 1, 1, 1);

        thumbnailspinbox = new QSpinBox(SettingsDialog);
        thumbnailspinbox->setObjectName(QString::fromUtf8("thumbnailspinbox"));
        thumbnailspinbox->setReadOnly(false);
        thumbnailspinbox->setProperty("showGroupSeparator", QVariant(false));
        thumbnailspinbox->setMinimum(64);
        thumbnailspinbox->setMaximum(320);
        thumbnailspinbox->setSingleStep(64);
        thumbnailspinbox->setValue(64);

        gridLayout->addWidget(thumbnailspinbox, 0, 1, 1, 1);

        casepathlabel = new QLabel(SettingsDialog);
        casepathlabel->setObjectName(QString::fromUtf8("casepathlabel"));

        gridLayout->addWidget(casepathlabel, 1, 0, 1, 1);

        casepathlineedit = new QLineEdit(SettingsDialog);
        casepathlineedit->setObjectName(QString::fromUtf8("casepathlineedit"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(2);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(casepathlineedit->sizePolicy().hasHeightForWidth());
        casepathlineedit->setSizePolicy(sizePolicy1);

        gridLayout->addWidget(casepathlineedit, 1, 1, 1, 4);


        retranslateUi(SettingsDialog);

        savebutton->setDefault(true);


        QMetaObject::connectSlotsByName(SettingsDialog);
    } // setupUi

    void retranslateUi(QDialog *SettingsDialog)
    {
        SettingsDialog->setWindowTitle(QApplication::translate("SettingsDialog", "Settings", nullptr));
        cancelbutton->setText(QApplication::translate("SettingsDialog", "Cancel", nullptr));
        savebutton->setText(QApplication::translate("SettingsDialog", "Save", nullptr));
        thumbnaillabel->setText(QApplication::translate("SettingsDialog", "Thumbnail Size:", nullptr));
        thumbnailspinbox->setSuffix(QApplication::translate("SettingsDialog", " pixels", nullptr));
        casepathlabel->setText(QApplication::translate("SettingsDialog", "Set Case Path:", nullptr));
        casepathlineedit->setPlaceholderText(QApplication::translate("SettingsDialog", "User's Home Directory", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SettingsDialog: public Ui_SettingsDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SETTINGSDIALOG_H
