#include "messageviewer.h"

// Copyright 2015 Pasquale J. Rinaldi, Jr.
// Distrubted under the terms of the GNU General Public License version 2

MessageViewer::MessageViewer(QWidget* parent) : QDialog(parent), ui(new Ui::MessageViewer)
{
    ui->setupUi(this);
    msglog = ui->textEdit;
    ui->checkBox->setVisible(false);
    this->hide();
}

MessageViewer::~MessageViewer()
{
    delete ui;
    this->close();
}

void MessageViewer::HideClicked()
{
    this->hide();
    emit HideMessageViewerWindow(false);
}

void MessageViewer::closeEvent(QCloseEvent* e)
{
    emit HideMessageViewerWindow(false);
    e->accept();
}
