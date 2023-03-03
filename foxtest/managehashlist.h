#ifndef MANAGEHASHLIST_H
#define MANAGEHASHLIST_H

#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>

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
        FXList* binarylist;
        FXLabel* binlabel;
        FXButton* browsebutton;
        FXString binstring;
	FXButton* emptybutton;
        FXButton* rembutton;
        FXButton* savebutton;

        std::vector<std::string>* binaries = NULL;

    protected:
        ManageHashList() {}

        long onSomeCommand(FXObject* sender, FXSelector sel, void* ptr);

    public:
        enum
        {
            //ID_TREELIST = 1,
            ID_LISTSELECT = 100,
	    ID_EMPTY = 101,
            ID_BROWSE = 102,
            ID_REMBIN = 103,
            ID_LAST
        };
        ManageHashList(FXWindow* parent, const FXString& title);
        void SetBinList(std::vector<std::string>* binlist);
        void LoadViewers(FXString curviewers);
        void UpdateList();
        long RemoveBin(FXObject*, FXSelector, void*);
        long ListSelection(FXObject*, FXSelector, void*);
        long SetBinPath(FXObject*, FXSelector, void*);

};

FXDEFMAP(ManageHashList) ManageHashListMap[]={
    FXMAPFUNC(SEL_COMMAND, ManageHashList::ID_REMBIN, ManageHashList::RemoveBin),
    FXMAPFUNC(SEL_COMMAND, ManageHashList::ID_BROWSE, ManageHashList::SetBinPath),
    FXMAPFUNC(SEL_CLICKED, ManageHashList::ID_LISTSELECT, ManageHashList::ListSelection),
};

#endif // MANAGEHASHLIST_H
