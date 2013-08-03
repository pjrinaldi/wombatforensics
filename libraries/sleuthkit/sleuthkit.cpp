#include "sleuthkit.h"

void SleuthKitPlugin::SetupSystemProperties(QString configFilePath)
{
    try
    {
        systemproperties = new TskSystemPropertiesImpl();
        systemproperties->initialize(configFilePath.toStdString());
        TskServices::Instance().setSystemProperties(*systemproperties);
        fprintf(stderr, "Configuration File Loading was successful!\n");
    }
    catch(TskException &ex)
    {
        fprintf(stderr, "Loading Config File config file: ", ex.message().c_str());
    }
}
void SleuthKitPlugin::SetupSystemLog(QString logFilePath)
{
    try
    {
        log = std::auto_ptr<Log>(new Log());
        log->open(logFilePath.toStdString().c_str());
        TskServices::Instance().setLog(*log);
        fprintf(stderr, "Loading Log File was successful!\n");
    }
    catch(TskException &ex)
    {
        fprintf(stderr, "Loading Log File: ", ex.message().c_str());
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
            TskServices::Instance().setImgDB(*imgdb);
        }
        fprintf(stderr, "Loading ImageDB was Successful!\n");
    }
    catch(TskException &ex)
    {
        fprintf(stderr, "Loading ImageDB: ", ex.message().c_str());
    }
}
