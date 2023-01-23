#ifndef ABOUTBOX_H
#define ABOUTBOX_H

#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>

#include "icons.h"

#include "/usr/local/include/fox-1.7/fx.h"

class AboutBox : public FXDialogBox
{
    FXDECLARE(AboutBox)

    private:
        FXVerticalFrame* mainframe;
	FXPNGImage* mainimage;
	FXImageFrame* imgframe;
        FXLabel* mainlabel;
        std::vector<std::string>* tags = NULL;

    protected:
        AboutBox() {}

        long onSomeCommand(FXObject* sender, FXSelector sel, void* ptr);

    public:
        enum
        {
            ID_TREELIST = 1,
            ID_LISTSELECT = 100,
            ID_NEWTAG = 101,
            ID_EDITTAG = 102,
            ID_REMTAG = 103,
            ID_LAST
        };
        AboutBox(FXWindow* parent, const FXString& title);
};

#endif // ABOUTBOX_H
