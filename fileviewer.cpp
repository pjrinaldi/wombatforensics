#include "fileviewer.h"

// Copyright 2013-2019 Pasquale J. Rinaldi, Jr.
// Distrubted under the terms of the GNU General Public License version 2

FileViewer::FileViewer(QWidget* parent) : QMainWindow(parent), ui(new Ui::FileViewer)
{
    ui->setupUi(this);
    connect(ui->filehexview, SIGNAL(currentAddressChanged(qint64)), this, SLOT(SetOffsetLabel(qint64)));
    connect(ui->filehexview, SIGNAL(selectionChanged()), this, SLOT(UpdateSelectValue()));
    this->statusBar()->setSizeGripEnabled(true);
    jumpto = new QLineEdit();
    jumplabel = new QLabel("Jump To:");
    jumpbutton = new QPushButton("Go");
    jumpto->setPlaceholderText("Enter Hex Offset");
    hexbutton = new QAction("Find in Hex/Ascii", this);
    hexbutton->setIcon(QIcon(":/bar/hexsrch"));
    ui->toolBar->addWidget(jumplabel);
    ui->toolBar->addWidget(jumpto);
    ui->toolBar->addWidget(jumpbutton);
    ui->toolBar->addSeparator();
    ui->toolBar->addAction(hexbutton);
    selectedoffset = new QLabel(this);
    selectedoffset->setText("Offset: 00");
    selectedhex = new QLabel(this);
    selectedhex->setText("Length: 0");
    this->statusBar()->addWidget(selectedoffset, 0);
    this->statusBar()->addWidget(selectedhex, 0);
    connect(jumpbutton, SIGNAL(clicked()), this, SLOT(JumpHex()));
    connect(hexbutton, SIGNAL(triggered()), this, SLOT(ShowSearch()));
    searchdialog = new SearchDialog(ui->filehexview, this);
}

FileViewer::~FileViewer()
{
    this->close();
}

void FileViewer::HideClicked()
{
    this->hide();
    emit HideFileViewer(false);
}

void FileViewer::closeEvent(QCloseEvent* event)
{
    emit HideFileViewer(false);
    event->accept();
}

void FileViewer::SetOffsetLabel(qint64 pos)
{
    QString label;
    label = "Offset: ";
    char buffer[64];
    #if _LARGEFILE_SOURCE
    sprintf(buffer,"0x%llx",pos);
    #else
    sprintf(buffer,"0x%x",pos);
    #endif
    label += buffer;
    selectedoffset->setText(label);
}

void FileViewer::BypassColor(bool bypass)
{
    ui->filehexview->BypassColor(bypass);
}


void FileViewer::UpdateSelectValue()
{
    QByteArray selectionbytes = ui->filehexview->selectionToByteArray();
    QString tmptext = "Length: " + QString::number(selectionbytes.size());
    selectedhex->setText(tmptext);
}

void FileViewer::UpdateHexView()
{
    hexfile.setFileName(hexstring);
    ui->filehexview->setData(hexfile);
    BypassColor(true);
    this->show();
}

void FileViewer::JumpHex()
{
    ui->filehexview->setCursorPosition(jumpto->text().toLongLong(0, 16)*2);
    ui->filehexview->ensureVisible();
}

void FileViewer::ShowSearch()
{
    searchdialog->show();
}
