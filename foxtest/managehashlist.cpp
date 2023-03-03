#include "managehashlist.h"

FXIMPLEMENT(ManageHashList,FXDialogBox,ManageHashListMap,ARRAYNUMBER(ManageHashListMap))

ManageHashList::ManageHashList(FXWindow* parent, const FXString& title):FXDialogBox(parent, title, DECOR_TITLE|DECOR_BORDER|DECOR_RESIZE|DECOR_CLOSE, 0, 0, 320, 260, 0,0,0,0, 4, 4)
{
    mainframe = new FXVerticalFrame(this, LAYOUT_TOP|LAYOUT_FILL_X|LAYOUT_FILL_Y);
    mainlabel = new FXLabel(mainframe, "Hash Lists");
    binarylist = new FXList(mainframe, this, ID_LISTSELECT, LIST_SINGLESELECT|LAYOUT_FILL_X|LAYOUT_FILL_Y);
    buttonframe = new FXHorizontalFrame(mainframe, LAYOUT_BOTTOM|LAYOUT_FILL_X);
    browsebutton = new FXButton(buttonframe, "Browse", NULL, this, ID_BROWSE, FRAME_RAISED|FRAME_THICK, 0,0,0,0, 20, 20);
    rembutton = new FXButton(buttonframe, "Remove Binary", NULL, this, ID_REMBIN, FRAME_RAISED|FRAME_THICK, 0,0,0,0, 20, 20);
    rembutton->disable();
    savebutton = new FXButton(buttonframe, "Save", NULL, this, FXDialogBox::ID_ACCEPT, FRAME_RAISED|FRAME_THICK, 0,0,0,0, 20,20);
}

void ManageHashList::SetBinList(std::vector<std::string>* binlist)
{
    binaries = binlist;
    if(binaries != NULL)
        UpdateList();
}

void ManageHashList::UpdateList()
{
    binarylist->clearItems();
    if(binaries != NULL)
    {
        for(int i=0; i < binaries->size(); i++)
            binarylist->appendItem(new FXListItem(FXString(binaries->at(i).c_str())));
    }
}

long ManageHashList::SetBinPath(FXObject*, FXSelector, void*)
{
    binstring = FXFileDialog::getOpenFilename(this, "Select the Binary to add", "/usr/bin/");
    if(!binstring.empty())
    {
        binaries->push_back(binstring.text());
        UpdateList();
    }

    return 1;
}

long ManageHashList::RemoveBin(FXObject*, FXSelector, void*)
{
    int curitem = binarylist->getCurrentItem();
    std::string curtext = binaries->at(curitem);
    binaries->erase(binaries->begin() + curitem);
    rembutton->disable();
    UpdateList();

    return 1;
}

long ManageHashList::ListSelection(FXObject*, FXSelector, void*)
{
    rembutton->enable();

    return 1;
}

void ManageHashList::LoadViewers(FXString curviewers)
{
    FXArray<FXint> posarray;
    int found = 0;
    posarray.append(-1);
    while(found > -1)
    {
        found = curviewers.find("|", found+1);
        if(found > -1)
            posarray.append(found);
    }
    posarray.append(curviewers.length());
    if(posarray.no() > 1 && binaries != NULL)
    {
        binaries->clear();
        for(int i=0; i < posarray.no() - 1; i++)
            binaries->push_back(curviewers.mid(posarray.at(i)+1, posarray.at(i+1) - posarray.at(i) - 1).text());
    }
    if(curviewers.length() > 0 && posarray.no() == 1)
        binaries->push_back(curviewers.text());
    if(binaries != NULL)
        UpdateList();
}
