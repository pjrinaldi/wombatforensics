#include "viewermanager.h"

ViewerManager::ViewerManager(QWidget* parent) : QDialog(parent), ui(new Ui::ViewerManager)
{
    ui->setupUi(this);
    this->hide();
    externlist.clear();
    connect(ui->browsebutton, SIGNAL(clicked()), this, SLOT(ShowBrowser()));
    connect(ui->addbutton, SIGNAL(clicked()), this, SLOT(AddViewer()));
    connect(ui->removebutton, SIGNAL(clicked()), this, SLOT(RemoveSelected()));
    // query for any external viewers and add here if they exist.
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
    viewmodel = new ViewerModel(externlist);
    ui->listView->setModel(viewmodel);
    // ERROR WITH NO SUCH SIGNAL
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
    viewmodel->AddViewer(fileviewerpath);
    //bool viewexists = false;
    //int isdeleted = 0;
    //QSqlQuery existquery(fappdb);
    //existquery.prepare("SELECT path, deleted FROM externalviewers;");
    //existquery.exec();
    /*
    if(existquery.size() > 0)
    {
        while(existquery.next())
        {
            if(fileviewerpath.compare(existquery.value(0).toString()) == 0)
            {
                viewexists = true;
                isdeleted = existquery.value(1).toInt();
            }
        }
    }
    existquery.finish();
    if(viewexists == true)
    {
        if(isdeleted == 1)
        {
            existquery.prepare("UPDATE externalviewers SET deleted = 0 WHERE path = ?;");
            existquery.addBindValue(fileviewerpath);
            existquery.exec();
            existquery.next();
            existquery.finish();
        }
    }
    else
    {
        existquery.prepare("INSERT INTO externalviewers (path, deleted) VALUES(?, 0);");
        existquery.addBindValue(fileviewerpath);
        existquery.exec();
        existquery.next();
        existquery.finish();
    }
    */
    ui->lineEdit->setText("");
    ui->addbutton->setEnabled(false);
    // may need to reset the model or update it when the new values are added.
    //ui->listView->reset();
}

void ViewerManager::RemoveSelected()
{
    viewmodel->RemoveSelected(selectedindex);
    /*
    QSqlQuery remquery(fappdb);
    remquery.prepare("UPDATE externalviewers SET deleted = 1 WHERE path = ?;");
    remquery.addBindValue(selectedindex.sibling(selectedindex.row(), 0).data().toString());
    remquery.exec();
    remquery.next();
    remquery.finish();
    */
    ui->removebutton->setEnabled(false);
}

void ViewerManager::SelectionChanged(const QItemSelection &newitem, const QItemSelection &olditem)
{
    if(olditem.indexes().count() > 0)
    {
    }
       //QModelIndex oldselectedindex = olditem.indexes().at(0);
    if(newitem.indexes().count() > 0)
    {
        selectedindex = newitem.indexes().at(0);
        ui->removebutton->setEnabled(true);

    }
    /*
     *    if(previtem.indexes().count() > 0)
        oldselectedindex = previtem.indexes().at(0);
    if(curitem.indexes().count() > 0)
    {
        selectedindex = curitem.indexes().at(0);
        ui->actionView_Properties->setEnabled(true);
        ui->actionView_File->setEnabled(true);
        ui->actionView_Image_Gallery->setEnabled(true);
        ui->actionExport_Evidence->setEnabled(true);
        wombatvarptr->selectedobject.id = selectedindex.sibling(selectedindex.row(), 0).data().toInt(); // object id
        wombatvarptr->selectedobject.name = selectedindex.sibling(selectedindex.row(), 1).data().toString(); // object name
        wombatdatabase->GetObjectValues(); // now i have selectedobject.values.
        LoadHexContents();
        if(propertywindow->isVisible())
            UpdateProperties();
 
     *
     */ 
    // update selected item so i know what might get removed. i need to query the name. loop where it contains.
}
