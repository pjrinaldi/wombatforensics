#include "sleuthkit.h"

void SleuthKitPlugin::SetupSystemProperties(QString settingsPath, QString configFilePath)
{
    QString tmpPath = settingsPath;
    tmpPath += configFilePath;
    try
    {
        systemproperties = new TskSystemPropertiesImpl();
        systemproperties->initialize(tmpPath.toStdString());
        TskServices::Instance().setSystemProperties(*systemproperties);
        fprintf(stderr, "Configuration File Loading was successful!\n");
    }
    catch(TskException &ex)
    {
        fprintf(stderr, "Loading Config File config file: %s\n", ex.message().c_str());
    }
    try
    {
        SetSystemProperty(TskSystemProperties::OUT_DIR, settingsPath.toStdString());
    }
    catch(TskException &ex)
    {
        fprintf(stderr, "Setting out dir failed: %s\n", ex.message().c_str());
    }
}
void SleuthKitPlugin::SetupSystemLog(QString dataPath, QString logFilePath)
{
    QString tmpPath = dataPath;
    tmpPath += logFilePath;
    try
    {
        log = std::auto_ptr<Log>(new Log());
        log->open(logFilePath.toStdString().c_str());
        TskServices::Instance().setLog(*log);
        fprintf(stderr, "Loading Log File was successful!\n");
    }
    catch(TskException &ex)
    {
        fprintf(stderr, "Loading Log File: %s\n", ex.message().c_str());
    }
}
void SleuthKitPlugin::SetupImageDatabase(QString imgDBPath)
{
    try
    {
        imgdb = std::auto_ptr<TskImgDB>(new TskImgDBSqlite(imgDBPath.toStdString().c_str()));
        if(imgdb->initialize() != 0)
            fprintf(stderr, "Error initializing db\n");
        else
        {
            fprintf(stderr, "DB was Initialized Successfully!\n");
        }
        TskServices::Instance().setImgDB(*imgdb);
        fprintf(stderr, "Loading ImageDB was Successful!\n");
    }
    catch(TskException &ex)
    {
        fprintf(stderr, "Loading ImageDB: %s\n", ex.message().c_str());
    }
}
void SleuthKitPlugin::SetupSystemBlackboard()
{
    try
    {
        TskServices::Instance().setBlackboard((TskBlackboard &)TskDBBlackboard::instance());
        fprintf(stderr, "Loading Blackboard was successful!\n");
    }
    catch(TskException &ex)
    {
        fprintf(stderr, "Loading Blackboard: %s\n", ex.message().c_str());
    }
}
void SleuthKitPlugin::SetupSystemSchedulerQueue()
{
    try
    {
        TskServices::Instance().setScheduler(scheduler);
        fprintf(stderr, "Loading Scheduler was successful!\n");
    }
    catch(TskException &ex)
    {
        fprintf(stderr, "Loading Scheduler: %s\n", ex.message().c_str());
    }
}
void SleuthKitPlugin::SetupSystemFileManager()
{
    try
    {
        TskServices::Instance().setFileManager(TskFileManagerImpl::instance());
        fprintf(stderr, "Loading File Manager was successful!\n");
    }
    catch(TskException &ex)
    {
        fprintf(stderr, "Loading FileManager: %s\n", ex.message().c_str());
    }
}
void SleuthKitPlugin::OpenEvidence(QString evidencePath)
{
    TskImageFileTsk imagefiletsk;
    try
    {
        imagefiletsk.open(evidencePath.toStdString());
        TskServices::Instance().setImageFile(imagefiletsk);
        fprintf(stderr, "Opening Image File was successful!\n");
    }
    catch(TskException &ex)
    {
        fprintf(stderr, "Opening Evidence: %s\n", ex.message().c_str());
    }
    try
    {
        imagefiletsk.extractFiles();
        fprintf(stderr, "Extracting Evidence was successful\n");
    }
    catch(TskException &ex)
    {
        fprintf(stderr, "Extracting Evidence: %s\n", ex.message().c_str());
    }
}
