#include "globals.h"

QSqlDatabase fcasedb;
QString fdbname = "t.db";
QThreadPool* threadpool = NULL;
QVector<QFuture<void> > threadvector;
QFutureWatcher<void> filewatcher;
int filesfound = 0;
int filesprocessed = 0;
InterfaceSignals* InterfaceSignals::_instance = 0;
