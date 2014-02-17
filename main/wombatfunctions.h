#ifndef WOMBATFUNCTIONS_H
#define WOMBATFUNCTIONS_H

#include "wombatinclude.h"

extern std::string GetTime(void);
extern bool FileExists(const std::string& filename);

QString ByteArrayToHex(QByteArray ba);
QString ByteArrayToHexDisplay(QByteArray ba);
QString ByteArrayToShortDisplay(QByteArray ba);

#endif // wombatfunctions.h
