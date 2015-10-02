#include "viewermanager.h"

ViewerManager::ViewerManager(QWidget* parent) : QDialog(parent), ui(new Ui::ViewerManager)
{
    ui->setupUi(this);
    this->hide();
    externlist.clear();
    connect(ui->browsebutton, SIGNAL(clicked()), this, SLOT(ShowBrowser()));
    connect(ui->addbutton, SIGNAL(clicked()), this, SLOT(AddViewer()));
    connect(ui->removebutton, SIGNAL(clicked()), this, SLOT(RemoveSelected()));
    if(!fappdb.isOpen())
        fappdb.open();
    QSqlQuery equery(fappdb);
    equery.prepare("SELECT path FROM externalviewers WHERE deleted = 0;");
    equery.exec();
    while(equery.next())
    {
        externlist.append(equery.value(0).toString());
    }
    equery.finish();
    externallist = externlist;
    viewmodel = new ViewerModel(externlist);
    ui->listView->setModel(viewmodel);
    connect(ui->listView->selectionModel(), SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)), this, SLOT(SelectionChanged(const QItemSelection &, const QItemSelection &)));
}

ViewerManager::~ViewerManager()
{
    delete ui;
    this->close();
}

void ViewerManager::HideClicked()
{
    this->hide();
    emit HideManagerWindow();
}

void ViewerManager::closeEvent(QCloseEvent* e)
{
    emit HideManagerWindow();
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
    viewmodel->AddViewer(fileviewerpath);
    ui->lineEdit->setText("");
    ui->addbutton->setEnabled(false);
}

void ViewerManager::RemoveSelected()
{
    viewmodel->RemoveSelected(selectedindex);
    ui->removebutton->setEnabled(false);
}

void ViewerManager::SelectionChanged(const QItemSelection &newitem, const QItemSelection &olditem)
{
    if(olditem.indexes().count() > 0)
    {
    }
    if(newitem.indexes().count() > 0)
    {
        selectedindex = newitem.indexes().at(0);
        ui->removebutton->setEnabled(true);

    }
}
