#ifndef TSKLOG_H
#define TSKLOG_H

#include <time.h>
#include <tsk/framework/services/Log.h>
#include <tsk/framework/framework_i.h>
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
 * Macro that gets the log service and writes an error message in a
 * single statement. 
 * @param msg Message to log
 * @returns void
 */
//#define LOGERROR(logMsg) TskServices::Instance().getLog().log(TskLog::Error, logMsg)
/*
 * Macro that gets the log service and writes a warning message in a
 * single statement. 
 * @param msg Message to log
 * @returns void
 */
//#define LOGWARN(logMsg) TskServices::Instance().getLog().log(TskLog::Warn, logMsg)

/**
 * Macro that gets the log service and writes an info message in a
 * single statement. 
 * @param msg Message to log
 * @returns void
 */
//#define LOGINFO(logMsg) TskServices::Instance().getLog().log(TskLog::Info, logMsg)

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
class TskLog : public Log
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

    TskLog();
    virtual ~TskLog();

    virtual void log(int caseID, int imageID, int analysisType, Channel msgType, const std::wstring &logMsg);
    virtual void log(int caseID, int imageID, int analysisType, Channel msgType, const std::string &logMsg);
    virtual void log(Channel msgType, const std::wstring &logMsg);
    virtual void log(Channel msgType, const std::string &logMsg);
    void logError(int caseID, int imageID, int analysisType, const std::wstring &logMsg) { log(caseID, imageID, analysisType, TskLog::Error, logMsg); };
    void logWarn(int caseID, int imageID, int analysisType, const std::wstring &logMsg)  { log(caseID, imageID, analysisType, TskLog::Warn,  logMsg); };
    void logInfo(int caseID, int imageID, int analysisType, const std::wstring &logMsg)  { log(caseID, imageID, analysisType, TskLog::Info,  logMsg); };
    void logError(const std::wstring &logMsg) { log(TskLog::Error, logMsg); };
    void logWarn(const std::wstring &logMsg) { log(TskLog::Warn, logMsg); };
    void logInfo(const std::wstring &logMsg) { log(TskLog::Info, logMsg); };
    int open(const wchar_t * logFileFullPath, const char* dbPath);
    int open(const char * outDir, const char* dbPath);
    int close();
    const wchar_t * getLogPathW();
    const char * getLogPath() { return logpath.c_str(); }
    void SetCaseID(int caseID);
    void SetImageID(int imageID);
    void SetAnalysisType(int analysisType);

protected:
    std::string logpath;
    std::string dbpath;
    std::ofstream outstream;
    int caseid;
    int imageid;
    int analysistype;
};
#endif
