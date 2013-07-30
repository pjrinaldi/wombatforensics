#include "sqlwrapper.h"

//SqlWrapper::SqlWrapper()
SqlWrapper::SqlWrapper(QString dbName) // open to create db if it doesn't exist
{
    sqldb = NULL;
    int sqlValue = -1;
    char* sqlErrMsg;
    const char* createString;
    vector<const char*> createStrings;
    if(dbName == "WombatData.db")
    {
        createStrings.clear();
        createStrings.push_back("CREATE TABLE log(logid INTEGER PRIMARY KEY, logchannel INTEGER, logmessage TEXT);");
        createStrings.push_back("CREATE TABLE cases(caseid INTEGER PRIMARY KEY, casename TEXT);");
        createStrings.push_back("CREATE TABLE caseimages(imageid INTEGER PRIMARY KEY, imagefullpath TEXT, imagename TEXT, caseid INTEGER);");
    }
    else
    {
        createStrings.clear();
        createStrings.push_back("PRAGMA page_size=4096;");
        createStrings.push_back("PRAGMA synchronous=0;");
        createStrings.push_back("CREATE TABLE db_info(name TEXT PRIMARY KEY, version TEXT);");
        createStrings.push_back("CREATE TABLE image_info (type INTEGER, ssize INTEGER);");
        createStrings.push_back("CREATE TABLE image_names (seq INTEGER PRIMARY KEY, name TEXT);");
        createStrings.push_back("CREATE TABLE vol_info (vol_id INTEGER PRIMARY KEY, sect_start INTEGER NOT NULL, sect_len INTEGER NOT NULL, description TEXT, flags INTEGER);");
        createStrings.push_back("CREATE TABLE fs_info (fs_id INTEGER PRIMARY KEY, img_byte_offset INTEGER, vol_id INTEGER NOT NULL, fs_type INTEGER, block_size INTEGER, block_count INTEGER, root_inum INTEGER, first_inum INTEGER, last_inum INTEGER);");
        createStrings.push_back("CREATE TABLE files (file_id INTEGER PRIMARY KEY, type_id INTEGER, name TEXT, par_file_id INTEGER, dir_type INTEGER, meta_type INTEGER, dir_flags INTEGER, meta_flags INTEGER, size INTEGER, ctime INTEGER, crtime INTEGER, atime INTEGER, mtime INTEGER, mode INTEGER, uid INTEGER, gid INTEGER, status INTEGER, full_path TEXT);");
        createStrings.push_back("CREATE TABLE fs_files (file_id INTEGER PRIMARY KEY, fs_id INTEGER, fs_file_id INTEGER, attr_type INTEGER, attr_id INTEGER);");
        createStrings.push_back("CREATE TABLE fs_blocks (fs_id INTEGER NOT NULL, file_id INTEGER NOT NULL, seq INTEGER, blk_start INTEGER NOT NULL, blk_len INTEGER NOT NULL);");
        createStrings.push_back("CREATE TABLE carved_files (file_id INTEGER PRIMARY KEY, vol_id INTEGER);");
        createStrings.push_back("CREATE TABLE carved_sectors (file_id INTEGER, seq INTEGER, sect_start INTEGER, sect_len INTEGER);");
        createStrings.push_back("CREATE TABLE derived_files (file_id INTEGER PRIMARY KEY, derivation_details TEXT);");
        createStrings.push_back("CREATE TABLE alloc_unalloc_map (vol_id INTEGER, unalloc_img_id INTEGER, unalloc_img_sect_start INTEGER, sect_len INTEGER, orig_img_sect_start INTEGER);");
        createStrings.push_back("CREATE TABLE file_hashes (file_id INTEGER PRIMARY KEY, md5 TEXT, sha1 TEXT, sha2_256 TEXT, sha2_512 TEXT, known INTEGER);");
        createStrings.push_back("CREATE TABLE modules (module_id INTEGER PRIMARY KEY, name TEXT UNIQUE NOT NULL, description TEXT);");
        createStrings.push_back("CREATE TABLE module_status (file_id INTEGER, module_id INTEGER, status INTEGER, PRIMARY KEY (file_id, module_id));");
        createStrings.push_back("CREATE TABLE unalloc_img_status (unalloc_img_id INTEGER PRIMARY KEY, status INTEGER);");
        createStrings.push_back("CREATE TABLE unused_sectors (file_id INTEGER PRIMARY KEY, sect_start INTEGER, sect_len INTEGER, vol_id INTEGER);");
        createStrings.push_back("CREATE TABLE blackboard_artifacts (artifact_id INTEGER PRIMARY KEY, obj_id INTEGER NOT NULL, artifact_type_id INTEGER);");
        createStrings.push_back("CREATE TABLE blackboard_attributes (artifact_id INTEGER NOT NULL, source TEXT, context TEXT, attribute_type_id INTEGER NOT NULL, value_type INTEGER NOT NULL, value_byte BLOB, value_text TEXT, value_int32 INTEGER, value_int64 INTEGER, value_double NUMERIC(20, 10), obj_id INTEGER NOT NULL);");
        createStrings.push_back("CREATE TABLE blackboard_artifact_types (artifact_type_id INTEGER PRIMARY KEY, type_name TEXT, display_name TEXT);");
        createStrings.push_back("CREATE TABLE blackboard_attribute_types (attribute_type_id INTEGER PRIMARY KEY, type_name TEXT, display_name TEXT);");
        createStrings.push_back("CREATE INDEX attrs_artifact_id ON blackboard_attributes(artifact_id);");
        createStrings.push_back("CREATE INDEX attrs_attribute_type ON blackboard_attributes(attribute_type_id);");
        createStrings.push_back("CREATE INDEX attrs_obj_id ON blackboard_attributes(obj_id);");
    }
    sqlErrMsg = 0;
    QString tmpPath = QDir(QCoreApplication::applicationDirPath()).absolutePath();
    tmpPath += "/data/";
    tmpPath += dbName;
    LOGINFO(tmpPath.toStdString().c_str());
    sqlValue = sqlite3_open_v2(tmpPath.toStdString().c_str(), &sqldb, SQLITE_OPEN_READWRITE, NULL); // open db
    if(sqlValue == 14) // if error is SQLITE_CANTOPEN, then create db with structure
    {
            int sqlValue = sqlite3_open(tmpPath.toStdString().c_str(), &sqldb);
            if(sqlValue == 0) // sqlite_ok
            {
                foreach(createString, createStrings)
                {
                    sqlValue = sqlite3_exec(sqldb, createString, NULL, NULL, &sqlErrMsg);
                    if(sqlValue != SQLITE_OK) // if sql was not successful
                    {
                            DisplayError("1.1", "OPEN", sqlErrMsg);
                    }
                }
            }
            else // some kind of failure
            {
                    DisplayError("1.0", "OPEN", sqlite3_errmsg(sqldb));
            }
    }
    else if(sqlValue == 0) // sqlite_OK, it exists
    {
            //no error, so i will return opendb at end;
    }
    else // if error is not ok or not existing, then display error in alert
    {
            DisplayError("1.2", "OPEN", sqlite3_errmsg(sqldb));
    }
    sqlite3_free(sqlErrMsg);
}

