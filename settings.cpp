#include "settings.h"

FXIMPLEMENT(Settings,FXDialogBox,SettingsMap,ARRAYNUMBER(SettingsMap))

Settings::Settings(FXWindow* parent, const FXString& title):FXDialogBox(parent, title, DECOR_TITLE|DECOR_BORDER|DECOR_CLOSE, 0, 0, 540, 420, 0,0,0,0, 10, 10)
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
    hframe9 = new FXHorizontalFrame(mainframe, LAYOUT_TOP|LAYOUT_FILL_X);
    vidlabel = new FXLabel(hframe9, "Video Viewer Path:", NULL, LAYOUT_FILL_X|JUSTIFY_LEFT);
    new FXSpring(hframe9);
    vidtextfield = new FXTextField(hframe9, 40);
    vidbutton = new FXButton(hframe9, "Browse", NULL, this, ID_VIDPATH, FRAME_RAISED|FRAME_THICK, 0, 0, 0, 0, 20, 20);
    hframe10 = new FXHorizontalFrame(mainframe, LAYOUT_TOP|LAYOUT_FILL_X);
    htmllabel = new FXLabel(hframe10, "Html Viewer Path:", NULL, LAYOUT_FILL_X|JUSTIFY_LEFT);
    new FXSpring(hframe10);
    htmltextfield = new FXTextField(hframe10, 40);
    htmlbutton = new FXButton(hframe10, "Browse", NULL, this, ID_HTMLPATH, FRAME_RAISED|FRAME_THICK, 0, 0, 0, 0, 20, 20);
    hframe11 = new FXHorizontalFrame(mainframe, LAYOUT_TOP|LAYOUT_FILL_X);
    pdflabel = new FXLabel(hframe11, "PDF Viewer Path:", NULL, LAYOUT_FILL_X|JUSTIFY_LEFT);
    new FXSpring(hframe11);
    pdftextfield = new FXTextField(hframe11, 40);
    pdfbutton = new FXButton(hframe11, "Browse", NULL, this, ID_PDFPATH, FRAME_RAISED|FRAME_THICK, 0,0,0,0, 20,20);
    hframe12 = new FXHorizontalFrame(mainframe, LAYOUT_TOP|LAYOUT_FILL_X);
    imglabel = new FXLabel(hframe12, "Image Viewer Path:", NULL, LAYOUT_FILL_X|JUSTIFY_LEFT);
    new FXSpring(hframe12);
    imgtextfield = new FXTextField(hframe12, 40);
    imgbutton = new FXButton(hframe12, "Browse", NULL, this, ID_IMGPATH, FRAME_RAISED|FRAME_THICK, 0,0,0,0, 20,20);
    hframe8 = new FXHorizontalFrame(mainframe, LAYOUT_TOP|LAYOUT_FILL_X);
    new FXLabel(hframe8, "", NULL, LAYOUT_FILL_X);
    new FXSpring(hframe8);
    cancelbutton = new FXButton(hframe8, "Cancel", NULL, this, FXDialogBox::ID_CANCEL, FRAME_RAISED|FRAME_THICK, 0,0,0,0, 20,20);
    savebutton = new FXButton(hframe8, "Save", NULL, this, FXDialogBox::ID_ACCEPT, FRAME_RAISED|FRAME_THICK, 0,0,0,0, 20,20);
}

FXString Settings::ReturnSettings()
{
    FXString settingsstring = FXString::value(thumbsizespinner->getValue());
    settingsstring += "|" + FXString::value(vidthumbspinner->getValue());
    settingsstring += "|" + casepathtextfield->getText();
    settingsstring += "|" + reportpathtextfield->getText();
    settingsstring += "|" + reporttzcombo->getItemText(reporttzcombo->getCurrentItem());
    settingsstring += "|" + FXString::value(autosavespinner->getValue());
    settingsstring += "|" + vidtextfield->getText();
    settingsstring += "|" + htmltextfield->getText();
    settingsstring += "|" + pdftextfield->getText();
    settingsstring += "|" + imgtextfield->getText();
    return settingsstring;
}

void Settings::LoadSettings(FXString cursettings)
{
    int found1 = cursettings.find("|");
    int found2 = cursettings.find("|", found1+1);
    int found3 = cursettings.find("|", found2+1);
    int found4 = cursettings.find("|", found3+1);
    int found5 = cursettings.find("|", found4+1);
    int found6 = cursettings.find("|", found5+1);
    int found7 = cursettings.find("|", found6+1);
    int found8 = cursettings.find("|", found7+1);
    int found9 = cursettings.find("|", found8+1);
    thumbsizespinner->setValue(cursettings.left(found1).toUInt());
    vidthumbspinner->setValue(cursettings.mid(found1+1, found2 - found1 - 1).toUInt());
    casepathtextfield->setText(cursettings.mid(found2+1, found3 - found2 - 1));
    reportpathtextfield->setText(cursettings.mid(found3+1, found4 - found3 - 1));
    reporttzcombo->setCurrentItem(cursettings.mid(found4+1, found5 - found4 - 1).toUInt());
    autosavespinner->setValue(cursettings.mid(found5+1, found6 - found5 - 1).toUInt());
    vidtextfield->setText(cursettings.mid(found6+1, found7 - found6 - 1));
    htmltextfield->setText(cursettings.mid(found7+1, found8 - found7 - 1));
    pdftextfield->setText(cursettings.mid(found8+1, found9 - found8 - 1));
    imgtextfield->setText(cursettings.mid(found9+1, cursettings.length() - found9 - 1));
}

long Settings::SetCasePath(FXObject*, FXSelector, void*)
{
    FXString casepathstring = FXDirDialog::getOpenDirectory(this, "Select the Directory to store Wombat Case Files", casepathtextfield->getText());
    if(!casepathstring.empty())
        casepathtextfield->setText(casepathstring + "/");

    return 1;
}

long Settings::SetReportPath(FXObject*, FXSelector, void*)
{
    FXString reportpathstring = FXDirDialog::getOpenDirectory(this, "Select the Directory to store the Reports", reportpathtextfield->getText());
    if(!reportpathstring.empty())
        reportpathtextfield->setText(reportpathstring + "/");

    return 1;
}

long Settings::SetVideoPath(FXObject*, FXSelector, void*)
{
    FXString vidpathstring = FXFileDialog::getOpenFilename(this, "Select the Default Video Player Binary", vidtextfield->getText());
    if(!vidpathstring.empty())
        vidtextfield->setText(vidpathstring);

    return 1;
}

long Settings::SetHtmlPath(FXObject*, FXSelector, void*)
{
    FXString htmlpathstring = FXFileDialog::getOpenFilename(this, "Select the Default Web Browser Binary", htmltextfield->getText());
    if(!htmlpathstring.empty())
        htmltextfield->setText(htmlpathstring);

    return 1;
}

long Settings::SetPdfPath(FXObject*, FXSelector, void*)
{
    FXString pdfpathstring = FXFileDialog::getOpenFilename(this, "Select the Default PDF Viewer Binary", pdftextfield->getText());
    if(!pdfpathstring.empty())
        pdftextfield->setText(pdfpathstring);

    return 1;
}

long Settings::SetImagePath(FXObject*, FXSelector, void*)
{
    FXString imgpathstring = FXFileDialog::getOpenFilename(this, "Select the Default Image Viewer Binary", imgtextfield->getText());
    if(!imgpathstring.empty())
        imgtextfield->setText(imgpathstring);

    return 1;
}
