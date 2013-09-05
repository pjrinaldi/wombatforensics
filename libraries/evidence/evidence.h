#ifndef EVIDENCE_H
#define EVIDENCE_H

#include <main/interfaces.h>
#include <QtPlugin>
#include <QObject>
#include <cstdio>
#include <string>
#include <QStringList>
#include <QFileDialog>
#include <QVariant>

class EvidencePlugin : public QObject, public PluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "wombat.PluginInterface" FILE "evidence.json")
    Q_INTERFACES(PluginInterface)
public:
    Q_INVOKABLE PluginMap Initialize(WombatVariable wombatvariable);
    Q_INVOKABLE void Run(QString input);
    void Finalize() {};

    QMap<QString, QVariant> plugmap;

private:
    PluginMap mastermap;
    WombatVariable mastervariable;
};
#endif // EVIDENCE_H
