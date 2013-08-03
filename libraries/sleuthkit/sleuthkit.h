#ifndef SLEUTHKIT_H 
#define SLEUTHKIT_H

#include <main/interfaces.h>
#include <sqlite3.h>
#include <tsk/framework/framework.h>
#include <tsk/framework/services/TskSystemPropertiesImpl.h>
#include <tsk/framework/services/TskImgDBSqlite.h>
#include <QtPlugin>
#include <QObject>
#include <QString>
#include <QStringList>

class SleuthKitPlugin : public QObject, public SleuthKitInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "wombat.SleuthKitInterface" FILE "sleuthkit.json")
    Q_INTERFACES(SleuthKitInterface)
    
public:
    //SleuthKit Interface Functions
    void SetupSystemProperties(QString configFilePath);
    void SetupSystemLog(QString logFilePath);
    void SetupImageDatabase(QString imgDBPath);
    //SetupImageDatabase() const;

    //QStringList evidenceActions() const;
    //QStringList evidenceActionIcons() const;

    //void addEvidence(int currentCaseID);
    //void remEvidence(int currentCaseID);

private:
    int currentcaseid;
    TskSystemPropertiesImpl* systemproperties;
    std::auto_ptr<Log> log;
    std::auto_ptr<TskImgDB> imgdb;

};

#endif // SLEUTHKIT_H
