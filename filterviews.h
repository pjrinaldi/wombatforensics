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
    class accessdatefilter;
    class analysistypefilter;
    class changedatefilter;
    class createdatefilter;
    class filetypefilter;
    class hashcategoryfilter;
    class hashfilter;
    class idfilter;
    class modifieddatefilter;
    class namefilter;
    class pathfilter;
    class sizefilter;
}

class FilterViews : public QWidget
{
    Q_OBJECT

public:
    FilterViews(QWidget* parent = 0);
    ~FilterViews();

private:
    Ui::accessdatefilter* uiadate;
    Ui::analysistypefilter* uiatype;
    Ui::changedatefilter* uichdate;
    Ui::createdatefilter* uicrdate;
    Ui::filetypefilter* uiftype;
    Ui::hashcategoryfilter* uihashc;
    Ui::hashfilter* uihash;
    Ui::idfilter* uiid;
    Ui::modifieddatefilter* uimdate;
    Ui::namefilter* uiname;
    Ui::pathfilter* uipath;
    Ui::sizefilter* uisize;
};

Q_DECLARE_METATYPE(FilterViews*);

#endif
