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
    //qDebug() << "Accessed Time: " << tmpfile->meta->atime;
    qDebug() << "Accessed Time (readable): " << tsk_fs_time_to_str(tmpfile->meta->atime, buf);
    uint8_t retval = tsk_fs_file_hash_calc(tmpfile, &hashresults, TSK_BASE_HASH_MD5);
    // HERE IS WHERE I NEED TO STORE THE VALUES IN THE EVIDENCEOBJECT.DIRFILEINFOVECTOR
    // possibly, not sure since i'm iterating over the files continuously, i should probably just add straight to the db here.
    // SHOULD BE ABLE TO USE STRAIGHT QTSQL, NO HELPER FUNCTIONS AS DEFINED IN WOMBATDATABASE...
    char sbuf[17];
    int sint = 0;
    for(int i=0; i < 16; i++)
    {
        sint = sprintf(sbuf+(2*i), "%02X", hashresults.md5_digest[i]);
    }
    qDebug() << "MD5 Hash: " << QString(sbuf);

    /*
     *        wombatvarptr->caseobject.dbname = wombatvarptr->caseobject.dirpath + casestring + ".db";
        wombatvarptr->casedb = QSqlDatabase::addDatabase("QSQLITE"); // may not need this
        wombatvarptr->casedb.setDatabaseName(wombatvarptr->caseobject.dbname);
    if(wombatptr->casedb.isOpen())
        qDebug() << "case is open.";
    else
        wombatptr->casedb.open();

     *
     *
     *
     */ 


    return TSK_WALK_CONT;
}
