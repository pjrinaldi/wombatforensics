#ifndef WOMBATVARIABLE_H
#define WOMBATVARIABLE_H

#include <QString>
#include <QList>
#include <QStringList>
#include <QObject>
#include <vector>
#include <string>

struct FileExportData
{
    enum PathStatus
    {
        include = 0,
        exclude = 1
    };

    enum FileStatus
    {
        selected = 0,
        checked = 1,
        listed = 2
    };

    std::string exportpath;
    int id;
    FileStatus filestatus;
    PathStatus pathstatus;
    std::string name;
    std::string fullpath;
    std::string evidencedbname;
    std::string evidencepath;
    int exportcount;
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
        remedvidence = 2,
        exportfiles = 3
    };

    int caseid = 0;
    int evidenceid = 0;
    int jobtype = 0;
    int jobid = 0;
    int fileid = 0;
    int volid = 0;
    int filecount = 0;
    int processcount = 0;
    int omnivalue = 1;
    int visibleviewer = 0;
    int selectedobjectid = 0;
    QString casespath = "";
    QString casedirpath = "";
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
    
    FileExportData exportdata;
    std::vector<FileExportData> exportdatalist;

};

Q_DECLARE_METATYPE(WombatVariable)
Q_DECLARE_METATYPE(FileExportData)

#endif // WOMBATVARIABLE_H
