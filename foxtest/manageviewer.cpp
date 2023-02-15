#include "manageviewer.h"

FXIMPLEMENT(ManageViewer,FXDialogBox,ManageViewerMap,ARRAYNUMBER(ManageViewerMap))

ManageViewer::ManageViewer(FXWindow* parent, const FXString& title):FXDialogBox(parent, title, DECOR_TITLE|DECOR_BORDER|DECOR_RESIZE|DECOR_CLOSE, 0, 0, 760, 460, 0,0,0,0, 4, 4)
{
    mainframe = new FXVerticalFrame(this, LAYOUT_TOP|LAYOUT_FILL_X|LAYOUT_FILL_Y);
    mainlabel = new FXLabel(mainframe, "Manage External Viewers");
    binlist = new FXList(mainframe, this, ID_LISTSELECT, LIST_SINGLESELECT|LAYOUT_FILL_X|LAYOUT_FILL_Y);
    buttonframe = new FXHorizontalFrame(mainframe, LAYOUT_BOTTOM|LAYOUT_FILL_X);
    binlabel = new FXLabel(buttonframe, "Viewer Path:", NULL, JUSTIFY_LEFT);
    new FXSpring(buttonframe);
    binarypath = new FXTextField(buttonframe, 30);
    browsebutton = new FXButton(buttonframe, "Browse", NULL, this, ID_BROWSE, FRAME_RAISED|FRAME_THICK, 0,0,0,0, 20, 20);
    addbutton = new FXButton(buttonframe, "Add Binary", NULL, this, ID_ADDBIN, FRAME_RAISED|FRAME_THICK, 0,0,0,0, 20, 20);
    rembutton = new FXButton(buttonframe, "Remove Binary", NULL, this, ID_REMBIN, FRAME_RAISED|FRAME_THICK, 0,0,0,0, 20, 20);
    rembutton->disable();
    savebutton = new FXButton(buttonframe, "Save", NULL, this, FXDialogBox::ID_ACCEPT, FRAME_RAISED|FRAME_THICK, 0,0,0,0, 20,20);
}

void ManageViewer::SetBinList(std::vector<std::string>* binlist)
{
    binaries = binlist;
    if(binaries != NULL)
        UpdateList();
}

void ManageViewer::UpdateList()
{
    binlist->clearItems();
    for(int i=0; i < binaries->size(); i++)
        binlist->appendItem(new FXListItem(FXString(binaries->at(i).c_str())));
}

long ManageViewer::SetBinPath(FXObject*, FXSelector, void*)
{
    binstring = FXFileDialog::getOpenFilename(this, "Select the Binary to add", "/usr/bin/");
    if(!binstring.empty())
        binarypath->setText(binstring);

    return 1;
}

long ManageViewer::AddBin(FXObject*, FXSelector, void*)
{
    binaries->push_back(binstring.text());
    UpdateList();

    return 1;
}

long ManageViewer::RemoveBin(FXObject*, FXSelector, void*)
{
    int curitem = binlist->getCurrentItem();
    std::string curtext = binaries->at(curitem);
    binaries->erase(binaries->begin() + curitem);
    rembutton->disable();
    UpdateList();

    return 1;
}

long ManageViewer::ListSelection(FXObject*, FXSelector, void*)
{
    rembutton->enable();

    return 1;
}

FXString ManageViewer::ReturnViewers()
{
    FXString viewerstring = "";

    return viewerstring;
}
/*
FXString Settings::ReturnSettings()
{
    FXString settingsstring = FXString::value(thumbsizespinner->getValue());
    settingsstring += "|" + FXString::value(vidthumbspinner->getValue());
    settingsstring += "|" + casepathtextfield->getText();
    settingsstring += "|" + reportpathtextfield->getText();
    settingsstring += "|" + reporttzcombo->getItemText(reporttzcombo->getCurrentItem());
    settingsstring += "|" + FXString::value(autosavespinner->getValue());
    return settingsstring;
}
*/

void ManageViewer::LoadViewers(FXString curviewers)
{
}

/*
void Settings::LoadSettings(FXString cursettings)
{
    int found1 = cursettings.find("|");
    int found2 = cursettings.find("|", found1+1);
    int found3 = cursettings.find("|", found2+1);
    int found4 = cursettings.find("|", found3+1);
    int found5 = cursettings.find("|", found4+1);
    thumbsizespinner->setValue(cursettings.left(found1).toUInt());
    vidthumbspinner->setValue(cursettings.mid(found1+1, found2 - found1 - 1).toUInt());
    casepathtextfield->setText(cursettings.mid(found2+1, found3 - found2 - 1));
    reportpathtextfield->setText(cursettings.mid(found3+1, found4 - found3 - 1));
    reporttzcombo->setCurrentItem(cursettings.mid(found4+1, found5 - found4 - 1).toUInt());
    autosavespinner->setValue(cursettings.mid(found5+1, cursettings.length() - found5 - 1).toUInt());
}
*/
