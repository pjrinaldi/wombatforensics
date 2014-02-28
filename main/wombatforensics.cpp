#include "wombatforensics.h"

WombatForensics::WombatForensics(QWidget *parent) : QMainWindow(parent), ui(new Ui::WombatForensics)
{
    ui->setupUi(this);
    threadpool = QThreadPool::globalInstance();
    wombatvarptr = &wombatvariable;
    wombatdatabase = new WombatDatabase(wombatvarptr);
    wombatframework = new WombatFramework(wombatvarptr);
    wombatprogresswindow = new ProgressWindow(wombatdatabase);
    //isleuthkit = new SleuthKitPlugin(wombatdatabase);
    connect(ui->webView, SIGNAL(loadFinished(bool)), this, SLOT(LoadComplete(bool)));
    connect(wombatprogresswindow, SIGNAL(HideProgressWindow(bool)), this, SLOT(HideProgressWindow(bool)), Qt::DirectConnection);
    //connect(isleuthkit, SIGNAL(UpdateStatus(int, int)), this, SLOT(UpdateProgress(int, int)), Qt::QueuedConnection);
    //connect(isleuthkit, SIGNAL(UpdateMessageTable()), this, SLOT(UpdateMessageTable()), Qt::QueuedConnection);
    //connect(isleuthkit, SIGNAL(ReturnImageNode(QStandardItem*)), this, SLOT(GetImageNode(QStandardItem*)), Qt::QueuedConnection);
    wombatvarptr->caseobject.id = 0;
    wombatvarptr->omnivalue = 1; // web view is default omniviewer view to display
    //wombatvarptr->evidenceobject.id = 0;
    //wombatvarptr->jobtype = 0;
    //wombatvarptr->jobid = -1;
    //qRegisterMetaType<FileExportData*>("FileExportData*");
    //qRegisterMetaType<WombatVariable*>("WombatVariable*");
    //connect(this, SIGNAL(LogVariable(WombatVariable*)), isleuthkit, SLOT(GetLogVariable(WombatVariable*)), Qt::QueuedConnection);
    connect(wombatdatabase, SIGNAL(DisplayError(QString, QString, QString)), this, SLOT(DisplayError(QString, QString, QString)), Qt::DirectConnection);
    //connect(isleuthkit, SIGNAL(LoadFileContents(QString)), this, SLOT(LoadFileContents(QString)), Qt::QueuedConnection);
    //connect(isleuthkit, SIGNAL(PopulateProgressWindow(WombatVariable*)), this, SLOT(PopulateProgressWindow(WombatVariable*)), Qt::QueuedConnection);
    wombatprogresswindow->setModal(false);
    //emit LogVariable(wombatvarptr);
    InitializeAppStructure();
    //InitializeSleuthKit();
    InitializeDirModel();
    InitializeWombatFramework();
}

void WombatForensics::HideProgressWindow(bool checkedstate)
{
    ui->actionView_Progress->setChecked(checkedstate);
}

void WombatForensics::InitializeAppStructure()
{
    QString homePath = QDir::homePath();
    homePath += "/WombatForensics/";
    wombatvarptr->settingspath = homePath + "settings";
    wombatvarptr->datapath = homePath + "data/";
    wombatvarptr->casespath = homePath + "cases/";
    wombatvarptr->tmpfilepath = homePath + "tmpfiles";
    bool mkPath = (new QDir())->mkpath(wombatvarptr->settingspath);
    if(mkPath == false)
        DisplayError("2.0", "App Settings Folder Failed.", "App Settings Folder was not created.");
    mkPath = (new QDir())->mkpath(wombatvarptr->datapath);
    if(mkPath == false)
        DisplayError("2.1", "App Data Folder Failed.", "Application Data Folder was not created.");
    mkPath = (new QDir())->mkpath(wombatvarptr->casespath);
    if(mkPath == false)
        DisplayError("2.2", "App Cases Folder Failed.", "App Cases Folder was not created.");
    mkPath = (new QDir())->mkpath(wombatvarptr->tmpfilepath);
    if(mkPath == false)
        DisplayError("2.2", "App TmpFile Folder Failed.", "App TmpFile Folder was not created.");
    wombatvarptr->wombatdbname = wombatvarptr->datapath + "WombatApp.db";
    wombatvarptr->appdb = QSqlDatabase::addDatabase("QSQLITE", "wombatapp");
    wombatvarptr->appdb.setDatabaseName(wombatvarptr->wombatdbname);
    bool appFileExist = FileExists(wombatvarptr->wombatdbname.toStdString());
    if(!appFileExist)
    {
        wombatdatabase->CreateAppDB();
        if(wombatvarptr->curerrmsg.compare("") != 0)
            DisplayError("1.0", "App File Error", wombatvarptr->curerrmsg);
    }
    else
    {
        wombatdatabase->OpenAppDB();
        if(wombatvarptr->curerrmsg.compare("") != 0)
            DisplayError("1.1", "SQL", wombatvarptr->curerrmsg);
    }
    if(wombatdatabase->ReturnCaseCount() == 0)
    {
        ui->actionOpen_Case->setEnabled(false);
    }
    else if(wombatdatabase->ReturnCaseCount() > 0)
    {
        ui->actionOpen_Case->setEnabled(true);
    }
    else
    {
        DisplayError("1.0", "Case Count", "Invalid Case Count returned.");
    }
    QList<int> sizelist;
    sizelist.append(height()/2);
    sizelist.append(height()/2);
    ui->splitter->setSizes(sizelist);
    //SetupDirModel();
    //SetupHexPage();
    SetupToolbar();
    
}

void WombatForensics::InitializeCaseStructure()
{
    // create new case here
    bool ok;
    wombatvarptr->caseobject.name = QInputDialog::getText(this, tr("New Case Creation"), "Enter Case Name: ", QLineEdit::Normal, "", &ok);
    if(ok && !wombatvarptr->caseobject.name.isEmpty())
    {
        
        wombatdatabase->InsertCase();

        QString tmpTitle = "Wombat Forensics - ";
        tmpTitle += wombatvarptr->caseobject.name;
        this->setWindowTitle(tmpTitle); // update application window.
        // make Cases Folder
        QString casestring = QString::number(wombatvarptr->caseobject.id) + "-" + wombatvarptr->caseobject.name;
        wombatvarptr->caseobject.dirpath = wombatvarptr->casespath + casestring + "/";
        bool mkPath = (new QDir())->mkpath(wombatvarptr->caseobject.dirpath);
        if(mkPath == false)
        {
            DisplayError("2.0", "Cases Folder Creation Failed.", "New Case folder was not created.");
        }
        // CREATE CASEID-CASENAME.DB RIGHT HERE.
        wombatvarptr->caseobject.dbname = wombatvarptr->caseobject.dirpath + casestring + ".db";
        wombatvarptr->casedb = QSqlDatabase::addDatabase("QSQLITE"); // may not need this
        wombatvarptr->casedb.setDatabaseName(wombatvarptr->caseobject.dbname);
        if(!FileExists(wombatvarptr->caseobject.dbname.toStdString()))
        {
            wombatdatabase->CreateCaseDB();
            if(wombatvarptr->curerrmsg.compare("") != 0)
                DisplayError("1.2", "Course DB Creation Error", wombatvarptr->curerrmsg);
        }
        else
        {
            wombatdatabase->OpenCaseDB();
            if(wombatvarptr->curerrmsg.compare("") != 0)
                DisplayError("1.3", "SQL", wombatvarptr->curerrmsg);
        }
        //wombatvarptr->evidenceobject.dirpath = wombatvarptr->caseobject.dirpath + "evidence/";
        //mkPath = (new QDir())->mkpath(wombatvarptr->evidenceobject.dirpath);
        //if(mkPath == false)
        //{
        //    DisplayError("2.0", "Case Evidence Folder Creation Failed", "Failed to create case evidence folder.");
        //}
        if(wombatdatabase->ReturnCaseCount() > 0)
        {
            ui->actionOpen_Case->setEnabled(true);
        }
    }
}

