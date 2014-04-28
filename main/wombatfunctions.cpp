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

bool ProcessingComplete()
{
    bool processingcomplete = false;
    for(int i = 0; i < threadvector.count(); i++)
    {
        processingcomplete = threadvector[i].isFinished();
    }
    
    return processingcomplete;
}

void ProcessFile(TSK_FS_FILE* tmpfile, const char* tmppath, void* tmpptr)
{
    qDebug() << "Active Thread Count: " << threadpool->activeThreadCount();
    /*
    char buf[128];
    TSK_FS_HASH_RESULTS hashresults;
    // TO CALCULATE THE HASH, I CAN LOOP OVER THE INUM AND APPLY THIS FUNCTION SET TO EACH TMPFILE I GET IN A SEPARATE
    // FUNCTION
    //qDebug() << "Accessed Time (readable): " << tsk_fs_time_to_str(tmpfile->meta->atime, buf);
    uint8_t retval = tsk_fs_file_hash_calc(tmpfile, &hashresults, TSK_BASE_HASH_MD5);
    char sbuf[17];
    int sint = 0;
    for(int i=0; i < 16; i++)
    {
        sint = sprintf(sbuf+(2*i), "%02X", hashresults.md5_digest[i]);
    }
    QString tmpstring = QString(sbuf);
    */
    QString tmpstring = "";

    if(fcasedb.isValid() && fcasedb.isOpen())
    {
        QSqlQuery fquery(fcasedb);
        fquery.prepare("INSERT INTO data(objecttype, type, name, parentid, fullpath, atime, ctime, crtime, mtime, size, address, md5) VALUES(5, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?);");
        if(tmpfile->name != NULL)
        {
            fquery.addBindValue((int)tmpfile->name->type);
            fquery.addBindValue(QString(tmpfile->name->name));
            fquery.addBindValue((int)tmpfile->name->par_addr);
        }
        fquery.addBindValue(QString(tmppath));
        if(tmpfile->meta != NULL)
        {
            fquery.addBindValue((int)tmpfile->meta->atime);
            fquery.addBindValue((int)tmpfile->meta->ctime);
            fquery.addBindValue((int)tmpfile->meta->crtime);
            fquery.addBindValue((int)tmpfile->meta->mtime);
            fquery.addBindValue((int)tmpfile->meta->size);
            fquery.addBindValue((int)tmpfile->meta->addr);
        }
        fquery.addBindValue(tmpstring);
        fquery.exec();
        fquery.finish();
    }
    else
    {
        //qDebug() << fcasedb.lastError().text();
    }
}

TSK_WALK_RET_ENUM FileEntries(TSK_FS_FILE* tmpfile, const char* tmppath, void* tmpptr)
{
    QFutureWatcher<void> tmpwatcher;
    QFuture<void> tmpfuture = QtConcurrent::run(ProcessFile, tmpfile, tmppath, tmpptr);
    filewatcher.setFuture(tmpfuture);
    threadvector.append(tmpfuture);

    return TSK_WALK_CONT;
}
