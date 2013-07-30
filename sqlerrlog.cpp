#include "sqlerrlog.h"

SqlErrLog::SqlErrLog() : Log()
{
}
SqlErrLog::~SqlErrLog()
{

}

void SqlErrLog::log(Channel a_channel, const std::string &a_msg)
{
    sqldb = NULL;
    int sqlValue;
    int channelInt = -1;
    if(a_channel == Log::Error)
        channelInt = 0;
    else if(a_channel == Log::Info)
        channelInt = 1;
    else if(a_channel == Log::Warn)
        channelInt = 2;
    else
        channelInt = 15;
    QString tmpPath = QDir(QCoreApplication::applicationDirPath()).absolutePath();
    if(tmpPath != "-15")
    {
        tmpPath += "/data/";
        tmpPath += "WombatData.db";
        sqlValue = sqlite3_open(tmpPath.toStdString().c_str(), &sqldb); // opendb
        //sqlite3_reset(sqlErrStatement);
        //sqlite3_clear_bindings(sqlErrStatement);
        if(sqlValue == 0) // sqlite_ok
        {
            if(sqlite3_prepare_v2(sqldb, "INSERT INTO log (logchannel, logmessage) VALUES(?, ?);", -1, &sqlErrStatement, NULL) == SQLITE_OK)
            {
                if(sqlite3_bind_int(sqlErrStatement, 1, channelInt) == SQLITE_OK)
                {
                    if(sqlite3_bind_text(sqlErrStatement, 2, a_msg.c_str(), -1, SQLITE_TRANSIENT) == SQLITE_OK)
                    {
                        sqlValue = sqlite3_step(sqlErrStatement);
                        sqlValue = sqlite3_finalize(sqlErrStatement);
                        sqlValue = sqlite3_close(sqldb);
                    }
                    else
                        fprintf(stderr, sqlite3_errmsg(sqldb));
                }
                else
                {
                    fprintf(stderr, sqlite3_errmsg(sqldb));
                }
            }
            else
                fprintf(stderr, sqlite3_errmsg(sqldb));
        }
        else
            fprintf(stderr, sqlite3_errmsg(sqldb));
    }
    //Log::log(a_channel, a_msg);
}
