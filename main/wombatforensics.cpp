#include "wombatforensics.h"

WombatForensics::WombatForensics(QWidget *parent) : QMainWindow(parent), ui(new Ui::WombatForensics)
{
    ui->setupUi(this);
    threadpool = QThreadPool::globalInstance();
    wombatvarptr = &wombatvariable;
    this->menuBar()->hide();
    this->statusBar()->setSizeGripEnabled(true);
    mainprogress = new QProgressBar(this);
    mainprogress->setMaximumHeight(15);
    mainprogress->setStyleSheet("font-size: 10px");
    int curprogress = (int)(((float)filesprocessed/(float)filesfound)*100);
    mainprogress->setFormat("Processed " + QString::number(filesprocessed) + " of " + QString::number(filesfound) + " " + QString::number(curprogress) + "%");
    this->statusBar()->setMaximumHeight(15);
    this->statusBar()->addPermanentWidget(mainprogress, 0);
    this->statusBar()->removeWidget(mainprogress);
    filecountlabel = new QLabel(this);
    filecountlabel->setStyleSheet("font-size: 10px");
    filecountlabel->setText("Files: 0");
    this->statusBar()->addWidget(filecountlabel, 0);
    //this->statusBar()->removeWidget(filecountlabel);
    tskobjptr = &tskobject;
    tskobjptr->readimginfo = NULL;
    wombatdatabase = new WombatDatabase(wombatvarptr);
    wombatframework = new WombatFramework(wombatvarptr);
    wombatprogresswindow = new ProgressWindow(wombatdatabase);
    isignals = new InterfaceSignals();
    connect(ui->webView, SIGNAL(loadFinished(bool)), this, SLOT(LoadComplete(bool)));
    connect(wombatprogresswindow, SIGNAL(HideProgressWindow(bool)), this, SLOT(HideProgressWindow(bool)), Qt::DirectConnection);
    connect(isignals, SIGNAL(ProgressUpdate(int, int)), this, SLOT(UpdateProgress(int, int)), Qt::QueuedConnection);
    wombatvarptr->caseobject.id = 0;
    wombatvarptr->omnivalue = 1; // web view is default omniviewer view to display
    connect(wombatdatabase, SIGNAL(DisplayError(QString, QString, QString)), this, SLOT(DisplayError(QString, QString, QString)), Qt::DirectConnection);
    wombatprogresswindow->setModal(false);
    InitializeAppStructure();
    connect(&sqlwatcher, SIGNAL(finished()), this, SLOT(InitializeQueryModel()), Qt::QueuedConnection);
    //connect(&openwatcher, SIGNAL(finished()), this, SLOT(InitializeQueryModel()), Qt::QueuedConnection);
    connect(&filewatcher, SIGNAL(finished()), this, SLOT(InitializeQueryModel()), Qt::QueuedConnection);
    //InitializeDirModel();
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
    SetupHexPage();
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
        wombatvarptr->casedb = QSqlDatabase::addDatabase("QSQLITE", "casedb"); // may not need this
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
        fcasedb = wombatvarptr->casedb;
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
        wombatvarptr->casedb = QSqlDatabase::addDatabase("QSQLITE", "casedb"); // may not need this
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
        fcasedb = wombatvarptr->casedb;
        wombatdatabase->GetEvidenceObjects();
        // NEED TO INITIALIZEEVIDENCEIMAGES() HERE
    }

}
void WombatForensics::InitializeWombatFramework()
{
    // MIGHT NOT NEED TO INITIALIZE ANYTHING HERE.
}

