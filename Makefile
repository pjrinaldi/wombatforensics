all: buildall

buildall:
	@echo "looking into evidence"
	(cd libraries/evidence; $(MAKE))
	@echo "looking into basictools"
	(cd libraries/basictools; $(MAKE))
	@echo "looking into sleuthkit"
	(cd libraries/sleuthkit; $(MAKE))
	@echo "looking into main"
	(cd main; $(MAKE))
