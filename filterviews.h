#ifndef FILTERVIEWS_H
#define FILTERVIEWS_H

#include "wombatinclude.h"
#include "globals.h"
#include "ui_accesseddatefilter.h"
#include "ui_analysistypefilter.h"
#include "ui_changeddatefilter.h"
#include "ui_createddatefilter.h"
#include "ui_filetypefilter.h"
#include "ui_hashcategoryfilter.h"
#include "ui_hashfilter.h"
#include "ui_idfilter.h"
#include "ui_modifieddatefilter.h"
#include "ui_namefilter.h"
#include "ui_pathfilter.h"
#include "ui_sizefilter.h"

namespace Ui {
    /*
    class accessdatefilter;
    class analysistypefilter;
    class changedatefilter;
    class filetypefilter;
    class hashcategoryfilter;
    class hashfilter;
    */
    class IdFilter;
    class NameFilter;
    class PathFilter;
    class SizeFilter;
    class CreatedDateFilter;
    /*
    class modifieddatefilter;
    */
}

class IdFilter : public QFrame
{
    Q_OBJECT
public:
    IdFilter(QWidget* parent = 0);
    ~IdFilter();
    void DisplayFilter(void);

private slots:
    void HideClicked();

private:
    Ui::IdFilter* ui;
};

class NameFilter : public QWidget
{
    Q_OBJECT

public:
    NameFilter(QWidget* parent = 0);
    ~NameFilter();
    void DisplayFilter(void);

private slots:
    void HideClicked();

private:
    Ui::NameFilter* ui;
};

class PathFilter : public QWidget
{
    Q_OBJECT

public:
    PathFilter(QWidget* parent = 0);
    ~PathFilter();
    void DisplayFilter(void);

private slots:
    void HideClicked();

private:
    Ui::PathFilter* ui;
};

class SizeFilter : public QWidget
{
    Q_OBJECT
public:
    SizeFilter(QWidget* parent = 0);
    ~SizeFilter();
    void DisplayFilter(void);

private slots:
    void HideClicked();

private:
    Ui::SizeFilter* ui;
};

class CreatedDateFilter : public QWidget
{
    Q_OBJECT
public:
    CreatedDateFilter(QWidget* parent = 0);
    ~CreatedDateFilter();
    void DisplayFilter();

private slots:
    void HideClicked();

private:
    Ui::CreatedDateFilter* ui;
};

Q_DECLARE_METATYPE(IdFilter*);
Q_DECLARE_METATYPE(NameFilter*);
Q_DECLARE_METATYPE(PathFilter*);
Q_DECLARE_METATYPE(SizeFilter*);
Q_DECLARE_METATYPE(CreatedDateFilter*);

#endif
