#ifndef TSKVARIABLE_H
#define TSKVARIABLE_H

// Copyright 2015 Pasquale J. Rinaldi, Jr.
// Distrubted under the terms of the GNU General Public License version 2

#include "tsk/libtsk.h"
#include <QDebug>

struct TskObject
{
    const TSK_TCHAR** imagepartspath;
    int partcount;
    unsigned long long objectid;
    unsigned long long offset;
    unsigned long long length;
    unsigned long long imglength;
    unsigned long long address;
    int objecttype;
    int type;
    int blocksize;
    int sectsize;
    unsigned long long fsoffset;
    unsigned long long resoffset;
    unsigned long long adsoffset;
    unsigned long long mftattrid;
    QString blockaddress;
    QStringList blkaddrlist;
    TSK_IMG_INFO* readimginfo;
    TSK_FS_INFO* readfsinfo;
    TSK_FS_FILE* readfileinfo;
};

struct FileSystemObject
{
    unsigned long long id;
    unsigned long long rootinum;
};

#endif // TSKVARIABLE_H
