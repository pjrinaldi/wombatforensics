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
}

FXString EvidenceManager::ReturnEvidence()
{
    /*
    FXString settingsstring = FXString::value(thumbsizespinner->getValue());
    settingsstring += "|" + FXString::value(vidthumbspinner->getValue());
    settingsstring += "|" + casepathtextfield->getText();
    settingsstring += "|" + reportpathtextfield->getText();
    settingsstring += "|" + reporttzcombo->getItemText(reporttzcombo->getCurrentItem());
    settingsstring += "|" + FXString::value(autosavespinner->getValue());
    return settingsstring;
    */
    return "";
}

void EvidenceManager::LoadEvidence(FXString curevidence)
{
    /*
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
    */
}

long EvidenceManager::AddEvidence(FXObject*, FXSelector, void*)
{
    if(prevevidpath.empty())
	prevevidpath = FXString(getenv("HOME")) + "/";
    FXString evidpathstring = FXFileDialog::getOpenFilename(this, "Add Evidence", prevevidpath);
    if(!evidpathstring.empty())
    {
        prevevidpath = evidpathstring;
        for(int i=0; i < evidarray.no(); i++)
        {
            if(FXString::compare(evidarray.at(i), evidpathstring))
                isexist = true;
        }
        if(!isexist)
        {
            evidlist->appendItem(new FXListItem(evidpathstring));
            evidarray.append(evidpathstring);
        }
        //std::cout << "evidpathstring:" << evidpathstring.text() << std::endl;
    }

    return 1;
}

long EvidenceManager::RemEvidence(FXObject*, FXSelector, void*)
{
    /*
    FXString reportpathstring = FXDirDialog::getOpenDirectory(this, "Select the Directory to store the Reports", reportpathtextfield->getText());
    if(!reportpathstring.empty())
        reportpathtextfield->setText(reportpathstring + "/");
    */

    return 1;
}
