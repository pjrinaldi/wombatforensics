#include "viewermanager.h"

// Copyright 2013-2019 Pasquale J. Rinaldi, Jr.
// Distrubted under the terms of the GNU General Public License version 2

ViewerManager::ViewerManager(QWidget* parent) : QDialog(parent), ui(new Ui::ViewerManager)
{
    ui->setupUi(this);
    this->hide();
    connect(ui->browsebutton, SIGNAL(clicked()), this, SLOT(ShowBrowser()));
    connect(ui->addbutton, SIGNAL(clicked()), this, SLOT(AddViewer()));
    connect(ui->removebutton, SIGNAL(clicked()), this, SLOT(RemoveSelected()));
    connect(ui->listWidget, SIGNAL(itemSelectionChanged()), this, SLOT(SelectionChanged()));
    UpdateList();
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
    fileviewerpath = QFileDialog::getOpenFileName(this, tr("Select Viewer Executable"), QDir::homePath(), "", NULL, QFileDialog::DontUseNativeDialog);
    if(!fileviewerpath.isNull())
    {
        ui->lineEdit->setText(fileviewerpath);
        ui->addbutton->setEnabled(true);
    }
}

void ViewerManager::AddViewer()
{
    viewerfile.open(QIODevice::Append);
    viewerfile.write(QString(fileviewerpath + ",").toStdString().c_str());
    viewerfile.close();
    ui->lineEdit->setText("");
    ui->addbutton->setEnabled(false);
    UpdateList();
}

void ViewerManager::RemoveSelected()
{
    QString selectedviewer = ui->listWidget->currentItem()->text();
    QString tmpstring;
    viewerfile.open(QIODevice::ReadOnly);
    QStringList tmplist = QString(viewerfile.readLine()).split(",", QString::SkipEmptyParts);
    viewerfile.close();
    for(int i=0; i < tmplist.count(); i++)
    {
        if(tmplist.at(i).contains(selectedviewer))
            tmplist.removeAt(i);
    }
    tmplist.removeDuplicates();
    for(int i=0; i < tmplist.count(); i++)
    {
        tmpstring.append(tmplist.at(i));
        tmpstring.append(",");
    }
    viewerfile.open(QIODevice::WriteOnly);
    viewerfile.write(tmpstring.toStdString().c_str());
    viewerfile.close();
    ui->removebutton->setEnabled(false);
    UpdateList();
}

void ViewerManager::SelectionChanged()
{
    ui->removebutton->setEnabled(true);
}
void ViewerManager::UpdateList()
{
    QString debugstr;
    ui->listWidget->clear();
    viewerfile.open(QIODevice::ReadOnly);
    QStringList itemlist = QString(viewerfile.readLine()).split(",", QString::SkipEmptyParts);
    itemlist.removeDuplicates();
    viewerfile.close();
    for(int i=0; i < itemlist.count(); i++)
    {
        new QListWidgetItem(itemlist.at(i), ui->listWidget);
    }
}
