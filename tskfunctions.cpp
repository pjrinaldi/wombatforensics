#include "tskfunctions.h"

TskFunctions::TskFunctions()
{
}

void TskFunctions::SetupTskFramework()
{
    // container for the framework setup
    InitializeFrameworkProperties();
    InitializeFrameworkLog();
    InitializeFrameworkDatabase();
    InitializeFrameworkBlackboard();
    InitializeFrameworkScheduler();

}

void TskFunctions::InitializeFrameworkProperties()
{

}

void TskFunctions::InitializeFrameworkLog()
{
    frameworkLog = std::auto_ptr<Log>(new SqlErrLog());
    ret = frameworkLog->open(QDir(QCoreApplication::applicationDirPath()).absolutePath().toStdString().c_str()); // modify the logdir path
    TskServices::Instance().setLog(*frameworkLog);
}

void TskFunctions::InitializeFrameworkScheduler()
{

}

void TskFunctions::InitializeFrameworkBlackboard()
{

}

void TskFunctions::InitializeFrameworkDatabase()
{
    wombatSqlObject = new SqlWrapper("/data/WombatData.db"); // create overall db.
}
