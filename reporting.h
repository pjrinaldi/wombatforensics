#ifndef REPORTING_H
#define REPORTING_H

#include "wombatinclude.h"
#include "globals.h"
//#include "tskcpp.h"
#include <liblnk.h>
#include <zip.h>

// Copyright 2013-2020 Pasquale J. Rinaldi, Jr.
// Distrubted under the terms of the GNU General Public License version 2

int UpdateBookmarkItems(QString tagname);
void UpdateEvidenceList(void);
void AddItem(QString content, QString section);
void AddTLinkItem(int tagid, QString tagname);
void AddELinkItem(int evidid, QString evidname);
void AddEvidItem(QString content);
void RemoveTLinkItem(QString tagname);
void RemoveELinkItem(QString evidname);
void AddTagItem(int tagid, QString tagname);
void RemoveTagItem(QString tagname);
void RemoveEvidItem(QString evidname);
void UpdateTLinkItem(int tagid, QString oldname, QString tagname);
void UpdateTagItem(int tagid, QString oldname, QString tagname);
void AddFileItem(QString tagname, QString content);
void RemoveFileItem(QString fileid);
void AddPartItem(QString content);
//void RemovePartItem(QString partname);

void InitializeTaggedList(void);
void SaveTaggedList(void);

#endif // reporting.h
