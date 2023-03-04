#include "managehashlist.h"

FXIMPLEMENT(ManageHashList,FXDialogBox,ManageHashListMap,ARRAYNUMBER(ManageHashListMap))

ManageHashList::ManageHashList(FXWindow* parent, const FXString& title):FXDialogBox(parent, title, DECOR_TITLE|DECOR_BORDER|DECOR_RESIZE|DECOR_CLOSE, 0, 0, 640, 475, 0,0,0,0, 4, 4)
{
    mainframe = new FXVerticalFrame(this, LAYOUT_TOP|LAYOUT_FILL_X|LAYOUT_FILL_Y);
    mainlabel = new FXLabel(mainframe, "Hash Lists");
    subframe = new FXHorizontalFrame(mainframe, LAYOUT_TOP|LAYOUT_FILL_X|LAYOUT_FILL_Y);
    whllist = new FXList(subframe, this, ID_LISTSELECT, LIST_SINGLESELECT|LAYOUT_FILL_X|LAYOUT_FILL_Y);
    buttonframe = new FXVerticalFrame(subframe, LAYOUT_TOP|LAYOUT_FILL_Y|JUSTIFY_RIGHT);
    browsebutton = new FXButton(buttonframe, "Import Hash List", NULL, this, ID_BROWSE, FRAME_RAISED|FRAME_THICK, 0,0,0,0, 20, 20);
    emptybutton = new FXButton(buttonframe, "Create Empty List", NULL, this, ID_EMPTY, FRAME_RAISED|FRAME_THICK, 0,0,0,0, 20, 20);
    rembutton = new FXButton(buttonframe, "Delete Selected", NULL, this, ID_REMBIN, FRAME_RAISED|FRAME_THICK, 0,0,0,0, 20, 20);
    rembutton->disable();
    savebutton = new FXButton(buttonframe, "Close", NULL, this, FXDialogBox::ID_ACCEPT, FRAME_RAISED|FRAME_THICK, 0,0,0,0, 20,20);
}

void ManageHashList::SetHashList(std::vector<std::string>* hashlist)
{
    hashlists = hashlist;
    if(hashlists != NULL)
        UpdateList();
}

void ManageHashList::SetCaseName(FXString cname)
{
    casename = cname;
}

void ManageHashList::UpdateList()
{
    whllist->clearItems();
    if(hashlists != NULL)
    {
        for(int i=0; i < hashlists->size(); i++)
            whllist->appendItem(new FXListItem(FXString(hashlists->at(i).c_str())));
    }
}

long ManageHashList::SetHashPath(FXObject*, FXSelector, void*)
{
    FXString hstr = FXString(getenv("HOME")) + "/";
    hashstring = FXFileDialog::getOpenFilename(this, "Select the Hash List to Import", hstr, "Wombat Hash Lists (*.whl)");
    if(!hashstring.empty())
    {
        int found = hashstring.find_last_of("/");
        FXString localstring = "/tmp/wf/" + casename + "/" + "hashlists/" + hashstring.mid(found+1, hashstring.length() - found);
        //std::cout << "localstring: " << localstring.text() << std::endl;
        bool filecopied = FXFile::copy(hashstring, localstring, true);
        if(filecopied)
        {
            hashlists->push_back(localstring.text());
            UpdateList();
        }
        //else
        //    FXuint result = FXMessageBox::question(this, MBOX_YES_NO, "Existing Case Status", "There is a case already open. Are you sure you want to close it?"); // no is 2, yes is 1
            //std::cout << "file already existed, not copied." << std::endl;
    }

    return 1;
}

long ManageHashList::RemoveWhl(FXObject*, FXSelector, void*)
{
    int curitem = whllist->getCurrentItem();
    std::string curtext = hashlists->at(curitem);
    hashlists->erase(hashlists->begin() + curitem);
    rembutton->disable();
    UpdateList();

    return 1;
}

long ManageHashList::ListSelection(FXObject*, FXSelector, void*)
{
    rembutton->enable();

    return 1;
}

long ManageHashList::CreateEmptyList(FXObject*, FXSelector, void*)
{
    std::cout << "create empty list here" << std::endl;
    return 1;
}

void ManageHashList::LoadHashList()
{
    std::cout << "load hashlists here before proceeding. " << std::endl;
    /*
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
    if(posarray.no() > 1 && hashlists != NULL)
    {
        hashlists->clear();
        for(int i=0; i < posarray.no() - 1; i++)
            hashlists->push_back(curviewers.mid(posarray.at(i)+1, posarray.at(i+1) - posarray.at(i) - 1).text());
    }
    if(curviewers.length() > 0 && posarray.no() == 1)
        hashlists->push_back(curviewers.text());
    if(hashlists != NULL)
        UpdateList();
    */
}
