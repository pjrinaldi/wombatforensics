#include "digdeeper.h"

FXIMPLEMENT(DigDeeper,FXDialogBox,DigDeeperMap,ARRAYNUMBER(DigDeeperMap))
//FXIMPLEMENT(DigDeeper,FXDialogBox,NULL,0)

DigDeeper::DigDeeper(FXWindow* parent, const FXString& title):FXDialogBox(parent, title, DECOR_TITLE|DECOR_BORDER|DECOR_CLOSE|DECOR_RESIZE, 0, 0, 500, 392, 0,0,0,0, 10, 10)
{
    mainframe = new FXVerticalFrame(this, LAYOUT_TOP|LAYOUT_FILL_X|LAYOUT_FILL_Y, 0, 0, 0, 0, 10, 10, 10, 10);
    filesgroup = new FXGroupBox(mainframe, "Process the Following File(s)", GROUPBOX_NORMAL|FRAME_THICK|LAYOUT_FILL_X);
    hframe1 = new FXHorizontalFrame(filesgroup, LAYOUT_TOP|LAYOUT_FILL_X);
    selectedradio = new FXRadioButton(hframe1, "Selected", this, ID_FILES, RADIOBUTTON_NORMAL|LAYOUT_FILL_X);
    new FXSpring(hframe1);
    checkedradio = new FXRadioButton(hframe1, "Checked (0)", this, ID_FILES, RADIOBUTTON_NORMAL|LAYOUT_FILL_X);
    new FXSpring(hframe1);
    allradio = new FXRadioButton(hframe1, "All (0)", this, ID_FILES, RADIOBUTTON_NORMAL|LAYOUT_FILL_X);
    optionsgroup = new FXGroupBox(mainframe, "For the Following Option(s)", GROUPBOX_NORMAL|FRAME_THICK|LAYOUT_FILL_X|LAYOUT_FILL_Y);
    vframe1 = new FXVerticalFrame(optionsgroup, LAYOUT_TOP|LAYOUT_FILL_X|LAYOUT_FILL_Y);
    expandarchives = new FXCheckButton(vframe1, "Expand Archives (zip)", NULL, 0, CHECKBUTTON_NORMAL);
    expandemail = new FXCheckButton(vframe1, "Expand Mailboxes (mbox)");
    imagethumbnails = new FXCheckButton(vframe1, "Generate Thumbnails for Images");
    videothumbnails = new FXCheckButton(vframe1, "Generate Thumbnails for Videos");
    hashfiles = new FXCheckButton(vframe1, "Calculate BLAKE3 Hash");
    hframe2 = new FXHorizontalFrame(vframe1, LAYOUT_TOP|LAYOUT_FILL_X|LAYOUT_FILL_Y, 0,0,0,0, 0,0,0,0);
    hashcomparison = new FXCheckButton(hframe2, "Compare Files to Selected Hash Lists:");
    hashlist = new FXList(hframe2, NULL, 0, FRAME_SUNKEN|FRAME_THICK|LIST_NORMAL|LAYOUT_FILL_X|LAYOUT_FILL_Y);
    
    hframe8 = new FXHorizontalFrame(mainframe, LAYOUT_TOP|LAYOUT_FILL_X);
    new FXLabel(hframe8, "", NULL, LAYOUT_FILL_X);
    new FXSpring(hframe8);
    cancelbutton = new FXButton(hframe8, "Cancel", NULL, this, FXDialogBox::ID_CANCEL, FRAME_RAISED|FRAME_THICK, 0,0,0,0, 20,20);
    savebutton = new FXButton(hframe8, "Process", NULL, this, FXDialogBox::ID_ACCEPT, FRAME_RAISED|FRAME_THICK, 0,0,0,0, 20,20);
}

/*
FXString DigDeeper::ReturnDigDeeper()
{
    FXString settingsstring = FXString::value(thumbsizespinner->getValue());
    settingsstring += "|" + FXString::value(vidthumbspinner->getValue());
    settingsstring += "|" + casepathtextfield->getText();
    settingsstring += "|" + reportpathtextfield->getText();
    settingsstring += "|" + reporttzcombo->getItemText(reporttzcombo->getCurrentItem());
    settingsstring += "|" + FXString::value(autosavespinner->getValue());
    return settingsstring;
}

void DigDeeper::LoadDigDeeper(FXString cursettings)
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

long DigDeeper::SetFilesRadioState(FXObject* sender, FXSelector, void*)
{
    FXString curtext = ((FXRadioButton*)sender)->getText();
    //std::cout << "curtext: "<< curtext.text() << std::endl;
    selectedradio->setCheck(false);
    checkedradio->setCheck(false);
    allradio->setCheck(false);
    if(curtext.left(1) == "S")
	selectedradio->setCheck(true);
    else if(curtext.left(1) == "C")
	checkedradio->setCheck(true);
    else if(curtext.left(1) == "A")
	allradio->setCheck(true);

    return 1;
}

/*
long DigDeeper::SetCasePath(FXObject*, FXSelector, void*)
{
    FXString casepathstring = FXDirDialog::getOpenDirectory(this, "Select the Directory to store Wombat Case Files", casepathtextfield->getText());
    if(!casepathstring.empty())
        casepathtextfield->setText(casepathstring + "/");

    return 1;
}

long DigDeeper::SetReportPath(FXObject*, FXSelector, void*)
{
    FXString reportpathstring = FXDirDialog::getOpenDirectory(this, "Select the Directory to store the Reports", reportpathtextfield->getText());
    if(!reportpathstring.empty())
        reportpathtextfield->setText(reportpathstring + "/");

    return 1;
}
*/
