#ifndef WOMBATFUNCTIONS_H
#define WOMBATFUNCTIONS_H

#include <string>
#include <cstdio>
#include <sys/stat.h>

extern std::string GetTime(void);
extern bool FileExists(const std::string& filename);

#endif // wombatfunctions.h
