#ifndef WOMBATFUNCTIONS_H
#define WOMBATFUNCTIONS_H

#include "wombatinclude.h"

std::string GetTime(void);
bool FileExists(const std::string& filename);

//template <typename Iterator>
//BytesToBinary(Iterator start, Iterator end);
/*
template<typename T>
void BytesToBinary(T start, T end)
{
    for(T it = start; it != end; ++it)
    {
        //qDebug() << "Byte to Hex: " << Translate::ByteToHex(it);
        cout << it;
    }
};*/
//QString ByteArrayToHex(QByteArray ba);
//QString ByteArrayToHexDisplay(QByteArray ba);
//QString ByteArrayToShortDisplay(QByteArray ba);
//QString ByteArrayToString(QByteArray ba, int base);
//QString SingleByteToString(QByteArray ba, int base);
//int CheckBitSet(unsigned char c, int n);
/*
 * 
 *class SqlObject
{
        public:
SqlObject(sqlite3_stmt* sqlStatement, const char* errorNumber);
SqlObject(sqlite3_stmt* sqlStatement, const char* errorNumber, sqlite3* openDB);
~SqlObject(void);
void SetSqlConstants(sqlite3* sqldb, sqlite3_stmt* sqlstatement);
*/
void PrepareSql(sqlite3* sqldb, sqlite3_stmt* sqlstatement, const char* sqlquery, const char* errornumber);
void BindValue(sqlite3_stmt* sqlstatement, int bindplace, int bindvalue);
/*
void                    BindValue(int bindPlace, double bindValue);
void                    BindValue(int bindPlace, int64 bindValue);
void                    BindValue(int bindPlace, const char* bindValue);
void                    BindValue(int bindPlace, const void* bindValue);// blob bind
void                    BindValue(int bindPlace);
int                             ReturnInt(int returnPlace);
double                  ReturnDouble(int returnPlace);
int64                   ReturnInt64(int returnPlace);
const char*             ReturnText(int returnPlace);
const void*             ReturnBlob(int returnPlace);
*/
void StepSql(sqlite3_stmt* sqlstatement, int &ret);
//void                    ClearBindings(void);
//int64                   ReturnLastInsertRowID(void);
//void ResetSql(void);
void FinalizeSql(sqlite3_stmt* sqlstatement);
/*void                    CloseSql(void);
sqlite3*                ReturnSqlDB(void);
private:
sqlite3*                sqldb;
const char*             sqlquery;
const char*             errornumber;
const char*             bindstring;
const char*             returnstring;
const void*             bindblob;
const void*             returnblob;
ErrorAlert*             ealert;
sqlite3_stmt*   sqlstatement;
int                             sqlcode;
int                             bindplace;
int                             returnplace;
int                             bindint;
int                             returnint;
double                  binddouble;
double                  returndouble;
int64                   bindint64;
int64                   returnint64;
BString                 tmpstring;
}; 
 * 
 */ 
#endif // wombatfunctions.h
