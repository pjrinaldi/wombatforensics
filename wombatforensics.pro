QT += widgets core concurrent sql multimedia multimediawidgets webenginewidgets gui
#QT -= opengl quick network qml
linux:CONFIG += debug_and_release debug_and_release_target qt x11 build_all c++11
#linux:CONFIG += release qt x11 build_all c++11
INCLUDEPATH += /usr/include/tsk/
INCLUDEPATH += /usr/include/tsk/auto/
INCLUDEPATH += /usr/include/tsk/base/
INCLUDEPATH += /usr/include/tsk/fs/
INCLUDEPATH += /usr/include/tsk/hashdb/
INCLUDEPATH += /usr/include/tsk/img/
INCLUDEPATH += /usr/include/tsk/vs/
VPATH += /usr/include/tsk/
VPATH += /usr/include/tsk/auto/
VPATH += /usr/include/tsk/base/
VPATH += /usr/include/tsk/fs/
VPATH += /usr/include/tsk/hashdb/
VPATH += /usr/include/tsk/img/
VPATH += /usr/include/tsk/vs/
FORMS = aboutbox.ui accesseddatefilter.ui analysistypefilter.ui byteconverter.ui changeddatefilter.ui createddatefilter.ui digdeeperdialog.ui exportdialog.ui filecategoryfilter.ui filetypefilter.ui fileviewer.ui hashcategoryfilter.ui htmlviewer.ui idfilter.ui imageviewer.ui imagewindow.ui jumpto.ui messageviewer.ui modifieddatefilter.ui namefilter.ui pathfilter.ui propertieswindow.ui settingsdialog.ui sizefilter.ui textviewer.ui videoviewer.ui viewermanager.ui wombatforensics.ui jumphex.ui addevidencedialog.ui digstatus.ui remevidencedialog.ui tagfilter.ui
HEADERS = wombatforensics.h wombatvariable.h wombatfunctions.h exportdialog.h tskvariable.h globals.h wombatinclude.h propertieswindow.h fileviewer.h filterviews.h imageviewer.h videoviewer.h viewermanager.h textviewer.h htmlviewer.h messageviewer.h byteconverter.h digdeeperdialog.h aboutbox.h qhexedit.h chunks.h commands.h settingsdialog.h addevidencedialog.h digstatus.h remevidencedialog.h tskcpp.h
#HEADERS = wombatforensics.h wombatvariable.h wombatfunctions.h exportdialog.h tskvariable.h globals.h wombatinclude.h propertieswindow.h fileviewer.h filterviews.h imageviewer.h videoviewer.h viewermanager.h textviewer.h htmlviewer.h messageviewer.h byteconverter.h digdeeperdialog.h aboutbox.h qhexedit.h chunks.h commands.h settingsdialog.h addevidencedialog.h digstatus.h
SOURCES = main.cpp wombatforensics.cpp wombatfunctions.cpp exportdialog.cpp globals.cpp propertieswindow.cpp fileviewer.cpp filterviews.cpp imageviewer.cpp videoviewer.cpp viewermanager.cpp textviewer.cpp htmlviewer.cpp messageviewer.cpp byteconverter.cpp digdeeperdialog.cpp aboutbox.cpp qhexedit.cpp chunks.cpp commands.cpp settingsdialog.cpp addevidencedialog.cpp digstatus.cpp remevidencedialog.cpp tskcpp.cpp
RESOURCES += wombatforensics.qrc
release: DESTDIR = release
debug:   DESTDIR = debug
linux:LIBS = -lewf -ltsk -ltar
if(!debug_and_release|build_pass):CONFIG(debug, debug|release) {
}
#linux:

#install
target.path = /usr/local/bin
#target.path = ./
INSTALLS += target

