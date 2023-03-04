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

    virtual void addEvidence(int currentCaseID) = 0;
    virtual void remEvidence(int currentCaseID) = 0;

};

class BasicToolsInterface
{
public:
    virtual ~BasicToolsInterface() {}

    virtual QStringList toolboxViews() const = 0;
    virtual QWidget* setupToolBoxDirectoryTree() const = 0;
    virtual QWidget* setupToolBoxFileExtensionTree() const = 0;
    virtual QWidget* setupTabWidget() const = 0;
    virtual QWidget* setupColumnView() const = 0;
    virtual QStringList evidenceToolboxIcons() const = 0;
};

#define EvidenceInterface_iid "wombat.EvidenceInterface"

Q_DECLARE_INTERFACE(EvidenceInterface, EvidenceInterface_iid)

#define BasicToolsInterface_iid "wombat.BasicToolsInterface"

Q_DECLARE_INTERFACE(BasicToolsInterface, BasicToolsInterface_iid)
            
#endif
