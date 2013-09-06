#ifndef TSKLOG_H
#define TSKLOG_H

#include <time.h>
#include "wombatvariable.h"
#include <tsk/framework/services/Log.h>
#include <tsk/framework/framework_i.h>
#include <sqlite3.h>
#include <string>
#include <iostream>
#include <fstream>
#include <QObject>

class TskLog : public QObject, public Log
{
    Q_OBJECT
public:

    //TskLog(WombatVariable logVariable);
    TskLog() {};
    virtual ~TskLog() 
    {
        Log::close();
    };

    void log(Channel msgType, const std::string &logMsg);
    void log(Channel msgType, const std::wstring &logMsg);
public slots:
    void LogVariable(WombatVariable wombatVariable)
    {
        logvariable = wombatVariable;
        fprintf(stderr, "Log got the variable and it has a caseid of: %d\n", logvariable.caseid);
    };
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
