#ifndef WOMBATVARIABLE_H
#define WOMBATVARIABLE_H

#include <QString>
#include <QList>
#include <QVector>
#include <QStringList>
#include <QObject>
#include <vector>
#include <string>
#include <QStandardItemModel>

#include "tsk/libtsk.h"

struct ItemObject
{
    int id = 0;
    int itemcount = 0;
    QString name = "";
    QStringList namelist;
    QString dirpath = "";
    QString fullpath = "";
    std::vector<std::string> fullpathvector;
    TSK_IMG_INFO* imageinfo = NULL; // These may disappear when the image is closed, will have to test this concept
    TSK_VS_INFO* volinfo = NULL; // may not need..
    std::vector<TSK_VS_INFO*> volinfovector; // should use instead of volinfo, simply search for volumes and add them based on the size and offset to search for another one...
    //TSK_VS_PART_INFO* partinfo; // may not need.
    std::vector<const TSK_VS_PART_INFO*> partinfovector; // may not need.
    std::vector<TSK_FS_INFO*> fsinfovector; 
    QString dbname = "";
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
    QStandardItemModel* dirmodel;
    //QString evidencedbname;
    //QStringList evidencepathlist;
    //QStringList evidencedbnamelist;
    //QList<int> evidenceidlist;
    QList<int> objectidlist;
    ItemObject caseobject;
    QStringList casenamelist;
    ItemObject evidenceobject;
    ItemObject volumeobject;
    QVector<ItemObject> evidenceobjectvector;
    QVector<ItemObject> volumeobjectvector; // may not need
    ItemObject partitionobject;
    QVector<ItemObject> partitionobjectvector;
    ItemObject filesystemobject;
    QVector<ItemObject> filesystemobjectvector;
    FileExportData exportdata;
    QVector<FileExportData> exportdatavector;
};

Q_DECLARE_METATYPE(WombatVariable)
Q_DECLARE_METATYPE(FileExportData)
Q_DECLARE_METATYPE(ItemObject)

#endif // WOMBATVARIABLE_H
