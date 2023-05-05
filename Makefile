OBJECTS = icons.o wombatforensics.o managetags.o aboutbox.o viewer.o common.o settings.o managecarving.o evidencemanager.o forensicimage.o partitions.o directories.o manageviewer.o messagelog.o filters.o filterview.o digdeeper.o managehashlist.o hexviewer.o artifactparser.o pdfviewer.o propviewer.o imageviewer.o fat12.o thumbviewer.o

ICONS = resources/folder-open.png resources/help-contents.png resources/managetags.png resources/paperairplane2.png resources/reportpreview1.png resources/wombat_32.ico resources/bookmark.png resources/bookmark-new.png resources/bookmark-rem.png resources/aboutwombat.png resources/burrow.png resources/document-new.png resources/document-save.png resources/viewmanage.png resources/messagelog.png resources/settings.png resources/byteconverter.png resources/carving.png resources/digdeeper.png resources/exportfiles.png resources/exportwli.png resources/hashlist.png resources/jumptohex.png resources/searchhex.png resources/xchomp.png resources/managecarved.png resources/evidencemanager.png resources/forimg.png resources/carvedfile.png resources/defaultfile.png resources/defaultfolder.png resources/deletedfile.png resources/deletedfolder.png resources/partition.png resources/virtualfile.png resources/virtualfolder.png resources/partition-path.png resources/folder-path.png resources/file-path.png resources/binmenu.png resources/check.png resources/uncheck.png resources/filter.png resources/fileprop.png resources/filehex.png resources/back16.png resources/frwd16.png resources/thumbmissing.png resources/videoerror.png resources/imagethumb.png resources/videothumb.png

# Generate header file and source file containing declarations and data arrays
icons: $(ICONS) icons.h
	reswrap --header -o icons.h $(ICONS) --source -i icons.h -o icons.cpp $(ICONS)

blake3: 
	cd blake3
	make blake3
	cd ..

# Link it all together
# If i don't pull text from pdf's or html i can get rid of poppler and liblexbor_static
wombatforensics: $(OBJECTS)
	g++ -I/usr/include/ImageMagick-7/ -DMAGICKCORE_HDRI_ENABLE=0 -DMAGICKCORE_QUANTUM_DEPTH=16 -O3 -o wombatforensics $(OBJECTS) -lX11 -lXext -lXft -lfontconfig -lfreetype -lXcursor -lXrender -lXrandr -lXfixes -lXi -lGLU -lGL -ldl -lpthread -ljpeg -lrt -lpng -ltiff -lz -lbz2 -ltar -lewf -laff4 -lsmraw -lfwsi -lfwnt -lzstd -lmagic -lzip -lpoppler-cpp -lffmpegthumbnailer -lMagick++-7.Q16HDRI -lMagickCore-7.Q16HDRI -lheif liblexbor_static.a blake3/libblake3.a libFOX-1.7.a


wombatforensics.o: wombatforensics.cpp icons.cpp managetags.cpp aboutbox.cpp viewer.cpp common.cpp settings.cpp managecarving.cpp evidencemanager.cpp forensicimage.cpp partitions.cpp directories.cpp manageviewer.cpp messagelog.cpp filters.cpp filterview.cpp digdeeper.cpp managehashlist.cpp hexviewer.cpp artifactparser.cpp pdfviewer.cpp propviewer.cpp imageviewer.cpp fat12.cpp thumbviewer.cpp nanosvg.h nanosvgrast.h stb_image_write.h
