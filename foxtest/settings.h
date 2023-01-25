#ifndef SETTINGS_H
#define SETTINGS_H

#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>

#include "icons.h"

#include "/usr/local/include/fox-1.7/fx.h"

class Settings : public FXDialogBox
{
    FXDECLARE(Settings)

    private:
        FXVerticalFrame* mainframe;
        FXHorizontalFrame* hframe1;
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

    protected:
        Settings() {}
        //long onSomeCommand(FXObject* sender, FXSelector sel, void* ptr);

    public:
        /*
        enum
        {
            ID_SAVE = 1,
            //ID_CANCEL = 1,
            //ID_ACCEPT = 2,
            //ID_TREELIST = 1,
            //ID_LISTSELECT = 100,
            //ID_NEWTAG = 101,
            //ID_EDITTAG = 102,
            //ID_REMTAG = 103,
            ID_LAST
        };
        */
        Settings(FXWindow* parent, const FXString& title);
        FXString ReturnSettings(void);
};

#endif // ABOUTBOX_H
