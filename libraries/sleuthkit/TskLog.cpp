/*
 * The Sleuth Kit
 *
 * Contact: Brian Carrier [carrier <at> sleuthkit [dot] org]
 * Copyright (c) 2010-2012 Basis Technology Corporation. All Rights
 * reserved.
 *
 * This software is distributed under the Common Public License 1.0
 */

#include <string.h>
#include <errno.h>
#include "string.h"
#include "TskLog.h"
#include <tsk/framework/utilities/TskUtilities.h>
#include "sys/stat.h"
#include <time.h>
#include <Poco/FileStream.h>
#include <Poco/Exception.h>
#include <Poco/LineEndingConverter.h>

// @@@ imports for directory creation and deletion
//#include "windows.h"

TskLog::TskLog() : logpath(""), outstream()
{
}

int TskLog::open(const wchar_t * logFileFullPath, const char* dbPath)
{
    return open(TskUtilities::toUTF8(logFileFullPath).c_str(), dbPath);
}

int TskLog::open(const char * logFileFullPath, const char* dbPath)
{
    close(); // if needed

    try {
        outstream.open(logFileFullPath, std::ios::app);
    } catch (const std::exception ex) {
        printf("The file '%s' cannot be opened. Exception: %s\n", logFileFullPath, ex.what());
        return 1;
    }

    logpath.assign(logFileFullPath);
    dbpath.assign(dbPath);

    return 0;
}

/**
 * Close the opened log file.
 * @returns 0 on success
 */
int TskLog::close()
{
    outstream.close();
    if (outstream.bad()) {
        printf("The file '%s' was not closed.", logpath.c_str());
        return 1;
    }
    return 0;
}

TskLog::~TskLog()
{
    close();
}

void TskLog::log(int caseID, int imageID, int analysisType, Channel msgType, const std::string &logMsg)
{
    std::string level;
    switch (msgType) {
    case Error:
        level.assign("[ERROR]");
        break;
    case Warn:
        level.assign("[WARN]");
        break;
    case Info:
        level.assign("[INFO]");
        break;
    }

    struct tm *newtime;
    time_t aclock;

    time(&aclock);   // Get time in seconds
    newtime = localtime(&aclock);   // Convert time to struct tm form 
    char timeStr[64];
    snprintf(timeStr, 64, "%.2d/%.2d/%.2d %.2d:%.2d:%.2d",
        newtime->tm_mon+1,newtime->tm_mday,newtime->tm_year % 100, 
        newtime->tm_hour, newtime->tm_min, newtime->tm_sec);

    if (outstream.good())
    {
        outstream << caseID << "\t" << imageID << "\t" << analysisType << "\t" << level << "\t" << timeStr << "\t" << logMsg << Poco::LineEnding::NEWLINE_DEFAULT;
        outstream.flush();
    }
    else
    {
        fprintf(stderr, "%s %s %s\n", timeStr, level.data(), logMsg.data());
    }
    // db log entry as follows.
    sqlite3* tmpImgDB;
    if(sqlite3_open(dbpath.c_str(), &tmpImgDB) == SQLITE_OK)
    {
        sqlite3_stmt* stmt;
        if(sqlite3_prepare_v2(tmpImgDB, "INSERT INTO log (caseid, imageid, analysistype, msgtype, msgdatetime, logmsg) VALUES(?, ?, ?, ?, ?, ?)", -1, &stmt, 0) == SQLITE_OK)
        {
            if(sqlite3_bind_int(stmt, 1, caseID) == SQLITE_OK)
            {
                if(sqlite3_bind_int(stmt, 2, imageID) == SQLITE_OK)
                {
                    if(sqlite3_bind_int(stmt, 3, analysisType) == SQLITE_OK)
                    {
                        if(sqlite3_bind_int(stmt, 4, msgType) == SQLITE_OK)
                        {
                            if(sqlite3_bind_text(stmt, 5, timeStr, -1, SQLITE_TRANSIENT) == SQLITE_OK)
                            {
                                if(sqlite3_bind_text(stmt, 6, logMsg.c_str(), -1, SQLITE_TRANSIENT) == SQLITE_OK)
                                {
                                    int ret = sqlite3_step(stmt);
                                    if(ret == SQLITE_ROW || ret == SQLITE_DONE)
                                    {
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        else
        {
        }
        sqlite3_finalize(stmt);
    }
    else
    {
    }
    sqlite3_close(tmpImgDB);
    //
    // update or return progress window values (probably return values to wombatforensics, which calls progress window)
}

void TskLog::log(int caseID, int imageID, int analysisType, Channel msgType, const std::wstring &logMsg)
{
    log(caseID, imageID, analysisType, msgType, TskUtilities::toUTF8(logMsg).c_str());
}
/**
 * Return the path to the log file.
 * @returns path to log or NULL if log is going to STDERR
 */
const wchar_t * TskLog::getLogPathW()
{
    return (const wchar_t *)TskUtilities::toUTF16(logpath).c_str();
}
