#include "wombatfunctions.h"
// phone test
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

TSK_WALK_RET_ENUM FileEntries(TSK_FS_FILE* tmpfile, const char* tmppath, void* tmpptr)
{
    char buf[128];
    TSK_FS_HASH_RESULTS hashresults;
    // "Name" << "Full Path" << "Size (Bytes)" << "Signature" << "Extension" << "Created (UTC)" << "Accessed (UTC)" << "Modified (UTC)" << "Status Changed (UTC)" << "MD5 Hash";
    qDebug() << "FS File Name: " << tmpfile->name->name;
    qDebug() << "FS File Type: " << tmpfile->name->type;
    qDebug() << "FS File Parent: " << tmpfile->name->par_addr;
    qDebug() << "Accessed Time: " << tmpfile->meta->atime;
    qDebug() << "Accessed Time (readable): " << tsk_fs_time_to_str(tmpfile->meta->atime, buf);
    uint8_t retval = tsk_fs_file_hash_calc(tmpfile, &hashresults, TSK_BASE_HASH_MD5);
    qDebug() << "MD5 Return Value: " << retval;
    //qDebug() << QString((const char*)(hashresults.md5_digest));
    //QString hashstring(reinterpret_cast<const char*>(hashresults->md5_digest), 16);
    //qDebug() << "MD5 of File: " << QString::fromLatin1((const char*)(hashresults.md5_digest), 16);
    //uint8_t retval = tsk_fs_file_hash_calc(TSK_FS_FILE* tmpfile, TSK_FS_HASH_RESULTS* hashresults, TSK_BASE_HASH_ENUM tmpflags);
    return TSK_WALK_CONT;
}
