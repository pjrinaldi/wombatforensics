#ifndef WOMBATFRAMEWORK_H
#define WOMBATFRAMEWORK_H

#include <stdlib.h>
#include <stdio.h>
#include <tsk/libtsk.h>
#include <QObject>

#include "wombatvariable.h"

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
