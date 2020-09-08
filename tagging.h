#ifndef TAGGING_H
#define TAGGING_H

// Copyright 2013-2020 Pasquale J. Rinaldi, Jr.
// Distrubted under the terms of the GNU General Public License version 2

#include "wombatinclude.h"
#include "wombatfunctions.h"
#include "globals.h"
//#include "tskcpp.h"

void AddTag(QString artifact, QString tagstring);
void RemTag(QString artifact, QString tagstring);
void CreateArtifactFile(QString artifact, QString idkeyvalue, QString htmlcontent);

//QFile artifactfile;
//QStringList artifactlist;
// RemoveTag();
// CreateNewTag();
// TagFile()

#endif // TAGGING_H
