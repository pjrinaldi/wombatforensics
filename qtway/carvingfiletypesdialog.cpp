#include "carvingfiletypesdialog.h"

// Copyright 2013-2020 Pasquale J. Rinaldi, Jr.
// Distrubted under the terms of the GNU General Public License version 2

CarvingFileTypesDialog::CarvingFileTypesDialog(QWidget* parent) : QDialog(parent), ui(new Ui::CarvingFileTypesDialog)
{
    ui->setupUi(this);
    ui->filetypeseditor->setVisible(false);
    connect(ui->addrowpushbutton, SIGNAL(clicked()), this, SLOT(AddRow()));
    connect(ui->closebutton, SIGNAL(clicked()), this, SLOT(close()));
    connect(ui->savepushbutton, SIGNAL(clicked()), this, SLOT(Save()));
    connect(ui->saveclosepushbutton, SIGNAL(clicked()), this, SLOT(SaveClose()));
    //decodedstring = "default";
    //qDebug() << "decodedstring at start:" << decodedstring;
    //FindCodecs();
    //ui->comboBox->clear();
    //foreach(QTextCodec* codec, codecs)
    //    ui->comboBox->addItem(codec->name(), codec->mibEnum());
    //connect(ui->comboBox, SIGNAL(activated(int)), this, SLOT(UpdateEncoding(int)));
}

CarvingFileTypesDialog::~CarvingFileTypesDialog()
{
    delete ui;
    this->close();
}

void CarvingFileTypesDialog::HideClicked()
{
}

void CarvingFileTypesDialog::ShowText()
{
    // Load Files..
    int trowcount = ui->filetypestablewidget->rowCount();
    QString homepath = QDir::homePath();
    homepath += "/.local/share/wombatforensics/";
    QFile ctypes(homepath + "carvetypes");
    if(!ctypes.isOpen())
        ctypes.open(QIODevice::ReadOnly | QIODevice::Text);
    if(ctypes.isOpen())
    {
        QTextStream in(&ctypes);
        int rowcount = 0;
        while(!in.atEnd())
        {
            if(rowcount == trowcount)
                ui->filetypestablewidget->setRowCount(trowcount + 5);
            QString tmpstr = "";
            QStringList linelist = in.readLine().split(",");
            for(int i=0; i < linelist.count(); i++)
            {
                ui->filetypestablewidget->setItem(rowcount, i, new QTableWidgetItem(linelist.at(i)));
                ui->filetypestablewidget->item(rowcount, i)->setFlags(ui->filetypestablewidget->item(rowcount, i)->flags() ^ Qt::ItemIsEditable);
            }
            //qDebug() << "rowcount:" << rowcount;
            rowcount++;
        }
        //ui->filetypeseditor->setPlainText(ctypes.readAll());
    }
    ctypes.close();
    for(int i=0; i < ui->filetypestablewidget->columnCount(); i++)
        ui->filetypestablewidget->resizeColumnToContents(i);
    this->show();
}

void CarvingFileTypesDialog::Save()
{
    QString homepath = QDir::homePath();
    homepath += "/.local/share/wombatforensics/carvetypes";
    //qDebug() << "homepath:" << homepath;
    QFile ctypes(homepath);
    if(!ctypes.isOpen())
        ctypes.open(QIODevice::WriteOnly | QIODevice::Text);
    //else
        //qDebug() << "ctypes is already open";
    if(ctypes.isOpen())
    {
        QTextStream out(&ctypes);
        //qDebug() << "rowcount:" << ui->filetypestablewidget->rowCount();
        for(int i=0; i < ui->filetypestablewidget->rowCount(); i++)
        {
            if(ui->filetypestablewidget->item(i,0))
                out << ui->filetypestablewidget->item(i, 0)->text() << "," << ui->filetypestablewidget->item(i, 1)->text() << "," << ui->filetypestablewidget->item(i, 2)->text() << "," << ui->filetypestablewidget->item(i, 3)->text() << "," << ui->filetypestablewidget->item(i, 4)->text() << "," << ui->filetypestablewidget->item(i, 5)->text() << Qt::endl;
        }
    }
    //else
    //    qDebug() << "ctypes failed to open.";
    ctypes.close();
}

void CarvingFileTypesDialog::SaveClose()
{
    Save();
    this->close();
}

void CarvingFileTypesDialog::AddRow()
{
    ui->filetypestablewidget->insertRow(ui->filetypestablewidget->rowCount());
}
/*
void CarvingFileTypesDialog::ShowText(const QModelIndex &index)
{
    //curindex = index;
    //curobjaddr = index.sibling(index.row(), 10).data().toString().split("-f").at(1).toLongLong();
    //UpdateEncoding(0);
    QFile tfile(wombatvariable.tmpfilepath + index.sibling(index.row(), 11).data().toString() + "-fhex");
    if(!tfile.isOpen())
        tfile.open(QIODevice::ReadOnly | QIODevice::Text);
    if(tfile.isOpen())
        decodedstring = tfile.readAll();
    tfile.close();
    ui->textEdit->setPlainText(decodedstring);
    this->setWindowTitle(QString("Text Viewer - ") + QString(index.sibling(index.row(), 11).data().toString()));
    this->show();
}
*/

void CarvingFileTypesDialog::closeEvent(QCloseEvent* e)
{
    emit HideCarvingFileTypesDialogWindow(false);
    e->accept();
}
/*
void CarvingFileTypesDialog::FindCodecs()
{
    QMap<QString, QTextCodec*> codecmap;
    QRegExp iso8859regex("ISO[- ]8859-([0-9]+).*");

    foreach(int mib, QTextCodec::availableMibs())
    {
        QTextCodec* codec = QTextCodec::codecForMib(mib);

        QString sortkey = codec->name().toUpper();
        int rank;

        if(sortkey.startsWith("UTF-8"))
            rank = 1;
        else if(sortkey.startsWith("UTF-16"))
            rank = 2;
        else if(iso8859regex.exactMatch(sortkey))
        {
            if(iso8859regex.cap(1).size() == 1)
                rank = 3;
            else
                rank = 4;
        }
        else
            rank = 5;
        sortkey.prepend(QChar('0' + rank));
        codecmap.insert(sortkey, codec);
    }
    codecs = codecmap.values();
}

void CarvingFileTypesDialog::UpdateEncoding(int unused)
{
    if(unused < 0)
    {
        // remove warning
    }
    //int mib = ui->comboBox->itemData(ui->comboBox->currentIndex()).toInt();
    //qDebug() << "combobox number:" << ui->comboBox->currentIndex();
    //QTextCodec* codec = QTextCodec::codecForMib(mib);
    //QTextCodec::ConverterState state;
    //decodedstring = codec->toUnicode(txtdata.constData(), txtdata.size(), &state);
    //ui->textEdit->setPlainText(decodedstring);
    //ui->textEdit->setPlainText(QString(txtdata));
}
*/
