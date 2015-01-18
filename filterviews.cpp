#include "filterviews.h"

IdFilter::IdFilter(QWidget* parent) : QFrame(parent), ui(new Ui::IdFilter)
{
    ui->setupUi(this);
    this->hide();
    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(HideClicked()));
}

IdFilter::~IdFilter()
{       
}

void IdFilter::DisplayFilter()
{
    QSqlQuery idquery(fcasedb);
    idquery.prepare("SELECT COUNT(objectid) FROM data;");
    idquery.exec();
    idquery.next();
    ui->morespinBox->setMaximum(idquery.value(0).toInt());
    ui->morespinBox->setMinimum(1);
    ui->lessspinBox->setMaximum(idquery.value(0).toInt());
    ui->lessspinBox->setMinimum(1);
    idquery.finish();
    if(this->pos().x() == 0)
        this->move(this->mapFromGlobal(QCursor::pos()));
    this->show();
}

void IdFilter::HideClicked()
{
    filtervalues.maxidbool = ui->morecheckBox->isChecked();
    filtervalues.minidbool = ui->lesscheckBox->isChecked();
    if(filtervalues.maxidbool)
        filtervalues.maxid = ui->morespinBox->value();
    if(filtervalues.minidbool)
        filtervalues.minid = ui->lessspinBox->value();
    this->hide();
}
