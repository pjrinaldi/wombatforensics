#include "filterviews.h"

// Copyright 2013-2020 Pasquale J. Rinaldi, Jr.
// Distrubted under the terms of the GNU General Public License version 2

IdFilter::IdFilter(QWidget* parent) : QFrame(parent), ui(new Ui::IdFilter)
{
    ui->setupUi(this);
    this->hide();
    ui->vspinBox->setVisible(false);
    ui->vcheckBox->setVisible(false);
    ui->idlabel->setText("");
    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(HideClicked()));
    connect(ui->espinBox, SIGNAL(textChanged(QString)), this, SLOT(BuildId(QString)));
    connect(ui->vspinBox, SIGNAL(textChanged(QString)), this, SLOT(BuildId(QString)));
    connect(ui->pspinBox, SIGNAL(textChanged(QString)), this, SLOT(BuildId(QString)));
    connect(ui->fspinBox, SIGNAL(textChanged(QString)), this, SLOT(BuildId(QString)));
    connect(ui->echeckBox, SIGNAL(clicked()), this, SLOT(Rebuild()));
    connect(ui->vcheckBox, SIGNAL(clicked()), this, SLOT(Rebuild()));
    connect(ui->pcheckBox, SIGNAL(clicked()), this, SLOT(Rebuild()));
    connect(ui->fcheckBox, SIGNAL(clicked()), this, SLOT(Rebuild()));
}

IdFilter::~IdFilter()
{       
}

void IdFilter::DisplayFilter()
{
    QDir eviddir = QDir(wombatvariable.tmpmntpath);
    ui->idlabel->setText(filtervalues.idfilter);
    QStringList tmplist = filtervalues.idfilter.split("-", Qt::SkipEmptyParts);
    if(tmplist.count() > 0)
    {
        for(int i = 0; i < tmplist.count(); i++)
        {
            if(tmplist.at(i).contains("e"))
            {
                ui->echeckBox->setChecked(true);
                ui->espinBox->setEnabled(true);
                ui->espinBox->setValue(tmplist.at(i).mid(1).toInt());
            }
            /*
            else if(tmplist.at(i).contains("v"))
            {
                ui->vcheckBox->setChecked(true);
                ui->vspinBox->setEnabled(true);
                ui->vspinBox->setValue(tmplist.at(i).mid(1).toInt());
            }
            */
            else if(tmplist.at(i).contains("p"))
            {
                ui->pcheckBox->setChecked(true);
                ui->pspinBox->setEnabled(true);
                ui->pspinBox->setValue(tmplist.at(i).mid(1).toInt());
            }
            else if(tmplist.at(i).contains("f"))
            {
                ui->fcheckBox->setChecked(true);
                ui->fspinBox->setEnabled(true);
                ui->fspinBox->setValue(tmplist.at(i).mid(1).toInt());
            }
        }
    }
    QPoint cursorpos = this->mapFromGlobal(QCursor::pos());
    QPoint newpos = QPoint(cursorpos.x() - this->width(), cursorpos.y());
    if(this->pos().x() == 0)
        this->move(newpos);
    this->show();
}

void IdFilter::HideClicked()
{
    filtervalues.idfilter = ui->idlabel->text();
    this->hide();
    emit HeaderChanged();
}

void IdFilter::BuildId(QString curstring)
{
    int a = 0;
    builtstring = "";
    if(sender()->objectName().contains("espinBox"))
        estring = curstring;
    else if(sender()->objectName().contains("vspinBox"))
        vstring = curstring;
    else if(sender()->objectName().contains("pspinBox"))
        pstring = curstring;
    else if(sender()->objectName().contains("fspinBox"))
        fstring = curstring;
    if(ui->espinBox->isEnabled())
    {
        builtstring += estring;
        a++;
    }
    if(ui->vspinBox->isEnabled())
    {
        if(a > 0)
            builtstring += "-";
        builtstring += vstring;
        a++;
    }
    if(ui->pspinBox->isEnabled())
    {
        if(a > 0)
            builtstring += "-";
        builtstring += pstring;
        a++;
    }
    if(ui->fspinBox->isEnabled())
    {
        if(a > 0)
            builtstring += "-";
        builtstring += fstring;
    }
    ui->idlabel->setText(builtstring);
}

