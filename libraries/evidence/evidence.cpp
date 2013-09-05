#include "evidence.h"

PluginMap EvidencePlugin::Initialize(WombatVariable wombatvariable)
{
    PluginMap mainmap;
    plugmap.clear();
    // QMenu setup
    plugmap.insert("addmenuitem", QStringList() << "Evidence" << "Add Evidence" << tr(":/basic/addevidence") << "Remove Evidence" << ":/basic/remevidence");
    plugmap.insert("addtoolbutton", QStringList() << "Add Evidence" << tr(":/basic/addevidence"));
    plugmap.insert("actions", QStringList() << "Add Evidence" << "Remove Evidence");
    mainmap.map = plugmap;
    mastermap.map = plugmap;
    return mainmap;
}

void EvidencePlugin::Run(QString input)
{
    fprintf(stderr, "Map Size: %d\n", mastermap.map.size());
    QStringList tmplist = mastermap.map["actions"].toStringList();
    foreach(QString tmpstring, tmplist)
    {
        if(input.compare(tmpstring) == 0)
            fprintf(stderr, "Run with input: %s\n", input.toStdString().c_str());
    }
}
