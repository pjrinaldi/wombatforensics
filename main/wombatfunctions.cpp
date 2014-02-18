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

QString SingleByteToString(QByteArray ba, int base)
{
    short intvalue = 0;
    memcpy(&intvalue, &ba.begin()[0], sizeof(short));
    QString tmpstring = "";
    tmpstring.setNum(intvalue, base);
    qDebug() << "Temp Value: " << tmpstring;
    if(base == 2 && tmpstring.size() < 8)
    {
    	int zerocount = 8 - tmpstring.size();
	tmpstring.prepend(QString::fromStdString(std::string(zerocount, '0')));
	return tmpstring;
    }
    else
    	return tmpstring;
}
// NEED TO FIX THE BYTE ARRAY TO STRING TO ACCOUNT FOR THE LENGTH OF THE BYTE ARRAY...
QString ByteArrayToString(QByteArray ba, int base)
{
    int intvalue = 0;
    memcpy(&intvalue, &ba.begin()[0], sizeof(int));
    QString tmpstring = "";
    tmpstring.setNum(intvalue, base);
    if(base == 2 && tmpstring.size() < 8)
    {
    	int zerocount = 8 - tmpstring.size();
	tmpstring.prepend(QString::fromStdString(std::string(zerocount, '0')));
	return tmpstring;
    }
    else
    	return tmpstring;
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


// COMPARE THE STRING VALUE TO DETERMINE IF ITS SET TO 1 OR NOT.
int CheckBitSet(unsigned char c, int n)
{
    static unsigned char mask[] = {1, 2, 4, 8, 16, 32, 64, 128};
    //static unsigned char mask[] = {128, 64, 32, 16, 8, 4, 2, 1};
    return ((c & mask[n]) != 0);
}
