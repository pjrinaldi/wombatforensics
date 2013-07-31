#-------------------------------------------------
#
# Project created by QtCreator 2013-07-10T12:27:14
#
#-------------------------------------------------

QT       += core gui widgets

TARGET = WombatForensics
TEMPLATE = app
DESTDIR = ../wombatforensics-output/debug/build
CONFIG += debug
SOURCES += main.cpp wombatforensics.cpp wombatcasedb.cpp

HEADERS  += wombatforensics.h \
            interfaces.h \
    wombatcasedb.h

LIBS    =   -ltskframework -lewf -lsqlite3 -ltsk -Lplugins

FORMS    += wombatforensics.ui

RESOURCES += \
    wombat.qrc
