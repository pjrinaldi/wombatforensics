
TEMPLATE = lib
QT += core gui widgets
CONFIG += plugin
CONFIG += debug
TARGET = $$qtLibraryTarget(evidenceplugin)
INCLUDEPATH += ../../

HEADERS += evidence.h
SOURCES += evidence.cpp
RESOURCES += evidence.qrc
DESTDIR = ../../build/plugins

OTHER_FILES += evidence.json

#install
target.path = ../../build/plugins
INSTALLS += target
