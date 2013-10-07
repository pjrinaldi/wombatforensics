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
#include "../utilities/TskUtilities.h"
#include "sys/stat.h"
#include <time.h>
#include <Poco/FileStream.h>
#include <Poco/Exception.h>
#include <Poco/LineEndingConverter.h>

// @@@ imports for directory creation and deletion
//#include "windows.h"

void TskLog::log(Channel msgType, const std::wstring &logMsg)
{
    log(msgType, TskUtilities::toUTF8(logMsg).c_str());
}

void TskLog::log(Channel msgType, const std::string &logMsg)
{
    //dbpath = logvariable.datapath.toStdString();
    //dbpath.append("WombatLog.db");
    fprintf(stderr, "DBPath: %s\n", dbpath.c_str());
    //std::string dbpath = "/home/pasquale/WombatForensics/data/WombatLog.db";
    Log::log(msgType, logMsg);

    struct tm *newtime;
    time_t aclock;

    time(&aclock);   // Get time in seconds
    newtime = localtime(&aclock);   // Convert time to struct tm form 
    char timeStr[64];
    snprintf(timeStr, 64, "%.2d/%.2d/%.2d %.2d:%.2d:%.2d",
        newtime->tm_mon+1,newtime->tm_mday,newtime->tm_year % 100, 
        newtime->tm_hour, newtime->tm_min, newtime->tm_sec);

    // db log entry as follows.
    sqlite3* tmpImgDB;
    if(sqlite3_open(dbpath.c_str(), &tmpImgDB) == SQLITE_OK)
    {
        sqlite3_stmt* stmt;
        if(sqlite3_prepare_v2(tmpImgDB, "INSERT INTO log (caseid, evidenceid, jobid, msgtype, msgdatetime, msg) VALUES(?, ?, ?, ?, ?, ?);", -1, &stmt, 0) == SQLITE_OK)
        {
            if(sqlite3_bind_int(stmt, 1, logvariable.caseid) == SQLITE_OK && sqlite3_bind_int(stmt, 2, logvariable.evidenceid) == SQLITE_OK && sqlite3_bind_int(stmt, 3, logvariable.jobid) == SQLITE_OK && sqlite3_bind_int(stmt, 4, msgType) == SQLITE_OK && sqlite3_bind_text(stmt, 5, timeStr, -1, SQLITE_TRANSIENT) == SQLITE_OK && sqlite3_bind_text(stmt, 6, logMsg.c_str(), -1, SQLITE_TRANSIENT) == SQLITE_OK)
            {
                int ret = sqlite3_step(stmt);
                if(ret == SQLITE_ROW || ret == SQLITE_DONE)
                {
                }
                else
                {
                    fprintf(stderr, "Error: %s\n", sqlite3_errmsg(tmpImgDB));
                }
            }
            else
                fprintf(stderr, "Error: %s\n", sqlite3_errmsg(tmpImgDB));
        }
        else
        {
            fprintf(stderr, "Error: %s\n", sqlite3_errmsg(tmpImgDB));
        }
        sqlite3_finalize(stmt);
    }
    else
    {
        fprintf(stderr, "Error: %s\n", sqlite3_errmsg(tmpImgDB));
    }
    sqlite3_close(tmpImgDB);
}
