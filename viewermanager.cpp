#include "viewermanager.h"

ViewerManager::ViewerManager(QWidget* parent) : QDialog(parent), ui(new Ui::ViewerManager)
{
    ui->setupUi(this);
    this->hide();
    connect(ui->browsebutton, SIGNAL(clicked()), this, SLOT(ShowBrowser()));
    connect(ui->addbutton, SIGNAL(clicked()), this, SLOT(AddViewer()));
    connect(ui->removebutton, SIGNAL(clicked()), this, SLOT(RemoveSelected()));
    connect(ui->listView, SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)), this, SLOT(SelectionChanged(const QItemSelection &, const QItemSelection &)));
}

ViewerManager::~ViewerManager()
{
    this->close();
}

void ViewerManager::HideClicked()
{
    this->hide();
    emit HideManagerWindow(false);
}

void ViewerManager::closeEvent(QCloseEvent* e)
{
    emit HideManagerWindow(false);
    e->accept();
}

void ViewerManager::ShowBrowser()
{
}

void ViewerManager::AddViewer()
{
}

void ViewerManager::RemoveSelected()
{
}

void ViewerManager::SelectionChanged(const QItemSelection &newitem, const QItemSelection &olditem)
{
}
