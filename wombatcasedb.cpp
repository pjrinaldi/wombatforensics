#include "wombatcasedb.h"

WombatCaseDb::WombatCaseDb(QString wombatdbname)
{
    wombatSqlObject = new SqlWrapper(wombatdbname, wombatdb);
    wombatSqlObject->OpenCreateSql(wombatdbname, wombatdb);
}

WombatCaseDb::~WombatCaseDb()
{

}

int WombatCaseDb::ReturnCaseCount()
{
    int ret = 0;
    wombatSqlObject->OpenSql(wombatSqlObject->dbname, wombatSqlObject->sqldb);
    wombatSqlObject->PrepareSql("SELECT COUNT(caseid) FROM cases;");
    wombatSqlObject->StepSql();
    ret = wombatSqlObject->ReturnInt(0);
    wombatSqlObject->FinalizeSql();
    wombatSqlObject->CloseSql();

    return ret;
}

int WombatCaseDb::InsertCase(QString caseText)
{
    int ret = 0;
    wombatSqlObject->OpenSql(wombatSqlObject->dbname, wombatSqlObject->sqldb);
    wombatSqlObject->PrepareSql("INSERT INTO cases (casename) VALUES(?);");
    wombatSqlObject->BindValue(1, caseText.toStdString().c_str());
    wombatSqlObject->StepSql();
    ret = wombatSqlObject->ReturnLastInsertRowID();
    wombatSqlObject->FinalizeSql();
    wombatSqlObject->CloseSql();

    return ret;
}
