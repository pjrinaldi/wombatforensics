#include "wombatfunctions.h"

std::string GetTime()
{
    struct tm *newtime;
    time_t aclock;

    time(&aclock);   // Get time in seconds
    newtime = localtime(&aclock);   // Convert time to struct tm form 
    char timeStr[64];
    snprintf(timeStr, 64, "%.2d/%.2d/%.2d %.2d:%.2d:%.2d",
        newtime->tm_mon+1,newtime->tm_mday,newtime->tm_year % 100, 
        newtime->tm_hour, newtime->tm_min, newtime->tm_sec);

    return timeStr;
}

bool FileExists(const std::string& filename)
{
    struct stat buf;
    if (stat(filename.c_str(), &buf) != -1)
    {
        return true;
    }
    return false;
}
/*
BytesToBinary(Iterator start, Iterator end)
{
    for(Iterator it = start; it != end; ++it)
    {
        qDebug() << "Byte to Hex: " << Translate::ByteToHex(it);
    }
}
*/
/*
QString ByteArrayToShortDisplay(QByteArray ba)
{
    short intvalue = 0;
    memcpy(&intvalue, &ba.begin()[0], sizeof(short));
    QString tmpstring = "";
    return tmpstring.setNum(intvalue);
}

QString SingleByteToString(QByteArray ba, int base)
{
    short intvalue = 0;
    memcpy(&intvalue, &ba.begin()[0], sizeof(short));
    QString tmpstring = "";
    tmpstring.setNum(intvalue, base);
    qDebug() << "Temp Value: " << tmpstring;
    if(base == 2 && tmpstring.size() < 8)
    {
    	int zerocount = 8 - tmpstring.size();
	tmpstring.prepend(QString::fromStdString(std::string(zerocount, '0')));
	return tmpstring;
    }
    else
    	return tmpstring;
}
// NEED TO FIX THE BYTE ARRAY TO STRING TO ACCOUNT FOR THE LENGTH OF THE BYTE ARRAY...
QString ByteArrayToString(QByteArray ba, int base)
{
    int intvalue = 0;
    memcpy(&intvalue, &ba.begin()[0], sizeof(int));
    QString tmpstring = "";
    tmpstring.setNum(intvalue, base);
    if(base == 2 && tmpstring.size() < 8)
    {
    	int zerocount = 8 - tmpstring.size();
	tmpstring.prepend(QString::fromStdString(std::string(zerocount, '0')));
	return tmpstring;
    }
    else
    	return tmpstring;
}

QString ByteArrayToHex(QByteArray ba)
{
    QString tmpstring = QString::fromUtf8(ba.toHex());
    QString outstring = "";
    for(int i=0; i < tmpstring.size() / 2; ++i)
    {
        outstring += tmpstring.at(2*i);
        outstring += tmpstring.at(2*i+1);
    }

    return outstring;
}

QString ByteArrayToHexDisplay(QByteArray ba)
{
    QString tmpstring = QString::fromUtf8(ba.toHex());
    QString outstring = "";
    for(int i=0; i < tmpstring.size()/2; ++i)
    {
        outstring += tmpstring.at(2*i);
        outstring += tmpstring.at(2*i+1);
        if(i < tmpstring.size()/2 - 1)
            outstring += " ";
    }

    return outstring;
}


// COMPARE THE STRING VALUE TO DETERMINE IF ITS SET TO 1 OR NOT.
int CheckBitSet(unsigned char c, int n)
{
    static unsigned char mask[] = {1, 2, 4, 8, 16, 32, 64, 128};
    //static unsigned char mask[] = {128, 64, 32, 16, 8, 4, 2, 1};
    return ((c & mask[n]) != 0);
}*/
/*
 * 
 * SqlObject::SqlObject(sqlite3_stmt* sqlStatement, const char* errorNumber, sqlite3* openDB)
{
        sqldb = openDB;
        sqlstatement = sqlStatement;
        errornumber = errorNumber;
}
SqlObject::SqlObject(sqlite3_stmt* sqlStatement, const char* errorNumber)
{
        sqldb = NULL;
        sqlstatement = sqlStatement;
        errornumber = errorNumber;
        char* sqlErrMsg;
        int     sqlValue;
        BString tmpString;
        sqlErrMsg = 0;
        BString tmpPath = GetAppDirPath(); // for testing purposes, use local one.
        //BString tmpPath = GetUserDirPath(); // for publish purposes, use real location
        if(tmpPath != "-15")
        {
                tmpPath += "/MasterPiece.db";
                sqlValue = sqlite3_open_v2(tmpPath, &sqldb, SQLITE_OPEN_READWRITE, NULL); // open db
                if(sqlite3_errcode(sqldb) == 14) // if error is SQLITE_CANTOPEN, then create db with structure
                {
                        sqlValue = sqlite3_open_v2(tmpPath, &sqldb, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL); 
                        if(sqlite3_errcode(sqldb) == 0) // sqlite_ok
                        {
                                tmpString = "CREATE TABLE ideatable(ideaid integer primary key autoincrement, ideaname text, ideatext text, ismp integer, mpid integer, ordernumber integer);";
                                sqlValue = sqlite3_exec(sqldb, tmpString, NULL, NULL, &sqlErrMsg);
                                if(sqlValue != SQLITE_OK) // if sql was not successful
                                {
                                        DisplayError("1.1", "OPEN", sqlErrMsg);
                                }
                        }
                        else // some kind of failure
                        {
                                DisplayError("1.0", "OPEN", sqlite3_errmsg(sqldb));
                        }
                }
                else if(sqlite3_errcode(sqldb) == 0) // sqlite_OK, it exists
                {
                        //no error, so i will return opendb at end;
                }
                else // if error is not ok or not existing, then display error in alert
                {
                        DisplayError("1.2", "OPEN", sqlite3_errmsg(sqldb));
                }
                sqlite3_free(sqlErrMsg);
        }
        else
        {
                DisplayError("1.0", "PATH", " to the User Directory not Found");
        }
}*/
void PrepareSql(sqlite3* sqldb, sqlite3_stmt* sqlstatement, const char* sqlquery, const char* errornumber)
{
    if(sqlite3_prepare_v2(sqldb, sqlquery, -1, &sqlstatement, NULL) != SQLITE_OK) // sql statement was not prepared
    {
        // log an error, or figure out how to call display error here...
        //DisplayError(errornumber, "PREPARE", sqlite3_errmsg(sqldb));
    }
}
void BindValue(sqlite3_stmt* sqlstatement, int bindplace, int bindvalue)
{
        if(sqlite3_bind_int(sqlstatement, bindplace, bindvalue) != SQLITE_OK) // sql int bind failed
        {
                //DisplayError(errornumber, "BIND INT", "MISUSE");
        }
}
/*
void SqlObject::BindValue(int bindPlace, double bindValue)
{
        bindplace = bindPlace;
        binddouble = bindValue;
        if(sqlite3_bind_double(sqlstatement, bindplace, binddouble) != SQLITE_OK) // sql double bind failed
        {
                DisplayError(errornumber, "BIND DOUBLE", "MISUSE");
        }
}
void SqlObject::BindValue(int bindPlace, int64 bindValue)
{
        bindplace = bindPlace;
        bindint64 = bindValue;
        if(sqlite3_bind_int64(sqlstatement, bindplace, bindint64) != SQLITE_OK) // sql int64 bind failed
        {
                DisplayError(errornumber, "BIND INT64", "MISUSE");
        }
}
void SqlObject::BindValue(int bindPlace, const char* bindValue)
{
        bindplace = bindPlace;
        bindstring = bindValue;
        if(sqlite3_bind_text(sqlstatement, bindplace, bindstring, -1, SQLITE_TRANSIENT) != SQLITE_OK) // sql text bind failed
        {
                DisplayError(errornumber, "BIND TEXT", "MISUSE");
        }
}
void SqlObject::BindValue(int bindPlace) // bind null
{
        bindplace = bindPlace;
        if(sqlite3_bind_null(sqlstatement, bindplace) != SQLITE_OK)
        {
                DisplayError(errornumber, "BIND NULL", "MISUSE");
        }
}
void SqlObject::BindValue(int bindPlace, const void* bindValue)
{
        bindplace = bindPlace;
        bindblob = bindValue;
        if(sqlite3_bind_blob(sqlstatement, bindplace, bindblob, -1, SQLITE_TRANSIENT) != SQLITE_OK) // sql blob bind failed
        {
                DisplayError(errornumber, "BIND BLOB", "MISUSE");
        }
}
int SqlObject::ReturnInt(int returnPlace)
{
        returnplace = returnPlace;
        return sqlite3_column_int(sqlstatement, returnplace);
}
double SqlObject::ReturnDouble(int returnPlace)
{
        returnplace = returnPlace;
        return sqlite3_column_double(sqlstatement, returnplace);
}
int64 SqlObject::ReturnInt64(int returnPlace)
{
        returnplace = returnPlace;
        return sqlite3_column_int64(sqlstatement, returnplace);
}
const char* SqlObject::ReturnText(int returnPlace)
{
        returnplace = returnPlace;
        return sqlite3_mprintf("%s", sqlite3_column_text(sqlstatement, returnplace));
}
const void* SqlObject::ReturnBlob(int returnPlace)
{
        returnplace = returnPlace;
        return sqlite3_column_blob(sqlstatement, returnplace);
}
int64 SqlObject::ReturnLastInsertRowID(void)
{
        return sqlite3_last_insert_rowid(sqldb);
}*/
void StepSql(sqlite3_stmt* sqlstatement, int &ret)
{
    ret = sqlite3_step(sqlstatement);
    if(ret != SQLITE_ROW && ret != SQLITE_DONE)
    {
        if(ret == SQLITE_BUSY) qDebug() << "log error"; //DisplayError(errornumber, "STEP", "BUSY");
        else if(ret == SQLITE_ERROR) qDebug() << "logerror"; //DisplayError(errornumber, "STEP", "ERROR");
        else if(ret == SQLITE_MISUSE) qDebug() << "log error"; //DisplayError(errornumber, "STEP", "MISUSE");
        else qDebug() << "logerror"; //DisplayError(errornumber, "STEP", "OTHER ISSUE");
    }
}
/*
void SqlObject::ClearBindings(void)
{
        if(sqlite3_clear_bindings(sqlstatement) != SQLITE_OK)
        {
                DisplayError(errornumber, "CLEAR", "ERROR");
        }
}
void SqlObject::ResetSql(void)
{
        if(sqlite3_reset(sqlstatement) != SQLITE_OK)
        {
                DisplayError(errornumber, "RESET", "ERROR");
        }
}*/
void FinalizeSql(sqlite3_stmt* sqlstatement)
{
    if(sqlite3_finalize(sqlstatement) != SQLITE_OK)
    {
        //DisplayError(errornumber, "FINALIZE", "ERROR");
    }
}
/*
void SqlObject::CloseSql(void)
{
        sqlcode = sqlite3_close(sqldb);
        if(sqlcode != SQLITE_OK)
        {
                if(sqlcode == SQLITE_BUSY) DisplayError(errornumber, "CLOSE", "BUSY");
                else if(sqlcode == SQLITE_ERROR) DisplayError(errornumber, "CLOSE", "ERROR");
                else DisplayError(errornumber, "CLOSE", "OTHER");
        }
}
sqlite3* SqlObject::ReturnSqlDB(void)
{
        return sqldb;
}
 * 
 */ 
