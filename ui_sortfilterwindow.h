/********************************************************************************
** Form generated from reading UI file 'sortfilterwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.4.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SORTFILTERWINDOW_H
#define UI_SORTFILTERWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QListView>

QT_BEGIN_NAMESPACE

class Ui_sortfilter
{
public:
    QHBoxLayout *horizontalLayout;
    QListView *listView;

    void setupUi(QDialog *sortfilter)
    {
        if (sortfilter->objectName().isEmpty())
            sortfilter->setObjectName(QStringLiteral("sortfilter"));
        sortfilter->resize(640, 480);
        sortfilter->setSizeGripEnabled(false);
        sortfilter->setModal(false);
        horizontalLayout = new QHBoxLayout(sortfilter);
        horizontalLayout->setSpacing(0);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        listView = new QListView(sortfilter);
        listView->setObjectName(QStringLiteral("listView"));

        horizontalLayout->addWidget(listView);


        retranslateUi(sortfilter);

        QMetaObject::connectSlotsByName(sortfilter);
    } // setupUi

    void retranslateUi(QDialog *sortfilter)
    {
        sortfilter->setWindowTitle(QApplication::translate("sortfilter", "Sort Filter Viewer", 0));
    } // retranslateUi

};

namespace Ui {
    class sortfilter: public Ui_sortfilter {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SORTFILTERWINDOW_H
