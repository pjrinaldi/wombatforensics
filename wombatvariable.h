#ifndef WOMBATVARIABLE_H
#define WOMBATVARIABLE_H

// Copyright 2013-2021 Pasquale J. Rinaldi, Jr.
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
};


Q_DECLARE_METATYPE(WombatVariable);

#endif // WOMBATVARIABLE_H
