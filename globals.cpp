#include "globals.h"

QSqlDatabase fcasedb;
QSqlQuery* fqueryptr = NULL;
QSqlDatabase thumbdb;
QSqlDatabase fappdb;
QFile logfile;
QTextStream msgstream;
QTextEdit* msglog = NULL;
QString fdbname = "t.db";
QThreadPool* threadpool = NULL;
QVector<QFuture<void> > threadvector;
QVector<FileData> filedatavector;
QFutureWatcher<void> filewatcher;
QFutureWatcher<void> thumbwatcher;
unsigned long long filesfound = 0;
unsigned long long filesprocessed = 0;
unsigned long long processphase = 0;
unsigned long long totalchecked = 0;
unsigned long long totalcount = 0;
unsigned long long currentevidenceid = 0;
unsigned long long currentfilesystemid = 0;
unsigned long long exportcount = 0;
unsigned long long errorcount = 0;
unsigned long long currentjobid = 0;
unsigned long long jumpoffset = 0;
int linefactor = 0;
int threadstarted = 0;
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
QMutex mutex;
FilterValues filtervalues;
