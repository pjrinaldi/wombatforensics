#ifndef ARTIFACTPARSER_H
#define ARTIFACTPARSER_H

// Copyright 2013-2020 Pasquale J. Rinaldi, Jr.
// Distrubted under the terms of the GNU General Public License version 2

#include "wombatinclude.h"
#include "wombatfunctions.h"
#include "globals.h"
//#include "tskcpp.h"
#include <liblnk.h>
#include <zip.h>

QString ParseLnkArtifact(QString lnkname, QString lnkid);
QString ParseI30Artifact(QString i30name, QString i30id);
QString ParseInfo2Artifact(QString info2name, QString info2id);
QString ParseIDollarArtifact(QString idollarname, QString idollarid);
QString ParsePrefetchArtifact(QString pfname, QString pfid);
QString ParseArchiveArtifact(QString archivename, QString archiveid);

void TransferThumbnails(QString thumbid, QString reppath);
void TransferFiles(QString thumbid, QString reppath);
void TransferArtifacts(QString artifact, QString reppath);

#endif // artifactparser.h
