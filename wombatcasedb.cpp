#include "wombatcasedb.h"

WombatCaseDb::WombatCaseDb()
{
}
// Function: fileExists
/**
    Check if a file exists
@param[in] filename - the name of the file to check

@return    true if the file exists, else false

*/
bool WombatCaseDb::FileExists(const std::string& filename)
{
    struct stat buf;
    if (stat(filename.c_str(), &buf) != -1)
    {
        return true;
    }
    return false;
}

void WombatCaseDb::DisplayError(QWidget *parent, QString errorNumber, QString errorType, QString errorValue)
{
    QString tmpString = errorNumber;
    tmpString += ". SqlError: ";
    tmpString += errorType;
    tmpString += " Returned ";
    tmpString += errorValue;
    //errlog->log(SqlErrLog::Error, tmpString.toStdString().c_str());
    QMessageBox::warning(parent, "Error", tmpString, QMessageBox::Ok);
}

const char* WombatCaseDb::CreateCaseDB(QString wombatdbname)
{
    std::vector<const char *> wombatTableSchema;
    wombatTableSchema.clear();
    wombatTableSchema.push_back("CREATE TABLE log(logid INTEGER PRIMARY KEY, logchannel INTEGER, logmessage TEXT);");
    wombatTableSchema.push_back("CREATE TABLE cases(caseid INTEGER PRIMARY KEY, casename TEXT);");
    wombatTableSchema.push_back("CREATE TABLE caseimages(imageid INTEGER PRIMARY KEY, imagefullpath TEXT, imagename TEXT, caseid INTEGER);");
    wombatTableSchema.push_back("CREATE TABLE settings(settingid INTEGER PRIMARY KEY, settingname TEXT, settingvalue TEXT);");

    if(sqlite3_open(wombatdbname.toStdString().c_str(), &wombatdb) == SQLITE_OK)
    {
        const char* tblString;
        foreach(tblString, wombatTableSchema)
        {
            if(sqlite3_prepare_v2(wombatdb, tblString, -1, &sqlstatement, NULL) == SQLITE_OK)
            {
                int ret = sqlite3_step(sqlstatement);
                if(ret != SQLITE_ROW && ret != SQLITE_DONE)
                {
                    return sqlite3_errmsg(wombatdb);
                }
            }
            else
            {
                return sqlite3_errmsg(wombatdb);
            }
        }
    }
    else
    {
        return sqlite3_errmsg(wombatdb);
    }

    return "";
}

WombatCaseDb::~WombatCaseDb()
{

}

int WombatCaseDb::ReturnCaseCount()
{
    int ret = 0;
    /*
    wombatSqlObject->OpenSql(wombatSqlObject->dbname, wombatSqlObject->sqldb);
    wombatSqlObject->PrepareSql("SELECT COUNT(caseid) FROM cases;");
    wombatSqlObject->StepSql();
    ret = wombatSqlObject->ReturnInt(0);
    wombatSqlObject->FinalizeSql();
    wombatSqlObject->CloseSql();
    */

    return ret;
}

int WombatCaseDb::InsertCase(QString caseText)
{
    int ret = 0;
    /*
    wombatSqlObject->OpenSql(wombatSqlObject->dbname, wombatSqlObject->sqldb);
    wombatSqlObject->PrepareSql("INSERT INTO cases (casename) VALUES(?);");
    wombatSqlObject->BindValue(1, caseText.toStdString().c_str());
    wombatSqlObject->StepSql();
    ret = wombatSqlObject->ReturnLastInsertRowID();
    wombatSqlObject->FinalizeSql();
    wombatSqlObject->CloseSql();
    */

    return ret;
}
