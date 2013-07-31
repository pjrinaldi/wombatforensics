#-------------------------------------------------
#
# Project created by QtCreator 2013-07-24T07:05:44
#
#-------------------------------------------------


TEMPLATE = lib
CONFIG += plugin
QT       += widgets
INCLUDEPATH += ../..
HEADERS += basictools.h
SOURCES += basictools.cpp
TARGET = $$qtLibraryTarget(basictools)
DESTDIR = ../../../build/plugins
# install
target.path = ../../../build/plugins
INSTALLS += target

#TARGET = BasicTools
#DESTDIR = $$[QT_INSTALL_PLUGINS]/
OTHER_FILES += BasicTools.json

#unix:!symbian {
#    maemo5 {
#        target.path = /opt/usr/lib
#    } else {
#        target.path = /usr/lib
#    }
#    INSTALLS += target
#}

#TEMPLATE      = lib
#CONFIG       += plugin static
#QT           += widgets
#INCLUDEPATH  += ../..
#HEADERS       = basictoolsplugin.h
#SOURCES       = basictoolsplugin.cpp
#TARGET        = $$qtLibraryTarget(pnp_basictools)
#DESTDIR       = ../../plugandpaint/plugins

# install
#target.path = ../../widgets/tools/plugandpaint/plugins
#INSTALLS += target

RESOURCES += \
    basictools.qrc

