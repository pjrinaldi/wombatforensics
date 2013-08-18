#ifndef TSKLOG_H
#define TSKLOG_H

//#include "tsk/framework/services/Log.h"
#include <time.h>
#include <sqlite3.h>
#include <string>
#include <iostream>
#include <fstream>

/**
 * \file Log.h
 * Interface and default logging infrastructure that enables applications and framework
 * to log to a single place.  
 */

/**
 * Logging class to enable the framework, apps that use it, and modules to
 * log error and warning messages.  The default implementation writes
 * the log messages to a file if open() was called or prints the messages to
 * stderr if open() was never called. The class can be extended 
 * if you want logs to be saved in another way.
 * Can be registered with and retrieved from TskServices.
 *
 * Developers can either directly call the log() method with the logging
 * level, can call the logError(), etc. methods on the class or use the
 * LOGERROR() etc. macros, which will also get the Log service from TskServices.
 */
class TskLog
{
public:
    /** 
     * Defined logging levels.
     */
    enum Channel {
        Error, ///< Critical error that stops processing
        Warn,  ///< Unexpected results that could be recovered from
        Info    ///< General debugging information
    };

    TskLog(int caseID, int imageID, int analysisType);
    virtual ~TskLog();

    void log(int caseID, int imageID, int analysisType, Channel msgType, const std::wstring &logMsg);
    void log(int caseID, int imageID, int analysisType, Channel msgType, const std::string &logMsg);
    void log(Channel msgType, const std::wstring &logMsg);
    void log(Channel msgType, const std::string &logMsg);
    void logError(int caseID, int imageID, int analysisType, const std::wstring &logMsg) { log(caseID, imageID, analysisType, TskLog::Error, logMsg); };
    void logWarn(int caseID, int imageID, int analysisType, const std::wstring &logMsg)  { log(caseID, imageID, analysisType, TskLog::Warn,  logMsg); };
    void logInfo(int caseID, int imageID, int analysisType, const std::wstring &logMsg)  { log(caseID, imageID, analysisType, TskLog::Info,  logMsg); };
    void logError(const std::wstring &logMsg) { log(TskLog::Error, logMsg); };
    void logWarn(const std::wstring &logMsg) { log(Tsk::Warn, logMsg); };
    void logInfo(const std::wstring &logMsg) { log(Tsk::Info, logMsg); };
    int open(const wchar_t * logFileFullPath, const char* dbPath);
    int open(const char * outDir, const char* dbPath);
    int close();
    const wchar_t * getLogPathW();
    const char * getLogPath() { return logpath.c_str(); }

protected:
    std::string logpath;
    std::string dbpath;
    std::ofstream outstream;
    int caseid;
    int imageid;
    int analysistype;
};
#endif
