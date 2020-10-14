QT += widgets core concurrent sql multimedia multimediawidgets webenginewidgets gui avwidgets
#QT -= opengl quick network qml
linux:CONFIG += debug_and_release debug_and_release_target qt x11 build_all c++14 
DEFINES += MAGICKCORE_HDRI_ENABLE=0
DEFINES += MAGICKCORE_QUANTUM_DEPTH=16
DEFINES += _FILE_OFFSET_BITS=64
#PRE_TARGETDEPS += ./libsquashfuse.a
#PRE_TARGETDEPS += ./libsquash.a
#linux:CONFIG += release qt x11 build_all c++11
INCLUDEPATH += /usr/include/tsk/
INCLUDEPATH += /usr/include/tsk/auto/
INCLUDEPATH += /usr/include/tsk/base/
INCLUDEPATH += /usr/include/tsk/fs/
INCLUDEPATH += /usr/include/tsk/hashdb/
INCLUDEPATH += /usr/include/tsk/img/
INCLUDEPATH += /usr/include/tsk/vs/
INCLUDEPATH += /usr/include/tsk/pool/
INCLUDEPATH += /usr/include/libffmpegthumbnailer/
INCLUDEPATH += /usr/include/ImageMagick-7/
#INCLUDEPATH += /usr/include/parted/
INCLUDEPATH += /usr/local/lib/
INCLUDEPATH += /usr/include/poppler/qt5/
#INCLUDEPATH += /usr/include/squashfuse/
#INCLUDEPATH += ./sqfsfuse/
#INCLUDEPATH += ./common/
#INCLUDEPATH += /usr/include/glib-2.0/
#INCLUDEPATH += /usr/lib/glib-2.0/include/
#INCLUDEPATH += /usr/include/afflib/
#INCLUDEPATH += ../scalpel/src/
#INCLUDEPATH += ../revit07-20070804/src/definitions_parser/
#INCLUDEPATH += ../revit07-20070804/src/input_analyzer/
#INCLUDEPATH += ../revit07-20070804/src/input_handler/
#INCLUDEPATH += ../revit07-20070804/src/notify/
#INCLUDEPATH += ../revit07-20070804/src/media_access/
#INCLUDEPATH += ../revit07-20070804/src/output_handler/
#INCLUDEPATH += ../revit07-20070804/src/state_hierarchy/
#INCLUDEPATH += ../revit07-20070804/src/
VPATH += /usr/include/tsk/
VPATH += /usr/include/tsk/auto/
VPATH += /usr/include/tsk/base/
VPATH += /usr/include/tsk/fs/
VPATH += /usr/include/tsk/hashdb/
VPATH += /usr/include/tsk/img/
VPATH += /usr/include/tsk/vs/
VPATH += /usr/include/tsk/pool/
#VPATH += /usr/include/libffmpegthumbnailer/
VPATH += /usr/include/parted/
VPATH += /usr/include/ImageMagick-7/
VPATH += /usr/local/lib/
VPATH += /usr/include/poppler/qt5/
#VPATH += ./sqfsfuse/
#VPATH += ./common/
#VPATH += /usr/include/glib-2.0/
#VPATH += /usr/lib/glib-2.0/include/
#VPATH += /usr/include/squashfuse/
#VPATH += /usr/include/afflib/
#VPATH += ../scalpel/src/
#VPATH += ../revit07-20070804/src/definitions_parser/
#VPATH += ../revit07-20070804/src/input_analyzer/
#VPATH += ../revit07-20070804/src/input_handler/
#VPATH += ../revit07-20070804/src/notify/
#VPATH += ../revit07-20070804/src/media_access/
#VPATH += ../revit07-20070804/src/output_handler/
#VPATH += ../revit07-20070804/src/state_hierarchy/
#VPATH += ../revit07-20070804/src/
FORMS = aboutbox.ui accesseddatefilter.ui analysistypefilter.ui byteconverter.ui changeddatefilter.ui createddatefilter.ui digdeeperdialog.ui exportdialog.ui filecategoryfilter.ui filetypefilter.ui fileviewer.ui hashcategoryfilter.ui htmlviewer.ui idfilter.ui imageviewer.ui imagewindow.ui jumpto.ui messageviewer.ui modifieddatefilter.ui namefilter.ui pathfilter.ui propertieswindow.ui settingsdialog.ui sizefilter.ui textviewer.ui videoviewer.ui viewermanager.ui wombatforensics.ui jumphex.ui addevidencedialog.ui remevidencedialog.ui tagfilter.ui tagmanager.ui searchdialog.ui carvedialog.ui filecarvingdialog.ui carvingfiletypesdialog.ui pdfviewer.ui registryviewer.ui newforimg.ui verifyevidencedialog.ui
HEADERS = wombatforensics.h wombatvariable.h wombatfunctions.h exportdialog.h tskvariable.h globals.h wombatinclude.h propertieswindow.h fileviewer.h filterviews.h imageviewer.h videoviewer.h viewermanager.h textviewer.h htmlviewer.h messageviewer.h byteconverter.h digdeeperdialog.h aboutbox.h qhexedit.h chunks.h commands.h settingsdialog.h addevidencedialog.h remevidencedialog.h tskcpp.h tagmanager.h searchdialog.h carvedialog.h filecarvingdialog.h carvingfiletypesdialog.h pdfviewer.h carving.h digging.h reporting.h artifactparser.h exporting.h properties.h newcase.h opencase.h registryviewer.h newforimg.h imagefunctions.h tagging.h verevidencedialog.h affuse.h ewffuse.h sqfuse.h zmgfuse.h zmgfs.c makezmg.h
SOURCES = main.cpp wombatforensics.cpp wombatfunctions.cpp exportdialog.cpp globals.cpp propertieswindow.cpp fileviewer.cpp filterviews.cpp imageviewer.cpp videoviewer.cpp viewermanager.cpp textviewer.cpp htmlviewer.cpp messageviewer.cpp byteconverter.cpp digdeeperdialog.cpp aboutbox.cpp qhexedit.cpp chunks.cpp commands.cpp settingsdialog.cpp addevidencedialog.cpp remevidencedialog.cpp tskcpp.cpp tagmanager.cpp searchdialog.cpp carvedialog.cpp filecarvingdialog.cpp carvingfiletypesdialog.cpp pdfviewer.cpp carving.cpp digging.cpp reporting.cpp artifactparser.cpp exporting.cpp properties.cpp newcase.cpp opencase.cpp registryviewer.cpp newforimg.cpp imagefunctions.cpp tagging.cpp verevidencedialog.cpp
RESOURCES += wombatforensics.qrc
release: DESTDIR = release
debug:   DESTDIR = debug
linux:LIBS = -lbfio -lewf -ltsk -ltar -lffmpegthumbnailer -lMagick++-7.Q16HDRI -lMagickCore-7.Q16HDRI -lQtAVWidgets -lQtAV -llnk -lfwnt -lzip -lpoppler-qt5 -lregf -lcrypto -ludev -lafflib -llzo2 -lz -lsquashfuse `pkg-config fuse3 --cflags --libs`
linux:LIBS += /home/pasquale/Projects/wombatforensics/libzmg.a
#linux:LIBS += /home/pasquale/Projects/wombatforensics/libsquash.a
#linux:LIBS = -lewf -ltsk -ltar -lffmpegthumbnailer -lMagick++-7.Q16HDRI -lMagickCore-7.Q16HDRI -lQtAVWidgets -lQtAV -llnk -lfwnt -lzip -ltre -lcrypto -lz -L. -lscalpel -L./revit.a -lafflib
#linux:LIBS = -lewf -ltsk -ltar -lffmpegthumbnailer -lMagick++-7.Q16HDRI -lMagickCore-7.Q16HDRI -lQtAVWidgets -lQtAV -ltre -L. -lscalpel -L/usr/local/lib/ldefinitions_parser -L/usr/local/lib/linput_analyzer -L/usr/local/lib/linput_handler -L/usr/local/lib/lmedia_access -L/usr/local/lib/loutput_handler -L/usr/local/lib/lstate_hierarchy -L/usr/local/lib/lnotify
if(!debug_and_release|build_pass):CONFIG(debug, debug|release) {
}
#linux:
#install
target.path = /usr/local/bin
#target.path = ./
INSTALLS += target

