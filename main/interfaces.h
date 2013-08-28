#ifndef INTERFACES_H
#define INTERFACES_H

#include <QtPlugin>
#include <QThread>
#include "progresswindow.h"
#include "wombatvariable.h"

class QStringList;
class QObject;
class QWidget;
class QStandardItem;


class EvidenceInterface 
{
public:
    virtual ~EvidenceInterface() {}

    virtual QStringList evidenceActions() const = 0;
    virtual QStringList evidenceActionIcons() const = 0;

    virtual QString addEvidence() = 0;
    virtual void remEvidence(int currentCaseID) = 0;

};

class BasicToolsInterface
{
public:
    virtual ~BasicToolsInterface() {}

    virtual QWidget* setupTypTab() = 0;
    virtual QWidget* setupDirTab() = 0;
    virtual QWidget* setupHexTab() = 0;
    virtual QWidget* setupTxtTab() = 0;
    virtual QStringList evidenceToolboxIcons() const = 0;
    virtual void LoadHexModel(QString tmpFilePath) = 0;
    virtual void LoadTxtContent(QString asciiText) = 0;
};

class SleuthKitInterface
{
public:
    virtual ~SleuthKitInterface() {}

    virtual void SetupSystemProperties(QString settingsPath, QString configFilePath) = 0;
    virtual void SetupSystemLog(QString dataPath, QString logFilePath, ProgressWindow* progressWindow, WombatVariable *wombatVariable) = 0;
    virtual QString SetupImageDatabase(QString imgDBPath, QString evidenceFilePath) = 0;
    virtual void OpenImageDatabase(QString imgDBPath, QString evidenceFilePath) = 0;
    virtual void SetupSystemBlackboard() = 0;
    virtual void SetupSystemSchedulerQueue() = 0;
    virtual void SetupSystemFileManager() = 0;
    virtual void OpenEvidence(QString evidencePath, ProgressWindow* progressWindow) = 0;
    virtual void LogEntry(QString logMsg) = 0;
    virtual QStandardItem* GetCurrentImageDirectoryTree(QString imageDbPath, QString imageName) = 0;
    virtual QString GetFileContents(int fileID) = 0;
    virtual QString GetFileTxtContents(int fileID) = 0;

};

#define EvidenceInterface_iid "wombat.EvidenceInterface"
Q_DECLARE_INTERFACE(EvidenceInterface, EvidenceInterface_iid)

#define BasicToolsInterface_iid "wombat.BasicToolsInterface"
Q_DECLARE_INTERFACE(BasicToolsInterface, BasicToolsInterface_iid)

#define SleuthKitInterface_iid "wombat.SleuthKitInterface"
Q_DECLARE_INTERFACE(SleuthKitInterface, SleuthKitInterface_iid)

#endif
