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
extern int currentevidenceid;
extern QList<QVariant> colvalues;

class InterfaceSignals : public QObject
{
    Q_OBJECT
public:
    InterfaceSignals() { };
    ~InterfaceSignals() { };

    void ProgUpd(void) { emit(ProgressUpdate(filesfound, filesprocessed)); }

signals:
    void ProgressUpdate(int filecount, int processcount);

};

extern InterfaceSignals* isignals;

class Node
{
public:
    Node(QList<QVariant> celldata)
    {
        nodevalues.clear();
        for(int i=0; i < celldata.count(); i++)
        {
            nodevalues.append(celldata.at(i));
        }
        parent = 0;
        fetchedchildren = false;
        haschildren = false;
    };

    ~Node()
    {
        qDeleteAll(children);
    };

    QList<QVariant> nodevalues;
    Node* parent;
    QList<Node*> children;
    bool fetchedchildren;
    bool haschildren;
    int childcount;
};

extern Node* rootnode;
extern Node* dummynode;
extern Node* parentnode;
extern Node* currentnode;

#endif
