#ifndef MANAGEVIEWER_H
#define MANAGEVIEWER_H

#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>

#include "icons.h"

#include "/usr/local/include/fox-1.7/fx.h"

class ManageViewer : public FXDialogBox
{
    FXDECLARE(ManageViewer)

    private:
        FXVerticalFrame* mainframe;
        FXHorizontalFrame* buttonframe;
        FXLabel* mainlabel;
        FXList* binarylist;
        FXLabel* binlabel;
        FXButton* browsebutton;
        FXString binstring;
        FXButton* rembutton;
        FXButton* savebutton;

        std::vector<std::string>* binaries = NULL;

    protected:
        ManageViewer() {}

        long onSomeCommand(FXObject* sender, FXSelector sel, void* ptr);

    public:
        enum
        {
            ID_TREELIST = 1,
            ID_LISTSELECT = 100,
            ID_BROWSE = 102,
            ID_REMBIN = 103,
            ID_LAST
        };
        ManageViewer(FXWindow* parent, const FXString& title);
        void SetBinList(std::vector<std::string>* binlist);
        void LoadViewers(FXString curviewers);
        void UpdateList();
        long RemoveBin(FXObject*, FXSelector, void*);
        long ListSelection(FXObject*, FXSelector, void*);
        long SetBinPath(FXObject*, FXSelector, void*);

};

FXDEFMAP(ManageViewer) ManageViewerMap[]={
    FXMAPFUNC(SEL_COMMAND, ManageViewer::ID_REMBIN, ManageViewer::RemoveBin),
    FXMAPFUNC(SEL_COMMAND, ManageViewer::ID_BROWSE, ManageViewer::SetBinPath),
    FXMAPFUNC(SEL_CLICKED, ManageViewer::ID_LISTSELECT, ManageViewer::ListSelection),
};

#endif // MANAGEVIEWER_H
