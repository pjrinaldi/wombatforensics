#include "manageomniviewer.h"
#include "ui_manageomniviewer.h"

ManageOmniViewer::ManageOmniViewer(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ManageOmniViewer)
{
    ui->setupUi(this);
}

ManageOmniViewer::~ManageOmniViewer()
{
    delete ui;
}
