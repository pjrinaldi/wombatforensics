/********************************************************************************
** Form generated from reading UI file 'digdeeperdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.13.2
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
    QCheckBox *videocheckBox;
    QHBoxLayout *horizontalLayout_3;
    QCheckBox *hashcheckbox;
    QRadioButton *md5radiobutton;
    QRadioButton *sha1radiobutton;
    QRadioButton *sha256radiobutton;
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
        DigDeeperDialog->resize(365, 314);
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

        md5radiobutton = new QRadioButton(groupBox);
        md5radiobutton->setObjectName(QString::fromUtf8("md5radiobutton"));

        horizontalLayout_3->addWidget(md5radiobutton);

        sha1radiobutton = new QRadioButton(groupBox);
        sha1radiobutton->setObjectName(QString::fromUtf8("sha1radiobutton"));

        horizontalLayout_3->addWidget(sha1radiobutton);

        sha256radiobutton = new QRadioButton(groupBox);
        sha256radiobutton->setObjectName(QString::fromUtf8("sha256radiobutton"));

        horizontalLayout_3->addWidget(sha256radiobutton);


        verticalLayout_4->addLayout(horizontalLayout_3);


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
        thumbnailcheckBox->setText(QCoreApplication::translate("DigDeeperDialog", "Generate Thumbnails for All Image Files", nullptr));
        videocheckBox->setText(QCoreApplication::translate("DigDeeperDialog", "Generate Thumbnails for All Video Files", nullptr));
        hashcheckbox->setText(QCoreApplication::translate("DigDeeperDialog", "Calcuate Hash:", nullptr));
        md5radiobutton->setText(QCoreApplication::translate("DigDeeperDialog", "MD5", nullptr));
        sha1radiobutton->setText(QCoreApplication::translate("DigDeeperDialog", "SHA1", nullptr));
        sha256radiobutton->setText(QCoreApplication::translate("DigDeeperDialog", "SHA256", nullptr));
        cancelButton->setText(QCoreApplication::translate("DigDeeperDialog", "Cancel", nullptr));
        processButton->setText(QCoreApplication::translate("DigDeeperDialog", "Process", nullptr));
    } // retranslateUi

};

namespace Ui {
    class DigDeeperDialog: public Ui_DigDeeperDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIGDEEPERDIALOG_H
