#ifndef GLOBALS_H
#define GLOBALS_H

#include "wombatinclude.h"

extern QSqlDatabase fcasedb;
extern QString fdbname;
extern QThreadPool* threadpool;
extern QVector<QFuture<void> > threadvector;
extern QFutureWatcher<void> filewatcher;
extern int filesfound;
extern int filesprocessed;

class InterfaceSignals : public QObject
{
    Q_OBJECT
public:
    InterfaceSignals() { };
    ~InterfaceSignals() { };
    //static void init() { if (_instance == 0) _instance = new InterfaceSignals(); }
    //static void term() { delete _instance; }
    //static InterfaceSignals* instance() { return _instance; }

    void ProgUpd(void) { emit(ProgressUpdate(filesfound, filesprocessed)); }

signals:
    void ProgressUpdate(int filecount, int processcount);

private:
    //InterfaceSignals(const InterfaceSignals&);
    //InterfaceSignals& operator=(const InterfaceSignals&);

    //static InterfaceSignals* _instance;
};

extern InterfaceSignals* isignals;
//extern InterfaceSignals* InterfaceSignals::_instance = 0;

#endif
