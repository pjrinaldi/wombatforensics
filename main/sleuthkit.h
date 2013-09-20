#ifndef SLEUTHKIT_H 
#define SLEUTHKIT_H

#include "wombatvariable.h"
#include "wombatdatabase.h"
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
    Q_INVOKABLE void PopulateCase(WombatVariable wombatVariable);
    Q_INVOKABLE void ShowFile(WombatVariable wombatVariable);
    void SetupSystemBlackboard();
    void GetImageTree(WombatVariable wombatvariable, int isAddEvidence);
    QString GetFileContents(int fileID);
    QString GetFileTxtContents(int fileID);
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
    void LoadFileContents(QString filepath);
    void PopulateProgressWindow(WombatVariable wvariable);
};

class TaskRunner : public QObject, public QRunnable
{
    Q_OBJECT
public:
    TaskRunner(TskSchedulerQueue::task_struct* tmptask, TskPipeline* tmppipe, TskPipelineManager tmpmgr)
    {
        task = tmptask;
        filepipeline = tmppipe;
        pipelinemgr = tmpmgr;
    }
    void run()
    {
        //TskPipelineManager pipelinemgr;
        //TskPipeline* filepipeline;
        /*
        try
        {
            filepipeline = pipelinemgr.createPipeline(TskPipelineManager::FILE_ANALYSIS_PIPELINE);
        }
        catch(const TskException &ex)
        {
            fprintf(stderr, "Error creating file analysis pipeline: %s\n", ex.message().c_str());
        }
        */
        try
        {
            if(task->task == Scheduler::FileAnalysis && filepipeline && !filepipeline->isEmpty())
            {
                filepipeline->run(task->id);
            }
            else
            {
                fprintf(stderr, "Skipping task: %d\n", task->task);
            }
        }
        catch(TskException &ex)
        {
            fprintf(stderr, "TskException: %s\n", ex.message().c_str());
        }
        if(filepipeline && !filepipeline->isEmpty())
        {
            filepipeline->logModuleExecutionTimes();
        }
    }
private:
    TskSchedulerQueue::task_struct* task;
    TskPipelineManager pipelinemgr;
    TskPipeline* filepipeline;
};
/*
class ThreadRunner : public QObject, public QRunnable
{
    Q_OBJECT
public:
    ThreadRunner(QObject* object, QString input, WombatVariable wVariable)
    {
        method = input;
        caller = (SleuthKitPlugin*)object;
        wombatvariable = wVariable;
    };
    void run()
    {
        if(method.compare("initialize") == 0)
            caller->Initialize(wombatvariable);
        if(method.compare("openevidence") == 0)
            caller->OpenEvidence(wombatvariable);
        if(method.compare("populatecase") == 0)
            caller->PopulateCase(wombatvariable);
        if(method.compare("showfile") == 0)
            caller->ShowFile(wombatvariable);
    };
private:
    QString method;
    SleuthKitPlugin* caller;
    WombatVariable wombatvariable;
};*/
#endif // SLEUTHKIT_H
