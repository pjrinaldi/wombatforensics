#ifndef TSKCPP_H
#define TSKCPP_H

// SLEUTHKIT FUNCTIONS NEEDED FROM HFS.C AND YAFFS.CPP WHICH ARE NOT IN ANY AVAIALBLE H FILE

#include "wombatinclude.h" // my include file code

void cnid_to_array(uint32_t cnid, uint8_t array[4]);
int hfs_cat_compare_keys(HFS_INFO* hfs, const hfs_btree_key_cat* key1, const hfs_btree_key_cat* key2);
TSK_OFF_T hfs_cat_get_record_offset(HFS_INFO* hfs, const hfs_btree_key_cat* needle);
uint8_t hfs_cat_read_thread_record(HFS_INFO* hfs, TSK_OFF_T off, hfs_thread* thread);
uint8_t hfs_cat_read_file_folder_record(HFS_INFO* hfs, TSK_OFF_T off, hfs_file_folder* record);
uint32_t hfs_convert_2_unix_time(uint32_t hfsdate);
void yaffscache_objects_stats(YAFFSFS_INFO* yfs, unsigned int* objcnt, uint32_t* objfirst, uint32_t* objlast, uint32_t* vercnt, uint32_t* verfirst, uint32_t* verlast);

// FUNCTION NEEDS TO BE CHECKED AND UPDATED WITH EACH NEW TSK RELEASE
typedef struct {
    const hfs_btree_key_cat *targ_key;
    TSK_OFF_T off;
} HFS_CAT_GET_RECORD_OFFSET_DATA;

#endif // tskcpp.h
