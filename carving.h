#ifndef CARVING_H
#define CARVING_H

// Copyright 2013-2020 Pasquale J. Rinaldi, Jr.
// Distributed under the terms of the GNU General Public License version 2

#include "globals.h"
#include "videoviewer.h"
// LIBPOPPLER-QT5 HEADER
#include <poppler-qt5.h>

void GenerateCarving(QStringList plist, QStringList flist);
void PopulateCarvedFiles(QString cfile);
void GetCarvers(QStringList& ctypelist, QStringList flist); 
void GetPartitionValues(qint64& partoffset, qint64& blocksize, qint64& partsize, QString &imgfile, QString &imgpath, QString curpartid);
//void GetPartitionValues(qint64& partoffset, qint64& blocksize, qint64& partsize, ForImg* rawfile, QString curpartid);
//void GetPartitionValues(qint64& partoffset, qint64& blocksize, qint64& partsize, QFile& rawfile, QString curpartid);
void GetExistingCarvedFiles(QHash<int, QString>& headhash, qint64& blocksize, qint64& partoffset);
void FirstCarve(qint64& blockcount, QStringList& ctypelist, QList<int>& blocklist, QHash<int, QString>& headhash, ForImg* rawfile, qint64& blocksize, qint64& partoffset);
//void FirstCarve(qint64& blockcount, QStringList& ctypelist, QList<int>& blocklist, QHash<int, QString>& headhash, QFile& rawfile, qint64& blocksize, qint64& partoffset);
void SecondCarve(QList<int>& blocklist, QHash<int, QString>& headhash, qint64& blocksize, ForImg* rawfile, qint64& partoffset, qint64& blockcount, QByteArray& footerarray, QString& curplist);
//void SecondCarve(QList<int>& blocklist, QHash<int, QString>& headhash, qint64& blocksize, QFile& rawfile, qint64& partoffset, qint64& blockcount, QByteArray& footerarray, QString& curplist);
void HeaderSearch(int& j, QString carvetype, ForImg* rawfile, qint64& blocksize, qint64& partoffset, QList<int>& blocklist, QHash<int, QString>& headhash);
//void HeaderSearch(int& j, QString carvetype, QFile& rawfile, qint64& blocksize, qint64& partoffset, QList<int>& blocklist, QHash<int, QString>& headhash);
void FooterSearch(int& j, QString carvetype, ForImg* rawfile, qint64& blocksize, qint64& partoffset, QList<int>& blocklist, QHash<int, QString>& headhash);
//void FooterSearch(int& j, QString carvetype, QFile& rawfile, qint64& blocksize, qint64& partoffset, QList<int>& blocklist, QHash<int, QString>& headhash);
void HeaderFooterSearch(QString& carvetype, QList<int>& blocklist, int& j, qint64& blocksize, ForImg* rawfile, qint64& partoffset, qint64& blockcount, QByteArray& footerarray, qint64& carvedstringsize);
//void HeaderFooterSearch(QString& carvetype, QList<int>& blocklist, int& j, qint64& blocksize, QFile& rawfile, qint64& partoffset, qint64& blockcount, QByteArray& footerarray, qint64& carvedstringsize);
void FooterHeaderSearch(QString& carvetype, QList<int>& blocklist, int& j, qint64& blocksize, ForImg* rawfile, qint64& partoffset, qint64& blockcount, QByteArray& footerarray, qint64& carvedstringsize, qint64& byteoffset);
//void FooterHeaderSearch(QString& carvetype, QList<int>& blocklist, int& j, qint64& blocksize, QFile& rawfile, qint64& partoffset, qint64& blockcount, QByteArray& footerarray, qint64& carvedstringsize, qint64& byteoffset);
void ValidateCarvedFile(bool& isvalidfile, QByteArray& footerarray, QString& carvetype, qint64& carvedstringsize, QString& curplist);
void WriteCarvedFile(QString& curplist, qint64& carvedstringsize, qint64& blocksize, int& curblock, QString& curtypestr, bool& isvalidfile);

#endif // carving.h
