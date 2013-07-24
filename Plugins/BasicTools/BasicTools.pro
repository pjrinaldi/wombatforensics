#-------------------------------------------------
#
# Project created by QtCreator 2013-07-24T07:05:44
#
#-------------------------------------------------

QT       += core

TARGET = BasicTools
TEMPLATE = lib
CONFIG += plugin static

DESTDIR = $$[QT_INSTALL_PLUGINS]/

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

#TEMPLATE      = lib
#CONFIG       += plugin static
#QT           += widgets
#INCLUDEPATH  += ../..
#HEADERS       = basictoolsplugin.h
#SOURCES       = basictoolsplugin.cpp
#TARGET        = $$qtLibraryTarget(pnp_basictools)
#DESTDIR       = ../../plugandpaint/plugins

# install
#target.path = $$[QT_INSTALL_EXAMPLES]/widgets/tools/plugandpaint/plugins
#INSTALLS += target

