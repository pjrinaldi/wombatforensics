#ifndef OPENCASE_H
#define OPENCASE_H

#include "wombatinclude.h"
#include "globals.h"
#include "tskcpp.h"
#include "properties.h"
#include "wombatfunctions.h"

// Copyright 2013-2020 Pasquale J. Rinaldi, Jr.
// Distrubted under the terms of the GNU General Public License version 2
/*
struct AddEvidenceVariable
{
    QString partitionpath;
    int evidcnt;
    int volcnt;
    int partint;
};
void PopulateTskTree(QString evidstring);
TSK_WALK_RET_ENUM TreeEntries(TSK_FS_FILE* tmpfile, const char* tmppath, void* tmpptr);
*/
void PopulateTreeModel(QString evidstring);
void ParseDir(TSK_FS_INFO* fsinfo, TSK_STACK* stack, TSK_INUM_T dirnum, const char* path, QString partitionpath);

#endif // opencase.h
