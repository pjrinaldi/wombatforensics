/********************************************************************************
** Form generated from reading UI file 'byteconverter.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_BYTECONVERTER_H
#define UI_BYTECONVERTER_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_ByteConverter
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QCheckBox *checkBox;

    void setupUi(QDialog *ByteConverter)
    {
        if (ByteConverter->objectName().isEmpty())
            ByteConverter->setObjectName(QString::fromUtf8("ByteConverter"));
        ByteConverter->resize(240, 223);
        verticalLayout = new QVBoxLayout(ByteConverter);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        label = new QLabel(ByteConverter);
        label->setObjectName(QString::fromUtf8("label"));
        label->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
        label->setMargin(10);

        verticalLayout->addWidget(label);

        checkBox = new QCheckBox(ByteConverter);
        checkBox->setObjectName(QString::fromUtf8("checkBox"));

        verticalLayout->addWidget(checkBox);


        retranslateUi(ByteConverter);

        QMetaObject::connectSlotsByName(ByteConverter);
    } // setupUi

    void retranslateUi(QDialog *ByteConverter)
    {
        ByteConverter->setWindowTitle(QCoreApplication::translate("ByteConverter", "Byte Converter", nullptr));
        label->setText(QCoreApplication::translate("ByteConverter", "one<br/>two<br/>three", nullptr));
        checkBox->setText(QCoreApplication::translate("ByteConverter", "Big Endian", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ByteConverter: public Ui_ByteConverter {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_BYTECONVERTER_H
