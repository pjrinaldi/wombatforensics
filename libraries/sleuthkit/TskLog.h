#ifndef TSKLOG_H
#define TSKLOG_H

#include <time.h>
#include <main/wombatvariable.h>
//#include <main/progresswindow.h>
#include <tsk/framework/services/Log.h>
#include <tsk/framework/framework_i.h>
#include <sqlite3.h>
#include <string>
#include <iostream>
#include <fstream>

class TskLog : public Log
{
public:

    TskLog(WombatVariable logVariable);
    TskLog();
    virtual ~TskLog();

    void log(Channel msgType, const std::string &logMsg);
    void log(Channel msgType, const std::wstring &logMsg);

protected:
    std::string logpath;
    std::string dbpath;
    std::ofstream outstream;
    int caseid;
    int imageid;
    int analysistype;
    WombatVariable logvariable;
};
#endif
