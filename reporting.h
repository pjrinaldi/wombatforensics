#ifndef REPORTING_H
#define REPORTING_H

#include "wombatinclude.h"
#include "globals.h"
#include "tskcpp.h"
#include <liblnk.h>
#include <zip.h>

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

#endif // reporting.h
