#ifndef WOMBATDATABASE_H
#define WOMBATDATABASE_H

#include <sqlite3.h>
#include <QMessageBox>
#include <QStringList>
#include <QString>
#include <string.h>
#include <cstring>
#include <sstream>
#include <iostream>
#include <stdio.h>
#include <sys/stat.h>
#include <vector>
#include <QtWidgets>
#include <QDir>
#include <QObject>

class WombatDatabase : public QObject
{
    Q_OBJECT
public:
    WombatDatabase();
    bool FileExists(const std::string& fileName);
    const char *CreateLogDB(QString dbname);
    const char *CreateCaseDB(QString dbname);
    const char *OpenCaseDB(QString dbname);
    const char *CloseCaseDB();
    ~WombatDatabase();
    int ReturnCaseCount(void);
    sqlite3* ReturnDB(QString dbname);
    int InsertCase(QString caseText);
    int InsertJob(int jobType, int caseID, int evidenceID);
    int InsertEvidence(QString evidenceName, QString evidenceFilePath, int caseID);
    int InsertObject(int caseid, int evidenceid, int fileid);
    void InsertMsg(int caseid, int evidenceid, int jobid, int msgtype, const char* msg);
    void UpdateJobEnd(int jobid, int filecount, int processcount);
    QStringList ReturnJobDetails(int jobid);
    QStringList ReturnCaseNameList(void);
    int ReturnCaseID(QString caseName);
    int ReturnObjectFileID(int objectid);
    int ReturnObjectEvidenceID(int objectid);
    QStringList ReturnCaseEvidence(int caseID);
    QStringList ReturnCaseActiveEvidence(int caseID);
    int ReturnJobCaseID(int jobid);
    int ReturnJobEvidenceID(int jobid);
    int ReturnObjectID(int caseid, int evidenceid, int fileid);
    int ReturnEvidenceID(QString evidencename);
    void RemoveEvidence(QString evidencename);
    QStringList ReturnCaseEvidenceID(int caseID);
    QStringList ReturnCaseEvidenceJobIdType(int caseid, QStringList evidenceidlist);
    QStringList ReturnCaseEvidenceAddJobID(int caseid, QStringList evidenceidlist);
    QStringList ReturnCaseEvidenceRemJobID(int caseid, QStringList evidenceidlist);
    QStringList ReturnMessageTableEntries(int caseID, int evidenceID, int jobID);
    QStringList ReturnEvidenceData(int evidenceid);
    std::string GetTime(void);
signals:
    void DisplayError(QString errorNumber, QString errorType, QString errorValue);
private:
    sqlite3 *wombatdb;
    sqlite3 *logdb;
    sqlite3_stmt *sqlstatement;
    sqlite3_stmt *logstatement;
};

#endif // WOMBATDATABASE_H