void WombatForensics::InitializeDirModel()
{
    // OLD MODEL, NOT USED NOW. REPLACED BY SQLTABLEMODEL AND SORTFILTERPROXYMODEL
    /*
    wombatvarptr->dirmodel = new QStandardItemModel();
    QStringList headerList;
    headerList << "Unique ID" << "Name" << "Full Path" << "Size (Bytes)" << "Signature" << "Extension" << "Created (UTC)" << "Accessed (UTC)" << "Modified (UTC)" << "Status Changed (UTC)" << "MD5 Hash";
    wombatvarptr->dirmodel->setHorizontalHeaderLabels(headerList);
    ui->dirTreeView->setModel(wombatvarptr->dirmodel);
    ResizeColumns();
    connect(ui->dirTreeView->selectionModel(), SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)), this, SLOT(SelectionChanged(const QItemSelection &, const QItemSelection &)));
    connect(ui->dirTreeView->selectionModel(), SIGNAL(currentChanged(const QModelIndex &, const QModelIndex &)), this, SLOT(CurrentChanged(const QModelIndex &, const QModelIndex &)));
    connect(ui->dirTreeView, SIGNAL(clicked(QModelIndex)), this, SLOT(dirTreeView_selectionChanged(QModelIndex)));
    connect(ui->dirTreeView, SIGNAL(expanded(const QModelIndex &)), this, SLOT(ResizeViewColumns(const QModelIndex &)));
*/
}

// CURRENTLY FUNCTION IS NOT USED.
void WombatForensics::UpdateTree()
{
    /*
    fcasedb.commit();
    FileViewSqlModel* tmpmodel = new FileViewSqlModel();
    tmpmodel->setQuery("SELECT objectid, objecttype, name FROM data", fcasedb);
    tmpmodel->setHeaderData(0, Qt::Horizontal, tr("ID"));
    tmpmodel->setHeaderData(1, Qt::Horizontal, tr("Type"));
    tmpmodel->setHeaderData(2, Qt::Horizontal, tr("Name"));

    ui->dirTreeView->setModel(tmpmodel);
    */
}

void WombatForensics::InitializeQueryModel()
{
    fcasedb.commit();
    qDebug() << "temp db commit finished";
    if(ProcessingComplete())
    {
        qDebug() << "All threads have finished.";
        fcasedb.commit();
        qDebug() << "DB Commit finished.";
        wombatdatabase->GetEvidenceObjects(); // get's all evidenceobjects from the db for the given case
        FileViewSqlModel* tmpmodel = new FileViewSqlModel();
        tmpmodel->setQuery("SELECT objectid, name, fullpath, size, objecttype, address, crtime, atime, mtime, ctime, md5 FROM data", fcasedb);
        tmpmodel->setHeaderData(0, Qt::Horizontal, tr("ID"));
        tmpmodel->setHeaderData(1, Qt::Horizontal, tr("Name"));
        tmpmodel->setHeaderData(2, Qt::Horizontal, tr("Full Path"));
        tmpmodel->setHeaderData(3, Qt::Horizontal, tr("Size (bytes)"));
        tmpmodel->setHeaderData(4, Qt::Horizontal, tr("Signature"));
        tmpmodel->setHeaderData(5, Qt::Horizontal, tr("Extension"));
        tmpmodel->setHeaderData(6, Qt::Horizontal, tr("Created (UTC)"));
        tmpmodel->setHeaderData(7, Qt::Horizontal, tr("Accessed (UTC)"));
        tmpmodel->setHeaderData(8, Qt::Horizontal, tr("Modified (UTC)"));
        tmpmodel->setHeaderData(9, Qt::Horizontal, tr("Status Changed (UTC)"));
        tmpmodel->setHeaderData(10, Qt::Horizontal, tr("MD5 Hash"));

        //ui->dirTreeView->setModel(tmpmodel);

        //treeproxy = new TreeProxy();
        checkableproxy = new CheckableProxyModel(this);
        //treeproxy->setSourceModel(tmpmodel);
        //checkableproxy->setSourceModel(treeproxy);
        //ui->dirTreeView->setModel(treeproxy);
        checkableproxy->setSourceModel(tmpmodel);
        ui->dirTreeView->setModel(checkableproxy);
        connect(ui->dirTreeView, SIGNAL(clicked(QModelIndex)), this, SLOT(dirTreeView_selectionChanged(QModelIndex)));
        //connect(ui->dirTreeView->selectionModel(), SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)), this, SLOT(SelectionChanged(const QItemSelection &, const QItemSelection &)));
        //connect(ui->dirTreeView->selectionModel(), SIGNAL(currentChanged(const QModelIndex &, const QModelIndex &)), this, SLOT(CurrentChanged(const QModelIndex &, const QModelIndex &)));
        ResizeColumns();
        wombatframework->CloseInfoStructures();
    }
}

