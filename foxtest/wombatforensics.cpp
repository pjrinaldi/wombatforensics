#include "wombatforensics.h"

FXIMPLEMENT(WombatForensics,FXMainWindow,WombatForensicsMap,ARRAYNUMBER(WombatForensicsMap))

WombatForensics::WombatForensics(FXApp* a):FXMainWindow(a, "Wombat Forensics", new FXICOIcon(a, wombat_32), new FXICOIcon(a, wombat_32), DECOR_ALL, 0, 0, 1640, 768)
{
    mainframe = new FXVerticalFrame(this, LAYOUT_TOP|LAYOUT_FILL_X|LAYOUT_FILL_Y, 0, 0, 0, 0, 0, 0, 0, 0);
    toolbar = new FXToolBar(mainframe, this, LAYOUT_TOP|LAYOUT_LEFT);
    pathtoolbar = new FXToolBar(mainframe, this, LAYOUT_TOP|LAYOUT_LEFT);
    hsplitter = new FXSplitter(mainframe, SPLITTER_VERTICAL|LAYOUT_LEFT|LAYOUT_FILL_X|LAYOUT_FILL_Y);
    tablelist = new FXTable(hsplitter, this, ID_TABLESELECT, TABLE_COL_SIZABLE|LAYOUT_LEFT|LAYOUT_FILL_X|LAYOUT_FILL_Y);
    tablelist->setTableSize(10, 14);
    tablelist->setEditable(false);
    tablelist->setColumnText(0, "");
    tablelist->setColumnText(1, "ID");
    tablelist->setColumnText(2, "Name");
    tablelist->setColumnText(3, "Path");
    tablelist->setColumnText(4, "Size (bytes)");
    tablelist->setColumnText(5, "Created");
    tablelist->setColumnText(6, "Accessed");
    tablelist->setColumnText(7, "Modified");
    tablelist->setColumnText(8, "Changed");
    tablelist->setColumnText(9, "Hash");
    tablelist->setColumnText(10, "Category");
    tablelist->setColumnText(11, "Signature");
    tablelist->setColumnText(12, "Tagged");
    tablelist->setColumnText(13, "Hash Match");
    tablelist->setColumnHeaderHeight(tablelist->getColumnHeaderHeight() + 5);
    tablelist->setRowHeaderWidth(0);
    tablelist->setHeight(this->getHeight() / 2);
    plainfont = new FXFont(a, "monospace");
    plaintext = new FXText(hsplitter, this, ID_HEXTEXT, LAYOUT_LEFT|LAYOUT_FILL_X|LAYOUT_FILL_Y);
    plaintext->setFont(plainfont);
    plaintext->setEditable(false);
    statusbar = new FXStatusBar(mainframe, LAYOUT_BOTTOM|LAYOUT_LEFT|LAYOUT_FILL_X);
    // TOOLBAR ICONS
    // WOMBAT CASE FILE ICONS
    newicon = new FXPNGIcon(this->getApp(), documentnew);
    newbutton = new FXButton(toolbar, "", newicon, this, ID_NEW, BUTTON_TOOLBAR|FRAME_RAISED);
    newbutton->setTipText("Create New Case");
    openicon = new FXPNGIcon(this->getApp(), folderopen);
    openbutton = new FXButton(toolbar, "", openicon, this, ID_OPEN, BUTTON_TOOLBAR|FRAME_RAISED);
    saveicon = new FXPNGIcon(this->getApp(), documentsave);
    savebutton = new FXButton(toolbar, "", saveicon, this, ID_SAVE, BUTTON_TOOLBAR|FRAME_RAISED);
    new FXVerticalSeparator(toolbar);
    // EVIDENCE ICONS
    evidmanicon = new FXPNGIcon(this->getApp(), evidencemanager);
    evidmanbutton = new FXButton(toolbar, "", evidmanicon, this, ID_EVIDMANAGE, BUTTON_TOOLBAR|FRAME_RAISED);
    /*
    listaddicon = new FXPNGIcon(this->getApp(), listadd);
    listaddbutton = new FXButton(toolbar, "", listaddicon, this, ID_ADDEVID, BUTTON_TOOLBAR|FRAME_RAISED);
    listremicon = new FXPNGIcon(this->getApp(), listrem);
    listrembutton = new FXButton(toolbar, "", listremicon, this, ID_REMEVID, BUTTON_TOOLBAR|FRAME_RAISED);
    */
    new FXVerticalSeparator(toolbar);
    // WOMBAT FORENSIC APP ICONS
    settingsicon = new FXPNGIcon(this->getApp(), settings);
    settingsbutton = new FXButton(toolbar, "", settingsicon, this, ID_SETTINGS, BUTTON_TOOLBAR|FRAME_RAISED);
    managecarvedicon = new FXPNGIcon(this->getApp(), managecarved);
    managecarvedbutton = new FXButton(toolbar, "", managecarvedicon, this, ID_MANAGECARVED, BUTTON_TOOLBAR|FRAME_RAISED);
    viewmanageicon = new FXPNGIcon(this->getApp(), viewmanage);
    viewmanagebutton = new FXButton(toolbar, "", viewmanageicon, this, ID_VIEWMANAGE, BUTTON_TOOLBAR|FRAME_RAISED);
    messagelogicon = new FXPNGIcon(this->getApp(), messagelog);
    messagelogbutton = new FXButton(toolbar, "", messagelogicon, this, ID_MSGLOG, BUTTON_TOOLBAR|FRAME_RAISED);
    new FXVerticalSeparator(toolbar);
    // ARTIFACT ICONS
    imgvidthumbicon = new FXPNGIcon(this->getApp(), imgvidthumbs);
    imgvidthumbbutton = new FXButton(toolbar, "", imgvidthumbicon, this, ID_IMGVIDTHUMB, BUTTON_TOOLBAR|FRAME_RAISED);
    digdeepericon = new FXPNGIcon(this->getApp(), digdeeper);
    digdeeperbutton = new FXButton(toolbar, "", digdeepericon, this, ID_DIGDEEPER, BUTTON_TOOLBAR|FRAME_RAISED);
    carvingicon = new FXPNGIcon(this->getApp(), carving);
    carvingbutton = new FXButton(toolbar, "", carvingicon, this, ID_CARVING, BUTTON_TOOLBAR|FRAME_RAISED);
    exportfilesicon = new FXPNGIcon(this->getApp(), exportfiles);
    exportfilesbutton = new FXButton(toolbar, "", exportfilesicon, this, ID_EXPORTFILES, BUTTON_TOOLBAR|FRAME_RAISED);
    exportwliicon = new FXPNGIcon(this->getApp(), exportwli);
    exportwlibutton = new FXButton(toolbar, "", exportwliicon, this, ID_EXPORTWLI, BUTTON_TOOLBAR|FRAME_RAISED);
    new FXVerticalSeparator(toolbar);
    // REPORTING ICONS
    managetagsicon = new FXPNGIcon(this->getApp(), managetags);
    managetagsbutton = new FXButton(toolbar, "", managetagsicon, this, ID_MANAGETAGS, BUTTON_TOOLBAR|FRAME_RAISED);
    managehashicon = new FXPNGIcon(this->getApp(), hashlist);
    managehashbutton = new FXButton(toolbar, "", managehashicon, this, ID_MANAGEHASH, BUTTON_TOOLBAR|FRAME_RAISED);
    previewicon = new FXPNGIcon(this->getApp(), reportpreview1);
    previewbutton = new FXButton(toolbar, "", previewicon, this, ID_PREVIEW, BUTTON_TOOLBAR|FRAME_RAISED);
    publishicon = new FXPNGIcon(this->getApp(), paperairplane2);
    publishbutton = new FXButton(toolbar, "", publishicon, this, ID_PUBLISH, BUTTON_TOOLBAR|FRAME_RAISED);
    new FXVerticalSeparator(toolbar);
    // IMAGING ICONS
    /*
    createwfiicon = new FXPNGIcon(this->getApp(), createwfi);
    createwfibutton = new FXButton(toolbar, "", createwfiicon, this, ID_CREATEWFI, BUTTON_TOOLBAR|FRAME_RAISED);
    verifywfiicon = new FXPNGIcon(this->getApp(), verifywfi);
    verifywfibutton = new FXButton(toolbar, "", verifywfiicon, this, ID_VERIFYWFI, BUTTON_TOOLBAR|FRAME_RAISED);
    new FXVerticalSeparator(toolbar);
    */
    // ABOUT ICONS
    xchompicon = new FXPNGIcon(this->getApp(), xchomp);
    xchompbutton = new FXButton(toolbar, "", xchompicon, this, ID_XCHOMP, BUTTON_TOOLBAR|FRAME_RAISED);
    abouticon = new FXPNGIcon(this->getApp(), helpcontents);
    aboutbutton = new FXButton(toolbar, "", abouticon, this, ID_ABOUT, BUTTON_TOOLBAR|FRAME_RAISED);
    // PATH TOOLBAR HOME ICON
    burrowicon = new FXPNGIcon(this->getApp(), burrow);
    burrowbutton = new FXButton(pathtoolbar, "", burrowicon, this, ID_HOME, BUTTON_TOOLBAR|FRAME_RAISED);

    // ITEM TYPE ICONS
    forimgicon = new FXPNGIcon(this->getApp(), forimg);
    forimgicon->create();
    carvedfileicon = new FXPNGIcon(this->getApp(), carvedfile);
    carvedfileicon->create();
    defaultfileicon = new FXPNGIcon(this->getApp(), defaultfile);
    defaultfileicon->create();
    defaultfoldericon = new FXPNGIcon(this->getApp(), defaultfolder);
    defaultfoldericon->create();
    deletedfileicon = new FXPNGIcon(this->getApp(), deletedfile);
    deletedfileicon->create();
    deletedfoldericon = new FXPNGIcon(this->getApp(), deletedfolder);
    deletedfoldericon->create();
    partitionicon = new FXPNGIcon(this->getApp(), partition);
    partitionicon->create();
    virtualfileicon = new FXPNGIcon(this->getApp(), virtualfile);
    virtualfileicon->create();
    virtualfoldericon = new FXPNGIcon(this->getApp(), virtualfolder);
    virtualfoldericon->create();

    statusbar->getStatusLine()->setNormalText("Open a Forensic Image, Device, or File to Begin");
    fileuserdata.clear();
    //sqlitefiles.clear();
    tags.clear();
    taggedlist.clear();
    iscaseopen = false;
    prevevidpath = "";
    homepath = FXString(getenv("HOME")) + "/";
    configpath = homepath + ".wombatforensics/";
    tmppath = "/tmp/";
    FXDir::create(configpath);
    bool issettings = settingfile.open(configpath + "settings", FXIO::Reading, FXIO::OwnerReadWrite);
    if(issettings == false)
    {
        // CREATE SETTINGS FILE AND LOAD DEFAULT SETTINGS TO THE FILE
        settingfile.close();
        FXFile::create(configpath + "settings", FXIO::OwnerReadWrite);
        settingfile.open(configpath + "settings", FXIO::Writing, FXIO::OwnerReadWrite);
        currentsettings = "128|25|/home/pasquale/WombatCases/|/home/pasquale/Reports/|America/New_York|20";
        settingfile.writeBlock(currentsettings.text(), currentsettings.length());
        settingfile.close();
    }
    else
    {
        settingfile.open(configpath + "settings", FXIO::Reading, FXIO::OwnerReadWrite);
        char* oldsettings = new char[settingfile.size()+1];
        settingfile.readBlock(oldsettings, settingfile.size());
        oldsettings[settingfile.size()] = 0;
        settingfile.close();
        currentsettings = FXString(oldsettings);
    }
    //std::cout << "casepath: " << GetSettings(2).text() << std::endl;
    bool iscarvetypes = carvetypesfile.open(configpath + "carvetypes", FXIO::Reading, FXIO::OwnerReadWrite);
    if(iscarvetypes == false)
    {
	carvetypesfile.close();
	FXFile::create(configpath + "carvetypes", FXIO::OwnerReadWrite);
	carvetypesfile.open(configpath + "carvetypes", FXIO::Writing, FXIO::OwnerReadWrite);
	currentcarvetypes = "Image,JPEG,FFD8,FFD9,JPG,2500000\nImage,PNG,89504E470D0A1A0A,49454E44AE426082,PNG,2500000\nImage,GIF,47494638??61,003B,GIF,2500000\nDocument,PDF,25504446,2525454F46,PDF,2500000\nVideo,MPEG-1/2,000001B?,000001B?,MPG,5000000\n";
	carvetypesfile.writeBlock(currentcarvetypes.text(), currentcarvetypes.length());
	carvetypesfile.close();
    }
    else
    {
	carvetypesfile.open(configpath + "carvetypes", FXIO::Reading, FXIO::OwnerReadWrite);
	char* oldcarvetypes = new char[carvetypesfile.size()+1];
	carvetypesfile.readBlock(oldcarvetypes, carvetypesfile.size());
        oldcarvetypes[carvetypesfile.size()] = 0;
	carvetypesfile.close();
	currentcarvetypes = FXString(oldcarvetypes);
    }
    forimgvector.clear();
    savebutton->disable();
    evidmanbutton->disable();
    imgvidthumbbutton->disable();
    digdeeperbutton->disable();
    carvingbutton->disable();
    exportfilesbutton->disable();
    exportwlibutton->disable();
    managetagsbutton->disable();
    managehashbutton->disable();
    previewbutton->disable();
    publishbutton->disable();
}


WombatForensics::~WombatForensics()
{
    //SaveCurrentCase();
    CloseCurrentCase();
    forimgicon->destroy();
    carvedfileicon->destroy();
    defaultfileicon->destroy();
    defaultfoldericon->destroy();
    deletedfileicon->destroy();
    deletedfoldericon->destroy();
    partitionicon->destroy();
    virtualfileicon->destroy();
    virtualfoldericon->destroy();
}

void WombatForensics::create()
{
    FXMainWindow::create();
    show(PLACEMENT_SCREEN);
}

FXString WombatForensics::GetSettings(int setting)
{
    FXArray<FXint> posarray;
    posarray.clear();
    int found = 0;
    posarray.append(-1);
    while(found > -1)
    {
        found = currentsettings.find("|", found+1);
        if(found > -1)
            posarray.append(found);
    }
    posarray.append(currentsettings.length());
    if(setting < posarray.no())
        return currentsettings.mid(posarray.at(setting)+1, posarray.at(setting+1) - posarray.at(setting) - 1);
    else
        return "";
}

long WombatForensics::NewCase(FXObject*, FXSelector, void*)
{
    if(iscaseopen)
    {
        FXuint result = FXMessageBox::question(this, MBOX_YES_NO, "Existing Case Status", "There is a case already open. Are you sure you want to close it?"); // no is 2, yes is 1
        if(result == 1) // YES
        {
            CloseCurrentCase();
            iscaseopen = false;
        }
        else // NO
            return 1;
    }
    StatusUpdate("Creating New Case...");
    casename = "";
    bool isset = FXInputDialog::getString(casename, this, "Enter Case Name", "New Case");
    if(isset)
    {
        this->getApp()->beginWaitCursor();
        iscaseopen = true;
        tmppath = "/tmp/wf/" + casename + "/";
        this->setTitle("Wombat Forensics - " + casename);
        FXDir::create("/tmp/wf/");
        FXDir::create(tmppath);
        FXDir::create(tmppath + "carved/");
        FXDir::create(tmppath + "archives/");
        FXDir::create(tmppath + "hashlists/");
        FXDir::create(tmppath + "mailboxes/");
        FXDir::create(tmppath + "thumbs/");
        FXFile::create(tmppath + "tags", FXIO::OwnerReadWrite);
        FXFile::create(tmppath + "msglog", FXIO::OwnerReadWrite);
        LogEntry("Case Structure Created Successfully");
        EnableCaseButtons();
        this->getApp()->endWaitCursor();
        StatusUpdate("Ready");
        /*
        // CHECK TEST AND IT WORKS WITH ID IN CHECK
        CheckTableItem* checkitem = new CheckTableItem(tablelist, NULL, NULL, "e0-p0-f1");
        tablelist->setItem(0, 0, checkitem);
        tablelist->fitColumnsToContents(0);
        tablelist->setColumnWidth(0, tablelist->getColumnWidth(0) + 25);
        // CHECK TEST AND IT WORKS WITHOUT ID IN CHECK
        CheckTableItem* checkitem = new CheckTableItem(tablelist, NULL, NULL, "");
        tablelist->setItem(0, 0, checkitem);
        tablelist->fitColumnsToContents(0);
        tablelist->setColumnWidth(0, tablelist->getColumnWidth(0) + 25);
        */
    }
    /*
    // create new case here
        QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
        qInfo() << "Bookmarks File Created";
        ReadBookmarks();
        ReadHashLists();
        msglog->clear();
        qInfo() << "Log File Created";
        //LogMessage("Log File Created");
        InitializeCheckState();
        ui->actionAdd_Evidence->setEnabled(true);
        ui->actionpreviewreport->setEnabled(true);
        ui->actionBookmark_Manager->setEnabled(true);
        ui->actionpublishresults->setEnabled(true);

        ui->actionHashListManager->setEnabled(true);
        qInfo() << "Case was Created";
        //LogMessage("Case was Created");
        QApplication::restoreOverrideCursor();
        StatusUpdate("Ready");
        autosavetimer->start(autosave * 60000); // 10 minutes in milliseconds for a general setting for real.
     */ 
    return 1;
}

