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
        FXLabel* sortlabel;
        FXSpinner* idspinner;
        FXButton* okbutton;
        //FXText* msglog;
        //FXFont* plainfont;
        //char dtbuf[35];

        int columnindex = 0;

    protected:
        Filters() {}
        ~Filters();

    public:
        enum
        {
            //ID_SPINID = 100,
            //ID_LOG = 1,
            ID_LAST
        };
        Filters(FXWindow* parent, const FXString& title);
        //void AddMsg(FXString msg);
        void SetIndex(int colindex);
        void SetRange(int maxindex);
        std::string ReturnFilter(void);
};

#endif // FILTERS_H
