#ifndef NTFS_H
#define NTFS_H

// Copyright 2013-2022 Pasquale J. Rinaldi, Jr.
// Distrubted under the terms of the GNU General Public License version 2

#include "wombatinclude.h"
#include "globals.h"
#include "wombatfunctions.h"
#include "commonfs.h"

quint64 ParseNtfsDirectory(ForImg* curimg, uint32_t curstartsector, uint8_t ptreecnt, quint64 ntinode, quint64 parinode, QString parfilename, QString parlayout, QHash<quint64, quint64>* dirntinodehash, QHash<quint64, uint16_t>* ntinodehash);
quint64 GetMftEntryContent(ForImg* curimg, uint32_t curstartsector, uint8_t ptreecnt, quint64 ntinode, quint64 parentntinode, uint64_t parntinode, QString mftlayout, uint16_t mftentrybytes, uint32_t bytespercluster, quint64 inodecnt, QString filename, qint64 parinode, QString parfilename, uint16_t i30seqid, uint16_t i30parseqid, uint64_t i30create, uint64_t i30modify, uint64_t i30change, uint64_t i30access, quint64 curpos, quint64 endoffset, QHash<quint64, quint64>* dirntinodehash, QHash<quint64, uint16_t>* ntinodehash);
void GetRunListLayout(ForImg* curimg, uint32_t curstartsector, uint32_t bytespercluster, uint16_t mftentrybytes, quint64 curoffset, QString* layout);
void ParseNtfsOrphans(ForImg* curimg, uint32_t curstartsector, uint8_t ptreecnt, quint64 curinode, QHash<quint64, quint64>* dirntinodehash, QHash<quint64, uint16_t>* ntinodehash);

#endif // ntfs.h
