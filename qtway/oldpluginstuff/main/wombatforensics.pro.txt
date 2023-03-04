QT += widgets gui core
CONFIG += debug

HEADERS = interfaces.h wombatforensics.h wombatcasedb.h ui_wombatforensics.h
SOURCES = main.cpp wombatforensics.cpp wombatcasedb.cpp
RESOURCES += wombat.qrc
DESTDIR = ../build
LIBS = -Lplugins -lsqlite3

if(!debug_and_release|build_pass):CONFIG(debug, debug|release) {
mac:LIBS = $$member(LIBS, 0) $$member(LIBS, 1)_debug
win32:LIBS = $$member(LIBS, 0) $$member(LIBS, 1)d
}

#install
target.path = ../build
INSTALLS += target