void WombatForensics::InitializeOpenCase()
{
    // open case here
    wombatvarptr->casenamelist;
    wombatvarptr->casenamelist.clear();
    // populate case list here
    wombatdatabase->ReturnCaseNameList();
    bool ok;
    wombatvarptr->caseobject.name = QInputDialog::getItem(this, tr("Open Existing Case"), tr("Select the Case to Open: "), wombatvarptr->casenamelist, 0, false, &ok);
    if(ok && !wombatvarptr->caseobject.name.isEmpty()) // open selected case
    {
        wombatdatabase->ReturnCaseID();
        QString tmpTitle = "Wombat Forensics - ";
        tmpTitle += wombatvarptr->caseobject.name;
        this->setWindowTitle(tmpTitle);
        QString casestring = QString::number(wombatvarptr->caseobject.id) + "-" + wombatvarptr->caseobject.name;
        wombatvarptr->caseobject.dirpath = wombatvarptr->casespath + casestring + "/";
        bool mkPath = (new QDir())->mkpath(wombatvarptr->caseobject.dirpath);
        if(mkPath == false)
        {
            DisplayError("2.0", "Cases Folder Check Failed.", "Existing Case folder did not exist.");
        }
        // CREATE CASEID-CASENAME.DB RIGHT HERE.
        wombatvarptr->caseobject.dbname = wombatvarptr->caseobject.dirpath + casestring + ".db";
        wombatvarptr->casedb = QSqlDatabase::addDatabase("QSQLITE"); // may not need this
        wombatvarptr->casedb.setDatabaseName(wombatvarptr->caseobject.dbname);
        bool caseFileExist = FileExists(wombatvarptr->caseobject.dbname.toStdString());
        if(!caseFileExist)
        {
            wombatdatabase->CreateCaseDB();
            if(wombatvarptr->curerrmsg.compare("") != 0)
                DisplayError("1.2", "Course DB Creation Error", wombatvarptr->curerrmsg);
        }
        else
        {
            wombatdatabase->OpenCaseDB();
            if(wombatvarptr->curerrmsg.compare("") != 0)
                DisplayError("1.3", "SQL", wombatvarptr->curerrmsg);
        }
        wombatdatabase->GetEvidenceObjects();
        /*wombatvarptr->evidenceobject.dirpath = wombatvarptr->caseobject.dirpath += "evidence/";
        mkPath = (new QDir())->mkpath(wombatvarptr->evidenceobject.dirpath);
        if(mkPath == false)
        {
            DisplayError("2.0", "Case Evidence Folder Check Failed", "Case Evidence folder did not exist.");
        }
            //ThreadRunner* trun = new ThreadRunner(isleuthkit, "populatecase", wombatvarptr);
            //threadpool->start(trun);
        */
            // NEED TO INITIALIZEEVIDENCEIMAGES() HERE
    }

}
void WombatForensics::InitializeWombatFramework()
{
    // MIGHT NOT NEED TO INITIALIZE ANYTHING HERE.
}
/*
void WombatForensics::InitializeSleuthKit()
{*/
    /*
    ThreadRunner* initrunner = new ThreadRunner(isleuthkit, "initialize", wombatvarptr);
    threadpool->start(initrunner);
    threadpool->waitForDone();
    */
/*}*/

void WombatForensics::InitializeDirModel()
{
    wombatvarptr->dirmodel = new QStandardItemModel();
    QStringList headerList;
    headerList << "Unique ID" << "Name" << "Full Path" << "Size (Bytes)" << "Signature" << "Extension" << "Created (UTC)" << "Accessed (UTC)" << "Modified (UTC)" << "Status Changed (UTC)" << "MD5 Hash";
    wombatvarptr->dirmodel->setHorizontalHeaderLabels(headerList);
    ui->dirTreeView->setModel(wombatvarptr->dirmodel);
    ResizeColumns();
    connect(ui->dirTreeView, SIGNAL(clicked(QModelIndex)), this, SLOT(dirTreeView_selectionChanged(QModelIndex)));
    connect(ui->dirTreeView, SIGNAL(expanded(const QModelIndex &)), this, SLOT(ResizeViewColumns(const QModelIndex &)));

}

void WombatForensics::InitializeEvidenceStructure()
{
    wombatframework->OpenEvidenceImage();
    wombatdatabase->InsertEvidenceObject(); // add evidence to data and image parts to dataruns
    wombatdatabase->GetEvidenceObject(); // get evidence object from the db.
    wombatframework->AddEvidenceNode(); // add evidence node to directory model
        // AFTER I ADD THE NODE, I CAN REFERENCE IT WITH THE BELOW CODE BASED ON IT'S ID USING THE ZERO 0, SINCE ITS UNIQUE.
        /*
        QList<QStandardItem*> idstring = wombatvarptr->dirmodel->findItems(QString::number(wombatvarptr->evidenceobject.id), Qt::MatchExactly, 0);
        QList<QStandardItem*> namestring = wombatvarptr->dirmodel->findItems(wombatvarptr->evidenceobject.name, Qt::MatchExactly, 1);
        fprintf(stderr, "dirmodel item 0: %s\n", idstring[0]->text().toStdString().c_str());
        fprintf(stderr, "dirmodel item 1: %s\n", namestring[0]->text().toStdString().c_str());
        */
    ResizeColumns();
    wombatframework->OpenVolumeSystem();
    wombatframework->GetVolumeSystemName();
    wombatdatabase->InsertVolumeObject(); // add volume to data
    wombatdatabase->GetVolumeObject();
        // NEED TO COMBINE THE FILE SYSTEM AND PARTITION OBJECT INFORMATION INTO A DB ENTRY.
        /* NO NEED TO PLACE A VOLUME OBJECT IN THE NODE TREE STRUCTURE...
         * SIMPLY ADD THE PARTITION(S)/FILE SYSTEM(S) TO THE IMAGE NODE
        tmplist.clear(); // clear tmplist to reuse for volumes.
        */
    wombatframework->OpenPartitions();
    wombatdatabase->InsertPartitionObjects();
    wombatdatabase->GetPartitionObjects();
    wombatdatabase->InsertFileSystemObjects();
    wombatdatabase->GetFileSystemObjects();
    wombatframework->AddPartitionNodes();
    /*for(int i=0; i < wombatvarptr->partitionobjectvector.count(); i++)
    {
        // PARTITION INFORMATION FROM TSK INFO   : FLAGS, LEN (# OF SECTORS), START, (FIRST SECTOR), DESC, SLOT_NUM, TABLE_NUM 
        fprintf(stderr, "Part Name: %s\n", wombatvarptr->partitionobjectvector[i].name.toStdString().c_str());
    }*/
    ResizeColumns();
        //wombatdatabase->InitializeEvidenceDatabase();
        //fprintf(stderr, "Image Type: %d\n", wombatvarptr->evidenceobject.imageinfo->itype);
    //wombatframework->BuildEvidenceModel();
    // NEED TO ADD THE EVIDENCE ITEM TO THE DATABASE
    // POPULATE THE WOMBATVARPTR FOR THE EVIDENCEOBJECT VECTOR
    // NEED TO CREATE THE EVIDENCE TSK DATABASE (EXTRACT EVIDENCE ACCORDING TO MODULES)
    // NEED TO BUILD DIRMODEL AS I GO AND POPULATE DIRTREEVIEW AS I GO WITH EACH FILE
    // FOR NOW I WON'T BUILD MODULES, I'LL JUST DESIGN A MULTI-THREADED APPROACH FOR IT AND ABSTRACT TO PLUGGABLE MODULES LATER
}

