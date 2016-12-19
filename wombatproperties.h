#ifndef WOMBATPROPERTIES_H
#define WOMBATPROPERTIES_H

// Copyright 2015 Pasquale J. Rinaldi, Jr.
// Distrubted under the terms of the GNU General Public License version 2

#include "wombatinclude.h"
#include "wombatvariable.h"
#include "wombatfunctions.h"


    //WombatProperties(WombatVariable* wombatvarptr);
    QString GetFileSystemLabel(TSK_FS_INFO* curinfo);
    QStringList PopulateEvidenceImageProperties(void);
    QStringList PopulateVolumeProperties(void);
    QStringList PopulatePartitionProperties(void);
    QStringList PopulateFileSystemProperties(TSK_FS_INFO* curfsinfo);
    QStringList PopulateFileProperties(void);
    QString ConvertGmtHours(int gmtvar);
    void yaffscache_objects_stats(YAFFSFS_INFO* yfs, unsigned int* objcnt, uint32_t* objfirst, uint32_t* objlast, uint32_t* vercnt, uint32_t* verfirst, uint32_t* verlast);
    uint8_t hfs_UTF16toUTF8(TSK_FS_INFO* fs, uint8_t* uni, int ulen, char* asc, int alen, uint32_t flags);
    static int hfs_cat_compare_keys(HFS_INFO* hfs, const hfs_btree_key_cat* key1, const hfs_btree_key_cat* key2);
    uint32_t hfs_convert_2_unix_time(uint32_t hfsdate);
    uint8_t hfs_cat_file_lookup(HFS_INFO* hfs, TSK_INUM_T inum, HFS_ENTRY* entry, unsigned char follow_hard_link);
    static uint8_t hfs_cat_get_record_offset_cb(HFS_INFO* hfs, int8_t level_type, const void* targ_data, const hfs_btree_key_cat* cur_key, TSK_OFF_T key_off, void* ptr);
    static TSK_OFF_T hfs_cat_get_record_offset(HFS_INFO* hfs, const hfs_btree_key_cat* needle);
    uint8_t hfs_cat_read_thread_record(HFS_INFO* hfs, TSK_OFF_T off, hfs_thread* thread);
    uint8_t hfs_cat_read_file_folder_record(HFS_INFO* hfs, TSK_OFF_T off, hfs_file_folder* record);

    WombatVariable* wombatptr;
    QStringList proplist;
    IMG_EWF_INFO* ewfinfo;
    uint8_t* ewfvalue;
    uint8_t uvalue8bit;
    int8_t value8bit;
    uint32_t value32bit;
    uint64_t value64bit;
    size64_t size64bit;
    libewf_error_t* ewferror;
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
    EXFATFS_MASTER_BOOT_REC* exfatsb;
    HFS_INFO* hfs;
    hfs_plus_vh* hsb;
    ISO_INFO* iso;
    iso9660_pvd_node* p;
    iso9660_svd_node* s;
    YAFFSFS_INFO* yfs;
    EXT2FS_INFO* ext2fs;
    char asc[512];
    char asc128[129];
    char timebuf[128];

#endif // WOMBATPROPERTIES_H
