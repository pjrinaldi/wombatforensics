#ifndef SLEUTHKIT_H 
#define SLEUTHKIT_H

#include <main/interfaces.h>
#include <main/progresswindow.h>
#include <main/wombatvariable.h>
#include <sqlite3.h>
#include <tsk/framework/framework.h>
#include <time.h>
#include "TskSystemPropertiesImpl.h"
#include "TskImgDBSqlite.h"
#include "TskDBBlackboard.h"
#include "TskSchedulerQueue.h"
#include "TskFileManagerImpl.h"
#include "TskImageFileTsk.h"
#include "TskFileTsk.h"
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
//#include <QtConcurrent>
#include <QThreadPool>

class SleuthKitPlugin : public QObject, public SleuthKitInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "wombat.SleuthKitInterface" FILE "sleuthkit.json")
    Q_INTERFACES(SleuthKitInterface)
    
public:
    //SleuthKit Interface Functions
    void SetupSystemProperties(QString settingsPath, QString configFilePath);
    void SetupSystemLog(QString dataPath, QString logFilePath, ProgressWindow* progressWindow, WombatVariable *wombatVariable);
    QString SetupImageDatabase(QString imgDBPath, QString evidenceFilePath);
    void OpenImageDatabase(QString imgDBPath, QString evidenceFilePath);
    void SetupSystemBlackboard();
    void SetupSystemSchedulerQueue();
    void SetupSystemFileManager();
    void OpenEvidence(QString evidencePath, ProgressWindow* progresswindow);
    //void PrepEvidence(QString evidencePath);
    void LogEntry(QString logMsg);
    QStandardItem* GetCurrentImageDirectoryTree(QString imageDbPath, QString imageName);
    QString GetFileContents(int fileID);
    QString GetFileTxtContents(int fileID);
    void RunTask(TskSchedulerQueue::task_struct &task); 

private:
    int currentcaseid;
    int progresscount;
    WombatVariable *sleuthvariable;
    TskSystemPropertiesImpl* systemproperties;
    TskFileManagerImpl* fileManager;
    std::auto_ptr<Log> log;
    std::auto_ptr<TskImgDB> imgdb;
    TskSchedulerQueue scheduler;
    QString evidencepath;
    ProgressWindow* evidenceprogress;

};

class OpenEvidenceRunner : public QObject, public QRunnable
{
    Q_OBJECT

public:
    OpenEvidenceRunner(QString evidencePath);
    void run();

signals:
    void SetFileCount(void);
private:
    QString evidencepath;
    TskImageFileTsk imagefiletsk;
};
/*
class ExtractEvidenceRunner : public QRunnable
{
public:
    ExtractEvidenceRunner();
    void run();
private:

};
*/
#endif // SLEUTHKIT_H
