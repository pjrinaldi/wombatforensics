#ifndef MANAGECARVING_H
#define MANAGECARVING_H

#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>

#include "icons.h"

#include "/usr/local/include/fox-1.7/fx.h"

class ManageCarving : public FXDialogBox
{
    FXDECLARE(ManageCarving)

    private:
        FXVerticalFrame* mainframe;
        FXHorizontalFrame* hframe1;
	FXTable* filetypetable;
	FXButton* addrowbutton;
	FXButton* savebutton;
	FXButton* saveclosebutton;
	FXButton* closebutton;

    protected:
        ManageCarving() {}

    public:
        enum
        {
            ID_MANAGE = 103,
	    ID_ADDROW = 104,
	    ID_FTTABLE = 106,
            ID_LAST
        };
        ManageCarving(FXWindow* parent, const FXString& title);
        FXString ReturnManageCarving(void);
        void LoadManageCarving(FXString cursettings);

	long AddRow(FXObject*, FXSelector, void*);
};

FXDEFMAP(ManageCarving) ManageCarvingMap[]={
    FXMAPFUNC(SEL_COMMAND, ManageCarving::ID_ADDROW, ManageCarving::AddRow),
};

#endif // ABOUTBOX_H
