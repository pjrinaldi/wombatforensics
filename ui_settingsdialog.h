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
    QSpacerItem *verticalSpacer;
    QLabel *casepathlabel;
    QLabel *thumbnaillabel;
    QPushButton *savebutton;
    QPushButton *cancelbutton;
    QLineEdit *reportpathlineedit;
    QSpinBox *thumbnailspinbox;
    QLabel *reportpathlabel;
    QLineEdit *casepathlineedit;
    QLabel *videolabel;
    QSpinBox *videospinbox;

    void setupUi(QDialog *SettingsDialog)
    {
        if (SettingsDialog->objectName().isEmpty())
            SettingsDialog->setObjectName(QString::fromUtf8("SettingsDialog"));
        SettingsDialog->setWindowModality(Qt::ApplicationModal);
        SettingsDialog->resize(407, 194);
        SettingsDialog->setModal(true);
        gridLayout = new QGridLayout(SettingsDialog);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(verticalSpacer, 4, 1, 1, 1);

        casepathlabel = new QLabel(SettingsDialog);
        casepathlabel->setObjectName(QString::fromUtf8("casepathlabel"));

        gridLayout->addWidget(casepathlabel, 2, 0, 1, 1);

        thumbnaillabel = new QLabel(SettingsDialog);
        thumbnaillabel->setObjectName(QString::fromUtf8("thumbnaillabel"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(thumbnaillabel->sizePolicy().hasHeightForWidth());
        thumbnaillabel->setSizePolicy(sizePolicy);
        thumbnaillabel->setScaledContents(false);

        gridLayout->addWidget(thumbnaillabel, 0, 0, 1, 1);

        savebutton = new QPushButton(SettingsDialog);
        savebutton->setObjectName(QString::fromUtf8("savebutton"));

        gridLayout->addWidget(savebutton, 5, 4, 1, 1);

        cancelbutton = new QPushButton(SettingsDialog);
        cancelbutton->setObjectName(QString::fromUtf8("cancelbutton"));

        gridLayout->addWidget(cancelbutton, 5, 3, 1, 1);

        reportpathlineedit = new QLineEdit(SettingsDialog);
        reportpathlineedit->setObjectName(QString::fromUtf8("reportpathlineedit"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(2);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(reportpathlineedit->sizePolicy().hasHeightForWidth());
        reportpathlineedit->setSizePolicy(sizePolicy1);
        reportpathlineedit->setClearButtonEnabled(false);

        gridLayout->addWidget(reportpathlineedit, 3, 1, 1, 4);

        thumbnailspinbox = new QSpinBox(SettingsDialog);
        thumbnailspinbox->setObjectName(QString::fromUtf8("thumbnailspinbox"));
        thumbnailspinbox->setReadOnly(false);
        thumbnailspinbox->setCorrectionMode(QAbstractSpinBox::CorrectToNearestValue);
        thumbnailspinbox->setProperty("showGroupSeparator", QVariant(false));
        thumbnailspinbox->setMinimum(64);
        thumbnailspinbox->setMaximum(320);
        thumbnailspinbox->setSingleStep(64);
        thumbnailspinbox->setValue(64);

        gridLayout->addWidget(thumbnailspinbox, 0, 1, 1, 1);

        reportpathlabel = new QLabel(SettingsDialog);
        reportpathlabel->setObjectName(QString::fromUtf8("reportpathlabel"));

        gridLayout->addWidget(reportpathlabel, 3, 0, 1, 1);

        casepathlineedit = new QLineEdit(SettingsDialog);
        casepathlineedit->setObjectName(QString::fromUtf8("casepathlineedit"));
        sizePolicy1.setHeightForWidth(casepathlineedit->sizePolicy().hasHeightForWidth());
        casepathlineedit->setSizePolicy(sizePolicy1);

        gridLayout->addWidget(casepathlineedit, 2, 1, 1, 4);

        videolabel = new QLabel(SettingsDialog);
        videolabel->setObjectName(QString::fromUtf8("videolabel"));

        gridLayout->addWidget(videolabel, 1, 0, 1, 2);

        videospinbox = new QSpinBox(SettingsDialog);
        videospinbox->setObjectName(QString::fromUtf8("videospinbox"));
        videospinbox->setReadOnly(false);
        videospinbox->setButtonSymbols(QAbstractSpinBox::UpDownArrows);
        videospinbox->setCorrectionMode(QAbstractSpinBox::CorrectToNearestValue);
        videospinbox->setMinimum(10);
        videospinbox->setMaximum(100);
        videospinbox->setSingleStep(10);

        gridLayout->addWidget(videospinbox, 1, 3, 1, 1);


        retranslateUi(SettingsDialog);

        savebutton->setDefault(true);


        QMetaObject::connectSlotsByName(SettingsDialog);
    } // setupUi

    void retranslateUi(QDialog *SettingsDialog)
    {
        SettingsDialog->setWindowTitle(QCoreApplication::translate("SettingsDialog", "Settings", nullptr));
        casepathlabel->setText(QCoreApplication::translate("SettingsDialog", "Set Case Path:", nullptr));
        thumbnaillabel->setText(QCoreApplication::translate("SettingsDialog", "Thumbnail Size:", nullptr));
        savebutton->setText(QCoreApplication::translate("SettingsDialog", "Save", nullptr));
        cancelbutton->setText(QCoreApplication::translate("SettingsDialog", "Cancel", nullptr));
        reportpathlineedit->setPlaceholderText(QCoreApplication::translate("SettingsDialog", "User's Home Directory", nullptr));
        thumbnailspinbox->setSuffix(QCoreApplication::translate("SettingsDialog", " pixels", nullptr));
        reportpathlabel->setText(QCoreApplication::translate("SettingsDialog", "Set Report Folder:", nullptr));
        casepathlineedit->setPlaceholderText(QCoreApplication::translate("SettingsDialog", "User's Home Directory", nullptr));
        videolabel->setText(QCoreApplication::translate("SettingsDialog", "Generate Video Thumbnail Every", nullptr));
        videospinbox->setSuffix(QCoreApplication::translate("SettingsDialog", " %", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SettingsDialog: public Ui_SettingsDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SETTINGSDIALOG_H
