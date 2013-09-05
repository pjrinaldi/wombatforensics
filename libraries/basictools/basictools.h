#ifndef BASICTOOLS_H
#define BASICTOOLS_H

#include "main/interfaces.h"
#include "binviewwidget.h"
#include <QtPlugin>
#include <QObject>
#include <QtWidgets>
//#include <sqlite3.h>
#include <QStringList>
//#include <QFileDialog>

class BasicTools : public QObject, public PluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "wombat.PluginInterface" FILE "basictools.json")
    Q_INTERFACES(PluginInterface)

public:
    // plugin interface functions
    //QList<QStringList> PluginActions() const;
    //QList<QStringList> PluginActionIcons() const;
    //QStringList PluginMenus() const;
    
    //Q_INVOKABLE void Initialize(WombatVariable wombatvariable) {};
    Q_INVOKABLE PluginMap Initialize(WombatVariable wombatvariable) {};
    //Q_INVOKABLE QMap<QString, QVariant> Initialize(WombatVariable wombatvariable) {};
    void Run(QString input);
    void Finalize() {};

    //QStringList pluginstringlist = evidenceToolboxIcons();
    //BasicTools Custom Functions
    //QStringList evidenceToolboxIcons() const;

    QWidget* setupHexTab();
    QWidget* setupTxtTab();
    QWidget* setupDirTab();
    QWidget* setupTypTab();
    void LoadHexModel(QString tmpFilePath);
    void LoadTxtContent(QString asciiText);
private:
    BinViewModel* hexmodel;
    BinViewWidget* hexwidget;
    QTreeView* dirtreeview;
    QTreeView* typtreeview;
    QTextEdit* txtwidget;
};

/*
class BasicTools : public QObject, public BasicToolsInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "wombat.BasicToolsInterface" FILE "basictools.json")
    Q_INTERFACES(BasicToolsInterface)
    
public:
    //BasicTools Interface Functions
    QStringList evidenceToolboxIcons() const;

    QWidget* setupHexTab();
    QWidget* setupTxtTab();
    QWidget* setupDirTab();
    QWidget* setupTypTab();
    void LoadHexModel(QString tmpFilePath);
    void LoadTxtContent(QString asciiText);

private:
    //sqlite3_stmt *sqlStatement;
    //int currentcaseid;
    BinViewModel* hexmodel;
    BinViewWidget* hexwidget;
    QTreeView* dirtreeview;
    QTreeView* typtreeview;
    QTextEdit* txtwidget;
};
*/
#endif // BASICTOOLS_H
