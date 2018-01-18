/********************************************************************************
** Form generated from reading UI file 'digdeeperdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.9.1
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
    QCheckBox *thumbnailcheckBox;
    QSpacerItem *verticalSpacer;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *cancelButton;
    QPushButton *processButton;
    QButtonGroup *exportButtonGroup;

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
        DigDeeperDialog->setWindowTitle(QApplication::translate("DigDeeperDialog", "Dig Deeper", Q_NULLPTR));
        groupBox_2->setTitle(QApplication::translate("DigDeeperDialog", "Process the Following File(s)", Q_NULLPTR));
        selectedFileRadioButton->setText(QApplication::translate("DigDeeperDialog", "Selected", Q_NULLPTR));
        checkedFileRadioButton->setText(QApplication::translate("DigDeeperDialog", "Checked (#)", Q_NULLPTR));
        listedFileRadioButton->setText(QApplication::translate("DigDeeperDialog", "Currently Listed (#)", Q_NULLPTR));
        groupBox->setTitle(QApplication::translate("DigDeeperDialog", "For the Following Options", Q_NULLPTR));
        thumbnailcheckBox->setText(QApplication::translate("DigDeeperDialog", "Generate Thumbnails for All Image Files", Q_NULLPTR));
        cancelButton->setText(QApplication::translate("DigDeeperDialog", "Cancel", Q_NULLPTR));
        processButton->setText(QApplication::translate("DigDeeperDialog", "Process", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class DigDeeperDialog: public Ui_DigDeeperDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIGDEEPERDIALOG_H
