#ifndef WOMBATFUNCTIONS_H
#define WOMBATFUNCTIONS_H

#include "wombatinclude.h"
#include "globals.h"

std::string GetTime(void);
char* TskTimeToStringUTC(time_t time, char buf[128]);
int GetChildCount(int type, int address);
bool FileExists(const std::string& filename);
bool ProcessingComplete(void);
void ProcessFile(QVector<QString> tmpstrings, QVector<int> tmpints);
TSK_WALK_RET_ENUM FileEntries(TSK_FS_FILE* tmpfile, const char* tmppath, void* tmpptr);

#endif // wombatfunctions.h
