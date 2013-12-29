#ifndef WOMBATFRAMEWORK_H
#define WOMBATFRAMEWORK_H

#include <stdlib.h>
#include <stdio.h>
#include <tsk/libtsk.h>
#include <QObject>

#include "wombatvariable.h"
// these files are included in libtsk.h
/*#include "tsk/base/tsk_base.h"
#include "tsk/img/tsk_img.h"
#include "tsk/vs/tsk_vs.h"
#include "tsk/fs/tsk_fs.h"
#include "tsk/hashdb/tsk_hashdb.h"
#include "tsk/auto/tsk_auto.h"*/

class WombatFramework : public QObject
{
    Q_OBJECT
public:
    WombatFramework(WombatVariable* wombatvariable);
    ~WombatFramework();

    void BuildEvidenceModel(void);
    
private:
    WombatVariable* wombatptr;

};

#endif // WOMBATFRAMEWORK_H
