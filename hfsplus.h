#ifndef HFSPLUS_H
#define HFSPLUS_H

// Copyright 2013-2022 Pasquale J. Rinaldi, Jr.
// Distrubted under the terms of the GNU General Public License version 2

#include "wombatinclude.h"
#include "globals.h"
#include "wombatfunctions.h"
#include "commonfs.h"

// might not need parinode and parfilename as it were...
qulonglong ParseHfsPlusDirectory(ForImg* curimg, uint32_t curstartsector, uint8_t ptreecnt);
void BuildPath(QString* path, QHash<uint32_t, QString>* dirhash, quint64 curparcnid);

#endif // hfsplus.h
