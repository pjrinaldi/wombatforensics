#include "wombatcasedb.h"

WombatCaseDb::WombatCaseDb(QString dbname)
{
    wombatSqlObject = new SqlWrapper(sqlStatement, "3.5", dbname);
}

WombatCaseDb::~WombatCaseDb()
{

}

int WombatCaseDb::ReturnCaseCount()
{
    int ret = 0;
    //wombatSqlObject->PrepSql();
    if(wombatSqlObject->PrepareSql("SELECT COUNT(caseid) FROM cases;") == SQLITE_OK)
    {
        if(wombatSqlObject->StepSql() == SQLITE_ROW)
            ret = wombatSqlObject->ReturnInt(0);
    }
    wombatSqlObject->FinalizeSql();

    return ret;
}
