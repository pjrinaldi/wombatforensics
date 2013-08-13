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

TskLog::TskLog() : logPath(""), outStream()
{
}

int TskLog::open(const wchar_t * logFileFullPath)
{
    return open(TskUtilities::toUTF8(logFileFullPath).c_str());
}

int TskLog::open(const char * logFileFullPath)
{
    close(); // if needed

    try {
        outStream.open(logFileFullPath, std::ios::app);
    } catch (const std::exception ex) {
        printf("The file '%s' cannot be opened. Exception: %s\n", logFileFullPath, ex.what());
        return 1;
    }

    logPath.assign(logFileFullPath);

    return 0;
}

/**
 * Close the opened log file.
 * @returns 0 on success
 */
int TskLog::close()
{
    outStream.close();
    if (outStream.bad()) {
        printf("The file '%s' was not closed.", logPath.c_str());
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

    if (outStream.good())
    {
        outStream << caseID << "\t" << imageID << "\t" << analysisType << "\t" << level << "\t" << timeStr << "\t" << logMsg << Poco::LineEnding::NEWLINE_DEFAULT;
        outStream.flush();
    }
    else
    {
        fprintf(stderr, "%s %s %s\n", timeStr, level.data(), logMsg.data());
    }
    // db log entry as follows.
    // update or return progress window values
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
    return (const wchar_t *)TskUtilities::toUTF16(logPath).c_str();
}
