QT += widgets gui core concurrent webkitwidgets sql av avwidgets
QT -= opengl quick network qml
mac:CONFIG += debug app_bundle
linux:CONFIG += debug #c++11
mac:TEMPLATE = app
INCLUDEPATH += ../sleuthkit/
INCLUDEPATH += ../sleuthkit/tsk/
INCLUDEPATH += ../file/src/
INCLUDEPATH += ../AFFLIBv3/lib/
INCLUDEPATH += ../libewf_64bit/include/
#INCLUDEPATH += ../QHexEdit/
INCLUDEPATH += ../qhexedit2/src/
#INCLUDEPATH += ../vlc-qt/src/widgets/
#INCLUDEPATH += ../vlc-qt/src/core/
#INCLUDEPATH += ../vlc-qt/src/
VPATH += ../sleuthkit/
VPATH += ../sleuthkit/tsk/
VPATH += ../file/src/
VPATH += ../AFFLIBv3/lib/
VPATH += ../libewf_64bit/include/
#VPATH += ../QHexEdit/
VPATH += ../qhexedit2/src/
#VPATH += ../vlc-qt/src/widgets/
#VPATH += ../vlc-qt/src/core/
#VPATH += ../vlc-qt/src/
HEADERS = wombatforensics.h wombatvariable.h wombatdatabase.h wombatframework.h wombatfunctions.h ui_wombatforensics.h progresswindow.h ui_progresswindow.h ui_exportdialog.h exportdialog.h cursor.h translate.h libtsk.h tskvariable.h globals.h wombatinclude.h propertieswindow.h ui_propertieswindow.h wombatproperties.h fileviewer.h ui_fileviewer.h imgreader.h imghexviewer.h filereader.h filehexviewer.h magic.h ui_accesseddatefilter.h ui_pathfilter.h ui_analysistypefilter.h ui_changeddatefilter.h ui_createddatefilter.h ui_filetypefilter.h ui_hashcategoryfilter.h ui_hashfilter.h ui_idfilter.h ui_modifieddatefilter.h ui_namefilter.h ui_sizefilter.h filterviews.h imageviewer.h ui_imagewindow.h ui_imageviewer.h ui_videoviewer.h videoviewer.h ui_viewermanager.h viewermanager.h ui_textviewer.h textviewer.h ui_htmlviewer.h htmlviewer.h ui_messageviewer.h messageviewer.h ui_byteconverter.h byteconverter.h qhexedit.h chunks.h commands.h #qhexeditdata.h qhexedit.h qhexeditcomments.h qhexeditdatadevice.h qhexeditdatareader.h qhexeditdatawriter.h qhexedithighlighter.h qhexeditprivate.h sparserangemap.h qhexedit.h chunks.h #qdebugstream.h 
SOURCES = main.cpp wombatforensics.cpp wombatdatabase.cpp wombatframework.cpp wombatfunctions.cpp progresswindow.cpp exportdialog.cpp cursor.cpp translate.cpp globals.cpp propertieswindow.cpp wombatproperties.cpp fileviewer.cpp imgreader.cpp imghexviewer.cpp filereader.cpp filehexviewer.cpp filterviews.cpp imageviewer.cpp videoviewer.cpp viewermanager.cpp textviewer.cpp htmlviewer.cpp messageviewer.cpp byteconverter.cpp qhexedit.cpp chunks.cpp commands.cpp #qhexeditdata.cpp qhexedit.cpp qhexeditcomments.cpp qhexeditdatadevice.cpp qhexeditdatareader.cpp qhexeditdatawriter.cpp qhexedithighlighter.cpp qhexeditprivate.cpp sparserangemap.cpp
RESOURCES += wombatforensics.qrc
DESTDIR = ./
mac:LIBS = -lsqlite3 -L/opt/local/lib -lewf -L/opt/local/lib -ltsk 
#mac:LIBS = -lsqlite3 -L/opt/local/lib -lewf -L/opt/local/lib -ltsk 
#mac:LIBS = -lsqlite3 -L/opt/local/lib -lewf -L/opt/local/lib -ltsk -L/opt/Qt5.3.1/5.3/gcc_64/plugins/sqldrivers -lqsqlite
linux:LIBS = -lewf -lafflib -ltsk -L/usr/local/lib -lmagic
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
