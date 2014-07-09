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

void LogEntry(int caseid, int evidenceid, int jobid, int type, QString msg)
{
    QSqlQuery logquery(logdb);
    logquery.prepare("INSERT INTO msglog (caseid, evidenceid, jobid, type, datetime, logmsg) VALUES(?, ?, ?, ?, DATETIME('now', 'unixepoch'), ?)");
    logquery.addBindValue(caseid);
    logquery.addBindValue(evidenceid);
    logquery.addBindValue(jobid);
    logquery.addBindValue(type);
    logquery.addBindValue(msg);
    logquery.exec();
}

void StartJob(int type, int caseid, int evidenceid)
{
    QSqlQuery jobquery(logdb);
    jobquery.prepare("INSERT INTO joblog (type, state, filecount, processcount, caseid, evidenceid, start, finish, errorcount) VALUES(?, 0, 0, 0, ?, ?, DATETIME('now', 'unixepoch'), 0, 0)");
    jobquery.addBindValue(type);
    jobquery.addBindValue(caseid);
    jobquery.addBindValue(evidenceid);
    if(jobquery.exec())
        currentjobid = jobquery.lastInsertId().toInt();
}

void EndJob(int jobid, int filecount, int processcount, int errorcount)
{
    QSqlQuery jobquery(logdb);
    jobquery.prepare("UPDATE joblog SET state = 1, filecount = ?, processcount = ?, finish = DATETIME('now', 'unixepoch'), errorcount = ? WHERE jobid = ?");
    jobquery.addBindValue(filecount);
    jobquery.addBindValue(processcount);
    jobquery.addBindValue(errorcount);
    jobquery.addBindValue(jobid);
    jobquery.exec();
}

char* TskTimeToStringUTC(time_t time, char buf[128])
{
    buf[0] = '\0';
    if (time <= 0) {
        strncpy(buf, "", 128);
    }
    else {
        //struct tm *tmTime = localtime(&time);
        struct tm *tmTime = gmtime(&time);

        snprintf(buf, 128, "%.4d-%.2d-%.2d %.2d:%.2d:%.2d", (int) tmTime->tm_year + 1900, (int) tmTime->tm_mon + 1, (int) tmTime->tm_mday, tmTime->tm_hour, (int) tmTime->tm_min, (int) tmTime->tm_sec);//, tzname[(tmTime->tm_isdst == 0) ? 0 : 1]);
    }
    return buf;
}

int GetChildCount(int type, int address, int parimgid)
{
    QSqlQuery childquery(fcasedb);
    QString querystring = "SELECT COUNT(objectid) FROM data WHERE parentid = ?";
    if(type < 4)
        querystring += " AND objecttype < 5";
    else
        querystring += " AND objecttype = 5";
    if(type != 1)
        querystring += " AND parimgid = ?";
    childquery.prepare(querystring);
    childquery.addBindValue(address);
    if(type != 1)
        childquery.addBindValue(parimgid);
    if(childquery.exec())
    {
        childquery.next();
        return childquery.value(0).toInt();
    }
    return 0;
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
    if(threadvector.count() == 0)
        return true;
    for(int i = 0; i < threadvector.count(); i++)
    {
        processingcomplete = threadvector[i].isFinished();
    }
    
    return processingcomplete;
}

void ProcessFile(QVector<QString> tmpstrings, QVector<int> tmpints)
{

    if(fcasedb.isValid() && fcasedb.isOpen())
    {
        QSqlQuery fquery(fcasedb);
        fquery.prepare("INSERT INTO data(objecttype, type, name, parentid, fullpath, atime, ctime, crtime, mtime, size, address, md5, parimgid, parfsid) VALUES(5, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?);");
        fquery.addBindValue(tmpints[0]);
        fquery.addBindValue(tmpstrings[0]);
        fquery.addBindValue(tmpints[1]);
        fquery.addBindValue(tmpstrings[1]);
        fquery.addBindValue(tmpints[2]);
        fquery.addBindValue(tmpints[3]);
        fquery.addBindValue(tmpints[4]);
        fquery.addBindValue(tmpints[5]);
        fquery.addBindValue(tmpints[6]);
        fquery.addBindValue(tmpints[7]);
        fquery.addBindValue(tmpstrings[2]);
        fquery.addBindValue(currentevidenceid);
        fquery.addBindValue(tmpints[8]);
        
        fquery.exec();
        fquery.finish();
        filesprocessed++;
        isignals->ProgUpd();
        
    }
    else
    {
        LogEntry(0, currentevidenceid, currentjobid, 0, QString("Error while processing " + tmpstrings[1] + " " + fcasedb.lastError().text()));
        errorcount++;
    }
}

TSK_WALK_RET_ENUM FileEntries(TSK_FS_FILE* tmpfile, const char* tmppath, void* tmpptr)
{
    if(tmpptr != NULL)
        LogEntry(0, 0, currentjobid, 2, "TmpPtr got a value somehow");
        qDebug() << "Not sure how tmpptr got set.";
    TSK_FS_HASH_RESULTS hashresults;
    uint8_t retval = tsk_fs_file_hash_calc(tmpfile, &hashresults, TSK_BASE_HASH_MD5);
    QString tmpstring;
    if(retval == 0)
    {
        char sbuf[17];
        int sint = 0;
        for(int i=0; i < 16; i++)
        {
            sint = sprintf(sbuf+(2*i), "%02X", hashresults.md5_digest[i]);
        }
        if(sint > 0)
            tmpstring = QString(sbuf);
    }
    else
        tmpstring = QString("");

    QVector<QString> filestrings;
    if(tmpfile->name != NULL) filestrings.append(QString(tmpfile->name->name));
    else filestrings.append(QString("unknown.wbt"));
    filestrings.append(QString("/") + QString(tmppath));
    filestrings.append(tmpstring);

    QVector<int> fileints;

    if(tmpfile->name != NULL)
    {
        fileints.append((int)tmpfile->name->type);
        fileints.append((int)tmpfile->name->par_addr);
    }
    else
    {
        fileints.append(0);
        fileints.append(0);
    }
    if(tmpfile->meta != NULL)
    {
        fileints.append((int)tmpfile->meta->atime);
        fileints.append((int)tmpfile->meta->ctime);
        fileints.append((int)tmpfile->meta->crtime);
        fileints.append((int)tmpfile->meta->mtime);
        fileints.append((int)tmpfile->meta->size);
        fileints.append((int)tmpfile->meta->addr);
    }
    else
    {
        fileints.append(0);
        fileints.append(0);
        fileints.append(0);
        fileints.append(0);
        fileints.append(0);
        fileints.append(0);
    }
    fileints.append(currentfilesystemid);

    QFuture<void> tmpfuture = QtConcurrent::run(ProcessFile, filestrings, fileints);
    filewatcher.setFuture(tmpfuture);
    threadvector.append(tmpfuture);
    filesfound++;

    return TSK_WALK_CONT;
}
