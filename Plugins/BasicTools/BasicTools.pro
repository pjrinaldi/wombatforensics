#-------------------------------------------------
#
# Project created by QtCreator 2013-07-10T13:22:22
#
#-------------------------------------------------

QT       += widgets

# TARGET = Basic
TARGET = $$qtLibraryTarget(pnp_basictools)
DESTDIR = ../../plugins
CONFIG += plugin static
TEMPLATE = lib
INCLUDEPATH += ../..
# DEFINES += BASICTOOLS_LIBRARY

SOURCES = basictoolsplugin.cpp
HEADERS = basictoolsplugin.h
# HEADERS += basictoolsplugin.h\
#         basictools_global.h

# install

# unix:!symbian {
#    maemo5 {
#        target.path = /opt/usr/lib
#    } else {
#        target.path = /usr/lib
#    }
#    INSTALLS += target
#}
target.path = ../../plugins
INSTALLS += target