long WombatForensics::OpenCase(FXObject*, FXSelector, void*)
{
    if(iscaseopen)
    {
        FXuint result = FXMessageBox::question(this, MBOX_YES_NO, "Existing Case Status", "There is a case already open. Are you sure you want to close it?"); // no is 2, yes is 1
        if(result == 1) // YES
        {
            CloseCurrentCase();
            iscaseopen = false;
        }
        else // NO
            return 1;
    }
    FXString casefilename = FXFileDialog::getOpenFilename(this, "Open Wombat Case File", GetSettings(2), "*.wfc");
    int found = casefilename.rfind("/");
    int rfound = casefilename.rfind(".");
    casename = casefilename.mid(found+1, rfound - found - 1);
    //std::cout << "open casename: " << casename.text() << std::endl;
    if(!casefilename.empty())
    {
        this->getApp()->beginWaitCursor();
	StatusUpdate("Case Opening...");
	this->setTitle("Wombat Forensics - " + casename);
        FXDir::create("/tmp/wf/");
	tmppath = tmppath + "wf/";
	// BEGIN UNTAR METHOD
	TAR* tarhandle;
	tar_open(&tarhandle, casefilename.text(), NULL, O_RDONLY, 0644, TAR_GNU);
	tar_extract_all(tarhandle, tmppath.text());
	tar_close(tarhandle);
	// END UNTAR METHOD
	iscaseopen = true;
	tmppath = tmppath + casename + "/";
	//std::cout << tmppath.text() << std::endl;
	LogEntry("Case was Opened Successfully");
        EnableCaseButtons();
        LoadCaseState();
        this->getApp()->endWaitCursor();
	StatusUpdate("Ready");
    }
    return 1;
}

long WombatForensics::SaveCase(FXObject*, FXSelector, void*)
{
    SaveCurrentCase();
    return 1;
}

void WombatForensics::SaveCurrentCase()
{
    // BEGIN TAR METHOD
    FXDir::create(GetSettings(2));
    //std::cout << "save casename:" << casename.text() << std::endl;
    FXString tmptar = GetSettings(2) + casename + ".wfc";
    //std::cout << "tmptar: " << tmptar.text() << std::endl;
    FXFile tarfile;
    bool istarfile = tarfile.open(tmptar, FXIO::Reading, FXIO::OwnerReadWrite);
    if(istarfile)
    {
        FXString oldtmptar = tmptar + ".old";
        rename(tmptar.text(), oldtmptar.text());
    }
    TAR* casehandle;
    tar_open(&casehandle, tmptar.text(), NULL, O_WRONLY | O_CREAT, 0644, TAR_GNU);
    tar_append_tree(casehandle, tmppath.text(), FXString("./" + casename).text());
    tar_close(casehandle);
    if(istarfile)
    {
        FXString oldtmptar = tmptar + ".old";
        std::remove(oldtmptar.text());
    }
    // END TAR METHOD
}

void WombatForensics::CloseCurrentCase()
{
    this->setTitle("Wombat Forensics");
    if(iscaseopen)
    {
	SaveCurrentCase();
	StatusUpdate("Wombat Case File Saved");
	// REMOVE /TMP/WF/CASENAME DIRECTORY
	std::filesystem::path tpath(tmppath.text());
	std::uintmax_t removecount = std::filesystem::remove_all(tpath);
	//homepath = FXString(getenv("HOME")) + "/";
	tmppath = "/tmp/";
	StatusUpdate("Case Successfully Closed");
    }
}

void WombatForensics::EnableCaseButtons()
{   
    savebutton->enable();
    evidmanbutton->enable();
    imgvidthumbbutton->enable();
    digdeeperbutton->enable();
    carvingbutton->enable();
    exportfilesbutton->enable();
    exportwlibutton->enable();
    managetagsbutton->enable();
    managehashbutton->enable();
    previewbutton->enable();
    publishbutton->enable();
}

void WombatForensics::LoadCaseState(void)
{
    // LOAD EVIDENCE
     
    evidencelist = "";
    filebuffer.open(FXString(tmppath + "evidence").text(), std::ios::in);
    if(filebuffer.is_open())
    {
	filebuffer.seekg(0, filebuffer.beg);
	filebuffer.seekg(0, filebuffer.end);
	uint64_t filesize = filebuffer.tellg();
	char* oldevidence = new char[filesize+1];
	filebuffer.seekg(0, filebuffer.beg);
	filebuffer.read(oldevidence, filesize);
	filebuffer.close();
        oldevidence[filesize] = 0;
	//std::cout << "old evidence: " << oldevidence << std::endl;
        evidencelist = FXString(oldevidence);
	//std::cout << "evidencelist at load: " << evidencelist.text() << std::endl;
    }
    UpdateForensicImages();
}

long WombatForensics::ManageEvidence(FXObject*, FXSelector, void*)
{
    if(prevevidpath.empty())
	prevevidpath = homepath;
    EvidenceManager evidencemanager(this, "Manage Evidence");
    //std::cout << "eivdence list before: " << evidencelist.text() << std::endl;
    evidencemanager.LoadEvidence(evidencelist);
    bool tosave = evidencemanager.execute(PLACEMENT_OWNER);
    if(tosave)
    {
	evidencelist = "";
	evidencelist = evidencemanager.ReturnEvidence();
        //std::cout << "evidence list after:" << evidencelist.text() << std::endl;
	// write evidence to file
	filewriter.open(FXString(tmppath + "evidence").text(), std::ios::out|std::ios::trunc);
	if(filewriter.is_open())
	{
	    filewriter.write(evidencelist.text(), evidencelist.length());
	    filewriter.close();
	}
	UpdateForensicImages();
    }

    return 1;
}

void WombatForensics::UpdateForensicImages()
{
    int found = 0;
    FXArray<FXint> posarray;
    posarray.clear();
    posarray.append(-1);
    while(found > -1)
    {
	found = evidencelist.find('\n', found+1);
	if(found > -1)
	    posarray.append(found);
    }
    forimgvector.clear();
    for(int i=0; i < posarray.no() - 1; i++)
	forimgvector.push_back(new ForImg(evidencelist.mid(posarray.at(i)+1, posarray.at(i+1) - posarray.at(i) - 1).text()));

    tablelist->setTableSize(forimgvector.size(), 14);
    tablelist->setColumnText(0, "");
    tablelist->setColumnText(1, "ID");
    tablelist->setColumnText(2, "Name");
    tablelist->setColumnText(3, "Path");
    tablelist->setColumnText(4, "Size (bytes)");
    tablelist->setColumnText(5, "Created");
    tablelist->setColumnText(6, "Accessed");
    tablelist->setColumnText(7, "Modified");
    tablelist->setColumnText(8, "Changed");
    tablelist->setColumnText(9, "Hash");
    tablelist->setColumnText(10, "Category");
    tablelist->setColumnText(11, "Signature");
    tablelist->setColumnText(12, "Tagged");
    tablelist->setColumnText(13, "Hash Match");
    for(int i=0; i < forimgvector.size(); i++)
    {
        itemtype = 1;
        tablelist->setItem(i, 0, new CheckTableItem(tablelist, NULL, NULL, ""));
        tablelist->setItemData(i, 1, &itemtype);
        tablelist->setItemData(i, 2, forimgvector.at(i));
        tablelist->setItemText(i, 1, FXString::value(i));
        //tablelist->setItemText(i, 1, "e" + FXString::value(i));
        tablelist->setItemText(i, 2, FXString(forimgvector.at(i)->ImageFileName().c_str()));
        tablelist->setItemIcon(i, 2, forimgicon);
        tablelist->setItemIconPosition(i, 2, FXTableItem::BEFORE);
        tablelist->setItemText(i, 3, FXString(forimgvector.at(i)->ImagePath().c_str()));
        tablelist->setItemText(i, 4, FXString(forimgvector.at(i)->SizeString().c_str()));
        /*
        std::cout << "Name: " << forimgvector.at(i)->ImageFileName() << std::endl;
        std::cout << "Path: " << forimgvector.at(i)->ImagePath() << std::endl;
        std::cout << "Size: " << forimgvector.at(i)->Size() << std::endl;
        */
    }
    tablelist->fitColumnsToContents(0);
    tablelist->setColumnWidth(0, tablelist->getColumnWidth(0) + 25);
    FitColumnContents(1);
    FitColumnContents(2);
    FitColumnContents(4);
    AlignColumn(tablelist, 1, FXTableItem::LEFT);
    AlignColumn(tablelist, 2, FXTableItem::LEFT);
    AlignColumn(tablelist, 3, FXTableItem::LEFT);
}

long WombatForensics::TableUpDown(FXObject*, FXSelector, void* ptr)
{
    int currow = tablelist->getCurrentRow();
    switch(((FXEvent*)ptr)->code)
    {
        case KEY_Up:
            tablelist->setCurrentItem(currow - 1, 0, true);
	    tablelist->selectRow(currow - 1, true);
            break;
        case KEY_Down:
            tablelist->setCurrentItem(currow + 1, 0, true);
	    tablelist->selectRow(currow + 1, true);
            break;
    }

    return 1;
}

/*
long WombatForensics::TableUp(FXObject*, FXSelector, void* ptr)
{
    int currow = proptable->getCurrentRow();
    switch(((FXEvent*)ptr)->code)
    {
        case KEY_Up:
            proptable->setCurrentItem(currow - 1, 0, true);
	    proptable->selectRow(currow - 1, true);
            break;
        case KEY_Down:
            proptable->setCurrentItem(currow + 1, 0, true);
	    proptable->selectRow(currow + 1, true);
            break;
    }

    return 1;
}

long WombatForensics::PageChanged(FXObject*, FXSelector, void*)
{
    int cpage = pagespinner->getValue();
    int curitem = sqlfilelist->getCurrentItem();
    if(curitem > -1)
    {
        FXString tmpstr = sqlfilelist->getItemText(sqlfilelist->getCurrentItem());
        int found = tmpstr.find(" (");
        int lfound = tmpstr.find("[");
        int rfound = tmpstr.find("]");
        curfilepath = tmpstr.mid(lfound+1, rfound - lfound - 1) + tmpstr.mid(0, found);
        filebuffer.open(curfilepath.text(), std::ios::in|std::ios::binary);
        filebuffer.seekg(0, filebuffer.beg);
        proptable->setCurrentItem(-1, -1);
        curfileuserdata = fileuserdata.at(curitem);
        lfound = curfileuserdata.find_first_of("|");
        rfound = curfileuserdata.find_last_of("|");
        FXString ftype = curfileuserdata.mid(0, lfound);
        FXString psize = curfileuserdata.mid(lfound+1, rfound-1);
        fileuserdata[curitem] = ftype + "|" + psize + "|" + FXString::value(cpage);
        curpage = cpage;
        LoadPage();
    }

    return 1;
}

long WombatForensics::ScrollChanged(FXObject*, FXSelector, void*)
{
    offsettext->verticalScrollBar()->setPosition(textscrollbar->getPosition()*18, true);
    hextext->verticalScrollBar()->setPosition(textscrollbar->getPosition()*18, true);
    asciitext->verticalScrollBar()->setPosition(textscrollbar->getPosition()*18, true);

    return 1;
}
*/

long WombatForensics::TagMenu(FXObject*, FXSelector, void* ptr)
{
    FXEvent* event = (FXEvent*)ptr;
    if(tablelist->getCurrentRow() > -1 && !tablelist->getItemText(tablelist->getCurrentRow(), 1).empty())
    {
        if(!event->moved)
        {
            FXMenuPane tagmenu(this, POPUP_SHRINKWRAP);
            new FXMenuCommand(&tagmenu, "Create New Tag", new FXPNGIcon(this->getApp(), bookmarknew), this, ID_NEWTAG);
            new FXMenuSeparator(&tagmenu);
            for(int i=0; i < tags.size(); i++)
            {
                new FXMenuCommand(&tagmenu, FXString(tags.at(i).c_str()), new FXPNGIcon(this->getApp(), bookmark), this, ID_SETTAG);
            }
            new FXMenuSeparator(&tagmenu);
            new FXMenuCommand(&tagmenu, "Remove Tag", new FXPNGIcon(this->getApp(), bookmarkrem), this, ID_REMTAG);
            tagmenu.forceRefresh();
            tagmenu.create();
            tagmenu.popup(nullptr, event->root_x, event->root_y);
            getApp()->runModalWhileShown(&tagmenu);
        }
    }
    return 1;
}