void WombatForensics::AddEvidence()
{
    wombatvarptr->evidenceobject.Clear(); // clear values of current evidence object to add a new one.
    QStringList tmplist = QFileDialog::getOpenFileNames(this, tr("Select Evidence Image(s)"), tr("./"));
    if(tmplist.count())
    {
        wombatvarptr->evidenceobject.name = tmplist[0].split("/").last();
        for(int i=0; i < tmplist.count(); i++)
        {
            fprintf(stderr, "fullpathvector: %s\n", tmplist[i].toStdString().c_str());
            wombatvarptr->evidenceobject.fullpathvector.push_back(tmplist[i].toStdString());
        }
        wombatvarptr->evidenceobject.itemcount = tmplist.count();
        wombatvarptr->evidenceobjectvector.append(wombatvarptr->evidenceobject); // add evidence to case evidence list
        fprintf(stderr, "eov count: %i\n", wombatvarptr->evidenceobjectvector.count());
        wombatprogresswindow->show();
        wombatprogresswindow->ClearTableWidget();
        QFuture<void> future1 = QtConcurrent::run(this, &WombatForensics::InitializeEvidenceStructure);
        //InitializeEvidenceStructure();
    }
    
    /*
    QString evidenceFilePath = QFileDialog::getOpenFileName(this, tr("Select Evidence Item"), tr("./"));
    if(evidenceFilePath != "")
    {
        wombatprogresswindow->show();
        wombatprogresswindow->ClearTableWidget();
        wombatvarptr->jobtype = 1; // add evidence
        // DETERMINE IF THE EVIDENCE NAME EXISTS, IF IT DOES THEN PROMPT USER THAT ITS OPEN ALREADY. IF THEY WANT TO OPEN A SECOND COPY
        // THEN SET NEWEVIDENCENAME EVIDENCEFILEPATH.SPLIT("/").LAST() + "COPY.DB"
        QString evidenceName = evidenceFilePath.split("/").last();
        evidenceName += ".db";
        wombatvarptr->evidenceobject.id = wombatdatabase->InsertEvidence(evidenceName, evidenceFilePath, wombatvarptr->caseobject.id);
        // could set curimgobjid = InsertObject(caseobject.id, evidenceobject.id, 3, evidenceobject.id); // set objectid here...
        // start to build the basic tree here adding the image node...
        wombatvarptr->evidenceobject.idlist.append(wombatvarptr->evidenceobject.id);
        wombatvarptr->evidencepath = evidenceFilePath;
        wombatvarptr->evidencepathlist << wombatvarptr->evidencepath;
        wombatvarptr->evidenceobject.dbname = evidenceName;
        wombatvarptr->evidencedbnamelist << wombatvarptr->evidenceobject.dbname;
        wombatvarptr->jobid = wombatdatabase->InsertJob(wombatvarptr->jobtype, wombatvarptr->caseobject.id, wombatvarptr->evidenceobject.id);
        emit LogVariable(wombatvarptr);
        QString tmpString = evidenceName;
        tmpString += " - ";
        tmpString += QString::fromStdString(GetTime());
        QStringList tmpList;
        tmpList << tmpString << QString::number(wombatvarptr->jobid);
        wombatprogresswindow->UpdateAnalysisTree(0, new QTreeWidgetItem(tmpList));
        wombatprogresswindow->UpdateFilesFound("0");
        wombatprogresswindow->UpdateFilesProcessed("0");
        wombatprogresswindow->UpdateAnalysisState("Adding Evidence to Database");
        //LOGINFO("Adding Evidence Started");
        wombatdatabase->InsertMsg(wombatvarptr->caseobject.id, wombatvarptr->evidenceobject.id, wombatvarptr->jobid, 2, "Adding Evidence Started");
        //ThreadRunner* trun = new ThreadRunner(isleuthkit, "openevidence", wombatvarptr);
        //threadpool->start(trun);
        */
    //}
}

void WombatForensics::UpdateViewer()
{
    wombatvarptr->visibleviewer = ReturnVisibleViewerID();
    if(wombatvarptr->visibleviewer == 0) // hex
        LoadHexContents();
    else if(wombatvarptr->visibleviewer == 1) // txt
        LoadTxtContents();
    else if(wombatvarptr->visibleviewer == 2) // web
        LoadWebContents();
    else if(wombatvarptr->visibleviewer == 3) // pic
        LoadImgContents();
    else if(wombatvarptr->visibleviewer == 4) // vid
        LoadVidContents();
}

void WombatForensics::LoadHexContents()
{
}

void WombatForensics::LoadTxtContents()
{
}

void WombatForensics::LoadWebContents()
{
    if(wombatvarptr->selectedobject.type == 1)
    {
        ui->webView->setUrl(QUrl("qrc:///html/infohtml"));
    }
}

void WombatForensics::LoadImgContents()
{
}

void WombatForensics::LoadVidContents()
{
}

