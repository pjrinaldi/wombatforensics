#include "globals.h"

QSqlDatabase fcasedb;
QSqlDatabase logdb;
QSqlDatabase thumbdb;
QSqlDatabase fappdb;
QString fdbname = "t.db";
QThreadPool* threadpool = NULL;
QVector<QFuture<void> > threadvector;
QFutureWatcher<void> filewatcher;
int filesfound = 0;
int filesprocessed = 0;
int totalchecked = 0;
int totalcount = 0;
int currentevidenceid = 0;
int currentfilesystemid = 0;
int exportcount = 0;
int errorcount = 0;
int currentjobid = 0;
int thumbsize = 16;
int mftrecordsize = 1024;
QString currentevidencename = "t.dd";
InterfaceSignals* isignals = new InterfaceSignals();
Node* currentnode = 0;
Node* rootnode = 0;
Node* dummynode = 0;
Node* parentnode = 0;
Node* toplevelnode = 0;
QList<QVariant> colvalues;
QList<TskObject> curlist;
QList<FileSystemObject> fsobjectlist;
QStringList propertylist;
QStringList thumblist;
QString blockstring = "";
QString thumbpath = "";
QString hexselection = "";
/*
#ifdef linux
    QString newline = "\n";
#endif
#ifdef WIN32
    QString newline = "\r\n";
#endif
#ifdef __APPLE__
    QString newline = "\n";
#endif
*/
QStringList externallist;
struct magic_set* magicptr = NULL;
struct magic_set* magicmimeptr = NULL;
FilterValues filtervalues;
