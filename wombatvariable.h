#ifndef WOMBATVARIABLE_H
#define WOMBATVARIABLE_H

// Copyright 2013-2019 Pasquale J. Rinaldi, Jr.
// Distrubted under the terms of the GNU General Public License version 2

#include "wombatinclude.h"

struct WombatVariable
{
    bool iscaseopen;
    QString tmpfilepath;
    QString tmpmntpath;
    QString imgdatapath;
    QString curerrmsg;
    QString evidencename;
    QString evidencepath;
    QString volumepath;
    QString partitionpath;
    QString curfilepath;
    std::vector<std::string> fullpathvector;
    qint64 itemcount;
    QString casename;
    QString casepath;
    QString selectedid;
    int segmentcount;
    TSK_IMG_TYPE_ENUM imgtype;
};

typedef struct
{
    TSK_IMG_INFO imginfo;
    libewf_handle_t* handle;
    char md5hash[33];
    int md5hashisset;
    TSK_TCHAR** images;
    int numimgs;
    uint8_t usedewfglob;
    tsk_lock_t readlock;
} IMG_EWF_INFO;

/*
struct FileSystemInfo
{
    uint16_t bytespersector;
    uint8_t sectorspercluster;
    uint16_t reservedareasize; // in sectors
    uint8_t fatcount;
    uint16_t rootdirmaxfiles; // # of 32-byte directory entries in the root dir, 0 for FAT32, FAT16 should use 512
    uint16_t fssectorcnt; // 0 if need 4-byte value
    uint8_t mediatype;
    uint16_t fatsize; // FAT size in sectors for FAT/12/16, 0 for FAT32
    uint32_t fs32sectorcnt; // 0 if can use 2-byte value
    uint8_t extbootsig; // 0x29 for FAT12/16
    uint32_t volserialnum; // volume serial number
    QString vollabel; // ASCII volume label
    QString fatlabel; // ASCI FAT/FAT12/FAT16
    uint32_t fat32size; // FAT size in sectors
    uint32_t rootdircluster; // cluster where root directory is located (FAT32 only)
    uint16_t fsinfosector; // FSINFO sector location
    int fstype; // fstype for filesystem
    QString fstypestr; // fstype string
    qint64 rootdiroffset; // offset to root directory
    qint64 fatoffset; // offset to 1st FAT
};

struct DirEntryInfo // 32 bytes
{
    uint8_t allocstatus; // first character of filename and/or allocation status
    QString name; // 2-11 characters of filename
    uint8_t attr; // bits
    uint8_t createdtenths; // (tenths of seconds)
    uint16_t createdtime; // housrs, minutes, seconds
    uint16_t createday;
    uint16_t accessday;
    uint16_t highfirstclusteraddr; // high 2 bytes of first cluster address (0 for FAT12/16)
    uint16_t writtentime;
    uint16_t writeday;
    uint16_t lowfirstclusteraddr; // low 2 bytes of first cluster address
    uint32_t size; // 0 for directories
};
*/
/*
typedef struct
{
    uint8_t parentreference[6];         // 0x00
    uint8_t parentsequencenumber[2];    // 0x06
    uint8_t createdtime[8];             // 0x08
    uint8_t modifiedtime[8];            // 0x10
    uint8_t changedtime[8];             // 0x18
    uint8_t accessedtime[8];            // 0x20
    uint8_t logicalsize[8];             // 0x28
    uint8_t physicalsize[8];            // 0x30
    uint8_t flags[8];                   // 0x38
    uint8_t filenamelength;             // 0x40
    uint8_t filenamenamespace;          // 0x41
    uint8_t filename;                   // 0x42
} ntfsattrfname;

typedef struct
{
    uint64_t mftreference;      // 0x00
    uint16_t indxrecordlength;  // 0x08
    uint16_t attrfnamelength;   // 0x0A
    uint32_t flags;             // 0x0C
    ntfsattrfname fnameinfo;    // 0x10
} ntfsindxrecord;
*/

Q_DECLARE_METATYPE(WombatVariable);
Q_DECLARE_METATYPE(IMG_EWF_INFO);
//Q_DECLARE_METATYPE(FileSystemInfo);
//Q_DECLARE_METATYPE(DirEntryInfo);
//Q_DECLARE_METATYPE(ntfsattrfname);
//Q_DECLARE_METATYPE(ntfsindxrecord);

#endif // WOMBATVARIABLE_H
