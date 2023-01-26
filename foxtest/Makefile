OBJECTS = icons.o wombatforensics.o managetags.o aboutbox.o viewer.o common.o settings.o managecarving.o

ICONS = resources/folder-open.png resources/help-contents.png resources/managetags.png resources/paperairplane2.png resources/reportpreview1.png resources/wombat_32.ico resources/bookmark.png resources/bookmark-new.png resources/bookmark-rem.png resources/aboutwombat.png resources/burrow.png resources/document-new.png resources/document-save.png resources/list-add.png resources/list-rem.png resources/viewmanage.png resources/messagelog.png resources/settings.png resources/byteconverter.png resources/carving.png resources/digdeeper.png resources/exportfiles.png resources/exportwli.png resources/hashlist.png resources/imgvidthumbs.png resources/jumptohex.png resources/searchhex.png resources/verifywfi.png resources/xchomp.png resources/createwfi.png resources/managecarved.png

# Generate header file and source file containing declarations and data arrays
icons: $(ICONS) icons.h
	reswrap --header -o icons.h $(ICONS) --source -i icons.h -o icons.cpp $(ICONS)

# Link it all together

wombatforensics: $(OBJECTS)
	g++ -O3 -o wombatforensics $(OBJECTS) -lX11 -lXext -lXft -lfontconfig -lfreetype -lXcursor -lXrender -lXrandr -lXfixes -lXi -lGLU -lGL -ldl -lpthread -ljpeg -lrt -lpng -ltiff -lz -lbz2 -ltar libFOX-1.7.a

wombatforensics.o: wombatforensics.cpp icons.cpp managetags.cpp aboutbox.cpp viewer.cpp common.cpp settings.cpp managecarving.cpp
