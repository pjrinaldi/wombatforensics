/********************************************************************************
** Form generated from reading UI file 'settingsdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.13.0
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
    QLabel *casepathlabel;
    QLineEdit *casepathlineedit;
    QLabel *thumbnaillabel;
    QPushButton *cancelbutton;
    QSpacerItem *verticalSpacer;
    QPushButton *savebutton;
    QSpinBox *thumbnailspinbox;
    QLabel *reportpathlabel;
    QLineEdit *reportpathlineedit;

    void setupUi(QDialog *SettingsDialog)
    {
        if (SettingsDialog->objectName().isEmpty())
            SettingsDialog->setObjectName(QString::fromUtf8("SettingsDialog"));
        SettingsDialog->setWindowModality(Qt::ApplicationModal);
        SettingsDialog->resize(407, 166);
        SettingsDialog->setModal(true);
        gridLayout = new QGridLayout(SettingsDialog);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        casepathlabel = new QLabel(SettingsDialog);
        casepathlabel->setObjectName(QString::fromUtf8("casepathlabel"));

        gridLayout->addWidget(casepathlabel, 1, 0, 1, 1);

        casepathlineedit = new QLineEdit(SettingsDialog);
        casepathlineedit->setObjectName(QString::fromUtf8("casepathlineedit"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(2);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(casepathlineedit->sizePolicy().hasHeightForWidth());
        casepathlineedit->setSizePolicy(sizePolicy);

        gridLayout->addWidget(casepathlineedit, 1, 1, 1, 4);

        thumbnaillabel = new QLabel(SettingsDialog);
        thumbnaillabel->setObjectName(QString::fromUtf8("thumbnaillabel"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(thumbnaillabel->sizePolicy().hasHeightForWidth());
        thumbnaillabel->setSizePolicy(sizePolicy1);
        thumbnaillabel->setScaledContents(false);

        gridLayout->addWidget(thumbnaillabel, 0, 0, 1, 1);

        cancelbutton = new QPushButton(SettingsDialog);
        cancelbutton->setObjectName(QString::fromUtf8("cancelbutton"));

        gridLayout->addWidget(cancelbutton, 4, 3, 1, 1);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(verticalSpacer, 3, 1, 1, 1);

        savebutton = new QPushButton(SettingsDialog);
        savebutton->setObjectName(QString::fromUtf8("savebutton"));

        gridLayout->addWidget(savebutton, 4, 4, 1, 1);

        thumbnailspinbox = new QSpinBox(SettingsDialog);
        thumbnailspinbox->setObjectName(QString::fromUtf8("thumbnailspinbox"));
        thumbnailspinbox->setReadOnly(false);
        thumbnailspinbox->setProperty("showGroupSeparator", QVariant(false));
        thumbnailspinbox->setMinimum(64);
        thumbnailspinbox->setMaximum(320);
        thumbnailspinbox->setSingleStep(64);
        thumbnailspinbox->setValue(64);

        gridLayout->addWidget(thumbnailspinbox, 0, 1, 1, 1);

        reportpathlabel = new QLabel(SettingsDialog);
        reportpathlabel->setObjectName(QString::fromUtf8("reportpathlabel"));

        gridLayout->addWidget(reportpathlabel, 2, 0, 1, 1);

        reportpathlineedit = new QLineEdit(SettingsDialog);
        reportpathlineedit->setObjectName(QString::fromUtf8("reportpathlineedit"));
        sizePolicy.setHeightForWidth(reportpathlineedit->sizePolicy().hasHeightForWidth());
        reportpathlineedit->setSizePolicy(sizePolicy);
        reportpathlineedit->setClearButtonEnabled(false);

        gridLayout->addWidget(reportpathlineedit, 2, 1, 1, 4);


        retranslateUi(SettingsDialog);

        savebutton->setDefault(true);


        QMetaObject::connectSlotsByName(SettingsDialog);
    } // setupUi

    void retranslateUi(QDialog *SettingsDialog)
    {
        SettingsDialog->setWindowTitle(QCoreApplication::translate("SettingsDialog", "Settings", nullptr));
        casepathlabel->setText(QCoreApplication::translate("SettingsDialog", "Set Case Path:", nullptr));
        casepathlineedit->setPlaceholderText(QCoreApplication::translate("SettingsDialog", "User's Home Directory", nullptr));
        thumbnaillabel->setText(QCoreApplication::translate("SettingsDialog", "Thumbnail Size:", nullptr));
        cancelbutton->setText(QCoreApplication::translate("SettingsDialog", "Cancel", nullptr));
        savebutton->setText(QCoreApplication::translate("SettingsDialog", "Save", nullptr));
        thumbnailspinbox->setSuffix(QCoreApplication::translate("SettingsDialog", " pixels", nullptr));
        reportpathlabel->setText(QCoreApplication::translate("SettingsDialog", "Set Report Folder:", nullptr));
        reportpathlineedit->setPlaceholderText(QCoreApplication::translate("SettingsDialog", "User's Home Directory", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SettingsDialog: public Ui_SettingsDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SETTINGSDIALOG_H
