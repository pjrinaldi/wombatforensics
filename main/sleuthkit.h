#ifndef SLEUTHKIT_H 
#define SLEUTHKIT_H

#include "wombatvariable.h"
#include "wombatcasedb.h"
#include "basictools.h"
#include <sqlite3.h>
#include <tsk/framework/framework.h>
#include <time.h>
#include "TskSystemPropertiesImpl.h"
#include "TskImgDBSqlite.h"
#include "TskDBBlackboard.h"
#include "TskSchedulerQueue.h"
#include <tsk/framework/file/TskFileManagerImpl.h>
#include "TskImageFileTsk.h"
#include <tsk/framework/file/TskFileTsk.h>
#include "TskLog.h"
//#include "TskServicesImpl.h"
#include <QtPlugin>
#include <QObject>
#include <QString>
#include <string>
#include <QStringList>
#include <QFile>
#include <QXmlStreamWriter>
#include <QStandardItemModel>
#include <QByteArray>
#include <QDataStream>
#include <QThreadPool>

class SleuthKitPlugin : public QObject
{
    Q_OBJECT
    
public:
    SleuthKitPlugin(WombatCaseDb* wombatcasedata);
    ~SleuthKitPlugin() {};
    //SleuthKit Interface Functions
    Q_INVOKABLE void Initialize(WombatVariable wombatVariable);
    void SetupSystemProperties();
    void SetupLog();
    void SetupScheduler();
    void SetupFileManager();
    void SetupBlackboard();
    void SetupImageDatabase();
    Q_INVOKABLE void OpenEvidence(WombatVariable wombatVariable);
    void SetupSystemBlackboard();
    //void LogEntry(QString logMsg);
    void GetImageTree(WombatVariable wombatvariable);
    QString GetFileContents(int fileID);
    QString GetFileTxtContents(int fileID);

private:
    WombatVariable wombatvariable;
    WombatCaseDb* wombatdata;
    TskSystemPropertiesImpl* systemproperties;
    TskFileManagerImpl* fileManager;
    TskLog* log;
    //std::auto_ptr<TskImgDB> imgdb;
    //std::auto_ptr<TskImgDB> initialdb;
    TskImgDBSqlite* initialdb;
    TskImgDBSqlite* imgdb;
    TskImgDBSqlite* tmpdb;
    TskSchedulerQueue scheduler;

public slots:
    void threadFinished(void);
    void GetLogVariable(WombatVariable wvariable)
    {
        emit SetLogVariable(wvariable);
    };
signals:
    void UpdateStatus(int filecount, int processcount);
    void ReturnImageNode(QStandardItem* imagenode);
    void UpdateMessageTable(void);
    void SetLogVariable(WombatVariable wvariable);
};

#endif // SLEUTHKIT_H
