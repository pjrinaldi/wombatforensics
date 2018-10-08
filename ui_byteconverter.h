/********************************************************************************
** Form generated from reading UI file 'byteconverter.ui'
**
** Created by: Qt User Interface Compiler version 5.11.1
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
            ByteConverter->setObjectName(QStringLiteral("ByteConverter"));
        ByteConverter->resize(240, 223);
        verticalLayout = new QVBoxLayout(ByteConverter);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        label = new QLabel(ByteConverter);
        label->setObjectName(QStringLiteral("label"));
        label->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
        label->setMargin(10);

        verticalLayout->addWidget(label);

        checkBox = new QCheckBox(ByteConverter);
        checkBox->setObjectName(QStringLiteral("checkBox"));

        verticalLayout->addWidget(checkBox);


        retranslateUi(ByteConverter);

        QMetaObject::connectSlotsByName(ByteConverter);
    } // setupUi

    void retranslateUi(QDialog *ByteConverter)
    {
        ByteConverter->setWindowTitle(QApplication::translate("ByteConverter", "Byte Converter", nullptr));
        label->setText(QApplication::translate("ByteConverter", "one<br/>two<br/>three", nullptr));
        checkBox->setText(QApplication::translate("ByteConverter", "Big Endian", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ByteConverter: public Ui_ByteConverter {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_BYTECONVERTER_H