/*
long WombatForensics::CreateNewTag(FXObject*, FXSelector, void*)
{
    / *
    FXString tagstr = "";
    bool isset = FXInputDialog::getString(tagstr, this, "Enter Tag Name", "New Tag");
    if(isset)
    {
        tags.push_back(tagstr.text());
        tablelist->setItemText(tablelist->getCurrentRow(), 0, tagstr);
    }
    FXString idkeyvalue = statusbar->getStatusLine()->getText() + "\\" + tablelist->getItemText(tablelist->getCurrentRow(), 1);
    for(int i=0; i < taggedlist.no(); i++)
    {
        if(taggedlist.at(i).contains(idkeyvalue))
            taggedlist.erase(i);
    }
    taggedlist.append(tagstr + "|" + idkeyvalue + "|" + plaintext->getText());
    * /
    return 1;
}

long WombatForensics::RemoveTag(FXObject*, FXSelector, void*)
{
    / *
    tablelist->setItemText(tablelist->getCurrentRow(), 0, "");
    FXString idkeyvalue = statusbar->getStatusLine()->getText() + "\\" + tablelist->getItemText(tablelist->getCurrentRow(), 1);
    for(int i=0; i < taggedlist.no(); i++)
    {
        if(taggedlist.at(i).contains(idkeyvalue))
            taggedlist.erase(i);
    }
    * /
    return 1;
}

long WombatForensics::KeySelected(FXObject* sender, FXSelector, void*)
{
    / *
    FXTreeItem* curitem = treelist->getCurrentItem();
    bool toplevel = false;
    std::vector<FXString> pathitems;
    pathitems.clear();
    pathitems.push_back(curitem->getText());
    FXTreeItem* parent;
    FXTreeItem* child;
    child = curitem;
    while(toplevel == false)
    {
	parent = child->getParent();
	if(parent == NULL)
	    toplevel = true;
	else
	{
	    pathitems.push_back(parent->getText());
	    child = parent;
	}
    }
    FXString keypath = "";
    for(int i=pathitems.size() - 2; i > -1; i--)
    {
	keypath += "\\" + pathitems.at(i);
    }
    StatusUpdate(keypath);
    libregf_file_t* regfile = NULL;
    libregf_error_t* regerr = NULL;
    libregf_file_initialize(&regfile, &regerr);
    libregf_file_open(regfile, hivefilepath.c_str(), LIBREGF_OPEN_READ, &regerr);
    libregf_key_t* curkey = NULL;
    libregf_file_get_key_by_utf8_path(regfile, (uint8_t*)(keypath.text()), keypath.count(), &curkey, &regerr);
    // valid key, get values...
    int valuecount = 0;
    libregf_key_get_number_of_values(curkey, &valuecount, &regerr);
    tablelist->clearItems();
    plaintext->setText("");
    tablelist->setTableSize(valuecount, 3);
    tablelist->setColumnText(0, "Tag");
    tablelist->setColumnText(1, "Value Name");
    tablelist->setColumnText(2, "Value Type");
    FXString tagstr = "";
    if(valuecount == 0) // no values, so create empty key
    {
	tablelist->setTableSize(1, 3);
	tablelist->setColumnText(0, "Tag");
	tablelist->setColumnText(1, "Value Name");
	tablelist->setColumnText(2, "Value Type");
	FXString curtagvalue = keypath + "\\" + "(empty)";
	tablelist->setItemText(0, 1, "(empty)");
	tablelist->setItemText(0, 2, "0x00");
        for(int j=0; j < taggedlist.no(); j++)
        {
            if(taggedlist.at(j).contains(curtagvalue))
            {
                std::size_t found = taggedlist.at(j).find("|");
                tagstr = taggedlist.at(j).left(found);
            }
        }
        tablelist->setItemText(0, 0, tagstr);
    }
    for(int i=0; i < valuecount; i++)
    {
	libregf_value_t* curval = NULL;
	libregf_key_get_value(curkey, i, &curval, &regerr);
	size_t namesize = 0;
	libregf_value_get_utf8_name_size(curval, &namesize, &regerr);
	uint8_t name[namesize];
	libregf_value_get_utf8_name(curval, name, namesize, &regerr);
	uint32_t type = 0;
	libregf_value_get_value_type(curval, &type, &regerr);
	FXString curtagvalue = keypath + "\\";
	if(namesize == 0)
	{
	    curtagvalue += "(unnamed)";
	    tablelist->setItemText(i, 1, "(unnamed)");
	    FXString typestr = FXString::value(type, 16);
	    tablelist->setItemText(i, 2, typestr);
	}
	else
	{
	    curtagvalue += reinterpret_cast<char*>(name);
	    FXString valuetypestr = "";
	    tablelist->setItemText(i, 1, reinterpret_cast<char*>(name));
            if(type == 0x00) // none
            {
            }
            else if(type == 0x01) // reg_sz
            {
                valuetypestr = "REG_SZ";
            }
            else if(type == 0x02) // reg_expand_sz
            {
                valuetypestr = "REG_EXPAND_SZ";
            }
            else if(type == 0x03) // reg_binary
            {
                valuetypestr = "REG_BINARY";
            }
            else if(type == 0x04) // reg_dword reg_dword_little_endian (4 bytes)
            {
                valuetypestr = "REG_DWORD";
            }
            else if(type == 0x05) // reg_dword_big_endian (4 bytes)
            {
                valuetypestr = "REG_DWORD_BIG_ENDIAN";
            }
            else if(type == 0x06) // reg_link
            {
                valuetypestr = "REG_LINK";
            }
            else if(type == 0x07) // reg_multi_sz
            {
                valuetypestr = "REG_MULTI_SZ";
            }
            else if(type == 0x08) // reg_resource_list
            {
                valuetypestr = "REG_RESOURCE_LIST";
            }
            else if(type == 0x09) // reg_full_resource_descriptor
            {
                valuetypestr = "REG_FULL_RESOURCE_DESCRIPTOR";
            }
            else if(type == 0x0a) // reg_resource_requirements_list
            {
                valuetypestr = "REG_RESOURCE_REQUIREMENTS_LIST";
            }
            else if(type == 0x0b) // reg_qword_little_endian (8 bytes)
            {
                valuetypestr = "REG_QWORD";
            }
            else
            {
            }
	    tablelist->setItemText(i, 2, valuetypestr);
	}
        for(int j=0; j < taggedlist.no(); j++)
        {
            if(taggedlist.at(j).contains(curtagvalue))
            {
                std::size_t found = taggedlist.at(j).find("|");
                tagstr = taggedlist.at(j).left(found);
                tablelist->setItemText(i, 0, tagstr); 
            }
        }
	libregf_value_free(&curval, &regerr);
    }
    libregf_key_free(&curkey, &regerr);
    libregf_file_close(regfile, &regerr);
    libregf_file_free(&regfile, &regerr);
    libregf_error_free(&regerr);
    //tablelist->selectRow(0);
    //tablelist->setCurrentItem(0, 0);
    * /
    return 1;
}

void WombatForensics::GetRootString(FXTreeItem* curitem, FXString* rootstring)
{
    if(curitem->getParent() == NULL)
	*rootstring = curitem->getText();
    else
        GetRootString(curitem->getParent(), rootstring);
}

FXString WombatForensics::ConvertUnixTimeToString(uint32_t input)
{
    time_t crtimet = (time_t)input;
    struct tm* dt;
    dt = gmtime(&crtimet);
    char timestr[30];
    strftime(timestr, sizeof(timestr), "%m/%d/%Y %I:%M:%S %p", dt);

    return timestr;
}

FXString WombatForensics::ConvertWindowsTimeToUnixTimeUTC(uint64_t input)
{
    uint64_t temp;
    temp = input / TICKS_PER_SECOND; //convert from 100ns intervals to seconds;
    temp = temp - EPOCH_DIFFERENCE;  //subtract number of seconds between epochs
    time_t crtimet = (time_t)temp;
    struct tm* dt;
    dt = gmtime(&crtimet);
    char timestr[30];
    strftime(timestr, sizeof(timestr), "%m/%d/%Y %I:%M:%S %p", dt);

    return timestr;
}


long WombatForensics::ValueSelected(FXObject*, FXSelector, void*)
{
    / *
    if(tablelist->getCurrentRow() > -1)
    {
	tablelist->selectRow(tablelist->getCurrentRow());
	int valueindex = tablelist->getCurrentRow();
        if(!tablelist->getItemText(tablelist->getCurrentRow(), 1).empty())
        {
            FXString valuename = tablelist->getItemText(tablelist->getCurrentRow(), 1);
            FXString valuetype = tablelist->getItemText(tablelist->getCurrentRow(), 2);
            FXTreeItem* curitem = treelist->getCurrentItem();
            FXString rootstring = "";
            FXString hivefilepath = "";
            GetRootString(curitem, &rootstring);
            for(int i=0; i < hives.size(); i++)
            {
                if(rootstring.contains(FXString(hives.at(i).string().c_str())))
                    hivefilepath = FXString(hives.at(i).string().c_str());
            }
            FXString keypath = statusbar->getStatusLine()->getNormalText();
            libregf_file_t* regfile = NULL;
            libregf_error_t* regerr = NULL;
            libregf_file_initialize(&regfile, &regerr);
            libregf_file_open(regfile, hivefilepath.text(), LIBREGF_OPEN_READ, &regerr);
            libregf_key_t* curkey = NULL;
            libregf_file_get_key_by_utf8_path(regfile, (uint8_t*)(keypath.text()), keypath.count(), &curkey, &regerr);
            libregf_value_t* curval = NULL;
            libregf_key_get_value(curkey, valueindex, &curval, &regerr);
            uint64_t lastwritetime = 0;
            libregf_key_get_last_written_time(curkey, &lastwritetime, &regerr);
            FXString valuedata = "Last Written Time:\t" + ConvertWindowsTimeToUnixTimeUTC(lastwritetime) + " UTC\n\n";
            valuedata += "Name:\t" + valuename + "\n\n";
            if(valuename.contains("(unnamed)"))
            {
                valuedata += "Content\n-------\n\n";
                valuedata += "Hex:\t0x" + FXString::value(valuetype.toInt(16), 16) + "\n";
                valuedata += "Integer:\t" + FXString::value(valuetype.toInt()) + "\n";
            }
            else
            {
                if(valuetype.contains("REG_SZ") || valuetype.contains("REG_EXPAND_SZ"))
                {
                    valuedata += "Content:\t";
                    size_t strsize = 0;
                    libregf_value_get_value_utf8_string_size(curval, &strsize, &regerr);
                    uint8_t valstr[strsize];
                    libregf_value_get_value_utf8_string(curval, valstr, strsize, &regerr);
                    valuedata += FXString(reinterpret_cast<char*>(valstr));
                }
                else if(valuetype.contains("REG_BINARY"))
                {
                    valuedata += "Content\n-------\n\n";
                    if(keypath.contains("UserAssist") && (keypath.contains("{750") || keypath.contains("{F4E") || keypath.contains("{5E6")))
                    {
                        valuedata += "ROT13 Decrypted Content:\t";
                        valuedata += DecryptRot13(valuename) + "\n";
                    }
                    else if(keypath.contains("SAM") && valuename.count() == 1 && valuename.contains("F"))
                    {
                        uint64_t tmp64 = 0;
                        size_t datasize = 0;
                        libregf_value_get_value_data_size(curval, &datasize, &regerr);
                        uint8_t data[datasize];
                        libregf_value_get_value_data(curval, data, datasize, &regerr);
                        valuedata += "Account Expiration:\t\t";
                        if(data[32] == 0xff)
                        {
                            valuedata += "No Expiration is Set\n";
                        }
                        else
                        {
                            tmp64 = (uint64_t)data[32] | (uint64_t)data[33] << 8 | (uint64_t)data[34] << 16 | (uint64_t)data[35] << 24 | (uint64_t)data[36] << 32 | (uint64_t)data[37] << 40 | (uint64_t)data[38] << 48 | (uint64_t)data[39] << 56;
                            valuedata += ConvertWindowsTimeToUnixTimeUTC(tmp64) + " UTC\n";
                        }
                        tmp64 = (uint64_t)data[8] | (uint64_t)data[9] << 8 | (uint64_t)data[10] << 16 | (uint64_t)data[11] << 24 | (uint64_t)data[12] << 32 | (uint64_t)data[13] << 40 | (uint64_t)data[14] << 48 | (uint64_t)data[15] << 56;
                        valuedata += "Last Logon Time:\t\t" + ConvertWindowsTimeToUnixTimeUTC(tmp64) + " UTC\n";
                        tmp64 = (uint64_t)data[40] | (uint64_t)data[41] << 8 | (uint64_t)data[42] << 16 | (uint64_t)data[43] << 24 | (uint64_t)data[44] << 32 | (uint64_t)data[45] << 40 | (uint64_t)data[46] << 48 | (uint64_t)data[47] << 56;
                        valuedata += "Last Failed Login:\t\t" + ConvertWindowsTimeToUnixTimeUTC(tmp64) + " UTC\n";
                        tmp64 = (uint64_t)data[24] | (uint64_t)data[25] << 8 | (uint64_t)data[26] << 16 | (uint64_t)data[27] << 24 | (uint64_t)data[28] << 32 | (uint64_t)data[29] << 40 | (uint64_t)data[30] << 48 | (uint64_t)data[31] << 56;
                        valuedata += "Last Time Password Changed:\t" + ConvertWindowsTimeToUnixTimeUTC(tmp64) + " UTC\n";
                    }
                    else if(valuename.contains("ShutdownTime"))
                    {
                        size_t datasize = 0;
                        libregf_value_get_value_data_size(curval, &datasize, &regerr);
                        uint8_t data[datasize];
                        libregf_value_get_value_data(curval, data, datasize, &regerr);
                        uint64_t tmp64 = (uint64_t)data[0] | (uint64_t)data[1] << 8 | (uint64_t)data[2] << 16 | (uint64_t)data[3] << 24 | (uint64_t)data[4] << 32 | (uint64_t)data[5] << 40 | (uint64_t)data[6] << 48 | (uint64_t)data[7] << 56;
                        valuedata += "Shutdown Time:\t" + ConvertWindowsTimeToUnixTimeUTC(tmp64) + " UTC\n";

                    }
                    else if(valuename.contains("MRUListEx"))
                    {
                        size_t datasize = 0;
                        libregf_value_get_value_data_size(curval, &datasize, &regerr);
                        uint8_t data[datasize];
                        libregf_value_get_value_data(curval, data, datasize, &regerr);
                        valuedata += "Order:\t[";
                        for(int i=0; i < sizeof(data) / 4; i++)
                        {
                            uint32_t tmp32 = (uint32_t)data[i*4] | (uint32_t)data[i*4 + 1] << 8 | (uint32_t)data[i*4 + 2] << 16 | (uint32_t)data[i*4 + 3] << 24;
                            if(tmp32 < 0xFFFFFFFF)
                                valuedata += FXString::value(tmp32);
                            if(i < ((sizeof(data) / 4) - 2))
                                valuedata += ", ";
                        }
                        valuedata += "]\n";
                    }
                    else if(keypath.contains("RecentDocs"))
                    {
                        if(!valuename.contains("MRUListEx"))
                        {
                            size_t datasize = 0;
                            libregf_value_get_value_data_size(curval, &datasize, &regerr);
                            uint8_t data[datasize];
                            libregf_value_get_value_data(curval, data, datasize, &regerr);
                            valuedata += "Name:\t";
                            for(int i=0; i < sizeof(data) / 2; i++)
                            {
                                uint16_t tmp16 = (uint16_t)data[i*2] | (uint16_t)data[i*2 + 1] << 8;
                                FXwchar tmpwc = FX::wc(&tmp16);
                                if(tmp16 == 0x0000)
                                    break;
                                valuedata += tmpwc;
                            }
                        }
                    }
                }
                else if(valuetype.contains("REG_DWORD"))
                {
                    valuedata += "Content:\t";
                    uint32_t dwordvalue = 0;
                    libregf_value_get_value_32bit(curval, &dwordvalue, &regerr);
                    if(valuename.lower().contains("date"))
                        valuedata += ConvertUnixTimeToString(dwordvalue);
                    else
                        valuedata += FXString::value(dwordvalue);
                }
                else if(valuetype.contains("REG_DWORD_BIG_ENDIAN"))
                {
                    valuedata += "Content:\t";
                    uint32_t dwordvalue = 0;
                    libregf_value_get_value_32bit(curval, &dwordvalue, &regerr);
                    valuedata += FXString::value(dwordvalue);
                }
                else if(valuetype.contains("REG_MULTI_SZ"))
                {
                    valuedata += "Content\n";
                    valuedata += "-------\n";
                    libregf_multi_string_t* multistring = NULL;
                    libregf_value_get_value_multi_string(curval, &multistring, &regerr);
                    int strcnt = 0;
                    libregf_multi_string_get_number_of_strings(multistring, &strcnt, &regerr);
                    for(int i=0; i < strcnt; i++)
                    {
                        size_t strsize = 0;
                        libregf_multi_string_get_utf8_string_size(multistring, i, &strsize, &regerr);
                        uint8_t valstr[strsize];
                        libregf_multi_string_get_utf8_string(multistring, i, valstr, strsize, &regerr);
                        valuedata += FXString(reinterpret_cast<char*>(valstr));
                    }
                    libregf_multi_string_free(&multistring, &regerr);
                }
                else if(valuetype.contains("REG_QWORD"))
                {
                    valuedata += "Content:\t";
                    uint64_t qwordvalue = 0;
                    libregf_value_get_value_64bit(curval, &qwordvalue, &regerr);
                    valuedata += FXString::value(qwordvalue);
                }
            }
            size_t datasize = 0;
            libregf_value_get_value_data_size(curval, &datasize, &regerr);
            uint8_t data[datasize];
            libregf_value_get_value_data(curval, data, datasize, &regerr);
            valuedata += "\n\nBinary Content\n--------------\n\n";
            if(datasize < 16)
            {
                valuedata += "0000\t";
                std::stringstream ss;
                ss << std::hex <<  std::setfill('0');
                for(int i=0; i < datasize; i++)
                    ss << std::setw(2) << ((uint)data[i]) << " ";
                valuedata += FXString(ss.str().c_str()).upper();
                for(int i=0; i < datasize; i++)
                {
                    if(isprint(data[i]))
                        valuedata += FXchar(reinterpret_cast<unsigned char>(data[i]));
                    else
                        valuedata += ".";
                }
                valuedata += "\n";
            }
            else
            {
                int linecount = datasize / 16;
                for(int i=0; i < linecount; i++)
                {
                    std::stringstream ss;
                    ss << std::hex << std::setfill('0') << std::setw(8) << i * 16 << "\t";
                    for(int j=0; j < 16; j++)
                    {
                        ss << std::setw(2) << ((uint)data[j+i*16]) << " ";
                    }
                    valuedata += FXString(ss.str().c_str()).upper();
                    for(int j=0; j < 16; j++)
                    {
                        if(isprint(data[j+i*16]))
                            valuedata += FXchar(reinterpret_cast<unsigned char>(data[j+i*16]));
                        else
                            valuedata += ".";
                    }
                    valuedata += "\n";
                }
            }
            plaintext->setText(valuedata);
            libregf_value_free(&curval, &regerr);
            libregf_key_free(&curkey, &regerr);
            libregf_file_close(regfile, &regerr);
            libregf_file_free(&regfile, &regerr);
            libregf_error_free(&regerr);
        }
    }
    * /
    return 1;
}

FXString WombatForensics::DecryptRot13(FXString encstr)
{
    FXString decstr = "";
    int i = 0;
    int strlength = 0;
    strlength = encstr.count();
    decstr = encstr;
    for(i = 0; i < strlength; i++)
    {
        decstr[i] = Rot13Char(decstr.at(i));
    }
    return decstr;
}

FXchar WombatForensics::Rot13Char(FXchar curchar)
{
    FXchar rot13char;
    if('0' <= curchar && curchar <= '4')
        rot13char = FXchar(curchar + 5);
    else if('5' <= curchar && curchar <= '9')
        rot13char = FXchar(curchar - 5);
    else if('A' <= curchar && curchar <= 'M')
        rot13char = FXchar(curchar + 13);
    else if('N' <= curchar && curchar <= 'Z')
        rot13char = FXchar(curchar - 13);
    else if('a' <= curchar && curchar <= 'm')
        rot13char = FXchar(curchar + 13);
    else if('n' <= curchar && curchar <= 'z')
        rot13char = FXchar(curchar - 13);
    else
        rot13char = curchar;
    return rot13char;
}
*/

long WombatForensics::OpenTagManager(FXObject*, FXSelector, void*)
{
    ManageTags tagmanager(this, "Manage Tags");
    tagmanager.SetTagList(&tags);
    tagmanager.execute(PLACEMENT_OWNER);

    return 1;
}

long  WombatForensics::OpenAboutBox(FXObject*, FXSelector, void*)
{
    AboutBox aboutbox(this, "About Wombat Forensics");
    aboutbox.execute(PLACEMENT_OWNER);

    return 1;
}

long WombatForensics::PreviewReport(FXObject*, FXSelector, void*)
{
    viewer = new Viewer(this, "Report Preview");
    //viewer->GenerateReport(taggedlist, tags);
    viewer->execute(PLACEMENT_OWNER);

    return 1;
}

long WombatForensics::OpenSettings(FXObject*, FXSelector, void*)
{
    Settings settings(this, "Settings");
    settings.LoadSettings(currentsettings);
    bool tosave = settings.execute(PLACEMENT_OWNER);
    if(tosave == 1)
    {
        currentsettings = settings.ReturnSettings();
        settingfile.open(configpath + "settings", FXIO::Writing, FXIO::OwnerReadWrite);
        settingfile.writeBlock(currentsettings.text(), currentsettings.length());
        settingfile.close();
    }

    return 1;
}

