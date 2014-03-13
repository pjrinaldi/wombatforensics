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
    void OpenEvidenceImages(void); // might not need this functions, since re-opening a case can pull it's info from the db.
    void AddEvidenceNodes(void);
    void OpenVolumeSystem(void);
    void GetVolumeSystemName(void);
    void OpenPartitions(void);
    void OpenFiles(void);
    void AddPartitionNodes(int increment);
    void CloseInfoStructures(void);

    void GetBootCode(int idx);
    int DetermineVectorIndex(void);

private:
    WombatVariable* wombatptr;
};

#endif // WOMBATFRAMEWORK_H
