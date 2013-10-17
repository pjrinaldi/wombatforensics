#ifndef WOMBATVARIABLE_H
#define WOMBATVARIABLE_H

#include <QString>
#include <QList>
#include <QStringList>
#include <QObject>

struct FileExportData
{
    int id;
    std::string name;
    std::string fullpath;
};

struct WombatVariable
{

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
    int fileid;
    int filecount;
    int processcount;
    QString casespath;
    QString casedirpath;
    QString evidencedirpath;
    QString evidencepath;
    QString settingspath;
    QString datapath;
    QString tmpfilepath;
    QString evidencedbname;
    QStringList evidencepathlist;
    QStringList evidencedbnamelist;
    QList<int> evidenceidlist;
    QList<int> objectidlist;

};

Q_DECLARE_METATYPE(WombatVariable)

#endif // WOMBATVARIABLE_H
