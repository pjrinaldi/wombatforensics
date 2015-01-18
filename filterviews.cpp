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
    this->show();
}
