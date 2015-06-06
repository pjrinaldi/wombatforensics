#ifndef WOMBATFUNCTIONS_H
#define WOMBATFUNCTIONS_H

#include "wombatinclude.h"
#include "wombatvariable.h"
#include "globals.h"

std::string GetTime(void);
char* TskTimeToStringUTC(time_t time, char buf[128]);
unsigned long long GetChildCount(int type, unsigned long long address, unsigned long long parimgid = 0);
bool FileExists(const std::string& filename);
bool ProcessingComplete(void);
void ProcessFile(QVector<QString> tmpstrings, QVector<unsigned long long> tmpints); 
TSK_WALK_RET_ENUM FileEntries(TSK_FS_FILE* tmpfile, const char* tmppath, void* tmpptr);
void SecondaryProcessing(void);
void HashFile(TSK_FS_FILE* tmpfile, unsigned long long objid);
//void LogEntry(unsigned long long caseid, unsigned long long evidenceid, unsigned long long jobid, int type, QString msg);
void LogMessage(QString message);
//void StartJob(int type, unsigned long long caseid, unsigned long long evidenceid);
//void EndJob(unsigned long long jobid, unsigned long long filecount, unsigned long long processcount, unsigned long long errorcount);
void cnid_to_array(uint32_t cnid, uint8_t array[4]);
std::string GetSegmentValue(IMG_AFF_INFO* curaffinfo, const char* segname);
TSK_WALK_RET_ENUM GetBlockAddress(TSK_FS_FILE* tmpfile, TSK_OFF_T off, TSK_DADDR_T addr, char* buf, size_t size, TSK_FS_BLOCK_FLAG_ENUM flags, void *ptr);
QString GetFilePermissions(TSK_FS_META* tmpmeta);
QImage MakeThumb(const QString &img);
#endif // wombatfunctions.h
