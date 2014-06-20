#include "globals.h"

QSqlDatabase fcasedb;
QString fdbname = "t.db";
QThreadPool* threadpool = NULL;
QVector<QFuture<void> > threadvector;
QFutureWatcher<void> filewatcher;
int filesfound = 0;
int filesprocessed = 0;
int totalchecked = 0;
int totalcount = 0;
int currentevidenceid = 0;
InterfaceSignals* isignals = new InterfaceSignals();
Node* currentnode = 0;
Node* rootnode = 0;
Node* dummynode = 0;
Node* parentnode = 0;
QList<QVariant> colvalues;
QList<int> checkedids;
QList<int> listedids;
