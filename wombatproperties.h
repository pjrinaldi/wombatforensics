#ifndef WOMBATPROPERTIES_H
#define WOMBATPROPERTIES_H

#include "wombatinclude.h"
#include "wombatvariable.h"
#include "wombatfunctions.h"

class WombatProperties : public QObject
{
    Q_OBJECT
public:
    WombatProperties(WombatVariable* wombatvarptr);
    QString GetFileSystemLabel(TSK_FS_INFO* curinfo);
    QStringList PopulateEvidenceImageProperties(void);
    QStringList PopulateVolumeProperties(void);
    QStringList PopulatePartitionProperties(void);
    QStringList PopulateFileSystemProperties(TSK_FS_INFO* curfsinfo);
    QStringList PopulateFileProperties(void);

private:
    WombatVariable* wombatptr;
    QStringList proplist;
    IMG_AFF_INFO* affinfo;
    IMG_EWF_INFO* ewfinfo;
    uint8_t* ewfvalue;
    uint8_t uvalue8bit;
    int8_t value8bit;
    uint32_t value32bit;
    uint64_t value64bit;
    size64_t size64bit;
    libewf_error_t* ewferror;
    /*
    TSK_FS_FILE* tmpfile = NULL;
    */
    FFS_INFO* ffs;
    ffs_sb1* sb1;
    ffs_sb2* sb2;
    mac_part* macpart;
    bsd_disklabel* bsdpart;
    sun_dlabel_sparc* sunsparcpart;
    sun_dlabel_i386* sunx86part;
    gpt_head* gptpart;
    FATFS_INFO* fatfs;
    FATXXFS_SB* fatsb;
    NTFS_INFO* ntfsinfo;
    ntfs_sb* ntfssb;
    /*
    FATXXFS_DENTRY* tmpfatdentry = NULL;
    FATXXFS_DENTRY* curentry = NULL;
    const TSK_FS_ATTR*tmpattr;
    TSK_DADDR_T cursector = 0;
    TSK_DADDR_T endsector = 0;
    int8_t isallocsec = 0;
    TSK_INUM_T curinum = 0;
    */
    //FATFS_DENTRY* dentry = NULL;
    EXT2FS_INFO* ext2fs;
    /*
    ISO_INFO* iso = NULL;
    iso9660_pvd_node* p = NULL;
    iso9660_svd_node* s = NULL;
    HFS_INFO* hfs = NULL;
    hfs_plus_vh* hsb = NULL;
    char fn[HFS_MAXNAMLEN + 1];
    HFS_ENTRY* hfsentry = NULL;
    hfs_btree_key_cat key;
    hfs_thread thread;
    hfs_file_folder record;
    TSK_OFF_T off;
    char* databuffer = NULL;
    ssize_t cnt;
    ssize_t bytesread = 0;
    int a;
    uint len = 0;
    */
    char asc[512];
    char timebuf[128];

};

#endif // WOMBATPROPERTIES_H
