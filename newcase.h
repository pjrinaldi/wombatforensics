#ifndef NEWCASE_H
#define NEWCASE_H

// Copyright 2013-2020 Pasquale J. Rinaldi, Jr.
// Distrubted under the terms of the GNU General Public License version 2

#include "wombatinclude.h"
#include "globals.h"
#include "wombatfunctions.h"
#include "reporting.h"

#include "commonfs.h"
#include "extfs.h"
#include "exfatfs.h"
#include "fatfs.h"
#include "ntfs.h"
#include "hfs.h"

void ProcessForensicImage(ForImg* curimg);
QString ParseFileSystem(ForImg* curimg, uint32_t curstartsector, uint8_t ptreecnt);
void ParseLogicalImage(ForImg* curimg);
void ParsePartition(ForImg* curimg, uint32_t cursectoroffset, uint32_t cursectorsize, uint8_t ptreecnt, uint8_t allocstatus);
uint8_t ParseExtendedPartition(ForImg* curimg, uint32_t curstartsector, uint32_t cursectorsize, uint8_t ptreecnt);
void ParseDirectoryStructure(ForImg* curimg, uint32_t curstartsector, uint8_t ptreecnt);
qulonglong AddVirtualFileSystemFiles(ForImg* curimg, uint8_t ptreecnt, uint8_t fatcount, qulonglong fatsize, qulonglong curinode);

#endif // newcase.h
