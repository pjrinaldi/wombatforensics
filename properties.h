#ifndef PROPERTIES_H
#define PROPERTIES_H

#include "wombatinclude.h"
#include "wombatfunctions.h"
#include "globals.h"
#include "tskcpp.h"
#include <zip.h>

// Copyright 2013-2020 Pasquale J. Rinaldi, Jr.
// Distrubted under the terms of the GNU General Public License version 2

QString GetBlockList(TSK_FS_FILE* tmpfile);
QString GetAdsBlockList(TSK_FS_FILE* tmpfile, qint64 attrid);
TSK_WALK_RET_ENUM GetBlockAddress(TSK_FS_FILE* tmpfile, TSK_OFF_T off, TSK_DADDR_T addr, char* buf, size_t size, TSK_FS_BLOCK_FLAG_ENUM flags, void *ptr);
QString GetFilePermissions(TSK_FS_META* tmpmeta);
QString GetFileSystemLabel(TSK_FS_INFO* fsinfo);

void WriteEvidenceProperties(TSK_IMG_INFO* curimginfo, QString evidencepath, QString imgfullpath);
void WriteVolumeProperties(TSK_VS_INFO* curvolinfo, QString volumepath);
void WriteFileSystemProperties(TSK_FS_INFO* curfsinfo, QString partitionpath);
void WriteFileProperties(TSK_FS_FILE* curfileinfo, QString partpath);
void WriteAlternateDataStreamProperties(TSK_FS_FILE* curfileinfo, QString adsname, QString attrid, QString partpath, bool isres);

#endif // properties.h
