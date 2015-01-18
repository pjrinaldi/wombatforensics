#ifndef FILTERVIEWS_H
#define FILTERVIEWS_H

#include "wombatinclude.h"
#include "ui_accesseddatefilter.h"
#include "ui_analysistypefilter.h"
#include "ui_changeddatefilter.h"
#include "ui_createdatefilter.h"
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
    class createdatefilter;
    class filetypefilter;
    class hashcategoryfilter;
    class hashfilter;
    */
    class IdFilter;
    /*
    class modifieddatefilter;
    class namefilter;
    class pathfilter;
    class sizefilter;
    */
}

class IdFilter : public QFrame
{
    Q_OBJECT
public:
    IdFilter(QWidget* parent = 0);
    ~IdFilter();
    void DisplayFilter(void);

private:
    Ui::IdFilter* ui;
};

Q_DECLARE_METATYPE(IdFilter*);

#endif
