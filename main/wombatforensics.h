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
#include <QThreadPool>
#include "interfaces.h"
#include "wombatvariable.h"
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
    WombatCaseDb *wombatcasedata;
    WombatVariable wombatvariable;
    ProgressWindow* wombatprogresswindow;

private slots:
    void RunPlugin();
    void AlterEvidence();
    void on_actionNew_Case_triggered();
    void on_actionOpen_Case_triggered();
    void on_actionView_Progress_triggered(bool checked);
    //void dirTreeView_selectionChanged(const QModelIndex &index);
    //void dialogClosed(QString file);
    void HideProgressWindow(bool checkstate);

protected:
    void closeEvent(QCloseEvent* event);
private:
    Ui::WombatForensics *ui;

    //QObject* loadPlugin(QString fileName);
    QList <PluginInfo> LoadPlugins();
    void PopulateActions(QObject *plugin);
    void PopulateTabWidgets(QObject *plugin);
    void SetupDirModel(void);
    void InitializeSleuthKit();
   /*
    QStringList locatePlugins(void);
    bool isPluginLoaded(QString pluginFileName);
    void populateActions(QObject *plugin);
    void populateTabWidgets(QObject *plugin);
    void SetupDirModel(void);
    void setupSleuthKitProperties(QObject *plugin, QString settingsPath, QString configFileName);
    void setupSleuthKitLog(QObject *plugin, QString dataPath, QString logFileName, WombatVariable *wombatVariable);
    QString setupSleuthKitImgDb(QObject *plugin, QString imgDBPath, QString evidenceFilePath);
    void OpenSleuthKitImgDb(QObject *plugin, QString imgDBPath, QString evidenceFilePath);
    void setupSleuthKitBlackboard(QObject *plugin);
    void setupSleuthKitSchedulerQueue(QObject *plugin);
    void setupSleuthKitFileManager(QObject *plugin);
    //Q_INVOKABLE void sleuthKitLoadEvidence(QObject *plugin, QString evidencePath, ProgressWindow* progressWindow);
    Q_INVOKABLE void sleuthKitLoadEvidence(QObject *plugin, QString evidencePath);
    void SleuthKitLogEntry(QObject *plugin, QString logMsg);
    */
    void AddActions(QObject *plugin, const QStringList &menus, const QList<QStringList> &texts, const QList<QStringList> &icons, QToolBar *toolbar, QMenuBar *menu, QActionGroup *actionGroup = 0);
    //void AddActions(QObject *plugin, const QStringList &texts, const QStringList &icons, QToolBar *toolbar, QMenu *menu, const char *member, QActionGroup *actionGroup = 0);
    /*
    QStandardItem* GetCurrentImageDirectoryTree(QObject *plugin, QString imageDbPath, QString imageName);
    void LoadHexViewer(QString tmpFilePath);
    void LoadTxtViewer(QString asciiText);
    */
    std::string GetTime(void);

    //QList<QObject*> wombatplugins;
    //QDir pluginsDir;
    QThreadPool *threadpool;
    //QStringList pluginFileNames;
    //QString currentcasedirpath;
    //QString currentcaseevidencepath;
    //QString wombatsettingspath;
    //QString wombatdatapath;
    //QString wombatcasespath;
    //QString wombattmpfilepath;
    //QStringList currentsleuthimages;
    //QObject *basictoolsplugin;
    //QObject *evidenceplugin;
    //QObject *sleuthkitplugin;
    QTreeView *currenttreeview;
    QStandardItemModel* currenttreemodel;
    QStandardItemModel* wombatdirmodel;
    QStandardItemModel* wombattypmodel;
};

class PluginRunner : public QObject, public QRunnable
{
    Q_OBJECT
public:
    PluginRunner(QObject* curCaller,WombatVariable wombatVariable, QString curMethod)
    {
        currentcaller = curCaller;
        wombatvariable = wombatVariable;
        currentmethod = curMethod;
    };
    void run()
    {
        qRegisterMetaType<WombatVariable>("WombatVariable");
        QMetaObject::invokeMethod(currentcaller, currentmethod.toStdString().c_str(), Qt::QueuedConnection, Q_ARG(WombatVariable, wombatvariable));
    };
private:
    QObject* currentcaller;
    QString currentmethod;
    WombatVariable wombatvariable;
};

#endif // WOMBATFORENSICS_H
