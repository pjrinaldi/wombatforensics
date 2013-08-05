#ifndef INTERFACES_H
#define INTERFACES_H

#include <QtPlugin>

class QStringList;
class QObject;
class QWidget;



class EvidenceInterface 
{
public:
    virtual ~EvidenceInterface() {}

    virtual QStringList evidenceActions() const = 0;
    virtual QStringList evidenceActionIcons() const = 0;

    virtual QString addEvidence(int currentCaseID) = 0;
    virtual void remEvidence(int currentCaseID) = 0;

};

class BasicToolsInterface
{
public:
    virtual ~BasicToolsInterface() {}

    virtual QStringList toolboxViews() const = 0;
    virtual QWidget* setupToolBoxDirectoryTree() const = 0;
    virtual QWidget* setupToolBoxFileExtensionTree() const = 0;
    virtual QWidget* setupHexTab() const = 0;
    virtual QWidget* setupTxtTab() const = 0;
    virtual QWidget* setupColumnView() const = 0;
    virtual QStringList evidenceToolboxIcons() const = 0;
};

class SleuthKitInterface
{
public:
    virtual ~SleuthKitInterface() {}

    virtual void SetupSystemProperties(QString settingsPath, QString configFilePath) = 0;
    virtual void SetupSystemLog(QString dataPath, QString logFilePath) = 0;
    virtual void SetupImageDatabase(QString imgDBPath, QString evidenceFilePath) = 0;
    virtual void SetupSystemBlackboard() = 0;
    virtual void SetupSystemSchedulerQueue() = 0;
    virtual void SetupSystemFileManager() = 0;
    virtual void OpenEvidence(QString evidencePath) = 0;
};

#define EvidenceInterface_iid "wombat.EvidenceInterface"

Q_DECLARE_INTERFACE(EvidenceInterface, EvidenceInterface_iid)

#define BasicToolsInterface_iid "wombat.BasicToolsInterface"

Q_DECLARE_INTERFACE(BasicToolsInterface, BasicToolsInterface_iid)

#define SleuthKitInterface_iid "wombat.SleuthKitInterface"
Q_DECLARE_INTERFACE(SleuthKitInterface, SleuthKitInterface_iid)

#endif