long WombatForensics::OpenManageCarved(FXObject*, FXSelector, void*)
{
    ManageCarving managecarving(this, "Manage Carving");
    managecarving.LoadManageCarving(currentcarvetypes);
    bool tosave = managecarving.execute(PLACEMENT_OWNER);
    if(tosave == 1)
    {
        currentcarvetypes = managecarving.ReturnManageCarving();
        carvetypesfile.open(configpath + "carvetypes", FXIO::Writing, FXIO::OwnerReadWrite);
        carvetypesfile.writeBlock(currentcarvetypes.text(), currentcarvetypes.length());
        carvetypesfile.close();
    }
    
    return 1;
}
/*
long WombatForensics::PublishReport(FXObject*, FXSelector, void*)
{
    FXString startpath = FXString(getenv("HOME")) + "/";
    FXString filename = FXFileDialog::getSaveFilename(this, "Publish Report", startpath, "Text Files (*.txt)\nHTML Files (*.html,*.htm)");
    if(!filename.empty())
    {
        FXFile* outfile = new FXFile(filename, FXIO::Writing, FXIO::OwnerReadWrite);
        FXString buf;
        if(filename.contains(".htm"))
        {
            buf = "<html><head><title>Wombat Registry Report</title></head>\n";
            buf += "<body style='font-color: #3a291a; background-color: #d6ceb5;'>\n";
            buf += "<h2>Wombat Registry Report</h2>\n";
            buf += "<div id='toc'><h3>Contents</h3>\n";
            for(int j=0; j < tags.size(); j++)
            {
                int tagcnt = 0;
                for(int i=0; i < taggedlist.no(); i++)
                {
                    if(taggedlist.at(i).contains(tags.at(j).c_str()))
                        tagcnt++;
                }
                buf += "<div><a href='#t" + FXString::value(j) + "'>" + FXString(tags.at(j).c_str()) + " (" + FXString::value(tagcnt) + ")</a></div>\n";
            }
            buf += "<h3>Tagged Items</h3>";
            for(int i=0; i < tags.size(); i++)
            {
                buf += "<div id='t" + FXString::value(i) + "'><h4>" + tags.at(i).c_str() + "<span style='font-size: 10px;'>&nbsp;&nbsp;<a href='#toc'>TOP</a></span></h4>\n";
                for(int j=0; j < taggedlist.no(); j++)
                {
                    std::size_t found = taggedlist.at(j).find("|");
                    std::size_t rfound = taggedlist.at(j).rfind("|");
                    FXString itemtag = taggedlist.at(j).mid(0, found);
                    FXString itemhdr = taggedlist.at(j).mid(found+1, rfound - found - 1);
                    FXString itemcon = taggedlist.at(j).mid(rfound+1, taggedlist.at(j).length() - rfound);
                    if(itemtag == tags.at(i).c_str())
                    {
                        buf += "<div style='border-bottom: 1px solid black;'>\n";
                        buf += "<div>Key:&nbsp;&nbsp;&nbsp;&nbsp;" + itemhdr + "</div>\n";
                        buf += "<div><pre>" + itemcon + "</pre></div>\n";
                        buf += "</div>\n";
                    }
                }
            }
            buf += "</body></html>";
        }
        else
        {
            viewer->GetText(&buf);
        }
        outfile->writeBlock(buf.text(), buf.length());
        outfile->close();
    }
    return 1;
}

long WombatForensics::OpenSqliteFile(FXObject*, FXSelector, void*)
{
    if(prevsqlitepath.empty())
        prevsqlitepath = FXString(getenv("HOME")) + "/";
    sqlitefilepath = FXFileDialog::getOpenFilename(this, "Open SQLite File", prevsqlitepath);
    //std::cout << "sqlitefilepath: " << sqlitefilepath.text() << std::endl;
    if(!sqlitefilepath.empty())
    {
        prevsqlitepath = sqlitefilepath;
        filetype = 0;
        filebuffer.open(sqlitefilepath.text(), std::ios::in|std::ios::binary);
        filebuffer.seekg(0, filebuffer.beg);
        filebuffer.seekg(0, filebuffer.end);
        filesize = filebuffer.tellg();
        filebuffer.seekg(0, filebuffer.beg);
        uint32_t sqlheader = 0;
        ReadContent(&filebuffer, &sqlheader, 0);
        //std::cout << "sql header:" << std::hex << sqlheader << std::endl;

        if(sqlheader == 0x377f0682 || sqlheader == 0x377f0683) // WAL
        {
            filetype = 1; // WAL
            ReadContent(&filebuffer, &pagesize, 8);
            //std::cout << "WAL page size:" << pagesize << std::endl;
        }
        else
        {
            uint64_t journalheader = 0;
            ReadContent(&filebuffer, &journalheader, 0);
            //std::cout << "journalheader: " << std::hex << journalheader << std::endl;
            if(journalheader == 0xd9d505f920a163d7) // JOURNAL
            {
                filetype = 2; // JOURNAL
                ReadContent(&filebuffer, &pagesize, 24);
                //std::cout << "page size:" << pagesize << std::endl;
            }
            else
            {
                char* sqliteheader = new char[15];
                filebuffer.seekg(0);
                filebuffer.read(sqliteheader, 15);
                if(FXString(sqliteheader) == "SQLite format 3") // SQLITE DB
                {
                    filetype = 3; // SQLITE DB
                    uint16_t ps = 0;
                    ReadContent(&filebuffer, &ps, 16);
                    pagesize = ps;
                    //std::cout << "pagesize: " << pagesize << std::endl;
                }
            }
        }
        filebuffer.close();
        if((uint)filetype > 0)
        {
            sqlitefiles.append(sqlitefilepath);
            pagecount = filesize / pagesize;
            //std::cout << "page count:" << pagecount << std::endl;
            int found = sqlitefilepath.find_last_of("/");
            FXString itemstring = sqlitefilepath.right(sqlitefilepath.length() - found - 1) + " (" + FXString::value(pagecount) + ") [" + sqlitefilepath.left(found+1) + "]";
            FXListItem* rootitem = new FXListItem(itemstring);
            //rootitem->setData(256, QVariant(filetype)); // file type
            //rootitem->setData(257, QVariant(pagesize)); // page size
            //rootitem->setData(258, QVariant(1)); // current page
            sqlfilelist->appendItem(rootitem);
            pagespinner->setRange(1, pagecount);
            pagespinner->setValue(1);
            ofpagelabel->setText(" of " + FXString::value(pagecount) + " pages");
            StatusUpdate("SQLite File: " + sqlitefilepath + " successfully opened.");
            // CURRENT ITEM INDEX(FILETYPE|PAGESIZE|CURPAGE)
            fileuserdata.append(FXString::value(filetype) + "|" + FXString::value(pagesize) + "|" + FXString::value(1));
            //std::cout << "filetype: " << FXString::value(filetype).text() << std::endl;
            // MAY NOT WANT TO SET SELECTED, SINCE IT DOESN'T SEEM TO TRIGGER THE FILESELECTED FUNCTION
            //sqlfilelist->selectItem(sqlfilelist->getNumItems() - 1);
        }
        else
            StatusUpdate("Not a SQLite file, file not opened.");
    }
    return 1;
}

long WombatForensics::FileSelected(FXObject*, FXSelector, void*)
{
    FXString tmpstr = sqlfilelist->getItemText(sqlfilelist->getCurrentItem());
    int found = tmpstr.find(" (");
    int lfound = tmpstr.find("[");
    int rfound = tmpstr.find("]");
    curfilepath = tmpstr.mid(lfound+1, rfound - lfound - 1) + tmpstr.mid(0, found);
    filebuffer.open(curfilepath.text(), std::ios::in|std::ios::binary);
    filebuffer.seekg(0, filebuffer.beg);
    curfileuserdata = fileuserdata.at(sqlfilelist->getCurrentItem());
    lfound = curfileuserdata.find_first_of("|");
    rfound = curfileuserdata.find_last_of("|");
    filetype = (uint8_t)curfileuserdata.mid(0, lfound).toUInt();
    pagesize = curfileuserdata.mid(lfound+1, rfound-1).toULong();
    curpage = curfileuserdata.mid(rfound+1, curfileuserdata.length() - rfound - 1).toULong();
    //std::cout << "cur file user data: " << curfileuserdata.text() << std::endl;
    //std::cout << curfilepath.text() << std::endl;
    //std::cout << tmpstr.mid(0, found).text() << std::endl;
    //std::cout << tmpstr.mid(lfound+1, rfound-1).text() << std::endl;
    //std::cout << curfilepath.text() << std::endl;
    //std::cout << curfileuserdata.text() << std::endl;
    pagespinner->setValue(curpage);
    proptable->setCurrentItem(-1, -1);
    LoadPage();
    //std::cout << curfileuserdata.mid(0, lfound).text() << std::endl;
    //std::cout << filetype << " " << pagesize << " " << curpage << std::endl;
    
    return 1;
}

void WombatForensics::LoadPage()
{
    //std::cout << "curpage: " << curpage << " filetype: " << filetype << std::endl;
    uint8_t* pagebuf = new uint8_t[pagesize];
    ReadContent(&filebuffer, pagebuf, (curpage - 1) * pagesize, pagesize);
    filebuffer.close();

    if(curpage == 1)
    {
        uint8_t* pageheader = substr(pagebuf, 0, 100);
        ParseFileHeader(pageheader);
        if((uint)filetype == 0x01)
            proptablerowcnt = 8;
        else if((uint)filetype == 0x02)
            proptablerowcnt = 6;
        else if((uint)filetype == 0x03)
            proptablerowcnt = 18;
        //std::cout << "prop table row cnt: " << proptablerowcnt << std::endl;
        //PopulateFileHeader();
    }
    ParsePageHeader(pagebuf, filetype, curpage);
    PopulatePageContent(pagebuf);


    //std::cout << "content: " << pagebuf[0] << std::endl;
}

void WombatForensics::PopulatePageContent(uint8_t* pagearray)
{
    FXString offsetcontent = "";
    FXString hexcontent = "";
    FXString asciicontent = "";
    int linecount = pagesize / 16;
    int linerem = pagesize % 16;
    if(linerem > 0)
        linecount++;
    //std::cout << "linecount: " << linecount << std::endl;
    offsettext->verticalScrollBar()->setRange(linecount - 1);
    offsettext->verticalScrollBar()->setLine(1);
    hextext->verticalScrollBar()->setRange(linecount - 1);
    hextext->verticalScrollBar()->setLine(1);
    asciitext->verticalScrollBar()->setRange(linecount - 1);
    asciitext->verticalScrollBar()->setLine(1);
    textscrollbar->setRange(linecount - 1);
    textscrollbar->setLine(1);
    // OFFSET CONTENT
    std::stringstream os;
    for(int i=0; i < linecount; i++)
        os << std::hex << std::setfill('0') << std::setw(5) << i * 16 << "\n";
    offsetcontent = FXString(os.str().c_str());
    offsettext->setText(offsetcontent);
    // HEX CONTENT
    std::stringstream hs;
    for(int i=0; i < linecount; i++)
    {
        for(int j=0; j < 16; j++)
        {
            if(j+i*16 < pagesize)
                hs << std::hex << std::setfill('0') << std::setw(2) << (uint)pagearray[j+i*16] << " ";
            else
                hs << "   ";
        }
        hs << "\n";
    }
    hexcontent = FXString(hs.str().c_str());
    hextext->setText(hexcontent);
    // ASCII CONTENT
    std::stringstream as;
    for(int i=0; i < linecount; i++)
    {
        for(int j=0; j < 16; j++)
        {
            if(j+i*16 < pagesize)
            {
                if(isprint(pagearray[j+i*16]))
                    asciicontent += FXchar(reinterpret_cast<unsigned char>(pagearray[j+i*16]));
                else
                    asciicontent += ".";
            }
        }
        asciicontent += "\n";
    }
    asciitext->setText(asciicontent);
}

void WombatForensics::ParseFileHeader(uint8_t* pageheader)
{
    //std::cout << "filetype: " << filetype << std::endl;
    //std::cout << "page hedaer: " << pageheader[0] << pageheader[1] << std::endl;
    if((uint)filetype == 0x01) // WAL
    {
        ReadInteger(pageheader, 0, &walheader.header);
        ReadInteger(pageheader, 4, &walheader.fileversion);
        ReadInteger(pageheader, 8, &walheader.pagesize);
        ReadInteger(pageheader, 12, &walheader.checkptseqnum);
        ReadInteger(pageheader, 16, &walheader.salt1);
        ReadInteger(pageheader, 20, &walheader.salt2);
        ReadInteger(pageheader, 24, &walheader.checksum1);
        ReadInteger(pageheader, 28, &walheader.checksum2);
    }
    else if((uint)filetype == 0x02) // JOURNAL
    {
        ReadInteger(pageheader, 0, &journalheader.header);
        ReadInteger(pageheader, 8, &journalheader.pagecnt);
        ReadInteger(pageheader, 12, &journalheader.randomnonce);
        ReadInteger(pageheader, 16, &journalheader.initsize);
        ReadInteger(pageheader, 20, &journalheader.sectorsize);
        ReadInteger(pageheader, 24, &journalheader.pagesize);
    }
    else if((uint)filetype == 0x03) // SQLITE DB
    {
        sqliteheader.header = FXString((char*)substr(pageheader, 0, 16));
        ReadInteger(pageheader, 16, &sqliteheader.pagesize);
        //std::cout << "page header: " << substr(pageheader, 0, 16)[0] << std::endl;
        //std::cout << "sqlite header:" << sqliteheader.header.text() << std::endl;
        //std::cout << "pagesize: " << sqliteheader.pagesize << std::endl;
        sqliteheader.writeversion = pageheader[18];
        sqliteheader.readversion = pageheader[19];
    
        sqliteheader.unusedpagespace = pageheader[20];
        ReadInteger(pageheader, 28, &sqliteheader.pagecount);
        ReadInteger(pageheader, 32, &sqliteheader.firstfreepagenum);
        ReadInteger(pageheader, 36, &sqliteheader.freepagescount);
        ReadInteger(pageheader, 40, &sqliteheader.schemacookie);
        ReadInteger(pageheader, 44, &sqliteheader.schemaformat);
        ReadInteger(pageheader, 48, &sqliteheader.pagecachesize);
        ReadInteger(pageheader, 52, &sqliteheader.largestrootbtreepagenumber);
        ReadInteger(pageheader, 56, &sqliteheader.textencoding);
        ReadInteger(pageheader, 60, &sqliteheader.userversion);
        ReadInteger(pageheader, 64, &sqliteheader.incrementalvacuummodebool);
        ReadInteger(pageheader, 68, &sqliteheader.appid);
        ReadInteger(pageheader, 92, &sqliteheader.versionvalidfornum);
        ReadInteger(pageheader, 96, &sqliteheader.version);
    }
}

void WombatForensics::AddContent(int row, FXString islive, FXString rowid, FXString offlen, FXString type, FXString val, FXString tag)
{
    tablelist->setItemText(row, 0, tag);
    tablelist->setItemText(row, 1, islive);
    tablelist->setItemText(row, 2, rowid);
    tablelist->setItemText(row, 3, offlen);
    tablelist->setItemText(row, 4, type);
    tablelist->setItemText(row, 5, val);
}

void WombatForensics::AddProperty(int row, FXString offlen, FXString val, FXString desc)
{
    proptable->setItemText(row, 0, offlen);
    proptable->setItemText(row, 1, val);
    proptable->setItemText(row, 2, desc);
}

void WombatForensics::PopulateFileHeader()
{
    if((uint)filetype == 0x01) // WAL
    {
        std::stringstream ss;
        ss << std::hex << std::setfill('0') << std::setw(8) << walheader.header;
        AddProperty(0, "0, 4", FXString("0x" + FXString(ss.str().c_str()).upper()), "WAL HEADER, last byte is either 0x82 or 0x83 which means something i forget right now.");
        AddProperty(1, "4, 4", FXString::value(walheader.fileversion), "WAL File Version");
        AddProperty(2, "8, 4", FXString::value(walheader.pagesize), "WAL Page Size");
        AddProperty(3, "12, 4", FXString::value(walheader.checkptseqnum), "WAL Checkpoint Sequence Number");
        AddProperty(4, "16, 4", FXString::value(walheader.salt1), "WAL Salt 1");
        AddProperty(5, "20, 4", FXString::value(walheader.salt2), "WAL Salt 2");
        AddProperty(6, "24, 4", FXString::value(walheader.checksum1), "WAL Checksum 1");
        AddProperty(7, "28, 4", FXString::value(walheader.checksum2), "WAL Checksum 2");
    }
    else if((uint)filetype == 0x02) // JOURNAL
    {
        std::stringstream ss;
        ss << std::hex << std::setfill('0') << std::setw(8) << journalheader.header;
        AddProperty(0, "0, 8", FXString("0x" + FXString(ss.str().c_str()).upper()), "JOURNAL HEADER");
        AddProperty(1, "8, 4", FXString::value(journalheader.pagecnt), "Journal Page Count");
        AddProperty(2, "12, 4", FXString::value(journalheader.randomnonce), "Journal Random Nonce");
        AddProperty(3, "16, 4", FXString::value(journalheader.initsize), "Journal Initial Size");
        AddProperty(4, "20, 4", FXString::value(journalheader.sectorsize), "Journal Sector Size");
        AddProperty(5, "24, 4", FXString::value(journalheader.pagesize), "Journal Page Size");
    }
    else if((uint)filetype == 0x03) // DB
    {
        AddProperty(0, "0, 16", sqliteheader.header, "SQLite Header");
        AddProperty(1, "16, 2", FXString::value(sqliteheader.pagesize), "SQLite Page Size");
        AddProperty(2, "18, 1", FXString::value(sqliteheader.writeversion), "SQLite Write Version");
        AddProperty(3, "19, 1", FXString::value(sqliteheader.readversion), "SQLite Read Version");
        AddProperty(4, "20, 1", FXString::value(sqliteheader.unusedpagespace), "SQLite Unused Page Space");
        AddProperty(5, "28, 4", FXString::value(sqliteheader.pagecount), "SQLite Page Count");
        AddProperty(6, "32, 4", FXString::value(sqliteheader.firstfreepagenum), "SQLite First Free Page Number");
        AddProperty(7, "36, 4", FXString::value(sqliteheader.freepagescount), "SQLite Free Pages Count");
        AddProperty(8, "40, 4", FXString::value(sqliteheader.schemacookie), "SQLite Schema Cookie");
        AddProperty(9, "44, 4", FXString::value(sqliteheader.schemaformat), "SQLite Schema Format");
        AddProperty(10, "48, 4", FXString::value(sqliteheader.pagecachesize), "SQLite Page Cache Size");
        AddProperty(11, "52, 4", FXString::value(sqliteheader.largestrootbtreepagenumber), "SQLite Largest Root B-Tree Page Number");
        AddProperty(12, "56, 4", FXString::value(sqliteheader.textencoding), "SQLite Text Encoding");
        AddProperty(13, "60, 4", FXString::value(sqliteheader.userversion), "SQLite User Version");
        AddProperty(14, "64, 4", FXString::value(sqliteheader.incrementalvacuummodebool), "SQLite Incremental Vacuum Mode Boolean");
        AddProperty(15, "68, 4", FXString::value(sqliteheader.appid), "SQLite App ID");
        AddProperty(16, "92, 4", FXString::value(sqliteheader.versionvalidfornum), "SQLite Version Valid for Number");
        AddProperty(17, "96, 4", FXString::value(sqliteheader.version), "SQLite Version");
    }
    //proptable->fitColumnsToContents(2);
    //AlignColumn(proptable, 2, FXTableItem::LEFT);
}

void WombatForensics::ParsePageHeader(uint8_t* pagearray, uint8_t fileheader, uint64_t curpage)
{
    uint64_t curpos = 0;
    uint64_t cellarrayoffset = 0;
    FXArray<uint16_t> celloffsetarray;
    //uint rowcnt = 0;
    if(curpage == 1)
    {
        if((uint)filetype == 0x01) // WAL
            curpos = 32;
        else if((uint)filetype == 0x02) // JOURNAL
            curpos = 28;
        else if((uint)filetype == 0x03) // DB
            curpos = 100;
        //rowcnt = proptable->getNumRows();
    }
    if((uint)filetype == 0x01) // WAL
    {
        ReadInteger(pagearray, curpos, &frameheader.pagenumber);
        ReadInteger(pagearray, curpos + 4, &frameheader.pagecount);
        ReadInteger(pagearray, curpos + 8, &frameheader.salt1);
        ReadInteger(pagearray, curpos + 12, &frameheader.salt2);
        ReadInteger(pagearray, curpos + 16, &frameheader.checksum1);
        ReadInteger(pagearray, curpos + 20, &frameheader.checksum1);
        //qDebug() << "frameheader:" << frameheader.pagenumber << frameheader.pagecount << frameheader.salt1 << frameheader.salt2 << frameheader.checksum1 << frameheader.checksum2;
        // AFTER THE FRAMEHEADER IS A PAGE, WHICH IS THE PAGESIZE OF THE DB PAGESIZE FROM THE FRAMEHEADER, SO I NEED TO REDO THE
        // HEX DISPLAY AND PAGE DISPLAY FOR THE WAL FILES...
    }
    else if((uint)filetype == 0x02) // JOURNAL
    {
    }
    else if((uint)filetype == 0x03) // DB
    {
        pageheader.type = pagearray[curpos];
        ReadInteger(pagearray, curpos + 1, &pageheader.firstfreeblock);
        ReadInteger(pagearray, curpos + 3, &pageheader.cellcount);
        ReadInteger(pagearray, curpos + 5, &pageheader.cellcontentstart);
        pageheader.fragmentedfreebytescount = pagearray[curpos, 7];
        proptablerowcnt += 5;
        //std::cout << "pagehader.type: " << std::hex << (uint)pageheader.type << std::endl;
        cellarrayoffset = curpos + 8;
        if((uint)pageheader.type == 0x02 || (uint)pageheader.type == 0x05)
        {
            std::cout << "interior table/leaf" << std::endl;
            proptablerowcnt += 1;
            ReadInteger(pagearray, curpos + 8, &pageheader.rightmostpagenumber);
            cellarrayoffset = curpos + 12;
            //AddProperty(rowcnt + 5, FXString::value(curpos + 8) + ", 4", FXString::value(pageheader.rightmostpagenumber), "Largest page number, right most pointer");
        }
        // Parse cell pointers and rows here
        celloffsetarray.clear();
        if(pageheader.cellcount > 0)
        {
            proptablerowcnt += pageheader.cellcount;
            for(int i=0; i < pageheader.cellcount; i++)
            {
                uint16_t tmpoff = 0;
                ReadInteger(pagearray, cellarrayoffset + 2*i, &tmpoff);
                celloffsetarray.append(tmpoff);
                //AddProperty(rowcnt + i, FXString::value(cellarrayoffset + 2*i) + ", 2", FXString::value(tmpoff), "Cell Array Offset " + FXString::value(i+1));
            }
        }
    }
    PopulatePropertyTable(&celloffsetarray);
    ParseRowContents(pagearray, &celloffsetarray);
}

void WombatForensics::PopulatePageHeader(FXArray<uint16_t>* celloffsetarray)
{
    uint64_t curpos = 0;
    uint64_t cellarrayoffset = 0;
    int rowcnt = 0;
    //int rowcnt = proptable->getNumRows();
    if((uint)filetype == 0x01) // WAL
    {
        if(curpage == 1)
        {
            rowcnt = 8;
            curpos = 32;
        }
    }
    else if((uint)filetype == 0x02) // JOURNAL
    {
        if(curpage == 1)
        {
            rowcnt = 6;
            curpos = 28;
        }
    }
    else if((uint)filetype == 0x03) // DB
    {
        if(curpage == 1)
        {
            rowcnt = 18;
            curpos = 100;
        }
        AddProperty(rowcnt, FXString::value(curpos) + ", 1", "0x" + FXString::value("%02x", pageheader.type), "Page Type: 0x02 | 0x05 - Index | Table Interior, 0x0d | 0x0d - Index | Table Leaf, any other value is error.");
        //AddProperty(rowcnt, FXString::value(curpos) + ", 1", "0x" + FXString::value(pageheader.type, 16), "Page Type: 0x02 | 0x05 - Index | Table Interior, 0x0d | 0x0d - Index | Table Leaf, any other value is error.");
        AddProperty(rowcnt + 1, FXString::value(curpos + 1) + ", 2", FXString::value(pageheader.firstfreeblock), "Start of the first free block on the page or zero for no free blocks");
        AddProperty(rowcnt + 2, FXString::value(curpos + 3) + ", 2", FXString::value(pageheader.cellcount), "Number of cells on the page");
        AddProperty(rowcnt + 3, FXString::value(curpos + 5) + ", 2", FXString::value(pageheader.cellcontentstart), "Start of the cell content area, zero represents 65536");
        AddProperty(rowcnt + 4, FXString::value(curpos + 7) + ", 1", FXString::value(pageheader.fragmentedfreebytescount), "Number of fragmented free bytes within cell content area");
        cellarrayoffset = curpos + 8;
        rowcnt = rowcnt + 4;
        if((uint)pageheader.type == 0x02 || (uint)pageheader.type == 0x05)
        {
            cellarrayoffset = curpos + 12;
            AddProperty(rowcnt + 5, FXString::value(curpos + 8) + ", 4", FXString::value(pageheader.rightmostpagenumber), "Largest page number, right most pointer");
            rowcnt = rowcnt + 5;
        }
        for(int i=0; i < pageheader.cellcount; i++)
        {
            AddProperty(rowcnt + i, FXString::value(cellarrayoffset + 2*i) + ", 2", FXString::value(celloffsetarray->at(i)), "Cell Array Offset " + FXString::value(i+1));
        }
    }
}

void WombatForensics::PopulatePropertyTable(FXArray<uint16_t>* celloffsetarray)
{
    proptable->setTableSize(proptablerowcnt, 3);
    proptable->setColumnText(0, "Offset, Length");
    proptable->setColumnText(1, "Value");
    proptable->setColumnText(2, "Description");
    if(curpage == 1)
        PopulateFileHeader();
    PopulatePageHeader(celloffsetarray);
    proptable->fitColumnsToContents(2);
    AlignColumn(proptable, 0, FXTableItem::LEFT);
    AlignColumn(proptable, 1, FXTableItem::LEFT);
    AlignColumn(proptable, 2, FXTableItem::LEFT);
}

uint WombatForensics::GetVarIntLength(uint8_t* pagearray, uint64_t pageoffset)
{
    bool getnextbyte = true;
    uint length = 1;
    while(getnextbyte == true)
    {
        uint8_t curbyte = 0;
        curbyte = pagearray[pageoffset + length - 1];
        //quint8 curbyte = qFromBigEndian<quint8>(pagearray->mid(pageoffset + length - 1, 1));
        if((uint)curbyte >= 0x80)
            length++;
        else
            getnextbyte = false;
    }
    return length;
}

uint WombatForensics::GetVarInt(uint8_t* pagearray, uint64_t pageoffset, uint varintlength)
{
    FXArray<uint8_t> varbytes;
    varbytes.clear();
    for(uint i=0; i < varintlength; i++)
        varbytes.append(pagearray[pageoffset + i]);
        //varbytes.append(pagearray->mid(pageoffset + i, 1));
    //qDebug() << "pageoffset:" << pageoffset << "length:" << varintlength;
    //qDebug() << "varbytes:" << varbytes.toHex() << "varbytes count:" << varbytes.count();
    if(varintlength > 1)
    {
        uint i;
        uint64_t x;
        uint64_t uX = 0;
        for(i=0; i < varintlength && i < 9; i++)
        {
            x = (uint8_t)varbytes.at(i);
            //x = (quint8)varbytes.at(i);
            uX = (uX<<7) + (x&0x7f);
            if((x&0x80)==0)
                break;
        }
        if(i == 9 && i < varintlength)
            uX = (uX<<8) + x;
        i++;
        //qDebug() << "varint:" << uX;
        return uX;
    }
    else
        return varbytes.at(0);
        //return qFromBigEndian<quint8>(varbytes);
}

uint64_t WombatForensics::GetSerialType(uint64_t contentoffset, FXString* tmptype, FXString* tmpval, FXString* tmpofflen, uint8_t* pagearray, int serialtype)
{
    if(serialtype == 0) // col length is zero, so content length doesn't change
    {
        *tmptype = "0 - NULL";
        *tmpval = "NULL";
        //qDebug() << "NULL";
        return contentoffset;
    }
    else if(serialtype == 1) // uint8_t (1)
    {
        *tmpofflen = FXString::value(contentoffset) + ", 1";
        *tmptype = "1 - 8-bit int";
        *tmpval = FXString::value((uint)(pagearray[contentoffset]));
        //qDebug() << "1 byte" << qFromBigEndian<quint8>(pagearray->mid(contentoffset, 1));
        return contentoffset + 1;
        //contentoffset++;
    }
    else if(serialtype == 2) // uint16_t (2)
    {
        *tmpofflen = FXString::value(contentoffset) + ", 2";
        *tmptype = "2 - 16-bit int";
        uint16_t tmp16 = 0;
        ReadInteger(pagearray, contentoffset, &tmp16);
        *tmpval = FXString::value(tmp16);
        //qDebug() << "2 byte:" << qFromBigEndian<quint16>(pagearray->mid(contentoffset, 2));
        return contentoffset + 2;
    }
    else if(serialtype == 3) // uint24_t (3)
    {
        *tmpofflen = FXString::value(contentoffset) + ", 3";
        *tmptype = "3 - 24-bit int";
        uint32_t tmp24 = __builtin_bswap32((uint32_t)pagearray[contentoffset] | (uint32_t)pagearray[contentoffset + 1] << 8 | (uint32_t)pagearray[contentoffset + 2] << 16);
        *tmpval = FXString::value(tmp24);
        //qDebug() << "3 bytes:" << qFromBigEndian<quint32>(pagearray->mid(contentoffset, 3));
        return contentoffset + 3;
    }
    else if(serialtype == 4) // uint32_t (4)
    {
        *tmpofflen = FXString::value(contentoffset) + ", 4";
        *tmptype = "4 - 32-bit int";
        uint32_t tmp32 = 0;
        ReadInteger(pagearray, contentoffset, &tmp32);
        *tmpval = FXString::value(tmp32);
        //qDebug() << "4 bytes:" << qFromBigEndian<quint32>(pagearray->mid(contentoffset, 4));
        return contentoffset + 4;
    }
    else if(serialtype == 5) // uint48_t (6)
    {
        *tmpofflen = FXString::value(contentoffset) + ", 6";
        *tmptype = "5 - 48-bit int";
        uint64_t tmp48 = __builtin_bswap64((uint64_t)pagearray[contentoffset] | (uint64_t)pagearray[contentoffset + 1] << 8 | (uint64_t)pagearray[contentoffset + 2] << 16 | (uint64_t)pagearray[contentoffset + 3] << 24 | (uint64_t)pagearray[contentoffset + 4] << 32 | (uint64_t)pagearray[contentoffset + 5] << 40);
        *tmpval = FXString::value(tmp48);
        //qDebug() << "6 bytes:" << qFromBigEndian<quint64>(pagearray->mid(contentoffset, 6));
        return contentoffset + 6;
    }
    else if(serialtype == 6) // uint64_t (8)
    {
        *tmpofflen = FXString::value(contentoffset) + ", 8";
        *tmptype = "6 - 64-bit int";
        uint64_t tmp64 = 0;
        ReadInteger(pagearray, contentoffset, &tmp64);
        *tmpval = FXString::value(tmp64);
        //qDebug() << "8 bytes:" << qFromBigEndian<quint64>(pagearray->mid(contentoffset, 8));
        return contentoffset + 8;
    }
    else if(serialtype == 7) // double (8)
    {
        *tmpofflen = FXString::value(contentoffset) + ", 8";
        *tmptype = "7 - 64-bit double";
        uint64_t tmp64 = 0;
        ReadInteger(pagearray, contentoffset, &tmp64);
        //double tmp64 = __builtin_bswap64((double)pagearray[contentoffset] | (double)pagearray[contentoffset + 1] << 8 | (double)pagearray[contentoffset + 2] << 16 | (double)pagearray[contentoffset + 3] << 24 | (double)pagearray[contentoffset + 4] << 32 | (double)pagearray[contentoffset + 5] << 40 | (double)pagearray[contentoffset + 6] << 48 | (double)pagearray[contentoffset + 7] << 56);
        *tmpval = FXString::value((double)tmp64);
        //qDebug() << "8 bytes:" << qFromBigEndian<double>(pagearray->mid(contentoffset, 8));
        return contentoffset + 8;
    }
    else if(serialtype == 8) // col length is zero, so content length doesn't change
    {
        *tmptype = "8 - Integer value 0";
        *tmpval = "0";
        return contentoffset;
        //qDebug() << "0";
    }
    else if(serialtype == 9) // col length is zero, so content length doesn't change)
    {
        *tmptype = "9 - Integer value 1";
        *tmpval = "1";
        return contentoffset;
        //qDebug() << "1";
    }
    else if(serialtype >= 12) // BLOB OR TEXT
    {
        if(serialtype % 2 == 0) // EVEN AND BLOB
        {
            *tmpofflen = FXString::value(contentoffset) + ", " + FXString::value((serialtype - 12) / 2);
            *tmptype = ">=12 && even - BLOB";
            std::stringstream ss;
            ss << std::hex << std::setfill('0') << std::setw(2);
            for(int i=contentoffset; i < ((serialtype - 12) / 2); i++)
                ss << (uint)pagearray[i];
            *tmpval = FXString(ss.str().c_str());
            // *tmpval = FXString((char*)substr(pagearray, contentoffset, (serialtype - 12) / 2));
            //tmpval = pagearray->mid(contentoffset, (serialtype - 12) / 2).toHex();
            //qDebug() << "blob size:" << (serialtype - 12) / 2 << pagearray->mid(contentoffset, (curst-12) / 2).toHex();
            return contentoffset + ((serialtype - 12) / 2);
        }
        else // ODD AND TEXT
        {
            *tmpofflen = FXString::value(contentoffset) + ", " + FXString::value((serialtype - 13) / 2);
            *tmptype = ">=13 & odd - TEXT";
            *tmpval = FXString((char*)substr(pagearray, contentoffset, (serialtype - 13) / 2));
            // *tmpval = QString::fromStdString(pagearray->mid(contentoffset, (serialtype - 13) / 2).toStdString());
            //sqliteheader.header = FXString((char*)substr(pageheader, 0, 16));
            //qDebug() << "Text Size:" << (serialtype - 13) / 2 << QString::fromStdString(pagearray->mid(contentoffset, (curst - 13) / 2).toStdString());
            return contentoffset + ((serialtype - 13) / 2);
        }
    }
    return contentoffset;
}

void WombatForensics::ParseRowContents(uint8_t* pagearray, FXArray<uint16_t>* celloffsetarray)
{
    if(pageheader.cellcount > 0)
    {
        int tmprow = 0;
        FXString tmprowid = "";
        for(int i=0; i < celloffsetarray->no(); i++)
        {
            if((uint)pageheader.type == 0x02) // index interior
            {
                uint32_t pagenum = 0;
                ReadInteger(pagearray, celloffsetarray->at(i), &pagenum);
                FXString curidvalue = FXString::value(sqlfilelist->getCurrentItem()) + "," + FXString::value(curpage) + "," + FXString::value(tmprow);
                FXString tagstr = "";
                / * TAGGING, WILL DO LATER
                for(int j=0; j < taggedlist.no(); j++)
                {
                    if(taggeditems.at(j).contains(curidvalue))
                        tagstr = taggeditems.at(j).split("|", Qt::SkipEmptyParts).first();
                }
                * /
                AddContent(tmprow, "True", "", FXString::value(celloffsetarray->at(i)) + ", 4", "Page Number", FXString::value(pagenum), tagstr);
                tmprow++;
                uint payloadlength = GetVarIntLength(pagearray, celloffsetarray->at(i) + 4);
                uint payloadsize = GetVarInt(pagearray, celloffsetarray->at(i) + 4, payloadlength);
                tagstr = "";
                curidvalue = FXString::value(sqlfilelist->getCurrentItem()) + "," + FXString::value(curpage) + "," + FXString::value(tmprow);
                / * TAGGING TO IMPLEMENT LATER
                for(int j=0; j < taggedlist.no(); j++)
                {
                    if(taggeditems.at(j).contains(curidvalue))
                        tagstr = taggeditems.at(j).split("|", Qt::SkipEmptyParts).first();
                }
                * /
                //sqliteheader.header = FXString((char*)substr(pageheader, 0, 16));
                AddContent(tmprow, "True", "", FXString::value(celloffsetarray->at(i) + 4) + ", " + FXString::value(payloadsize), "Payload", FXString((char*)substr(pagearray, celloffsetarray->at(i) + 4 + payloadlength, payloadsize)), tagstr);
                tmprow++;
                / *
                 * payload = 33, byte array 4, initial payload not in overflow - 27, 
                uint recordlengthlength = GetVarIntLength(pagearray, celloffarray.at(i) + payloadlength);
                uint recordlength = GetVarInt(pagearray, celloffarray.at(i) + payloadlength, recordlengthlength);
                quint64 contentoffset = celloffarray.at(i) + payloadlength + recordlength;
                QList<int> serialtypes;
                serialtypes.clear();
                QByteArray serialarray = pagearray->mid(celloffarray.at(i) + payloadlength + recordlengthlength, recordlength - recordlengthlength);
                //qDebug() << "serialarray:" << serialarray.toHex() << "serialarray.count:" << serialarray.count();
                uint curserialtypelength = 0;
                while(curserialtypelength < serialarray.count())
                {
                    uint curstlen = GetVarIntLength(&serialarray, curserialtypelength);
                    uint curst = GetVarInt(&serialarray, curserialtypelength, curstlen);
                    curserialtypelength += curstlen;
                    serialtypes.append(GetSerialType(curst));
                }
                if(i == 0)
                {
                    uint tmprowcnt = serialtypes.count() * celloffarray.count();
                    ui->tablewidget->setRowCount(tmprowcnt);
                }
                for(int j=0; j < serialtypes.count(); j++)
                {
                    uint curst = serialtypes.at(j);
                    QString tmpofflen = "";
                    QString tmptype = "";
                    QString tmpval = "";
                    GetSerialType(contentoffset, &tmptype, &tmpval, &tmpofflen, pagearray, serialtypes.at(j));
                    //qDebug() << "curtmprow:" << curtmprow;
                    AddContent(curtmprow, "True", "", tmpofflen, tmptype, tmpval);
                    curtmprow++;
                }
                * /
            }
            else if((uint)pageheader.type == 0x05) // table interior
            {
                uint32_t pagenum = 0;
                ReadInteger(pagearray, celloffsetarray->at(i), &pagenum);
                uint rowidlength = GetVarIntLength(pagearray, celloffsetarray->at(i) + 4);
                uint rowid = GetVarInt(pagearray, celloffsetarray->at(i) + 4, rowidlength);
                FXString curidvalue = FXString::value(sqlfilelist->getCurrentItem()) + "," + FXString::value(curpage) + "," + FXString::value(tmprow);
                FXString tagstr = "";
                AddContent(tmprow, "True", FXString::value(rowid), FXString::value(celloffsetarray->at(i)) + ", 4", "Page Number", FXString::value(pagenum), tagstr);
                tmprow++;
            }
            else if((uint)pageheader.type == 0x0a) // index leaf
            {
                uint payloadlength = GetVarIntLength(pagearray, celloffsetarray->at(i));
                uint payloadsize = GetVarInt(pagearray, celloffsetarray->at(i), payloadlength);
                uint recordlengthlength = GetVarIntLength(pagearray, celloffsetarray->at(i) + payloadlength);
                uint recordlength = GetVarInt(pagearray, celloffsetarray->at(i) + payloadlength, recordlengthlength);
                uint64_t contentoffset = celloffsetarray->at(i) + payloadlength + recordlength;
                FXArray<int> serialtypes;
                serialtypes.clear();
                uint8_t* serialarray = substr(pagearray, celloffsetarray->at(i) + payloadlength + recordlengthlength, recordlength - recordlengthlength);
                uint curserialtypelength = 0;
                while(curserialtypelength < recordlength - recordlengthlength)
                {
                    uint curstlen = GetVarIntLength(serialarray, curserialtypelength);
                    uint curst = GetVarInt(serialarray, curserialtypelength, curstlen);
                    curserialtypelength += curstlen;
                    serialtypes.append(curst);
                }
                if(i == 0)
                {
                    tablelist->setTableSize(serialtypes.no() * celloffsetarray->no(), 6);
                    tablelist->setColumnText(0, "Tag");
                    tablelist->setColumnText(1, "Is Live");
                    tablelist->setColumnText(2, "Row ID");
                    tablelist->setColumnText(3, "Offset, Length");
                    tablelist->setColumnText(4, "Type");
                    tablelist->setColumnText(5, "Value");
                }
                for(int j=0; j < serialtypes.no(); j++)
                {
                    FXString tmptype = "";
                    FXString tmpval = "";
                    FXString tmpofflen = "";
                    GetSerialType(contentoffset, &tmptype, &tmpval, &tmpofflen, pagearray, serialtypes.at(j));
                    FXString curidvalue = FXString::value(sqlfilelist->getCurrentItem()) + "," + FXString::value(curpage) + "," + FXString::value(tmprow);
                    FXString tagstr = "";
                    AddContent(tmprow, "True", "", tmpofflen, tmptype, tmpval, tagstr);
                    tmprow++;
                }
            }
            else if((uint)pageheader.type == 0x0d) // table leaf
            {
                uint payloadlength = GetVarIntLength(pagearray, celloffsetarray->at(i));
                uint payloadsize = GetVarInt(pagearray, celloffsetarray->at(i), payloadlength);
                uint rowidlength = GetVarIntLength(pagearray, celloffsetarray->at(i) + payloadlength);
                uint rowid = GetVarInt(pagearray, celloffsetarray->at(i) + payloadlength, rowidlength);
                tmprowid = FXString::value(rowid);
                uint recordlengthlength = GetVarIntLength(pagearray, celloffsetarray->at(i) + payloadlength + rowidlength);
                uint recordlength = GetVarInt(pagearray, celloffsetarray->at(i) + payloadlength + rowidlength, recordlengthlength);
                uint64_t contentoffset = celloffsetarray->at(i) + payloadlength + rowidlength + recordlength;
                FXArray<int> serialtypes;
                serialtypes.clear();
                uint8_t* serialarray = substr(pagearray, celloffsetarray->at(i) + payloadlength + rowidlength + recordlengthlength, recordlength - recordlengthlength);
                uint curserialtypelength = 0;
                while(curserialtypelength < recordlength - recordlengthlength)
                {
                    uint curstlen = GetVarIntLength(serialarray, curserialtypelength);
                    uint curst = GetVarInt(serialarray, curserialtypelength, curstlen);
                    curserialtypelength += curstlen;
                    serialtypes.append(curst);
                }
                if(i == 0)
                {
                    tablelist->setTableSize(serialtypes.no() * celloffsetarray->no(), 6);
                    tablelist->setColumnText(0, "Tag");
                    tablelist->setColumnText(1, "Is Live");
                    tablelist->setColumnText(2, "Row ID");
                    tablelist->setColumnText(3, "Offset, Length");
                    tablelist->setColumnText(4, "Type");
                    tablelist->setColumnText(5, "Value");
                }
                for(int j=0; j < serialtypes.no(); j++)
                {
                    FXString tmptype = "";
                    FXString tmpval = "";
                    FXString tmpofflen = "";
                    contentoffset = GetSerialType(contentoffset, &tmptype, &tmpval, &tmpofflen, pagearray, serialtypes.at(j));
                    FXString curidvalue = FXString::value(sqlfilelist->getCurrentItem()) + "," + FXString::value(curpage) + "," + FXString::value(tmprow);
                    FXString tagstr = "";
                    AddContent(tmprow, "True", tmprowid, tmpofflen, tmptype, tmpval, tagstr);
                    tmprow++;
                }
            }
            // UPDATE TAGS HERE....
        }
        tablelist->fitColumnsToContents(4);
        tablelist->fitColumnsToContents(5);
        AlignColumn(tablelist, 0, FXTableItem::LEFT);
        AlignColumn(tablelist, 1, FXTableItem::LEFT);
        AlignColumn(tablelist, 2, FXTableItem::LEFT);
        AlignColumn(tablelist, 3, FXTableItem::LEFT);
        AlignColumn(tablelist, 4, FXTableItem::LEFT);
        AlignColumn(tablelist, 5, FXTableItem::LEFT);
    }
    else // root page of a table with no rows
    {
        //qDebug() << "cell content area offset is equal to pagesize - reserved space bytes.";
    }
}
*/

