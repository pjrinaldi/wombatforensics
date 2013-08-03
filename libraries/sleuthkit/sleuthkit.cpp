#include "sleuthkit.h"

void SleuthKitPlugin::SetupSystemProperties(QString configFilePath)
{
    try
    {
        systemproperties = new TskSystemPropertiesImpl();
        systemproperties->initialize(configFilePath.toStdString());
        TskServices::Instance().setSystemProperties(*systemproperties);
        fprintf(stderr, "Configuration File Loading was successful!");
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
        fprintf(stderr, "Loading Log File was successful!");
    }
    catch(TskException &ex)
    {
        fprintf(stderr, "Loading Log File: ", ex.message().c_str());
    }
}