//SqlWrapper::SqlWrapper(sqlite3_stmt* sqlStatement, const char* errorNumber)
SqlWrapper::SqlWrapper(sqlite3_stmt* sqlStatement, const char* errorNumber, QString dbName) // open to write data to the db.
{
        sqldb = NULL;
        sqlstatement = sqlStatement;
        errornumber = errorNumber;
        char* sqlErrMsg;
        int     sqlValue;
        sqlErrMsg = 0;
        QString tmpPath = QDir(QCoreApplication::applicationDirPath()).absolutePath();
        tmpPath += "/data/";
        tmpPath += dbName;
        sqlValue = sqlite3_open(tmpPath.toStdString().c_str(), &sqldb); // opendb
        if(sqlValue != 0)
            DisplayError("1.0", "OPEN", sqlite3_errmsg(sqldb));
        sqlite3_free(sqlErrMsg);
}
SqlWrapper::SqlWrapper(sqlite3_stmt* sqlStatement, QString dbName) // open to write data to the db.
{
        sqldb = NULL;
        sqlstatement = sqlStatement;
        char* sqlErrMsg;
        int     sqlValue;
        sqlErrMsg = 0;
        QString tmpPath = QDir(QCoreApplication::applicationDirPath()).absolutePath();
        tmpPath += "/data/";
        tmpPath += dbName;
        sqlValue = sqlite3_open(tmpPath.toStdString().c_str(), &sqldb); // opendb
        if(sqlValue != 0)
            DisplayError("1.0", "OPEN", sqlite3_errmsg(sqldb));
        sqlite3_free(sqlErrMsg);
}
SqlWrapper::~SqlWrapper(void)
{
}
int SqlWrapper::SetBusyHandler(int busyHandler(void *, int))
{
    return sqlite3_busy_handler(sqldb, busyHandler, sqldb);
}

