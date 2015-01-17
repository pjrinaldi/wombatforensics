#ifndef FILTERVIEWS_H
#define FILTERVIEWS_H

#include "wombatinclude.h"
#include "ui_accesseddatefilter.h"
#include "ui_analysistypefilter.h"
#include "ui_changeddatefilter.h"
#include "ui_createdatefilter.h"
#include "ui_hashcategoryfilter.h"
#include "ui_hashfilter.h"
#include "ui_idfilter.h"
#include "ui_modifieddatefilter.h"
#include "ui_namefilter.h"
#include "ui_pathfilter.h"
#include "ui_sizefilter.h"

namespace Ui {
    class FilterViews;
}

class FilterViews : public QWidget
{
    Q_OBJECT

public:
    FilterViews(QWidget* parent = 0);
    ~FilterViews();

private:
    Ui::FilterViews *ui;
};

Q_DECLARE_METATYPE(FilterViews*);

#endif