void WombatForensics::LoadComplete(bool isok)
{
    // IT WORKS NOW. SO I HAVE TO WAIT TILL IT LOADS. WILL NEED TO WORK ON THREADING WHEN I OPEN A LARGER FILE
    // NEED TO GET THIS INFORMATION FROM THE DB AND NOT THE IMAGEINFO VARIABLE WHICH MIGHT NOT EXIST AT THE MOMENT. OR I NEED TO POPULATE THE VARIABLE
    // ON OPEN SO I CAN USE THE VARIABLES.
    wombatvarptr->htmlcontent = "";
    if(isok)
    {
        if(wombatvarptr->selectedobject.type == 1) // image file
        {
            wombatvarptr->htmlcontent += "<div id='infotitle'>image information</div><br/>";
            wombatvarptr->htmlcontent += "<table><tr><td class='property'>imagetype:</td><td class='pvalue'>";
            wombatvarptr->htmlcontent += QString(tsk_img_type_todesc(wombatvarptr->evidenceobject.imageinfo->itype)) + "</td></tr>";
            wombatvarptr->htmlcontent += "<tr><td class='property'>size:</td><td class='pvalue'>";
            wombatvarptr->htmlcontent += QLocale::system().toString((int)wombatvarptr->evidenceobject.imageinfo->size) + " bytes</td></tr>";
            wombatvarptr->htmlcontent += "<tr><td class='property'>sector size:</td><td class='pvalue'>";
            wombatvarptr->htmlcontent += QLocale::system().toString(wombatvarptr->evidenceobject.imageinfo->sector_size) + " bytes</td></tr>";
            wombatvarptr->htmlcontent += "<tr><td class='property'>sector count:</td><td class='pvalue'>";
            wombatvarptr->htmlcontent += QLocale::system().toString((int)((float)wombatvarptr->evidenceobject.imageinfo->size/(float)wombatvarptr->evidenceobject.imageinfo->sector_size));
            // might not want to do the volume type one if there's no volume. have to think on it.
            wombatvarptr->htmlcontent += " sectors</td></tr><tr><td class='property'>volume type</td><td class='pvalue'>";
            wombatvarptr->htmlcontent += wombatvarptr->volumeobject.name + "</td></tr>";
            wombatframework->GetBootCode(); // determine boot type in this function and populate html string information into wombatvarptr value
            //wombatvarptr->htmlcontent += "</table>";
            //QWebElement tmpelement = ui->webView->page()->currentFrame()->documentElement().lastChild();
            //tmpelement.appendInside("");
            QWebElement tmpelement = ui->webView->page()->currentFrame()->documentElement().lastChild();
            tmpelement.appendInside(wombatvarptr->htmlcontent);
            // check for partition table and populate the values accordingly.
            // the fs stuff i find at jump and oem and the others are for the filesystem/partition boot sector. this isn't valid when there is an mbr.
            // need to determine if there is an mbr and then pull the partition table information from it. otherwise simply display the image info
            // and have no mbr present in first sector.
            // when you click on the partition, this is where the partition boot sector information will go.
            //tmpelement.appendInside("<br/><br/><div class='tabletitle'>boot sector</div>");
            //tmpelement.appendInside("<br/><table><tr><th>byte offset</th><th>value</th><th>description</th></tr><tr class='odd'><td>0-2</td><td class='bvalue'>" + wombatvarptr->bootsectorlist[0] + "</td><td class='desc'>Jump instruction to the boot code</td></tr><tr class='even'><td>3-10</td><td class='bvalue'>" + wombatvarptr->bootsectorlist[1] + "</td><td class='desc'>OEM name string field. This field is ignored by Microsoft operating systems</td></tr><tr class='odd'><td>11-12</td><td class='bvalue'>" + wombatvarptr->bootsectorlist[2] + " bytes</td><td class='desc'>Bytes per sector</td></tr><tr class='even'><td>13-13</td><td class='bvalue'>" + wombatvarptr->bootsectorlist[3] + " sectors</td><td class='desc'>Seectors per cluster</td></tr><tr class='odd'><td colspan='3' class='bot'></td></tr></table>");
        }
    }
}
/*
void WombatForensics::GetDosBootCode()
{
    int retval;
    wombatvarptr->bootbuffer = NULL;
    wombatvarptr->bootbuffer = new char[wombatvarptr->evidenceobject.imageinfo->sector_size];
    retval = tsk_img_read(wombatvarptr->evidenceobject.imageinfo, 0, wombatvarptr->bootbuffer, wombatvarptr->evidenceobject.imageinfo->sector_size);
    if(retval > 0)
    {
        wombatvarptr->bootbytearray = QByteArray::fromRawData(wombatvarptr->bootbuffer, wombatvarptr->evidenceobject.imageinfo->sector_size);
        fprintf(stderr, "oem from byte array: %s\n", QString::fromUtf8(wombatvarptr->bootbytearray.mid(3,8)).toStdString().c_str());
        wombatvarptr->bootsectorlist << ByteArrayToHexDisplay(wombatvarptr->bootbytearray.mid(0,3));
        wombatvarptr->bootsectorlist << QString::fromUtf8(wombatvarptr->bootbytearray.mid(3,8));
        wombatvarptr->bootsectorlist << ByteArrayToShortDisplay(wombatvarptr->bootbytearray.mid(11,2));
        wombatvarptr->bootsectorlist << ByteArrayToShortDisplay(wombatvarptr->bootbytearray.mid(13,1));
    }
    else
        fprintf(stderr, "filling bootbuffer failed\n");

 *
    uint64_t bytelen = 512*(seclength - 1);
    uint64_t bytestart = 512*secstart;
    char* volbuffer = NULL;
    volbuffer = new char[bytelen+512];

    {
        // need to figure out why this fails... and returns -1
        retval = TskServices::Instance().getImageFile().getSectorData(secstart, seclength-1, volbuffer);
        fprintf(stderr, "sector data return value: %i\n", retval);
        if (retval == -1)
        {
        }
    }
    if(retval > 0)
    {
        QFile tmpfile("/home/pasquale/WombatForensics/tmpfiles/volbyte.dat");
        tmpfile.open(QIODevice::WriteOnly);
        tmpfile.write(volbuffer, bytelen);
        tmpfile.close();
    }
    delete[] volbuffer;
    //delete volbuffer;

    return "/home/pasquale/WombatForensics/tmpfiles/volbyte.dat";

 *
 *
 *    // set hex (which i'll probably remove anyway since it's highlighted in same window)
    int sellength = txt.size()/2;
    QString tmptext = "Length: " + QString::number(sellength);
    selectedhex->setText(tmptext);
    // get initial bytes value and then update ascii
    std::vector<uchar> bytes;
    Translate::HexToByte(bytes, txt);
    QString ascii;
    Translate::ByteToChar(ascii, bytes);
    tmptext = "Ascii: " + ascii;
    selectedascii->setText(tmptext);
    QString strvalue;
    uchar * ucharPtr;
    // update the int entry:
    // pad right with 0x00
    int intvalue = 0;
    ucharPtr = (uchar*) &intvalue;
    memcpy(&intvalue,&bytes.begin()[0], min(sizeof(int),bytes.size()));
    strvalue.setNum(intvalue);
    tmptext = "Int: " + strvalue;
    selectedinteger->setText(tmptext);
    // update float entry;
    float fvalue;
    ucharPtr = (uchar*)(&fvalue);
    if(bytes.size() < sizeof(float) )
    {
        for(unsigned int i= 0; i < sizeof(float); ++i)
        {
            if( i < bytes.size() )
            {
                *ucharPtr++ = bytes[i];
            }
            else
            {
                *ucharPtr++ = 0x00;
            }
        }
    }
    else
    {
        memcpy(&fvalue,&bytes.begin()[0],sizeof(float));
    }
    strvalue.setNum( fvalue );
    tmptext = "Float: " + strvalue;
    selectedfloat->setText(tmptext);
    // update double
    double dvalue;
    ucharPtr = (uchar*)&dvalue;
    if(bytes.size() < sizeof(double) )
    {
        for(unsigned int i= 0; i < sizeof(double); ++i)
        {
            if( i < bytes.size() )
            {
                *ucharPtr++ = bytes[i];
            }
            else
            {
                *ucharPtr++ = 0x00;
            }
        }
    }
    else
    {
        memcpy(&dvalue,&bytes.begin()[0],sizeof(double));
    }
    strvalue.setNum( dvalue );
    tmptext = "Double: " + strvalue;
    selecteddouble->setText(tmptext);

}
*/

void WombatForensics::RemEvidence()
{
    wombatprogresswindow->ClearTableWidget();
    wombatvarptr->jobtype = 2; // remove evidence
    QStringList evidenceList;
    evidenceList.clear();
    // populate case list here
    //evidenceList = wombatdatabase->ReturnCaseActiveEvidence(wombatvarptr->caseobject.id);
    bool ok;
    QString item = QInputDialog::getItem(this, tr("Remove Existing Evidence"), tr("Select Evidence to Remove: "), evidenceList, 0, false, &ok);
    if(ok && !item.isEmpty()) // open selected case
    {
        //wombatvarptr->evidenceobject.id = wombatdatabase->ReturnEvidenceID(item);
        //wombatvarptr->jobid = wombatdatabase->InsertJob(wombatvarptr->jobtype, wombatvarptr->caseobject.id, wombatvarptr->evidenceobject.id);
        emit LogVariable(wombatvarptr);
        QString tmpstring = item.split("/").last() + " - " + QString::fromStdString(GetTime());
        QStringList tmplist;
        tmplist << tmpstring << QString::number(wombatvarptr->jobid);
        wombatprogresswindow->UpdateAnalysisTree(2, new QTreeWidgetItem(tmplist));
        wombatprogresswindow->UpdateFilesFound("");
        wombatprogresswindow->UpdateFilesProcessed("");
        wombatprogresswindow->UpdateAnalysisState("Removing Evidence");
        //LOGINFO("Removing Evidence Started");
        //wombatdatabase->InsertMsg(wombatvarptr->caseobject.id, wombatvarptr->evidenceobject.id, wombatvarptr->jobid, 2, "Removing Evidence Started");
        UpdateMessageTable();
        //wombatdatabase->RemoveEvidence(item);
        wombatprogresswindow->UpdateProgressBar(25);
        //QString tmppath = wombatvarptr->evidenceobject.dirpath + item.split("/").last() + ".db";
        //if(QFile::remove(tmppath))
        //{
        //}
        //else
            //emit DisplayError("2.1", "Evidence DB File was NOT Removed", "");
        wombatprogresswindow->UpdateProgressBar(50);
        UpdateCaseData();
        wombatprogresswindow->UpdateProgressBar(75);
        //LOGINFO("Removing Evidence Finished");
        //wombatdatabase->InsertMsg(wombatvarptr->caseobject.id, wombatvarptr->evidenceobject.id, wombatvarptr->jobid, 2, "Removing Evidence Finished");
        //wombatdatabase->UpdateJobEnd(wombatvarptr->jobid, 0, 0);
        UpdateMessageTable();
        wombatprogresswindow->UpdateAnalysisState("Removing Evidence Finished");
        wombatprogresswindow->UpdateProgressBar(100);
        UpdateMessageTable();
    }
}

int WombatForensics::StandardItemCheckState(QStandardItem* tmpitem, int checkcount)
{
    int curcount = checkcount;
    QModelIndex curindex = tmpitem->index();
    if(tmpitem->hasChildren())
    {
        for(int i=0; i < tmpitem->rowCount(); i++)
        {
            curcount = StandardItemCheckState(tmpitem->child(i,0), curcount);
        }
    }
    if(curindex.sibling(curindex.row(),1).flags().testFlag(Qt::ItemIsUserCheckable))
    {
        if(tmpitem->parent()->child(curindex.row(), 1)->checkState())
            curcount++;
    }
    
    return curcount;
}

