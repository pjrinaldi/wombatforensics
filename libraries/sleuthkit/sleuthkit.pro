
TEMPLATE = lib
QT += core
CONFIG += plugin
CONFIG += debug
TARGET = $$qtLibraryTarget(sleuthkitplugin)
INCLUDEPATH += ../../

HEADERS += sleuthkit.h
SOURCES += sleuthkit.cpp
#RESOURCES += sleuthkit.qrc
DESTDIR = ../../build/plugins

OTHER_FILES += sleuthkit.json

#install
target.path = ../../build/plugins
INSTALLS += target