void WombatForensics::AlignColumn(FXTable* curtable, int col, FXuint justify)
{
    for(int i=0; i < curtable->getNumRows(); i++)
        curtable->setItemJustify(i, col, justify);
}

void WombatForensics::FitColumnContents(int col)
{
    tablelist->fitColumnsToContents(col);
    tablelist->setColumnWidth(col, tablelist->getColumnWidth(col) + 10);
}

/*
long WombatForensics::PropertySelected(FXObject*, FXSelector, void*)
{
    proptable->selectRow(proptable->getCurrentRow());
    if(proptable->getCurrentRow() > -1 && !proptable->getItemText(proptable->getCurrentRow(), 0).empty())
    {
        FXString offlen = proptable->getItemText(proptable->getCurrentRow(), 0);
        int found = offlen.find(", ");
        uint64_t curoffset = offlen.mid(0, found).toULong();
        uint64_t curlength = offlen.mid(found+1, offlen.length() - found).toULong();
        uint linenumber = curoffset / 16;
        // SET SCROLLBAR
        textscrollbar->setPosition(linenumber);
        // SET OFFSET
        offsettext->moveCursorRowColumn(linenumber, 0);
        // SET HEX SELECTION
        hextext->moveCursor(curoffset*3 + linenumber);
        hextext->setSelection(curoffset * 3 + linenumber, curlength*3 - 1);
        //hextext->setAnchorPos(curoffset*3 + linenumber);
        //hextext->moveCursorAndSelect(curoffset*3 + linenumber + curlength*3 - 1, 1);
        // SET ASCII HIGHLIGHT
        asciitext->moveCursor(curoffset + linenumber);
        asciitext->setHighlight(curoffset + linenumber, curlength);
        StatusUpdate("Offset: 0x" + FXString::value("%05x", curoffset) + " | Length: " + FXString::value(curlength));
    }

    return 1;
}
*/

