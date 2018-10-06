QT += widgets core concurrent sql multimedia multimediawidgets webenginewidgets gui
QT -= opengl quick network qml
mac:CONFIG += debug app_bundle
linux:CONFIG += debug qt #c++11
mac:TEMPLATE = app
#INCLUDEPATH += ../sleuthkit/
#INCLUDEPATH += ../sleuthkit/tsk/
#INCLUDEPATH += ../libewf_64bit/include/
INCLUDEPATH += ./common/
#VPATH += ../sleuthkit/
#VPATH += ../sleuthkit/tsk/
#VPATH += ../libewf_64bit/include/
FORMS = aboutbox.ui accesseddatefilter.ui analysistypefilter.ui byteconverter.ui changeddatefilter.ui createddatefilter.ui digdeeperdialog.ui exportdialog.ui filecategoryfilter.ui filetypefilter.ui fileviewer.ui hashcategoryfilter.ui htmlviewer.ui idfilter.ui imageviewer.ui imagewindow.ui jumpto.ui messageviewer.ui modifieddatefilter.ui namefilter.ui pathfilter.ui propertieswindow.ui settingsdialog.ui sizefilter.ui textviewer.ui videoviewer.ui viewermanager.ui wombatforensics.ui
HEADERS = wombatforensics.h wombatvariable.h wombatfunctions.h exportdialog.h libtsk.h tskvariable.h globals.h wombatinclude.h propertieswindow.h fileviewer.h filterviews.h imageviewer.h videoviewer.h viewermanager.h textviewer.h htmlviewer.h messageviewer.h byteconverter.h digdeeperdialog.h aboutbox.h qhexedit.h chunks.h commands.h settingsdialog.h
SOURCES = main.cpp wombatforensics.cpp wombatfunctions.cpp exportdialog.cpp globals.cpp propertieswindow.cpp fileviewer.cpp filterviews.cpp imageviewer.cpp videoviewer.cpp viewermanager.cpp textviewer.cpp htmlviewer.cpp messageviewer.cpp byteconverter.cpp digdeeperdialog.cpp aboutbox.cpp qhexedit.cpp chunks.cpp commands.cpp settingsdialog.cpp
RESOURCES += wombatforensics.qrc
DESTDIR = ./
mac:LIBS = -lsqlite3 -L/opt/local/lib -lewf -L/opt/local/lib -ltsk 
linux:LIBS = -lewf -ltsk -L/home/pasquale/Projects/wombatforensics/lib -lsc_ex -lsc_da -lsc_ch -lsc_fa -lsc_fi -lsc_ut -lwv_core -lsc_fut -lsc_lo -lsc_fnt
if(!debug_and_release|build_pass):CONFIG(debug, debug|release) {
win32:LIBS = $$member(LIBS, 0) $$member(LIBS, 1)d
}
#win32:RC_ICONS = wombat_32.ico
#mac:ICON = myapp.ico
#linux:

#install
target.path = ./
INSTALLS += target
