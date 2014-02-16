#ifndef WOMBATFUNCTIONS_H
#define WOMBATFUNCTIONS_H

#include <string>
#include <cstdio>
#include <sys/stat.h>

std::string GetTime(void);
bool FileExists(const std::string& filename);

#endif // wombatfunctions.h
