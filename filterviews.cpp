#include "filterviews.h"

FilterViews::FilterViews(QWidget* parent) : QWidget(parent)
{
    ui->setupUi(this);
}

FilterViews::~FilterViews()
{
    this->close();
}
