/********************************************************************************
** Form generated from reading UI file 'jumpto.ui'
**
** Created by: Qt User Interface Compiler version 5.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_JUMPTO_H
#define UI_JUMPTO_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_JumpFilter
{
public:
    QGridLayout *gridLayout;
    QLineEdit *lineEdit;
    QLabel *label;
    QPushButton *pushButton;

    void setupUi(QFrame *JumpFilter)
    {
        if (JumpFilter->objectName().isEmpty())
            JumpFilter->setObjectName(QStringLiteral("JumpFilter"));
        JumpFilter->setWindowModality(Qt::WindowModal);
        JumpFilter->resize(266, 47);
        JumpFilter->setAutoFillBackground(true);
        JumpFilter->setFrameShape(QFrame::StyledPanel);
        JumpFilter->setFrameShadow(QFrame::Raised);
        gridLayout = new QGridLayout(JumpFilter);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setSizeConstraint(QLayout::SetFixedSize);
        lineEdit = new QLineEdit(JumpFilter);
        lineEdit->setObjectName(QStringLiteral("lineEdit"));
        lineEdit->setMinimumSize(QSize(100, 0));
        lineEdit->setClearButtonEnabled(false);

        gridLayout->addWidget(lineEdit, 0, 1, 1, 1);

        label = new QLabel(JumpFilter);
        label->setObjectName(QStringLiteral("label"));
        label->setScaledContents(false);

        gridLayout->addWidget(label, 0, 0, 1, 1);

        pushButton = new QPushButton(JumpFilter);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(pushButton->sizePolicy().hasHeightForWidth());
        pushButton->setSizePolicy(sizePolicy);
        pushButton->setMinimumSize(QSize(30, 0));
        pushButton->setMaximumSize(QSize(30, 16777215));
        pushButton->setBaseSize(QSize(30, 0));
        QFont font;
        font.setPointSize(8);
        font.setStrikeOut(false);
        pushButton->setFont(font);
        pushButton->setCursor(QCursor(Qt::ArrowCursor));
        pushButton->setAutoDefault(true);
        pushButton->setFlat(true);

        gridLayout->addWidget(pushButton, 0, 2, 1, 1);


        retranslateUi(JumpFilter);

        pushButton->setDefault(true);


        QMetaObject::connectSlotsByName(JumpFilter);
    } // setupUi

    void retranslateUi(QFrame *JumpFilter)
    {
        JumpFilter->setWindowTitle(QApplication::translate("JumpFilter", "Frame", Q_NULLPTR));
        lineEdit->setPlaceholderText(QApplication::translate("JumpFilter", "Enter Hex Offset", Q_NULLPTR));
        label->setText(QApplication::translate("JumpFilter", "Jump to:", Q_NULLPTR));
        pushButton->setText(QApplication::translate("JumpFilter", "Go", Q_NULLPTR));
#ifndef QT_NO_SHORTCUT
        pushButton->setShortcut(QApplication::translate("JumpFilter", "Return", Q_NULLPTR));
#endif // QT_NO_SHORTCUT
    } // retranslateUi

};

namespace Ui {
    class JumpFilter: public Ui_JumpFilter {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_JUMPTO_H
