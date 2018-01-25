#ifndef GLOBALS_H
#define GLOBALS_H

#include "wombatinclude.h"
#include "wombatvariable.h"

// Copyright 2015 Pasquale J. Rinaldi, Jr.
// Distrubted under the terms of the GNU General Public License version 2

extern WombatVariable wombatvariable;
extern QFile logfile;
extern QFile viewerfile;
extern QFile treefile;
extern QDir thumbdir;
extern QTextEdit* msglog;
extern QFutureWatcher<void> thumbwatcher;
extern QFutureWatcher<void> secondwatcher;
extern unsigned long long filesfound;
extern unsigned long long filesprocessed;
extern unsigned long long fileschecked;
extern unsigned long long processphase;
extern unsigned long long totalcount;
extern unsigned long long totalchecked;
extern unsigned long long exportcount;
extern unsigned long long digcount;
extern unsigned long long errorcount;
extern unsigned long long jumpoffset;
extern unsigned long long filejumpoffset;
extern int partint;
extern int evidcnt;
extern int volcnt;
extern int childcount;
extern int linefactor;
extern int filelinefactor;
extern int thumbsize;
extern int mftrecordsize;
extern QList<QVariant> colvalues;
extern QStringList propertylist;
extern QStringList thumblist;
extern QStringList thumbpathlist;
extern QStringList exportlist;
extern QStringList digfilelist;
extern QString exportpath;
extern bool originalpath;
extern QString blockstring;
extern QString thumbpath;
extern QString hexselection;
extern QMap<QString, bool> checkhash;

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
    QString idfilter;
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
    QString filegroup;
    bool filegroupbool;
    bool hashbool;
    bool hashbool2;
    int hashdupcnt;
    QString hashfilter;
    QStringList hashlist;
};

extern FilterValues filtervalues;
class InterfaceSignals : public QObject
{
    Q_OBJECT
public:
    InterfaceSignals() { };
    ~InterfaceSignals() { };

    void ProgUpd(void) { emit(ProgressUpdate(filesfound)); }
    void ExportUpd(void) { emit(ExportUpdate()); }
    void DigUpd(void) { emit(DigUpdate()); }

signals:
    void ProgressUpdate(unsigned long long filecount);
    void ExportUpdate(void);
    void DigUpdate(void);

};

extern InterfaceSignals* isignals;


class TreeNode
{
public:
    explicit TreeNode(const QList<QVariant> &data, TreeNode* parent = 0, int itype = -1)
    {
        parentitem = parent;
        itemdata = data;
        itemtype = itype;
    };

    ~TreeNode()
    {
        qDeleteAll(childitems);
    }

    void AppendChild(TreeNode* child)
    {
        childitems.append(child);
    };

    TreeNode* child(int row)
    {
        return childitems.value(row);
    };

    int ChildCount(void) const
    {
        return childitems.count();
    };

    int ColumnCount(void) const
    {
        return itemdata.count();
    };

    QVariant Data(int column) const
    {
        return itemdata.value(column);
    };

    int Row(void) const
    {
        if(parentitem)
            return parentitem->childitems.indexOf(const_cast<TreeNode*>(this));

        return 0;
    };

    TreeNode* ParentItem(void)
    {
        return parentitem;
    };
    
    bool IsChecked() const
    {
        return checked;
    };

    void SetChecked(bool set)
    {
        checked = set;
    };

    int itemtype;
    bool checked = false;

private:
    QList<TreeNode*> childitems;
    QList<QVariant> itemdata;
    TreeNode* parentitem;
};

extern TSK_IMG_INFO* readimginfo;
extern TSK_VS_INFO* readvsinfo;
extern const TSK_VS_PART_INFO* readpartinfo;
extern TSK_FS_INFO* readfsinfo;
extern TSK_FS_FILE* readfileinfo;
extern char asc[512];
extern iso9660_pvd_node* p;
extern HFS_INFO* hfs;

#endif
