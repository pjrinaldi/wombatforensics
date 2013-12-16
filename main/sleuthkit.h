#ifndef SLEUTHKIT_H 
#define SLEUTHKIT_H

#include "wombatvariable.h"
#include "wombatdatabase.h"
#include <sqlite3.h>
#include "../sleuthkit/framework/tsk/framework/framework.h" // provides tsk framework
#include <time.h>
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
#include <QtConcurrent>
#include <fstream>
#include <cstdio>

class SleuthKitPlugin : public QObject
{
    Q_OBJECT
    
public:
    SleuthKitPlugin(WombatDatabase* wombatcasedata);
    ~SleuthKitPlugin() {};
    //SleuthKit Interface Functions
    Q_INVOKABLE void Initialize(WombatVariable wombatVariable);
    void SetupSystemProperties();
    void SetupLog();
    void SetupScheduler();
    void SetupFileManager();
    void SetupBlackboard();
    void SetupImageDatabase();
    void SetEvidenceDB(WombatVariable wombatVariable);
    Q_INVOKABLE void OpenEvidence(WombatVariable wombatVariable);
    Q_INVOKABLE void RefreshTreeViews(WombatVariable wombatVariable);
    Q_INVOKABLE void PopulateCase(WombatVariable wombatVariable);
    Q_INVOKABLE void ShowFile(WombatVariable wombatVariable);
    Q_INVOKABLE void ExportFiles(WombatVariable wombatVariable);
    void SetupSystemBlackboard();
    void GetImageTree(WombatVariable wombatvariable, int isAddEvidence);
    QString GetFileContents(int fileID);
    QString GetVolumeFilePath(WombatVariable wombatVariable, int volID);
    QStringList GetVolumeContents(WombatVariable wombatVariable);
    void ExportFile(std::string tmppath, int objectID);
    void TaskMap(TskSchedulerQueue::task_struct* &task);

private:
    WombatVariable wombatvariable;
    WombatDatabase* wombatdata;
    TskSystemPropertiesImpl* systemproperties;
    TskFileManagerImpl* fileManager;
    TskLog* log;
    TskImgDBSqlite* initialdb;
    TskImgDBSqlite* imgdb;
    TskImgDBSqlite* tmpdb;
    TskSchedulerQueue scheduler;
    void FinishExport(int processcount);

public slots:
    void threadFinished(void);
    void GetLogVariable(WombatVariable &wvariable)
    {
        emit SetLogVariable(&wvariable);
    };
signals:
    void UpdateStatus(int filecount, int processcount);
    void ReturnImageNode(QStandardItem* imagenode);
    void UpdateMessageTable(void);
    void SetLogVariable(WombatVariable* wvariable);
    void LoadFileContents(QString filepath);
    void PopulateProgressWindow(WombatVariable wvariable);
};

class TaskRunner : public QObject, public QRunnable
{
    Q_OBJECT
public:
    TaskRunner(TskSchedulerQueue::task_struct* tmptask, TskPipeline* tmppipe)
    {
        task = tmptask;
        filepipeline = tmppipe;
    }
    void run()
    {
        try
        {
            fprintf(stderr, "task id: %d\n", task->id);
            filepipeline->run(task->id);
        }
        catch(TskException &ex)
        {
            fprintf(stderr, "TskException: %s\n", ex.message().c_str());
        }
    }
private:
    TskSchedulerQueue::task_struct* task;
    TskPipeline* filepipeline;
};
#endif // SLEUTHKIT_H
