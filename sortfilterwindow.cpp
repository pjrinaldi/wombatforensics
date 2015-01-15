#include "sortfilterwindow.h"

SortFilterWindow::SortFilterWindow(QWidget* parent) : QDialog(parent), ui(new Ui::sortfilter)
{
    ui->setupUi(this);
    model = new QSqlQueryModel;
}

void SortFilterWindow::ShowQuery()
{
    model->setQuery("SELECT objectid, name, size FROM data", fcasedb);
    if(model->lastError().isValid())
        qDebug() << model->lastError();
    model->setHeaderData(0, Qt::Horizontal, tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, tr("Name"));
    model->setHeaderData(2, Qt::Horizontal, tr("Size"));
    ui->tableView->setModel(model);
}

SortFilterWindow::~SortFilterWindow()
{
    this->close();
}

void SortFilterWindow::closeEvent(QCloseEvent* e)
{
    e->accept();
}
