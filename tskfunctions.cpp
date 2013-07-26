#include "tskfunctions.h"

TskFunctions::TskFunctions()
{
}

void TskFunctions::SetupTskFramework()
{
    // container for the framework setup
    InitializeFrameworkProperties();
    InitializeFrameworkDatabase();
    InitializeFrameworkLog();
    //InitializeFrameworkBlackboard();
    //InitializeFrameworkScheduler();
    //InitializeFrameworkFileManager();

}

void TskFunctions::InitializeFrameworkProperties()
{
    try
    {
        TskSystemPropertiesImpl *systemProperties = new TskSystemPropertiesImpl();
        systemProperties->initialize();
        QString tmpString = QDir(QCoreApplication::applicationDirPath()).absolutePath();
        QDir appPath = QDir(QCoreApplication::applicationDirPath());
        appPath.mkdir("tmpdata");
        tmpString += "/tmpdata";
        systemProperties->set(TskSystemProperties::OUT_DIR, tmpString.toStdString());
        TskServices::Instance().setSystemProperties(*systemProperties);
    }
    catch(TskException &ex)
    {
        LOGERROR(ex.message().c_str());
    }
}

void TskFunctions::InitializeFrameworkLog()
{
    frameworkLog = std::auto_ptr<Log>(new SqlErrLog());
    QString tmpPath = QDir(QCoreApplication::applicationDirPath()).absolutePath();
    tmpPath += "/data/";
    tmpPath += "wombatforensics.log";
    ret = frameworkLog->open(tmpPath.toStdString().c_str()); // modify the logdir path
    TskServices::Instance().setLog(*frameworkLog);
}

void TskFunctions::InitializeFrameworkScheduler()
{
    TskSchedulerQueue scheduler;
    TskServices::Instance().setScheduler(scheduler);
}

void TskFunctions::InitializeFrameworkBlackboard()
{
    TskServices::Instance().setBlackboard((TskBlackboard &) TskDBBlackboard::instance());
}

void TskFunctions::InitializeFrameworkDatabase()
{
    wombatSqlObject = new SqlWrapper("WombatData.db"); // create overall db.
}
void TskFunctions::InitializeFrameworkFileManager()
{
    TskServices::Instance().setFileManager(TskFileManagerImpl::instance());
}