void IdFilter::Rebuild()
{
    if(sender()->objectName().contains("echeckBox"))
        emit ui->espinBox->textChanged(ui->espinBox->text());
    else if(sender()->objectName().contains("vcheckBox"))
        emit ui->vspinBox->textChanged(ui->vspinBox->text());
    else if(sender()->objectName().contains("pcheckBox"))
        emit ui->pspinBox->textChanged(ui->pspinBox->text());
    else if(sender()->objectName().contains("fcheckBox"))
        emit ui->fspinBox->textChanged(ui->fspinBox->text());
}

JumpHex::JumpHex(QWidget* parent) : QDialog(parent), ui(new Ui::JumpHex)
{
    ui->setupUi(this);
    this->hide();
    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(HideClicked()));
}

JumpHex::~JumpHex()
{
}

void JumpHex::DisplayFilter()
{
    /*
    QPoint cursorpos = this->mapFromGlobal(QCursor::pos());
    QPoint newpos = QPoint(cursorpos.x() - this->width(), cursorpos.y());
    if(this->pos().x() == 0)
        this->move(newpos);
    this->show();
    */
}

void JumpHex::HideClicked()
{
    jumpoffset = ui->lineEdit->text().toLongLong(0, 16);
    emit SetOffset(); 
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
    QPoint cursorpos = this->mapFromGlobal(QCursor::pos());
    QPoint newpos = QPoint(cursorpos.x() - this->width(), cursorpos.y());
    if(this->pos().x() == 0)
        this->move(newpos);
    this->show();
    /*
    if(this->pos().x() == 0)
        this->move(this->mapFromGlobal(QCursor::pos()));
    this->show();
    */
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
    QPoint cursorpos = this->mapFromGlobal(QCursor::pos());
    QPoint newpos = QPoint(cursorpos.x() - this->width(), cursorpos.y());
    if(this->pos().x() == 0)
        this->move(newpos);
    this->show();
    /*
    if(this->pos().x() == 0)
        this->move(this->mapFromGlobal(QCursor::pos()));
    this->show();
    */
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
    QPoint cursorpos = this->mapFromGlobal(QCursor::pos());
    QPoint newpos = QPoint(cursorpos.x() - this->width(), cursorpos.y());
    if(this->pos().x() == 0)
        this->move(newpos);
    this->show();
    /*
    if(this->pos().x() == 0)
        this->move(this->mapFromGlobal(QCursor::pos()));
    this->show();
    */
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
    QStringList tmptype;
    tmptype.clear();
    ui->typecomboBox->clear();
    // NEED TO FIGURE OUT HOW TO GET THE CORRECT ROW SO I AM PULLING ALL THE MATCHING SIGNATURE ENTRIES
    // MAYBE CALLING int treenodemodel->GetColumnIndex(QString "sig") which returns the index... this would apply for all the selectedindex as well...
    int colindex = treenodemodel->GetColumnIndex("sig");
    QModelIndexList indexlist = treenodemodel->match(treenodemodel->index(0, colindex, QModelIndex()), Qt::DisplayRole, QVariant("*"), -1, Qt::MatchFlags(Qt::MatchWildcard | Qt::MatchRecursive));
    for(int i=0; i < indexlist.count(); i++)
    {
        TreeNode* tmpnode = static_cast<TreeNode*>(indexlist.at(i).internalPointer());
        if(tmpnode->Data("sig").toString() != "0")
            tmptype.append(tmpnode->Data("sig").toString());
    }
    tmptype.removeDuplicates();
    for(int i=0; i < tmptype.count(); i++)
        ui->typecomboBox->addItem(tmptype.at(i));
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
    filtervalues.filetypebool = ui->typecheckBox->isChecked();
    if(filtervalues.filetypebool)
        filtervalues.filetype = ui->typecomboBox->currentText();
    this->hide();
    emit HeaderChanged();
}

TagFilter::TagFilter(QWidget* parent) : QWidget(parent), ui(new Ui::TagFilter)
{
    ui->setupUi(this);
    this->hide();
    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(HideClicked()));
}

TagFilter::~TagFilter()
{
    delete ui;
}

