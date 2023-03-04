#ifndef MANAGEHASHLIST_H
#define MANAGEHASHLIST_H

#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>
#include <filesystem>

#include "icons.h"

#include "/usr/local/include/fox-1.7/fx.h"

class ManageHashList : public FXDialogBox
{
    FXDECLARE(ManageHashList)

    private:
        FXVerticalFrame* mainframe;
        FXVerticalFrame* buttonframe;
        FXHorizontalFrame* subframe;
        FXLabel* mainlabel;
        FXList* whllist;
        FXLabel* binlabel;
        FXButton* browsebutton;
        FXString hashstring;
        FXString casename;
	FXButton* emptybutton;
        FXButton* rembutton;
        FXButton* savebutton;

        std::vector<std::string>* hashlists = NULL;

    protected:
        ManageHashList() {}

        long onSomeCommand(FXObject* sender, FXSelector sel, void* ptr);

    public:
        enum
        {
            ID_LISTSELECT = 100,
	    ID_EMPTY = 101,
            ID_BROWSE = 102,
            ID_REMBIN = 103,
            ID_LAST
        };
        ManageHashList(FXWindow* parent, const FXString& title);
        void SetHashList(std::vector<std::string>* hashlist);
        void SetCaseName(FXString casename);
        void LoadHashList(void);
        void UpdateList();
        long RemoveWhl(FXObject*, FXSelector, void*);
        long ListSelection(FXObject*, FXSelector, void*);
        long SetHashPath(FXObject*, FXSelector, void*);
	long CreateEmptyList(FXObject*, FXSelector, void*);

};

FXDEFMAP(ManageHashList) ManageHashListMap[]={
    FXMAPFUNC(SEL_COMMAND, ManageHashList::ID_REMBIN, ManageHashList::RemoveWhl),
    FXMAPFUNC(SEL_COMMAND, ManageHashList::ID_BROWSE, ManageHashList::SetHashPath),
    FXMAPFUNC(SEL_COMMAND, ManageHashList::ID_EMPTY, ManageHashList::CreateEmptyList),
    FXMAPFUNC(SEL_CLICKED, ManageHashList::ID_LISTSELECT, ManageHashList::ListSelection),
};

#endif // MANAGEHASHLIST_H
