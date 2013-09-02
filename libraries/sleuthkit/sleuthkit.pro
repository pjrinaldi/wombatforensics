
TEMPLATE = lib
QT += core concurrent gui widgets
CONFIG += plugin
CONFIG += debug
TARGET = $$qtLibraryTarget(sleuthkitplugin)
INCLUDEPATH += ../../

HEADERS += sleuthkit.h TskDBBlackboard.h TskFileManagerImpl.h TskImageFileTsk.h TskImgDBSqlite.h TskSchedulerQueue.h TskSystemPropertiesImpl.h TskFileTsk.h TskLog.h ../../main/progresswindow.h ../../main/wombatvariable.h TskAutoImpl.h TskExecutableModule.h TskFileAnalysisPipeline.h TskFileAnalysisPluginModule.h TskModule.h TskPipeline.h TskPipelineManager.h TskPluginModule.h TskReportPipeline.h TskReportPluginModule.h
SOURCES += sleuthkit.cpp TskDBBlackboard.cpp TskFileManagerImpl.cpp TskImageFileTsk.cpp TskImgDBSqlite.cpp TskSchedulerQueue.cpp TskSystemPropertiesImpl.cpp TskFileTsk.cpp TskLog.cpp ../../main/progresswindow.cpp ../../main/wombatvariable.cpp TskAutoImpl.cpp TskExecutableModule.cpp TskFileAnalysisPipeline.cpp TskFileAnalysisPluginModule.cpp TskModule.cpp TskPipeline.cpp TskPipelineManager.cpp TskPluginModule.cpp TskReportPipeline.cpp TSkReportPluginModule.cpp
#RESOURCES += sleuthkit.qrc
DESTDIR = ../../build/plugins

OTHER_FILES += sleuthkit.json

#install
target.path = ../../build/plugins
INSTALLS += target
