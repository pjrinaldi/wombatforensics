#ifndef NEWCASE_H
#define NEWCASE_H

// Copyright 2013-2020 Pasquale J. Rinaldi, Jr.
// Distrubted under the terms of the GNU General Public License version 2

#include "wombatinclude.h"
#include "globals.h"
#include "tskcpp.h"
#include "properties.h"
#include "wombatfunctions.h"

void ProcessVolume(QString evidstring);
void ParseVolume(QString estring, qint64 imgsize, QList<qint64> *offlist, QList<qint64> *sizlist, QList<QHash<QString, QVariant>> *fsinfolist);
void ParseExtendedPartition(QString estring, uint32_t primaryextendedoffset, uint32_t offset, uint32_t size, QList<qint64>* offlist, QList<qint64>* sizelist, QList<QHash<QString, QVariant>>* fsinfolist);
void ParseFileSystemInformation(QString estring, off64_t offset, QList<QHash<QString, QVariant>> *fsinfolist);
void ParseDirectory(QString estring, quint64 diroffset, uint64_t dirsize, QHash<QString, QVariant> *fsinfo, QList<QHash<QString, QVariant>> *fileinfolist, QList<QString>* orphanlist);
void ParseSubDirectory(QString estring, QHash<QString, QVariant>* fsinfo, QHash<QString, QVariant>* parfileinfo, QList<QHash<QString, QVariant>>* fileinfolist, uint* inodecnt, QByteArray* fatbuf, QList<QString>* orphanlist);
void GetNextCluster(uint clusternum, QString typestr, QByteArray* fatbuf, QList<uint>* clusterlist);
//void PopulateFileSystem();
void WriteFileSystemProperties(QHash<QString, QVariant>* fsinfo, QString pathstring);
void AddVirtualFileSystemFiles(QHash<QString, QVariant>* fsinfo, int* curinode, QString partpath, QString parentstr);
void PopulateFiles(QString emntstring, QString curpartpath, QHash<QString, QVariant>* fsinfo, QList<QHash<QString, QVariant>>* fileinfolist, QList<QString>* orphanlist, int eid, int pid);

void InitializeEvidenceStructure(QString evidstring);
void ProcessDir(TSK_FS_INFO* fsinfo, TSK_STACK* stack, TSK_INUM_T dirinum, const char* path, int eint, int vint, int pint, QString partitionpath);

#endif // newcase.h
