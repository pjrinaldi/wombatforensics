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
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QTableView>

QT_BEGIN_NAMESPACE

class Ui_PropertiesWindow
{
public:
    QHBoxLayout *horizontalLayout;
    QSplitter *splitter;
    QTableView *tableView;
    QGroupBox *groupBox;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label;

    void setupUi(QDialog *PropertiesWindow)
    {
        if (PropertiesWindow->objectName().isEmpty())
            PropertiesWindow->setObjectName(QStringLiteral("PropertiesWindow"));
        PropertiesWindow->setWindowModality(Qt::ApplicationModal);
        PropertiesWindow->resize(240, 310);
        PropertiesWindow->setAutoFillBackground(false);
        PropertiesWindow->setSizeGripEnabled(false);
        PropertiesWindow->setModal(true);
        horizontalLayout = new QHBoxLayout(PropertiesWindow);
        horizontalLayout->setSpacing(0);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        splitter = new QSplitter(PropertiesWindow);
        splitter->setObjectName(QStringLiteral("splitter"));
        splitter->setOrientation(Qt::Vertical);
        tableView = new QTableView(splitter);
        tableView->setObjectName(QStringLiteral("tableView"));
        tableView->setSizeAdjustPolicy(QAbstractScrollArea::AdjustIgnored);
        tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
        tableView->setProperty("showDropIndicator", QVariant(false));
        tableView->setDragDropOverwriteMode(false);
        tableView->setAlternatingRowColors(true);
        tableView->setSelectionMode(QAbstractItemView::SingleSelection);
        tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
        tableView->setShowGrid(true);
        tableView->setCornerButtonEnabled(false);
        splitter->addWidget(tableView);
        tableView->verticalHeader()->setVisible(false);
        tableView->verticalHeader()->setHighlightSections(false);
        groupBox = new QGroupBox(splitter);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        QFont font;
        font.setPointSize(7);
        groupBox->setFont(font);
        groupBox->setAutoFillBackground(false);
        groupBox->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
        groupBox->setFlat(false);
        groupBox->setCheckable(false);
        horizontalLayout_2 = new QHBoxLayout(groupBox);
        horizontalLayout_2->setSpacing(0);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        label = new QLabel(groupBox);
        label->setObjectName(QStringLiteral("label"));
        label->setFont(font);
        label->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
        label->setWordWrap(true);
        label->setMargin(1);
        label->setTextInteractionFlags(Qt::NoTextInteraction);

        horizontalLayout_2->addWidget(label);

        splitter->addWidget(groupBox);

        horizontalLayout->addWidget(splitter);


        retranslateUi(PropertiesWindow);

        QMetaObject::connectSlotsByName(PropertiesWindow);
    } // setupUi

    void retranslateUi(QDialog *PropertiesWindow)
    {
        PropertiesWindow->setWindowTitle(QApplication::translate("PropertiesWindow", "Properties", 0));
        groupBox->setTitle(QApplication::translate("PropertiesWindow", "Description", 0));
        label->setText(QApplication::translate("PropertiesWindow", "Information on the selected object and its value.", 0));
    } // retranslateUi

};

namespace Ui {
    class PropertiesWindow: public Ui_PropertiesWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PROPERTIESWINDOW_H
