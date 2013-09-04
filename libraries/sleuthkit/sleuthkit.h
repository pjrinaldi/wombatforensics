#ifndef SLEUTHKIT_H 
#define SLEUTHKIT_H

#include <main/interfaces.h>
//#include <main/progresswindow.h>
#include <main/wombatvariable.h>
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

class SleuthKitPlugin : public QObject, public SleuthKitInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "wombat.SleuthKitInterface" FILE "sleuthkit.json")
    Q_INTERFACES(SleuthKitInterface)
    
public:
    //SleuthKit Interface Functions
    Q_INVOKABLE void Initialize(WombatVariable wombatVariable);
    void SetupSystemProperties();
    void SetupLog();
    //void SetupSystemLog(QString dataPath, QString logFilePath, ProgressWindow* progressWindow, WombatVariable *wombatVariable);
    QString SetupImageDatabase(QString imgDBPath, QString evidenceFilePath);
    void OpenImageDatabase(QString imgDBPath, QString evidenceFilePath);
    void SetupSystemBlackboard();
    void SetupSystemSchedulerQueue();
    void SetupSystemFileManager();
    //void OpenEvidence(QString evidencePath, ProgressWindow* progresswindow);
    void PrepEvidence(QString evidencePath);
    void LogEntry(QString logMsg);
    QStandardItem* GetCurrentImageDirectoryTree(QString imageDbPath, QString imageName);
    QString GetFileContents(int fileID);
    QString GetFileTxtContents(int fileID);

private:
    int currentcaseid;
    int progresscount;
    WombatVariable wombatvariable;
    TskSystemPropertiesImpl* systemproperties;
    TskFileManagerImpl* fileManager;
    std::auto_ptr<Log> log;
    std::auto_ptr<TskImgDB> imgdb;
    TskSchedulerQueue scheduler;
    QString evidencepath;
    //ProgressWindow* evidenceprogress;

public slots:
    void threadFinished(void);
    void UpdateProgress(int count, int progress);
};

class OpenEvidenceRunner : public QObject, public QRunnable
{
    Q_OBJECT

public:
    OpenEvidenceRunner(QString evidencePath);
    void run();

signals:
    void Finished(void);
private:
    QString evidencepath;
    TskImageFileTsk imagefiletsk;
};

class ProcessEvidenceRunner : public QObject, public QRunnable
{
    Q_OBJECT

public:
    ProcessEvidenceRunner(int filecount);
    void run();
signals:
    void Finished(void);
    void UpdateStatus(int count, int progress);
private:
    int fileCount;
};

#endif // SLEUTHKIT_H
