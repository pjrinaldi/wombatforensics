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
	//FXText* filetypetext;
	FXTable* filetypetable;
	FXButton* addrowbutton;
	FXButton* savebutton;
	FXButton* saveclosebutton;
	FXButton* closebutton;
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
        FXHorizontalFrame* hframe7;
        FXLabel* carvingmanagerlabel;
        FXButton* carvingmanagerbutton;
        FXHorizontalFrame* hframe8;
        FXButton* cancelbutton;
        FXButton* savebutton;
        FXString timezones = "America/New_York\nAmerica/Chicago\n";
	*/

    protected:
        ManageCarving() {}
        //long onSomeCommand(FXObject* sender, FXSelector sel, void* ptr);

    public:
        enum
        {
            ID_MANAGE = 103,
	    ID_ADDROW = 104,
	    ID_FTTEXT = 105,
	    ID_FTTABLE = 106,
            //ID_CANCEL = 1,
            //ID_ACCEPT = 2,
            //ID_TREELIST = 1,
            //ID_LISTSELECT = 100,
            //ID_NEWTAG = 101,
            //ID_EDITTAG = 102,
            //ID_REMTAG = 103,
            ID_LAST
        };
        ManageCarving(FXWindow* parent, const FXString& title);
        FXString ReturnManageCarving(void);
        void LoadManageCarving(FXString cursettings);

        long SetCasePath(FXObject*, FXSelector, void*);
        long SetReportPath(FXObject*, FXSelector, void*);
        long OpenCarvingManager(FXObject*, FXSelector, void*);
	long AddRow(FXObject*, FXSelector, void*);
};

FXDEFMAP(ManageCarving) ManageCarvingMap[]={
    FXMAPFUNC(SEL_COMMAND, ManageCarving::ID_ADDROW, ManageCarving::AddRow),
    //FXMAPFUNC(SEL_COMMAND, ManageCarving::ID_REPORTPATH, ManageCarving::SetReportPath),
    //FXMAPFUNC(SEL_COMMAND, ManageCarving::ID_MANAGE, ManageCarving::OpenCarvingManager),
};

#endif // ABOUTBOX_H
