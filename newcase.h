#ifndef NEWCASE_H
#define NEWCASE_H

#include "wombatinclude.h"
#include "globals.h"
#include "tskcpp.h"
#include "properties.h"
#include "wombatfunctions.h"
//#include "digging.h" // for generatecategorysignature which i should move back to wombatfunctions and just #include "wombatfunctions.h" here...

void InitializeEvidenceStructure(QString evidstring);
void ProcessDir(TSK_FS_INFO* fsinfo, TSK_STACK* stack, TSK_INUM_T dirinum, const char* path, int eint, int vint, int pint, QString partitionpath);

#endif // newcase.h