int SqlWrapper::FileControl(int chunkSize)
{
    return sqlite3_file_control(sqldb, NULL, SQLITE_FCNTL_CHUNK_SIZE, &chunkSize);
}

int SqlWrapper::PrepareSql(const char *sqlQuery)
{
    sqlquery = sqlQuery;
    return sqlite3_prepare_v2(sqldb, sqlquery, -1, &sqlstatement, NULL);
}
/*
void SqlWrapper::PrepareSql(const char* sqlQuery)
{
        sqlquery = sqlQuery;
        if(sqlite3_prepare_v2(sqldb, sqlquery, -1, &sqlstatement, NULL) != SQLITE_OK) // sql statement was not prepared
        {
                DisplayError(errornumber, "PREPARE", sqlite3_errmsg(sqldb));
        }
}
*/
void SqlWrapper::BindValue(int bindPlace, int bindValue)
{
        bindplace = bindPlace;
        bindint = bindValue;
        if(sqlite3_bind_int(sqlstatement, bindplace, bindint) != SQLITE_OK) // sql int bind failed
        {
                DisplayError(errornumber, "BIND INT", "MISUSE");
        }
}
void SqlWrapper::BindValue(int bindPlace, double bindValue)
{
        bindplace = bindPlace;
        binddouble = bindValue;
        if(sqlite3_bind_double(sqlstatement, bindplace, binddouble) != SQLITE_OK) // sql double bind failed
        {
                DisplayError(errornumber, "BIND DOUBLE", "MISUSE");
        }
}
void SqlWrapper::BindValue(int bindPlace, sqlite3_int64 bindValue)
{
        bindplace = bindPlace;
        bindint64 = bindValue;
        if(sqlite3_bind_int64(sqlstatement, bindplace, bindint64) != SQLITE_OK) // sql int64 bind failed
        {
                DisplayError(errornumber, "BIND INT64", "MISUSE");
        }
}
void SqlWrapper::BindValue(int bindPlace, const char* bindValue)
{
        bindplace = bindPlace;
        bindstring = bindValue;
        if(sqlite3_bind_text(sqlstatement, bindplace, bindstring, -1, SQLITE_TRANSIENT) != SQLITE_OK) // sql text bind failed
        {
                DisplayError(errornumber, "BIND TEXT", "MISUSE");
        }
}
void SqlWrapper::BindValue(int bindPlace) // bind null
{
        bindplace = bindPlace;
        if(sqlite3_bind_null(sqlstatement, bindplace) != SQLITE_OK)
        {
                DisplayError(errornumber, "BIND NULL", "MISUSE");
        }
}
void SqlWrapper::BindValue(int bindPlace, const void* bindValue)
{
        bindplace = bindPlace;
        bindblob = bindValue;
        if(sqlite3_bind_blob(sqlstatement, bindplace, bindblob, -1, SQLITE_TRANSIENT) != SQLITE_OK) // sql blob bind failed
        {
                DisplayError(errornumber, "BIND BLOB", "MISUSE");
        }
}
int SqlWrapper::BindValue(int bindPlace, const void * bindValue, int blobSize)
{
    bindplace = bindPlace;
    bindblob = bindValue;
    blobsize = blobSize;
    return sqlite3_bind_blob(sqlstatement, bindplace, bindblob, blobsize, SQLITE_STATIC);
}

int SqlWrapper::ReturnInt(int returnPlace)
{
        returnplace = returnPlace;
        return sqlite3_column_int(sqlstatement, returnplace);
}
double SqlWrapper::ReturnDouble(int returnPlace)
{
        returnplace = returnPlace;
        return sqlite3_column_double(sqlstatement, returnplace);
}
sqlite3_int64 SqlWrapper::ReturnInt64(int returnPlace)
{
        returnplace = returnPlace;
        return sqlite3_column_int64(sqlstatement, returnplace);
}
const char* SqlWrapper::ReturnText(int returnPlace)
{
        returnplace = returnPlace;
        return sqlite3_mprintf("%s", sqlite3_column_text(sqlstatement, returnplace));
}
const void* SqlWrapper::ReturnText16(int returnPlace)
{
    returnplace = returnPlace;
    return sqlite3_column_text16(sqlstatement, returnplace);
}

