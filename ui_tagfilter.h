/********************************************************************************
** Form generated from reading UI file 'tagfilter.ui'
**
** Created by: Qt User Interface Compiler version 5.15.7
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TAGFILTER_H
#define UI_TAGFILTER_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_TagFilter
{
public:
    QGridLayout *gridLayout;
    QCheckBox *tagcheckBox;
    QComboBox *tagcomboBox;
    QPushButton *pushButton;
    QPushButton *pushButton_2;

    void setupUi(QDialog *TagFilter)
    {
        if (TagFilter->objectName().isEmpty())
            TagFilter->setObjectName(QString::fromUtf8("TagFilter"));
        TagFilter->setWindowModality(Qt::ApplicationModal);
        TagFilter->resize(395, 74);
        gridLayout = new QGridLayout(TagFilter);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        tagcheckBox = new QCheckBox(TagFilter);
        tagcheckBox->setObjectName(QString::fromUtf8("tagcheckBox"));

        gridLayout->addWidget(tagcheckBox, 0, 0, 1, 1);

        tagcomboBox = new QComboBox(TagFilter);
        tagcomboBox->setObjectName(QString::fromUtf8("tagcomboBox"));
        tagcomboBox->setEnabled(false);
        tagcomboBox->setInsertPolicy(QComboBox::NoInsert);

        gridLayout->addWidget(tagcomboBox, 0, 1, 1, 1);

        pushButton = new QPushButton(TagFilter);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setStyleSheet(QString::fromUtf8(""));
        pushButton->setFlat(false);

        gridLayout->addWidget(pushButton, 1, 1, 1, 1);

        pushButton_2 = new QPushButton(TagFilter);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));

        gridLayout->addWidget(pushButton_2, 1, 0, 1, 1);


        retranslateUi(TagFilter);

        pushButton->setDefault(true);


        QMetaObject::connectSlotsByName(TagFilter);
    } // setupUi

    void retranslateUi(QDialog *TagFilter)
    {
        TagFilter->setWindowTitle(QCoreApplication::translate("TagFilter", "Tag Filter", nullptr));
        tagcheckBox->setText(QCoreApplication::translate("TagFilter", "Show Items Tagged As", nullptr));
        pushButton->setText(QCoreApplication::translate("TagFilter", "Apply", nullptr));
        pushButton_2->setText(QCoreApplication::translate("TagFilter", "Cancel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class TagFilter: public Ui_TagFilter {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TAGFILTER_H
