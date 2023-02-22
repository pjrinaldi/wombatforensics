#ifndef FILTERS_H
#define FILTERS_H

#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>

#include "common.h"

#include "/usr/local/include/fox-1.7/fx.h"

class Filters : public FXDialogBox
{
    FXDECLARE(Filters)

    private:
        FXVerticalFrame* mainframe;
        FXText* msglog;
        FXFont* plainfont;
        char dtbuf[35];

    protected:
        Filters() {}
        ~Filters();

    public:
        enum
        {
            ID_LOG = 1,
            ID_LAST
        };
        Filters(FXWindow* parent, const FXString& title);
        void AddMsg(FXString msg);
};

#endif // FILTERS_H
