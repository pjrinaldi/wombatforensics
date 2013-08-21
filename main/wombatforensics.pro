QT += widgets gui core
CONFIG += debug

HEADERS = interfaces.h wombatforensics.h wombatvariable.h wombatcasedb.h ui_wombatforensics.h progresswindow.h ui_progresswindow.h
SOURCES = main.cpp wombatforensics.cpp wombatvariable.cpp wombatcasedb.cpp progresswindow.cpp
RESOURCES += wombat.qrc progresswindow.qrc
DESTDIR = ../build
LIBS = -Lplugins -lsqlite3 -lewf -ltsk -ltskframework

if(!debug_and_release|build_pass):CONFIG(debug, debug|release) {
mac:LIBS = $$member(LIBS, 0) $$member(LIBS, 1)_debug
win32:LIBS = $$member(LIBS, 0) $$member(LIBS, 1)d
}

#install
target.path = ../build
INSTALLS += target
