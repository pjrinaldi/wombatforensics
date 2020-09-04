#include "newforimg.h"
#include "imagefunctions.h"

// Copyright 2013-2020 Pasquale J. Rinaldi, Jr.
// Distrubted under the terms of the GNU General Public License version 2

ForImgDialog::ForImgDialog(QWidget* parent) : QDialog(parent), ui(new Ui::ForImgDialog)
{
    ui->setupUi(this);
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
    connect(ui->cancelbutton, SIGNAL(clicked()), this, SLOT(HideClicked()), Qt::DirectConnection);
    connect(ui->createbutton, SIGNAL(clicked()), this, SLOT(CreateImage()), Qt::DirectConnection);
    connect(ui->browsebutton, SIGNAL(clicked()), this, SLOT(GetFolder()), Qt::DirectConnection);
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
    if(ui->rawradio->isChecked())
    {
	//unsigned long long curpos = 0;
	unsigned long long totalbytes = 0;
	totalbytes = GetTotalBytes(ui->sourcecombo->currentText().toStdString());
	ReadBytes(ui->sourcecombo->currentText().toStdString(), QString(ui->pathedit->text() + "/" + ui->nameedit->text() + ".dd").toStdString());
	qDebug() << "totalbytes:" << totalbytes;
	/*
	QFile infile(ui->sourcecombo->currentText());
	infile.open(QIODevice::ReadOnly);
	infile.seek(0);
	//QByteArray tmparray;
	//QByteArray tmparray = infile.read(8);
	//qDebug() << "1st 8 bytes:" << tmparray.toHex();
	QFile outfile(ui->pathedit->text() + "/" + ui->nameedit->text() + ".dd");
	outfile.open(QIODevice::Append);
	while(curpos <= totalbytes)
	{
	    //tmparray.clear();
	    infile.seek(curpos);
	    //tmparray = infile.read(4096);
	    outfile.write(infile.read(4096));
	    curpos = curpos + 4096;
	    qDebug() << "percent complete:" << (curpos/totalbytes) * 100.0;
	}
	infile.close();
	outfile.close();
	*/
	/*
	QFile infile(ui->sourcecombo->currentText());
	isopen = infile.open(QIODevice::ReadOnly);
	qDebug() << "is open:" << isopen;
	qint64 totalbytes = infile.size();
	qDebug() << "totalbytes:" << totalbytes;
	infile.close();
	*/
	/*
        QString rawstr = QString("dc3dd if=" + ui->sourcecombo->currentText() + " hash=md5 log=" + ui->nameedit->text() + ".log" + " of=" + ui->pathedit->text() + "/" + ui->nameedit->text() + ".dd");
	qDebug() << "rawstr:" << rawstr;
	//QProcess::execute(rawstr);
	QProcess* rawimg = new QProcess(this);
	rawimg->start(rawstr);
	//rawimg->start("dc3dd", QStringList() << QString("if=" + ui->sourcecombo->currentText()) << "hash=md5" << QString("log=" + ui->nameedit->text() + ".log") << QString("of=" + ui->pathedit->text() + "/" + ui->nameedit->text() + ".dd"));
	//*/
	/*
	if(!rawimg->waitForFinished())
	    qDebug() << "failed:" << rawimg->errorString();
	else
	    qDebug() << "finished:" << rawimg->readAll();
	*/

        //qDebug() << "image type:" << "raw image";
    }
    else if(ui->aff4radio->isChecked())
    {
        //qDebug() << "image type:" << "aff4 image";
    }
    else if(ui->sfsradio->isChecked())
    {
        //qDebug() << "image type:" << "sfs image";
    }
    // ensure values are valid and filled in/selected otherwise popup error and don't close...
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

