#ifndef COMMONFS_H
#define COMMONFS_H

// Copyright 2013-2022 Pasquale J. Rinaldi, Jr.
// Distrubted under the terms of the GNU General Public License version 2

#include "wombatinclude.h"
#include "globals.h"
#include "wombatfunctions.h"

QString ConvertBlocksToExtents(QList<uint> blocklist, uint blocksize, qulonglong rootdiroffset);
void GetNextCluster(ForImg* curimg, uint32_t clusternum, uint8_t fstype, qulonglong fatoffset, QList<uint>* clusterlist);

#endif // commonfs.h
