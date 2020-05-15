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
//Q_DECLARE_METATYPE(ntfsattrfname);
//Q_DECLARE_METATYPE(ntfsindxrecord);

#endif // WOMBATVARIABLE_H
