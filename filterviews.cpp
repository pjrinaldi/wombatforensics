#include "filterviews.h"

IdFilter::IdFilter(QWidget* parent) : QFrame(parent), ui(new Ui::IdFilter)
{
    ui->setupUi(this);
    this->hide();
}

IdFilter::~IdFilter()
{
}

void IdFilter::DisplayFilter()
{
    //qDebug() << "cur pos:" << this->pos();
    //this->move(this->mapFromParent(QCursor::pos()));
    if(this->pos().x() == 0)
        this->move(this->mapFromGlobal(QCursor::pos()));
    this->show();
}
