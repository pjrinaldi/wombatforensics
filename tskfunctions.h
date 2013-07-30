#ifndef TSKFUNCTIONS_H
#define TSKFUNCTIONS_H

#include "wombattskimgdbsqlite.h"
#include "tsk/framework/services/TskSchedulerQueue.h"
#include "tsk/framework/file/TskFileManagerImpl.h"
#include "tsk/framework/services/TskSystemPropertiesImpl.h"

//#include <iostream>
//#include <cstdio>
//#include <cstdlib>
//#include <string>
//#include <time.h>
//#include <memory>

//#include "/usr/local/include/tsk/tsk_tools_i.h"
//#include "tsk/framework/services/TskSystemPropertiesImpl.h"
//#include "tsk/framework/extraction/TskCarvePrepSectorConcat.h"
//#include "tsk/framework/extraction/TskCarveExtractScalpel.h"
//#include "tsk/framework/extraction/TskExtract.h"
//#include "Poco/File.h"
//#ifdef TSK_WIN32
//#include <Windows.h>
//#else
//#include <sys/stat.h>
//#endif
//#include "Poco/File.h"
//#include "Poco/UnicodeConverter.h"

class TskFunctions
{
public:
    TskFunctions();
    void SetupTskFramework();
    void InitializeFrameworkProperties(void);
    void InitializeFrameworkLog(void);
    void InitializeFrameworkScheduler(void);
    void InitializeFrameworkBlackboard(void);
    void InitializeFrameworkDatabase(void);
    void InitializeFrameworkFileManager(void);
    SqlWrapper *wombatSqlObject;

    int ret;
    //std::auto_ptr<TskImgDB> frameworkDatabase;
    std::auto_ptr<Log> frameworkLog;
    //SqlErrLog *frameworkLog;

};

#endif // TSKFUNCTIONS_H
