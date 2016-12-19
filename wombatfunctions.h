#ifndef WOMBATFUNCTIONS_H
#define WOMBATFUNCTIONS_H

// Copyright 2015 Pasquale J. Rinaldi, Jr.
// Distrubted under the terms of the GNU General Public License version 2

#include "wombatinclude.h"
#include "tsk_hfs.h"
//#include "wombatvariable.h"
#include "globals.h"

std::string GetTime(void);
char* TskTimeToStringUTC(time_t time, char buf[128]);
unsigned long long GetChildCount(QString filefilter);
QStringList GetChildFiles(QString filefilter);
//unsigned long long GetChildCount(int type, unsigned long long address, unsigned long long parimgid = 0, unsigned long long parfsid = 0);
bool FileExists(const std::string& filename);
bool ProcessingComplete(void);
//void ProcessFile(QVector<QString> tmpstrings, QVector<unsigned long long> tmpints, FileData tmpdata, bool adsbool); 
TSK_WALK_RET_ENUM FileEntries(TSK_FS_FILE* tmpfile, const char* tmppath, void* tmpptr);
void GenerateThumbnails(void);
QVariant HashFile(TSK_FS_FILE* tmpfile, unsigned long long objid);
//QVariant MagicFile(TSK_FS_FILE* tmpfile, unsigned long long objid);
//QVariant AlternateDataStreamMagicFile(TSK_FS_FILE* readfileinfo, unsigned long long adsobjid);
void ThumbFile(TSK_FS_FILE* tmpfile, unsigned long long objid);
//void BlockFile(TSK_FS_FILE* tmpfile, unsigned long long objid, QVector<unsigned long long> adsattrid);
//void AlternateDataStreamBlockFile(TSK_FS_FILE* tmpfile, QVector<unsigned long long> adsobjid, QVector<unsigned long long> adsattrid);
void WriteEvidenceProperties(TSK_IMG_INFO* curimginfo);
void PropertyFile(TSK_FS_FILE* tmpfile, unsigned long long objid, unsigned long long fsoffset, int blksize, unsigned long long parfsid);
void AlternateDataStreamPropertyFile(TSK_FS_FILE* tmpfile, QVector<unsigned long long> adsobjid, QVector<unsigned long long> adsattrid);
void LogMessage(QString message);
void cnid_to_array(uint32_t cnid, uint8_t array[4]);
TSK_WALK_RET_ENUM GetBlockAddress(TSK_FS_FILE* tmpfile, TSK_OFF_T off, TSK_DADDR_T addr, char* buf, size_t size, TSK_FS_BLOCK_FLAG_ENUM flags, void *ptr);
QString GetFilePermissions(TSK_FS_META* tmpmeta);
QImage MakeThumb(const QString &img);
void SqlMap(FileData &filedata);
void FileMap(FileData &filedata);
void InitializeEvidenceStructure(WombatVariable &wombatvarvector);
QString GetFileSystemLabel(TSK_FS_INFO* fsinfo);
static TSK_OFF_T hfs_cat_get_record_offset(HFS_INFO* hfs, const hfs_btree_key_cat* needle);
uint8_t hfs_cat_read_thread_record(HFS_INFO* hfs, TSK_OFF_T off, hfs_thread* thread);
uint8_t hfs_cat_read_file_folder_record(HFS_INFO* hfs, TSK_OFF_T off, hfs_file_folder* record);
static int hfs_cat_compare_keys(HFS_INFO* hfs, const hfs_btree_key_cat* key1, const hfs_btree_key_cat* key2);
uint32_t hfs_convert_2_unix_time(uint32_t hfsdate);
/*
uint8_t hfs_cat_file_lookup(HFS_INFO* hfs, TSK_INUM_T inum, HFS_ENTRY* entry, unsigned char follow_hard_link);
uint8_t hfs_UTF16toUTF8(TSK_FS_INFO* fs, uint8_t* uni, int ulen, char* asc, int alen, uint32_t flags);
static TSK_OFF_T hfs_cat_get_record_offset(HFS_INFO* hfs, const hfs_btree_key_cat* needle);
uint8_t hfs_cat_read_thread_record(HFS_INFO* hfs, TSK_OFF_T off, hfs_thread* thread);
uint8_t hfs_cat_read_file_folder_record(HFS_INFO* hfs, TSK_OFF_T off, hfs_file_folder* record);
static uint8_t hfs_cat_get_record_offset_cb(HFS_INFO* hfs, int8_t level_type, const void* targ_data, const hfs_btree_key_cat* cur_key, TSK_OFF_T key_off, void* ptr);
static int hfs_cat_compare_keys(HFS_INFO* hfs, const hfs_btree_key_cat* key1, const hfs_btree_key_cat* key2);
static uint8_t hfs_cat_traverse(HFS_INFO* hfs, const void* targ_data, TSK_HFS_BTREE_CB a_cb, void* ptr);
*/

#endif // wombatfunctions.h
