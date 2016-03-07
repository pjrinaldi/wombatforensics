#ifndef WOMBATVARIABLE_H
#define WOMBATVARIABLE_H

// Copyright 2015 Pasquale J. Rinaldi, Jr.
// Distrubted under the terms of the GNU General Public License version 2

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
    //int sectsize;
    unsigned long long parentid;
    unsigned long long parimgid;
    unsigned long long parfsid;
    //QStringList namelist;
    QString fullpath; 
    std::vector<std::string> fullpathvector;
    //TSK_IMG_INFO* imageinfo; // These may disappear when the image is closed, will have to test this concept
    //TSK_VS_INFO* volinfo;
    //std::vector<const TSK_VS_PART_INFO*> partinfovector;
    //std::vector<TSK_FS_INFO*> fsinfovector;
    //std::vector<unsigned long long> fsidvector;
    //QString dbname;
    /*
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
    */
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

struct FileDeepData
{
    enum FileStatus
    {
        selected = 0,
        checked = 1,
        listed = 2
    };
    
    enum DigOptions
    {
        md5 = 0,
        thumb = 1
    };

    std::vector<DigOptions> digoptions;
    FileStatus filestatus;
    unsigned long long id;
    std::string name;
    std::string fullpath;
    EvidenceObject evidenceobject;
    unsigned long long digcount;
};
/*
struct AdsProcessObject
{
    unsigned long long objectid;
    unsigned long long attrid;
};
struct SecondaryProcessObject
{
    unsigned long long objectid;
    unsigned long long parimgid;
    unsigned long long parfsid;
    unsigned long long address;
    QString mimetype;
    QString name;
    QString blockaddress;
    QVector<AdsProcessObject> adsprocessvector;
};
*/
struct SelectedObject
{
    unsigned long long id;
    int objtype;
    QString name;
    QString fullpath;
    //QString blockaddress;
    //QString filesignature;
    QString filemime;
    unsigned long long size;
    unsigned long long parimgid;
    unsigned long long parfsid;
    //int sectsize;
    //unsigned long long sectstart;
    //unsigned long long sectlength;
    //int blocksize;
    //unsigned long long blockcount;
    //unsigned long long byteoffset;
    unsigned long long address;
    unsigned long long parentid;
    unsigned long long mftattrid;
    int type;
    //int flags;
};

struct WombatVariable
{
    // id values
    //int jobtype;
    //unsigned long long jobid;
    //unsigned long long fileid;
    //unsigned long long volid;
    //unsigned long long filecount;
    //unsigned long long processcount;
    //unsigned long long currentevidenceid;
    unsigned long long currentvolumeid;
    unsigned long long currentpartitionid;
    unsigned long long currentfilesystemid;
    unsigned long long currentrootinum;
    QString currentevidencename;
    //QString currentvolumename;
    QString casespath;
    QString settingspath;
    QString datapath;
    QString tmpfilepath;
    QString curerrmsg;
    QString wombatdbname;
    QString thumbdbname;
    //QString evidremovestring;
    unsigned long long evidremoveid;
    unsigned long long evidrowsremoved;
    CaseObject caseobject;
    QStringList casenamelist;
    QStringList evidencenamelist;
    EvidenceObject evidenceobject;
    //QVector<EvidenceObject> evidenceobjectvector;
    SelectedObject selectedobject;
    //FileExportData exportdata;
    //QVector<FileExportData> exportdatavector;
    //QString htmlcontent;
    //QSqlDatabase casedb;
    //QSqlDatabase appdb;
    //QList<QSqlRecord> sqlrecords;
    //QList<QVariant> bindvalues;
};

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

Q_DECLARE_METATYPE(WombatVariable);
Q_DECLARE_METATYPE(FileExportData);
Q_DECLARE_METATYPE(FileDeepData);
/*
Q_DECLARE_METATYPE(AdsProcessObject);
Q_DECLARE_METATYPE(SecondaryProcessObject);
*/
Q_DECLARE_METATYPE(CaseObject);
Q_DECLARE_METATYPE(EvidenceObject);
Q_DECLARE_METATYPE(SelectedObject);
Q_DECLARE_METATYPE(IMG_EWF_INFO);

#endif // WOMBATVARIABLE_H
