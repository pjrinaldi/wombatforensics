#ifndef WOMBATFORENSICS_H
#define WOMBATFORENSICS_H

#include <QMainWindow>
#include <QDir>
#include <QStringList>
#include <QActionGroup>
#include <QFileDialog>
#include <QObject>
#include <QStandardItemModel>

// BEGIN TSK TEST INCLUDES
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <sstream>
#include <time.h>
#include <memory>

#include "interfaces.h"
#include "/usr/local/include/tsk/tsk_tools_i.h"
#include "tsk/framework/framework.h"
#include "tsk/framework/services/TskSchedulerQueue.h"
#include "tsk/framework/services/TskSystemPropertiesImpl.h"
#include "tsk/framework/services/TskImgDBSqlite.h"
#include "tsk/framework/file/TskFileManagerImpl.h"
#include "tsk/framework/extraction/TskCarvePrepSectorConcat.h"
#include "tsk/framework/extraction/TskCarveExtractScalpel.h"
#include "tsk/framework/extraction/TskExtract.h"

#include "Poco/Path.h"
#include "Poco/File.h"

#ifdef TSK_WIN32
#include <Windows.h>
#else
#include <sys/stat.h>
#endif

#include "Poco/File.h"
#include "Poco/UnicodeConverter.h"

// END TSK TEST INCLUDES

//class QActionGroup;

namespace Ui {
class WombatForensics;
}

class WombatForensics : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit WombatForensics(QWidget *parent = 0);
    ~WombatForensics();
    
private slots:
    void alterEvidence();
    void on_actionAdd_Evidence_triggered();
    void on_actionNew_Case_triggered();
    void on_actionOpen_Case_triggered();
    void on_toolBox_currentChanged(int index);

private:
    Ui::WombatForensics *ui;

    void loadPlugins();
    void populateMenus(QObject *plugin);
    void addToMenu(QObject *plugin, const QStringList &texts, QMenu *menu, const char *member, QActionGroup *actionGroup = 0);

    void makeDir(const char *dir);

    QDir pluginsDir;
    QStringList pluginFileNames;

};

#endif // WOMBATFORENSICS_H
