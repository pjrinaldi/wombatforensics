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

#include "interfaces.h"
#include "tskfunctions.h"
#include "wombatcasedb.h"

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

    void loadPlugins();
    void populateMenus(QObject *plugin);
    void populateToolBox(QObject *plugin);
    void populateToolBar(QObject *plugin);

    void addToMenu(QObject *plugin, const QStringList &texts, QMenu *menu, const char *member, QActionGroup *actionGroup = 0);

    void makeDir(const char *dir);

    sqlite3_stmt *sqlStatement;
    int currentcaseid;
    QDir pluginsDir;
    QStringList pluginFileNames;
};

#endif // WOMBATFORENSICS_H
