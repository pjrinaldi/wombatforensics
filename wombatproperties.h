#ifndef WOMBATPROPERTIES_H
#define WOMBATPROPERTIES_H

#include "wombatinclude.h"
#include "wombatvariable.h"
#include "wombatfunctions.h"

class WombatProperties : public QObject
{
    Q_OBJECT
public:
    WombatProperties(WombatVariable* wombatvarptr);
    QString GetFileSystemLabel(void);
    QStringList PopulateEvidenceImageProperties(void);
    QStringList PopulatePartitionProperties(void);
    QStringList PopulateFileSystemProperties(void);
    QStringList PopulateFileProperties(void);

private:
    WombatVariable* wombatptr;
    QStringList propertylist;
    IMG_AFF_INFO* affinfo;
    IMG_EWF_INFO* ewfinfo;
    uint8_t* ewfvalue;
    uint8_t uvalue8bit;
    int8_t value8bit;
    uint32_t value32bit;
    uint64_t value64bit;
    size64_t size64bit;
    libewf_error_t* ewferror;
};

#endif // WOMBATPROPERTIES_H
