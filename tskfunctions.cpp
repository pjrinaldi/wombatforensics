#include "tskfunctions.h"

void TskFunctions::SetupTskFramework()
{
    // container for the framework setup
}

void TskFunctions::InitializeFrameworkProperties()
{

}

void TskFunctions::InitializeFrameworkLog()
{

}

void TskFunctions::InitializeFrameworkScheduler()
{

}

void TskFunctions::InitializeFrameworkBlackboard()
{

}

void TskFunctions::InitializeFrameworkDatabase()
{

}

class SqlErrLog : public Log
{
public:
    SqlErrLog() : Log()
    {

    }

    ~SqlErrLog()
    {

    }

    void log(Channel a_channel, const std::string &a_msg)
    {
        Log::log(a_channel, a_msg);
        // put code to write to sql table here...

    }
};
