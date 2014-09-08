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
