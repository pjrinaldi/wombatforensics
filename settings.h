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
        FXHorizontalFrame* hframe9;
        FXLabel* vidlabel;
        FXTextField* vidtextfield;
        FXButton* vidbutton;
        FXHorizontalFrame* hframe10;
        FXLabel* htmllabel;
        FXTextField* htmltextfield;
        FXButton* htmlbutton;
        FXHorizontalFrame* hframe11;
        FXLabel* pdflabel;
        FXTextField* pdftextfield;
        FXButton* pdfbutton;
        FXHorizontalFrame* hframe12;
        FXLabel* imglabel;
        FXTextField* imgtextfield;
        FXButton* imgbutton;
        FXHorizontalFrame* hframe13;
        FXLabel* hexlabel;
        FXTextField* hextextfield;
        FXButton* hexbutton;

    protected:
        Settings() {}

    public:
        enum
        {
            ID_CASEPATH = 101,
            ID_REPORTPATH = 102,
            ID_VIDPATH = 103,
            ID_HTMLPATH = 104,
            ID_PDFPATH = 105,
            ID_IMGPATH = 106,
            ID_HEXPATH = 107,
            ID_LAST
        };
        Settings(FXWindow* parent, const FXString& title);
        FXString ReturnSettings(void);
        void LoadSettings(FXString cursettings);

        long SetCasePath(FXObject*, FXSelector, void*);
        long SetReportPath(FXObject*, FXSelector, void*);
        long SetVideoPath(FXObject*, FXSelector, void*);
        long SetHtmlPath(FXObject*, FXSelector, void*);
        long SetPdfPath(FXObject*, FXSelector, void*);
        long SetImagePath(FXObject*, FXSelector, void*);
        long SetHexPath(FXObject*, FXSelector, void*);
};

FXDEFMAP(Settings) SettingsMap[]={
    FXMAPFUNC(SEL_COMMAND, Settings::ID_CASEPATH, Settings::SetCasePath),
    FXMAPFUNC(SEL_COMMAND, Settings::ID_REPORTPATH, Settings::SetReportPath),
    FXMAPFUNC(SEL_COMMAND, Settings::ID_VIDPATH, Settings::SetVideoPath),
    FXMAPFUNC(SEL_COMMAND, Settings::ID_HTMLPATH, Settings::SetHtmlPath),
    FXMAPFUNC(SEL_COMMAND, Settings::ID_PDFPATH, Settings::SetPdfPath),
    FXMAPFUNC(SEL_COMMAND, Settings::ID_IMGPATH, Settings::SetImagePath),
    FXMAPFUNC(SEL_COMMAND, Settings::ID_HEXPATH, Settings::SetHexPath),
};

#endif // ABOUTBOX_H
