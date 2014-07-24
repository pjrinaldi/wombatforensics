#ifndef WOMBATFRAMEWORK_H
#define WOMBATFRAMEWORK_H

#include "wombatinclude.h"
#include "wombatfunctions.h"
#include "wombatvariable.h"

class WombatFramework : public QObject
{
    Q_OBJECT
public:
    WombatFramework(WombatVariable* wombatvariable);
    ~WombatFramework();

    void BuildEvidenceModel(void);
    void OpenEvidenceImage(void);
    void OpenVolumeSystem(void);
    void GetVolumeSystemName(void);
    void GetFileSystemProperties(void);
    void OpenPartitions(void);
    void OpenFiles(void);
    void CloseInfoStructures(void);
    //void GetBootCode(int idx);

private:
    WombatVariable* wombatptr;
};

#endif // WOMBATFRAMEWORK_H
