#ifndef WOMBATFUNCTIONS_H
#define WOMBATFUNCTIONS_H

#include "wombatinclude.h"

std::string GetTime(void);
bool FileExists(const std::string& filename);

//template <typename Iterator>
//BytesToBinary(Iterator start, Iterator end);
/*
template<typename T>
void BytesToBinary(T start, T end)
{
    for(T it = start; it != end; ++it)
    {
        //qDebug() << "Byte to Hex: " << Translate::ByteToHex(it);
        cout << it;
    }
};*/
//QString ByteArrayToHex(QByteArray ba);
//QString ByteArrayToHexDisplay(QByteArray ba);
//QString ByteArrayToShortDisplay(QByteArray ba);
//QString ByteArrayToString(QByteArray ba, int base);
//QString SingleByteToString(QByteArray ba, int base);
//int CheckBitSet(unsigned char c, int n);

#endif // wombatfunctions.h
