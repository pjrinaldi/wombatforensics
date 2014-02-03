QT += widgets gui core concurrent webkitwidgets
mac:CONFIG += debug app_bundle
linux:CONFIG += debug
mac:TEMPLATE = app
#INCLUDEPATH += ../sleuthkit/framework/tsk/framework/services/
#INCLUDEPATH += ../sleuthkit/framework/tsk/framework/extraction/
INCLUDEPATH += ../sleuthkit/tsk/
#INCLUDEPATH += ../lfhex-0.42/src/
#INCLUDEPATH += ../vlc-qt/src/widgets/
#INCLUDEPATH += ../vlc-qt/src/core/
#INCLUDEPATH += ../vlc-qt/src/
#VPATH += ../sleuthkit/framework/tsk/framework/services/
#VPATH += ../sleuthkit/framework/tsk/framework/extraction/
VPATH += ../sleuthkit/tsk/
#VPATH += ../vlc-qt/src/widgets/
#VPATH += ../vlc-qt/src/core/
#VPATH += ../vlc-qt/src/
HEADERS = wombatforensics.h wombatvariable.h wombatdatabase.h wombatframework.h ui_wombatforensics.h progresswindow.h ui_progresswindow.h ui_exportdialog.h exportdialog.h hexEditor.hpp box.hpp cursor.hpp reader.hpp grid.hpp offsetConstraint.hpp translate.hpp libtsk.h
#HEADERS = wombatforensics.h wombatvariable.h wombatdatabase.h wombatframework.h ui_wombatforensics.h progresswindow.h ui_progresswindow.h sleuthkit.h TskDBBlackboard.h TskImageFileTsk.h TskImgDBSqlite.h TskLog.h TskSystemPropertiesImpl.h TskSchedulerQueue.h ui_exportdialog.h exportdialog.h hexEditor.hpp box.hpp cursor.hpp reader.hpp grid.hpp offsetConstraint.hpp translate.hpp libtsk.h
#SOURCES = main.cpp wombatforensics.cpp wombatdatabase.cpp wombatframework.cpp progresswindow.cpp sleuthkit.cpp TskDBBlackboard.cpp TskImageFileTsk.cpp TskImgDBSqlite.cpp TskLog.cpp TskSystemPropertiesImpl.cpp TskSchedulerQueue.cpp exportdialog.cpp hexEditor.cpp cursor.cpp grid.cpp offsetConstraint.cpp reader.cpp translate.cpp
SOURCES = main.cpp wombatforensics.cpp wombatdatabase.cpp wombatframework.cpp progresswindow.cpp exportdialog.cpp hexEditor.cpp cursor.cpp grid.cpp offsetConstraint.cpp reader.cpp translate.cpp
RESOURCES += wombatforensics.qrc progresswindow.qrc
DESTDIR = ./
#DESTDIR = ../build
mac:LIBS = -lsqlite3 -L/opt/local/lib -lewf -ltsk
linux:LIBS = -lsqlite3 -lewf -ltsk
if(!debug_and_release|build_pass):CONFIG(debug, debug|release) {
#mac:LIBS = $$member(LIBS, 0) $$member(LIBS, 1)_debug
win32:LIBS = $$member(LIBS, 0) $$member(LIBS, 1)d
}

#install
#target.path = ../build
target.path = ./
INSTALLS += target
