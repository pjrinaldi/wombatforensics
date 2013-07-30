#ifndef SQLWRAPPER_H
#define SQLWRAPPER_H

#include "sqlerrlog.h"

class SqlWrapper
{
public:
    //SqlWrapper(sqlite3_stmt* sqlStatement, const char* errorNumber, QString dbName);
    //SqlWrapper(QString dbName);
    //SqlWrapper(sqlite3_stmt *sqlStatement, QString dbName);
    //SqlWrapper();
    ~SqlWrapper();
    int OpenSql()
    int PrepareSql(const char *sqlQuery);
    //void PrepareSql(const char* sqlQuery);
    void BindValue(int bindPlace, int bindValue);
    void BindValue(int bindPlace, double bindValue);
    void BindValue(int bindPlace, sqlite3_int64 bindValue);
    void BindValue(int bindPlace, const char* bindValue);
    void BindValue(int bindPlace, const void* bindValue);// simple blob bind
    int BindValue(int bindPlace, const void* bindValue, int blobSize); // complex blob bind
    void BindValue(int bindPlace);
    int ReturnInt(int returnPlace);
    double ReturnDouble(int returnPlace);
    sqlite3_int64 ReturnInt64(int returnPlace);
    const char* ReturnText(int returnPlace);
    const void* ReturnText16(int returnPlace);
    const void* ReturnBlob(int returnPlace);
    int ReturnBlobSize(int returnPlace);
    int ReturnTable(char ***queryResults, int numRows, int numColumns, char **errMsg);
    int SetBusyHandler(int busyHandler(void *, int));
    int FileControl(int chunkSize);
    int StepSql(void);
    int ExecuteSql(char **errmsg);
    int ReturnColumnType(int returnPlace);
    void ClearBindings(void);
    sqlite3_int64 ReturnLastInsertRowID(void);
    void Free(void *varToFree);
    void FreeTable(char **queryResults);
    void ResetSql(void);
    void FinalizeSql(void);
    void PrepSql(void);
    void CloseSql(void);
    sqlite3* ReturnSqlDB(void);
    //void SetErrorLog(std::auto_ptr<Log> *errLog);
    //void SetErrorLog(SqlErrLog *errlog);
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
    int blobsize;
    int returnplace;
    int bindint;
    int returnint;
    double binddouble;
    double returndouble;
    sqlite3_int64 bindint64;
    sqlite3_int64 returnint64;
    QString tmpstring;
    QDir appDir;
    //std::auto_ptr<Log> *errlog;
    //SqlErrLog *errlog;

};

#endif // SQLWRAPPER_H
