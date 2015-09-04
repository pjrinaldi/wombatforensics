#ifndef WOMBATDATABASE_H
#define WOMBATDATABASE_H

#include "wombatinclude.h"
#include "wombatvariable.h"
#include "wombatproperties.h"
#include "wombatfunctions.h"

class WombatDatabase : public QObject
{
    Q_OBJECT
public:
    WombatDatabase(WombatVariable* wombatvarptr);
    void CreateAppDB(void);
    void CreateCaseDB(void);
    //void CreateLogDB(void);
    //void CreateLogFile(void);
    void CreateThumbDB(void);
    void OpenCaseDB(void);
    void OpenAppDB(void);
    //void OpenLogDB(void);
    void OpenThumbDB(void);
    void CloseAppDB(void);
    void CloseCaseDB(void);
    //void CloseLogDB(void);
    void CloseThumbDB(void);
    void InsertEvidenceObject(void);
    void GetEvidenceObject(void);
    void GetEvidenceObjects(void);
    void InsertVolumeObject(void);
    void InsertPartitionObjects(void);
    void InsertFileSystemProperties(unsigned long long curfsid, TSK_FS_INFO* curfsinfo);
    void InsertEvidenceProperties(void);
    void InsertVolumeProperties(void);
    void GetObjectValues(void);
    void GetThumbnails(void);

    ~WombatDatabase();

    unsigned long long ReturnCaseCount(void);
    void InsertCase();
    void ReturnCaseNameList(void);
    void ReturnEvidenceNameList(void);
    void ReturnCaseID(void);
    void ReturnFileSystemObjectList(unsigned long long curid);
    void ReturnObjectPropertyList(void);
    void RemoveEvidence(void);

    QList<QSqlRecord> GetSqlResults(QString query, QVariantList invalues);
    unsigned long long InsertSqlGetID(QString query, QVariantList invalues);
    void InsertSql(QString query, QVariantList invalues);
    void InsertSql(QString query);
    unsigned long long ReturnSqlRowsAffected(QString query, QVariantList invalues);
    unsigned long long GetEvidenceFileCount(void);
    unsigned long long ReturnFileSystemOffset(unsigned long long fsid);
    unsigned long long GetResidentOffset(unsigned long long fileaddress);
    unsigned long long GetParentSize(unsigned long long fileaddress);

    WombatProperties* wombatprop;

signals:
    void DisplayError(QString errorNumber, QString errorType, QString errorValue);
private:
    WombatVariable* wombatptr;
};

#endif // WOMBATDATABASE_H
