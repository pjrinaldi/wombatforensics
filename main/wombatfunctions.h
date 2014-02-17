#ifndef WOMBATFUNCTIONS_H
#define WOMBATFUNCTIONS_H

#include "wombatinclude.h"

std::string GetTime(void);
bool FileExists(const std::string& filename);

QString ByteArrayToHex(QByteArray ba);
QString ByteArrayToHexDisplay(QByteArray ba);
QString ByteArrayToShortDisplay(QByteArray ba);
int CheckBitSet(unsigned char c, int n);

#endif // wombatfunctions.h
