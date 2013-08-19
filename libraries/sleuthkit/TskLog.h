#ifndef TSKLOG_H
#define TSKLOG_H

#include <time.h>
#include <tsk/framework/services/Log.h>
#include <tsk/framework/framework_i.h>
#include <sqlite3.h>
#include <string>
#include <iostream>
#include <fstream>

//#define LOGERROR(logMsg) TskServices::Instance().getLog().log(TskLog::Error, logMsg)
//#define LOGWARN(logMsg) TskServices::Instance().getLog().log(TskLog::Warn, logMsg)
//#define LOGINFO(logMsg) TskServices::Instance().getLog().log(TskLog::Info, logMsg)

class TskLog : public Log
{
public:
    /** 
     * Defined logging levels.
     */
    /*
    enum Channel {
        Error, ///< Critical error that stops processing
        Warn,  ///< Unexpected results that could be recovered from
        Info    ///< General debugging information
    };
    */

    TskLog();
    virtual ~TskLog();

    //virtual void log(int caseID, int imageID, int analysisType, Channel msgType, const std::wstring &logMsg);
    //virtual void log(int caseID, int imageID, int analysisType, Channel msgType, const std::string &logMsg);
    //virtual void log(Channel msgType, const std::wstring &logMsg);
    void log(Channel msgType, const std::string &logMsg);
    //void dblog();
    //void logError(int caseID, int imageID, int analysisType, const std::wstring &logMsg) { log(caseID, imageID, analysisType, Log::Error, logMsg); };
    //void logWarn(int caseID, int imageID, int analysisType, const std::wstring &logMsg)  { log(caseID, imageID, analysisType, Log::Warn,  logMsg); };
    //void logInfo(int caseID, int imageID, int analysisType, const std::wstring &logMsg)  { log(caseID, imageID, analysisType, Log::Info,  logMsg); };
    //void logError(const std::wstring &logMsg) { log(TskLog::Error, logMsg); };
    //void logWarn(const std::wstring &logMsg) { log(TskLog::Warn, logMsg); };
    //void logInfo(const std::wstring &logMsg) { log(TskLog::Info, logMsg); };
    //int open(const wchar_t * logFileFullPath, const char* dbPath);
    //int open(const char * outDir, const char* dbPath);
    //int open(const char *outDir);
    //int close();
    //const wchar_t * getLogPathW();
    //const char * getLogPath() { return logpath.c_str(); }
    //void SetCaseID(int caseID);
    //void SetImageID(int imageID);
    //void SetAnalysisType(int analysisType);

protected:
    std::string logpath;
    std::string dbpath;
    std::ofstream outstream;
    int caseid;
    int imageid;
    int analysistype;
};
#endif
