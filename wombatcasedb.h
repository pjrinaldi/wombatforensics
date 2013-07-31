#ifndef WOMBATCASEDB_H
#define WOMBATCASEDB_H

#include "sqlwrapper.h"

class WombatCaseDb
{
public:
    WombatCaseDb(QString dbname);
    ~WombatCaseDb();
    int ReturnCaseCount(void);
    int InsertCase(QString caseText);

private:
    sqlite3 *wombatdb;
    sqlite3_stmt *sqlStatement;
    SqlWrapper *wombatSqlObject;
};

#endif // WOMBATCASEDB_H