QVector<FileExportData> WombatForensics::SetFileExportProperties(QStandardItem* tmpitem, FileExportData* tmpexport, QVector<FileExportData> tmpexportlist)
{
    QModelIndex curindex = tmpitem->index();
    if(tmpitem->hasChildren())
    {
        for(int i=0; i < tmpitem->rowCount(); i++)
        {
            tmpexportlist = SetFileExportProperties(tmpitem->child(i,0), tmpexport, tmpexportlist);
        }
    }
    if(curindex.sibling(curindex.row(), 1).flags().testFlag(Qt::ItemIsUserCheckable))
    {
        if(tmpitem->parent()->child(curindex.row(), 1)->checkState())
        {
            
            // update evidence/file info for each item bit.
            tmpexport->id = curindex.sibling(curindex.row(), 1).data().toString().toInt(); // unique object id
            //wombatvarptr->evidenceobject.id = wombatdatabase->ReturnObjectEvidenceID(tmpexport->id); // evidence id
            //QStringList currentevidencelist = wombatdatabase->ReturnEvidenceData(wombatvarptr->evidenceobject.id); // evidence data
            //tmpexport->evidenceobject.fullpath = currentevidencelist[0]; // evidence path
            //tmpexport->evidenceobject.dbname = currentevidencelist[1]; // evidence db name
            tmpexport->evidenceobject.fullpath = "";
            tmpexport->evidenceobject.dbname = "";
            tmpexport->name = curindex.sibling(curindex.row(), 0).data().toString().toStdString(); // file name
            if(tmpexport->pathstatus == FileExportData::include)
            {
                // export path with original path
                tmpexport->fullpath = tmpexport->exportpath + "/" + curindex.sibling(curindex.row(), 2).data().toString().toStdString();
            }
            else if(tmpexport->pathstatus == FileExportData::exclude)
            {
                tmpexport->fullpath = tmpexport->exportpath + "/" + tmpexport->name; // export path without original path
            }
            //fprintf(stderr, "export full path checked: %s\n", tmpexport.fullpath.c_str());
    
            tmpexportlist.push_back(*tmpexport);
        }
    }

    return tmpexportlist;
}
QVector<FileExportData> WombatForensics::SetListExportProperties(QStandardItem* tmpitem, FileExportData* tmpexport, QVector<FileExportData> tmpexportlist)
{
    QModelIndex curindex = tmpitem->index();
    if(tmpitem->hasChildren())
    {
        for(int i = 0; i < tmpitem->rowCount(); i++)
        {
            tmpexportlist = SetListExportProperties(tmpitem->child(i, 0), tmpexport, tmpexportlist);
        }
    }
    // get every item.
    tmpexport->id = curindex.sibling(curindex.row(), 1).data().toString().toInt(); // unique object id
    //wombatvarptr->evidenceobject.id = wombatdatabase->ReturnObjectEvidenceID(tmpexport->id); // evidence id
    //QStringList currentevidencelist = wombatdatabase->ReturnEvidenceData(wombatvarptr->evidenceobject.id); // evidence data
    tmpexport->evidenceobject.fullpath = ""; //currentevidencelist[0]; // evidence path
    tmpexport->evidenceobject.dbname = ""; //currentevidencelist[1]; // evidence db name
    tmpexport->name = curindex.sibling(curindex.row(), 0).data().toString().toStdString(); // file name
    if(tmpexport->pathstatus == FileExportData::include)
    {
        // export path with original path
        tmpexport->fullpath = tmpexport->exportpath + "/" + curindex.sibling(curindex.row(), 2).data().toString().toStdString();
    }
    else if(tmpexport->pathstatus == FileExportData::exclude)
    {
        tmpexport->fullpath = tmpexport->exportpath + "/" + tmpexport->name; // export path without original path
    }
    //fprintf(stderr, "export full path listed: %s\n", tmpexport.fullpath.c_str());
    tmpexportlist.push_back(*tmpexport);

    return tmpexportlist;
}
int WombatForensics::StandardItemListCount(QStandardItem* tmpitem, int listcount)
{
    int curcount = listcount;
    if(tmpitem->hasChildren())
    {
        for(int i=0; i < tmpitem->rowCount(); i++)
        {
            curcount = StandardItemListCount(tmpitem->child(i,0), curcount);
        }
    }
    curcount++;

    return curcount;
}

void WombatForensics::ExportEvidence()
{
    int checkcount = 0;
    int listcount = 0;

    QStandardItem* rootitem = wombatdirmodel->invisibleRootItem();
    for(int i = 0; i < rootitem->rowCount(); i++)
    {
        QStandardItem* imagenode = rootitem->child(i,0);
        for(int j = 0; j < imagenode->rowCount(); j++)
        {
            QStandardItem* volumenode = imagenode->child(j,0);
            for(int k = 0; k < volumenode->rowCount(); k++)
            {
                QStandardItem* fsnode = volumenode->child(k,0);
                for(int m = 0; m < fsnode->rowCount(); m++)
                {
                    QStandardItem* filenode = fsnode->child(m,0);
                    checkcount = StandardItemCheckState(filenode, checkcount);
                    listcount = StandardItemListCount(filenode, listcount);
                }
            }
        }
    }
    //fprintf(stderr, "checked count: %i\n", checkcount);
    //fprintf(stderr, "listed item count: %i\n", listcount);
    exportdialog = new ExportDialog(this, checkcount, listcount);
    connect(exportdialog, SIGNAL(FileExport(FileExportData*)), this, SLOT(FileExport(FileExportData*)), Qt::DirectConnection);
    exportdialog->show();
}