long WombatForensics::ContentSelected(FXObject*, FXSelector, void*)
{
    tablelist->selectRow(tablelist->getCurrentRow());

    return 1;
}

long WombatForensics::LoadChildren(FXObject*, FXSelector, void*)
{
    //std::cout << *((int*)tablelist->getItemData(tablelist->getCurrentRow(), 1)) << std::endl;
    curforimg = (ForImg*)tablelist->getItemData(tablelist->getCurrentRow(), 2);
    if(itemtype == 1)
    {
        volnames.clear();
        volsizes.clear();
        voloffsets.clear();
        //std::string volname = "";
        //uint64_t partitionsize = 0;
        LoadPartitions(curforimg, &volnames, &volsizes, &voloffsets);
        //LoadPartitions(curforimg, &volname, &partitionsize);
        //std::cout << "volname: " << volname << " partition size: " << partitionsize << std::endl;
        for(int i=0; i < volnames.size(); i++)
        {
            std::cout << volnames.at(i) << " " << volsizes.at(i) << " " << voloffsets.at(i) << std::endl;
        }
    }
    else if(itemtype == 2)
    {
        std::cout << "need to load the root directory for the partition selected here." << std::endl;
    }
    else
        std::cout << "not a forensic image, so need to load something else here." << std::endl;

    // TEST FUNCTIONING OF WHETHER IT IS CHECKED...
    //bool iscurchecked = ((CheckTableItem*)tablelist->getItem(tablelist->getCurrentRow(), 0))->getCheck();
    //std::cout << "is current item checked: " << iscurchecked << std::endl;
    return 1;
}

