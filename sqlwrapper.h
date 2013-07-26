#ifndef SQLWRAPPER_H
#define SQLWRAPPER_H

#include <sqlite3.h>
#include <QString>
#include <QDir>
#include <QCoreApplication>

class SqlWrapper
{
public:
    //SqlWrapper(sqlite3_stmt* sqlStatement, const char* errorNumber, QString dbName);
    SqlWrapper(sqlite3_stmt* sqlStatement, const char* errorNumber);
    //SqlWrapper(QString dbName);
    SqlWrapper();
    ~SqlWrapper();

    void PrepareSql(const char* sqlQuery);
    void BindValue(int bindPlace, int bindValue);
    void BindValue(int bindPlace, double bindValue);
    void BindValue(int bindPlace, sqlite3_int64 bindValue);
    void BindValue(int bindPlace, const char* bindValue);
    void BindValue(int bindPlace, const void* bindValue);// blob bind
    void BindValue(int bindPlace);
    int ReturnInt(int returnPlace);
    double ReturnDouble(int returnPlace);
    sqlite3_int64 ReturnInt64(int returnPlace);
    const char* ReturnText(int returnPlace);
    const void* ReturnBlob(int returnPlace);
    int StepSql(void);
    void ClearBindings(void);
    sqlite3_int64 ReturnLastInsertRowID(void);
    void ResetSql(void);
    void FinalizeSql(void);
    void CloseSql(void);
    sqlite3* ReturnSqlDB(void);
    void DisplayError(const char* errorNumber, const char* errorType, const char* errorValue);

private:
    sqlite3* sqldb;
    const char* sqlquery;
    const char* errornumber;
    const char* bindstring;
    const char* returnstring;
    const void* bindblob;
    const void* returnblob;
    sqlite3_stmt* sqlstatement;
    int sqlcode;
    int bindplace;
    int returnplace;
    int bindint;
    int returnint;
    double binddouble;
    double returndouble;
    sqlite3_int64 bindint64;
    sqlite3_int64 returnint64;
    QString tmpstring;
    QDir appDir;

};

#endif // SQLWRAPPER_H
