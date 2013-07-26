#include "sqlerrlog.h"

SqlErrLog::SqlErrLog() : Log()
{
}
SqlErrLog::~SqlErrLog()
{

}

void SqlErrLog::log(Channel a_channel, const std::string &a_msg)
{
    Log::log(a_channel, a_msg);
    sqldb = NULL;
    int sqlValue;
    QString tmpPath = QDir(QCoreApplication::applicationDirPath()).absolutePath();
    if(tmpPath != "-15")
    {
        tmpPath += "/data/";
        tmpPath += "WombatData.db";
        sqlValue = sqlite3_open_v2(tmpPath.toStdString().c_str(), &sqldb, SQLITE_OPEN_READWRITE, NULL); // opendb
        if(sqlValue == 0) // sqlite_ok
        {
            sqlValue = sqlite3_prepare_v2(sqldb, "INSERT INTO logtable (logchannel, logmessage) VALUES(?, ?);", -1, &sqlStatement, NULL);
            sqlValue = sqlite3_bind_int(sqlStatement, 1, a_channel);
            sqlValue = sqlite3_bind_text(sqlStatement, 2, a_msg.c_str(), -1, SQLITE_TRANSIENT);
            sqlValue = sqlite3_step(sqlStatement);
            sqlValue = sqlite3_finalize(sqlStatement);
            sqlValue = sqlite3_close(sqldb);
        }
    }
}
