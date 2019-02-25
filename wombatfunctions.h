#ifndef WOMBATFUNCTIONS_H
#define WOMBATFUNCTIONS_H

// Copyright 2015 Pasquale J. Rinaldi, Jr.
// Distrubted under the terms of the GNU General Public License version 2

#include "wombatinclude.h"
#include "tsk_hfs.h"
#include "globals.h"

std::string GetTime(void);
unsigned long long GetChildCount(QString filefilter);
QStringList GetChildFiles(QString filefilter);
bool FileExists(const std::string& filename);
TSK_WALK_RET_ENUM RootEntries(TSK_FS_FILE* tmpfile, const char* tmppath, void* tmpptr);
TSK_WALK_RET_ENUM TreeEntries(TSK_FS_FILE* tmpfile, const char* tmppath, void* tmpptr);
void GenerateThumbnails(QString thumbid);
void GenerateHash(QString itemid);
void LoadImagesHash(void);
void PopulateTreeModel(QString evidstring);
void FileRecurse(QString partitionpath, QString paraddr);
QString GetBlockList(TSK_FS_FILE* tmpfile);
void WriteEvidenceProperties(TSK_IMG_INFO* curimginfo, QString evidencepath, QString imgfullpath);
void WriteVolumeProperties(TSK_VS_INFO* curvolinfo, QString volumepath);
void WriteFileSystemProperties(TSK_FS_INFO* curfsinfo, QString partitionpath);
void WriteFileProperties(TSK_FS_FILE* curfileinfo, AddEvidenceVariable* aevar);
void BuildStatFile(TSK_FS_FILE* curfileinfo, const char* tmppath, AddEvidenceVariable* aevar);
void WriteAlternateDataStreamProperties(TSK_FS_FILE* curfileinfo, QString adsname, unsigned long long adssize, QString attrid, AddEvidenceVariable* aevar);
//void LogMessage(QString message);
void cnid_to_array(uint32_t cnid, uint8_t array[4]);
TSK_WALK_RET_ENUM GetBlockAddress(TSK_FS_FILE* tmpfile, TSK_OFF_T off, TSK_DADDR_T addr, char* buf, size_t size, TSK_FS_BLOCK_FLAG_ENUM flags, void *ptr);
QString GetFilePermissions(TSK_FS_META* tmpmeta);
QString ConvertGmtHours(int gmtvar);
void yaffscache_objects_stats(YAFFSFS_INFO* yfs, unsigned int* objcnt, uint32_t* objfirst, uint32_t* objlast, uint32_t* vercnt, uint32_t* verfirst, uint32_t* verlast);
void ProcessExport(QString curid);
void InitializeEvidenceStructure(QString evidstring);
//void InitializeEvidenceStructure(void);
int SegmentDigits(int);
QString GetFileSystemLabel(TSK_FS_INFO* fsinfo);
TSK_OFF_T hfs_cat_get_record_offset(HFS_INFO* hfs, const hfs_btree_key_cat* needle);
uint8_t hfs_cat_read_thread_record(HFS_INFO* hfs, TSK_OFF_T off, hfs_thread* thread);
uint8_t hfs_cat_read_file_folder_record(HFS_INFO* hfs, TSK_OFF_T off, hfs_file_folder* record);
int hfs_cat_compare_keys(HFS_INFO* hfs, const hfs_btree_key_cat* key1, const hfs_btree_key_cat* key2);
uint32_t hfs_convert_2_unix_time(uint32_t hfsdate);

#endif // wombatfunctions.h
