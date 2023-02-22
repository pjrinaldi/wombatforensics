#ifndef FILTERVIEW_H
#define FILTERVIEW_H

#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>

#include "icons.h"
#include "directories.h"
#include "common.h"

#include "/usr/local/include/fox-1.7/fx.h"

class FilterView : public FXDialogBox
{
    FXDECLARE(FilterView)

    private:
        FXVerticalFrame* mainframe;
        FXTable* filteredlist;
        FXIcon* filtericon;
        /*
        FXHorizontalFrame* hframe1;
	FXTable* filetypetable;
	FXButton* addrowbutton;
	FXButton* savebutton;
	FXButton* saveclosebutton;
	FXButton* closebutton;
        */

    protected:
        FilterView() {}
        ~FilterView();

    public:
        enum
        {
            //ID_MANAGE = 103,
	    //ID_ADDROW = 104,
	    //ID_FTTABLE = 106,
            ID_LAST
        };
        FilterView(FXWindow* parent, const FXString& title);
        void FitColumnContents(int col);
        void AlignColumn(FXTable* curtable, int row, FXuint justify);
        //FXString ReturnFilterView(void);
        void LoadFilterView(FXString cursettings);
        void ApplyFilter(CurrentItem* curitem, int colindex, std::string filterstring);

	//long AddRow(FXObject*, FXSelector, void*);
};

FXDEFMAP(FilterView) FilterViewMap[]={
    //FXMAPFUNC(SEL_COMMAND, FilterView::ID_ADDROW, FilterView::AddRow),
};

#endif // FILTERVIEW_H
