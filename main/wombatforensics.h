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
#include <QTreeWidgetItem>
#include <string>
#include <QString>

#include "interfaces.h"
#include "wombatcasedb.h"
#include "ui_wombatforensics.h"
#include "progresswindow.h"

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
    ProgressWindow* wombatprogresswindow;

private slots:
    void alterEvidence();
    void on_actionNew_Case_triggered();
    void on_actionOpen_Case_triggered();
    void on_actionView_Progress_triggered();
    void dirTreeView_selectionChanged(const QModelIndex &index);

private:
    Ui::WombatForensics *ui;

    QObject* loadPlugin(QString fileName);
    QStringList locatePlugins(void);
    bool isPluginLoaded(QString pluginFileName);
    void populateActions(QObject *plugin);
    void populateTabWidgets(QObject *plugin);
    void SetupDirModel(void);
    void setupSleuthKitProperties(QObject *plugin, QString settingsPath, QString configFileName);
    void setupSleuthKitLog(QObject *plugin, QString dataPath, QString logFileName);
    QString setupSleuthKitImgDb(QObject *plugin, QString imgDBPath, QString evidenceFilePath);
    void OpenSleuthKitImgDb(QObject *plugin, QString imgDBPath, QString evidenceFilePath);
    void setupSleuthKitBlackboard(QObject *plugin);
    void setupSleuthKitSchedulerQueue(QObject *plugin);
    void setupSleuthKitFileManager(QObject *plugin);
    void sleuthKitLoadEvidence(QObject *plugin, QString evidencePath);
    void addActions(QObject *plugin, const QStringList &texts, const QStringList &icons, QToolBar *toolbar, QMenu *menu, const char *member, QActionGroup *actionGroup = 0);
    QStandardItem* GetCurrentImageDirectoryTree(QObject *plugin, QString imageDbPath, QString imageName);
    void LoadHexViewer(QString tmpFilePath);
    void LoadTxtViewer(QString asciiText);
    std::string GetTime(void);

    int64_t currentcaseid;
    QDir pluginsDir;
    QStringList pluginFileNames;
    QString currentcasedirpath;
    QString currentcaseevidencepath;
    QString wombatsettingspath;
    QString wombatdatapath;
    QString wombatcasespath;
    QString wombattmpfilepath;
    QStringList currentsleuthimages;
    QObject *basictoolsplugin;
    QObject *evidenceplugin;
    QObject *sleuthkitplugin;
    QTreeView *currenttreeview;
    QStandardItemModel* currenttreemodel;
    QStandardItemModel* wombatdirmodel;
    QStandardItemModel* wombattypmodel;
};

#endif // WOMBATFORENSICS_H