void WombatForensics::FileExport(FileExportData* exportdata)
{
    QVector<FileExportData> exportevidencelist;
    if(exportdata->filestatus == FileExportData::selected)
    {
        exportdata->exportcount = 1;
        exportdata->id = curselindex.sibling(curselindex.row(), 1).data().toString().toInt(); // unique objectid
        //wombatvarptr->evidenceobject.id = wombatdatabase->ReturnObjectEvidenceID(exportdata->id); // evidence id
        //QStringList currentevidencelist = wombatdatabase->ReturnEvidenceData(wombatvarptr->evidenceobject.id); // evidence data
        exportdata->evidenceobject.fullpath = "";//currentevidencelist[0]; // evidence path
        exportdata->evidenceobject.dbname = "";//currentevidencelist[1]; // evidence db name
        exportdata->name = curselindex.sibling(curselindex.row(),0).data().toString().toStdString(); // file name
        if(exportdata->pathstatus == FileExportData::include)
        {
            exportdata->fullpath = exportdata->exportpath;
            exportdata->fullpath += "/";
            exportdata->fullpath += curselindex.sibling(curselindex.row(), 2).data().toString().toStdString(); // export path with original path
        }
        else if(exportdata->pathstatus == FileExportData::exclude)
        {
            exportdata->fullpath = exportdata->exportpath + "/" + exportdata->name; // export path without original path
        }
        //fprintf(stderr, "export selected full path: %s\n", exportdata.fullpath.c_str());

        exportevidencelist.push_back(*exportdata);
    }
    else if(exportdata->filestatus == FileExportData::checked)
    {
        QStandardItem* rootitem = wombatdirmodel->invisibleRootItem();
        for(int i = 0; i < rootitem->rowCount(); i++) // loop over images
        {
            QStandardItem* imagenode = rootitem->child(i,0);
            for(int j = 0; j < imagenode->rowCount(); j++) // loop over volume(s)
            {
                QStandardItem* volumenode = imagenode->child(j,0); // loop over partition(s)
                for(int k = 0; k < volumenode->rowCount(); k++)
                {
                    QStandardItem* fsnode = volumenode->child(k,0); // file system node
                    for(int m = 0; m < fsnode->rowCount(); m++)
                    {
                        QStandardItem* filenode = fsnode->child(m,0); // file system root node
                        exportdata->exportcount = StandardItemCheckState(filenode, exportdata->exportcount);
                        exportevidencelist = SetFileExportProperties(filenode, exportdata, exportevidencelist);
                    }
                }
            }
        }
    }
    else if(exportdata->filestatus == FileExportData::listed)
    {
        QStandardItem* rootitem = wombatdirmodel->invisibleRootItem();
        for(int i = 0; i < rootitem->rowCount(); i++) // loop over images
        {
            QStandardItem* imagenode = rootitem->child(i,0);
            for(int j = 0; j < imagenode->rowCount(); j++) // loop over volumes
            {
                QStandardItem* volumenode = imagenode->child(j,0); // loop over partitions
                for(int k = 0; k < volumenode->rowCount(); k++)
                {
                    QStandardItem* fsnode = volumenode->child(k,0); // file system node
                    for(int m = 0; m < fsnode->rowCount(); m++)
                    {
                        QStandardItem* filenode = fsnode->child(m,0); // file system root node
                        exportdata->exportcount = StandardItemListCount(filenode, exportdata->exportcount);
                        exportevidencelist = SetListExportProperties(filenode, exportdata, exportevidencelist);
                    }
                }
            }
        }
    }
    wombatvarptr->exportdatavector = exportevidencelist;
    wombatprogresswindow->ClearTableWidget();
    wombatvarptr->jobtype = 3; // export files
    //wombatvarptr->jobid = wombatdatabase->InsertJob(wombatvarptr->jobtype, wombatvarptr->caseobject.id, wombatvarptr->evidenceobject.id);
    emit LogVariable(wombatvarptr);
    QString tmpString = "File Export - " + QString::fromStdString(GetTime());
    QStringList tmpList;
    tmpList << tmpString << QString::number(wombatvarptr->jobid);
    wombatprogresswindow->UpdateAnalysisTree(1, new QTreeWidgetItem(tmpList));
    wombatprogresswindow->UpdateFilesFound(QString::number(wombatvarptr->exportdata.exportcount));
    wombatprogresswindow->UpdateFilesProcessed("0");
    wombatprogresswindow->UpdateAnalysisState("Exporting Files");
    //LOGINFO("File Export Started");
    //wombatdatabase->InsertMsg(wombatvarptr->caseobject.id, wombatvarptr->evidenceobject.id, wombatvarptr->jobid, 2, "File Export Started");
    //ThreadRunner* trun = new ThreadRunner(isleuthkit, "exportfiles", wombatvarptr);
    //threadpool->start(trun);
}

void WombatForensics::UpdateCaseData()
{
    // refresh views here
    wombatdirmodel->clear();
    QStringList headerList;
    headerList << "Name" << "Unique ID" << "Full Path" << "Size (Bytes)" << "Created (UTC)" << "Accessed (UTC)" << "Modified (UTC)" << "Status Changed (UTC)" << "MD5 Hash";
    wombatdirmodel->setHorizontalHeaderLabels(headerList);
    ui->dirTreeView->setModel(wombatdirmodel);
    //ThreadRunner* trun = new ThreadRunner(isleuthkit, "refreshtreeviews", wombatvarptr);
    //threadpool->start(trun);
}

void WombatForensics::UpdateProgress(int filecount, int processcount)
{
    int curprogress = (int)((((float)processcount)/(float)filecount)*100);
    wombatprogresswindow->UpdateFilesFound(QString::number(filecount));
    wombatprogresswindow->UpdateFilesProcessed(QString::number(processcount));
    wombatprogresswindow->UpdateProgressBar(curprogress);
}

void WombatForensics::UpdateMessageTable()
{
    wombatprogresswindow->ClearTableWidget();
    //QStringList tmplist = wombatdatabase->ReturnMessageTableEntries(wombatvarptr->jobid);
    //wombatprogresswindow->UpdateMessageTable(tmplist);
}

void WombatForensics::PopulateProgressWindow(WombatVariable* wvariable)
{
    int treebranch = 0;
    QString tmpstring;
    //QStringList joblist = wombatdatabase->ReturnJobDetails(wvariable->jobid);
    if(wvariable->jobtype == 1 || wvariable->jobtype == 2)
        tmpstring = wvariable->evidenceobject.dbname + " - " + "";//joblist[0];
    else if(wvariable->jobtype == 3)
        tmpstring = "File Export - ";// + "";//joblist[0];
    else
        tmpstring = wvariable->evidenceobject.dbname + " - " + "";//joblist[0];
    QStringList tmplist;
    tmplist << tmpstring << QString::number(wvariable->jobid);
    if(wvariable->jobtype == 1) treebranch = 0;
    else if(wvariable->jobtype == 2) treebranch = 2;
    else treebranch = 1;
    wombatprogresswindow->UpdateAnalysisTree(treebranch,  new QTreeWidgetItem(tmplist));
    if(wvariable->jobtype == 2)
    {
        wombatprogresswindow->UpdateProgressBar(100);
        wombatprogresswindow->UpdateFilesFound("");
        wombatprogresswindow->UpdateFilesProcessed("");
    }
    else
    {
        //wombatprogresswindow->UpdateFilesFound(joblist[1]);
        //wombatprogresswindow->UpdateFilesProcessed(joblist[2]);
        //int finalprogress = (int)((((float)joblist[2].toInt())/(float)joblist[1].toInt())*100);
        //wombatprogresswindow->UpdateProgressBar(finalprogress);
    }
    //wombatprogresswindow->UpdateAnalysisState(joblist[3]);
    UpdateMessageTable();
}


void WombatForensics::DisplayError(QString errorNumber, QString errorType, QString errorValue)
{
    QString tmpString = errorNumber;
    tmpString += ". SqlError: ";
    tmpString += errorType;
    tmpString += " Returned ";
    tmpString += errorValue;
    QMessageBox::warning(this, "Error", tmpString, QMessageBox::Ok);
}

void WombatForensics::GetImageNode(QStandardItem* imagenode)
{
    /*
    setUpdatesEnabled(false);
    QStandardItem* currentroot = wombatdirmodel->invisibleRootItem();
    currentroot->appendRow(imagenode);
    ui->dirTreeView->setModel(wombatdirmodel);
    //ResizeColumns(wombatdirmodel);
    UpdateMessageTable();
    setUpdatesEnabled(true);
    */
}

//void WombatForensics::ResizeColumns(QStandardItemModel* currentmodel)
void WombatForensics::ResizeColumns(void)
{
    for(int i=0; i < wombatvarptr->dirmodel->columnCount(); i++)
    {
        // may need to compare treeview->model() == currentmodel) to determine what to set it to.
        // depending on the design though, i may not need multiple layouts since the columns can be sorted.
        // have to see as i go. for now its good.
        ui->dirTreeView->resizeColumnToContents(i);
    }
}

void WombatForensics::SetupDirModel(void)
{
    /*
    wombatdirmodel = new QStandardItemModel();
    QStringList headerList;
    headerList << "Name" << "Unique ID" << "Full Path" << "Size (Bytes)" << "Signature" << "Extension" << "Created (UTC)" << "Accessed (UTC)" << "Modified (UTC)" << "Status Changed (UTC)" << "MD5 Hash";
    wombatdirmodel->setHorizontalHeaderLabels(headerList);
    QStandardItem *evidenceNode = wombatdirmodel->invisibleRootItem();
    ui->dirTreeView->setModel(wombatdirmodel);
    ResizeColumns(wombatdirmodel);
    connect(ui->dirTreeView, SIGNAL(clicked(QModelIndex)), this, SLOT(dirTreeView_selectionChanged(QModelIndex)));
    connect(ui->dirTreeView, SIGNAL(expanded(const QModelIndex &)), this, SLOT(ResizeViewColumns(const QModelIndex &)));
    */
}

