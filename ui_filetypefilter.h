/********************************************************************************
** Form generated from reading UI file 'filetypefilter.ui'
**
** Created by: Qt User Interface Compiler version 5.4.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FILETYPEFILTER_H
#define UI_FILETYPEFILTER_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_FileTypeFilter
{
public:
    QGridLayout *gridLayout;
    QComboBox *categorycomboBox;
    QComboBox *typecomboBox;
    QCheckBox *categorycheckBox;
    QCheckBox *typecheckBox;
    QPushButton *pushButton;

    void setupUi(QWidget *FileTypeFilter)
    {
        if (FileTypeFilter->objectName().isEmpty())
            FileTypeFilter->setObjectName(QStringLiteral("FileTypeFilter"));
        FileTypeFilter->setWindowModality(Qt::ApplicationModal);
        FileTypeFilter->resize(429, 105);
        QFont font;
        font.setPointSize(8);
        FileTypeFilter->setFont(font);
        FileTypeFilter->setAutoFillBackground(true);
        gridLayout = new QGridLayout(FileTypeFilter);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setHorizontalSpacing(0);
        categorycomboBox = new QComboBox(FileTypeFilter);
        categorycomboBox->setObjectName(QStringLiteral("categorycomboBox"));
        categorycomboBox->setEditable(false);
        categorycomboBox->setInsertPolicy(QComboBox::NoInsert);

        gridLayout->addWidget(categorycomboBox, 0, 1, 1, 1);

        typecomboBox = new QComboBox(FileTypeFilter);
        typecomboBox->setObjectName(QStringLiteral("typecomboBox"));
        typecomboBox->setInsertPolicy(QComboBox::NoInsert);

        gridLayout->addWidget(typecomboBox, 1, 1, 1, 1);

        categorycheckBox = new QCheckBox(FileTypeFilter);
        categorycheckBox->setObjectName(QStringLiteral("categorycheckBox"));

        gridLayout->addWidget(categorycheckBox, 0, 0, 1, 1);

        typecheckBox = new QCheckBox(FileTypeFilter);
        typecheckBox->setObjectName(QStringLiteral("typecheckBox"));

        gridLayout->addWidget(typecheckBox, 1, 0, 1, 1);

        pushButton = new QPushButton(FileTypeFilter);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setDefault(true);
        pushButton->setFlat(true);

        gridLayout->addWidget(pushButton, 2, 1, 1, 1);


        retranslateUi(FileTypeFilter);
        QObject::connect(categorycheckBox, SIGNAL(toggled(bool)), categorycomboBox, SLOT(setEnabled(bool)));
        QObject::connect(typecheckBox, SIGNAL(toggled(bool)), typecomboBox, SLOT(setEnabled(bool)));

        QMetaObject::connectSlotsByName(FileTypeFilter);
    } // setupUi

    void retranslateUi(QWidget *FileTypeFilter)
    {
        FileTypeFilter->setWindowTitle(QApplication::translate("FileTypeFilter", "Filter", 0));
        categorycomboBox->setCurrentText(QString());
        categorycheckBox->setText(QApplication::translate("FileTypeFilter", "Show Items where category matches", 0));
        typecheckBox->setText(QApplication::translate("FileTypeFilter", "Show Items where type matches", 0));
        pushButton->setText(QApplication::translate("FileTypeFilter", "Apply", 0));
    } // retranslateUi

};

namespace Ui {
    class FileTypeFilter: public Ui_FileTypeFilter {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FILETYPEFILTER_H
