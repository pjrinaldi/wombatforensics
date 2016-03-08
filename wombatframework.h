#ifndef WOMBATFRAMEWORK_H
#define WOMBATFRAMEWORK_H

// Copyright 2015 Pasquale J. Rinaldi, Jr.
// Distrubted under the terms of the GNU General Public License version 2

#include "wombatinclude.h"
#include "wombatfunctions.h"
#include "wombatvariable.h"

class WombatFramework : public QObject
{
    Q_OBJECT
public:
    WombatFramework(WombatVariable* wombatvariable);
    ~WombatFramework();

    /*
    void OpenEvidenceImage(void);
    void OpenVolumeSystem(void);
    void GetVolumeSystemName(void);
    void OpenPartitions(void);
    void OpenFiles(void);
    void CloseInfoStructures(void);
    */
private:
    //WombatVariable* wombatptr;
    //QFutureWatcher<void> secondwatcher;
};

//void SqlMap(FileData &filedata);

#endif // WOMBATFRAMEWORK_H
