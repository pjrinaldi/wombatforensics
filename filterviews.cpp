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
    ui->moredateTimeEdit->setDateTime(QDateTime::fromTime_t(filtervalues.maxcreate, Qt::OffsetFromUTC, 0));
    ui->lessdateTimeEdit->setDateTime(QDateTime::fromTime_t(filtervalues.mincreate, Qt::OffsetFromUTC, 0));
    if(this->pos().x() == 0)
        this->move(this->mapFromGlobal(QCursor::pos()));
    this->show();
}

void CreatedDateFilter::HideClicked()
{
    filtervalues.maxcreatebool = ui->morecheckBox->isChecked();
    if(filtervalues.maxcreatebool)
        filtervalues.maxcreate = ui->moredateTimeEdit->dateTime().toTime_t();
    filtervalues.mincreatebool = ui->lesscheckBox->isChecked();
    if(filtervalues.mincreatebool)
        filtervalues.mincreate = ui->lessdateTimeEdit->dateTime().toTime_t();
    this->hide();
}

AccessedDateFilter::AccessedDateFilter(QWidget* parent) : QWidget(parent), ui(new Ui::AccessedDateFilter)
{
    ui->setupUi(this);
    this->hide();
    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(HideClicked()));
}

AccessedDateFilter::~AccessedDateFilter()
{
}

void AccessedDateFilter::DisplayFilter()
{
    ui->moredateTimeEdit->setDateTime(QDateTime::fromTime_t(filtervalues.maxaccess, Qt::OffsetFromUTC, 0));
    ui->lessdateTimeEdit->setDateTime(QDateTime::fromTime_t(filtervalues.minaccess, Qt::OffsetFromUTC, 0));
    if(this->pos().x() == 0)
        this->move(this->mapFromGlobal(QCursor::pos()));
    this->show();
}

void AccessedDateFilter::HideClicked()
{
    filtervalues.maxaccessbool = ui->morecheckBox->isChecked();
    if(filtervalues.maxaccessbool)
        filtervalues.maxaccess = ui->moredateTimeEdit->dateTime().toTime_t();
    filtervalues.minaccessbool = ui->lesscheckBox->isChecked();
    if(filtervalues.minaccessbool)
        filtervalues.minaccess = ui->lessdateTimeEdit->dateTime().toTime_t();
    this->hide();
}

ModifiedDateFilter::ModifiedDateFilter(QWidget* parent) : QWidget(parent), ui(new Ui::ModifiedDateFilter)
{
    ui->setupUi(this);
    this->hide();
    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(HideClicked()));
}

ModifiedDateFilter::~ModifiedDateFilter()
{
}

void ModifiedDateFilter::DisplayFilter()
{
    ui->moredateTimeEdit->setDateTime(QDateTime::fromTime_t(filtervalues.maxmodify, Qt::OffsetFromUTC, 0));
    ui->lessdateTimeEdit->setDateTime(QDateTime::fromTime_t(filtervalues.minmodify, Qt::OffsetFromUTC, 0));
    if(this->pos().x() == 0)
        this->move(this->mapFromGlobal(QCursor::pos()));
    this->show();
}

void ModifiedDateFilter::HideClicked()
{
    filtervalues.maxmodifybool = ui->morecheckBox->isChecked();
    if(filtervalues.maxmodifybool)
        filtervalues.maxmodify = ui->moredateTimeEdit->dateTime().toTime_t();
    filtervalues.minmodifybool = ui->lesscheckBox->isChecked();
    if(filtervalues.minmodifybool)
        filtervalues.minmodify = ui->lessdateTimeEdit->dateTime().toTime_t();
    this->hide();
}
