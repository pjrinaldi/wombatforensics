#ifndef TSKVARIABLE_H
#define TSKVARIABLE_H

#include "tsk/libtsk.h"
#include <QDebug>

struct TskObject
{
    const TSK_TCHAR** imagepartspath;
    int partcount;
    int offset;
    int length;
    int imglength;
    int address;
    int objecttype;
    int type;
    int blocksize;
    int sectsize;
    int fsoffset;
    int resoffset;
    QString blockaddress;
    QStringList blkaddrlist;
    TSK_IMG_INFO* readimginfo;
    TSK_FS_INFO* readfsinfo;
    TSK_FS_FILE* readfileinfo;
};

struct FileSystemObject
{
    int id;
    int rootinum;
};

#endif // TSKVARIABLE_H
