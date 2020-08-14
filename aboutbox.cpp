#include "aboutbox.h"

// Copyright 2013-2020 Pasquale J. Rinaldi, Jr.
// Distrubted under the terms of the GNU General Public License version 2

AboutBox::AboutBox(QWidget* parent) : QDialog(parent), ui(new Ui::AboutBox)
{
    ui->setupUi(this);
    QString abouttext = "<h3>About WombatForensics v0.2</h3>";
    abouttext += "<h5>License: GPLv2</h5>";
    abouttext += "<4>Copyright 2013-2020 Pasquale J. Rinaldi, Jr.</h4>";
    abouttext += "<h5>Email: pjrinaldi@gmail.com</h5>";
    abouttext += "<p>This program incorporates modified code from the qhexedit2 project to implement the hexviewers.";
    abouttext += " qhexedit2 is provided under the GPLv2.1. I also used Qt5.15.0, provided by the LGPL, the sleuthkit c/c++ functions, provided under the Common Public License, libtar provided under the BSD 4-Clause, and some of the elementary (GPLv3), and tango (public domain) icons. The Video thumbnail error icon was created by unlimicon from Noun Project.";
    abouttext += "<p>I also make use of the ewfmount (LPGLv3) and affuse (public domain) programs.";
    abouttext += " Lnk file parser uses the liblnk library from Joachim Metz provided under the LGPLv3+.";
    abouttext += " Used libfwnt library from Joachim Metz (LGPLv3) to uncompress the MAM prefetch file format.";
    abouttext += " PDF page generation and display provided by the Poppler Qt5 library which is released under the GPL.";
    abouttext += " Image thumbnails were generated using the Magick++ library which is provided under a modified Apache 2.0. Video thumbnails were generated using the libffmpegthumbnailer library and Magick++.";
    abouttext += " The libffmpegthumbnailer library is provided under the GPLv2. Video player makes use of the QtAV library.";
    abouttext += " Zip archive extraction and preview uses the libzip library released under 3-clause BSD license and Copyright (C) 1999-2019 Dieter Baron and Thomas Klausner.";
    abouttext += " xchomp didn't have a license that I could find, but was created by Polygen Corporation, Jerry J. Shekhel.";
    abouttext += "<p>The wombat icon as well as the About dialog graphic was designed by Lindsey Pargman and a lot of the design implementation ideas were brainstormed with Mark Smith.</p>";
    abouttext += "<p>If I have forgotten to mention any library or anyone, let me know and I will update this content accordingly.</p>";
    ui->textlabel->setText(abouttext);
    this->hide();
}

AboutBox::~AboutBox()
{
}

void AboutBox::HideClicked()
{
    this->hide();
}

void AboutBox::ShowAbout()
{
    this->show();
}

void AboutBox::mousePressEvent(QMouseEvent* e)
{
    if(e->type() == QEvent::MouseButtonPress)
    {
        this->hide();
    }
}