void WombatForensics::SetupHexPage(void)
{
    // hex editor page
    QBoxLayout* mainlayout = new QBoxLayout(QBoxLayout::TopToBottom, ui->hexPage);
    QHBoxLayout* hexLayout = new QHBoxLayout();
    hstatus = new QStatusBar(ui->hexPage);
    hstatus->setSizeGripEnabled(false);
    selectedoffset = new QLabel("Offset: 00");
    selectedhex = new QLabel("Length: 0");
    selectedascii = new QLabel("Ascii: ");
    selectedinteger = new QLabel("Integer: ");
    selectedfloat = new QLabel("Float: ");
    selecteddouble = new QLabel("Double: ");
    hstatus->addWidget(selectedoffset);
    hstatus->addWidget(selectedhex);
    hstatus->addWidget(selectedascii);
    hstatus->addWidget(selectedinteger);
    hstatus->addWidget(selectedfloat);
    hstatus->addWidget(selecteddouble);
    hexwidget = new HexEditor(ui->hexPage);
    hexwidget->setStyleSheet(QStringLiteral("background-color: rgb(255, 255, 255);"));
    hexwidget->setObjectName("bt-hexview");
    hexwidget->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
    hexLayout->addWidget(hexwidget);
    hexvsb = new QScrollBar(hexwidget);
    hexLayout->addWidget(hexvsb);
    hexvsb->setRange(0, 0);
    mainlayout->addLayout(hexLayout);
    mainlayout->addWidget(hstatus);

    connect(hexwidget, SIGNAL(rangeChanged(off_t,off_t)), this, SLOT(setScrollBarRange(off_t,off_t)));
    connect(hexwidget, SIGNAL(topLeftChanged(off_t)), this, SLOT(setScrollBarValue(off_t)));
    connect(hexwidget, SIGNAL(offsetChanged(off_t)), this, SLOT(setOffsetLabel(off_t)));
    connect(hexvsb, SIGNAL(valueChanged(int)), hexwidget, SLOT(setTopLeftToPercent(int)));
    connect(hexwidget, SIGNAL(selectionChanged(const QString &)), this, SLOT(UpdateSelectValue(const QString&)));
}

void WombatForensics::SetupToolbar(void)
{
    // setup actiongroups and initial settings here.
    viewgroup = new QActionGroup(this);
    viewgroup->addAction(ui->actionViewHex);
    viewgroup->addAction(ui->actionViewTxt);
    viewgroup->addAction(ui->actionViewOmni);
    ui->actionViewHex->setChecked(true);
    connect(viewgroup, SIGNAL(triggered(QAction*)), this, SLOT(ViewGroupTriggered(QAction*)));
}

int WombatForensics::ReturnVisibleViewerID(void)
{
    return ui->viewerstack->currentIndex();
}

WombatForensics::~WombatForensics()
{
    delete ui;
}

void WombatForensics::closeEvent(QCloseEvent* event)
{
    wombatprogresswindow->close();
    RemoveTmpFiles();
    //const char* errmsg = wombatdatabase->CloseCaseDB();
}

void WombatForensics::RemoveTmpFiles()
{
    QString homePath = QDir::homePath();
    homePath += "/WombatForensics/";
    homePath += "tmpfiles/";
    QDir tmpdir(homePath);
    if(!tmpdir.removeRecursively())
    {
        DisplayError("2.7", "Tmp File Removal", "All tmp files may not have been removed. Please manually remove them to avoid evidence contamination.");
    }
}

void WombatForensics::on_actionNew_Case_triggered()
{
    int ret = QMessageBox::Yes;
    // determine if a case is open
    if(wombatvarptr->caseobject.id > 0)
    {
        ret = QMessageBox::question(this, tr("Close Current Case"), tr("There is a case already open. Are you sure you want to close it?"), QMessageBox::Yes | QMessageBox::No);
    }
    if (ret == QMessageBox::Yes)
    {
        InitializeCaseStructure();
    }
}

void WombatForensics::on_actionOpen_Case_triggered()
{
    int ret = QMessageBox::Yes;
    // determine if a case is open
    if(wombatvarptr->caseobject.id > 0)
    {
        ret = QMessageBox::question(this, tr("Close Current Case"), tr("There is a case already open. Are you sure you want to close it?"), QMessageBox::Yes | QMessageBox::No);
    }
    if (ret == QMessageBox::Yes)
    {
        InitializeOpenCase();
    }
}

void WombatForensics::ViewGroupTriggered(QAction* selaction)
{
    wombatvarptr->visibleviewer = ReturnVisibleViewerID();
    if(wombatvarptr->visibleviewer > 1) // if an omniviewer is visible, set the current omnivalue.
        wombatvarptr->omnivalue = wombatvarptr->visibleviewer - 1; // may not need this since i can simply -1 it.

    // READ RESPECTIVE SIGNATURE HERE TO SET OMNIVALUE
    // GET CURRENTLY SELECTED FILE AND IF IT'S A VALUE, THEN LOAD THE RESPECTIVE CONTENT.
    // wombatvarptr->omnivalue = 1;
    if(selaction == ui->actionViewHex)
    {
        ui->viewerstack->setCurrentIndex(0); // hex
        // show the correct viewer page from stacked widget
    }
    else if(selaction == ui->actionViewTxt)
    {
        ui->viewerstack->setCurrentIndex(1); // txt
        //fprintf(stderr, "Text Button Text: %s\n", selaction->text().toStdString().c_str());
    }
    else if(selaction == ui->actionViewOmni) // omni 3,4,5
    {
        ui->viewerstack->setCurrentIndex(wombatvarptr->omnivalue + 1);
    }
    UpdateViewer();
}

void WombatForensics::on_actionView_Progress_triggered(bool checked)
{
    if(!checked)
        wombatprogresswindow->hide();
    else
        wombatprogresswindow->show();
}

void WombatForensics::UpdateOmniValue()
{
    if(wombatvarptr->selectedobject.type > 0 && wombatvarptr->selectedobject.type < 4)
    {
        wombatvarptr->omnivalue = 1;
    }
    else // if image - omnivalue = 2, video - omnivalue = 3.
    {
    }
}

