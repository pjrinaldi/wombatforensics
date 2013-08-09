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

private:
    sqlite3 *wombatdb;
    QWidget *wombatparent;
    sqlite3_stmt *sqlstatement;
};

#endif // WOMBATCASEDB_H
