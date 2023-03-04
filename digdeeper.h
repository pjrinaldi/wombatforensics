#ifndef DIGDEEPER_H
#define DIGDEEPER_H

#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>
#include <filesystem>

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
	FXGroupBox* optionsgroup;
	FXVerticalFrame* vframe1;
	FXCheckButton* expandarchives;
	FXCheckButton* expandemail;
	FXCheckButton* imagethumbnails;
	FXCheckButton* videothumbnails;
	FXCheckButton* hashfiles;
	FXHorizontalFrame* hframe2;
	FXCheckButton* hashcomparison;
	FXList* hashlist;

        FXHorizontalFrame* hframe8;
        FXButton* cancelbutton;
        FXButton* savebutton;
        FXString casename = "";
        //FXString timezones = "America/New_York\nAmerica/Chicago\n";

    protected:
        DigDeeper() {}

    public:
        enum
        {
	    ID_FILES = 100,
            ID_LAST
        };
        DigDeeper(FXWindow* parent, const FXString& title);
        void ReturnDigging(std::vector<std::string>* digginglist);
        void SetCaseName(FXString cname);
        void LoadHashLists(void);
        void LoadFileCounts(void);
        //void LoadDigDeeper(FXString cursettings);

	long SetFilesRadioState(FXObject* sender, FXSelector, void*);
        //long SetCasePath(FXObject*, FXSelector, void*);
        //long SetReportPath(FXObject*, FXSelector, void*);
};

FXDEFMAP(DigDeeper) DigDeeperMap[]={
    FXMAPFUNC(SEL_COMMAND, DigDeeper::ID_FILES, DigDeeper::SetFilesRadioState),
    //FXMAPFUNC(SEL_COMMAND, DigDeeper::ID_CASEPATH, DigDeeper::SetCasePath),
    //FXMAPFUNC(SEL_COMMAND, DigDeeper::ID_REPORTPATH, DigDeeper::SetReportPath),
};
#endif // DIGDEEPER_H
