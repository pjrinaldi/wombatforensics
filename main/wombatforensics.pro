QT += widgets gui core concurrent webkitwidgets
CONFIG += debug

INCLUDEPATH += ../sleuthkit/framework/tsk/framework/services/
INCLUDEPATH += ../sleuthkit/framework/tsk/framework/extraction/
#INCLUDEPATH += ../lfhex-0.42/src/
#INCLUDEPATH += ../vlc-qt/src/widgets/
#INCLUDEPATH += ../vlc-qt/src/core/
#INCLUDEPATH += ../vlc-qt/src/
VPATH += ../sleuthkit/framework/tsk/framework/services/
VPATH += ../sleuthkit/framework/tsk/framework/extraction/
#VPATH += ../vlc-qt/src/widgets/
#VPATH += ../vlc-qt/src/core/
#VPATH += ../vlc-qt/src/
HEADERS = wombatforensics.h wombatvariable.h wombatdatabase.h ui_wombatforensics.h progresswindow.h ui_progresswindow.h sleuthkit.h TskDBBlackboard.h TskImageFileTsk.h TskImgDBSqlite.h TskLog.h TskSystemPropertiesImpl.h TskSchedulerQueue.h ui_exportdialog.h exportdialog.h hexEditor.hpp box.hpp cursor.hpp reader.hpp grid.hpp offsetConstraint.hpp translate.hpp
SOURCES = main.cpp wombatforensics.cpp wombatdatabase.cpp progresswindow.cpp sleuthkit.cpp TskDBBlackboard.cpp TskImageFileTsk.cpp TskImgDBSqlite.cpp TskLog.cpp TskSystemPropertiesImpl.cpp TskSchedulerQueue.cpp exportdialog.cpp hexEditor.cpp cursor.cpp grid.cpp offsetConstraint.cpp reader.cpp translate.cpp
RESOURCES += wombatforensics.qrc progresswindow.qrc
DESTDIR = ./
#DESTDIR = ../build
LIBS = -lsqlite3 -lewf -ltsk -ltskframework
#LIBS = -lsqlite3 -lewf -ltsk -ltskframework -L/home/pasquale/vlc-qt -lvlc-qt-widgets
#LIBS = -lsqlite3 -lewf -ltsk -L/home/pasquale/libs -Wl,-rpath,/home/pasquale/libs -ltskframework

if(!debug_and_release|build_pass):CONFIG(debug, debug|release) {
mac:LIBS = $$member(LIBS, 0) $$member(LIBS, 1)_debug
win32:LIBS = $$member(LIBS, 0) $$member(LIBS, 1)d
}

#install
#target.path = ../build
target.path = ./
INSTALLS += target
