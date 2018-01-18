#include "propertieswindow.h"

// Copyright 2015 Pasquale J. Rinaldi, Jr.
// Distrubted under the terms of the GNU General Public License version 2

PropertiesWindow::PropertiesWindow(QWidget* parent) : QDialog(parent), ui(new Ui::PropertiesWindow)
{
    ui->setupUi(this);
}

PropertiesWindow::~PropertiesWindow()
{
    this->close();
}

void PropertiesWindow::HideClicked()
{
    this->hide();
    emit HidePropertyWindow(false);
}

void PropertiesWindow::closeEvent(QCloseEvent* event)
{
    emit HidePropertyWindow(false);
    event->accept();
}

void PropertiesWindow::UpdateTableView()
{
    pmodel = new PropertyModel(propertylist);
    ui->tableView->setModel(pmodel);
    ui->tableView->hideColumn(2);
    for(int i=0; i < pmodel->rowCount(); i++)
    {
        if(pmodel->index(i, 1).data().toString().size() > 30)
            ui->tableView->resizeRowToContents(i);
    }
    connect(ui->tableView->selectionModel(), SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)), this, SLOT(SelectionChanged(const QItemSelection &, const QItemSelection &)));
}

void PropertiesWindow::SelectionChanged(const QItemSelection &selitem, const QItemSelection &deselitem)
{
    if(deselitem.indexes().count() > 0)
        oldselectedindex = deselitem.indexes().at(0);
    selectedindex = selitem.indexes().at(0);
    ui->label->setText(selectedindex.sibling(selectedindex.row(), 2).data().toString());
}
