/********************************************************************************
** Form generated from reading UI file 'digdeeperdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.15.8
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIGDEEPERDIALOG_H
#define UI_DIGDEEPERDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QListWidget>
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
    QHBoxLayout *horizontalLayout_4;
    QCheckBox *expandarchivescheckbox;
    QCheckBox *emailcheckbox;
    QCheckBox *thumbnailcheckBox;
    QCheckBox *videocheckBox;
    QHBoxLayout *horizontalLayout_3;
    QCheckBox *hashcheckbox;
    QHBoxLayout *horizontalLayout_5;
    QCheckBox *hashlistcheckbox;
    QListWidget *hashlistwidget;
    QSpacerItem *horizontalSpacer_2;
    QSpacerItem *verticalSpacer;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *cancelButton;
    QPushButton *processButton;
    QButtonGroup *exportButtonGroup;

    void setupUi(QDialog *DigDeeperDialog)
    {
        if (DigDeeperDialog->objectName().isEmpty())
            DigDeeperDialog->setObjectName(QString::fromUtf8("DigDeeperDialog"));
        DigDeeperDialog->setWindowModality(Qt::ApplicationModal);
        DigDeeperDialog->resize(516, 397);
        DigDeeperDialog->setModal(true);
        verticalLayout_2 = new QVBoxLayout(DigDeeperDialog);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(9, 9, 9, 9);
        groupBox_2 = new QGroupBox(DigDeeperDialog);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        verticalLayout_3 = new QVBoxLayout(groupBox_2);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        selectedFileRadioButton = new QRadioButton(groupBox_2);
        exportButtonGroup = new QButtonGroup(DigDeeperDialog);
        exportButtonGroup->setObjectName(QString::fromUtf8("exportButtonGroup"));
        exportButtonGroup->addButton(selectedFileRadioButton);
        selectedFileRadioButton->setObjectName(QString::fromUtf8("selectedFileRadioButton"));

        horizontalLayout_2->addWidget(selectedFileRadioButton);

        checkedFileRadioButton = new QRadioButton(groupBox_2);
        exportButtonGroup->addButton(checkedFileRadioButton);
        checkedFileRadioButton->setObjectName(QString::fromUtf8("checkedFileRadioButton"));

        horizontalLayout_2->addWidget(checkedFileRadioButton);

        listedFileRadioButton = new QRadioButton(groupBox_2);
        exportButtonGroup->addButton(listedFileRadioButton);
        listedFileRadioButton->setObjectName(QString::fromUtf8("listedFileRadioButton"));

        horizontalLayout_2->addWidget(listedFileRadioButton);


        verticalLayout_3->addLayout(horizontalLayout_2);


        verticalLayout->addWidget(groupBox_2);

        groupBox = new QGroupBox(DigDeeperDialog);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        verticalLayout_4 = new QVBoxLayout(groupBox);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        expandarchivescheckbox = new QCheckBox(groupBox);
        expandarchivescheckbox->setObjectName(QString::fromUtf8("expandarchivescheckbox"));

        horizontalLayout_4->addWidget(expandarchivescheckbox);


        verticalLayout_4->addLayout(horizontalLayout_4);

        emailcheckbox = new QCheckBox(groupBox);
        emailcheckbox->setObjectName(QString::fromUtf8("emailcheckbox"));

        verticalLayout_4->addWidget(emailcheckbox);

        thumbnailcheckBox = new QCheckBox(groupBox);
        thumbnailcheckBox->setObjectName(QString::fromUtf8("thumbnailcheckBox"));

        verticalLayout_4->addWidget(thumbnailcheckBox);

        videocheckBox = new QCheckBox(groupBox);
        videocheckBox->setObjectName(QString::fromUtf8("videocheckBox"));

        verticalLayout_4->addWidget(videocheckBox);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        hashcheckbox = new QCheckBox(groupBox);
        hashcheckbox->setObjectName(QString::fromUtf8("hashcheckbox"));

        horizontalLayout_3->addWidget(hashcheckbox);


        verticalLayout_4->addLayout(horizontalLayout_3);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        horizontalLayout_5->setContentsMargins(0, 0, 0, 0);
        hashlistcheckbox = new QCheckBox(groupBox);
        hashlistcheckbox->setObjectName(QString::fromUtf8("hashlistcheckbox"));

        horizontalLayout_5->addWidget(hashlistcheckbox, 0, Qt::AlignTop);

        hashlistwidget = new QListWidget(groupBox);
        hashlistwidget->setObjectName(QString::fromUtf8("hashlistwidget"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(hashlistwidget->sizePolicy().hasHeightForWidth());
        hashlistwidget->setSizePolicy(sizePolicy);
        hashlistwidget->setMaximumSize(QSize(16777215, 70));
        hashlistwidget->setProperty("showDropIndicator", QVariant(false));
        hashlistwidget->setAlternatingRowColors(true);
        hashlistwidget->setSelectionMode(QAbstractItemView::ExtendedSelection);
        hashlistwidget->setUniformItemSizes(true);

        horizontalLayout_5->addWidget(hashlistwidget);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_2);


        verticalLayout_4->addLayout(horizontalLayout_5);


        verticalLayout->addWidget(groupBox);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(-1, -1, 9, -1);
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        cancelButton = new QPushButton(DigDeeperDialog);
        cancelButton->setObjectName(QString::fromUtf8("cancelButton"));

        horizontalLayout->addWidget(cancelButton);

        processButton = new QPushButton(DigDeeperDialog);
        processButton->setObjectName(QString::fromUtf8("processButton"));

        horizontalLayout->addWidget(processButton);


        verticalLayout->addLayout(horizontalLayout);


        verticalLayout_2->addLayout(verticalLayout);


        retranslateUi(DigDeeperDialog);
        QObject::connect(cancelButton, SIGNAL(clicked()), DigDeeperDialog, SLOT(close()));

        QMetaObject::connectSlotsByName(DigDeeperDialog);
    } // setupUi

    void retranslateUi(QDialog *DigDeeperDialog)
    {
        DigDeeperDialog->setWindowTitle(QCoreApplication::translate("DigDeeperDialog", "Dig Deeper", nullptr));
        groupBox_2->setTitle(QCoreApplication::translate("DigDeeperDialog", "Process the Following File(s)", nullptr));
        selectedFileRadioButton->setText(QCoreApplication::translate("DigDeeperDialog", "Selected", nullptr));
        checkedFileRadioButton->setText(QCoreApplication::translate("DigDeeperDialog", "Checked (#)", nullptr));
        listedFileRadioButton->setText(QCoreApplication::translate("DigDeeperDialog", "All (#)", nullptr));
        groupBox->setTitle(QCoreApplication::translate("DigDeeperDialog", "For the Following Options", nullptr));
        expandarchivescheckbox->setText(QCoreApplication::translate("DigDeeperDialog", "Expand Archives (zip)", nullptr));
        emailcheckbox->setText(QCoreApplication::translate("DigDeeperDialog", "Expand E-Mail Mailboxes (mbox)", nullptr));
        thumbnailcheckBox->setText(QCoreApplication::translate("DigDeeperDialog", "Generate Thumbnails for Image Files", nullptr));
        videocheckBox->setText(QCoreApplication::translate("DigDeeperDialog", "Generate Thumbnails for Video Files", nullptr));
        hashcheckbox->setText(QCoreApplication::translate("DigDeeperDialog", "Calcuate BLAKE3 Hash", nullptr));
        hashlistcheckbox->setText(QCoreApplication::translate("DigDeeperDialog", "Compare Files to Selected Hash Lists:", nullptr));
        cancelButton->setText(QCoreApplication::translate("DigDeeperDialog", "Cancel", nullptr));
        processButton->setText(QCoreApplication::translate("DigDeeperDialog", "Process", nullptr));
    } // retranslateUi

};

namespace Ui {
    class DigDeeperDialog: public Ui_DigDeeperDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIGDEEPERDIALOG_H