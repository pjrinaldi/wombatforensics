
TEMPLATE = lib
QT += core gui widgets
CONFIG += plugin
CONFIG += debug
TARGET = $$qtLibraryTarget(progresswindowplugin)
INCLUDEPATH += ../../

# Input
HEADERS += progresswindow.h ui_progresswindow.h
SOURCES += progresswindow.cpp
RESOURCES += progresswindow.qrc
DESTDIR = ../../build/plugins

OTHER_FILES += progresswindow.json

#install
target.path = ../../build/plugins
INSTALLS += target
