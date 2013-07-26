#-------------------------------------------------
#
# Project created by QtCreator 2013-07-10T12:27:14
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = WombatForensics
TEMPLATE = app
#DESTDIR = ./build
#target.path = ./build

SOURCES += main.cpp\
        wombatforensics.cpp \
    tskfunctions.cpp \
    sqlwrapper.cpp \
    sqlerrlog.cpp \
    wombattskimgdbsqlite.cpp

HEADERS  += wombatforensics.h \
            interfaces.h \
    tskfunctions.h \
    sqlwrapper.h \
    sqlerrlog.h \
    wombattskimgdbsqlite.h

# LIBS    =   -Wl,-Bstatic -ltskframework -lewf -ltsk -lsqlite3 -Lplugins -lbasictools
LIBS    =   -ltskframework -lewf -lsqlite3 -ltsk -Lplugins -lbasictools

FORMS    += wombatforensics.ui \
    NewCaseDialog.ui

# QT += widgets

#HEADERS        = interfaces.h \
#                 mainwindow.h \
#                 paintarea.h \
#                 plugindialog.h
#SOURCES        = main.cpp \
#                 mainwindow.cpp \
#                 paintarea.cpp \
#                 plugindialog.cpp

#LIBS           = -Lplugins -lpnp_basictools

#if(!debug_and_release|build_pass):CONFIG(debug, debug|release) {
#   mac:LIBS = $$member(LIBS, 0) $$member(LIBS, 1)_debug
#   win32:LIBS = $$member(LIBS, 0) $$member(LIBS, 1)d
#}

# install
#target.path = $$[QT_INSTALL_EXAMPLES]/widgets/tools/plugandpaint
#INSTALLS += target

RESOURCES += \
    wombat.qrc
