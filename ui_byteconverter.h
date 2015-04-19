/********************************************************************************
** Form generated from reading UI file 'byteconverter.ui'
**
** Created by: Qt User Interface Compiler version 5.4.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_BYTECONVERTER_H
#define UI_BYTECONVERTER_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>

QT_BEGIN_NAMESPACE

class Ui_ByteConverter
{
public:
    QHBoxLayout *horizontalLayout;
    QLabel *label;

    void setupUi(QDialog *ByteConverter)
    {
        if (ByteConverter->objectName().isEmpty())
            ByteConverter->setObjectName(QStringLiteral("ByteConverter"));
        ByteConverter->resize(240, 223);
        horizontalLayout = new QHBoxLayout(ByteConverter);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        label = new QLabel(ByteConverter);
        label->setObjectName(QStringLiteral("label"));
        label->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
        label->setMargin(10);

        horizontalLayout->addWidget(label);


        retranslateUi(ByteConverter);

        QMetaObject::connectSlotsByName(ByteConverter);
    } // setupUi

    void retranslateUi(QDialog *ByteConverter)
    {
        ByteConverter->setWindowTitle(QApplication::translate("ByteConverter", "Dialog", 0));
        label->setText(QApplication::translate("ByteConverter", "one<br/>two<br/>three", 0));
    } // retranslateUi

};

namespace Ui {
    class ByteConverter: public Ui_ByteConverter {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_BYTECONVERTER_H
