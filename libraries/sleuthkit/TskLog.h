#ifndef TSKLOG_H
#define TSKLOG_H

#include "tsk/framework/services/Log.h"
#include <time.h>
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
#define LOGERROR(msg) TskServices::Instance().getLog().log(Log::Error, msg)

/**
 * Macro that gets the log service and writes a warning message in a
 * single statement. 
 * @param msg Message to log
 * @returns void
 */
#define LOGWARN(msg) TskServices::Instance().getLog().log(Log::Warn, msg)


/**
 * Macro that gets the log service and writes an info message in a
 * single statement. 
 * @param msg Message to log
 * @returns void
 */
#define LOGINFO(msg) TskServices::Instance().getLog().log(Log::Info, msg)


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
    void logError(int caseID, int imageID, int analysisType, const std::wstring &logMsg) { log(caseID, imageID, analysisType, TskLog::Error, logMsg); };
    void logWarn(const std::wstring &msg)  { log(Log::Warn,  msg); };
    void logInfo(const std::wstring &msg)  { log(Log::Info,  msg); };
    int open(const wchar_t * a_logFileFullPath);
    int open(const char * a_outDir);
    int open();
    int close();
    const wchar_t * getLogPathW();
    const char * getLogPath() { return m_filePath.c_str(); }

protected:
    std::string m_filePath;
    std::ofstream m_outStream;
};
#endif
