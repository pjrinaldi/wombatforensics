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
#include <QMap>
#include <QVariant>
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
    //void AlterEvidence();
    void on_actionNew_Case_triggered();
    void on_actionOpen_Case_triggered();
    void on_actionView_Progress_triggered(bool checked);
    //void dirTreeView_selectionChanged(const QModelIndex &index);
    //void dialogClosed(QString file);
    void HideProgressWindow(bool checkstate);
    //QMenu* AddMenu(QString tmpmenu);
    //QAction* AddMenuItem(QStringList tmplist);
    //void AddToolButton(QAction* action);
    //void AddViewTab(QWidget* widget, QString title);
    //void AddInfoTab(QWidget* widget, QString title);
    //void ConnectWidget(QObject* object, const char* signal);
    void GetPluginMap(PluginMap testmap, QObject* caller);

protected:
    void closeEvent(QCloseEvent* event);
private:
    Ui::WombatForensics *ui;

    //QObject* loadPlugin(QString fileName);
    QList <PluginInfo> LoadPlugins();
    //void PopulateActions(QObject *plugin);
    //void PopulateTabWidgets(QObject *plugin);
    void SetupDirModel(void);
    void InitializeSleuthKit();
   /*
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
    /*
    QStandardItem* GetCurrentImageDirectoryTree(QObject *plugin, QString imageDbPath, QString imageName);
    void LoadHexViewer(QString tmpFilePath);
    void LoadTxtViewer(QString asciiText);
    */
    std::string GetTime(void);
    QThreadPool *threadpool;
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
    PluginMap runnermap;
    void run()
    {
        qRegisterMetaType<WombatVariable>("WombatVariable");
        qRegisterMetaType<PluginMap>("PluginMap");
        QMetaObject::invokeMethod(currentcaller, currentmethod.toStdString().c_str(), Qt::DirectConnection, Q_RETURN_ARG(PluginMap, runnermap), Q_ARG(WombatVariable, wombatvariable));
        emit GetPluginMap(runnermap, currentcaller);
    };
signals:
    void GetPluginMap(PluginMap tmpmap, QObject* caller);
private:
    QObject* currentcaller;
    QString currentmethod;
    WombatVariable wombatvariable;
};

#endif // WOMBATFORENSICS_H
