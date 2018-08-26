#include "globals.h"

// Copyright 2015 Pasquale J. Rinaldi, Jr.
// Distrubted under the terms of the GNU General Public License version 2

WombatVariable wombatvariable;
QFile logfile;
QFile viewerfile;
QFile treefile;
QDir thumbdir;
QDir currentrawimagedir;
QTextEdit* msglog = NULL;
QFutureWatcher<void> thumbwatcher;
QFutureWatcher<void> secondwatcher;
unsigned long long filesfound = 0;
unsigned long long fileschecked = 0;
unsigned long long processphase = 0;
unsigned long long totalchecked = 0;
unsigned long long totalcount = 0;
unsigned long long exportcount = 0;
unsigned long long digcount = 0;
unsigned long long errorcount = 0;
unsigned long long jumpoffset = 0;
unsigned long long filejumpoffset = 0;
int partint = 0;
int volcnt = 0;
int evidcnt = 0;
int childcount = 0;
int linefactor = 0;
int filelinefactor = 0;
int thumbsize = 312;
int mftrecordsize = 1024;
InterfaceSignals* isignals = new InterfaceSignals();
QList<QVariant> colvalues;
QStringList propertylist;
QStringList thumblist;
QStringList thumbpathlist;
QStringList exportlist;
QStringList digfilelist;
QString exportpath = "";
bool originalpath = true;
QString blockstring = "";
QString thumbpath = "";
QString hexselection = "";
QString hexstring = "";
QHash<QString, bool> checkhash;
FilterValues filtervalues;
TSK_IMG_INFO* readimginfo = NULL;
TSK_VS_INFO* readvsinfo = NULL;
const TSK_VS_PART_INFO* readpartinfo = NULL;
TSK_FS_INFO* readfsinfo = NULL;
TSK_FS_FILE* readfileinfo = NULL;
char asc[512];
iso9660_pvd_node* p;
HFS_INFO* hfs = NULL;
