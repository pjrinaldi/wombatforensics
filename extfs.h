#ifndef EXTFS_H
#define EXTFS_H

// Copyright 2013-2020 Pasquale J. Rinaldi, Jr.
// Distrubted under the terms of the GNU General Public License version 2

#include "wombatinclude.h"
#include "globals.h"
#include "wombatfunctions.h"
#include "commonfs.h"

void GetContentBlocks(ForImg* curimg, uint32_t curstartsector, uint32_t blocksize, quint64 curoffset, QString* incompatflags, QList<uint32_t>* blocklist);
quint64 ParseExtDirectory(ForImg* curimg, uint32_t curstartsector, uint8_t ptreecnt, quint64 curextinode, quint64 parinode, QString parfilename, QString dirlayout);

#endif // extfs.h
