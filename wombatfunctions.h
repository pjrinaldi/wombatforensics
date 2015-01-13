#ifndef WOMBATFUNCTIONS_H
#define WOMBATFUNCTIONS_H

#include "wombatinclude.h"
#include "wombatvariable.h"
#include "globals.h"

std::string GetTime(void);
char* TskTimeToStringUTC(time_t time, char buf[128]);
int GetChildCount(int type, int address, int parimgid = 0);
bool FileExists(const std::string& filename);
bool ProcessingComplete(void);
void ProcessFile(QVector<QString> tmpstrings, QVector<int> tmpints, QStringList tmplist);
TSK_WALK_RET_ENUM FileEntries(TSK_FS_FILE* tmpfile, const char* tmppath, void* tmpptr);
void LogEntry(int caseid, int evidenceid, int jobid, int type, QString msg);
void StartJob(int type, int caseid, int evidenceid);
void EndJob(int jobid, int filecount, int processcount, int errorcount);
void cnid_to_array(uint32_t cnid, uint8_t array[4]);
std::string GetSegmentValue(IMG_AFF_INFO* curaffinfo, const char* segname);
TSK_WALK_RET_ENUM GetBlockAddress(TSK_FS_FILE* tmpfile, TSK_OFF_T off, TSK_DADDR_T addr, char* buf, size_t size, TSK_FS_BLOCK_FLAG_ENUM flags, void *ptr);
QString GetFilePermissions(TSK_FS_META* tmpmeta);
#endif // wombatfunctions.h
