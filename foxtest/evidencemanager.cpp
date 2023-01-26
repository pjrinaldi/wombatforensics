#include "evidencemanager.h"

FXIMPLEMENT(EvidenceManager,FXDialogBox,EvidenceManagerMap,ARRAYNUMBER(EvidenceManagerMap))

EvidenceManager::EvidenceManager(FXWindow* parent, const FXString& title):FXDialogBox(parent, title, DECOR_TITLE|DECOR_BORDER|DECOR_CLOSE, 0, 0, 480, 260, 0,0,0,0, 10, 10)
{
    mainframe = new FXVerticalFrame(this, LAYOUT_TOP|LAYOUT_FILL_X|LAYOUT_FILL_Y, 0, 0, 0, 0, 10, 10, 10, 10);
    hframe1 = new FXHorizontalFrame(mainframe, LAYOUT_TOP|LAYOUT_FILL_X);
    thumbsizelabel = new FXLabel(hframe1, "Thumbnail Size (pixels):", NULL, LAYOUT_FILL_X|JUSTIFY_LEFT);
    new FXSpring(hframe1);
    thumbsizespinner = new FXSpinner(hframe1, 5);
    thumbsizespinner->setRange(64, 384);
    thumbsizespinner->setIncrement(64);
    hframe2 = new FXHorizontalFrame(mainframe, LAYOUT_TOP|LAYOUT_FILL_X);
    vidthumblabel = new FXLabel(hframe2, "Generate Video Thumbnail Every (%): ", NULL, LAYOUT_FILL_X|JUSTIFY_LEFT);
    new FXSpring(hframe2);
    vidthumbspinner = new FXSpinner(hframe2, 5);
    vidthumbspinner->setRange(10, 100);
    vidthumbspinner->setIncrement(5);
    hframe3 = new FXHorizontalFrame(mainframe, LAYOUT_TOP|LAYOUT_FILL_X);
    casepathlabel = new FXLabel(hframe3, "Set Case Path:", NULL, LAYOUT_FILL_X|JUSTIFY_LEFT);
    new FXSpring(hframe3);
    casepathtextfield = new FXTextField(hframe3, 40);
    casepathbutton = new FXButton(hframe3, "Browse", NULL, this, ID_CASEPATH, FRAME_RAISED|FRAME_THICK, 0,0,0,0, 20,20);
    hframe4 = new FXHorizontalFrame(mainframe, LAYOUT_TOP|LAYOUT_FILL_X);
    reportpathlabel = new FXLabel(hframe4, "Set Report Path:", NULL, LAYOUT_FILL_X|JUSTIFY_LEFT);
    new FXSpring(hframe4);
    reportpathtextfield = new FXTextField(hframe4, 40);
    reportpathbutton = new FXButton(hframe4, "Browse", NULL, this, ID_REPORTPATH, FRAME_RAISED|FRAME_THICK, 0,0,0,0, 20,20);
    hframe5 = new FXHorizontalFrame(mainframe, LAYOUT_TOP|LAYOUT_FILL_X);
    reporttzlabel = new FXLabel(hframe5, "Report TimeZone:", NULL, LAYOUT_FILL_X|JUSTIFY_LEFT);
    reporttzcombo = new FXComboBox(hframe5, 45);
    reporttzcombo->fillItems(timezones);
    reporttzcombo->setNumVisible(10);
    hframe6 = new FXHorizontalFrame(mainframe, LAYOUT_TOP|LAYOUT_FILL_X);
    autosavelabel = new FXLabel(hframe6, "Set Autosave Interval (minutes):", NULL, LAYOUT_FILL_X|JUSTIFY_LEFT);
    new FXSpring(hframe6);
    autosavespinner = new FXSpinner(hframe6, 5);
    autosavespinner->setRange(1, 120);
    autosavespinner->setIncrement(1);
    hframe8 = new FXHorizontalFrame(mainframe, LAYOUT_TOP|LAYOUT_FILL_X);
    new FXLabel(hframe8, "", NULL, LAYOUT_FILL_X);
    new FXSpring(hframe8);
    cancelbutton = new FXButton(hframe8, "Cancel", NULL, this, FXDialogBox::ID_CANCEL, FRAME_RAISED|FRAME_THICK, 0,0,0,0, 20,20);
    savebutton = new FXButton(hframe8, "Save", NULL, this, FXDialogBox::ID_ACCEPT, FRAME_RAISED|FRAME_THICK, 0,0,0,0, 20,20);
}

FXString EvidenceManager::ReturnEvidenceManager()
{
    FXString settingsstring = FXString::value(thumbsizespinner->getValue());
    settingsstring += "|" + FXString::value(vidthumbspinner->getValue());
    settingsstring += "|" + casepathtextfield->getText();
    settingsstring += "|" + reportpathtextfield->getText();
    settingsstring += "|" + reporttzcombo->getItemText(reporttzcombo->getCurrentItem());
    settingsstring += "|" + FXString::value(autosavespinner->getValue());
    return settingsstring;
}

void EvidenceManager::LoadEvidenceManager(FXString cursettings)
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

long EvidenceManager::SetCasePath(FXObject*, FXSelector, void*)
{
    FXString casepathstring = FXDirDialog::getOpenDirectory(this, "Select the Directory to store Wombat Case Files", casepathtextfield->getText());
    if(!casepathstring.empty())
        casepathtextfield->setText(casepathstring + "/");

    return 1;
}

long EvidenceManager::SetReportPath(FXObject*, FXSelector, void*)
{
    FXString reportpathstring = FXDirDialog::getOpenDirectory(this, "Select the Directory to store the Reports", reportpathtextfield->getText());
    if(!reportpathstring.empty())
        reportpathtextfield->setText(reportpathstring + "/");

    return 1;
}
