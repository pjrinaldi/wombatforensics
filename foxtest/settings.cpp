#include "settings.h"

FXIMPLEMENT(Settings,FXDialogBox,NULL, 0)

Settings::Settings(FXWindow* parent, const FXString& title):FXDialogBox(parent, title, DECOR_TITLE|DECOR_BORDER|DECOR_CLOSE, 0, 0, 500, 290, 0,0,0,0, 0, 0)
{
    mainframe = new FXVerticalFrame(this, LAYOUT_TOP|LAYOUT_FILL_X|LAYOUT_FILL_Y, 0, 0, 0, 0, 0, 0, 0, 0);
    hframe1 = new FXHorizontalFrame(mainframe, LAYOUT_TOP|LAYOUT_FILL_X);
    thumbsizelabel = new FXLabel(hframe1, "Thumbnail Size (pixels):", NULL, JUSTIFY_LEFT);
    new FXSpring(hframe1, 400);
    thumbsizespinner = new FXSpinner(hframe1, 5);
    thumbsizespinner->setRange(64, 384);
    thumbsizespinner->setIncrement(64);
    thumbsizespinner->setValue(128);
    hframe2 = new FXHorizontalFrame(mainframe, LAYOUT_TOP|LAYOUT_FILL_X);
    hframe3 = new FXHorizontalFrame(mainframe, LAYOUT_TOP|LAYOUT_FILL_X);
    hframe4 = new FXHorizontalFrame(mainframe, LAYOUT_TOP|LAYOUT_FILL_X);
    hframe5 = new FXHorizontalFrame(mainframe, LAYOUT_TOP|LAYOUT_FILL_X);
    hframe6 = new FXHorizontalFrame(mainframe, LAYOUT_TOP|LAYOUT_FILL_X);
    hframe7 = new FXHorizontalFrame(mainframe, LAYOUT_TOP|LAYOUT_FILL_X);
    hframe8 = new FXHorizontalFrame(mainframe, LAYOUT_TOP|LAYOUT_FILL_X);
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
    this->setBackColor(FX::colorFromName("white"));
    mainframe->setBackColor(FX::colorFromName("white"));
    //mainlabel->setBackColor(FX::colorFromName("white"));
}
