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
        EvidenceManager() {}

    public:
        enum
        {
            ID_CASEPATH = 1,
            ID_REPORTPATH = 2,
            ID_LAST
        };
        EvidenceManager(FXWindow* parent, const FXString& title);
        FXString ReturnEvidenceManager(void);
        void LoadEvidenceManager(FXString cursettings);

        long SetCasePath(FXObject*, FXSelector, void*);
        long SetReportPath(FXObject*, FXSelector, void*);
};

FXDEFMAP(EvidenceManager) EvidenceManagerMap[]={
    FXMAPFUNC(SEL_COMMAND, EvidenceManager::ID_CASEPATH, EvidenceManager::SetCasePath),
    FXMAPFUNC(SEL_COMMAND, EvidenceManager::ID_REPORTPATH, EvidenceManager::SetReportPath),
};

#endif // EVIDENCEMANAGER_H
