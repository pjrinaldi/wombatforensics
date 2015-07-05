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
    ui->morespinBox->setMaximum(idquery.value(0).toULongLong());
    ui->morespinBox->setMinimum(1);
    ui->lessspinBox->setMaximum(idquery.value(0).toULongLong());
    ui->lessspinBox->setMinimum(1);
    ui->morespinBox->setValue(filtervalues.maxid);
    ui->lessspinBox->setValue(filtervalues.minid);
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
    emit HeaderChanged();
}

JumpFilter::JumpFilter(QWidget* parent) : QFrame(parent), ui(new Ui::JumpFilter)
{
    ui->setupUi(this);
    this->hide();
    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(HideClicked()));
}

JumpFilter::~JumpFilter()
{
}

void JumpFilter::DisplayFilter()
{
    QPoint cursorpos = this->mapFromGlobal(QCursor::pos());
    QPoint newpos = QPoint(cursorpos.x() - this->width(), cursorpos.y());
    if(this->pos().x() == 0)
        this->move(newpos);
    this->show();
}

void JumpFilter::HideClicked()
{
    jumpoffset = ui->lineEdit->text().toULongLong(0, 16);
    emit SetOffset(); 
    this->hide();
}

FileJumpFilter::FileJumpFilter(QWidget* parent) : QFrame(parent), ui(new Ui::JumpFilter)
{
    ui->setupUi(this);
    this->hide();
    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(HideClicked()));
}

FileJumpFilter::~FileJumpFilter()
{
}

void FileJumpFilter::DisplayFilter()
{
    QPoint cursorpos = this->mapFromGlobal(QCursor::pos());
    QPoint newpos = QPoint(cursorpos.x() - this->width(), cursorpos.y());
    if(this->pos().x() == 0)
        this->move(newpos);
    this->show();
}

void FileJumpFilter::HideClicked()
{
    filejumpoffset = ui->lineEdit->text().toULongLong(0, 16);
    emit SetOffset(); 
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
    ui->lineEdit->setText(filtervalues.namefilter);
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
    emit HeaderChanged();
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
    ui->lineEdit->setText(filtervalues.pathfilter);
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
    emit HeaderChanged();
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
    ui->morespinBox->setMaximum(sizequery.value(0).toULongLong());
    ui->lessspinBox->setMaximum(sizequery.value(0).toULongLong());
    ui->morespinBox->setValue(filtervalues.maxsize);
    ui->lessspinBox->setValue(filtervalues.minsize);
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
    emit HeaderChanged();
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
    QPoint cursorpos = this->mapFromGlobal(QCursor::pos());
    QPoint newpos = QPoint(cursorpos.x() - this->width(), cursorpos.y());
    if(this->pos().x() == 0)
        this->move(newpos);
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
    emit HeaderChanged();
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
    QPoint cursorpos = this->mapFromGlobal(QCursor::pos());
    QPoint newpos = QPoint(cursorpos.x() - this->width(), cursorpos.y());
    if(this->pos().x() == 0)
        this->move(newpos);
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
    emit HeaderChanged();
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
    QPoint cursorpos = this->mapFromGlobal(QCursor::pos());
    QPoint newpos = QPoint(cursorpos.x() - this->width(), cursorpos.y());
    if(this->pos().x() == 0)
        this->move(newpos);
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
    emit HeaderChanged();
}

ChangedDateFilter::ChangedDateFilter(QWidget* parent) : QWidget(parent), ui(new Ui::ChangedDateFilter)
{
    ui->setupUi(this);
    this->hide();
    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(HideClicked()));
}

ChangedDateFilter::~ChangedDateFilter()
{
}

void ChangedDateFilter::DisplayFilter()
{
    ui->moredateTimeEdit->setDateTime(QDateTime::fromTime_t(filtervalues.maxchange, Qt::OffsetFromUTC, 0));
    ui->lessdateTimeEdit->setDateTime(QDateTime::fromTime_t(filtervalues.minchange, Qt::OffsetFromUTC, 0));
    QPoint cursorpos = this->mapFromGlobal(QCursor::pos());
    QPoint newpos = QPoint(cursorpos.x() - this->width(), cursorpos.y());
    if(this->pos().x() == 0)
        this->move(newpos);
    this->show();
}

void ChangedDateFilter::HideClicked()
{
    filtervalues.maxchangebool = ui->morecheckBox->isChecked();
    if(filtervalues.maxchangebool)
        filtervalues.maxchange = ui->moredateTimeEdit->dateTime().toTime_t();
    filtervalues.minchangebool = ui->lesscheckBox->isChecked();
    if(filtervalues.minchangebool)
        filtervalues.minchange = ui->lessdateTimeEdit->dateTime().toTime_t();
    this->hide();
    emit HeaderChanged();
}

