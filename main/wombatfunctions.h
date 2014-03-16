#ifndef WOMBATFUNCTIONS_H
#define WOMBATFUNCTIONS_H

#include "wombatinclude.h"

std::string GetTime(void);
bool FileExists(const std::string& filename);
TSK_WALK_RET_ENUM dirwalktest(TSK_FS_FILE* tmpfile, const char* tmppath, void* tmpptr);

#endif // wombatfunctions.h