void WombatForensics::SelectionChanged(const QItemSelection &curitem, const QItemSelection &previtem)
{
    qDebug() << "selection stuff can happen now.";
}

void WombatForensics::CurrentChanged(const QModelIndex &curindex, const QModelIndex &previndex)
{
    qDebug() << "current index changed.";
}

void WombatForensics::InitializeEvidenceStructure()
{
    wombatframework->OpenEvidenceImage();
    wombatdatabase->InsertEvidenceObject(); // add evidence to data and image parts to dataruns
    wombatframework->OpenVolumeSystem();
    wombatframework->GetVolumeSystemName();
    wombatdatabase->InsertVolumeObject(); // add volume to data
    wombatframework->OpenPartitions();
    wombatdatabase->InsertPartitionObjects();
    wombatdatabase->InsertFileSystemObjects();
    //  openfuture = QtConcurrent::run(wombatframework, &WombatFramework::OpenFiles);
    //  openwatcher.setFuture(openfuture);
    wombatframework->OpenFiles();
    // OPEN FILES INCLUDES WALKING FILE TREE->ADDING TO DB->GETTING DB INFO->ADDING TO NODE TREE.
    // MIGHT BE ABLE TO GET RID OF THE BELOW FUNCTIONS IF I CAN GET MY QUERYMODEL WORKING...
    //wombatdatabase->GetEvidenceObjects(); // get's all evidenceobjects from the db for the given case
    //wombatdatabase->GetVolumeObjects();
    //wombatdatabase->GetPartitionObjects();
    //wombatdatabase->GetFileSystemObjects();

    //wombatframework->AddEvidenceNodes(); // add evidence node to directory model
}

