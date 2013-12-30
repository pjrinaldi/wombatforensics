#ifndef WOMBATVARIABLE_H
#define WOMBATVARIABLE_H

#include <QString>
#include <QList>
#include <QVector>
#include <QStringList>
#include <QObject>
#include <vector>
#include <string>

#include "tsk/libtsk.h"

struct ItemObject
{
    int id;
    int itemcount;
    QString name;
    QStringList namelist;
    QString dirpath;
    QString fullpath;
    //std::list<const char*> fullpathlist;
    const char* fullpathlist[];
    QString dbname;
};

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
    ItemObject evidenceobject;
    //std::string evidencedbname;
    //std::string evidencepath;
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

    // id values
    //int caseid;
    //int evidenceid;
    int jobtype;
    int jobid;
    int fileid;
    int volid;
    int filecount;
    int processcount;
    int omnivalue;
    int visibleviewer;
    int selectedobjectid;
    // directory/file paths
    QString casespath;
    //QString casedirpath;
    //QString casedbpath;
    //QString evidencedirpath;
    //QString evidencepath;
    QString settingspath;
    QString datapath;
    QString tmpfilepath;
    QString curerrmsg;
    QString wombatdbname;
    //QString evidencedbname;
    //QStringList evidencepathlist;
    //QStringList evidencedbnamelist;
    //QList<int> evidenceidlist;
    QList<int> objectidlist;
    ItemObject caseobject;
    QStringList casenamelist;
    //QVector<CaseObject*> caseobjectvector;
    ItemObject evidenceobject;
    QVector<ItemObject> evidenceobjectvector;
    FileExportData exportdata;
    QVector<FileExportData> exportdatavector;

};

Q_DECLARE_METATYPE(WombatVariable)
Q_DECLARE_METATYPE(FileExportData)
Q_DECLARE_METATYPE(ItemObject)

#endif // WOMBATVARIABLE_H
