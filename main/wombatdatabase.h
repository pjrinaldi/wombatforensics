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

#include "wombatvariable.h"

class WombatDatabase : public QObject
{
    Q_OBJECT
public:
    WombatDatabase(WombatVariable* wombatvarptr);
    bool FileExists(const std::string& fileName);
    const char* CreateAppDB(QString dbname);
    void CreateAppDB(void);
    const char* CreateCaseDB(QString dbname);
    const char* OpenCaseDB(QString dbname);
    const char* OpenAppDB(QString dbname);
    const char* CloseAppDB();
    void InitializeEvidenceDatabase(void);
    ~WombatDatabase();
    int ReturnCaseCount(void);
    sqlite3* ReturnDB(QString dbname);
    int InsertCase(QString caseText);
    int InsertJob(int jobType, int caseID, int evidenceID);
    int InsertEvidence(QString evidenceName, QString evidenceFilePath, int caseID);
    int InsertObject(int caseid, int evidenceid, int itemtype, int curid);
    void InsertMsg(int caseid, int evidenceid, int jobid, int msgtype, const char* msg);
    void UpdateJobEnd(int jobid, int filecount, int processcount);
    QStringList ReturnJobDetails(int jobid);
    QStringList ReturnCaseNameList(void);
    int ReturnCaseID(QString caseName);
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
    std::string GetTime(void);
signals:
    void DisplayError(QString errorNumber, QString errorType, QString errorValue);
private:
    sqlite3* wombatdb;
    sqlite3* casedb;
    std::vector<sqlite3*> evidencedblist;
    sqlite3_stmt* casestatement;
    sqlite3_stmt* wombatstatement;
    WombatVariable* wombatptr;
};

#endif // WOMBATDATABASE_H
