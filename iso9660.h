#ifndef ISO9660_H
#define ISO9660_H

// Copyright 2013-2022 Pasquale J. Rinaldi, Jr.
// Distrubted under the terms of the GNU General Public License version 2

#include "wombatinclude.h"
#include "globals.h"
#include "wombatfunctions.h"
#include "commonfs.h"

void ParseIsoDirectory(ForImg* curimg, uint32_t curstartsector, uint8_t ptreecnt);
quint64 ParseDirectoryContents(ForImg* curimg, uint8_t ptreecnt, uint32_t extblk, uint32_t dirlen, uint32_t parblk, uint32_t blocksize, bool utf16, quint64 parinode, QString parpath, QList<uint32_t>* parsedblocks);

#endif // iso9660.h
