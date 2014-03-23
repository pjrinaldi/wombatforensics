QT += widgets gui core concurrent webkitwidgets sql
QT -= opengl quick network qml
mac:CONFIG += debug app_bundle
linux:CONFIG += debug
mac:TEMPLATE = app
INCLUDEPATH += ../sleuthkit/
#INCLUDEPATH += ../vlc-qt/src/widgets/
#INCLUDEPATH += ../vlc-qt/src/core/
#INCLUDEPATH += ../vlc-qt/src/
VPATH += ../sleuthkit/
#VPATH += ../vlc-qt/src/widgets/
#VPATH += ../vlc-qt/src/core/
#VPATH += ../vlc-qt/src/
HEADERS = wombatforensics.h wombatvariable.h wombatdatabase.h wombatframework.h wombatfunctions.h ui_wombatforensics.h progresswindow.h ui_progresswindow.h ui_exportdialog.h exportdialog.h hexEditor.hpp box.hpp cursor.hpp reader.hpp grid.hpp offsetConstraint.hpp translate.hpp libtsk.h tskvariable.h
SOURCES = main.cpp wombatforensics.cpp wombatdatabase.cpp wombatframework.cpp wombatfunctions.cpp progresswindow.cpp exportdialog.cpp hexEditor.cpp cursor.cpp grid.cpp offsetConstraint.cpp reader.cpp translate.cpp
RESOURCES += wombatforensics.qrc progresswindow.qrc
DESTDIR = ./
mac:LIBS = -lsqlite3 -L/opt/local/lib -lewf -L/opt/local/lib -ltsk 
#mac:LIBS = -lsqlite3 -L/opt/local/lib -lewf -L/opt/local/lib -ltsk 
#mac:LIBS = -lsqlite3 -L/opt/local/lib -lewf -L/opt/local/lib -ltsk -L/opt/Qt5.1.0/5.1.0/gcc_64/plugins/sqldrivers -lqsqlite
linux:LIBS = -lewf -ltsk 
#linux:LIBS = -lsqlite3 -lewf -ltsk 
#linux:LIBS = -lsqlite3 -lewf -ltsk -lqsqlite 
if(!debug_and_release|build_pass):CONFIG(debug, debug|release) {
win32:LIBS = $$member(LIBS, 0) $$member(LIBS, 1)d
}

#install
target.path = ./
INSTALLS += target
