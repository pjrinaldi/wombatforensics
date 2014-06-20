#ifndef WOMBATVARIABLE_H
#define WOMBATVARIABLE_H

#include "wombatinclude.h"

struct CaseObject
{
    int id;
    QString name;
    QString dbname;
    QString dirpath;
};
/*
struct VolumeObject
{
    int id;
    int objecttype;
    int type;
    int blocksize;
    int childcount;
    int byteoffset;
    int parentid;
    int parimgid;
    QString name;
};

struct PartitionObject
{
    int id;
    int objecttype;
    int flags;
    int sectstart;
    int sectlength;
    int blocksize;
    int parentid;
    int parimgid;
    QString name;
};

struct FileSystemObject
{
    int id;
    int objecttype;
    int type;
    int flags;
    int byteoffset;
    int parentid;
    int parimgid;
    int blocksize;
    int size;
    int blockcount;
    int firstinum;
    int lastinum;
    int rootinum;
    QString name;
};

struct FileObject
{
    // "Name" << "Full Path" << "Size (Bytes)" << "Signature" << "Extension" << "Created (UTC)" << "Accessed (UTC)" << "Modified (UTC)" << "Status Changed (UTC)" << "MD5 Hash";
    int id;
    int type;
    int objecttype;
    int nameflags;
    QString name;
    int metaaddress;
    int paraddress;
    QString shortname;
    int address;
    QString atime;
    QString crtime;
    QString ctime;
    QString mtime;
    int metaflags;
    int size;
    int parentid;
    int parimgid;
    // file and dir data stored here... metaVAR and/or nameVAR if they duplicate.
};
*/
struct EvidenceObject
{
    int id;
    int type;
    int objecttype;
    int itemcount;
    QString name;
    int size;
    int sectsize;
    int parentid;
    int parimgid;
    QStringList namelist;
    QString fullpath; 
    std::vector<std::string> fullpathvector;
    TSK_IMG_INFO* imageinfo; // These may disappear when the image is closed, will have to test this concept
    TSK_VS_INFO* volinfo; // may not need..
    //std::vector<TSK_VS_INFO*> volinfovector; // should use instead of volinfo, simply search for volumes and add them based on the size and offset to search for another one...
    TSK_VS_PART_INFO* partinfo; // may not need.
    std::vector<const TSK_VS_PART_INFO*> partinfovector;
    std::vector<TSK_FS_INFO*> fsinfovector; 
    TSK_FS_FILE* fileinfo; // may not need
    TSK_FS_DIR* dirinfo; // may not need
    std::vector<void*> dirfileinfovector; // may not need.
    QString dbname;
    void Clear()
    {
        fullpath = "";
        name = "";
        id = 0;
        type = 0;
        itemcount = 0;
        size = 0;
        sectsize = 0;
        namelist.clear();
        parentid = 0;
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
    int exportcount;
};

struct SelectedObject
{
    int id;
    int type;
    QString name;
    int size;
    int parimgid;
    int sectstart;
    int sectlength;
    int blocksize;
    int blockcount;
    int byteoffset;
    int address;
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
    int jobtype;
    int jobid;
    int fileid;
    int volid;
    int filecount;
    int processcount;
    int omnivalue;
    int visibleviewer;
    int currentevidenceid;
    int currentvolumeid;
    int currentpartitionid;
    int currentfilesystemid;
    int currentrootinum;
    QString currentevidencename;
    QString currentvolumename;
    QString casespath;
    QString settingspath;
    QString datapath;
    QString tmpfilepath;
    QString curerrmsg;
    QString wombatdbname;
    //QStandardItemModel* dirmodel;
    QList<int> objectidlist;
    CaseObject caseobject;
    QStringList casenamelist;
    EvidenceObject evidenceobject;
    //VolumeObject volumeobject;
    QVector<EvidenceObject> evidenceobjectvector;
    //QVector<VolumeObject> volumeobjectvector;
    //PartitionObject partitionobject;
    //QVector<PartitionObject> partitionobjectvector;
    //FileSystemObject filesystemobject;
    //QVector<FileSystemObject> filesystemobjectvector;
    //QVector<FileObject> fileobjectvector;
    SelectedObject selectedobject;
    FileExportData exportdata;
    QVector<FileExportData> exportdatavector;
    // QByteArray bootbytearray; // byte array of currently extracted rawdata -> clear() before each use [don't think i use this anymore]
    std::vector<uchar> rawbyteintvector;
    QString htmlcontent;
    QSqlDatabase casedb;
    QSqlDatabase appdb;
    QList<QSqlRecord> sqlrecords;
    QList<QVariant> bindvalues;
};

Q_DECLARE_METATYPE(WombatVariable)
Q_DECLARE_METATYPE(FileExportData)
Q_DECLARE_METATYPE(CaseObject)
Q_DECLARE_METATYPE(EvidenceObject)
//Q_DECLARE_METATYPE(PartitionObject)
//Q_DECLARE_METATYPE(VolumeObject)
//Q_DECLARE_METATYPE(FileSystemObject)
//Q_DECLARE_METATYPE(FileObject);
Q_DECLARE_METATYPE(SelectedObject);

#endif // WOMBATVARIABLE_H
