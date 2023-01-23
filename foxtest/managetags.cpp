#include "managetags.h"

FXIMPLEMENT(ManageTags,FXDialogBox,ManageTagsMap,ARRAYNUMBER(ManageTagsMap))

ManageTags::ManageTags(FXWindow* parent, const FXString& title):FXDialogBox(parent, title, DECOR_TITLE|DECOR_BORDER|DECOR_RESIZE|DECOR_CLOSE, 0, 0, 320, 260, 0,0,0,0, 4, 4)
{
    mainframe = new FXVerticalFrame(this, LAYOUT_TOP|LAYOUT_FILL_X|LAYOUT_FILL_Y);
    mainlabel = new FXLabel(mainframe, "Case Tags");
    taglist = new FXList(mainframe, this, ID_LISTSELECT, LIST_SINGLESELECT|LAYOUT_FILL_X|LAYOUT_FILL_Y);
    buttonframe = new FXHorizontalFrame(mainframe, LAYOUT_BOTTOM|LAYOUT_FILL_X);
    newicon = new FXPNGIcon(this->getApp(), bookmarknew);
    newbutton = new FXButton(buttonframe, "New Tag", newicon, this, ID_NEWTAG);
    editicon = new FXPNGIcon(this->getApp(), bookmark);
    editbutton = new FXButton(buttonframe, "Modify Tag", editicon, this, ID_EDITTAG);
    remicon = new FXPNGIcon(this->getApp(), bookmarkrem);
    rembutton = new FXButton(buttonframe, "Remove Tag", remicon, this, ID_REMTAG);
    rembutton->disable();
    editbutton->disable();
}

void ManageTags::SetTagList(std::vector<std::string>* tagslist)
{
    tags = tagslist;
    if(tags != NULL)
        UpdateList();
}

void ManageTags::UpdateList()
{
    taglist->clearItems();
    for(int i=0; i < tags->size(); i++)
        taglist->appendItem(new FXListItem(FXString(tags->at(i).c_str())));
}

long ManageTags::AddTag(FXObject*, FXSelector, void*)
{
    FXString tagstr = "";
    bool isset = FXInputDialog::getString(tagstr, this, "Enter Tag Name", "New Tag");
    if(isset)
    {
        tags->push_back(tagstr.text());
        UpdateList();
    }
    return 1;
}

long ManageTags::ModifyTag(FXObject*, FXSelector, void*)
{
    FXint curitem = taglist->getCurrentItem();
    std::string curtext = tags->at(curitem);
    FXString modtagname = "";
    bool isset = FXInputDialog::getString(modtagname, this, "Modify Tag Name", "Modify Tag");
    if(isset)
    {
        taglist->getItem(curitem)->setText(modtagname);
        taglist->update();
        tags->at(curitem) = modtagname.text();
    }
    return 1;
}

long ManageTags::RemoveTag(FXObject*, FXSelector, void*)
{
    int curitem = taglist->getCurrentItem();
    std::string curtext = tags->at(curitem);
    tags->erase(tags->begin() + curitem);
    rembutton->disable();
    editbutton->disable();
    UpdateList();
    return 1;
}

long ManageTags::ListSelection(FXObject*, FXSelector, void*)
{
    rembutton->enable();
    editbutton->enable();
    return 1;
}
