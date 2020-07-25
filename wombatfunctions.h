#ifndef WOMBATFUNCTIONS_H
#define WOMBATFUNCTIONS_H

// Copyright 2013-2020 Pasquale J. Rinaldi, Jr.
// Distrubted under the terms of the GNU General Public License version 2

#include "wombatinclude.h"
#include "globals.h"
#include "tskcpp.h"
#include <zip.h>

void SleepLoop(const int ms);
std::string GetTime(void);
qint64 GetChildCount(QString filefilter);

QStringList GetChildFiles(QString filefilter);
bool FileExists(const std::string& filename);

int SegmentDigits(int);

void GenerateWombatCaseFile(void);
void RewriteSelectedIdContent(QModelIndex selectedid);

QString GenerateCategorySignature(const QMimeType mimetype);

void SavePasswordList(void);
void InitializePasswordList(void);

QString ConvertGmtHours(int gmtvar);

#define TICKS_PER_SECOND 10000000
#define EPOCH_DIFFERENCE 11644473600LL

QString ConvertWindowsTimeToUnixTime(uint64_t input);
QString ConvertWindowsTimeToUnixTimeUTC(uint64_t input);
QString ConvertUnixTimeToString(uint32_t input);

#endif // wombatfunctions.h