void WombatForensics::dirTreeView_selectionChanged(const QModelIndex &index)
{
    wombatvarptr->selectedobject.id = index.sibling(index.row(), 0).data().toInt(); // object id
    wombatdatabase->GetObjectType(); // now i have selected object type.
    UpdateOmniValue();
    UpdateViewer();
    // NEED TO DETERMINE THE DATA TYPE TO CALL THE CORRECT DATA TO UPDATE.
    //
    //QString sigtext = "";
    // I CAN EITHER GET THE SIGNATURE USING COMPARISON OR BY CALLING FILE AND MAGIC LIKE THE MODULE
    // SELECTING ITEM GETS IT'S ID VALUES AND SET'S RESPECTIVE WOMBATVARPTR->VALUES
    // GET THE RESPECTIVE VISIBLE VIEWER FROM THE VIEWER STACK.
    // LOAD THE DATA INTO THE RESPECTIVE VIEWER.

    /*
    wombatvarptr->visibleviewer = ReturnVisibleViewerID();
    // NEED TO DETERMINE WHICH VIEWER IS VISIBLE AND THEN LOAD THE RESPECITIVE DATA ACCORDINGLY.
    // QString imagename = wombatvarptr->evidencepath.split("/").last();
    QString tmptext = "";
    QString sigtext = "";
    tmptext = index.sibling(index.row(), 1).data().toString();
    if(tmptext != "")
    {
        wombatvarptr->evidenceobject.id = wombatdatabase->ReturnObjectEvidenceID(tmptext.toInt());
        QStringList currentevidencelist = wombatdatabase->ReturnEvidenceData(wombatvarptr->evidenceobject.id);
        wombatvarptr->evidencepath = currentevidencelist[0];
        wombatvarptr->evidenceobject.dbname = currentevidencelist[1];
        wombatvarptr->fileid = wombatdatabase->ReturnObjectFileID(tmptext.toInt());
        sigtext = index.sibling(index.row(), 4).data().toString(); // signature value which i need to compare to the xml of known values
        wombatvarptr->omnivalue = DetermineOmniView(sigtext);
        if(wombatvarptr->omnivalue == 0)
        {
            //ui->fileViewTabWidget->setTabEnabled(2, false); // where i disable the omni button 
        }
        else
        {
            ui->viewerstack->setCurrentIndex(wombatvarptr->omnivalue + 1);
            //ui->fileViewTabWidget->setTabEnabled(2, true); // where i enable the omni button*/
            /*if(omnivalue == 1)
                ui->viewerstack->setCurrentIndex(2);
            else if(omnivalue == 2)
                ui->viewerstack->setCurrentIndex(3);
            else if(omnivalue == 3)
                ui->viewerstack->setCurrentIndex(4);
                */
/*        }
    }
    else
    {
        tmptext = index.sibling(index.row(), 0).data().toString();
        QStringList evidenceobject.idlist = wombatdatabase->ReturnCaseActiveEvidenceID(wombatvarptr->caseobject.id);
        QStringList volumedesclist = isleuthkit->GetVolumeContents(wombatvarptr);
        for(int i=0; i < evidenceobject.idlist.count() / 3; i++)
        {
            if(tmptext.compare(evidenceobject.idlist[3*i+1].split("/").last()) == 0)
            {
                wombatvarptr->evidenceobject.id = evidenceobject.idlist[3*i].toInt();
                wombatvarptr->evidencepath = evidenceobject.idlist[3*i+1];
                wombatvarptr->evidenceobject.dbname = evidenceobject.idlist[3*i+2];
            }
        }
        // need to do other tmptext.compare's to see whether it's volume or fs...
        if(tmptext.compare(wombatvarptr->evidencepath.split("/").last()) == 0)
        {
            wombatvarptr->fileid = -1;
        }
        else // try one parent and see if it is a volume...
        {
            QString parenttext = index.parent().sibling(index.row(), 0).data().toString();
            for(int i=0; i < evidenceobject.idlist.count() / 3; i++)
            {
                if(parenttext.compare(evidenceobject.idlist[3*i+1].split("/").last()) == 0) // volume
                {
                    wombatvarptr->evidenceobject.id = evidenceobject.idlist[3*i].toInt();
                    wombatvarptr->evidencepath = evidenceobject.idlist[3*i+1];
                    wombatvarptr->evidenceobject.dbname = evidenceobject.idlist[3*i+2];
                }
            }
            fprintf(stderr, "evipath: %s\n", wombatvarptr->evidencepath.toStdString().c_str());
            bool isvolume = false;
            for(int i=0; i < volumedesclist.count() / 2; i++)
            {
                if(tmptext.compare(volumedesclist[i]) == 0)
                {
                    isvolume = true;
                    wombatvarptr->volid = volumedesclist[2*i+1].toInt();
                }
            }
            if(isvolume == true)
            {
                wombatvarptr->fileid = -2;
            }
            else // try a file system fileid = -3
            {
            }
        }
    }
    ThreadRunner* tmprun = new ThreadRunner(isleuthkit, "showfile", wombatvarptr);
    threadpool->start(tmprun);
    */
}

int WombatForensics::DetermineOmniView(QString currentSignature)
{
    int retvalue = 0;
    QString tmppath = wombatvarptr->settingspath;
    tmppath += "/tsk-magicview.xml";
    QFile magicfile(tmppath);
    if(magicfile.exists()) // if the xml exists, read it.
    {
        if(magicfile.open(QFile::ReadOnly | QFile::Text))
        {
            QXmlStreamReader reader(&magicfile);
            while(!reader.atEnd())
            {
                reader.readNext();
                if(reader.isStartElement() && reader.name() == "signature")
                {
                    if(currentSignature.toLower().compare(reader.readElementText().toLower()) == 0)
                    {
                        retvalue = reader.attributes().value("viewer").toString().toInt();
                    }
                }
            }
            if(reader.hasError())
                fprintf(stderr, "Reader Error: %s\n", reader.errorString().toStdString().c_str());
            magicfile.close();
        }
        else
            fprintf(stderr, "Couldn't Read the omni file\n");
    }
    else
        fprintf(stderr, "Couldn't find the omni file\n");
    return retvalue;
}

void WombatForensics::UpdateSelectValue(const QString &txt)
{
    // set hex (which i'll probably remove anyway since it's highlighted in same window)
    int sellength = txt.size()/2;
    QString tmptext = "Length: " + QString::number(sellength);
    selectedhex->setText(tmptext);
    // get initial bytes value and then update ascii
    std::vector<uchar> bytes;
    Translate::HexToByte(bytes, txt);
    QString ascii;
    Translate::ByteToChar(ascii, bytes);
    tmptext = "Ascii: " + ascii;
    selectedascii->setText(tmptext);
    QString strvalue;
    uchar * ucharPtr;
    // update the int entry:
    // pad right with 0x00
    int intvalue = 0;
    ucharPtr = (uchar*) &intvalue;
    memcpy(&intvalue,&bytes.begin()[0], min(sizeof(int),bytes.size()));
    strvalue.setNum(intvalue);
    tmptext = "Int: " + strvalue;
    selectedinteger->setText(tmptext);
    // update float entry;
    float fvalue;
    ucharPtr = (uchar*)(&fvalue);
    if(bytes.size() < sizeof(float) )
    {
        for(unsigned int i= 0; i < sizeof(float); ++i)
        {
            if( i < bytes.size() )
            {
                *ucharPtr++ = bytes[i];
            }
            else
            {
                *ucharPtr++ = 0x00;
            }
        }
    }
    else
    {
        memcpy(&fvalue,&bytes.begin()[0],sizeof(float));
    }
    strvalue.setNum( fvalue );
    tmptext = "Float: " + strvalue;
    selectedfloat->setText(tmptext);
    // update double
    double dvalue;
    ucharPtr = (uchar*)&dvalue;
    if(bytes.size() < sizeof(double) )
    {
        for(unsigned int i= 0; i < sizeof(double); ++i)
        {
            if( i < bytes.size() )
            {
                *ucharPtr++ = bytes[i];
            }
            else
            {
                *ucharPtr++ = 0x00;
            }
        }
    }
    else
    {
        memcpy(&dvalue,&bytes.begin()[0],sizeof(double));
    }
    strvalue.setNum( dvalue );
    tmptext = "Double: " + strvalue;
    selecteddouble->setText(tmptext);
}

void WombatForensics::LoadFileContents(QString filepath)
{
    if(filepath != "")
    {
        QFileInfo pathinfo(filepath);
        if(!pathinfo.isDir())
        {
            LoadHexModel(filepath);
            LoadTxtContent(filepath);
            LoadOmniContent(filepath); // possibly add a view type here: 1 - web, 2 - pic, 3 - vid
        }
        else
        {
            //txtwidget->setPlainText("");
        }
    }
    else
    {
        //txtwidget->setPlainText("");
        // load nothing here...
    }
}

void WombatForensics::LoadHexModel(QString tmpFilePath)
{
    hexwidget->open(tmpFilePath);
    hexwidget->set2BPC();
    hexwidget->setBaseHex();
}
void WombatForensics::LoadTxtContent(QString asciiText)
{
    /*
    QFile tmpFile(asciiText);
    tmpFile.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream stream(&tmpFile);
    txtwidget->setPlainText(stream.readAll());
    tmpFile.close();
    */
}

void WombatForensics::LoadOmniContent(QString filePath)
{
}

void WombatForensics::setOffsetLabel(off_t pos)
{
  QString label;
  label = "Offset: ";
  char    buffer[64];
#if _LARGEFILE_SOURCE
  sprintf(buffer,"0x%lx",pos);
#else
  sprintf(buffer,"0x%x",pos);
#endif
  label += buffer;
  selectedoffset->setText(label);
}

void WombatForensics::setScrollBarRange(off_t low, off_t high)
{
   (void)low;(void)high;
   // range must be contained in the space of an integer, just do 100
   // increments
   hexvsb->setRange(0,100);
}

void WombatForensics::setScrollBarValue(off_t pos)
{
  // pos is the topLeft pos, set the scrollbar to the
  // location of the last byte on the page
  // Note: offsetToPercent now rounds up, so we don't
  // have to worry about if this is the topLeft or bottom right
  hexvsb->setValue(hexwidget->offsetToPercent(pos));
}
