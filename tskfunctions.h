#ifndef TSKFUNCTIONS_H
#define TSKFUNCTIONS_H

#include "tsk/framework/framework.h"


//#include <iostream>
//#include <cstdio>
//#include <cstdlib>
//#include <string>
//#include <sstream>
//#include <time.h>
//#include <memory>

//#include "/usr/local/include/tsk/tsk_tools_i.h"
//#include "tsk/framework/services/TskSchedulerQueue.h"
//#include "tsk/framework/services/TskSystemPropertiesImpl.h"
//#include "tsk/framework/services/TskImgDBSqlite.h"
//#include "tsk/framework/file/TskFileManagerImpl.h"
//#include "tsk/framework/extraction/TskCarvePrepSectorConcat.h"
//#include "tsk/framework/extraction/TskCarveExtractScalpel.h"
//#include "tsk/framework/extraction/TskExtract.h"
//#include "Poco/Path.h"
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
    void SetupTskFramework(void);
    void InitializeFrameworkProperties(void);
    void InitializeFrameworkLog(void);
    void InitializeFrameworkScheduler(void);
    void InitializeFrameworkBlackboard(void);
    void InitializeFrameworkDatabase(void);

    std::auto_ptr<TskImgDB> frameworkDatabase;
};

#endif // TSKFUNCTIONS_H
