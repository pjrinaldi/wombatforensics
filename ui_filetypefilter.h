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
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_FileTypeFilter
{
public:
    QGridLayout *gridLayout;
    QPushButton *pushButton;
    QCheckBox *typecheckBox;
    QComboBox *typecomboBox;

    void setupUi(QWidget *FileTypeFilter)
    {
        if (FileTypeFilter->objectName().isEmpty())
            FileTypeFilter->setObjectName(QString::fromUtf8("FileTypeFilter"));
        FileTypeFilter->setWindowModality(Qt::ApplicationModal);
        FileTypeFilter->resize(429, 80);
        QFont font;
        font.setPointSize(8);
        FileTypeFilter->setFont(font);
        FileTypeFilter->setAutoFillBackground(true);
        gridLayout = new QGridLayout(FileTypeFilter);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setHorizontalSpacing(0);
        pushButton = new QPushButton(FileTypeFilter);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setStyleSheet(QString::fromUtf8("border: 1px solid black; padding: 5px 10px 5px 10px;"));
        pushButton->setFlat(true);

        gridLayout->addWidget(pushButton, 1, 1, 1, 1);

        typecheckBox = new QCheckBox(FileTypeFilter);
        typecheckBox->setObjectName(QString::fromUtf8("typecheckBox"));

        gridLayout->addWidget(typecheckBox, 0, 0, 1, 1);

        typecomboBox = new QComboBox(FileTypeFilter);
        typecomboBox->setObjectName(QString::fromUtf8("typecomboBox"));
        typecomboBox->setEnabled(false);
        typecomboBox->setInsertPolicy(QComboBox::NoInsert);

        gridLayout->addWidget(typecomboBox, 0, 1, 1, 1);


        retranslateUi(FileTypeFilter);
        QObject::connect(typecheckBox, SIGNAL(toggled(bool)), typecomboBox, SLOT(setEnabled(bool)));

        pushButton->setDefault(true);


        QMetaObject::connectSlotsByName(FileTypeFilter);
    } // setupUi

    void retranslateUi(QWidget *FileTypeFilter)
    {
        FileTypeFilter->setWindowTitle(QCoreApplication::translate("FileTypeFilter", "Filter", nullptr));
        pushButton->setText(QCoreApplication::translate("FileTypeFilter", "Apply", nullptr));
        typecheckBox->setText(QCoreApplication::translate("FileTypeFilter", "Show Items where type matches", nullptr));
    } // retranslateUi

};

namespace Ui {
    class FileTypeFilter: public Ui_FileTypeFilter {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FILETYPEFILTER_H
