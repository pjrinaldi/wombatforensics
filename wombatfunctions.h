#ifndef WOMBATFUNCTIONS_H
#define WOMBATFUNCTIONS_H

// Copyright 2015 Pasquale J. Rinaldi, Jr.
// Distrubted under the terms of the GNU General Public License version 2

#include "wombatinclude.h"
#include "tsk_hfs.h"
#include "globals.h"

std::string GetTime(void);
char* TskTimeToStringUTC(time_t time, char buf[128]);
unsigned long long GetChildCount(QString filefilter);
QStringList GetChildFiles(QString filefilter);
bool FileExists(const std::string& filename);
//bool ProcessingComplete(void);
TSK_WALK_RET_ENUM FileEntries(TSK_FS_FILE* tmpfile, const char* tmppath, void* tmpptr);
void GenerateThumbnails(QString thumbid); // run function
//void GenerateThumbnails(void); // run function
QString GetBlockList(TSK_FS_FILE* tmpfile);
void WriteEvidenceProperties(TSK_IMG_INFO* curimginfo);
void WriteVolumeProperties(TSK_VS_INFO* curvolinfo);
void WriteFileSystemProperties(TSK_FS_INFO* curfsinfo);
void WriteFileProperties(TSK_FS_FILE* curfileinfo);
void WriteAlternateDataStreamProperties(TSK_FS_FILE* curfileinfo, QString adsname, QString fvalue, QString attrid);
void LogMessage(QString message);
void cnid_to_array(uint32_t cnid, uint8_t array[4]);
TSK_WALK_RET_ENUM GetBlockAddress(TSK_FS_FILE* tmpfile, TSK_OFF_T off, TSK_DADDR_T addr, char* buf, size_t size, TSK_FS_BLOCK_FLAG_ENUM flags, void *ptr);
QString GetFilePermissions(TSK_FS_META* tmpmeta);
QString ConvertGmtHours(int gmtvar);
void yaffscache_objects_stats(YAFFSFS_INFO* yfs, unsigned int* objcnt, uint32_t* objfirst, uint32_t* objlast, uint32_t* vercnt, uint32_t* verfirst, uint32_t* verlast);
void InitializeEvidenceStructure(int dumint);
void ProcessExport(QString curid);
//void InitializeEvidenceStructure(void);
QString GetFileSystemLabel(TSK_FS_INFO* fsinfo);
TSK_OFF_T hfs_cat_get_record_offset(HFS_INFO* hfs, const hfs_btree_key_cat* needle);
uint8_t hfs_cat_read_thread_record(HFS_INFO* hfs, TSK_OFF_T off, hfs_thread* thread);
uint8_t hfs_cat_read_file_folder_record(HFS_INFO* hfs, TSK_OFF_T off, hfs_file_folder* record);
int hfs_cat_compare_keys(HFS_INFO* hfs, const hfs_btree_key_cat* key1, const hfs_btree_key_cat* key2);
uint32_t hfs_convert_2_unix_time(uint32_t hfsdate);

#endif // wombatfunctions.h
