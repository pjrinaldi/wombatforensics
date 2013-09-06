
TEMPLATE = lib
QT += core gui widgets
CONFIG += plugin
CONFIG += debug
TARGET = $$qtLibraryTarget(basictoolsplugin)
INCLUDEPATH += ../../

# Input
HEADERS += basictools.h binviewwidget.h binviewmodel.h
SOURCES += basictools.cpp binviewwidget.cpp binviewmodel.cpp
RESOURCES += basictools.qrc
DESTDIR = ../../build/plugins

OTHER_FILES += basictools.json

#install
target.path = ../../build/plugins
INSTALLS += target
