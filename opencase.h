#ifndef OPENCASE_H
#define OPENCASE_H

#include "wombatinclude.h"
#include "globals.h"
#include "tskcpp.h"
#include "properties.h"
#include "wombatfunctions.h"

void PopulateTreeModel(QString evidstring);
void ParseDir(TSK_FS_INFO* fsinfo, TSK_STACK* stack, TSK_INUM_T dirnum, const char* path, QString partitionpath);

#endif // opencase.h
