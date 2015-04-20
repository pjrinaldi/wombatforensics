#ifndef GLOBALS_H
#define GLOBALS_H

#include "wombatinclude.h"

extern QSqlDatabase fcasedb;
extern QSqlDatabase logdb;
extern QSqlDatabase thumbdb;
extern QSqlDatabase fappdb;
extern QString fdbname;
extern QThreadPool* threadpool;
extern QVector<QFuture<void> > threadvector;
extern QFutureWatcher<void> filewatcher;
extern QFutureWatcher<void> thumbwatcher;
extern unsigned long long filesfound;
extern unsigned long long filesprocessed;
extern unsigned long long totalcount;
extern unsigned long long totalchecked;
extern unsigned long long currentevidenceid;
extern unsigned long long exportcount;
extern unsigned long long currentfilesystemid;
extern unsigned long long errorcount;
extern unsigned long long currentjobid;
extern int thumbsize;
extern int mftrecordsize;
extern QString currentevidencename;
extern QList<QVariant> colvalues;
extern QList<TskObject> curlist;
extern QList<FileSystemObject> fsobjectlist;
extern QStringList propertylist;
extern QStringList thumblist;
extern QString blockstring;
extern QString thumbpath;
extern QString hexselection;
//extern QString newline;
extern QStringList externallist;
extern struct magic_set* magicptr;
extern struct magic_set* magicmimeptr;

struct dosdate
{
    int dyear;
    char dday;
    char dmon;
};

struct dostime
{
    unsigned char dmin;
    unsigned char dhour;
    unsigned char dhund;
    unsigned char dsec;
};

struct FilterValues
{
    bool maxidbool;
    bool minidbool;
    unsigned long long maxid;
    unsigned long long minid;
    bool namebool;
    QString namefilter;
    bool pathbool;
    QString pathfilter;
    bool maxsizebool;
    bool minsizebool;
    unsigned long long maxsize;
    unsigned long long minsize;
    bool maxcreatebool;
    bool mincreatebool;
    int maxcreate;
    int mincreate;
    bool maxaccessbool;
    bool minaccessbool;
    int maxaccess;
    int minaccess;
    bool maxmodifybool;
    bool minmodifybool;
    int maxmodify;
    int minmodify;
    bool maxchangebool;
    bool minchangebool;
    int maxchange;
    int minchange;
    bool filecategorybool;
    bool filetypebool;
    QString filecategory;
    QString filetype;
    bool hashbool;
    QStringList hashlist;
    QVector<unsigned long long> hashcount;
    QVector<unsigned long long> hashidlist;
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
    void ProgressUpdate(unsigned long long filecount, unsigned long long processcount);

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
    unsigned long long parentid;
    unsigned long long childcount;
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
    int GetChildRow(unsigned long long curid)
    {
        for(int i=0; i < children.count(); i++)
        {
            if(curid == children.at(i)->nodevalues.at(0).toULongLong())
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
