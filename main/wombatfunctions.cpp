#include "wombatfunctions.h"

std::string GetTime()
{
    struct tm *newtime;
    time_t aclock;

    time(&aclock);   // Get time in seconds
    newtime = localtime(&aclock);   // Convert time to struct tm form 
    char timeStr[64];
    snprintf(timeStr, 64, "%.2d/%.2d/%.2d %.2d:%.2d:%.2d",
        newtime->tm_mon+1,newtime->tm_mday,newtime->tm_year % 100, 
        newtime->tm_hour, newtime->tm_min, newtime->tm_sec);

    return timeStr;
}

bool FileExists(const std::string& filename)
{
    struct stat buf;
    if (stat(filename.c_str(), &buf) != -1)
    {
        return true;
    }
    return false;
}

QString ByteArrayToShortDisplay(QByteArray ba)
{
    short intvalue = 0;
    memcpy(&intvalue, &ba.begin()[0], sizeof(short));
    QString tmpstring = "";
    return tmpstring.setNum(intvalue);
}

QString ByteArrayToHex(QByteArray ba)
{
    QString tmpstring = QString::fromUtf8(ba.toHex());
    QString outstring = "";
    for(int i=0; i < tmpstring.size() / 2; ++i)
    {
        outstring += tmpstring.at(2*i);
        outstring += tmpstring.at(2*i+1);
    }

    return outstring;
}

QString ByteArrayToHexDisplay(QByteArray ba)
{
    QString tmpstring = QString::fromUtf8(ba.toHex());
    QString outstring = "";
    for(int i=0; i < tmpstring.size()/2; ++i)
    {
        outstring += tmpstring.at(2*i);
        outstring += tmpstring.at(2*i+1);
        if(i < tmpstring.size()/2 - 1)
            outstring += " ";
    }

    return outstring;
}
int CheckBitSet(unsigned char c, int n)
{
    static unsigned char mask[] = {128, 64, 32, 16, 8, 4, 2, 1};
    return ((c & mask[n]) != 0)
}
}
