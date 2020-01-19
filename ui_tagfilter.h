/********************************************************************************
** Form generated from reading UI file 'tagfilter.ui'
**
** Created by: Qt User Interface Compiler version 5.14.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TAGFILTER_H
#define UI_TAGFILTER_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_TagFilter
{
public:
    QGridLayout *gridLayout;
    QPushButton *pushButton;
    QCheckBox *tagcheckBox;
    QComboBox *tagcomboBox;

    void setupUi(QWidget *TagFilter)
    {
        if (TagFilter->objectName().isEmpty())
            TagFilter->setObjectName(QString::fromUtf8("TagFilter"));
        TagFilter->setWindowModality(Qt::ApplicationModal);
        TagFilter->resize(429, 80);
        QFont font;
        font.setPointSize(8);
        TagFilter->setFont(font);
        TagFilter->setAutoFillBackground(true);
        gridLayout = new QGridLayout(TagFilter);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setHorizontalSpacing(0);
        pushButton = new QPushButton(TagFilter);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setStyleSheet(QString::fromUtf8("border: 1px solid black; padding: 5px 10px 5px 10px;"));
        pushButton->setFlat(true);

        gridLayout->addWidget(pushButton, 1, 1, 1, 1);

        tagcheckBox = new QCheckBox(TagFilter);
        tagcheckBox->setObjectName(QString::fromUtf8("tagcheckBox"));

        gridLayout->addWidget(tagcheckBox, 0, 0, 1, 1);

        tagcomboBox = new QComboBox(TagFilter);
        tagcomboBox->setObjectName(QString::fromUtf8("tagcomboBox"));
        tagcomboBox->setEnabled(false);
        tagcomboBox->setInsertPolicy(QComboBox::NoInsert);

        gridLayout->addWidget(tagcomboBox, 0, 1, 1, 1);


        retranslateUi(TagFilter);
        QObject::connect(tagcheckBox, SIGNAL(toggled(bool)), tagcomboBox, SLOT(setEnabled(bool)));

        pushButton->setDefault(true);


        QMetaObject::connectSlotsByName(TagFilter);
    } // setupUi

    void retranslateUi(QWidget *TagFilter)
    {
        TagFilter->setWindowTitle(QCoreApplication::translate("TagFilter", "Filter", nullptr));
        pushButton->setText(QCoreApplication::translate("TagFilter", "Apply", nullptr));
        tagcheckBox->setText(QCoreApplication::translate("TagFilter", "Show Items Tagged As", nullptr));
    } // retranslateUi

};

namespace Ui {
    class TagFilter: public Ui_TagFilter {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TAGFILTER_H
