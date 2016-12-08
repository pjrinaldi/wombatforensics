#include "globals.h"

// Copyright 2015 Pasquale J. Rinaldi, Jr.
// Distrubted under the terms of the GNU General Public License version 2

//QSqlDatabase fcasedb;
//QSqlDatabase thumbdb;
//QSqlDatabase fappdb;
QFile logfile;
QTextEdit* msglog = NULL;
QVector<FileData> filedatavector;
QFutureWatcher<void> thumbwatcher;
QFutureWatcher<void> secondwatcher;
unsigned long long filesfound = 0;
unsigned long long filesprocessed = 0;
unsigned long long processphase = 0;
unsigned long long totalchecked = 0;
unsigned long long totalcount = 0;
unsigned long long currentevidenceid = 0;
unsigned long long currentfilesystemid = 0;
unsigned long long exportcount = 0;
unsigned long long digcount = 0;
unsigned long long errorcount = 0;
unsigned long long jumpoffset = 0;
unsigned long long filejumpoffset = 0;
int linefactor = 0;
int filelinefactor = 0;
int thumbsize = 64;
int mftrecordsize = 1024;
QString currentevidencename = "t.dd";
InterfaceSignals* isignals = new InterfaceSignals();
Node* currentnode = 0;
Node* rootnode = 0;
Node* dummynode = 0;
Node* parentnode = 0;
Node* toplevelnode = 0;
Node* actionnode = 0;
QList<QVariant> colvalues;
QList<TskObject> curlist;
QList<FileSystemObject> fsobjectlist;
QStringList propertylist;
QStringList thumblist;
QString blockstring = "";
QString thumbpath = "";
QString hexselection = "";
QStringList externallist;
QMutex mutex;
QMutex mutex2;
QMap<unsigned long long, int> checkhash;
FilterValues filtervalues;
//TSK_IMG_INFO* IMG_2ND_PROC = NULL;