FileTypeFilter::FileTypeFilter(QWidget* parent) : QWidget(parent), ui(new Ui::FileTypeFilter)
{
    ui->setupUi(this);
    this->hide();
    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(HideClicked()));
}

FileTypeFilter::~FileTypeFilter()
{
}

void FileTypeFilter::DisplayFilter()
{
    QStringList tmpcategory, tmptype;
    tmpcategory.clear();
    tmptype.clear();
    ui->categorycomboBox->clear();
    ui->typecomboBox->clear();
    QSqlQuery typequery(fcasedb);
    typequery.prepare("SELECT DISTINCT filemime FROM data WHERE objecttype = 5;");
    if(typequery.exec())
    {
        while(typequery.next())
        {
            tmpcategory.append(typequery.value(0).toString().split("/", QString::SkipEmptyParts).at(0));
            if(typequery.value(0).toString().split("/", QString::SkipEmptyParts).count() == 2)
                tmptype.append(typequery.value(0).toString().split("/", QString::SkipEmptyParts).at(1));
        }
    }
    else
        qDebug() << fcasedb.lastError().text();
    typequery.finish();
    tmpcategory.removeDuplicates();
    tmptype.removeDuplicates();
    for(int i=0; i < tmpcategory.count(); i++)
        ui->categorycomboBox->addItem(tmpcategory.at(i));
    for(int i=0; i < tmptype.count(); i++)
        ui->typecomboBox->addItem(tmptype.at(i));
    if(filtervalues.filecategory.compare("") != 0)
        ui->categorycomboBox->setCurrentText(filtervalues.filecategory);
    if(filtervalues.filetype.compare("") != 0)
        ui->typecomboBox->setCurrentText(filtervalues.filetype);
    QPoint cursorpos = this->mapFromGlobal(QCursor::pos());
    QPoint newpos = QPoint(cursorpos.x() - this->width(), cursorpos.y());
    if(this->pos().x() == 0)
        this->move(newpos);
    this->show();
}

void FileTypeFilter::HideClicked()
{
    filtervalues.filecategorybool = ui->categorycheckBox->isChecked();
    if(filtervalues.filecategorybool)
        filtervalues.filecategory = ui->categorycomboBox->currentText();
    filtervalues.filetypebool = ui->typecheckBox->isChecked();
    if(filtervalues.filetypebool)
        filtervalues.filetype = ui->typecomboBox->currentText();
    this->hide();
    emit HeaderChanged();
}

HashFilter::HashFilter(QWidget* parent) : QWidget(parent), ui(new Ui::HashFilter)
{
    ui->setupUi(this);
    this->hide();
    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(HideClicked()));
}

HashFilter::~HashFilter()
{
}

void HashFilter::DisplayFilter()
{
    QPoint cursorpos = this->mapFromGlobal(QCursor::pos());
    QPoint newpos = QPoint(cursorpos.x() - this->width(), cursorpos.y());
    if(this->pos().x() == 0)
        this->move(newpos);
    this->show();
}

void HashFilter::HideClicked()
{
    filtervalues.hashbool = ui->checkBox->isChecked();
    if(filtervalues.hashbool)
    {
        filtervalues.hashlist.clear();
        filtervalues.hashcount.clear();
        filtervalues.hashidlist.clear();
        QSqlQuery hashquery(fcasedb);
        hashquery.prepare("SELECT md5, COUNT(md5) FROM data GROUP BY md5 HAVING COUNT(md5) > 1 AND md5 != '';");
        if(hashquery.exec())
        {
            while(hashquery.next())
            {
                filtervalues.hashlist.append(hashquery.value(0).toString());
                filtervalues.hashcount.append(hashquery.value(1).toULongLong());
            }
        }
        else
            qDebug() << fcasedb.lastError().text();
        for(int i=0; i < filtervalues.hashlist.count(); i++)
        {
            hashquery.prepare("SELECT objectid FROM data WHERE md5 = ? ORDER BY objectid DESC limit ?;");
            hashquery.addBindValue(filtervalues.hashlist.at(i));
            hashquery.addBindValue(filtervalues.hashcount.at(i) - 1);
            if(hashquery.exec())
            {
                while(hashquery.next())
                {
                    filtervalues.hashidlist.append(hashquery.value(0).toULongLong());
                }
            }
            else
                qDebug() << fcasedb.lastError().text();
        }
        hashquery.finish();
    }
    this->hide();
    emit HeaderChanged();
}
