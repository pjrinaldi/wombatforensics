
TEMPLATE = lib
QT += core gui widgets
CONFIG += plugin
CONFIG += debug
TARGET = $$qtLibraryTarget(sleuthkitplugin)
INCLUDEPATH += ../../

HEADERS += sleuthkit.h TskDBBlackboard.h TskFileManagerImpl.h TskImageFileTsk.h TskImgDBSqlite.h TskSchedulerQueue.h TskSystemPropertiesImpl.h TskFileTsk.h TskLog.h ../../main/progresswindow.h ../../main/wombatvariable.h
SOURCES += sleuthkit.cpp TskDBBlackboard.cpp TskFileManagerImpl.cpp TskImageFileTsk.cpp TskImgDBSqlite.cpp TskSchedulerQueue.cpp TskSystemPropertiesImpl.cpp TskFileTsk.cpp TskLog.cpp ../../main/progresswindow.cpp ../../main/wombatvariable.cpp
#RESOURCES += sleuthkit.qrc
DESTDIR = ../../build/plugins

OTHER_FILES += sleuthkit.json

#install
target.path = ../../build/plugins
INSTALLS += target
