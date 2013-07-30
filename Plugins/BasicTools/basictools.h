#ifndef BASICTOOLS_H
#define BASICTOOLS_H

#include <interfaces.h>
#include "wombattskimagefiletsk.h"
#include "wombattskimgdbsqlite.h"
#include "tsk/framework/services/TskImgDBSqlite.h"
#include "tsk/framework/extraction/TskImageFileTsk.h"
#include "tsk/framework/services/TskSchedulerQueue.h"
#include "tsk/framework/file/TskFileManagerImpl.h"
#include "tsk/framework/services/TskSystemPropertiesImpl.h"
#include <QStringList>
#include <QtWidgets>
#include <QFileDialog>

class BasicTools : public QObject, public EvidenceInterface, public BasicToolsInterface, public TskFrameworkInterface
{
    Q_OBJECT
#if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA(IID "wombat.forensics.EvidenceInterface" FILE "BasicTools.json")
#endif // QT_VERSION >= 0x050000
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

    QWidget* setupToolBox() const;
    QWidget* setupTabWidget() const;
    QWidget* setupColumnView() const;

    //TskFramework Interface Functions

private:
    sqlite3_stmt *sqlStatement;
    int currentcaseid;

};

#endif // BASICTOOLS_H
