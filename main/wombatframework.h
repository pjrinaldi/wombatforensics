#ifndef WOMBATFRAMEWORK_H
#define WOMBATFRAMEWORK_H

#include <stdlib.h>
#include <stdio.h>
#include <tsk/libtsk.h>
#include <QObject>
#include <QtConcurrent>

#include "wombatvariable.h"

class WombatFramework : public QObject
{
    Q_OBJECT
public:
    WombatFramework(WombatVariable* wombatvariable);
    ~WombatFramework();

    void BuildEvidenceModel(void);
    //TSK_IMG_INFO* OpenEvidenceImage(void);
    //void OpenEvidenceImages(void);
    void OpenEvidenceImages(ItemObject &itemobject);
    
private:
    WombatVariable* wombatptr;
};

#endif // WOMBATFRAMEWORK_H
