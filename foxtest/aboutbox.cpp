#include "aboutbox.h"

FXIMPLEMENT(AboutBox,FXDialogBox,NULL, 0)

AboutBox::AboutBox(FXWindow* parent, const FXString& title):FXDialogBox(parent, title, DECOR_TITLE|DECOR_BORDER|DECOR_CLOSE, 0, 0, 400, 650, 0,0,0,0, 0, 0)
{
    mainframe = new FXVerticalFrame(this, LAYOUT_TOP|LAYOUT_FILL_X|LAYOUT_FILL_Y, 0, 0, 0, 0, 0, 0, 0, 0);
    mainimage = new FXPNGImage(this->getApp(), aboutwombat);
    imgframe = new FXImageFrame(mainframe, mainimage, FRAME_NONE);
    FXString abouttext = "\n\nAbout Wombat Forensics v0.5\n\n";
    abouttext += "License: GPLv2\n";
    abouttext += "Copyright: 2013-2023 Pasquale J. Rinaldi, Jr.\n";
    abouttext += "Email: pjrinaldi@gmail.com\n\n";
    abouttext += "This program incorporates Fox Toolkit v1.7.6.81 (LGPLv2),\n";
    abouttext += "some of the elementary icons (GPLv3),\n";
    abouttext += "and tango icons (public domain).\n\n";
    //abouttext += "Wombat SQLite Forensics parses a SQLite file (db, wal, journal)\n";
    //abouttext += "to manually parse the files from a forensic perspective rather than\n";
    //abouttext += "a database browser.\n\n";
    abouttext += "Wombat icon and about image designed by Lindsay Pargman.\n";
    abouttext += "Design Implementation ideas brainstormed with Mark Smith.\n\n";
    abouttext += "If I have forgotten to mention any library or anyone,\n";
    abouttext += " let me know and I will update this content accordingly.";
    mainlabel = new FXLabel(mainframe, abouttext, NULL, JUSTIFY_LEFT);
    this->setBackColor(FX::colorFromName("white"));
    mainframe->setBackColor(FX::colorFromName("white"));
    mainlabel->setBackColor(FX::colorFromName("white"));
}
/*
    abouttext += "<p>I also make use of the libewf (LPGLv3), and afflib (public domain) to read content from these forensic image types.";
    abouttext += " Lnk file parser uses the liblnk library from Joachim Metz provided under the LGPLv3+.";
    abouttext += " Used libfwnt library from Joachim Metz (LGPLv3) to uncompress the MAM prefetch file format.";
    abouttext += " PDF page generation and display provided by the Poppler Qt5 library which is released under the GPL.";
    abouttext += " Make use of libudev to get a list of devices to select for creating a forensic image.";
    abouttext += " Registry parsing provided by libregf library from Joachim Metz provided under the LGPLv3.";
    abouttext += "<p>Image thumbnails were generated using the Magick++ library which is provided under a modified Apache 2.0. Video thumbnails were generated using the libffmpegthumbnailer library and Magick++.";
    abouttext += " The libffmpegthumbnailer library is provided under the GPLv2. Video player makes use of the QtAV library.";
    abouttext += "<p>Zip archive extraction and preview uses the libzip library released under 3-clause BSD license and Copyright (C) 1999-2019 Dieter Baron and Thomas Klausner.";
    abouttext += " Hashing makes use of the BLAKE3 hashing function which is released under the CC0 1.0 Universal license by the BLAKE3-team.";
    abouttext += "<p>xchomp didn't have a license that I could find, but was created by Polygen Corporation, Jerry J. Shekhel.";

 */ 
