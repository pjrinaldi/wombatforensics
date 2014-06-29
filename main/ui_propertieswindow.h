/********************************************************************************
** Form generated from reading UI file 'propertieswindow.ui'
**
** Created by: Qt User Interface Compiler version 5.3.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PROPERTIESWINDOW_H
#define UI_PROPERTIESWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QTableView>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_PropertiesWindow
{
public:
    QVBoxLayout *verticalLayout;
    QTableView *tableView;

    void setupUi(QDialog *PropertiesWindow)
    {
        if (PropertiesWindow->objectName().isEmpty())
            PropertiesWindow->setObjectName(QStringLiteral("PropertiesWindow"));
        PropertiesWindow->setWindowModality(Qt::ApplicationModal);
        PropertiesWindow->resize(240, 310);
        PropertiesWindow->setAutoFillBackground(false);
        PropertiesWindow->setSizeGripEnabled(false);
        PropertiesWindow->setModal(true);
        verticalLayout = new QVBoxLayout(PropertiesWindow);
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        tableView = new QTableView(PropertiesWindow);
        tableView->setObjectName(QStringLiteral("tableView"));
        tableView->setSizeAdjustPolicy(QAbstractScrollArea::AdjustIgnored);
        tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
        tableView->setProperty("showDropIndicator", QVariant(false));
        tableView->setDragDropOverwriteMode(false);
        tableView->setAlternatingRowColors(true);
        tableView->setSelectionMode(QAbstractItemView::NoSelection);
        tableView->setShowGrid(true);
        tableView->setCornerButtonEnabled(false);
        tableView->verticalHeader()->setVisible(false);

        verticalLayout->addWidget(tableView);


        retranslateUi(PropertiesWindow);

        QMetaObject::connectSlotsByName(PropertiesWindow);
    } // setupUi

    void retranslateUi(QDialog *PropertiesWindow)
    {
        PropertiesWindow->setWindowTitle(QApplication::translate("PropertiesWindow", "Properties", 0));
    } // retranslateUi

};

namespace Ui {
    class PropertiesWindow: public Ui_PropertiesWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PROPERTIESWINDOW_H
