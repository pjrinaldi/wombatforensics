#include "evidencemanager.h"

FXIMPLEMENT(EvidenceManager,FXDialogBox,EvidenceManagerMap,ARRAYNUMBER(EvidenceManagerMap))

EvidenceManager::EvidenceManager(FXWindow* parent, const FXString& title):FXDialogBox(parent, title, DECOR_TITLE|DECOR_BORDER|DECOR_CLOSE, 0, 0, 480, 260)
{
    mainframe = new FXVerticalFrame(this, LAYOUT_TOP|LAYOUT_FILL_X|LAYOUT_FILL_Y);
    hframe1 = new FXHorizontalFrame(mainframe, LAYOUT_TOP|LAYOUT_FILL_X);
    new FXLabel(hframe1, "", NULL, LAYOUT_FILL_X);
    new FXSpring(hframe1);
    addbutton = new FXButton(hframe1, "Add", NULL, this, ID_ADDEVID, FRAME_RAISED|FRAME_THICK, 0, 0, 0, 0, 20, 20);
    rembutton = new FXButton(hframe1, "Remove Selected", NULL, this, ID_REMEVID, FRAME_RAISED|FRAME_THICK, 0, 0, 0, 0, 20, 20);
    groupbox = new FXGroupBox(mainframe, "Evidence", GROUPBOX_NORMAL|LAYOUT_FILL_X|LAYOUT_FILL_Y); 
    evidlist = new FXList(groupbox, this, ID_EVIDLIST, LAYOUT_FILL_X|LAYOUT_FILL_Y);
    hframe2 = new FXHorizontalFrame(mainframe, LAYOUT_TOP|LAYOUT_FILL_X);
    new FXLabel(hframe2, "", NULL, LAYOUT_FILL_X);
    new FXSpring(hframe2);
    cancelbutton = new FXButton(hframe2, "Cancel", NULL, this, FXDialogBox::ID_CANCEL, FRAME_RAISED|FRAME_THICK, 0,0,0,0, 20,20);
    donebutton = new FXButton(hframe2, "Done", NULL, this, FXDialogBox::ID_ACCEPT, FRAME_RAISED|FRAME_THICK, 0,0,0,0, 20,20);
    isexist = false;
    rembutton->disable();
}

FXString EvidenceManager::ReturnEvidence()
{
    evidliststr = "";
    for(int i=0; i < evidlist->getNumItems(); i++)
    {
	if(!evidlist->getItemText(i).empty())
	    evidliststr += evidlist->getItemText(i) + "\n";
    }

    return evidliststr;
}

void EvidenceManager::LoadEvidence(FXString curevidence)
{
    evidlist->clearItems();
    FXArray<FXint> posarray;
    posarray.clear();
    int found = 0;
    posarray.append(-1);
    while(found > -1)
    {
        found = curevidence.find('\n', found+1);
        if(found > -1)
            posarray.append(found);
    }
    //std::cout << "pos array count: " << posarray.no() << std::endl;
    for(int i=0; i < posarray.no() - 1; i++)
    {
        FXString curevidstr = curevidence.mid(posarray.at(i)+1, posarray.at(i+1) - posarray.at(i) - 1);
	//std::cout << "curevidstr: " << curevidstr.text() << std::endl;
        evidlist->appendItem(new FXListItem(curevidstr));
    }

}

long EvidenceManager::AddEvidence(FXObject*, FXSelector, void*)
{
    if(prevevidpath.empty())
	prevevidpath = FXString(getenv("HOME")) + "/";
    FXString evidpathstring = FXFileDialog::getOpenFilename(this, "Add Evidence", prevevidpath);
    if(!evidpathstring.empty())
    {
        prevevidpath = evidpathstring;
        isexist = false;
	for(int i=0; i < evidlist->getNumItems(); i++)
	{
	    if(FXString::compare(evidlist->getItemText(i), evidpathstring) == 0)
		isexist = true;
	}
        if(!isexist)
            evidlist->appendItem(new FXListItem(evidpathstring));
    }

    return 1;
}

long EvidenceManager::EvidenceSelected(FXObject*, FXSelector, void*)
{
    rembutton->disable();
    if(evidlist->getCurrentItem() > -1)
        rembutton->enable();

    return 1;
}
long EvidenceManager::RemEvidence(FXObject*, FXSelector, void*)
{
    if(evidlist->getCurrentItem() > -1)
        evidlist->removeItem(evidlist->getCurrentItem());

    return 1;
}
