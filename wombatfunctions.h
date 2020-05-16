#ifndef WOMBATFUNCTIONS_H
#define WOMBATFUNCTIONS_H

// Copyright 2013-2020 Pasquale J. Rinaldi, Jr.
// Distrubted under the terms of the GNU General Public License version 2

#include "wombatinclude.h"
#include "globals.h"
//#include "wombatvariable.h"
#include "tskcpp.h"
#include <liblnk.h>

#define TICKS_PER_SECOND 10000000
#define EPOCH_DIFFERENCE 11644473600LL
QString ConvertWindowsTimeToUnixTime(uint64_t input);
QString ParseLnkArtifact(QString lnkname, QString lnkid);
QString ParseI30Artifact(QString i30name, QString i30id);

std::string GetTime(void);
qint64 GetChildCount(QString filefilter);

int UpdateBookmarkItems(QString tagname);
void UpdateEvidenceList(void);

void AddItem(QString content, QString section);
void AddTLinkItem(int tagid, QString tagname);
void AddELinkItem(int evidid, QString evidname);
void AddEvidItem(QString content);
void RemoveTLinkItem(QString tagname);
void RemoveELinkItem(QString evidname);
void AddTagItem(int tagid, QString tagname);
void RemoveTagItem(QString tagname);
void RemoveEvidItem(QString evidname);
void UpdateTLinkItem(int tagid, QString oldname, QString tagname);
void UpdateTagItem(int tagid, QString oldname, QString tagname);
void AddFileItem(QString tagname, QString content);
void RemoveFileItem(QString fileid);
QStringList GetChildFiles(QString filefilter);
bool FileExists(const std::string& filename);
void GenerateThumbnails(QString thumbid);
void GenerateHash(QString itemid);
void GenerateVidThumbnails(QString thumbid);
void GenerateDigging(QString thumbid);
void TestCarving(QStringList plist, QStringList flist);
//void GenerateCarving(QString partid);
void LoadImagesHash(void);
void SaveImagesHash(void);
void PopulateTreeModel(QString evidstring);
void FileRecurse(QString partitionpath, QString paraddr, QString rootinum);
void AddFileData(QString file, QString partpath, QString rootinum);
QString GetBlockList(TSK_FS_FILE* tmpfile);
void WriteEvidenceProperties(TSK_IMG_INFO* curimginfo, QString evidencepath, QString imgfullpath);
void WriteVolumeProperties(TSK_VS_INFO* curvolinfo, QString volumepath);
void WriteFileSystemProperties(TSK_FS_INFO* curfsinfo, QString partitionpath);
void WriteFileProperties(TSK_FS_FILE* curfileinfo, QString partpath);
void WriteAlternateDataStreamProperties(TSK_FS_FILE* curfileinfo, QString adsname, QString attrid, QString partpath, bool isres);
//void LogMessage(QString message);
TSK_WALK_RET_ENUM GetBlockAddress(TSK_FS_FILE* tmpfile, TSK_OFF_T off, TSK_DADDR_T addr, char* buf, size_t size, TSK_FS_BLOCK_FLAG_ENUM flags, void *ptr);
QString GetFilePermissions(TSK_FS_META* tmpmeta);
QString ConvertGmtHours(int gmtvar);
void ProcessExport(QString curid);
void InitializeEvidenceStructure(QString evidstring);
int SegmentDigits(int);
QString GetFileSystemLabel(TSK_FS_INFO* fsinfo);
void PopulateCarvedFiles(QString cfile);
QString GenerateCategorySignature(const QMimeType mimetype);
void TransferThumbnails(QString thumbid, QString reppath);
void TransferFiles(QString thumbid, QString reppath);
void GenerateWombatCaseFile(void);
void RewriteSelectedIdContent(QModelIndex selectedid);
void ProcessDir(TSK_FS_INFO* fsinfo, TSK_STACK* stack, TSK_INUM_T dirinum, const char* path, int eint, int vint, int pint, QString partitionpath);
void ParseDir(TSK_FS_INFO* fsinfo, TSK_STACK* stack, TSK_INUM_T dirnum, const char* path, QString partitionpath);
QByteArray ReturnFileContent(QString objectid);
void InitializeHashList(void);
void InitializeTaggedList(void);
void SaveHashList(void);
void SaveTaggedList(void);
void SavePasswordList(void);
void InitializePasswordList(void);

#endif // wombatfunctions.h
