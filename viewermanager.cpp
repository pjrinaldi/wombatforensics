#include "viewermanager.h"

ViewerManager::ViewerManager(QWidget* parent) : QDialog(parent), ui(new Ui::ViewerManager)
{
    ui->setupUi(this);
    this->hide();
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

