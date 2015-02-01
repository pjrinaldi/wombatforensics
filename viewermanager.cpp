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
    delete ui;
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
    fileviewerpath = QFileDialog::getOpenFileName(this, tr("Select Viewer Executable"), QDir::homePath());
    if(!fileviewerpath.isNull())
    {
        ui->lineEdit->setText(fileviewerpath);
        ui->addbutton->setEnabled(true);
    }
}

void ViewerManager::AddViewer()
{
    // check if it exists set to 1 else add to externalviewer db;
}

void ViewerManager::RemoveSelected()
{
    // if set deleted to 0;
}

void ViewerManager::SelectionChanged(const QItemSelection &newitem, const QItemSelection &olditem)
{
    // update selected item so i know what might get removed. i need to query the name. loop where it contains.
}