/*
void WombatForensics::LoadPartitions(ForImg* curforimg)
{
    uint16_t mbrsig = 0;
    uint16_t applesig  = 0;
    uint32_t bsdsig = 0;
    uint16_t sunsig = 0;
    uint64_t gptsig = 0;
    ReadForImgContent(curforimg, &mbrsig, 510);
    ReadForImgContent(curforimg, &applesig, 0);
    ReadForImgContent(curforimg, &bsdsig, 0);
    ReadForImgContent(curforimg, &sunsig, 508);
    ReadForImgContent(curforimg, &gptsig, 0);
    if(mbrsig == 0xaa55) // possibly MBR or GPT
    {
        uint8_t gptdif = 0x00;
        curforimg->ReadContent(&gptdif, 450, 1);
        if((uint)gptdif == 0xee) // GPT DISK
        {
            ReadForImgContent(curforimg, &gptsig, 512, 8);
            if(gptsig == 0x4546492050415254) // GUID PARTITION TABLE
                LoadGptPartitions(curforimg);
        }
        else // MBR DISK
        {
            char* exfattype = new char[6];
            curforimg->ReadContent((uint8_t*)exfattype, 3, 5);
            exfattype[5] = 0;
            char* fattype = new char[6];
            curforimg->ReadContent((uint8_t*)fattype, 54, 5);
            fattype[5] = 0;
            char* fat32type = new char[6];
            curforimg->ReadContent((uint8_t*)fat32type, 82, 5);
            fat32type[5] = 0;
            char* ntfstype = new char[5];
            curforimg->ReadContent((uint8_t*)ntfstype, 3, 4);
            ntfstype[4] = 0;
            char* bfstype = new char[5];
            curforimg->ReadContent((uint8_t*)bfstype, 544, 4);
            bfstype[4] = 0;
            if(strcmp(exfattype, "EXFAT") == 0 || strcmp(fattype, "FAT12") == 0 || strcmp(fattype, "FAT16") == 0 || strcmp(fattype, "FAT32") == 0 || strcmp(ntfstype, "NTFS") == 0 || strcmp(bfstype, "1SFB") == 0) // EXFAT | FAT12 | FAT16 | FAT32 | NTFS | BFS W/O PARTITION TABLE
            {
                // table initialization
                tablelist->setTableSize(1, 14);
                tablelist->setColumnText(0, "");
                tablelist->setColumnText(1, "ID");
                tablelist->setColumnText(2, "Name");
                tablelist->setColumnText(3, "Path");
                tablelist->setColumnText(4, "Size (bytes)");
                tablelist->setColumnText(5, "Created");
                tablelist->setColumnText(6, "Accessed");
                tablelist->setColumnText(7, "Modified");
                tablelist->setColumnText(8, "Changed");
                tablelist->setColumnText(9, "Hash");
                tablelist->setColumnText(10, "Category");
                tablelist->setColumnText(11, "Signature");
                tablelist->setColumnText(12, "Tagged");
                tablelist->setColumnText(13, "Hash Match");

                // partition information
                itemtype = 2;
                tablelist->setItem(0, 0, new CheckTableItem(tablelist, NULL, NULL, ""));
                tablelist->setItemData(0, 1, &itemtype);
                tablelist->setItemText(0, 1, FXString::value(0));
                tablelist->setItemData(0, 2, curforimg);
                tablelist->setItemText(0, 2, GetFileSystemName(curforimg, 0));
                tablelist->setItemIcon(0, 2, partitionicon);
                tablelist->setItemIconPosition(0, 2, FXTableItem::BEFORE);
                tablelist->setItemText(0, 4, FXString(ReturnFormattingSize(curforimg->Size()).c_str()));

                // table formatting
                tablelist->fitColumnsToContents(0);
                tablelist->setColumnWidth(0, tablelist->getColumnWidth(0) + 25);
                FitColumnContents(1);
                FitColumnContents(2);
                FitColumnContents(4);
                AlignColumn(tablelist, 1, FXTableItem::LEFT);
                AlignColumn(tablelist, 2, FXTableItem::LEFT);

                //std::cout << "parse fs here.." << std::endl;
                // parse partition(curofrimg, 0, curforimg->Size/512, 0, 1);
            }
            uint8_t ptreecnt = 0;
            int pcount = 0;
            for(int i=0; i < 4; i++)
            {
                uint32_t partsize = 0;
                ReadForImgContent(curforimg, &partsize, 458 + i*16);
                if(partsize > 0)
                    pcount++;
            }
            for(int i=0; i < pcount; i++)
            {
                uint8_t curparttype = 0;
                curforimg->ReadContent(&curparttype, 450 + i*16, 1);
                uint32_t curoffset = 0;
                ReadForImgContent(curforimg, &curoffset, 454 + i*16);
                uint32_t cursize = 0;
                ReadForImgContent(curforimg, &cursize, 458 + i*16);
                uint64_t sectorcheck = 0;
                if(i == 0)
                    sectorcheck = 0;
                else if(i > 0 && i < pcount - 1)
                {
                    uint32_t prevoffset = 0;
                    ReadForImgContent(curforimg, &prevoffset, 454 + (i-1) * 16);
                    uint32_t prevsize = 0;
                    ReadForImgContent(curforimg, &prevsize, 458 + (i-1) * 16);
                    sectorcheck = prevoffset + prevsize;
                }
                else if(i == pcount - 1)
                    sectorcheck = curforimg->Size() / 512;
                if(curoffset > sectorcheck) // add unallocated partition
                {
                    // Parse Partition(curforimg, sectorcheck, curoffset, ptreecnt, 0);
                }
                if((uint)curparttype == 0x05) // extended partition
                {
                    // ptreecnt = Parse Extended Partition(curforimg, curoffset, cursize, ptreecnt);
                }
                else if((uint)curparttype == 0x00)
                {
                    // do nothing here cause it's an empty partition
                }
                else if((uint)curparttype == 0x82) // Sun i386
                {
                    // parse sun table here passing pofflist nad psizelist
                }
                else if((uint)curparttype == 0xa5 || (uint)curparttype == 0xa6 || (uint)curparttype == 0xa9) // BSD
                {
                    // parse BSD table here passing pofflist and psizelist
                }
                else
                {
                    if(cursize > 0)
                    {
                        // Parse Partition(curforimg, curoffset, cursize, ptreecnt, 1);
                    }
                }
                if( i == pcount - 1 && curoffset + cursize < curforimg->Size() / 512 - 1) // ADD UNALLOCATED AT END
                {
                    // parse partition(curforimg, curoffset + cursize, curimg->Size / 512 - (curoffset + cursize), ptreecnt, 0);
                }
                    
            }
        }
    }
    else if(applesig == 0x504d) // APPLE PARTITION
    {
        std::cout << "apple sig here..." << std::endl;
    }
    else if(bsdsig == 0x82564557) // BSD PARTITION
    {
        std::cout << "bsd part here..." << std::endl;
    }
    else if(sunsig == 0xDABE) // SUN PARTITION
    {
        std::cout << "determine if sparc or i386 and then process partitions." << std::endl;
    }
    else if(gptsig == 0x4546492050415254) // GPT PARTITION
        LoadGptPartitions(curforimg);
    else // NO PARTITION MAP, JUST A FS AT ROOT OF IMAGE
    {
        // table initialization
        tablelist->setTableSize(1, 14);
        tablelist->setColumnText(0, "");
        tablelist->setColumnText(1, "ID");
        tablelist->setColumnText(2, "Name");
        tablelist->setColumnText(3, "Path");
        tablelist->setColumnText(4, "Size (bytes)");
        tablelist->setColumnText(5, "Created");
        tablelist->setColumnText(6, "Accessed");
        tablelist->setColumnText(7, "Modified");
        tablelist->setColumnText(8, "Changed");
        tablelist->setColumnText(9, "Hash");
        tablelist->setColumnText(10, "Category");
        tablelist->setColumnText(11, "Signature");
        tablelist->setColumnText(12, "Tagged");
        tablelist->setColumnText(13, "Hash Match");

        // partition information
        itemtype = 2;
        tablelist->setItem(0, 0, new CheckTableItem(tablelist, NULL, NULL, ""));
        tablelist->setItemData(0, 1, &itemtype);
        tablelist->setItemText(0, 1, FXString::value(0));
        tablelist->setItemData(0, 2, curforimg);
        tablelist->setItemText(0, 2, GetFileSystemName(curforimg, 0));
        tablelist->setItemIcon(0, 2, partitionicon);
        tablelist->setItemIconPosition(0, 2, FXTableItem::BEFORE);
        tablelist->setItemText(0, 4, FXString(ReturnFormattingSize(curforimg->Size()).c_str()));

        // ParsePartition(curforimg, 0, curimg->Size()/512, 0 (part id), 1 (allocated));
        // table formatting
        tablelist->fitColumnsToContents(0);
        tablelist->setColumnWidth(0, tablelist->getColumnWidth(0) + 25);
        FitColumnContents(1);
        FitColumnContents(2);
        FitColumnContents(4);
        AlignColumn(tablelist, 1, FXTableItem::LEFT);
        AlignColumn(tablelist, 2, FXTableItem::LEFT);
    }
}

void WombatForensics::LoadGptPartitions(ForImg* curforimg)
{
    uint64_t parttablestart = 0;
    uint32_t partentrycount = 0;
    uint32_t partentrysize = 0;
    ReadForImgContent(curforimg, &parttablestart, 584);
    ReadForImgContent(curforimg, &partentrycount, 592);
    ReadForImgContent(curforimg, &partentrysize, 596);
    int ptreecnt = 0;
    int pcount = 0;
    for(int i=0; i < partentrycount; i++)
    {
        int cnt = i * partentrysize;
        uint64_t curstartsector = 0;
        uint64_t curendsector = 0;
        ReadForImgContent(curforimg, &curstartsector, parttablestart * 512 + cnt + 32);
        ReadForImgContent(curforimg, &curendsector, parttablestart * 512 + cnt + 40);
        if(curendsector - curstartsector > 0) // PARTITION VALUES MAKE SENSE
            pcount++;
    }
    tablelist->setTableSize(pcount * 2 + 1, 14);
    tablelist->setColumnText(0, "");
    tablelist->setColumnText(1, "ID");
    tablelist->setColumnText(2, "Name");
    tablelist->setColumnText(3, "Path");
    tablelist->setColumnText(4, "Size (bytes)");
    tablelist->setColumnText(5, "Created");
    tablelist->setColumnText(6, "Accessed");
    tablelist->setColumnText(7, "Modified");
    tablelist->setColumnText(8, "Changed");
    tablelist->setColumnText(9, "Hash");
    tablelist->setColumnText(10, "Category");
    tablelist->setColumnText(11, "Signature");
    tablelist->setColumnText(12, "Tagged");
    tablelist->setColumnText(13, "Hash Match");
    for(int i=0; i < pcount; i++)
    {
        itemtype = 2;
        uint64_t sectorcheck = 0;
        int cnt = i * partentrysize;
        uint64_t curstartsector = 0;
        uint64_t curendsector = 0;
        ReadForImgContent(curforimg, &curstartsector, parttablestart * 512 + cnt + 32);
        ReadForImgContent(curforimg, &curendsector, parttablestart * 512 + cnt + 40);
        if(i == 0) // INITIAL PARTITION
            sectorcheck = 0;
        else if(i > 0 && i < partentrycount)
        {
            uint64_t prestart = 0;
            uint64_t preend = 0;
            ReadForImgContent(curforimg, &prestart, parttablestart * 512 + (i-1) * partentrysize + 32);
            ReadForImgContent(curforimg, &preend, parttablestart * 512 + (i-1) * partentrysize + 40);
            sectorcheck = prestart + preend;
        }
        else if(i == pcount - 1)
            sectorcheck = curforimg->Size() / 512;
        if(curendsector - curstartsector > 0) // PARTITION VALUES MAKE SENSE
        {
            if(curstartsector > sectorcheck) // UNALLOCATED PARTITION BEFORE THE FIRST PARTITION
            {
                tablelist->setItem(ptreecnt, 0, new CheckTableItem(tablelist, NULL, NULL, ""));
                tablelist->setItemData(ptreecnt, 1, &itemtype);
                tablelist->setItemText(ptreecnt, 1, FXString::value(ptreecnt));
                tablelist->setItemText(ptreecnt, 2, "UNALLOCATED");
                tablelist->setItemData(ptreecnt, 2, curforimg);
                tablelist->setItemIcon(ptreecnt, 2, partitionicon);
                tablelist->setItemIconPosition(ptreecnt, 2, FXTableItem::BEFORE);

                tablelist->setItemText(ptreecnt, 4, FXString(ReturnFormattingSize(curstartsector*512).c_str()));
                // LOAD UNALLOCATED PARTITON sectorcheck, curstartsector, 0
                ptreecnt++;
            }
            // LOAD ALLOCATED PARTITION READ FROM TABLE curstartsector, (curendsector - curstartsector + 1), 1)
            tablelist->setItem(ptreecnt, 0, new CheckTableItem(tablelist, NULL, NULL, ""));
            tablelist->setItemData(ptreecnt, 1, &itemtype);
            tablelist->setItemText(ptreecnt, 1, FXString::value(ptreecnt));
            tablelist->setItemText(ptreecnt, 2, GetFileSystemName(curforimg, curstartsector*512));
            tablelist->setItemData(ptreecnt, 2, curforimg);
            tablelist->setItemIcon(ptreecnt, 2, partitionicon);
            tablelist->setItemIconPosition(ptreecnt, 2, FXTableItem::BEFORE);
            tablelist->setItemText(ptreecnt, 4, FXString(ReturnFormattingSize((curendsector - curstartsector + 1)*512).c_str()));
            ptreecnt++;
            if(i == pcount - 1) // ADD UNALLOCATED AFTER LAST VALID PARTITION IF EXISTS
            {
                if(curendsector < curforimg->Size() / 512)
                {
                    // LOAD UNALLOCATED curendsector+1, curforimg->Size() / 512 - 1 - curendsector, 0)
                    tablelist->setItem(ptreecnt, 0, new CheckTableItem(tablelist, NULL, NULL, ""));
                    tablelist->setItemData(ptreecnt, 1, &itemtype);
                    tablelist->setItemText(ptreecnt, 1, FXString::value(ptreecnt));
                    tablelist->setItemText(ptreecnt, 2, "UNALLOCATED");
                    tablelist->setItemData(ptreecnt, 2, curforimg);
                    tablelist->setItemIcon(ptreecnt, 2, partitionicon);
                    tablelist->setItemIconPosition(ptreecnt, 2, FXTableItem::BEFORE);
                    tablelist->setItemText(ptreecnt, 4, FXString(ReturnFormattingSize((curforimg->Size() / 512 - 1 - curendsector)*512).c_str()));
                }
            }
        }
    }
    tablelist->fitColumnsToContents(0);
    tablelist->setColumnWidth(0, tablelist->getColumnWidth(0) + 25);
    FitColumnContents(1);
    FitColumnContents(2);
    FitColumnContents(4);
    AlignColumn(tablelist, 1, FXTableItem::LEFT);
    AlignColumn(tablelist, 2, FXTableItem::LEFT);
}

FXString WombatForensics::GetFileSystemName(ForImg* curforimg, uint64_t offset)
{
    FXString partitionname = "";
    uint16_t sig16 = 0;
    uint32_t sig32 = 0;
    uint64_t sig64 = 0;
    ReadForImgContent(curforimg, &sig32, offset + 544); // BFS1
    ReadForImgContent(curforimg, &sig16, offset + 510); // FAT, NTFS, or BFS
    if(sig32 == 0x42465331) // BFS1 - BeFS
    {
        char* bfsname = new char[33];
        curforimg->ReadContent((uint8_t*)bfsname, offset + 512, 32);
        bfsname[32] = 0;
        partitionname = FXString(bfsname) + " [BFS]";
    }
    if(sig16 == 0xaa55) // FAT12, FAT16, FAT32, EXFAT, or NTFS
    {
        char* fattype = new char[6];
        curforimg->ReadContent((uint8_t*)fattype, offset + 3, 5);
        fattype[5] = 0;
        if(strcmp(fattype, "EXFAT") == 0)
        {
	    uint32_t fatsize = 0;
	    ReadForImgContent(curforimg, &fatsize, offset + 84);
	    uint8_t tmp8 = 0;
	    curforimg->ReadContent(&tmp8, offset + 108, 1);
	    uint16_t bytespersector = pow(2, (uint)tmp8);
	    curforimg->ReadContent(&tmp8, offset + 109, 1);
	    uint16_t sectorspercluster = pow(2, (uint)tmp8);
	    uint8_t fatcount = 0;
	    curforimg->ReadContent(&fatcount, offset + 110, 1);
	    uint32_t fatoff = 0;
	    ReadForImgContent(curforimg, &fatoff, offset + 80);
	    uint64_t fatoffset = fatoff + offset;
	    uint32_t rootdircluster = 0;
	    ReadForImgContent(curforimg, &rootdircluster, offset + 96);
	    uint32_t clusterstart = 0;
	    ReadForImgContent(curforimg, &clusterstart, offset + 88);
	    //std::cout << "cluster start: " << clusterstart << std::endl;
	    uint64_t rootdiroffset = (uint64_t)(offset + (((rootdircluster - 2) * sectorspercluster) + clusterstart) * bytespersector);
	    //std::cout << "root dir offset: " << rootdiroffset << std::endl;
	    FXArray<uint> clusterlist;
	    clusterlist.clear();
	    if(rootdircluster >= 2)
	    {
		clusterlist.append(rootdircluster);
		GetNextCluster(curforimg, rootdircluster, 4, fatoffset, &clusterlist);
	    }
            //std::cout << "it's exfat..." << std::endl;
	    FXString rootdirlayout = ConvertBlocksToExtents(clusterlist, sectorspercluster * bytespersector, clusterstart * bytespersector);
	    //std::cout << "rootdirlayout: " << rootdirlayout.text() << std::endl;
	    for(int i=0; i < clusterlist.no(); i++)
	    {
		//std::cout << "cluster: " << i << " | " << clusterlist.at(i) << std::endl;
		uint64_t clustersize = sectorspercluster * bytespersector;
		//std::cout << "clustersize: " << clustersize << std::endl;
		uint curoffset = 0;
		while(curoffset < clustersize)
		{
		    uint64_t clusteroffset = (((clusterlist.at(i) - 2) * sectorspercluster) + clusterstart) * bytespersector;
		    //std::cout << "clusteroffset: " << clusteroffset << std::endl;
		    uint8_t firstbyte = 0;
		    curforimg->ReadContent(&firstbyte, rootdiroffset + curoffset, 1);
		    //std::cout  << "firstbyte: " << std::hex << (uint)firstbyte << std::endl;
		    if((uint)firstbyte == 0x83)
			break;
		    curoffset += 32;
		}
		//std::cout << "curoffset: " << curoffset << std::endl;
		if(curoffset < clustersize)
		{
		    uint8_t secondbyte = 0;
		    curforimg->ReadContent(&secondbyte, rootdiroffset + curoffset + 1, 1);
		    //std::cout << "second byte: " << (uint)secondbyte << std::endl;
		    if((uint)secondbyte > 0)
		    {
			for(uint j=0; j < (uint)secondbyte; j++)
			{
			    uint16_t singleletter = 0;
			    ReadForImgContent(curforimg, &singleletter, rootdiroffset + curoffset + 2 + j*2);
			    partitionname += (char)singleletter;
			    //std::cout << "singleletter: " << (char)singleletter << std::endl;
			}
		    }
		}
	    }
	    partitionname += " [EXFAT]";
	    //std::cout << "part name: " << partitionname.text() << std::endl;
        }
        else if(FXString(fattype).find("NTFS") > -1)
        {
	    uint16_t bytespersector = 0;
	    ReadForImgContent(curforimg, &bytespersector, offset + 11);
	    uint8_t sectorspercluster = 0;
	    curforimg->ReadContent(&sectorspercluster, offset + 13, 1);
	    uint64_t mftstartingcluster = 0;
	    ReadForImgContent(curforimg, &mftstartingcluster, offset + 48);
	    //std::cout << "mft starting cluster: " << mftstartingcluster << std::endl;
	    //uint16_t bytespersector = pow(2, (uint)tmp8);
	    int8_t mftentrysize = 0;
	    curforimg->ReadContent((uint8_t*)&mftentrysize, offset + 64, 1);
            uint64_t mftentrybytesize = 0;
            if((int)mftentrysize == -10)
                mftentrybytesize = pow(2, abs((int)mftentrysize));
            else
                mftentrybytesize = (int)mftentrysize * sectorspercluster * bytespersector;
	    //uint64_t mftentrybytesize = (uint)mftentrysize * sectorspercluster * bytespersector;
	    //std::cout << "mftentrysize: " << (int)mftentrysize << std::endl;
	    //std::cout << "mftentrybytesize: " << mftentrybytesize << std::endl;
	    //mftentrybytesize = 1024;
	    uint64_t mftoffset = offset + mftstartingcluster * sectorspercluster * bytespersector;
	    //std::cout << "mft offset: " << mftoffset << std::endl;
	    char* mftsig = new char[5];
	    curforimg->ReadContent((uint8_t*)mftsig, mftoffset + 3 * mftentrybytesize, 4);
            mftsig[4] = 0;
	    //std::cout << "mftsig: |" << mftsig << "|" << std::endl;
	    //if(FXString(mftsig).find("FILE") > -1) // proper mft entry
	    if(strcmp(mftsig, "FILE") == 0) // proper mft entry
	    {
		uint16_t curoffset = 0;
		ReadForImgContent(curforimg, &curoffset, mftoffset + 3*mftentrybytesize + 20);
		uint16_t attrcnt = 0;
		ReadForImgContent(curforimg, &attrcnt, mftoffset + 3*mftentrybytesize + 40);
		//std::cout << "attrcnt:" << attrcnt << std::endl;
		for(uint i=0; i < attrcnt; i++)
		{
		    uint32_t attrtype = 0;
		    ReadForImgContent(curforimg, &attrtype, mftoffset + 3*mftentrybytesize + curoffset);
		    //std::cout << "attr type: " << std::hex << attrtype << std::endl;
		    if(attrtype == 0x60)
			break;
		    uint32_t attrlen = 0;
		    ReadForImgContent(curforimg, &attrlen, mftoffset + 3*mftentrybytesize + curoffset + 4);
		    //std::cout << "attrlen: " << attrlen << std::endl;
		    if(attrlen == 0 || attrtype == 0xffffffff)
			break;
		    curoffset += attrlen;
		    //std::cout << "curoffset: " << curoffset << std::endl;
		}
		//std::cout << "curoffset: " << curoffset;
		uint32_t attrnamelength = 0;
		ReadForImgContent(curforimg, &attrnamelength, mftoffset + 3*mftentrybytesize + curoffset + 16);
		for(uint k=0; k < attrnamelength / 2; k++)
		{
		    uint16_t singleletter = 0;
		    uint16_t attrnameoffset = 0;
		    ReadForImgContent(curforimg, &attrnameoffset, mftoffset + 3*mftentrybytesize + curoffset + 20);
		    ReadForImgContent(curforimg, &singleletter, mftoffset + 3*mftentrybytesize + curoffset + attrnameoffset + k*2);
		    partitionname += (char)singleletter;
		}
		partitionname += " [NTFS]";
		//std::cout << "partition name: " << partitionname.text() << std::endl;
	    }
        }
        else
        {
            char* pname = new char[12];
            curforimg->ReadContent((uint8_t*)fattype, offset + 54, 5);
            if(strcmp(fattype, "FAT12") == 0)
            {
                curforimg->ReadContent((uint8_t*)pname, offset + 43, 11);
                pname[11] = 0;
                partitionname = FXString(pname) + " [FAT12]";
            }
            else if(strcmp(fattype, "FAT16") == 0)
            {
                curforimg->ReadContent((uint8_t*)pname, offset + 43, 11);
                pname[11] = 0;
                partitionname = FXString(pname) + " [FAT16]";
            }
            else
            {
                curforimg->ReadContent((uint8_t*)fattype, offset + 82, 5);
                fattype[5] = 0;
                if(strcmp(fattype, "FAT32") == 0)
                {
                    curforimg->ReadContent((uint8_t*)pname, offset + 71, 11);
                    pname[11] = 0;
                    partitionname = FXString(pname) + " [FAT32]";
                }
            }
        }
    }
    return partitionname;
}

FXString WombatForensics::ConvertBlocksToExtents(FXArray<uint> blocklist, uint blocksize, uint64_t rootdiroffset)
{
    // FirstSectorOfCluster = ((N-2) * sectorspercluster) + firstdatasector [rootdirstart];
    //QString rootdirlayout = QString::number(rootdiroffset + ((rootdircluster - 2) * sectorspercluster * bytespersector)) + "," + QString::number(sectorspercluster * bytespersector) + ";";
    FXString extentstring = "";
    int blkcnt = 1;
    uint startvalue = blocklist.at(0);
    for(int i=1; i < blocklist.no(); i++)
    {
        uint oldvalue = blocklist.at(i-1);
        uint newvalue = blocklist.at(i);
        if(newvalue - oldvalue == 1)
            blkcnt++;
        else
        {
            if(rootdiroffset > 0)
	    {
                //extentstring += QString::number(rootdiroffset + ((startvalue - 2) * blocksize)) + "," + QString::number(blkcnt * blocksize) + ";";
                extentstring += FXString::value(rootdiroffset + ((startvalue - 2) * blocksize)) + "," + FXString::value(blkcnt * blocksize) + ";";
	    }
            else
	    {
                extentstring += FXString::value(startvalue * blocksize) + "," + FXString::value(blkcnt * blocksize) + ";";
	    }
            startvalue = blocklist.at(i);
            blkcnt = 1;
        }
        if(i == blocklist.no() - 1)
        {
            if(rootdiroffset > 0)
	    {
                extentstring += FXString::value(rootdiroffset + ((startvalue - 2) * blocksize)) + "," + FXString::value(blkcnt * blocksize) + ";";
	    }
            else
	    {
                extentstring += FXString::value(startvalue * blocksize) + "," + FXString::value(blkcnt * blocksize) + ";";
	    }
            startvalue = blocklist.at(i);
            blkcnt = 1;
        }
    }
    if(blocklist.no() == 1)
    {
        if(rootdiroffset > 0)
	{
            extentstring += FXString::value(rootdiroffset + ((startvalue - 2) * blocksize)) + "," + FXString::value(blkcnt * blocksize) + ";";
	}
        else
	{
            extentstring += FXString::value(startvalue * blocksize) + "," + FXString::value(blkcnt * blocksize) + ";";
	}
    }
    return extentstring;
}

void WombatForensics::GetNextCluster(ForImg* curimg, uint32_t clusternum, uint8_t fstype, uint64_t fatoffset, FXArray<uint>* clusterlist)
{
    uint32_t curcluster32 = 0;
    uint16_t curcluster16 = 0;
    int fatbyte1 = 0;
    if((uint)fstype == 1) // FAT12
    {
        fatbyte1 = clusternum + (clusternum / 2);
        if(clusternum & 0x0001) // ODD
	{
	    ReadForImgContent(curimg, &curcluster16, fatoffset + fatbyte1);
	    curcluster16 = curcluster16 >> 4;
	}
            //curcluster = (qFromLittleEndian<uint16_t>(curimg->ReadContent(fatoffset + fatbyte1, 2)) >> 4);
        else // EVEN
	{
	    ReadForImgContent(curimg, &curcluster16, fatoffset + fatbyte1);
	    curcluster16 = curcluster16 & 0x0FFF;
	}
            //curcluster = (qFromLittleEndian<uint16_t>(curimg->ReadContent(fatoffset + fatbyte1, 2)) & 0x0FFF);
        //if(curcluster != 0x0FF7)
        //    clusterlist->append(curcluster);
        //if(curcluster < 0x0FF8 && curcluster >= 2)
        if(curcluster16 < 0x0FF7 && curcluster16 >= 2)
        {
            clusterlist->append(curcluster16);
            GetNextCluster(curimg, curcluster16, fstype, fatoffset, clusterlist);
        }
    }
    else if((uint)fstype == 2) // FAT16
    {
        if(clusternum >= 2)
        {
            fatbyte1 = clusternum * 2;
	    ReadForImgContent(curimg, &curcluster16, fatoffset + fatbyte1);
	    
            //curcluster = qFromLittleEndian<uint16_t>(curimg->ReadContent(fatoffset + fatbyte1, 2));
            //if(curcluster != 0xFFF7)
            //    clusterlist->append(curcluster);
            //if(curcluster < 0xFFF8 && curcluster >= 2)
            if(curcluster16 < 0xFFF7 && curcluster16 >= 2)
            {
                clusterlist->append(curcluster16);
                GetNextCluster(curimg, curcluster16, fstype, fatoffset, clusterlist);
            }
        }
    }
    else if((uint)fstype == 3) // FAT32
    {
        if(clusternum >= 2)
        {
            fatbyte1 = clusternum * 4;
	    ReadForImgContent(curimg, &curcluster32, fatoffset + fatbyte1);
	    curcluster32 = curcluster32 & 0x0FFFFFFF;
            //curcluster = (qFromLittleEndian<uint32_t>(curimg->ReadContent(fatoffset + fatbyte1, 4)) & 0x0FFFFFFF);
	    //if(curcluster != 0x0FFFFFF7)
		//clusterlist->append(curcluster);
	    if(curcluster32 < 0x0FFFFFF7 && curcluster32 >= 2)
            {
                clusterlist->append(curcluster32);
		GetNextCluster(curimg, curcluster32, fstype, fatoffset, clusterlist);
            }
        }
    }
    else if((uint)fstype == 4) // EXFAT
    {
        if(clusternum >= 2)
        {
            fatbyte1 = clusternum * 4;
	    ReadForImgContent(curimg, &curcluster32, fatoffset + fatbyte1);
            //curcluster = qFromLittleEndian<uint32_t>(curimg->ReadContent(fatoffset + fatbyte1, 4));
	    //if(curcluster != 0xFFFFFFF7)
		//clusterlist->append(curcluster);
	    if(curcluster32 < 0xFFFFFFF7 && curcluster32 >= 2)
            {
                clusterlist->append(curcluster32);
		GetNextCluster(curimg, curcluster32, fstype, fatoffset, clusterlist);
            }
        }
    }
}
*/

