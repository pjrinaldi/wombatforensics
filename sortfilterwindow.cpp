#include "sortfilterwindow.h"

SortFilterWindow::SortFilterWindow(QWidget* parent) : QDialog(parent), ui(new Ui::sortfilter)
{
    ui->setupUi(this);
    QSqlQueryModel* model = new QSqlQueryModel;
    model->setQuery("SELECT objectid, name, size FROM data");
    model->setHeaderData(0, Qt::Horizontal, tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, tr("Name"));
    model->setHeaderData(2, Qt::Horizontal, tr("Size"));
    ui->listView->setModel(model);
}

SortFilterWindow::~SortFilterWindow()
{
    this->close();
}

void SortFilterWindow::closeEvent(QCloseEvent* e)
{
    e->accept();
}
