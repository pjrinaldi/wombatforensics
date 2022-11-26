/********************************************************************************
** Form generated from reading UI file 'carvedialog.ui'
**
** Created by: Qt User Interface Compiler version 5.15.7
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CARVEDIALOG_H
#define UI_CARVEDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>

QT_BEGIN_NAMESPACE

class Ui_CarveDialog
{
public:
    QGridLayout *gridLayout;
    QPushButton *carvebutton;
    QPushButton *cancelbutton;
    QLabel *titlelabel;
    QLineEdit *titlelineedit;
    QSpacerItem *verticalSpacer;
    QSpacerItem *horizontalSpacer;
    QLabel *taglabel;
    QComboBox *tagcombobox;

    void setupUi(QDialog *carvedialog)
    {
        if (carvedialog->objectName().isEmpty())
            carvedialog->setObjectName(QString::fromUtf8("carvedialog"));
        carvedialog->resize(320, 114);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/bar/managetags"), QSize(), QIcon::Normal, QIcon::Off);
        carvedialog->setWindowIcon(icon);
        gridLayout = new QGridLayout(carvedialog);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        carvebutton = new QPushButton(carvedialog);
        carvebutton->setObjectName(QString::fromUtf8("carvebutton"));

        gridLayout->addWidget(carvebutton, 3, 2, 1, 1);

        cancelbutton = new QPushButton(carvedialog);
        cancelbutton->setObjectName(QString::fromUtf8("cancelbutton"));

        gridLayout->addWidget(cancelbutton, 3, 1, 1, 1);

        titlelabel = new QLabel(carvedialog);
        titlelabel->setObjectName(QString::fromUtf8("titlelabel"));

        gridLayout->addWidget(titlelabel, 0, 0, 1, 1);

        titlelineedit = new QLineEdit(carvedialog);
        titlelineedit->setObjectName(QString::fromUtf8("titlelineedit"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(2);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(titlelineedit->sizePolicy().hasHeightForWidth());
        titlelineedit->setSizePolicy(sizePolicy);

        gridLayout->addWidget(titlelineedit, 0, 1, 1, 2);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(verticalSpacer, 2, 1, 1, 1);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 3, 0, 1, 1);

        taglabel = new QLabel(carvedialog);
        taglabel->setObjectName(QString::fromUtf8("taglabel"));

        gridLayout->addWidget(taglabel, 1, 0, 1, 1);

        tagcombobox = new QComboBox(carvedialog);
        tagcombobox->setObjectName(QString::fromUtf8("tagcombobox"));

        gridLayout->addWidget(tagcombobox, 1, 1, 1, 2);


        retranslateUi(carvedialog);

        QMetaObject::connectSlotsByName(carvedialog);
    } // setupUi

    void retranslateUi(QDialog *carvedialog)
    {
        carvedialog->setWindowTitle(QCoreApplication::translate("CarveDialog", "Assign To Carved Section", nullptr));
        carvebutton->setText(QCoreApplication::translate("CarveDialog", "Assign", nullptr));
        cancelbutton->setText(QCoreApplication::translate("CarveDialog", "Cancel", nullptr));
        titlelabel->setText(QCoreApplication::translate("CarveDialog", "Apply Title:", nullptr));
        titlelineedit->setPlaceholderText(QCoreApplication::translate("CarveDialog", "Enter Name/Title for Carved Section", nullptr));
        taglabel->setText(QCoreApplication::translate("CarveDialog", "Apply Tag:", nullptr));
    } // retranslateUi

};

namespace Ui {
    class CarveDialog: public Ui_CarveDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CARVEDIALOG_H
