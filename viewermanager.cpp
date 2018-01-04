#include "viewermanager.h"

// Copyright 2015 Pasquale J. Rinaldi, Jr.
// Distrubted under the terms of the GNU General Public License version 2

ViewerManager::ViewerManager(QWidget* parent) : QDialog(parent), ui(new Ui::ViewerManager)
{
    ui->setupUi(this);
    this->hide();
    //externallist.clear();
    //externlist.clear();
    connect(ui->browsebutton, SIGNAL(clicked()), this, SLOT(ShowBrowser()));
    connect(ui->addbutton, SIGNAL(clicked()), this, SLOT(AddViewer()));
    connect(ui->removebutton, SIGNAL(clicked()), this, SLOT(RemoveSelected()));
    connect(ui->listWidget, SIGNAL(itemSelectionChanged()), this, SLOT(SelectionChanged()));
    //qDebug() << "initialize viewermanager filename:" << viewerfile.fileName();
    UpdateList();
    /*
    viewerfile.open(QIODevice::ReadOnly | QIODevice::Text);
    while(!viewerfile.atEnd())
    {
        QString line = viewerfile.readLine();
        new QListWidgetItem(line, listWidget);
        //externallist.append(line);
    }
    viewerfile.close();
    */
    /*
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
    */
    //externallist = externlist;
    //viewmodel = new ViewerModel(externallist);
    //ui->listView->setModel(viewmodel);
    //connect(ui->listView->selectionModel(), SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)), this, SLOT(SelectionChanged(const QItemSelection &, const QItemSelection &)));
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
    viewerfile.open(QIODevice::Append | QIODevice::ReadOnly | QIODevice::Text);
    viewerfile.write(QString(fileviewerpath + "\n").toStdString().c_str());
    viewerfile.flush();
    //qDebug() << "viewer file add:" << viewerfile.readAll();
    viewerfile.close();
    //viewmodel->AddViewer(fileviewerpath);
    ui->lineEdit->setText("");
    ui->addbutton->setEnabled(false);
    UpdateList();
}

void ViewerManager::RemoveSelected()
{
    QString selectedviewer = ui->listWidget->currentItem()->text();
    QString tmpstring;
    //viewmodel->RemoveSelected(selectedindex);
    viewerfile.open(QIODevice::ReadWrite | QIODevice::Text);
    while(!viewerfile.atEnd())
    {
        QString line = viewerfile.readLine();
        if(!line.contains(selectedviewer))
            tmpstring.append(line + "\n");
        //if(!line.contains(selectedindex.sibling(selectedindex.row(), 0).data().toString()))
            //tmpstring.append(line + "\n");
    }
    qDebug() << "tmpstring" << tmpstring;
    viewerfile.write(tmpstring.toStdString().c_str());
    viewerfile.flush();
    //qDebug() << "viewer file remove:" << viewerfile.readAll();
    viewerfile.close();
    ui->removebutton->setEnabled(false);
    UpdateList();
}

void ViewerManager::SelectionChanged()
{
    /*
    if(olditem.indexes().count() > 0)
    {
    }
    if(newitem.indexes().count() > 0)
    {
        selectedindex = newitem.indexes().at(0);
        ui->removebutton->setEnabled(true);

    }
    */
    ui->removebutton->setEnabled(true);
}
void ViewerManager::UpdateList()
{
    ui->listWidget->clear();
    viewerfile.open(QIODevice::ReadOnly | QIODevice::Text);
    while(!viewerfile.atEnd())
    {
        QString line = viewerfile.readLine();
        new QListWidgetItem(line, ui->listWidget);
        //externallist.append(line);
    }
    viewerfile.close();
}
