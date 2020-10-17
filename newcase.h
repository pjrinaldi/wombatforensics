#ifndef NEWCASE_H
#define NEWCASE_H

// Copyright 2013-2020 Pasquale J. Rinaldi, Jr.
// Distrubted under the terms of the GNU General Public License version 2

#include "wombatinclude.h"
#include "globals.h"
#include "tskcpp.h"
#include "properties.h"
#include "wombatfunctions.h"
#include "libvsmbr.h"
#include "libvsgpt.h"

void ProcessVolume(QString evidstring);
void InitializeEvidenceStructure(QString evidstring);
void ProcessDir(TSK_FS_INFO* fsinfo, TSK_STACK* stack, TSK_INUM_T dirinum, const char* path, int eint, int vint, int pint, QString partitionpath);

#endif // newcase.h
