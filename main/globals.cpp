#include "globals.h"

QSqlDatabase fcasedb;
QString fdbname = "t.db";
QThreadPool* threadpool = NULL;
QVector<QFuture<void> > threadvector;
