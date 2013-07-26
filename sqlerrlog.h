#ifndef SQLERRLOG_H
#define SQLERRLOG_H

#include <sqlite3.h>
#include <QString>
#include <QDir>
#include <QCoreApplication>
#include "tsk/framework/framework.h"

class SqlErrLog : public Log
{
public:
    sqlite3 *sqldb;
    sqlite3_stmt *sqlErrStatement;
    char *sqlErrMsg;
    int sqlValue;

    SqlErrLog();
    ~SqlErrLog();

    void log(Channel a_channel, const std::string &a_msg);

};

#endif // SQLERRLOG_H
