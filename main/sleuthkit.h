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
    void SetEvidenceDB(WombatVariable wombatVariable);
    Q_INVOKABLE void OpenEvidence(WombatVariable wombatVariable);
    Q_INVOKABLE void PopulateCase(WombatVariable wombatVariable);
    Q_INVOKABLE void ShowFile(WombatVariable wombatVariable);
    void SetupSystemBlackboard();
    void GetImageTree(WombatVariable wombatvariable, int isAddEvidence);
    QString GetFileContents(int fileID);
    QString GetFileTxtContents(int fileID);

private:
    WombatVariable wombatvariable;
    WombatCaseDb* wombatdata;
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
};

#endif // SLEUTHKIT_H
