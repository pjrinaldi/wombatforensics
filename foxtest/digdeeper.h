#ifndef DIGDEEPER_H
#define DIGDEEPER_H

#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>

#include "icons.h"

#include "/usr/local/include/fox-1.7/fx.h"

class DigDeeper : public FXDialogBox
{
    FXDECLARE(DigDeeper)

    private:
        FXVerticalFrame* mainframe;
	FXGroupBox* filesgroup;
        FXHorizontalFrame* hframe1;
	FXRadioButton* selectedradio;
	FXRadioButton* checkedradio;
	FXRadioButton* allradio;
	/*
        FXLabel* thumbsizelabel;
        FXSpinner* thumbsizespinner;
        FXHorizontalFrame* hframe2;
        FXLabel* vidthumblabel;
        FXSpinner* vidthumbspinner;
        FXHorizontalFrame* hframe3;
        FXLabel* casepathlabel;
        FXTextField* casepathtextfield;
        FXButton* casepathbutton;
        FXHorizontalFrame* hframe4;
        FXLabel* reportpathlabel;
        FXTextField* reportpathtextfield;
        FXButton* reportpathbutton;
        FXHorizontalFrame* hframe5;
        FXLabel* reporttzlabel;
        FXComboBox* reporttzcombo;
        FXHorizontalFrame* hframe6;
        FXLabel* autosavelabel;
        FXSpinner* autosavespinner;
	*/
        FXHorizontalFrame* hframe8;
        FXButton* cancelbutton;
        FXButton* savebutton;
        //FXString timezones = "America/New_York\nAmerica/Chicago\n";

    protected:
        DigDeeper() {}

    public:
        enum
        {
	    ID_FILES = 100,
            //ID_CASEPATH = 1,
            //ID_REPORTPATH = 2,
            ID_LAST
        };
        DigDeeper(FXWindow* parent, const FXString& title);
        //FXString ReturnDigDeeper(void);
        //void LoadDigDeeper(FXString cursettings);

        //long SetCasePath(FXObject*, FXSelector, void*);
        //long SetReportPath(FXObject*, FXSelector, void*);
};

/*
FXDEFMAP(DigDeeper) DigDeeperMap[]={
    //FXMAPFUNC(SEL_COMMAND, DigDeeper::ID_CASEPATH, DigDeeper::SetCasePath),
    //FXMAPFUNC(SEL_COMMAND, DigDeeper::ID_REPORTPATH, DigDeeper::SetReportPath),
};
*/
#endif // DIGDEEPER_H
