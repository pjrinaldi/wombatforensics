#ifndef SLEUTHKIT_H 
#define SLEUTHKIT_H

#include "wombatvariable.h"
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
    SleuthKitPlugin() {};
    ~SleuthKitPlugin() {};
    //SleuthKit Interface Functions
    Q_INVOKABLE void Initialize(WombatVariable wombatVariable);
    void SetupSystemProperties();
    void SetupLog();
    void SetupScheduler();
    void SetupFileManager();
    void SetupBlackboard();
    Q_INVOKABLE void OpenEvidence(WombatVariable wombatVariable);
    void SetupSystemBlackboard();
    void LogEntry(QString logMsg);
    QStandardItem* GetCurrentImageDirectoryTree(QString imageDbPath, QString imageName);
    QString GetFileContents(int fileID);
    QString GetFileTxtContents(int fileID);

private:
    WombatVariable wombatvariable;
    TskSystemPropertiesImpl* systemproperties;
    TskFileManagerImpl* fileManager;
    std::auto_ptr<Log> log;
    std::auto_ptr<TskImgDB> imgdb;
    TskSchedulerQueue scheduler;

public slots:
    void threadFinished(void);
signals:
    void UpdateStatus(int filecount, int processcount);
};

#endif // SLEUTHKIT_H
