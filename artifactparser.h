#ifndef ARTIFACTPARSER_H
#define ARTIFACTPARSER_H

// Copyright 2013-2020 Pasquale J. Rinaldi, Jr.
// Distrubted under the terms of the GNU General Public License version 2

#include "wombatinclude.h"
#include "globals.h"
#include "tskcpp.h"
#include <liblnk.h>
#include <zip.h>

#define TICKS_PER_SECOND 10000000
#define EPOCH_DIFFERENCE 11644473600LL

QString ConvertWindowsTimeToUnixTime(uint64_t input);
QString ParseLnkArtifact(QString lnkname, QString lnkid);
QString ParseI30Artifact(QString i30name, QString i30id);
QString ParseInfo2Artifact(QString info2name, QString info2id);
QString ParseIDollarArtifact(QString idollarname, QString idollarid);
QString ParsePrefetchArtifact(QString pfname, QString pfid);
QString ParseArchiveArtifact(QString archivename, QString archiveid);

#endif // artifactparser.h
