#ifndef WOMBATCASEDB_H
#define WOMBATCASEDB_H

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

class WombatCaseDb
{
public:
    WombatCaseDb(QWidget *Parent);
    bool FileExists(const std::string& fileName);
    void DisplayError(QWidget *parent, QString errorNumber, QString errorType, QString errorValue);
    const char *CreateLogDB(QString dbname);
    const char *CreateCaseDB(QString dbname);
    const char *OpenCaseDB(QString dbname);
    const char *CloseCaseDB();
    ~WombatCaseDb();
    int ReturnCaseCount(void);
    sqlite3* ReturnDB(QString dbname);
    int InsertCase(QString caseText);
    int InsertJob(int jobType, int caseID, int evidenceID);
    int InsertEvidence(QString evidenceName, QString evidenceFilePath, int caseID);
    QStringList ReturnCaseNameList(void);
    int ReturnCaseID(QString caseName);
    QStringList ReturnCaseEvidence(int caseID);
    QStringList ReturnCaseEvidenceID(int caseID);
    std::string GetTime(void);

private:
    sqlite3 *wombatdb;
    sqlite3 *logdb;
    QWidget *wombatparent;
    sqlite3_stmt *sqlstatement;
    sqlite3_stmt *logstatement;
};

#endif // WOMBATCASEDB_H
