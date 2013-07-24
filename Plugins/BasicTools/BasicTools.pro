#-------------------------------------------------
#
# Project created by QtCreator 2013-07-24T07:05:44
#
#-------------------------------------------------

QT       += core sql

TARGET = BasicTools
TEMPLATE = lib
CONFIG += plugin

DESTDIR = $$[QT_INSTALL_PLUGINS]/sqldrivers

SOURCES += basictools.cpp

HEADERS += basictools.h
OTHER_FILES += BasicTools.json

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}
