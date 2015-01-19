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

NameFilter::NameFilter(QWidget* parent) : QWidget(parent), ui(new Ui::NameFilter)
{
    ui->setupUi(this);
    this->hide();
    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(HideClicked()));
}

NameFilter::~NameFilter()
{
}

void NameFilter::DisplayFilter()
{
    if(this->pos().x() == 0)
        this->move(this->mapFromGlobal(QCursor::pos()));
    this->show();
}

void NameFilter::HideClicked()
{
    filtervalues.namebool = ui->checkBox->isChecked();
    if(filtervalues.namebool)
        filtervalues.namefilter = ui->lineEdit->text();
    this->hide();
}

PathFilter::PathFilter(QWidget* parent) : QWidget(parent), ui(new Ui::PathFilter)
{
    ui->setupUi(this);
    this->hide();
    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(HideClicked()));
}

PathFilter::~PathFilter()
{
}

void PathFilter::DisplayFilter()
{
    if(this->pos().x() == 0)
        this->move(this->mapFromGlobal(QCursor::pos()));
    this->show();
}

void PathFilter::HideClicked()
{
    filtervalues.pathbool = ui->checkBox->isChecked();
    if(filtervalues.pathbool)
        filtervalues.pathfilter = ui->lineEdit->text();
    this->hide();
}

SizeFilter::SizeFilter(QWidget* parent) : QWidget(parent), ui(new Ui::SizeFilter)
{
    ui->setupUi(this);
    this->hide();
    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(HideClicked()));
}

SizeFilter::~SizeFilter()
{
}

void SizeFilter::DisplayFilter()
{
    QSqlQuery sizequery(fcasedb);
    sizequery.prepare("SELECT MAX(size) FROM data;");
    sizequery.exec();
    sizequery.next();
    ui->morespinBox->setMaximum(sizequery.value(0).toInt());
    ui->lessspinBox->setMaximum(sizequery.value(0).toInt());
    sizequery.finish();
    if(this->pos().x() == 0)
        this->move(this->mapFromGlobal(QCursor::pos()));
    this->show();
}

void SizeFilter::HideClicked()
{
    filtervalues.maxsizebool = ui->morecheckBox->isChecked();
    if(filtervalues.maxsizebool)
        filtervalues.maxsize = ui->morespinBox->value();
    filtervalues.minsizebool = ui->lesscheckBox->isChecked();
    if(filtervalues.minsizebool)
        filtervalues.minsize = ui->lessspinBox->value();
    this->hide();
}

CreatedDateFilter::CreatedDateFilter(QWidget* parent) : QWidget(parent), ui(new Ui::CreatedDateFilter)
{
    ui->setupUi(this);
    this->hide();
    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(HideClicked()));
}

CreatedDateFilter::~CreatedDateFilter()
{
}

void CreatedDateFilter::DisplayFilter()
{
    ui->moredateTimeEdit->setDateTime(QDateTime::currentDateTimeUtc());
    ui->lessdateTimeEdit->setDateTime(QDateTime::currentDateTimeUtc());
    if(this->pos().x() == 0)
        this->move(this->mapFromGlobal(QCursor::pos()));
    this->show();
}

void CreatedDateFilter::HideClicked()
{
    filtervalues.maxcreatebool = ui->morecheckBox->isChecked();
    if(filtervalues.maxcreatebool)
        filtervalues.maxcreate = ui->moredateTimeEdit->dateTime();
    filtervalues.mincreatebool = ui->lesscheckBox->isChecked();
    if(filtervalues.mincreatebool)
        filtervalues.mincreate = ui->lessdateTimeEdit->dateTime();
    this->hide();
}
