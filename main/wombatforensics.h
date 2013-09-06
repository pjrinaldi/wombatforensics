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
#include <QTreeWidgetItem>
#include <string>
#include <QString>
#include <QThreadPool>

#include "wombatvariable.h"
#include "wombatcasedb.h"
#include "ui_wombatforensics.h"
#include "progresswindow.h"
#include "sleuthkit.h"
#include "basictools.h"

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
    SleuthKitPlugin* isleuthkit;
    BasicTools* ibasictools;

signals:
    void LogVariable(WombatVariable wombatVariable);
private slots:
    void AddEvidence();
    void RemEvidence();
    void on_actionNew_Case_triggered();
    void on_actionOpen_Case_triggered();
    void on_actionView_Progress_triggered(bool checked);
    void UpdateProgress(int count, int processcount);
    void UpdateMessageTable();
    void GetImageNode(QStandardItem* imageNode);
    void dirTreeView_selectionChanged(const QModelIndex &index);
    void HideProgressWindow(bool checkstate);

protected:
    void closeEvent(QCloseEvent* event);
private:
    Ui::WombatForensics *ui;

    void SetupDirModel(void);
    void InitializeSleuthKit();
    
    QStandardItem* GetCurrentImageDirectoryTree(QObject *plugin, QString imageDbPath, QString imageName);
    std::string GetTime(void);
    QThreadPool *threadpool;
    QTreeView *currenttreeview;

    QStandardItemModel* currenttreemodel;
    QStandardItemModel* wombatdirmodel;
    QStandardItemModel* wombattypmodel;
};

class ThreadRunner : public QObject, public QRunnable
{
    Q_OBJECT
public:
    ThreadRunner(QObject* object, QString input, WombatVariable wVariable)
    {
        method = input;
        caller = (SleuthKitPlugin*)object;
        wombatvariable = wVariable;
    };
    void run()
    {
        if(method.compare("initialize") == 0)
            caller->Initialize(wombatvariable);
        if(method.compare("openevidence") == 0)
            caller->OpenEvidence(wombatvariable);
    };
private:
    QString method;
    SleuthKitPlugin* caller;
    WombatVariable wombatvariable;
};

#endif // WOMBATFORENSICS_H
