#include "filecarvingdialog.h"
#include "ui_filecarvingdialog.h"

// Copyright 2013-2020 Pasquale J. Rinaldi, Jr.
// Distrubted under the terms of the GNU General Public License version 2

FileCarvingDialog::FileCarvingDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FileCarvingDialog)
{
    ui->setupUi(this);
    connect(ui->processButton, SIGNAL(clicked()), this, SLOT(EnableProcess()));
}

FileCarvingDialog::~FileCarvingDialog()
{
    delete ui;
}

void FileCarvingDialog::PopulateFileTypes()
{
    QString homepath = QDir::homePath();
    homepath += "/.local/share/wombatforensics/";
    QFile ctypes(homepath + "carvetypes");
    QStringList categorylist;
    categorylist.clear();
    QStringList linelist;
    linelist.clear();
    if(!ctypes.isOpen())
        ctypes.open(QIODevice::ReadOnly | QIODevice::Text);
    if(ctypes.isOpen())
    {
        QTextStream in(&ctypes);
        while(!in.atEnd())
        {
            QString tmpstr = in.readLine();
            linelist.append(tmpstr);
            categorylist.append(tmpstr.split(",").first());
        }
    ctypes.close();
    }
    categorylist.removeDuplicates();
    for(int i=0; i < categorylist.count(); i++)
    {
        QTreeWidgetItem* tmpitem = new QTreeWidgetItem(ui->filetypetree);
        tmpitem->setText(0, categorylist.at(i));
        tmpitem->setCheckState(0, Qt::Unchecked);
        tmpitem->setFlags(Qt::ItemIsAutoTristate|Qt::ItemIsSelectable|Qt::ItemIsEnabled|Qt::ItemIsUserCheckable);
        for(int j=0; j < linelist.count(); j++)
        {
            if(linelist.at(j).split(",").first().contains(categorylist.at(i)))
            {
                QTreeWidgetItem* tmpsubitem = new QTreeWidgetItem(tmpitem);
                tmpsubitem->setText(0, linelist.at(j).split(",").at(1));
                tmpsubitem->setCheckState(0, Qt::Unchecked);
            }
        }
    }
}

void FileCarvingDialog::PopulatePartitions(QStringList plist)
{
    for(int i=0; i < plist.count(); i++)
    {
	new QListWidgetItem(plist.at(i), ui->partitionlist);
    }
}

void FileCarvingDialog::EnableProcess()
{
    this->close();
    QStringList selectedpartitions;
    selectedpartitions.clear();
    for(int i=0; i < ui->partitionlist->selectedItems().count(); i++)
	selectedpartitions.append(ui->partitionlist->selectedItems().at(i)->text().split(":").first());
    QStringList selectedfiletypes;
    selectedfiletypes.clear();
    foreach(QTreeWidgetItem* item, ui->filetypetree->findItems("", Qt::MatchContains | Qt::MatchRecursive))
    {
	if(item->checkState(0) == 2 && item->parent())
	    selectedfiletypes.append(item->text(0));
    }
    emit StartCarve(selectedpartitions, selectedfiletypes);
    this->close();
}
