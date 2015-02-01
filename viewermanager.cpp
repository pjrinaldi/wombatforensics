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
    QSqlQuery equery(appdb);
    equery.prepare("SELECT path FROM externalviewers WHERE deleted = 0;");
    equery.exec();
    if(equery.size() > 0)
    {
        while(equery.next())
        {
            externlist.append(equery.value(0).toString());
        }
    }
    equery.finish();
    viewmodel = new ViewerModel(externlist);
    ui->listView->setModel(viewmodel);
    // ERROR WITH NO SUCH SIGNAL
    //connect(ui->listView, SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)), this, SLOT(SelectionChanged(const QItemSelection &, const QItemSelection &)));
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
    bool viewexists = false;
    int isdeleted = 0;
    QSqlQuery existquery(appdb);
    existquery.prepare("SELECT path, deleted FROM externalviewers;");
    existquery.exec();
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
    ui->lineEdit->setText("");
    ui->addbutton->setEnabled(false);
    // may need to reset the model or update it when the new values are added.
    ui->listView->reset();
}

void ViewerManager::RemoveSelected()
{
    // if set deleted to 1;
}

void ViewerManager::SelectionChanged(const QItemSelection &newitem, const QItemSelection &olditem)
{
    // update selected item so i know what might get removed. i need to query the name. loop where it contains.
}
