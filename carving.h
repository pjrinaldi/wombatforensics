#ifndef CARVING_H
#define CARVING_H

// Copyright 2013-2020 Pasquale J. Rinaldi, Jr.
// Distributed under the terms of the GNU General Public License version 2


#include "globals.h"
#include "tskcpp.h"
#include "tskcpp.h"
#include <liblnk.h>
#include <zip.h>
#include "videoviewer.h"
// LIBPOPPLER-QT5 HEADER
#include <poppler-qt5.h>

void GenerateCarving(QStringList plist, QStringList flist);
void PopulateCarvedFiles(QString cfile);
void HeaderSearch();
void FooterSearch();

#endif // carving.h
