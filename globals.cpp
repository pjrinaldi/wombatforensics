#include "globals.h"

// Copyright 2015 Pasquale J. Rinaldi, Jr.
// Distrubted under the terms of the GNU General Public License version 2

WombatVariable wombatvariable;
QFile logfile;
QFile viewerfile;
QFile settingsfile;
QFile treefile;
QDir thumbdir;
QDir currentrawimagedir;
QTextEdit* msglog = NULL;
QFutureWatcher<void> secondwatcher;
qint64 filesfound = 0;
qint64 fileschecked = 0;
qint64 processphase = 0;
qint64 totalchecked = 0;
qint64 totalcount = 0;
qint64 exportcount = 0;
qint64 dighashcount = 0;
qint64 digimgthumbcount = 0;
qint64 errorcount = 0;
qint64 jumpoffset = 0;
qint64 filejumpoffset = 0;
qint64 orphancount = 1;
int ecount = 0;
int childcount = 0;
int linefactor = 0;
int filelinefactor = 0;
int thumbsize = 320;
int mftrecordsize = 1024;
int hashsum = 1;
TreeNodeModel* treenodemodel = NULL;
TreeNode* selectednode = NULL;
InterfaceSignals* isignals = new InterfaceSignals();
QList<QVariant> colvalues;
QStringList propertylist;
QStringList thumblist;
QStringList thumbpathlist;
QStringList exportlist;
QStringList digfilelist;
QStringList listeditems;
QStringList evidencelist;
QString exportpath = "";
bool originalpath = true;
QString blockstring = "";
QString thumbpath = "";
QString hexselection = "";
QString hexstring = "";
QHash<QString, bool> checkhash;
QHash<QString, QString> imageshash;
QMutex mutex;
FilterValues filtervalues;
