QT += widgets gui core concurrent webkitwidgets sql
QT -= opengl quick network qml
mac:CONFIG += debug app_bundle
linux:CONFIG += debug
mac:TEMPLATE = app
INCLUDEPATH += ./sleuthkit/
INCLUDEPATH += ./sleuthkit/tsk/
#INCLUDEPATH += ./vlc-qt/src/widgets/
#INCLUDEPATH += ./vlc-qt/src/core/
#INCLUDEPATH += ./vlc-qt/src/
VPATH += ./sleuthkit/
VPATH += ./sleuthkit/tsk/
#VPATH += ./vlc-qt/src/widgets/
#VPATH += ./vlc-qt/src/core/
#VPATH += ./vlc-qt/src/
HEADERS = wombatforensics.h wombatvariable.h wombatdatabase.h wombatframework.h wombatfunctions.h ui_wombatforensics.h progresswindow.h ui_progresswindow.h ui_exportdialog.h exportdialog.h hexEditor.h cursor.h reader.h translate.h libtsk.h tskvariable.h globals.h wombatinclude.h propertieswindow.h ui_propertieswindow.h
SOURCES = main.cpp wombatforensics.cpp wombatdatabase.cpp wombatframework.cpp wombatfunctions.cpp progresswindow.cpp exportdialog.cpp hexEditor.cpp cursor.cpp reader.cpp translate.cpp globals.cpp propertieswindow.cpp
RESOURCES += wombatforensics.qrc
DESTDIR = ./
mac:LIBS = -lsqlite3 -L/opt/local/lib -lewf -L/opt/local/lib -ltsk 
#mac:LIBS = -lsqlite3 -L/opt/local/lib -lewf -L/opt/local/lib -ltsk 
#mac:LIBS = -lsqlite3 -L/opt/local/lib -lewf -L/opt/local/lib -ltsk -L/opt/Qt5.3.1/5.3/gcc_64/plugins/sqldrivers -lqsqlite
linux:LIBS = -lewf -ltsk 
#linux:LIBS = -lsqlite3 -lewf -ltsk 
#linux:LIBS = -lsqlite3 -lewf -ltsk -lqsqlite 
if(!debug_and_release|build_pass):CONFIG(debug, debug|release) {
win32:LIBS = $$member(LIBS, 0) $$member(LIBS, 1)d
}
#win32:RC_ICONS = wombat_32.ico
#mac:ICON = myapp.ico
#linux:

#install
target.path = ./
INSTALLS += target
