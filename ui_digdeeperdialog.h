/********************************************************************************
** Form generated from reading UI file 'digdeeperdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.5.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIGDEEPERDIALOG_H
#define UI_DIGDEEPERDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_DigDeeperDialog
{
public:
    QVBoxLayout *verticalLayout_2;
    QVBoxLayout *verticalLayout;
    QGroupBox *groupBox_2;
    QVBoxLayout *verticalLayout_3;
    QHBoxLayout *horizontalLayout_2;
    QRadioButton *selectedFileRadioButton;
    QRadioButton *checkedFileRadioButton;
    QRadioButton *listedFileRadioButton;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout_4;
    QCheckBox *md5checkBox;
    QCheckBox *thumbnailcheckBox;
    QSpacerItem *verticalSpacer;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *cancelButton;
    QPushButton *processButton;
    QButtonGroup *exportButtonGroup;
    QButtonGroup *pathButtonGroup;

    void setupUi(QDialog *DigDeeperDialog)
    {
        if (DigDeeperDialog->objectName().isEmpty())
            DigDeeperDialog->setObjectName(QStringLiteral("DigDeeperDialog"));
        DigDeeperDialog->setWindowModality(Qt::ApplicationModal);
        DigDeeperDialog->resize(400, 300);
        DigDeeperDialog->setModal(true);
        verticalLayout_2 = new QVBoxLayout(DigDeeperDialog);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(9, 9, 9, 9);
        groupBox_2 = new QGroupBox(DigDeeperDialog);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        verticalLayout_3 = new QVBoxLayout(groupBox_2);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        selectedFileRadioButton = new QRadioButton(groupBox_2);
        exportButtonGroup = new QButtonGroup(DigDeeperDialog);
        exportButtonGroup->setObjectName(QStringLiteral("exportButtonGroup"));
        exportButtonGroup->addButton(selectedFileRadioButton);
        selectedFileRadioButton->setObjectName(QStringLiteral("selectedFileRadioButton"));

        horizontalLayout_2->addWidget(selectedFileRadioButton);

        checkedFileRadioButton = new QRadioButton(groupBox_2);
        exportButtonGroup->addButton(checkedFileRadioButton);
        checkedFileRadioButton->setObjectName(QStringLiteral("checkedFileRadioButton"));

        horizontalLayout_2->addWidget(checkedFileRadioButton);

        listedFileRadioButton = new QRadioButton(groupBox_2);
        exportButtonGroup->addButton(listedFileRadioButton);
        listedFileRadioButton->setObjectName(QStringLiteral("listedFileRadioButton"));

        horizontalLayout_2->addWidget(listedFileRadioButton);


        verticalLayout_3->addLayout(horizontalLayout_2);


        verticalLayout->addWidget(groupBox_2);

        groupBox = new QGroupBox(DigDeeperDialog);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        verticalLayout_4 = new QVBoxLayout(groupBox);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        md5checkBox = new QCheckBox(groupBox);
        md5checkBox->setObjectName(QStringLiteral("md5checkBox"));

        verticalLayout_4->addWidget(md5checkBox);

        thumbnailcheckBox = new QCheckBox(groupBox);
        thumbnailcheckBox->setObjectName(QStringLiteral("thumbnailcheckBox"));

        verticalLayout_4->addWidget(thumbnailcheckBox);


        verticalLayout->addWidget(groupBox);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(-1, -1, 9, -1);
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        cancelButton = new QPushButton(DigDeeperDialog);
        cancelButton->setObjectName(QStringLiteral("cancelButton"));

        horizontalLayout->addWidget(cancelButton);

        processButton = new QPushButton(DigDeeperDialog);
        processButton->setObjectName(QStringLiteral("processButton"));

        horizontalLayout->addWidget(processButton);


        verticalLayout->addLayout(horizontalLayout);


        verticalLayout_2->addLayout(verticalLayout);


        retranslateUi(DigDeeperDialog);
        QObject::connect(cancelButton, SIGNAL(clicked()), DigDeeperDialog, SLOT(close()));

        QMetaObject::connectSlotsByName(DigDeeperDialog);
    } // setupUi

    void retranslateUi(QDialog *DigDeeperDialog)
    {
        DigDeeperDialog->setWindowTitle(QApplication::translate("DigDeeperDialog", "Dig Deeper", 0));
        groupBox_2->setTitle(QApplication::translate("DigDeeperDialog", "Process the Following File(s)", 0));
        selectedFileRadioButton->setText(QApplication::translate("DigDeeperDialog", "Selected", 0));
        checkedFileRadioButton->setText(QApplication::translate("DigDeeperDialog", "Checked (#)", 0));
        listedFileRadioButton->setText(QApplication::translate("DigDeeperDialog", "Currently Listed (#)", 0));
        groupBox->setTitle(QApplication::translate("DigDeeperDialog", "For the Following Options", 0));
        md5checkBox->setText(QApplication::translate("DigDeeperDialog", "Calculate the MD5 Hash Value", 0));
        thumbnailcheckBox->setText(QApplication::translate("DigDeeperDialog", "Generate Thumbnails for Image Files", 0));
        cancelButton->setText(QApplication::translate("DigDeeperDialog", "Cancel", 0));
        processButton->setText(QApplication::translate("DigDeeperDialog", "Process", 0));
    } // retranslateUi

};

namespace Ui {
    class DigDeeperDialog: public Ui_DigDeeperDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIGDEEPERDIALOG_H
