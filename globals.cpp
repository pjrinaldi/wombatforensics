#include "globals.h"

// Copyright 2013-2019 Pasquale J. Rinaldi, Jr.
// Distrubted under the terms of the GNU General Public License version 2

WombatVariable wombatvariable;
QFile logfile;
QFile viewerfile;
QFile settingsfile;
QDir thumbdir;
QTextEdit* msglog = NULL;
qint64 filesfound = 0;
qint64 fileschecked = 0;
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
int thumbsize = 320; // thumbnail size settings variable
int mftrecordsize = 1024;
int hashsum = 1;
int idcol = 0;
TreeNodeModel* treenodemodel = NULL;
TreeNode* selectednode = NULL;
InterfaceSignals* isignals = new InterfaceSignals();
QStringList propertylist;
QStringList exportlist;
QStringList digfilelist;
QStringList listeditems;
QStringList evidencelist;
QString casepath = QDir::homePath(); // case path settings variable
QString exportpath = "";
bool originalpath = true;
QString hexstring = "";
QHash<QString, bool> checkhash;
QHash<QString, QString> imageshash;
QMutex mutex;
FilterValues filtervalues;
