#ifndef BASICTOOLS_H
#define BASICTOOLS_H

#include "../../interfaces.h"
#include <QtPlugin>
#include <QObject>
#include <QtWidgets>
#include "sqlite3.h"
#include <QStringList>
#include <QFileDialog>

class BasicTools : public QObject, public EvidenceInterface, public BasicToolsInterface, public TskFrameworkInterface
{
    Q_PLUGIN_METADATA(IID "wombat.forensics.BasicToolsInterface" FILE "BasicTools.json")
    Q_INTERFACES(EvidenceInterface BasicToolsInterface TskFrameworkInterface)
    
public:
    //Evidence Interface Functions
    QStringList evidenceActions() const;
    QStringList evidenceActionIcons() const;

    void addEvidence(int currentCaseID);
    void remEvidence(int currentCaseID);

    //BasicTools Interface Functions
    QStringList toolboxViews() const;
    QStringList evidenceToolboxIcons() const;

    QWidget* setupToolBoxDirectoryTree() const;
    QWidget* setupToolBoxFileExtensionTree() const;
    QWidget* setupTabWidget() const;
    QWidget* setupColumnView() const;

    //TskFramework Interface Functions

private:
    sqlite3_stmt *sqlStatement;
    int currentcaseid;

};

#endif // BASICTOOLS_H
