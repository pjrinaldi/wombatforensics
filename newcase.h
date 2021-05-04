#ifndef NEWCASE_H
#define NEWCASE_H

// Copyright 2013-2020 Pasquale J. Rinaldi, Jr.
// Distrubted under the terms of the GNU General Public License version 2

#include "wombatinclude.h"
#include "globals.h"
#include "wombatfunctions.h"

void ProcessVolume(ForensicImage* tmpimage);
//void ProcessVolume(QString evidstring);
//void ParseVolume(QString estring, qint64 imgsize, QList<qint64> *offlist, QList<qint64> *sizlist, QList<QHash<QString, QVariant>> *fsinfolist);
void ParseVolume(ForensicImage* tmpimage, qint64 imgsize, QList<qint64> *offlist, QList<qint64> *sizlist, QList<QHash<QString, QVariant>> *fsinfolist);
void ParseExtendedPartition(ForensicImage* tmpimage, uint32_t primaryextendedoffset, uint32_t offset, uint32_t size, QList<qint64>* offlist, QList<qint64>* sizelist, QList<QHash<QString, QVariant>>* fsinfolist);
//void ParseExtendedPartition(QString estring, uint32_t primaryextendedoffset, uint32_t offset, uint32_t size, QList<qint64>* offlist, QList<qint64>* sizelist, QList<QHash<QString, QVariant>>* fsinfolist);
void ParseFileSystemInformation(ForensicImage* tmpimage, off64_t offset, QList<QHash<QString, QVariant>> *fsinfolist);
//void ParseFileSystemInformation(QString estring, off64_t offset, QList<QHash<QString, QVariant>> *fsinfolist);
void ParseFatDirEntry(QString estring, QHash<QString, QVariant> *fsinfo, QList<QHash<QString, QVariant>> *fileinfolist, QList<QHash<QString, QVariant>>* orphanlist);
void ParseExFatDirEntry(QString estring, QHash<QString, QVariant> *fsinfo, QList<QHash<QString, QVariant>> *fileinfolist, QList<QHash<QString, QVariant>>* orphanlist);
void ParseExfatSubDir(QString estring, QHash<QString, QVariant>* fsinfo, QHash<QString, QVariant>* parfileinfo, QList<QHash<QString, QVariant>>* fileinfolist, uint* inodecnt, QByteArray* fatbuf);
//void ParseDirectory(QString estring, QHash<QString, QVariant> *fsinfo, QList<QHash<QString, QVariant>> *fileinfolist, QList<QString>* orphanlist);
void ParseSubDirectory(QString estring, QHash<QString, QVariant>* fsinfo, QHash<QString, QVariant>* parfileinfo, QList<QHash<QString, QVariant>>* fileinfolist, uint* inodecnt, QByteArray* fatbuf, QList<QHash<QString, QVariant>>* orphanlist);
void ParseExtDirectory(QString estring, QHash<QString, QVariant>* fsinfo, QList<QHash<QString, QVariant>>* fileinfolist, QList<QHash<QString, QVariant>>* orphanlist, QHash<QString, QVariant>* parfileinfo, qulonglong curinode, qulonglong curicnt);

void GetMftEntryContent(QString estring, qulonglong ntinode, QHash<QString, QVariant>* fileinfo, QHash<QString, QVariant>* fsinfo, QList<QHash<QString, QVariant>>* adsinfolist);
void ParseNtfsDirectory(QString estring, QHash<QString, QVariant>* fsinfo, QList<QHash<QString, QVariant>>* fileinfolist, QList<QHash<QString, QVariant>>* orphanlist, QHash<QString, QVariant>* parfileinfo, qulonglong curmftentry, qulonglong curicnt);
void ParseMFT(QString estring, QHash<QString, QVariant>* fsinfo, QList<QHash<QString, QVariant>>* fileinfolist, QList<QHash<QString, QVariant>>* orphanlist);

void GetNextCluster(uint32_t clusternum, uint fstype, QByteArray* fatbuf, QList<uint>* clusterlist);
//void PopulateFileSystem();
void WriteFileSystemProperties(QHash<QString, QVariant>* fsinfo, QString pathstring);
void AddVirtualFileSystemFiles(QHash<QString, QVariant>* fsinfo, int* curinode, QString partpath, QString parentstr);
void PopulateFiles(QString emntstring, QString curpartpath, QHash<QString, QVariant>* fsinfo, QList<QHash<QString, QVariant>>* fileinfolist, QList<QHash<QString, QVariant>>* orphanlist, int eid, int pid);
void WriteFileProperties(QHash<QString, QVariant>*fileinfo, QString pathstring);

#endif // newcase.h
