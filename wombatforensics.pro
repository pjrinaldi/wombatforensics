QT += widgets core concurrent multimedia multimediawidgets gui avwidgets # webengine
#QT -= opengl quick network qml
linux:CONFIG += debug_and_release debug_and_release_target qt x11 build_all c++14 
DEFINES += MAGICKCORE_HDRI_ENABLE=0
DEFINES += MAGICKCORE_QUANTUM_DEPTH=16
INCLUDEPATH += /usr/include/libffmpegthumbnailer/
INCLUDEPATH += /usr/include/ImageMagick-7/
INCLUDEPATH += /usr/local/lib/
INCLUDEPATH += /usr/include/poppler/qt5/
INCLUDEPATH += /usr/local/include/
VPATH += /usr/include/parted/
VPATH += /usr/include/ImageMagick-7/
VPATH += /usr/local/lib/
VPATH += /usr/include/poppler/qt5/
VPATH += /usr/local/include/
FORMS = aboutbox.ui accesseddatefilter.ui analysistypefilter.ui byteconverter.ui changeddatefilter.ui createddatefilter.ui digdeeperdialog.ui exportdialog.ui filecategoryfilter.ui filetypefilter.ui fileviewer.ui hashcategoryfilter.ui htmlviewer.ui idfilter.ui imageviewer.ui imagewindow.ui jumpto.ui messageviewer.ui modifieddatefilter.ui namefilter.ui pathfilter.ui propertieswindow.ui settingsdialog.ui sizefilter.ui textviewer.ui videoviewer.ui viewermanager.ui wombatforensics.ui jumphex.ui addevidencedialog.ui remevidencedialog.ui tagfilter.ui tagmanager.ui searchdialog.ui carvedialog.ui filecarvingdialog.ui carvingfiletypesdialog.ui pdfviewer.ui registryviewer.ui newforimg.ui verifyevidencedialog.ui hashlistmanager.ui hashfilter.ui hashmatchfilter.ui mboxviewer.ui
HEADERS = wombatforensics.h wombatvariable.h wombatfunctions.h exportdialog.h globals.h wombatinclude.h propertieswindow.h fileviewer.h filterviews.h imageviewer.h videoviewer.h viewermanager.h textviewer.h htmlviewer.h messageviewer.h byteconverter.h digdeeperdialog.h aboutbox.h qhexedit.h chunks.h commands.h settingsdialog.h addevidencedialog.h remevidencedialog.h tagmanager.h searchdialog.h carvedialog.h filecarvingdialog.h carvingfiletypesdialog.h pdfviewer.h carving.h digging.h reporting.h artifactparser.h exporting.h opencase.h registryviewer.h newforimg.h imagefunctions.h tagging.h verevidencedialog.h blake3.h hashlistmanager.h cssstrings.h commonfs.h extfs.h exfatfs.h newcase.h fatfs.h ntfs.h hfs.h hfsplus.h mboxviewer.h bitlocker.h xfs.h apfs.h
SOURCES = main.cpp wombatforensics.cpp wombatfunctions.cpp exportdialog.cpp globals.cpp propertieswindow.cpp fileviewer.cpp filterviews.cpp imageviewer.cpp videoviewer.cpp viewermanager.cpp textviewer.cpp htmlviewer.cpp messageviewer.cpp byteconverter.cpp digdeeperdialog.cpp aboutbox.cpp qhexedit.cpp chunks.cpp commands.cpp settingsdialog.cpp addevidencedialog.cpp remevidencedialog.cpp tagmanager.cpp searchdialog.cpp carvedialog.cpp filecarvingdialog.cpp carvingfiletypesdialog.cpp pdfviewer.cpp carving.cpp digging.cpp reporting.cpp artifactparser.cpp exporting.cpp commonfs.cpp extfs.cpp newcase.cpp opencase.cpp registryviewer.cpp newforimg.cpp imagefunctions.cpp tagging.cpp verevidencedialog.cpp hashlistmanager.cpp cssstrings.cpp exfatfs.cpp fatfs.cpp ntfs.cpp hfs.cpp hfsplus.cpp mboxviewer.cpp bitlocker.cpp xfs.cpp apfs.cpp
RESOURCES += wombatforensics.qrc
release: DESTDIR = release
debug:   DESTDIR = debug
linux:LIBS = -lewf -ltar -lffmpegthumbnailer -lMagick++-7.Q16HDRI -lMagickCore-7.Q16HDRI -lQtAVWidgets -lQtAV -lmagic -llnk -lfwnt -lzip -lpoppler-qt5 -lregf -ludev -lafflib -llzo2 -lz -llz4 libblake3.a

if(!debug_and_release|build_pass):CONFIG(debug, debug|release) {
}
#linux:
#install
target.path = /usr/local/bin
#target.path = ./
INSTALLS += target
