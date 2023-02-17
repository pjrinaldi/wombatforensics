#include "messagelog.h"

FXIMPLEMENT(MessageLog,FXDialogBox,NULL, 0)

MessageLog::MessageLog(FXWindow* parent, const FXString& title):FXDialogBox(parent, title, DECOR_TITLE|DECOR_BORDER|DECOR_CLOSE, 0, 0, 400, 250, 0,0,0,0, 0, 0)
{
    mainframe = new FXVerticalFrame(this, LAYOUT_TOP|LAYOUT_FILL_X|LAYOUT_FILL_Y, 0, 0, 0, 0, 0, 0, 0, 0);
    msglog = new FXConsole(mainframe, this, ID_LOG, LAYOUT_TOP|LAYOUT_FILL_X|LAYOUT_FILL_Y);
    /*
    mainimage = new FXPNGImage(this->getApp(), aboutwombat);
    imgframe = new FXImageFrame(mainframe, mainimage, FRAME_NONE);
    FXString abouttext = "\n\nAbout Wombat Forensics v0.5\n\n";
    abouttext += "License: GPLv2\n";
    abouttext += "Copyright: 2013-2023 Pasquale J. Rinaldi, Jr.\n";
    abouttext += "Email: pjrinaldi@gmail.com\n\n";
    abouttext += "This program incorporates Fox Toolkit v1.7.6.81 (LGPLv2),\n";
    abouttext += "some of the elementary icons (GPLv3),\n";
    abouttext += "and tango icons (public domain).\n\n";
    abouttext += "Wombat icon and about image designed by Lindsay Pargman.\n";
    abouttext += "Design Implementation ideas brainstormed with Mark Smith.\n\n";
    abouttext += "If I have forgotten to mention any library or anyone,\n";
    abouttext += " let me know and I will update this content accordingly.";
    mainlabel = new FXLabel(mainframe, abouttext, NULL, JUSTIFY_LEFT);
    this->setBackColor(FX::colorFromName("white"));
    mainframe->setBackColor(FX::colorFromName("white"));
    mainlabel->setBackColor(FX::colorFromName("white"));
    */
}