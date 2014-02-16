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
#include <QStandardItem>
#include <QByteArray>
#include <QDebug>

#include "tsk/libtsk.h"

struct CaseObject
{
    int id;
    QString name;
    QString dbname;
    QString dirpath;
};

struct VolumeObject
{
    int id;
    QString name;
};

struct PartitionObject
{
    int id;
    QString name;
};
/*
struct FileSystemObject
{
    int id;
    QString name;
};
*/
struct EvidenceObject
{
    int id;
    int itemcount;
    QString name;
    QStringList namelist;
    QString dirpath;
    QString fullpath;
    std::vector<std::string> fullpathvector;
    TSK_IMG_INFO* imageinfo; // These may disappear when the image is closed, will have to test this concept
    TSK_VS_INFO* volinfo; // may not need..
    //std::vector<TSK_VS_INFO*> volinfovector; // should use instead of volinfo, simply search for volumes and add them based on the size and offset to search for another one...
    TSK_VS_PART_INFO* partinfo; // may not need.
    std::vector<const TSK_VS_PART_INFO*> partinfovector;
    std::vector<TSK_FS_INFO*> fsinfovector; 
    QString dbname;
    void Clear()
    {
        fullpath = "";
        name = "";
        id = 0;
        itemcount = 0;
        namelist.clear();
        dirpath = "";
        fullpathvector.clear();
        imageinfo = NULL;
        volinfo = NULL;
        partinfo = NULL;
        partinfovector.clear();
        fsinfovector.clear();
        dbname = "";
    };
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
    EvidenceObject evidenceobject;
    //std::string evidencedbname;
    //std::string evidencepath;
    int exportcount;
};

struct SelectedObject
{
    int id;
    int type;
    int name;
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
    //int selectedobjectid;
    //int selectedobjecttype;
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
    CaseObject caseobject;
    QStringList casenamelist;
    EvidenceObject evidenceobject;
    VolumeObject volumeobject;
    QVector<EvidenceObject> evidenceobjectvector;
    QVector<VolumeObject> volumeobjectvector; // may not need
    PartitionObject partitionobject;
    QVector<PartitionObject> partitionobjectvector;
    //FileSystemObject filesystemobject;
    //QVector<FileSystemObject> filesystemobjectvector;
    SelectedObject selectedobject;
    FileExportData exportdata;
    QVector<FileExportData> exportdatavector;
    QStringList bootsectorlist; // string of values to place in html template page
    QByteArray bootbytearray; // byte array of currently extracted rawdata -> clear() before each use
    char* bootbuffer; // char buffer of raw data prior to converting bytearray -> set NULL before each use
};

Q_DECLARE_METATYPE(WombatVariable)
Q_DECLARE_METATYPE(FileExportData)
Q_DECLARE_METATYPE(CaseObject)
Q_DECLARE_METATYPE(EvidenceObject)
Q_DECLARE_METATYPE(PartitionObject)
Q_DECLARE_METATYPE(VolumeObject)
//Q_DECLARE_METATYPE(FileSystemObject)
Q_DECLARE_METATYPE(SelectedObject);

#endif // WOMBATVARIABLE_H
