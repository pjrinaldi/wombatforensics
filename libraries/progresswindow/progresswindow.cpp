#include "progresswindow.h"
#include "ui_progresswindow.h"

ProgressWindow::ProgressWindow(QWidget *parent) : QDialog(parent), ui(new Ui::ProgressWindow)
{
    ui->setupUi(this);
}

ProgressWindow::~ProgressWindow()
{
    delete ui;
}

QString ProgressWindow::progressWindowAction()
{
    return tr("View Progress Window...");
}

QString ProgressWindow::progressWindowIcon()
{
    return tr(":/progresswindow/window");
}

void ProgressWindow::ShowWindow()
{
    show();
}

