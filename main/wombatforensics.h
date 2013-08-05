#ifndef WOMBATFORENSICS_H
#define WOMBATFORENSICS_H

#include <QMainWindow>
#include <QDir>
#include <QStringList>
#include <QActionGroup>
#include <QFileDialog>
#include <QObject>
#include <QStandardItemModel>
#include <QInputDialog>
#include <QMessageBox>
#include <QPluginLoader>

#include "interfaces.h"
#include "wombatcasedb.h"
#include "ui_wombatforensics.h"

namespace Ui {
class WombatForensics;
}

class WombatForensics : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit WombatForensics(QWidget *parent = 0);
    ~WombatForensics();
    WombatCaseDb *wombatCaseData;

private slots:
    void alterEvidence();
    void on_actionNew_Case_triggered();
    void on_actionOpen_Case_triggered();

private:
    Ui::WombatForensics *ui;

    QObject* loadPlugin(QString fileName);
    QStringList locatePlugins(void);
    bool isPluginLoaded(QString pluginFileName);
    void populateActions(QObject *plugin);
    void populateToolBox(QObject *plugin);
    void populateTabWidget(QObject *plugin);
    void setupSleuthKitProperties(QObject *plugin, QString settingsPath, QString configFileName);
    void setupSleuthKitLog(QObject *plugin, QString dataPath, QString logFileName);
    void setupSleuthKitImgDb(QObject *plugin, QString imgDBPath, QString evidenceFilePath);
    void setupSleuthKitBlackboard(QObject *plugin);
    void setupSleuthKitSchedulerQueue(QObject *plugin);
    void setupSleuthKitFileManager(QObject *plugin);
    void sleuthKitLoadEvidence(QObject *plugin, QString evidencePath);
    void addActions(QObject *plugin, const QStringList &texts, const QStringList &icons, QToolBar *toolbar, QMenu *menu, const char *member, QActionGroup *actionGroup = 0);

    int64_t currentcaseid;
    QDir pluginsDir;
    QStringList pluginFileNames;
    QString currentcasedirpath;
    QString currentcaseevidencepath;
    QString wombatsettingspath;
    QString wombatdatapath;
    QString wombatcasespath;
    QObject *basictoolsplugin;
    QObject *evidenceplugin;
    QObject *sleuthkitplugin;
};

#endif // WOMBATFORENSICS_H