/*
void DetermineFileSystem(std::string devicestring, int* fstype)
{
    // get ext2,3,4 signature
    devicebuffer.seekg(1080);
    devicebuffer.read((char*)extsig, 2); // 0x53, 0xef
    // get windows mbr signature (FAT, NTFS, BFS)
    devicebuffer.seekg(510);
    devicebuffer.read((char*)winsig, 2); // 0x55, 0xaa
    // get BFS signature
    devicebuffer.seekg(544);
    devicebuffer.read(bfssig, 4);
    std::string bfsigstr(bfssig);
    delete[] bfssig;
    // get apfs signature
    devicebuffer.seekg(32);
    devicebuffer.read(apfsig, 4);
    std::string apfsigstr(apfsig);
    delete[] apfsig;
    // get hfs signature
    devicebuffer.seekg(1024);
    devicebuffer.read(hfssig, 2);
    std::string hfssigstr(hfssig);
    delete[] hfssig;
    // get xfs signature
    devicebuffer.seekg(0);
    devicebuffer.read(xfssig, 4);
    std::string xfssigstr(xfssig);
    delete[] xfssig;
    // get btrfs signature
    devicebuffer.seekg(65600);
    devicebuffer.read(btrsig, 8);
    std::string btrsigstr(btrsig);
    delete[] btrsig;
    // get bitlocker signature
    devicebuffer.seekg(0);
    devicebuffer.read(btlsig, 8);
    std::string btlsigstr(btlsig);
    delete[] btlsig;
    // get iso signature
    devicebuffer.seekg(32769);
    devicebuffer.read(isosig, 5);
    std::string isosigstr(isosig);
    delete[] isosig;
    // get udf signature
    devicebuffer.seekg(40961);
    devicebuffer.read(udfsig, 5);
    std::string udfsigstr(udfsig);
    delete[] udfsig;
    // get refs signature
    devicebuffer.seekg(3);
    devicebuffer.read((char*)refsig, 8);
    // get f2fs signature
    devicebuffer.seekg(1024);
    devicebuffer.read((char*)f2fsig, 4);
    // get zfs signature
    devicebuffer.seekg(135168);
    devicebuffer.read((char*)zfssig, 4);
    // get bcachefs signature
    devicebuffer.seekg(4120);
    devicebuffer.read((char*)bcfsig, 16);
    // get zonefs signature
    devicebuffer.seekg(0);
    devicebuffer.read((char*)zonsig, 4);
    //std::cout << "compare:" << bfsigstr.substr(0,4).compare("1SFB") << std::endl;
    //std::cout << "extsig1 array: " << std::hex << static_cast<int>((unsigned char)extsig[1]) << std::endl;
    //std::cout << "extsig1 " << std::hex << static_cast<int>((unsigned char)extsig1) << std::endl;
    if(extsig[0] == 0x53 && extsig[1] == 0xef) // EXT2,3,4 SIGNATURE == 0
    {
        *fstype = 0;
    }
    else if(winsig[0] == 0x55 && winsig[1] == 0xaa && bfsigstr.find("1SFB") == std::string::npos) // FAT NTFS, BFS SIGNATURE
    {
        char* exfatbuf = new char[5];
        char* fatbuf = new char[5];
        char* fat32buf = new char[5];
        devicebuffer.seekg(3);
        devicebuffer.read(exfatbuf, 5);
        std::string exfatstr(exfatbuf);
        delete[] exfatbuf;
        devicebuffer.seekg(54);
        devicebuffer.read(fatbuf, 5);
        std::string fatstr(fatbuf);
        delete[] fatbuf;
        devicebuffer.seekg(82);
        devicebuffer.read(fat32buf, 5);
        std::string fat32str(fat32buf);
        delete[] fat32buf;
        if(fatstr.find("FAT12") != std::string::npos)
            *fstype = 1;
        else if(fatstr.find("FAT16") != std::string::npos)
            *fstype = 2;
        else if(fat32str.find("FAT32") != std::string::npos)
            *fstype = 3;
        else if(exfatstr.find("EXFAT") != std::string::npos)
            *fstype = 4;
        else if(exfatstr.find("NTFS") != std::string::npos)
            *fstype = 5;
    }
    else if(apfsigstr.find("NXSB") != std::string::npos) // APFS
        *fstype = 6;
    else if(hfssigstr.find("H+") != std::string::npos) // HFS+
        *fstype = 7;
    else if(hfssigstr.find("HX") != std::string::npos) // HFSX
        *fstype = 8;
    else if(xfssigstr.find("XFSB") != std::string::npos) // XFS
        *fstype = 9;
    else if(btrsigstr.find("_BHRfS_M") != std::string::npos) // BTRFS
        *fstype = 10;
    else if(btlsigstr.find("-FVE-FS-") != std::string::npos) // BTILOCKER
        *fstype = 11;
    else if(bfsigstr.find("1SFB") != std::string::npos) // BFS
        *fstype = 12;
    else if(f2fsig[0] == 0x10 && f2fsig[1] == 0x20 && f2fsig[3] == 0xf5 && f2fsig[3] == 0xf2) // F2FS
        *fstype = 13;
    else if(isosigstr.find("CD001") != std::string::npos && udfsigstr.find("BEA01") == std::string::npos) // ISO9660
        *fstype = 14;
    else if(isosigstr.find("CD001") != std::string::npos && udfsigstr.find("BEA01") != std::string::npos) // UDF
        *fstype = 15;
    else if(hfssigstr.find("BD") != std::string::npos) // Legacy HFS
        *fstype = 16;
    else if(zfssig[0] == 0x0c && zfssig[1] == 0xb1 && zfssig[2] == 0xba && zfssig[3] == 0x00) // ZFS
        *fstype = 17;
    else if(refsig[0] == 0x00 && refsig[1] == 0x00 && refsig[2] == 0x00 && refsig[3] == 0x00 && refsig[4] == 0x53 && refsig[5] == 0x46 && refsig[6] == 0x65 && refsig[7] == 0x52) // ReFS
        *fstype = 18;
    else if(f2fsig[0] == 0xe2 && f2fsig[1] == 0xe1 && f2fsig[2] == 0x5e && f2fsig[3] == 0x0f) // EROFS
        *fstype = 19;
    else if(bcfsig[0] == 0xc6 && bcfsig[1] == 0x85 && bcfsig[2] == 0x73 && bcfsig[3] == 0xf6 && bcfsig[4] == 0x4e && bcfsig[5] == 0x1a && bcfsig[6] == 0x45 && bcfsig[7] == 0xca && bcfsig[8] == 0x82 && bcfsig[9] == 0x65 && bcfsig[10] == 0xf5 && bcfsig[11] == 0x7f && bcfsig[12] == 0x48 && bcfsig[13] == 0xba && bcfsig[14] == 0x6d && bcfsig[15] == 0x81) // BCACHEFS
        *fstype = 20;
    else if(zonsig[0] == 0x5a && zonsig[1] == 0x4f && zonsig[2] == 0x46 && zonsig[3] == 0x53) // ZONEFS
        *fstype = 21;
    else // UNKNOWN FILE SYSTEM SO FAR
        *fstype = 50; 
    devicebuffer.close();
    delete[] extsig;
    delete[] winsig;
    delete[] refsig;
    delete[] f2fsig;
    delete[] zfssig;
    delete[] bcfsig;
    delete[] zonsig;
}
 */ 

/*
long WombatForensics::SetTag(FXObject* sender, FXSelector, void*)
{
    / *
    FXString tagstr = ((FXMenuCommand*)sender)->getText();
    tablelist->setItemText(tablelist->getCurrentRow(), 0, tagstr);
    FXString idkeyvalue = statusbar->getStatusLine()->getText() + "\\" + tablelist->getItemText(tablelist->getCurrentRow(), 1);
    for(int i=0; i < taggedlist.no(); i++)
    {
        if(taggedlist.at(i).contains(idkeyvalue))
            taggedlist.erase(i);
    }
    taggedlist.append(tagstr + "|" + idkeyvalue + "|" + plaintext->getText());
    * /
    return 1;
}
*/

int main(int argc, char* argv[])
{
    FXApp* wr = new FXApp("SQLite Forensics", "Wombat");

    wr->init(argc, argv);

    new WombatForensics(wr);

    wr->create();
    wr->run();

    return 0;
}
