#ifndef GLOBALS_H
#define GLOBALS_H

#include "wombatinclude.h"

extern QSqlDatabase fcasedb;
extern QSqlDatabase logdb;
extern QString fdbname;
extern QThreadPool* threadpool;
extern QVector<QFuture<void> > threadvector;
extern QFutureWatcher<void> filewatcher;
extern int filesfound;
extern int filesprocessed;
extern int totalcount;
extern int totalchecked;
extern int currentevidenceid;
extern int exportcount;
extern int currentfilesystemid;
extern int errorcount;
extern int currentjobid;
extern QString currentevidencename;
extern QList<QVariant> colvalues;
extern QList<TskObject> curlist;
extern QList<FileSystemObject> fsobjectlist;
extern QStringList propertylist;
extern QString blockstring;
extern struct magic_set* magicptr;

struct FilterValues
{
    bool maxidbool;
    bool minidbool;
    int maxid;
    int minid;
    bool namebool;
    QString namefilter;
    bool pathbool;
    QString pathfilter;
    bool maxsizebool;
    bool minsizebool;
    int maxsize;
    int minsize;
};

extern FilterValues filtervalues;

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
        haschildren = false;
        childcount = 0;
        checkstate = 0;
        parentid = 0;
    };

    ~Node()
    {
        if(children.count() > 0)
            qDeleteAll(children);
    };

    QList<QVariant> nodevalues;
    Node* parent;
    QList<Node*> children;
    bool haschildren;
    int parentid;
    int childcount;
    int checkstate;
    bool HasChildren(void)
    {
        if(childcount > 0)
            return true;
        return false;
    };
    void RemoveChild(int idx)
    {
        children.removeAt(idx);
    };
    int GetChildRow(int curid)
    {
        for(int i=0; i < children.count(); i++)
        {
            if(curid == children.at(i)->nodevalues.at(0).toInt())
                return i;
        }
        return -1;
    };
};

extern Node* rootnode;
extern Node* dummynode;
extern Node* parentnode;
extern Node* currentnode;
extern Node* toplevelnode;

#endif
