/********************************************************************************
** Form generated from reading UI file 'settingsdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SETTINGSDIALOG_H
#define UI_SETTINGSDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
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
    QSpinBox *videospinbox;
    QLabel *timezonelabel;
    QComboBox *timezonecombobox;
    QLabel *autosavelabel;
    QPushButton *reportpathbutton;
    QPushButton *savebutton;
    QPushButton *casepathbutton;
    QLineEdit *reportpathlineedit;
    QLabel *casepathlabel;
    QPushButton *cancelbutton;
    QSpinBox *autosavespinbox;
    QLineEdit *casepathlineedit;
    QLabel *thumbnaillabel;
    QSpacerItem *verticalSpacer;
    QLabel *videolabel;
    QLabel *reportpathlabel;
    QSpinBox *thumbnailspinbox;
    QLabel *carvingfiletypelabel;
    QPushButton *carvingfiletypepushbutton;

    void setupUi(QDialog *SettingsDialog)
    {
        if (SettingsDialog->objectName().isEmpty())
            SettingsDialog->setObjectName(QString::fromUtf8("SettingsDialog"));
        SettingsDialog->setWindowModality(Qt::ApplicationModal);
        SettingsDialog->resize(507, 289);
        SettingsDialog->setModal(true);
        gridLayout = new QGridLayout(SettingsDialog);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        videospinbox = new QSpinBox(SettingsDialog);
        videospinbox->setObjectName(QString::fromUtf8("videospinbox"));
        videospinbox->setReadOnly(false);
        videospinbox->setButtonSymbols(QAbstractSpinBox::UpDownArrows);
        videospinbox->setCorrectionMode(QAbstractSpinBox::CorrectToNearestValue);
        videospinbox->setMinimum(10);
        videospinbox->setMaximum(100);
        videospinbox->setSingleStep(5);

        gridLayout->addWidget(videospinbox, 1, 4, 1, 1);

        timezonelabel = new QLabel(SettingsDialog);
        timezonelabel->setObjectName(QString::fromUtf8("timezonelabel"));

        gridLayout->addWidget(timezonelabel, 4, 0, 1, 1);

        timezonecombobox = new QComboBox(SettingsDialog);
        timezonecombobox->setObjectName(QString::fromUtf8("timezonecombobox"));

        gridLayout->addWidget(timezonecombobox, 4, 1, 1, 4);

        autosavelabel = new QLabel(SettingsDialog);
        autosavelabel->setObjectName(QString::fromUtf8("autosavelabel"));

        gridLayout->addWidget(autosavelabel, 5, 0, 1, 3);

        reportpathbutton = new QPushButton(SettingsDialog);
        reportpathbutton->setObjectName(QString::fromUtf8("reportpathbutton"));

        gridLayout->addWidget(reportpathbutton, 3, 4, 1, 1);

        savebutton = new QPushButton(SettingsDialog);
        savebutton->setObjectName(QString::fromUtf8("savebutton"));

        gridLayout->addWidget(savebutton, 8, 4, 1, 1);

        casepathbutton = new QPushButton(SettingsDialog);
        casepathbutton->setObjectName(QString::fromUtf8("casepathbutton"));
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(casepathbutton->sizePolicy().hasHeightForWidth());
        casepathbutton->setSizePolicy(sizePolicy);

        gridLayout->addWidget(casepathbutton, 2, 4, 1, 1);

        reportpathlineedit = new QLineEdit(SettingsDialog);
        reportpathlineedit->setObjectName(QString::fromUtf8("reportpathlineedit"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(2);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(reportpathlineedit->sizePolicy().hasHeightForWidth());
        reportpathlineedit->setSizePolicy(sizePolicy1);
        reportpathlineedit->setClearButtonEnabled(false);

        gridLayout->addWidget(reportpathlineedit, 3, 1, 1, 3);

        casepathlabel = new QLabel(SettingsDialog);
        casepathlabel->setObjectName(QString::fromUtf8("casepathlabel"));

        gridLayout->addWidget(casepathlabel, 2, 0, 1, 1);

        cancelbutton = new QPushButton(SettingsDialog);
        cancelbutton->setObjectName(QString::fromUtf8("cancelbutton"));

        gridLayout->addWidget(cancelbutton, 8, 3, 1, 1);

        autosavespinbox = new QSpinBox(SettingsDialog);
        autosavespinbox->setObjectName(QString::fromUtf8("autosavespinbox"));
        autosavespinbox->setMinimum(1);
        autosavespinbox->setMaximum(120);
        autosavespinbox->setSingleStep(1);
        autosavespinbox->setValue(20);

        gridLayout->addWidget(autosavespinbox, 5, 4, 1, 1);

        casepathlineedit = new QLineEdit(SettingsDialog);
        casepathlineedit->setObjectName(QString::fromUtf8("casepathlineedit"));
        sizePolicy1.setHeightForWidth(casepathlineedit->sizePolicy().hasHeightForWidth());
        casepathlineedit->setSizePolicy(sizePolicy1);

        gridLayout->addWidget(casepathlineedit, 2, 1, 1, 3);

        thumbnaillabel = new QLabel(SettingsDialog);
        thumbnaillabel->setObjectName(QString::fromUtf8("thumbnaillabel"));
        QSizePolicy sizePolicy2(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(thumbnaillabel->sizePolicy().hasHeightForWidth());
        thumbnaillabel->setSizePolicy(sizePolicy2);
        thumbnaillabel->setScaledContents(false);

        gridLayout->addWidget(thumbnaillabel, 0, 0, 1, 2);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(verticalSpacer, 7, 1, 1, 1);

        videolabel = new QLabel(SettingsDialog);
        videolabel->setObjectName(QString::fromUtf8("videolabel"));

        gridLayout->addWidget(videolabel, 1, 0, 1, 4);

        reportpathlabel = new QLabel(SettingsDialog);
        reportpathlabel->setObjectName(QString::fromUtf8("reportpathlabel"));

        gridLayout->addWidget(reportpathlabel, 3, 0, 1, 1);

        thumbnailspinbox = new QSpinBox(SettingsDialog);
        thumbnailspinbox->setObjectName(QString::fromUtf8("thumbnailspinbox"));
        thumbnailspinbox->setReadOnly(false);
        thumbnailspinbox->setCorrectionMode(QAbstractSpinBox::CorrectToNearestValue);
        thumbnailspinbox->setProperty("showGroupSeparator", QVariant(false));
        thumbnailspinbox->setMinimum(64);
        thumbnailspinbox->setMaximum(384);
        thumbnailspinbox->setSingleStep(64);
        thumbnailspinbox->setValue(128);

        gridLayout->addWidget(thumbnailspinbox, 0, 4, 1, 1);

        carvingfiletypelabel = new QLabel(SettingsDialog);
        carvingfiletypelabel->setObjectName(QString::fromUtf8("carvingfiletypelabel"));

        gridLayout->addWidget(carvingfiletypelabel, 6, 0, 1, 2);

        carvingfiletypepushbutton = new QPushButton(SettingsDialog);
        carvingfiletypepushbutton->setObjectName(QString::fromUtf8("carvingfiletypepushbutton"));

        gridLayout->addWidget(carvingfiletypepushbutton, 6, 4, 1, 1);


        retranslateUi(SettingsDialog);

        savebutton->setDefault(true);


        QMetaObject::connectSlotsByName(SettingsDialog);
    } // setupUi

    void retranslateUi(QDialog *SettingsDialog)
    {
        SettingsDialog->setWindowTitle(QCoreApplication::translate("SettingsDialog", "Settings", nullptr));
        videospinbox->setSuffix(QCoreApplication::translate("SettingsDialog", " %", nullptr));
        timezonelabel->setText(QCoreApplication::translate("SettingsDialog", "Reporting Time Zone:", nullptr));
#if QT_CONFIG(tooltip)
        autosavelabel->setToolTip(QCoreApplication::translate("SettingsDialog", "When Saving, removes tmpfiles, stores checked and selected states", nullptr));
#endif // QT_CONFIG(tooltip)
        autosavelabel->setText(QCoreApplication::translate("SettingsDialog", "Set AutoSave Interval:", nullptr));
        reportpathbutton->setText(QCoreApplication::translate("SettingsDialog", "Browse", nullptr));
        savebutton->setText(QCoreApplication::translate("SettingsDialog", "Save", nullptr));
        casepathbutton->setText(QCoreApplication::translate("SettingsDialog", "Browse", nullptr));
        reportpathlineedit->setPlaceholderText(QCoreApplication::translate("SettingsDialog", "User's Home Directory", nullptr));
        casepathlabel->setText(QCoreApplication::translate("SettingsDialog", "Set Case Path:", nullptr));
        cancelbutton->setText(QCoreApplication::translate("SettingsDialog", "Cancel", nullptr));
#if QT_CONFIG(tooltip)
        autosavespinbox->setToolTip(QCoreApplication::translate("SettingsDialog", "Interval to Remove temp files and save checked and selected states", nullptr));
#endif // QT_CONFIG(tooltip)
        autosavespinbox->setSuffix(QCoreApplication::translate("SettingsDialog", " minutes", nullptr));
        casepathlineedit->setPlaceholderText(QCoreApplication::translate("SettingsDialog", "User's Home Directory", nullptr));
        thumbnaillabel->setText(QCoreApplication::translate("SettingsDialog", "IMG/VID Thumbnail Size:", nullptr));
        videolabel->setText(QCoreApplication::translate("SettingsDialog", "Generate Video Thumbnail Every:", nullptr));
        reportpathlabel->setText(QCoreApplication::translate("SettingsDialog", "Set Report Folder:", nullptr));
        thumbnailspinbox->setSuffix(QCoreApplication::translate("SettingsDialog", " pixels", nullptr));
        carvingfiletypelabel->setText(QCoreApplication::translate("SettingsDialog", "Manage Carving File Types List", nullptr));
        carvingfiletypepushbutton->setText(QCoreApplication::translate("SettingsDialog", "Manage", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SettingsDialog: public Ui_SettingsDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SETTINGSDIALOG_H
