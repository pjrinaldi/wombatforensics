#include "globals.h"

QSqlDatabase fcasedb;
QString fdbname = "t.db";
QThreadPool* threadpool = NULL;
QVector<QFuture<void> > threadvector;
QFutureWatcher<void> filewatcher;
int filesfound = 0;
int filesprocessed = 0;
int currentevidenceid = 0;
InterfaceSignals* isignals = new InterfaceSignals();
