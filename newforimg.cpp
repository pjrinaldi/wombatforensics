#include "newforimg.h"

// Copyright 2013-2020 Pasquale J. Rinaldi, Jr.
// Distrubted under the terms of the GNU General Public License version 2

ForImgDialog::ForImgDialog(QWidget* parent) : QDialog(parent), ui(new Ui::ForImgDialog)
{
    ui->setupUi(this);
    // DEVICE LISTING FOR IMAGING...
    devicelist.clear();
    ped_device_probe_all();
    PedDevice* tmpdevice = ped_device_get_next(NULL);
    ui->sourcecombo->addItem(QString::fromStdString(std::string(tmpdevice->path)));
    while(tmpdevice != NULL)
    {
        tmpdevice = tmpdevice->next;
        if(tmpdevice != NULL)
            ui->sourcecombo->addItem(QString::fromStdString(std::string(tmpdevice->path)));
    }
    // END DEVICE LISTING...

    connect(ui->cancelbutton, SIGNAL(clicked()), this, SLOT(HideClicked()), Qt::DirectConnection);
    connect(ui->createbutton, SIGNAL(clicked()), this, SLOT(CreateImage()), Qt::DirectConnection);
    connect(ui->browsebutton, SIGNAL(clicked()), this, SLOT(GetFolder()), Qt::DirectConnection);
    /*
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->label->setText("");
    connect(ui->treeWidget, SIGNAL(itemSelectionChanged()), this, SLOT(KeySelected()), Qt::DirectConnection);
    connect(ui->tableWidget, SIGNAL(itemSelectionChanged()), this, SLOT(ValueSelected()), Qt::DirectConnection);
    //connect(ui->tableWidget, SIGNAL(itemDoubleClicked(QTableWidgetItem*)), this, SLOT(DoubleClick(QTableWidgetItem*)), Qt::DirectConnection);
    QStringList taglist;
    taglist.clear();
    tagmenu = new QMenu(ui->tableWidget);
    bookmarkfile.open(QIODevice::ReadOnly | QIODevice::Text);
    if(bookmarkfile.isOpen())
	taglist = QString(bookmarkfile.readLine()).split(",", Qt::SkipEmptyParts);
    bookmarkfile.close();
    QAction* newtagaction = new QAction("New Tag", tagmenu);
    newtagaction->setIcon(QIcon(":/bar/newtag"));
    connect(newtagaction, SIGNAL(triggered()), this, SLOT(CreateNewTag()));
    tagmenu->addAction(newtagaction);
    tagmenu->addSeparator();
    for(int i=0; i < taglist.count(); i++)
    {
	QAction* tmpaction = new QAction(taglist.at(i), tagmenu);
	tmpaction->setIcon(QIcon(":/bar/addtotag"));
	tmpaction->setData(QVariant("t" + QString::number(i)));
	connect(tmpaction, SIGNAL(triggered()), this, SLOT(SetTag()));
	tagmenu->addAction(tmpaction);
    }
    tagmenu->addSeparator();
    QAction* remtagaction = new QAction("Remove Tag", tagmenu);
    remtagaction->setIcon(QIcon(":/bar/tag-rem"));
    connect(remtagaction, SIGNAL(triggered()), this, SLOT(RemoveTag()));
    tagmenu->addAction(remtagaction);
    ui->tableWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->tableWidget, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(TagMenu(const QPoint &)), Qt::DirectConnection);
    /*
    connect(ui->cancelbutton, SIGNAL(clicked()), this, SLOT(HideClicked()));
    connect(ui->carvebutton, SIGNAL(clicked()), this, SLOT(Assign()));
    connect(ui->titlelineedit, SIGNAL(editingFinished()), this, SLOT(UpdateAssign()));
    ui->carvebutton->setEnabled(false);
    UpdateList();
    */
}

ForImgDialog::~ForImgDialog()
{
    delete ui;
}

void ForImgDialog::HideClicked()
{
    this->close();
}

void ForImgDialog::CreateImage()
{
    qDebug() << "image name:" << ui->nameedit->text();
    qDebug() << "image path:" << ui->pathedit->text();
    qDebug() << "source device:" << ui->sourcecombo->currentText();
    if(ui->rawradio->isChecked())
    {
        qDebug() << "raw image selected:" << QString("dc3dd if=" + ui->sourcecombo->currentText() + " hash=md5 log=" + ui->nameedit->text() + ".log" + " of=" + ui->pathedit->text() + "/" + ui->nameedit->text() + ".dd");
        qDebug() << "image type:" << "raw image";
    }
    else if(ui->aff4radio->isChecked())
    {
        qDebug() << "image type:" << "aff4 image";
    }
    else if(ui->sfsradio->isChecked())
    {
        qDebug() << "image type:" << "sfs image";
    }
    //qDebug() << "get entered values and launch forensic image external process here...";
    this->close();
}

void ForImgDialog::GetFolder()
{
    QString userdir = QFileDialog::getExistingDirectory(this, tr("Select Destination"), QDir::homePath(), QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    ui->pathedit->setText(userdir);
}
void ForImgDialog::closeEvent(QCloseEvent* e)
{
    e->accept();
}

