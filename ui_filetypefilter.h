/********************************************************************************
** Form generated from reading UI file 'filetypefilter.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FILETYPEFILTER_H
#define UI_FILETYPEFILTER_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_FileTypeFilter
{
public:
    QGridLayout *gridLayout;
    QCheckBox *typecheckBox;
    QComboBox *typecomboBox;
    QPushButton *pushButton;
    QPushButton *pushButton_2;

    void setupUi(QDialog *FileTypeFilter)
    {
        if (FileTypeFilter->objectName().isEmpty())
            FileTypeFilter->setObjectName(QString::fromUtf8("FileTypeFilter"));
        FileTypeFilter->setWindowModality(Qt::ApplicationModal);
        FileTypeFilter->resize(406, 74);
        gridLayout = new QGridLayout(FileTypeFilter);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        typecheckBox = new QCheckBox(FileTypeFilter);
        typecheckBox->setObjectName(QString::fromUtf8("typecheckBox"));

        gridLayout->addWidget(typecheckBox, 0, 0, 1, 1);

        typecomboBox = new QComboBox(FileTypeFilter);
        typecomboBox->setObjectName(QString::fromUtf8("typecomboBox"));
        typecomboBox->setEnabled(false);
        typecomboBox->setInsertPolicy(QComboBox::NoInsert);

        gridLayout->addWidget(typecomboBox, 0, 1, 1, 1);

        pushButton = new QPushButton(FileTypeFilter);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setStyleSheet(QString::fromUtf8(""));
        pushButton->setFlat(false);

        gridLayout->addWidget(pushButton, 1, 1, 1, 1);

        pushButton_2 = new QPushButton(FileTypeFilter);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));

        gridLayout->addWidget(pushButton_2, 1, 0, 1, 1);


        retranslateUi(FileTypeFilter);

        pushButton->setDefault(true);


        QMetaObject::connectSlotsByName(FileTypeFilter);
    } // setupUi

    void retranslateUi(QDialog *FileTypeFilter)
    {
        FileTypeFilter->setWindowTitle(QCoreApplication::translate("FileTypeFilter", "File Type Filter", nullptr));
        typecheckBox->setText(QCoreApplication::translate("FileTypeFilter", "Show Items where type matches", nullptr));
        pushButton->setText(QCoreApplication::translate("FileTypeFilter", "Apply", nullptr));
        pushButton_2->setText(QCoreApplication::translate("FileTypeFilter", "Cancel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class FileTypeFilter: public Ui_FileTypeFilter {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FILETYPEFILTER_H
