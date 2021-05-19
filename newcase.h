#ifndef NEWCASE_H
#define NEWCASE_H

// Copyright 2013-2020 Pasquale J. Rinaldi, Jr.
// Distrubted under the terms of the GNU General Public License version 2

#include "wombatinclude.h"
#include "globals.h"
#include "wombatfunctions.h"

void ProcessForensicImage(ForImg* curimg);
QString ParseFileSystem(ForImg* curimg, uint32_t curstartsector, uint8_t ptreecnt);
void ParsePartition(ForImg* curimg, uint32_t cursectoroffset, uint32_t cursectorsize, uint8_t ptreecnt, uint8_t allocstatus);
uint8_t ParseExtendedPartition(ForImg* curimg, uint32_t curstartsector, uint32_t cursectorsize, uint8_t ptreecnt);


void ProcessVolume(ForImg* tmpimage);
//void ProcessVolume(ForensicImage* tmpimage);
//void ProcessVolume(QString evidstring);
//void ParseVolume(QString estring, qint64 imgsize, QList<qint64> *offlist, QList<qint64> *sizlist, QList<QHash<QString, QVariant>> *fsinfolist);
void ParseVolume(ForImg* tmpimage, qint64 imgsize, QList<qint64> *offlist, QList<qint64> *sizlist, QList<QHash<QString, QVariant>> *fsinfolist);
//void ParseVolume(ForensicImage* tmpimage, qint64 imgsize, QList<qint64> *offlist, QList<qint64> *sizlist, QList<QHash<QString, QVariant>> *fsinfolist);
void ParseExtendedPartition(ForImg* tmpimage, uint32_t primaryextendedoffset, uint32_t offset, uint32_t size, QList<qint64>* offlist, QList<qint64>* sizelist, QList<QHash<QString, QVariant>>* fsinfolist);
//void ParseExtendedPartition(ForensicImage* tmpimage, uint32_t primaryextendedoffset, uint32_t offset, uint32_t size, QList<qint64>* offlist, QList<qint64>* sizelist, QList<QHash<QString, QVariant>>* fsinfolist);
//void ParseExtendedPartition(QString estring, uint32_t primaryextendedoffset, uint32_t offset, uint32_t size, QList<qint64>* offlist, QList<qint64>* sizelist, QList<QHash<QString, QVariant>>* fsinfolist);
void ParseFileSystemInformation(ForImg* tmpimage, off64_t offset, QList<QHash<QString, QVariant>> *fsinfolist);
//void ParseFileSystemInformation(ForensicImage* tmpimage, off64_t offset, QList<QHash<QString, QVariant>> *fsinfolist);
//void ParseFileSystemInformation(QString estring, off64_t offset, QList<QHash<QString, QVariant>> *fsinfolist);
void ParseFatDirEntry(ForensicImage* tmpimage, QHash<QString, QVariant> *fsinfo, QList<QHash<QString, QVariant>> *fileinfolist, QList<QHash<QString, QVariant>>* orphanlist);
//void ParseFatDirEntry(QString estring, QHash<QString, QVariant> *fsinfo, QList<QHash<QString, QVariant>> *fileinfolist, QList<QHash<QString, QVariant>>* orphanlist);
void ParseExFatDirEntry(ForensicImage* tmpimage, QHash<QString, QVariant> *fsinfo, QList<QHash<QString, QVariant>> *fileinfolist, QList<QHash<QString, QVariant>>* orphanlist);
//void ParseExFatDirEntry(QString estring, QHash<QString, QVariant> *fsinfo, QList<QHash<QString, QVariant>> *fileinfolist, QList<QHash<QString, QVariant>>* orphanlist);
void ParseExfatSubDir(ForensicImage* tmpimage, QHash<QString, QVariant>* fsinfo, QHash<QString, QVariant>* parfileinfo, QList<QHash<QString, QVariant>>* fileinfolist, uint* inodecnt, QByteArray* fatbuf);
//void ParseExfatSubDir(QString estring, QHash<QString, QVariant>* fsinfo, QHash<QString, QVariant>* parfileinfo, QList<QHash<QString, QVariant>>* fileinfolist, uint* inodecnt, QByteArray* fatbuf);
//void ParseDirectory(QString estring, QHash<QString, QVariant> *fsinfo, QList<QHash<QString, QVariant>> *fileinfolist, QList<QString>* orphanlist);
void ParseSubDirectory(ForensicImage* tmpimage, QHash<QString, QVariant>* fsinfo, QHash<QString, QVariant>* parfileinfo, QList<QHash<QString, QVariant>>* fileinfolist, uint* inodecnt, QByteArray* fatbuf, QList<QHash<QString, QVariant>>* orphanlist);
//void ParseSubDirectory(QString estring, QHash<QString, QVariant>* fsinfo, QHash<QString, QVariant>* parfileinfo, QList<QHash<QString, QVariant>>* fileinfolist, uint* inodecnt, QByteArray* fatbuf, QList<QHash<QString, QVariant>>* orphanlist);
void ParseExtDirectory(ForensicImage* tmpimage, QHash<QString, QVariant>* fsinfo, QList<QHash<QString, QVariant>>* fileinfolist, QList<QHash<QString, QVariant>>* orphanlist, QHash<QString, QVariant>* parfileinfo, qulonglong curinode, qulonglong curicnt);
//void ParseExtDirectory(QString estring, QHash<QString, QVariant>* fsinfo, QList<QHash<QString, QVariant>>* fileinfolist, QList<QHash<QString, QVariant>>* orphanlist, QHash<QString, QVariant>* parfileinfo, qulonglong curinode, qulonglong curicnt);

