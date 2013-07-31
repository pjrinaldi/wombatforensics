#ifndef WOMBATCASEDB_H
#define WOMBATCASEDB_H

#include "sqlite3.h"
#include <QMessageBox>
#include <string>
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
    WombatCaseDb();
    bool FileExists(const std::string& fileName);
    void DisplayError(QWidget *parent, QString errorNumber, QString errorType, QString errorValue);
    const char *CreateCaseDB(QString dbname);
    ~WombatCaseDb();
    int ReturnCaseCount(void);
    sqlite3* ReturnDB(QString dbname);
    int InsertCase(QString caseText);

private:
    sqlite3 *wombatdb;
    sqlite3_stmt *sqlstatement;
};

#endif // WOMBATCASEDB_H
