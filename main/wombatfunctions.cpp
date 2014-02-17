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

QString ByteArrayToHexDisplay(QByteArray ba)
{
    QString tmpstring = QString::fromUtf8(ba.toHex());
    QString outstring = "";
    for(int i=0; i < tmpstring.size() / 2; ++i)
    {
        outstring += tmpstring.at(2*i);
        outstring += tmpstring.at(2*i+1);
        outstring += " ";
    }

    return outstring;
}
