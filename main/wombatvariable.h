#ifndef WOMBATVARIABLE_H
#define WOMBATVARIABLE_H

class WombatVariable
{
public:
    WombatVariable();
    int GetCaseID();
    int GetImageID();
    int GetAnalysisType();
    void SetCaseID(int caseID);
    void SetImageID(int imageID);
    void SetAnalysisType(int analysisType);
    ~WombatVariable();

private:
    int caseid;
    int imageid;
    int analysistype;
};
/*
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
    int64_t InsertCase(QString caseText);
    void InsertImage(QString imageName, QString imageFilePath, int caseID);
    QStringList ReturnCaseNameList(void);
    int ReturnCaseID(QString caseName);
    QStringList ReturnCaseImages(int caseID);
    std::string GetTime(void);

private:
    sqlite3 *wombatdb;
    sqlite3 *logdb;
    QWidget *wombatparent;
    sqlite3_stmt *sqlstatement;
    sqlite3_stmt *logstatement;
};
*/
#endif // WOMBATVARIABLE_H
