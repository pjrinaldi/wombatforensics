/********************************************************************************
** Form generated from reading UI file 'propertieswindow.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PROPERTIESWINDOW_H
#define UI_PROPERTIESWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
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
            PropertiesWindow->setObjectName(QString::fromUtf8("PropertiesWindow"));
        PropertiesWindow->setWindowModality(Qt::ApplicationModal);
        PropertiesWindow->resize(240, 310);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(PropertiesWindow->sizePolicy().hasHeightForWidth());
        PropertiesWindow->setSizePolicy(sizePolicy);
        PropertiesWindow->setAutoFillBackground(false);
        PropertiesWindow->setSizeGripEnabled(false);
        PropertiesWindow->setModal(true);
        horizontalLayout = new QHBoxLayout(PropertiesWindow);
        horizontalLayout->setSpacing(0);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        splitter = new QSplitter(PropertiesWindow);
        splitter->setObjectName(QString::fromUtf8("splitter"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(1);
        sizePolicy1.setVerticalStretch(1);
        sizePolicy1.setHeightForWidth(splitter->sizePolicy().hasHeightForWidth());
        splitter->setSizePolicy(sizePolicy1);
        splitter->setOrientation(Qt::Vertical);
        splitter->setOpaqueResize(false);
        splitter->setChildrenCollapsible(false);
        tableView = new QTableView(splitter);
        tableView->setObjectName(QString::fromUtf8("tableView"));
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(2);
        sizePolicy2.setHeightForWidth(tableView->sizePolicy().hasHeightForWidth());
        tableView->setSizePolicy(sizePolicy2);
        tableView->setMinimumSize(QSize(240, 200));
        tableView->setMaximumSize(QSize(16777215, 16777215));
        QFont font;
        font.setPointSize(7);
        tableView->setFont(font);
        tableView->setAutoFillBackground(false);
        tableView->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
        tableView->setAutoScroll(true);
        tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
        tableView->setProperty("showDropIndicator", QVariant(false));
        tableView->setDragDropOverwriteMode(false);
        tableView->setAlternatingRowColors(true);
        tableView->setSelectionMode(QAbstractItemView::SingleSelection);
        tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
        tableView->setTextElideMode(Qt::ElideRight);
        tableView->setVerticalScrollMode(QAbstractItemView::ScrollPerItem);
        tableView->setHorizontalScrollMode(QAbstractItemView::ScrollPerItem);
        tableView->setShowGrid(true);
        tableView->setWordWrap(true);
        tableView->setCornerButtonEnabled(false);
        splitter->addWidget(tableView);
        tableView->horizontalHeader()->setHighlightSections(false);
        tableView->horizontalHeader()->setStretchLastSection(true);
        tableView->verticalHeader()->setVisible(false);
        tableView->verticalHeader()->setDefaultSectionSize(14);
        tableView->verticalHeader()->setHighlightSections(false);
        groupBox = new QGroupBox(splitter);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        QSizePolicy sizePolicy3(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy3.setHorizontalStretch(1);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(groupBox->sizePolicy().hasHeightForWidth());
        groupBox->setSizePolicy(sizePolicy3);
        groupBox->setMinimumSize(QSize(0, 104));
        groupBox->setMaximumSize(QSize(16777215, 104));
        groupBox->setFont(font);
        groupBox->setAutoFillBackground(false);
        groupBox->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
        groupBox->setFlat(false);
        groupBox->setCheckable(false);
        horizontalLayout_2 = new QHBoxLayout(groupBox);
        horizontalLayout_2->setSpacing(0);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        label = new QLabel(groupBox);
        label->setObjectName(QString::fromUtf8("label"));
        QSizePolicy sizePolicy4(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy4);
        label->setMinimumSize(QSize(192, 86));
        label->setMaximumSize(QSize(16777215, 86));
        label->setFont(font);
        label->setTextFormat(Qt::PlainText);
        label->setScaledContents(true);
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
        PropertiesWindow->setWindowTitle(QCoreApplication::translate("PropertiesWindow", "Properties", nullptr));
        groupBox->setTitle(QCoreApplication::translate("PropertiesWindow", "Description", nullptr));
        label->setText(QCoreApplication::translate("PropertiesWindow", "Information on the selected object and its value.", nullptr));
    } // retranslateUi

};

namespace Ui {
    class PropertiesWindow: public Ui_PropertiesWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PROPERTIESWINDOW_H
