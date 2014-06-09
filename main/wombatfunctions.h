#ifndef WOMBATFUNCTIONS_H
#define WOMBATFUNCTIONS_H

#include "wombatinclude.h"
#include "globals.h"

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
    };

    ~Node()
    {
        qDeleteAll(children);
    };

    QList<QVariant> nodevalues;
    Node* parent;
    QList<Node*> children;
    bool HasChildren(void)
    {
        if(children.count() > 0)
            return true;
        return false;
    };
};

std::string GetTime(void);
char* TskTimeToStringUTC(time_t time, char buf[128]);
int FindParentNode(Node* curnode, Node* parentnode, int rootinum);
bool FileExists(const std::string& filename);
bool ProcessingComplete(void);
void ProcessFile(QVector<QString> tmpstrings, QVector<int> tmpints);
//void ProcessFile(TSK_FS_FILE* tmpfile, const char* tmppath, void* tmpptr);
TSK_WALK_RET_ENUM FileEntries(TSK_FS_FILE* tmpfile, const char* tmppath, void* tmpptr);

#endif // wombatfunctions.h
