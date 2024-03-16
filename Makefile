OBJECTS = icons.o wombatforensics.o managetags.o aboutbox.o viewer.o common.o settings.o managecarving.o evidencemanager.o forensicimage.o partitions.o directories.o manageviewer.o messagelog.o filters.o filterview.o digdeeper.o managehashlist.o hexviewer.o artifactparser.o pdfviewer.o propviewer.o imageviewer.o fat12.o fatcommon.o thumbviewer.o fat16.o fat32.o exfat.o extfs.o ntfs.o ntfscommon.o

WALAFUS_OBJS = walafus/wltg_reader.cpp walafus/filesystem.cpp walafus/wltg_internal_readers.cpp walafus/wltg_packer.cpp walafus/wltg_internal_writers.cpp

ICONS = resources/folder-open.png resources/help-contents.png resources/managetags.png resources/paperairplane2.png resources/reportpreview1.png resources/wombat_32.ico resources/bookmark.png resources/bookmark-new.png resources/bookmark-rem.png resources/aboutwombat.png resources/burrow.png resources/document-new.png resources/document-save.png resources/viewmanage.png resources/messagelog.png resources/settings.png resources/byteconverter.png resources/carving.png resources/digdeeper.png resources/exportfiles.png resources/exportwli.png resources/hashlist.png resources/jumptohex.png resources/searchhex.png resources/xchomp.png resources/managecarved.png resources/evidencemanager.png resources/forimg.png resources/carvedfile.png resources/defaultfile.png resources/defaultfolder.png resources/deletedfile.png resources/deletedfolder.png resources/partition.png resources/virtualfile.png resources/virtualfolder.png resources/partition-path.png resources/folder-path.png resources/file-path.png resources/binmenu.png resources/check.png resources/uncheck.png resources/filter.png resources/fileprop.png resources/filehex.png resources/back16.png resources/frwd16.png resources/thumbmissing.png resources/videoerror.png resources/imagethumb.png resources/videothumb.png resources/forimgpath.png

# Generate header file and source file containing declarations and data arrays
icons: $(ICONS) icons.h
	reswrap --header -o icons.h $(ICONS) --source -i icons.h -o icons.cpp $(ICONS)

blake3: 
	cd blake3
	make blake3
	cd ..

# Link it all together
# If i don't pull text from pdf's or html i can get rid of poppler and liblexbor_static
wombatforensics: $(OBJECTS) $(WALAFUS_OBJS)
	g++ -O3 -o wombatforensics $(OBJECTS) $(WALAFUS_OBJS) -lX11 -lXext -lXft -lfontconfig -lfreetype -lXcursor -lXrender -lXrandr -lXfixes -lXi -lGLU -lGL -ldl -lpthread -ljpeg -lrt -lpng -ltiff -lheif -lz -lbz2 -ltar -lmagic -lzip -lpoppler-cpp -lsnappy -lraptor2 -llz4 -lzstd -lcrypto -lffmpegthumbnailer -llzma libfwsi.a libfwnt.a libhmac.a libewf.a libaff4.a libsmraw.a libvhdi.a libqcow.a libvmdk.a libphdi.a liblexbor_static.a blake3/libblake3.a libsquashfs.a libFOX-1.7.a

wombatforensics.o: wombatforensics.cpp icons.cpp managetags.cpp aboutbox.cpp viewer.cpp common.cpp settings.cpp managecarving.cpp evidencemanager.cpp forensicimage.cpp partitions.cpp directories.cpp manageviewer.cpp messagelog.cpp filters.cpp filterview.cpp digdeeper.cpp managehashlist.cpp hexviewer.cpp artifactparser.cpp pdfviewer.cpp propviewer.cpp imageviewer.cpp fat12.cpp fatcommon.cpp thumbviewer.cpp nanosvg.h nanosvgrast.h stb_image_write.h cimg/CImg.h fat16.cpp fat32.cpp exfat.cpp extfs.cpp ntfs.cpp ntfscommon.cpp
