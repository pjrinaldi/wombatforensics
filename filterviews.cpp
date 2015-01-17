#include "filterviews.h"

FilterViews::FilterViews(QWidget* parent) : QWidget(parent)
{
    uiadate->setupUi(this);
    uiatype->setupUi(this);
    uichdate->setupUi(this);
    uicrdate->setupUi(this);
    uiftype->setupUi(this);
    uihashc->setupUi(this);
    uihash->setupUi(this);
    uiid->setupUi(this);
    uimdate->setupUi(this);
    uiname->setupUi(this);
    uipath->setupUi(this);
    uisize->setupUi(this);
}

FilterViews::~FilterViews()
{
    this->close();
}
