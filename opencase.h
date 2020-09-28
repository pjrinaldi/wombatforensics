#ifndef OPENCASE_H
#define OPENCASE_H

#include "wombatinclude.h"
#include "globals.h"
//#include "fusefunctions.h"
//#include "sqfuse.h"
#include "tskcpp.h"
#include "properties.h"
#include "wombatfunctions.h"

// Copyright 2013-2020 Pasquale J. Rinaldi, Jr.
// Distrubted under the terms of the GNU General Public License version 2

void PopulateTreeModel(QString evidstring);
void ParseDir(TSK_FS_INFO* fsinfo, TSK_STACK* stack, TSK_INUM_T dirnum, const char* path, QString partitionpath);

#endif // opencase.h
