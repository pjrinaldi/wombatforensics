#ifndef EVIDENCEMANAGER_H
#define EVIDENCEMANAGER_H

#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>

#include "icons.h"

#include "/usr/local/include/fox-1.7/fx.h"

class EvidenceManager : public FXDialogBox
{
    FXDECLARE(EvidenceManager)

    private:
        FXVerticalFrame* mainframe;
        FXHorizontalFrame* hframe1;
        FXButton* addbutton;
        FXButton* rembutton;
        FXGroupBox* groupbox;
        FXList* evidlist;
        FXHorizontalFrame* hframe2;
        FXButton* cancelbutton;
        FXButton* donebutton;

        FXArray<FXString> evidarray;
        FXString evidliststr;
        FXString prevevidpath;
        bool isexist;

    protected:
        EvidenceManager() {}

    public:
        enum
        {
            ID_ADDEVID = 100,
            ID_REMEVID = 101,
            ID_EVIDLIST = 102,
            ID_LAST
        };
        EvidenceManager(FXWindow* parent, const FXString& title);
        FXString ReturnEvidence(void);
        void LoadEvidence(FXString cursettings);

        long AddEvidence(FXObject*, FXSelector, void*);
        long RemEvidence(FXObject*, FXSelector, void*);
        long EvidenceSelected(FXObject*, FXSelector, void*);
};

FXDEFMAP(EvidenceManager) EvidenceManagerMap[]={
    FXMAPFUNC(SEL_COMMAND, EvidenceManager::ID_ADDEVID, EvidenceManager::AddEvidence),
    FXMAPFUNC(SEL_COMMAND, EvidenceManager::ID_REMEVID, EvidenceManager::RemEvidence),
    FXMAPFUNC(SEL_SELECTED, EvidenceManager::ID_EVIDLIST, EvidenceManager::EvidenceSelected),
};

#endif // EVIDENCEMANAGER_H
