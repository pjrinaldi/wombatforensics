#include "tskfunctions.h"

TskFunctions::TskFunctions()
{
}

void TskFunctions::SetupTskFramework()
{
    //maininstance = mainInstance;
    // container for the framework setup
    InitializeFrameworkProperties();
    InitializeFrameworkDatabase();
    InitializeFrameworkLog();
    //InitializeFrameworkBlackboard();
    InitializeFrameworkScheduler();
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
        //maininstance.setSystemProperties(*systemProperties);
    }
    catch(TskException &ex)
    {
        //LOGERROR(ex.message().c_str());
    }
}

void TskFunctions::InitializeFrameworkLog()
{
    frameworkLog = std::auto_ptr<Log>(new Log());
    //frameworkLog = std::auto_ptr<Log>(new SqlErrLog());
    //frameworkLog = new SqlErrLog();
    QString tmpPath = QDir(QCoreApplication::applicationDirPath()).absolutePath();
    tmpPath += "/data/";
    tmpPath += "wombatforensics.log";
    ret = frameworkLog->open(tmpPath.toStdString().c_str()); // modify the logdir path
    TskServices::Instance().setLog(*frameworkLog);
    //maininstance.setLog(*frameworkLog);
}

void TskFunctions::InitializeFrameworkScheduler()
{
    TskSchedulerQueue scheduler;
    TskServices::Instance().setScheduler(scheduler);
    //maininstance.setScheduler(scheduler);
}

void TskFunctions::InitializeFrameworkBlackboard()
{
    //maininstance.setBlackboard((TskBlackboard &) TskDBBlackboard::instance());
    TskServices::Instance().setBlackboard((TskBlackboard &) TskDBBlackboard::instance());
}

void TskFunctions::InitializeFrameworkDatabase()
{
    //wombatSqlObject = new SqlWrapper("WombatData.db"); // create overall db.
    //wombatSqlObject->SetErrorLog(frameworkLog);
}
void TskFunctions::InitializeFrameworkFileManager()
{
    //maininstance.setFileManager(TskFileManagerImpl::instance());
    TskServices::Instance().setFileManager(TskFileManagerImpl::instance());
}
