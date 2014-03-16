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

TSK_WALK_RET_ENUM dirwalktest(TSK_FS_FILE* tmpfile, const char* tmppath, void* tmpptr)
{
    qDebug() << "FS File Name: " << tmpfile->name->name;
    qDebug() << "FS File Type: " << tmpfile->name->type;
    qDebug() << "FS File Parent: " << tmpfile->name->par_addr;
    return TSK_WALK_CONT;
}
