DIRS = libraries main

evidence: 
$(ECHO) looking into evidence : $(MAKE) $(MFLAGS)
cd libraries/evidence; $(MAKE) $(MFLAGS)

basictools:
$(ECHO) looking into basictools : $(MAKE) $(MFLAGS)
cd libraries/basictools; $(MAKE) $(MFLAGS)

sleuthkit:
$(ECHO) looking into sleuthkit : $(MAKE) $(MFLAGS)
cd libraries/sleuthkit; $(MAKE) $(MFLAGS)

main:
$(ECHO) looing into main : $(MAKE) $(MFLAGS)
cd main; $(MAKE) $(MFLAGS)

