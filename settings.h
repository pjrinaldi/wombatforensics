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
        FXHorizontalFrame* hframe8;
        FXButton* cancelbutton;
        FXButton* savebutton;
        FXString timezones = "America/New_York\nAmerica/Chicago\n";

    protected:
        Settings() {}

    public:
        enum
        {
            ID_CASEPATH = 1,
            ID_REPORTPATH = 2,
            ID_LAST
        };
        Settings(FXWindow* parent, const FXString& title);
        FXString ReturnSettings(void);
        void LoadSettings(FXString cursettings);

        long SetCasePath(FXObject*, FXSelector, void*);
        long SetReportPath(FXObject*, FXSelector, void*);
};

FXDEFMAP(Settings) SettingsMap[]={
    FXMAPFUNC(SEL_COMMAND, Settings::ID_CASEPATH, Settings::SetCasePath),
    FXMAPFUNC(SEL_COMMAND, Settings::ID_REPORTPATH, Settings::SetReportPath),
};

#endif // ABOUTBOX_H
