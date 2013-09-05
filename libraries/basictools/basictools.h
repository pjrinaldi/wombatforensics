#ifndef BASICTOOLS_H
#define BASICTOOLS_H

#include <main/interfaces.h>
#include "binviewwidget.h"
#include <QtPlugin>
#include <QObject>
#include <QtWidgets>
#include <QStringList>
#include <QVariantMap>

class BasicTools : public QObject, public ViewerInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "wombat.ViewerInterface" FILE "basictools.json")
    Q_INTERFACES(ViewerInterface)

public:
    // plugin interface functions
    //Q_INVOKABLE PluginMap Initialize(WombatVariable wombatvariable);
    QVariantMap Initialize();
    void Run(QString input);
    //Q_INVOKABLE void Run(QString input);
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
    QMap<QString, QVariant> plugmap;
private:
    PluginMap mastermap;
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
