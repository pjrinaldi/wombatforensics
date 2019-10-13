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

struct AddEvidenceVariable
{
    QString partitionpath;
    int evidcnt;
    int volcnt;
    int partint;
};

struct FSFileVariable
{
    TskFsFile* fsfile;
    QString partpath;
    TSK_STACK* stack;
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

Q_DECLARE_METATYPE(WombatVariable);
Q_DECLARE_METATYPE(AddEvidenceVariable);
Q_DECLARE_METATYPE(IMG_EWF_INFO);
Q_DECLARE_METATYPE(FSFileVariable);

#endif // WOMBATVARIABLE_H