const void* SqlWrapper::ReturnBlob(int returnPlace)
{
        returnplace = returnPlace;
        return sqlite3_column_blob(sqlstatement, returnplace);
}
int SqlWrapper::ReturnBlobSize(int returnPlace)
{
    returnplace = returnPlace;
    return sqlite3_column_bytes(sqlstatement, returnplace);
}

sqlite3_int64 SqlWrapper::ReturnLastInsertRowID(void)
{
        return sqlite3_last_insert_rowid(sqldb);
}
int SqlWrapper::StepSql(void)
{
        sqlcode = sqlite3_step(sqlstatement);
        if(sqlcode != SQLITE_ROW && sqlcode != SQLITE_DONE)
        {
                if(sqlcode == SQLITE_BUSY) DisplayError(errornumber, "STEP", "BUSY");
                else if(sqlcode == SQLITE_ERROR) DisplayError(errornumber, "STEP", "ERROR");
                else if(sqlcode == SQLITE_MISUSE) DisplayError(errornumber, "STEP", "MISUSE");
                else DisplayError(errornumber, "STEP", "OTHER ISSUE");
        }
        return sqlcode;
}

int SqlWrapper::ExecuteSql(char **errmsg)
{
    return sqlite3_exec(sqldb, sqlquery, NULL, NULL, errmsg);
}

int SqlWrapper::ReturnColumnType(int returnPlace)
{
    returnplace = returnPlace;
    return sqlite3_column_type(sqlstatement, returnplace);
}

int SqlWrapper::ReturnTable(char ***queryResults, int numRows, int numColumns, char **errMsg)
{
    return sqlite3_get_table(sqldb, sqlquery, queryResults, &numRows, &numColumns, errMsg);
}

void SqlWrapper::FreeTable(char **queryResults)
{
    sqlite3_free_table(queryResults);
}

void SqlWrapper::ClearBindings(void)
{
        if(sqlite3_clear_bindings(sqlstatement) != SQLITE_OK)
        {
                DisplayError(errornumber, "CLEAR", "ERROR");
        }
}

void SqlWrapper::Free(void*varToFree)
{
    sqlite3_free(varToFree);
}

void SqlWrapper::ResetSql(void)
{
        if(sqlite3_reset(sqlstatement) != SQLITE_OK)
        {
                DisplayError(errornumber, "RESET", "ERROR");
        }
}
void SqlWrapper::FinalizeSql(void)
{
        if(sqlite3_finalize(sqlstatement) != SQLITE_OK)
        {
                DisplayError(errornumber, "FINALIZE", "ERROR");
        }
}
void SqlWrapper::PrepSql(void)
{
    //ResetSql();
    //ClearBindings();
    //FinalizeSql();
}

void SqlWrapper::CloseSql(void)
{
        sqlcode = sqlite3_close(sqldb);
        if(sqlcode != SQLITE_OK)
        {
                if(sqlcode == SQLITE_BUSY) DisplayError(errornumber, "CLOSE", "BUSY");
                else if(sqlcode == SQLITE_ERROR) DisplayError(errornumber, "CLOSE", "ERROR");
                else DisplayError(errornumber, "CLOSE", "OTHER");
        }
}
sqlite3* SqlWrapper::ReturnSqlDB(void)
{
        return sqldb;
}
/*
void SqlWrapper::SetErrorLog(SqlErrLog *errLog)
{
    errlog = errLog;
}
*/
void SqlWrapper::DisplayError(const char* errorNumber, const char* errorType, const char* errorValue)
{
        QString tmpString = errorNumber;
        //ErrorAlert* ealert;
        tmpString += ". SqlError: ";
        tmpString += errorType;
        tmpString += " Returned ";
        tmpString += errorValue;
        //errlog->log(SqlErrLog::Error, tmpString.toStdString().c_str());
        fprintf(stderr, tmpString.toStdString().c_str());
        //ealert = new ErrorAlert(tmpString);
        //ealert->Launch();
}
