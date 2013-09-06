
TEMPLATE = lib
QT += core gui widgets
CONFIG += plugin
CONFIG += debug
TARGET = $$qtLibraryTarget(sleuthkitplugin)
INCLUDEPATH += ../../

HEADERS += sleuthkit.h TskDBBlackboard.h TskImageFileTsk.h TskImgDBSqlite.h TskSchedulerQueue.h TskSystemPropertiesImpl.h TskLog.h ../../main/wombatvariable.h
SOURCES += sleuthkit.cpp TskDBBlackboard.cpp TskImageFileTsk.cpp TskImgDBSqlite.cpp TskSchedulerQueue.cpp TskSystemPropertiesImpl.cpp TskLog.cpp
#RESOURCES += sleuthkit.qrc
DESTDIR = ../../build/plugins

OTHER_FILES += sleuthkit.json

#install
target.path = ../../build/plugins
INSTALLS += target
