#ifndef WOMBATFUNCTIONS_H
#define WOMBATFUNCTIONS_H

// Copyright 2013-2020 Pasquale J. Rinaldi, Jr.
// Distrubted under the terms of the GNU General Public License version 2

#include "wombatinclude.h"
#include "globals.h"
#include <magic.h>

void SleepLoop(const int ms);
std::string GetTime(void);
qint64 GetChildCount(QString filefilter);

QStringList GetChildFiles(QString filefilter);
bool FileExists(const std::string& filename);

int SegmentDigits(int);

void GenerateWombatCaseFile(void);
void RewriteSelectedIdContent(QModelIndex selectedid);

QString GenerateCategorySignature(QByteArray sigbuf, QString filename);

QString GenerateCategorySignature(ForImg* curimg, QString filename, qulonglong fileoffset);

void SavePasswordList(void);
void InitializePasswordList(void);

QString ConvertGmtHours(int gmtvar);

#define TICKS_PER_SECOND 10000000
#define EPOCH_DIFFERENCE 11644473600LL
#define NSEC_BTWN_1904_1970	(uint32_t) 2082844800U

qint64 ConvertDosTimeToUnixTime(uint8_t t1, uint8_t t2, uint8_t d1, uint8_t d2);
qint64 ConvertExfatTimeToUnixTime(uint8_t t1, uint8_t t2, uint8_t d1, uint8_t d2, uint8_t utc);
QString ConvertWindowsTimeToUnixTime(uint64_t input);
QString ConvertWindowsTimeToUnixTimeUTC(uint64_t input);
QString ConvertUnixTimeToString(uint32_t input);
uint32_t ConvertNtfsTimeToUnixTime(uint64_t ntdate);
uint32_t ConvertHfsTimeToUnixTime(uint32_t hfsdate);


QString ConvertBlocksToExtents(QList<uint32_t> blocklist, uint blocksize);

QString HashFiles(QString itemid);

#endif // wombatfunctions.h
