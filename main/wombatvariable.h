#ifndef WOMBATVARIABLE_H
#define WOMBATVARIABLE_H

#include <QString>
#include <QList>
#include <QStringList>
#include <QObject>
#include <QVariant>
#include <QMap>

struct PluginInfo
{
    QObject* plugin;
    QString name;
};

struct ViewerMap
{
    QVariantMap map;
    QStringList icons;
    QString name;
};

struct PluginMap
{
    QMap<QString, QVariant> map;
};

struct WombatVariable
{
    void AddEvidence(QString evidence) { evidencelist.append(evidence); };

    // ADD ENUMS FOR JOBSTATUS, JOBTYPE, ETC TO SIMPLIFY INTEGER ASSOCIATION AND READABILITY IN CODE
    enum JobStatus
    {
        failed = 0,
        finish = 1,
        finisherror = 2,
        cancelled = 3
    };

    enum JobType
    {
        generic = 0,
        addevidence = 1,
        remedvidence = 2
    };

    int caseid;
    int evidenceid;
    int jobtype;
    int jobid;
    QList<PluginInfo> pluginfo;
    QString casespath;
    QString casedirpath;
    QString evidencedirpath;
    QStringList evidencelist;
    QString settingspath;
    QString datapath;
    QString tmpfilepath;
    QObject* parent;

};

Q_DECLARE_METATYPE(WombatVariable)

#endif // WOMBATVARIABLE_H