void WombatForensics::AddEvidence()
{
    wombatvarptr->evidenceobject.Clear(); // clear values of current evidence object to add a new one.
    //qDebug() << "fullpathvector count after clear: " << wombatvarptr->evidenceobject.fullpathvector.size();
    // might need to call these to a global tmp and then store it after initializeevidencestructure...
    // NEED TO CHECK WHAT GETEVIDENCEOBJECTS() RETURNS FOR A TEST IMAGE OPEN...
    QStringList tmplist = QFileDialog::getOpenFileNames(this, tr("Select Evidence Image(s)"), tr("./"));
    //qDebug() << "tmplist count: " << tmplist.count();
    if(tmplist.count())
    {
        wombatvarptr->currentevidencename = tmplist[0].split("/").last();
        //qDebug() << "tmplist count 2: " << tmplist.count();
        for(int i=0; i < tmplist.count(); i++)
        {
            fprintf(stderr, "fullpathvector[%i]: %s\n", i, tmplist[i].toStdString().c_str());
            wombatvarptr->evidenceobject.fullpathvector.push_back(tmplist[i].toStdString());
        }
        wombatvarptr->evidenceobject.itemcount = tmplist.count();
        //qDebug() << " ptr itemcount: " << wombatvarptr->evidenceobject.itemcount;
        // REPLACE WITH PROGRESSBAR/STATUSBAR
        this->statusBar()->addPermanentWidget(mainprogress, 0);
        this->mainprogress->show();
        int curprogress = (int)(((float)filesprocessed/(float)filesfound)*100);
        mainprogress->setValue(curprogress);
        mainprogress->setFormat("Processed " + QString::number(filesprocessed) + " of " + QString::number(filesfound) + " " + QString::number(curprogress) + "%");
        //wombatprogresswindow->show();
        //wombatprogresswindow->ClearTableWidget(); // hiding these 2 for now since i'm not ready to populate progress yet and it gets in the way.
        // THIS SHOULD HANDLE WHEN THE THREADS ARE ALL DONE.

        sqlfuture = QtConcurrent::run(this, &WombatForensics::InitializeEvidenceStructure);
        sqlwatcher.setFuture(sqlfuture);

        // SHOULD GO HERE, BUT IT NEEDS TO LAUNCH WHEN THE THREADS ARE DONE...
        // FOR NOW I'LL PUT IT IN THE OFFSHOOT THREAD.
        // if i call these in concurrent::runs-> then they shouldn't run until the end...?
        //ResizeColumns();
        // the below must be called when all threads are done and i don't need the variable anymore.
        //wombatframework->CloseInfoStructures();
    }
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
    if(tskobjptr->readimginfo != NULL)
        tsk_img_close(tskobjptr->readimginfo);
    // int curidx = wombatframework->DetermineVectorIndex(); // shouldn't need this, since i'm pulling from sql.

    if(wombatvarptr->selectedobject.type == 1) // image file
    {
        OpenParentImage(wombatvarptr->selectedobject.id);
        // OpenParentImage(wombatvarptr->evidenceobjectvector[curidx].id); 
        tskobjptr->offset = 0;
        tskobjptr->length = wombatvarptr->selectedobject.size;
        //tskobjptr->length = wombatvarptr->evidenceobjectvector[curidx].size;
    }
    else if(wombatvarptr->selectedobject.type == 3) // partition object
    {
        OpenParentImage(wombatvarptr->selectedobject.parimgid);
        //OpenParentImage(wombatvarptr->partitionobjectvector[curidx].parimgid);
        //tskobjptr->offset = wombatvarptr->partitionobjectvector[curidx].sectstart * wombatvarptr->partitionobjectvector[curidx].blocksize;
        tskobjptr->offset = wombatvarptr->selectedobject.sectstart * wombatvarptr->selectedobject.blocksize;
        //tskobjptr->length = wombatvarptr->partitionobjectvector[curidx].sectlength * wombatvarptr->partitionobjectvector[curidx].blocksize;
        tskobjptr->length = wombatvarptr->selectedobject.sectlength * wombatvarptr->selectedobject.blocksize;
        // need to get the volume parent and then the image parent , so i can open the image from bytes...
        // need to do a self looping query, where i use the parentid and check if its null, if its not null, then run it again.
        // else return imageid and then loop over evidenceobjectvector for index. then loop over fullpathvector to get the
        // imagepartspath and open the respective image.
        // THEN I'LL HAVE TO GET THE OFFSET AND LENGTH FOR THE PARTITION...
        //qDebug() << "Partition Object";
    }
    else if(wombatvarptr->selectedobject.type == 4) // fs object
    {
        // set tskobjptr->offset and tskobjptr->length here prior to calling this...
        OpenParentImage(wombatvarptr->selectedobject.parimgid);
        //OpenParentImage(wombatvarptr->filesystemobjectvector[curidx].parimgid);
        tskobjptr->offset = wombatvarptr->selectedobject.byteoffset;
        //tskobjptr->offset = wombatvarptr->filesystemobjectvector[curidx].byteoffset;
        tskobjptr->length = wombatvarptr->selectedobject.blocksize * wombatvarptr->selectedobject.blockcount;
        //tskobjptr->length = wombatvarptr->filesystemobjectvector[curidx].blocksize * wombatvarptr->filesystemobjectvector[curidx].blockcount;
        //qDebug() << "File System Object";
    }
    else if(wombatvarptr->selectedobject.type == 5) // file object
    {
        OpenParentImage(wombatvarptr->selectedobject.parimgid);
        //tskobjptr->offset = 
        //tskobjptr->offset = wombatvarptr->selectedobject.byteoffset;
    }
    hexwidget->openimage();
    hexwidget->set2BPC();
    hexwidget->setBaseHex();
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
    wombatvarptr->htmlcontent = "";
    int curidx = wombatframework->DetermineVectorIndex();
    if(isok && curidx > -1)
    {
        if(wombatvarptr->selectedobject.type == 1) // image file
        {
            wombatvarptr->htmlcontent += "<div id='infotitle'>image information</div><br/>";
            wombatvarptr->htmlcontent += "<table><tr><td class='property'>imagetype:</td><td class='pvalue'>";
            wombatvarptr->htmlcontent += QString(tsk_img_type_todesc((TSK_IMG_TYPE_ENUM)wombatvarptr->evidenceobjectvector[curidx].type)) + "</td></tr>";
            wombatvarptr->htmlcontent += "<tr><td class='property'>size:</td><td class='pvalue'>";
            wombatvarptr->htmlcontent += QLocale::system().toString((int)wombatvarptr->evidenceobjectvector[curidx].size) + " bytes</td></tr>";
            wombatvarptr->htmlcontent += "<tr><td class='property'>sector size:</td><td class='pvalue'>";
            wombatvarptr->htmlcontent += QLocale::system().toString(wombatvarptr->evidenceobjectvector[curidx].sectsize) + " bytes</td></tr>";
            wombatvarptr->htmlcontent += "<tr><td class='property'>sector count:</td><td class='pvalue'>";
            wombatvarptr->htmlcontent += QLocale::system().toString((int)((float)wombatvarptr->evidenceobjectvector[curidx].size/(float)wombatvarptr->evidenceobjectvector[curidx].sectsize));
            wombatvarptr->htmlcontent += " sectors</td></tr>";
            // might not want to do the volume type one if there's no volume. have to think on it.
            //wombatvarptr->htmlcontent += " sectors</td></tr><tr><td class='property'>volume type</td><td class='pvalue'>";
            //wombatvarptr->htmlcontent += wombatvarptr->volumeobject.name + "</td></tr>";
            wombatframework->GetBootCode(curidx); // determine boot type in this function and populate html string information into wombatvarptr value
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
        else if(wombatvarptr->selectedobject.type == 2) // volume file (it should never be a volume since i don't add it to the image tree)
        {
        }
        else if(wombatvarptr->selectedobject.type == 3) // partition file
        {
        }
        else if(wombatvarptr->selectedobject.type == 4) // file system file
        {
        }
        else // implement for files, directories etc.. as i go.
        {
        }
    }
}

void WombatForensics::OpenParentImage(int imgid)
{
    int curidx = 0;
    qDebug() << "evidobjvec.count: " << wombatvarptr->evidenceobjectvector.count();
    for(int i=0; i < wombatvarptr->evidenceobjectvector.count(); i++)
    {
        if(imgid == wombatvarptr->evidenceobjectvector[i].id)
            curidx = i;
    }
    tskobjptr->imagepartspath = (const char**)malloc(wombatvarptr->evidenceobjectvector[curidx].fullpathvector.size()*sizeof(char*));
    tskobjptr->partcount = wombatvarptr->evidenceobjectvector[curidx].fullpathvector.size();
    for(int i=0; i < wombatvarptr->evidenceobjectvector[curidx].fullpathvector.size(); i++)
    {
        tskobjptr->imagepartspath[i] = wombatvarptr->evidenceobjectvector[curidx].fullpathvector[i].c_str();
    }
    tskobjptr->readimginfo = tsk_img_open(tskobjptr->partcount, tskobjptr->imagepartspath, TSK_IMG_TYPE_DETECT, 0);
    if(tskobjptr->readimginfo == NULL)
    {
        //qDebug() << "print image error here";
    }
    free(tskobjptr->imagepartspath);
}

void WombatForensics::RemEvidence()
{
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
{/*
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
    exportdialog = new ExportDialog(this, checkcount, listcount);
    connect(exportdialog, SIGNAL(FileExport(FileExportData*)), this, SLOT(FileExport(FileExportData*)), Qt::DirectConnection);
    exportdialog->show();*/
}

void WombatForensics::FileExport(FileExportData* exportdata)
{/*
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
    //threadpool->start(trun);*/
}

void WombatForensics::UpdateProgress(int filecount, int processcount)
{
    //qDebug() << "Global Class Called This to AutoUpdate!!!";
    int curprogress = (int)((((float)processcount)/(float)filecount)*100);
    mainprogress->setValue(curprogress);
    mainprogress->setFormat("Processed " + QString::number(processcount) + " of " + QString::number(filecount) + " " + QString::number(curprogress) + "%");
    //wombatprogresswindow->UpdateFilesFound(QString::number(filecount));
    //wombatprogresswindow->UpdateFilesProcessed(QString::number(processcount));
    //wombatprogresswindow->UpdateProgressBar(curprogress);
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

void WombatForensics::ResizeColumns(void)
{
    for(int i=0; i < ((FileViewSqlModel*)ui->dirTreeView->model())->columnCount(); i++)
    {
        // may need to compare treeview->model() == currentmodel) to determine what to set it to.
        // depending on the design though, i may not need multiple layouts since the columns can be sorted.
        // have to see as i go. for now its good.
        ui->dirTreeView->resizeColumnToContents(i);
    }
}

void WombatForensics::SetupHexPage(void)
{
    // hex editor page
    QBoxLayout* mainlayout = new QBoxLayout(QBoxLayout::TopToBottom, ui->hexPage);
    QHBoxLayout* hexLayout = new QHBoxLayout();
    hstatus = new QStatusBar(ui->hexPage);
    hstatus->setSizeGripEnabled(false);
    hstatus->setMaximumHeight(20);
    selectedoffset = new QLabel(this);
    selectedhex = new QLabel(this);
    selectedoffset->setText("Offset: 00");
    selectedoffset->setAlignment(Qt::AlignVCenter);
    selectedhex->setText("Length: 0");
    //selectedoffset = new QLabel("Offset: 00");
    //selectedhex = new QLabel("Length: 0");
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
    hexwidget = new HexEditor(ui->hexPage, tskobjptr);
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
    if(ui->dirTreeView->model() != NULL)
    {
        if(checkableproxy->sourceModel() != NULL)
            //((FileViewSqlModel*)treeproxy->sourceModel())->clear(); // clear sql so db can be closed.
            ((FileViewSqlModel*)checkableproxy->sourceModel())->clear(); // clear sql so db can be closed.
    }
    //((FileViewSqlModel*)ui->dirTreeView->model())->clear(); // clear sql so db can be closed.
    wombatprogresswindow->close();
    RemoveTmpFiles();
    // USE THIS FUNCTION AND PRINCIPLE TO BUILD THE PROGRESS WINDOW FUNCTIONALITY.
    if(ProcessingComplete())
    {
        qDebug() << "All threads are done";
    }
    else
    {
        qDebug() << "All threads aren't done yet.";
    }
    wombatdatabase->CloseCaseDB();
    wombatdatabase->CloseAppDB();
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
    qDebug() << "selection changed before mapping.";
    QModelIndex srcindex = checkableproxy->mapToSource(index);
    qDebug() << "selection changed id: " << srcindex.sibling(srcindex.row(), 0).data().toInt();
    wombatvarptr->selectedobject.id = index.sibling(index.row(), 0).data().toInt(); // object id
    wombatdatabase->GetObjectValues(); // now i have selectedobject.values.
    qDebug() << "selected id: " << wombatvarptr->selectedobject.id << " type: " << wombatvarptr->selectedobject.type;
    UpdateOmniValue();
    UpdateViewer();
    // NEED TO DETERMINE THE DATA TYPE TO CALL THE CORRECT DATA TO UPDATE.
    //
    //QString sigtext = "";
    // I CAN EITHER GET THE SIGNATURE USING COMPARISON OR BY CALLING FILE AND MAGIC LIKE THE MODULE
    // SELECTING ITEM GETS IT'S ID VALUES AND SET'S RESPECTIVE WOMBATVARPTR->VALUES
    // GET THE RESPECTIVE VISIBLE VIEWER FROM THE VIEWER STACK.
    // LOAD THE DATA INTO THE RESPECTIVE VIEWER.
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