void GetMftEntryContent(ForImg* tmpimage, qulonglong ntinode, QHash<QString, QVariant>* fileinfo, QHash<QString, QVariant>* fsinfo, QList<QHash<QString, QVariant>>* adsinfolist);
//void GetMftEntryContent(ForensicImage* tmpimage, qulonglong ntinode, QHash<QString, QVariant>* fileinfo, QHash<QString, QVariant>* fsinfo, QList<QHash<QString, QVariant>>* adsinfolist);
//void GetMftEntryContent(QString estring, qulonglong ntinode, QHash<QString, QVariant>* fileinfo, QHash<QString, QVariant>* fsinfo, QList<QHash<QString, QVariant>>* adsinfolist);
void ParseNtfsDirectory(ForImg* tmpimage, QHash<QString, QVariant>* fsinfo, QList<QHash<QString, QVariant>>* fileinfolist, QList<QHash<QString, QVariant>>* orphanlist, QHash<QString, QVariant>* parfileinfo, qulonglong curmftentry, qulonglong curicnt);
//void ParseNtfsDirectory(ForensicImage* tmpimage, QHash<QString, QVariant>* fsinfo, QList<QHash<QString, QVariant>>* fileinfolist, QList<QHash<QString, QVariant>>* orphanlist, QHash<QString, QVariant>* parfileinfo, qulonglong curmftentry, qulonglong curicnt);
//void ParseNtfsDirectory(QString estring, QHash<QString, QVariant>* fsinfo, QList<QHash<QString, QVariant>>* fileinfolist, QList<QHash<QString, QVariant>>* orphanlist, QHash<QString, QVariant>* parfileinfo, qulonglong curmftentry, qulonglong curicnt);
//void ParseMFT(QString estring, QHash<QString, QVariant>* fsinfo, QList<QHash<QString, QVariant>>* fileinfolist, QList<QHash<QString, QVariant>>* orphanlist);
void ParseMFT(ForImg* tmpimage, QHash<QString, QVariant>* fsinfo, QList<QHash<QString, QVariant>>* fileinfolist, QList<QHash<QString, QVariant>>* orphanlist);
//void ParseMFT(ForensicImage* tmpimage, QHash<QString, QVariant>* fsinfo, QList<QHash<QString, QVariant>>* fileinfolist, QList<QHash<QString, QVariant>>* orphanlist);

void GetNextCluster(uint32_t clusternum, uint fstype, QByteArray* fatbuf, QList<uint>* clusterlist);
//void PopulateFileSystem();
void WriteFileSystemProperties(QHash<QString, QVariant>* fsinfo, QString pathstring);
void AddVirtualFileSystemFiles(QHash<QString, QVariant>* fsinfo, int* curinode, QString partpath, QString parentstr);
void PopulateFiles(ForImg* tmpimage, QString curpartpath, QHash<QString, QVariant>* fsinfo, QList<QHash<QString, QVariant>>* fileinfolist, QList<QHash<QString, QVariant>>* orphanlist, int eid, int pid);
//void PopulateFiles(ForensicImage* tmpimage, QString curpartpath, QHash<QString, QVariant>* fsinfo, QList<QHash<QString, QVariant>>* fileinfolist, QList<QHash<QString, QVariant>>* orphanlist, int eid, int pid);
//void PopulateFiles(QString emntstring, QString curpartpath, QHash<QString, QVariant>* fsinfo, QList<QHash<QString, QVariant>>* fileinfolist, QList<QHash<QString, QVariant>>* orphanlist, int eid, int pid);
void WriteFileProperties(QHash<QString, QVariant>*fileinfo, QString pathstring);

#endif // newcase.h