void TagFilter::DisplayFilter()
{
    QStringList tags;
    tags.clear();
    ui->tagcomboBox->clear();
    QFile tmpfile(wombatvariable.tmpmntpath + "bookmarks");
    QString tmpstr = "";
    tmpfile.open(QIODevice::ReadOnly | QIODevice::Text);
    if(tmpfile.isOpen())
        tmpstr = tmpfile.readLine();
    tmpfile.close();
    tags = tmpstr.split(",", Qt::SkipEmptyParts);
    for(int i=0; i < tags.count(); i++)
        ui->tagcomboBox->addItem(tags.at(i));
    if(filtervalues.tag.compare("") != 0)
        ui->tagcomboBox->setCurrentText(filtervalues.tag);
    QPoint cursorpos = this->mapFromGlobal(QCursor::pos());
    QPoint newpos = QPoint(cursorpos.x() - this->width(), cursorpos.y());
    if(this->pos().x() == 0)
        this->move(newpos);
    this->show();
}

void TagFilter::HideClicked()
{
    filtervalues.tagbool = ui->tagcheckBox->isChecked();
    if(filtervalues.tagbool)
        filtervalues.tag = ui->tagcomboBox->currentText();
    this->hide();
    emit HeaderChanged();
}

FileCategoryFilter::FileCategoryFilter(QWidget* parent) : QWidget(parent), ui(new Ui::FileCategoryFilter)
{
    ui->setupUi(this);
    this->hide();
    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(HideClicked()));
}

FileCategoryFilter::~FileCategoryFilter()
{
}

void FileCategoryFilter::DisplayFilter()
{
    QStringList tmpcategory;
    tmpcategory.clear();
    ui->categorycomboBox->clear();
    int colindex = treenodemodel->GetColumnIndex("cat");
    QModelIndexList indexlist = treenodemodel->match(treenodemodel->index(0, colindex, QModelIndex()), Qt::DisplayRole, QVariant("*"), -1, Qt::MatchFlags(Qt::MatchWildcard | Qt::MatchRecursive));
    for(int i=0; i < indexlist.count(); i++)
    {
        TreeNode* tmpnode = static_cast<TreeNode*>(indexlist.at(i).internalPointer());
        if(tmpnode->Data("cat").toString() != "0")
            tmpcategory.append(tmpnode->Data("cat").toString());
    }
    tmpcategory.removeDuplicates();
    for(int i=0; i < tmpcategory.count(); i++)
        ui->categorycomboBox->addItem(tmpcategory.at(i));
    if(filtervalues.filegroup.compare("") != 0)
        ui->categorycomboBox->setCurrentText(filtervalues.filegroup);
    QPoint cursorpos = this->mapFromGlobal(QCursor::pos());
    QPoint newpos = QPoint(cursorpos.x() - this->width(), cursorpos.y());
    if(this->pos().x() == 0)
        this->move(newpos);
    this->show();
}

void FileCategoryFilter::HideClicked()
{
    filtervalues.filegroupbool = ui->categorycheckBox->isChecked();
    if(filtervalues.filegroupbool)
        filtervalues.filegroup = ui->categorycomboBox->currentText();
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
    filtervalues.hashbool2 = ui->checkBox_2->isChecked();
    int colindex = treenodemodel->GetColumnIndex("hash");
    QModelIndexList indexlist = treenodemodel->match(treenodemodel->index(0, colindex, QModelIndex()), Qt::DisplayRole, QVariant("*"), -1, Qt::MatchFlags(Qt::MatchWildcard | Qt::MatchRecursive));
    for(int i=0; i < indexlist.count(); i++)
    {
        TreeNode* tmpnode = static_cast<TreeNode*>(indexlist.at(i).internalPointer());
        if(tmpnode->Data("hash").toString() == "0")
            filtervalues.hashlist.append(tmpnode->Data("hash").toString());
    }
    if(filtervalues.hashbool)
    {
        QStringList tmplist;
        tmplist.clear();
        filtervalues.hashlist.sort();
        for(int i = 1; i < filtervalues.hashlist.count(); i++)
        {
            if(filtervalues.hashlist.at(i).contains(filtervalues.hashlist.at(i-1)))
                tmplist.append(filtervalues.hashlist.at(i));
        }
        filtervalues.hashlist = tmplist;
        filtervalues.hashlist.removeDuplicates();
        filtervalues.hashlist.removeOne("0");
    }
    if(filtervalues.hashbool2)
    {
        filtervalues.hashfilter = ui->lineEdit->text();
    }
    this->hide();
    emit HeaderChanged();
}
