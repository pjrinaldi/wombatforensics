#ifndef SLEUTHKIT_H 
#define SLEUTHKIT_H

#include <main/interfaces.h>
#include <main/progresswindow.h>
#include <sqlite3.h>
#include "framework.h"
//#include <tsk/framework/framework.h>
#include <time.h>
#include "TskSystemPropertiesImpl.h"
#include "TskImgDBSqlite.h"
#include "TskDBBlackboard.h"
#include "TskSchedulerQueue.h"
#include "TskFileManagerImpl.h"
#include "TskImageFileTsk.h"
#include "TskFileTsk.h"
#include "TskServices.h"
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

/**
 * Macro that gets the log service and writes an error message in a
 * single statement. 
 * @param msg Message to log
 * @returns void
 */
#define LOGERROR(caseID, imageID, analysisType, logMsg) TskServices::Instance.getLog().logError(caseID, imageID, analysisType, TskLog::Error, logMsg)

/**
 * Macro that gets the log service and writes a warning message in a
 * single statement. 
 * @param msg Message to log
 * @returns void
 */
#define LOGWARN(caseID, imageID, analysisType, logMsg) TskServices::Instance.getLog().logWarn(caseID, imageID, analysisType, TskLog::Warn, logMsg)

/**
 * Macro that gets the log service and writes an info message in a
 * single statement. 
 * @param msg Message to log
 * @returns void
 */
#define LOGINFO(caseID, imageID, analysisType, logMsg) TskServices::Instance.getLog().logError(caseID, imageID, analysisType, TskLog::Info, logMsg)

class SleuthKitPlugin : public QObject, public SleuthKitInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "wombat.SleuthKitInterface" FILE "sleuthkit.json")
    Q_INTERFACES(SleuthKitInterface)
    
public:
    //SleuthKit Interface Functions
    void SetupSystemProperties(QString settingsPath, QString configFilePath);
    void SetupSystemLog(QString dataPath, QString logFilePath);
    QString SetupImageDatabase(QString imgDBPath, QString evidenceFilePath);
    void OpenImageDatabase(QString imgDBPath, QString evidenceFilePath);
    void SetupSystemBlackboard();
    void SetupSystemSchedulerQueue();
    void SetupSystemFileManager();
    void OpenEvidence(QString evidencePath, ProgressWindow* progresswindow);
    QStandardItem* GetCurrentImageDirectoryTree(QString imageDbPath, QString imageName);
    QString GetFileContents(int fileID);
    QString GetFileTxtContents(int fileID);

private:
    int currentcaseid;
    TskSystemPropertiesImpl* systemproperties;
    TskFileManagerImpl* fileManager;
    TskLog *log;
    std::auto_ptr<TskImgDB> imgdb;
    TskSchedulerQueue scheduler;
};

#endif // SLEUTHKIT_H
