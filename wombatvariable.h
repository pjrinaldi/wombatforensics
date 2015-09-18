#ifndef WOMBATVARIABLE_H
#define WOMBATVARIABLE_H

#include "wombatinclude.h"

struct CaseObject
{
    unsigned long long id;
    QString name;
    QString dbname;
    QString dirpath;
};

struct EvidenceObject
{
    unsigned long long id;
    int type;
    int objecttype;
    unsigned long long itemcount;
    QString name;
    unsigned long long size;
    int sectsize;
    unsigned long long parentid;
    unsigned long long parimgid;
    QStringList namelist;
    QString fullpath; 
    std::vector<std::string> fullpathvector;
    TSK_IMG_INFO* imageinfo; // These may disappear when the image is closed, will have to test this concept
    TSK_VS_INFO* volinfo;
    std::vector<const TSK_VS_PART_INFO*> partinfovector;
    std::vector<TSK_FS_INFO*> fsinfovector;
    std::vector<unsigned long long> fsidvector;
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
    unsigned long long id;
    FileStatus filestatus;
    PathStatus pathstatus;
    std::string name;
    std::string fullpath;
    EvidenceObject evidenceobject;
    unsigned long long exportcount;
};

struct SelectedObject
{
    unsigned long long id;
    int objtype;
    QString name;
    QString fullpath;
    QString blockaddress;
    QString filesignature;
    QString filemime;
    unsigned long long size;
    unsigned long long parimgid;
    unsigned long long parfsid;
    int sectsize;
    unsigned long long sectstart;
    unsigned long long sectlength;
    int blocksize;
    unsigned long long blockcount;
    unsigned long long byteoffset;
    unsigned long long address;
    unsigned long long parentid;
    unsigned long long mftattrid;
    int type;
    int flags;
};

struct WombatVariable
{
    /*
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
    */

    // id values
    int jobtype;
    unsigned long long jobid;
    unsigned long long fileid;
    unsigned long long volid;
    unsigned long long filecount;
    unsigned long long processcount;
    unsigned long long currentevidenceid;
    unsigned long long currentvolumeid;
    unsigned long long currentpartitionid;
    unsigned long long currentfilesystemid;
    unsigned long long currentrootinum;
    QString currentevidencename;
    QString currentvolumename;
    QString casespath;
    QString settingspath;
    QString datapath;
    QString tmpfilepath;
    QString curerrmsg;
    QString wombatdbname;
    QString thumbdbname;
    QString evidremovestring;
    unsigned long long evidremoveid;
    unsigned long long evidrowsremoved;
    CaseObject caseobject;
    QStringList casenamelist;
    QStringList evidencenamelist;
    EvidenceObject evidenceobject;
    QVector<EvidenceObject> evidenceobjectvector;
    SelectedObject selectedobject;
    FileExportData exportdata;
    QVector<FileExportData> exportdatavector;
    QString htmlcontent;
    QSqlDatabase casedb;
    QSqlDatabase appdb;
    QList<QSqlRecord> sqlrecords;
    QList<QVariant> bindvalues;
};

/*
typedef struct
{
    TSK_IMG_INFO imginfo;
    //AFFILE* affile;
    TSK_OFF_T seekpos;
    uint16_t type;
} IMG_AFF_INFO;
*/

typedef struct
{
    TSK_IMG_INFO imginfo;
    libewf_handle_t* handle;
    char md5hash[33];
    int md5hashisset;
    TSK_TCHAR** images;
    int numimgs;
    uint8_t usedewfglob;
    tsk_lock_t readlock;
} IMG_EWF_INFO;

Q_DECLARE_METATYPE(WombatVariable)
Q_DECLARE_METATYPE(FileExportData)
Q_DECLARE_METATYPE(CaseObject)
Q_DECLARE_METATYPE(EvidenceObject)
Q_DECLARE_METATYPE(SelectedObject);
//Q_DECLARE_METATYPE(IMG_AFF_INFO);
Q_DECLARE_METATYPE(IMG_EWF_INFO);

#endif // WOMBATVARIABLE_H
