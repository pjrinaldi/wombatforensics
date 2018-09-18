QT += widgets core concurrent sql multimedia multimediawidgets webenginewidgets
QT -= opengl gui quick network qml
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
#FORMS = // list of ui files to process and generate _.h files.
HEADERS = wombatforensics.h wombatvariable.h wombatfunctions.h ui_wombatforensics.h ui_exportdialog.h exportdialog.h libtsk.h tskvariable.h globals.h wombatinclude.h propertieswindow.h ui_propertieswindow.h fileviewer.h ui_fileviewer.h ui_accesseddatefilter.h ui_pathfilter.h ui_analysistypefilter.h ui_changeddatefilter.h ui_createddatefilter.h ui_filetypefilter.h ui_hashcategoryfilter.h ui_hashfilter.h ui_idfilter.h ui_modifieddatefilter.h ui_namefilter.h ui_sizefilter.h filterviews.h imageviewer.h ui_imagewindow.h ui_imageviewer.h ui_videoviewer.h videoviewer.h ui_viewermanager.h viewermanager.h ui_textviewer.h textviewer.h ui_htmlviewer.h htmlviewer.h ui_messageviewer.h messageviewer.h ui_byteconverter.h byteconverter.h digdeeperdialog.ui ui_digdeeperdialog.h digdeeperdialog.h ui_filecategoryfilter.h aboutbox.h ui_cancelthread.h cancelthread.h qhexedit.h chunks.h commands.h ui_settingsdialog.h settingsdialog.h
SOURCES = main.cpp wombatforensics.cpp wombatfunctions.cpp exportdialog.cpp globals.cpp propertieswindow.cpp fileviewer.cpp filterviews.cpp imageviewer.cpp videoviewer.cpp viewermanager.cpp textviewer.cpp htmlviewer.cpp messageviewer.cpp byteconverter.cpp digdeeperdialog.cpp aboutbox.cpp cancelthread.cpp qhexedit.cpp chunks.cpp commands.cpp settingsdialog.cpp
RESOURCES += wombatforensics.qrc
DESTDIR = ./
mac:LIBS = -lsqlite3 -L/opt/local/lib -lewf -L/opt/local/lib -ltsk 
linux:LIBS = -lewf -ltsk -L/home/pasquale/Projects/wombatforensics/lib -lsc_ex -lsc_da -lsc_ch -lsc_fa -lsc_fi -lsc_ut -lwv_core -lsc_fut -lsc_lo
if(!debug_and_release|build_pass):CONFIG(debug, debug|release) {
win32:LIBS = $$member(LIBS, 0) $$member(LIBS, 1)d
}
#win32:RC_ICONS = wombat_32.ico
#mac:ICON = myapp.ico
#linux:

#install
target.path = ./
INSTALLS += target
