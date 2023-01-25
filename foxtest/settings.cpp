#include "settings.h"

FXIMPLEMENT(Settings,FXDialogBox,NULL, 0)

Settings::Settings(FXWindow* parent, const FXString& title):FXDialogBox(parent, title, DECOR_TITLE|DECOR_BORDER|DECOR_CLOSE, 0, 0, 480, 300, 0,0,0,0, 10, 10)
{
    mainframe = new FXVerticalFrame(this, LAYOUT_TOP|LAYOUT_FILL_X|LAYOUT_FILL_Y, 0, 0, 0, 0, 10, 10, 10, 10);
    hframe1 = new FXHorizontalFrame(mainframe, LAYOUT_TOP|LAYOUT_FILL_X);
    thumbsizelabel = new FXLabel(hframe1, "Thumbnail Size (pixels):", NULL, LAYOUT_FILL_X|JUSTIFY_LEFT);
    new FXSpring(hframe1);
    thumbsizespinner = new FXSpinner(hframe1, 5);
    thumbsizespinner->setRange(64, 384);
    thumbsizespinner->setIncrement(64);
    thumbsizespinner->setValue(128);
    hframe2 = new FXHorizontalFrame(mainframe, LAYOUT_TOP|LAYOUT_FILL_X);
    vidthumblabel = new FXLabel(hframe2, "Generate Video Thumbnail Every (%): ", NULL, LAYOUT_FILL_X|JUSTIFY_LEFT);
    new FXSpring(hframe2);
    vidthumbspinner = new FXSpinner(hframe2, 5);
    vidthumbspinner->setRange(10, 100);
    vidthumbspinner->setIncrement(5);
    vidthumbspinner->setValue(25);
    hframe3 = new FXHorizontalFrame(mainframe, LAYOUT_TOP|LAYOUT_FILL_X);
    casepathlabel = new FXLabel(hframe3, "Set Case Path:", NULL, LAYOUT_FILL_X|JUSTIFY_LEFT);
    new FXSpring(hframe3);
    casepathtextfield = new FXTextField(hframe3, 40);
    casepathbutton = new FXButton(hframe3, "Browse");
    casepathtextfield->setText(FXString(getenv("HOME")) + "/WombatCases/");
    hframe4 = new FXHorizontalFrame(mainframe, LAYOUT_TOP|LAYOUT_FILL_X);
    reportpathlabel = new FXLabel(hframe4, "Set Report Path:", NULL, LAYOUT_FILL_X|JUSTIFY_LEFT);
    new FXSpring(hframe4);
    reportpathtextfield = new FXTextField(hframe4, 40);
    reportpathbutton = new FXButton(hframe4, "Browse");
    reportpathtextfield->setText(FXString(getenv("HOME")) + "/Reports/");
    hframe5 = new FXHorizontalFrame(mainframe, LAYOUT_TOP|LAYOUT_FILL_X);
    reporttzlabel = new FXLabel(hframe5, "Report TimeZone:", NULL, LAYOUT_FILL_X|JUSTIFY_LEFT);
    reporttzcombo = new FXComboBox(hframe5, 45);
    hframe6 = new FXHorizontalFrame(mainframe, LAYOUT_TOP|LAYOUT_FILL_X);
    autosavelabel = new FXLabel(hframe6, "Set Autosave Interval (minutes):", NULL, LAYOUT_FILL_X|JUSTIFY_LEFT);
    new FXSpring(hframe6);
    autosavespinner = new FXSpinner(hframe6, 5);
    autosavespinner->setRange(1, 120);
    autosavespinner->setIncrement(1);
    autosavespinner->setValue(20);
    hframe7 = new FXHorizontalFrame(mainframe, LAYOUT_TOP|LAYOUT_FILL_X);
    carvingmanagerlabel = new FXLabel(hframe7, "Manage Carving File Types List:", NULL, LAYOUT_FILL_X|JUSTIFY_LEFT);
    new FXSpring(hframe7);
    carvingmanagerbutton = new FXButton(hframe7, "Manage");
    hframe8 = new FXHorizontalFrame(mainframe, LAYOUT_TOP|LAYOUT_FILL_X);
    new FXLabel(hframe8, "", NULL, LAYOUT_FILL_X);
    new FXSpring(hframe8);
    cancelbutton = new FXButton(hframe8, "Cancel");
    savebutton = new FXButton(hframe8, "Save");
    //mainimage = new FXPNGImage(this->getApp(), aboutwombat);
    //imgframe = new FXImageFrame(mainframe, mainimage, FRAME_NONE);
    /*
    FXString abouttext = "\n\nAbout Wombat SQLite v0.1\n\n";
    abouttext += "License: GPLv2\n";
    abouttext += "Copyright: 2022-2023 Pasquale J. Rinaldi, Jr.\n";
    abouttext += "Email: pjrinaldi@gmail.com\n\n";
    abouttext += "This program incorporates Fox Toolkit v1.7.6.81 (LGPLv2),\n";
    abouttext += "some of the elementary icons (GPLv3),\n";
    abouttext += "and tango icons (public domain).\n\n";
    abouttext += "Wombat SQLite Forensics parses a SQLite file (db, wal, journal)\n";
    abouttext += "to manually parse the files from a forensic perspective rather than\n";
    abouttext += "a database browser.\n\n";
    abouttext += "Wombat icon and about image designed by Lindsay Pargman.\n";
    abouttext += "Design Implementation ideas brainstormed with Mark Smith.\n\n";
    abouttext += "If I have forgotten to mention any library or anyone,\n";
    abouttext += " let me know and I will update this content accordingly.";
    mainlabel = new FXLabel(mainframe, abouttext, NULL, JUSTIFY_LEFT);
    */
    //this->setBackColor(FX::colorFromName("white"));
    //mainframe->setBackColor(FX::colorFromName("white"));
    //mainlabel->setBackColor(FX::colorFromName("white"));
}
