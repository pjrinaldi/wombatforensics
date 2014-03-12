#ifndef WOMBATDATABASE_H
#define WOMBATDATABASE_H

#include "wombatinclude.h"
#include "wombatvariable.h"
#include "wombatfunctions.h"

class WombatDatabase : public QObject
{
    Q_OBJECT
public:
    WombatDatabase(WombatVariable* wombatvarptr);
    void CreateAppDB(void);
    void CreateCaseDB(void);
    void OpenCaseDB(void);
    void OpenAppDB(void);
    void CloseAppDB(void);
    void InsertEvidenceObject(void);
    void GetEvidenceObject(void);
    void GetEvidenceObjects(void);
    void InsertVolumeObject(void);
    void GetVolumeObjects(void);
    void InsertPartitionObjects(void);
    void GetPartitionObjects(void);
    void InsertFileSystemObjects(void);
    void InsertFileObjects(void);
    void GetFileSystemObjects(void);
    void GetObjectType(void);

    ~WombatDatabase();

    int ReturnCaseCount(void);
    void InsertCase();
    /*
    int InsertJob(int jobType, int caseID, int evidenceID);
    int InsertEvidence(QString evidenceName, QString evidenceFilePath, int caseID);
    int InsertObject(int caseid, int evidenceid, int itemtype, int curid);
    void InsertMsg(int caseid, int evidenceid, int jobid, int msgtype, const char* msg);
    void UpdateJobEnd(int jobid, int filecount, int processcount);
    QStringList ReturnJobDetails(int jobid);
    */
    void ReturnCaseNameList(void);
    void ReturnCaseID(void);
    /*
    QStringList ReturnCaseActiveEvidenceID(int caseid);
    int ReturnObjectFileID(int objectid);
    int ReturnObjectEvidenceID(int objectid);
    QStringList ReturnCaseActiveEvidence(int caseID);
    int ReturnJobCaseID(int jobid);
    int ReturnJobEvidenceID(int jobid);
    int ReturnObjectID(int caseid, int evidenceid, int fileid);
    int ReturnEvidenceID(QString evidencename);
    int ReturnEvidenceDeletedState(int evidenceid);
    QString ReturnEvidencePath(int evidenceid);
    void RemoveEvidence(QString evidencename);
    QStringList ReturnCaseEvidenceIdJobIdType(int caseid);
    QStringList ReturnMessageTableEntries(int jobid);
    QStringList ReturnEvidenceData(int evidenceid);
    */

    QList<QSqlRecord> GetSqlResults(QString query, QVariantList invalues);
    int InsertSqlGetID(QString query, QVariantList invalues);
    void InsertSql(QString query, QVariantList invalues);
    void InsertSql(QString query);


signals:
    void DisplayError(QString errorNumber, QString errorType, QString errorValue);
private:
    WombatVariable* wombatptr;
};

#endif // WOMBATDATABASE_H
