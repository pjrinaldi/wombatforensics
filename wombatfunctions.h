#ifndef WOMBATFUNCTIONS_H
#define WOMBATFUNCTIONS_H

// Copyright 2015 Pasquale J. Rinaldi, Jr.
// Distrubted under the terms of the GNU General Public License version 2

#include "wombatinclude.h"
//#include "wombatvariable.h"
#include "globals.h"

std::string GetTime(void);
char* TskTimeToStringUTC(time_t time, char buf[128]);
unsigned long long GetChildCount(int type, unsigned long long address, unsigned long long parimgid = 0, unsigned long long parfsid = 0);
bool FileExists(const std::string& filename);
bool ProcessingComplete(void);
//void ProcessFile(QVector<QString> tmpstrings, QVector<unsigned long long> tmpints, FileData tmpdata, bool adsbool); 
TSK_WALK_RET_ENUM FileEntries(TSK_FS_FILE* tmpfile, const char* tmppath, void* tmpptr);
void GenerateThumbnails(void);
QVariant HashFile(TSK_FS_FILE* tmpfile, unsigned long long objid);
//QVariant MagicFile(TSK_FS_FILE* tmpfile, unsigned long long objid);
//QVariant AlternateDataStreamMagicFile(TSK_FS_FILE* readfileinfo, unsigned long long adsobjid);
void ThumbFile(TSK_FS_FILE* tmpfile, unsigned long long objid);
//void BlockFile(TSK_FS_FILE* tmpfile, unsigned long long objid, QVector<unsigned long long> adsattrid);
//void AlternateDataStreamBlockFile(TSK_FS_FILE* tmpfile, QVector<unsigned long long> adsobjid, QVector<unsigned long long> adsattrid);
void PropertyFile(TSK_FS_FILE* tmpfile, unsigned long long objid, unsigned long long fsoffset, int blksize, unsigned long long parfsid);
void AlternateDataStreamPropertyFile(TSK_FS_FILE* tmpfile, QVector<unsigned long long> adsobjid, QVector<unsigned long long> adsattrid);
void LogMessage(QString message);
void cnid_to_array(uint32_t cnid, uint8_t array[4]);
TSK_WALK_RET_ENUM GetBlockAddress(TSK_FS_FILE* tmpfile, TSK_OFF_T off, TSK_DADDR_T addr, char* buf, size_t size, TSK_FS_BLOCK_FLAG_ENUM flags, void *ptr);
QString GetFilePermissions(TSK_FS_META* tmpmeta);
QImage MakeThumb(const QString &img);
void SqlMap(FileData &filedata);
void FileMap(FileData &filedata);
void InitializeEvidenceStructure(QList<WombatVariable> wombatvarvector);
#endif // wombatfunctions.h
