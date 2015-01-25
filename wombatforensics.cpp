#include "wombatforensics.h"

WombatForensics::WombatForensics(QWidget *parent) : QMainWindow(parent), ui(new Ui::WombatForensics)
{
    ui->setupUi(this);
    threadpool = QThreadPool::globalInstance();
    wombatvarptr = &wombatvariable;
    this->menuBar()->hide();
    this->statusBar()->setSizeGripEnabled(true);
    selectedoffset = new QLabel(this);
    selectedoffset->setText("Offset: 00");
    selectedhex = new QLabel(this);
    selectedhex->setText("Length: 0");
    filtercountlabel = new QLabel(this);
    filtercountlabel->setText("Filtered: 0");
    filecountlabel = new QLabel(this);
    filecountlabel->setText("Files: 0");
    processcountlabel = new QLabel(this);
    processcountlabel->setText("Processed: 0");
    statuslabel = new QLabel(this);
    statuslabel->setText("");
    vline1 = new QFrame(this);
    vline1->setFrameStyle(QFrame::VLine | QFrame::Raised);
    vline1->setLineWidth(1);
    vline1->setMidLineWidth(0);
    vline2 = new QFrame(this);
    vline2->setFrameStyle(QFrame::VLine | QFrame::Raised);
    vline2->setLineWidth(1);
    vline2->setMidLineWidth(0);
    this->statusBar()->addWidget(selectedoffset, 0);
    this->statusBar()->addWidget(selectedhex, 0);
    this->statusBar()->addWidget(vline1, 0);
    this->statusBar()->addWidget(filtercountlabel, 0);
    this->statusBar()->addWidget(filecountlabel, 0);
    this->statusBar()->addWidget(processcountlabel, 0);
    this->statusBar()->addPermanentWidget(vline2, 0);
    this->statusBar()->addPermanentWidget(statuslabel, 0);
    QWidget* spacer = new QWidget();
    ui->analysisToolBar->addAction(ui->menuBookmark_Manager->menuAction());
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    ui->analysisToolBar->addWidget(spacer);
    ui->analysisToolBar->addAction(ui->menuAbout->menuAction());
    tskobjptr = &tskobject;
    tskobjptr->readimginfo = NULL;
    tskobjptr->readfsinfo = NULL;
    tskobjptr->readfileinfo = NULL;
    wombatdatabase = new WombatDatabase(wombatvarptr);
    wombatframework = new WombatFramework(wombatvarptr);
    propertywindow = new PropertiesWindow(wombatdatabase);
    fileviewer = new FileViewer(this, tskobjptr);
    isignals = new InterfaceSignals();
    idfilterview = new IdFilter(this);
    namefilterview = new NameFilter(this);
    pathfilterview = new PathFilter(this);
    sizefilterview = new SizeFilter(this);
    createfilterview = new CreatedDateFilter(this);
    accessfilterview = new AccessedDateFilter(this);
    modifyfilterview = new ModifiedDateFilter(this);
    changefilterview = new ChangedDateFilter(this);
    filetypefilterview = new FileTypeFilter(this);
    hashfilterview = new HashFilter(this);
    imagewindow = new ImageViewer(this);
    filtervalues.maxcreate = QDateTime::currentDateTimeUtc().toTime_t();
    filtervalues.mincreate = QDateTime::currentDateTimeUtc().toTime_t();
    filtervalues.maxaccess = QDateTime::currentDateTimeUtc().toTime_t();
    filtervalues.minaccess = QDateTime::currentDateTimeUtc().toTime_t();
    filtervalues.maxmodify = QDateTime::currentDateTimeUtc().toTime_t();
    filtervalues.minmodify = QDateTime::currentDateTimeUtc().toTime_t();
    filtervalues.maxchange = QDateTime::currentDateTimeUtc().toTime_t();
    filtervalues.minchange = QDateTime::currentDateTimeUtc().toTime_t();
    connect(imagewindow->sb, SIGNAL(valueChanged(int)), this, SLOT(UpdateThumbnails(int)));
    //connect(ui->webView, SIGNAL(loadFinished(bool)), this, SLOT(LoadComplete(bool)));
    connect(ui->actionView_Properties, SIGNAL(triggered(bool)), this, SLOT(on_actionView_Properties_triggered(bool)), Qt::DirectConnection);
    connect(ui->actionView_File, SIGNAL(triggered(bool)), this, SLOT(on_actionView_File_triggered(bool)), Qt::DirectConnection);
    connect(propertywindow, SIGNAL(HidePropertyWindow(bool)), this, SLOT(HidePropertyWindow(bool)), Qt::DirectConnection);
    connect(fileviewer, SIGNAL(HideFileViewer(bool)), this, SLOT(HideFileViewer(bool)), Qt::DirectConnection);
    connect(isignals, SIGNAL(ProgressUpdate(int, int)), this, SLOT(UpdateProgress(int, int)), Qt::QueuedConnection);
    wombatvarptr->caseobject.id = 0;
    connect(wombatdatabase, SIGNAL(DisplayError(QString, QString, QString)), this, SLOT(DisplayError(QString, QString, QString)), Qt::DirectConnection);
    propertywindow->setModal(false);
    InitializeAppStructure();
    connect(&sqlwatcher, SIGNAL(finished()), this, SLOT(InitializeQueryModel()), Qt::QueuedConnection);
    connect(&remwatcher, SIGNAL(finished()), this, SLOT(FinishRemoval()), Qt::QueuedConnection);
    connect(ui->actionView_Image_Gallery, SIGNAL(triggered(bool)), this, SLOT(on_actionView_Image_Gallery_triggered(bool)), Qt::DirectConnection);

    treemenu = new QMenu(ui->dirTreeView);
    treemenu->addAction(ui->menuBookmark_Manager->menuAction());
    treemodel = new TreeModel(this);
    ui->dirTreeView->setModel(treemodel);
    ui->dirTreeView->hideColumn(4);
    ui->dirTreeView->hideColumn(5);
    ui->dirTreeView->hideColumn(11);
    ui->dirTreeView->hideColumn(12);
    ui->dirTreeView->hideColumn(13);
    ui->dirTreeView->hideColumn(14);
    ui->dirTreeView->hideColumn(15);
    ui->dirTreeView->hideColumn(17);
    ui->dirTreeView->setSortingEnabled(true); // enables the sorting arrow, but doesn't sort anything.
    ui->dirTreeView->header()->setSortIndicatorShown(false);
    ui->dirTreeView->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->dirTreeView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->dirTreeView, SIGNAL(collapsed(const QModelIndex &)), this, SLOT(ExpandCollapseResize(const QModelIndex &)));
    connect(ui->dirTreeView, SIGNAL(expanded(const QModelIndex &)), this, SLOT(ExpandCollapseResize(const QModelIndex &)));
    connect(ui->dirTreeView->selectionModel(), SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)), this, SLOT(SelectionChanged(const QItemSelection &, const QItemSelection &)));
    connect(ui->dirTreeView, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(TreeContextMenu(const QPoint &)));
    connect(ui->dirTreeView->header(), SIGNAL(sectionClicked(int)), this, SLOT(SetFilter(int)));
    //connect(ui->dirTreeView->model(), SIGNAL(headerDataChanged(Qt::Orientation, int, int)), ui->dirTreeView->header(), SLOT(headerDataChanged(Qt::Orientation, int, int)));
    connect(idfilterview, SIGNAL(HeaderChanged()), this, SLOT(FilterApplied()));
    connect(namefilterview, SIGNAL(HeaderChanged()), this, SLOT(FilterApplied()));
    connect(pathfilterview, SIGNAL(HeaderChanged()), this, SLOT(FilterApplied()));
    connect(sizefilterview, SIGNAL(HeaderChanged()), this, SLOT(FilterApplied()));
    connect(createfilterview, SIGNAL(HeaderChanged()), this, SLOT(FilterApplied()));
    connect(accessfilterview, SIGNAL(HeaderChanged()), this, SLOT(FilterApplied()));
    connect(modifyfilterview, SIGNAL(HeaderChanged()), this, SLOT(FilterApplied()));
    connect(changefilterview, SIGNAL(HeaderChanged()), this, SLOT(FilterApplied()));
    connect(filetypefilterview, SIGNAL(HeaderChanged()), this, SLOT(FilterApplied()));
    connect(hashfilterview, SIGNAL(HeaderChanged()), this, SLOT(FilterApplied()));
    jumpforward = new QShortcut(ui->dirTreeView);
    jumpbackward = new QShortcut(ui->dirTreeView);
    jumpforward->setKey(Qt::CTRL + Qt::Key_J);
    jumpbackward->setKey(Qt::CTRL + Qt::SHIFT + Qt::Key_J);
    connect(jumpforward, SIGNAL(activated()), this, SLOT(NextItem()));
    connect(jumpbackward, SIGNAL(activated()), this, SLOT(PreviousItem()));
}

void WombatForensics::HidePropertyWindow(bool checkedstate)
{
    ui->actionView_Properties->setChecked(checkedstate);
}

void WombatForensics::HideFileViewer(bool checkedstate)
{
    ui->actionView_File->setChecked(checkedstate);
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
    //wombatvarptr->tmpfilepath = homePath + "tmpfiles"; // currently not used if i continue to view objects in memory...
    bool mkPath = (new QDir())->mkpath(wombatvarptr->settingspath);
    if(mkPath == false)
        DisplayError("2.0", "App Settings Folder Failed.", "App Settings Folder was not created.");
    mkPath = (new QDir())->mkpath(wombatvarptr->datapath);
    if(mkPath == false)
        DisplayError("2.1", "App Data Folder Failed.", "Application Data Folder was not created.");
    mkPath = (new QDir())->mkpath(wombatvarptr->casespath);
    if(mkPath == false)
        DisplayError("2.2", "App Cases Folder Failed.", "App Cases Folder was not created.");
    //mkPath = (new QDir())->mkpath(wombatvarptr->tmpfilepath);
    //if(mkPath == false)
    //    DisplayError("2.2", "App TmpFile Folder Failed.", "App TmpFile Folder was not created.");
    wombatvarptr->wombatdbname = wombatvarptr->datapath + "WombatApp.db";
    wombatvarptr->appdb = QSqlDatabase::addDatabase("QSQLITE", "appdb");
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
    ui->actionAdd_Evidence->setEnabled(false);
    ui->actionRemove_Evidence->setEnabled(false);
    ui->actionView_Progress->setEnabled(false);
    ui->actionView_Properties->setEnabled(false);
    ui->actionView_File->setEnabled(false);
    ui->actionExport_Evidence->setEnabled(false);
    ui->menuBookmark_Manager->setEnabled(false);
    ui->actionView_Image_Gallery->setEnabled(false);
    QList<int> sizelist;
    sizelist.append(height()/2);
    sizelist.append(height()/2);
    ui->splitter->setSizes(sizelist);
    QFile magfile(":/magic/magicfile");
    QString magicpath = wombatvarptr->datapath + "magic.mgc";
    if(!FileExists(magicpath.toStdString()))
    {
        QFile installmagfile(magicpath);
        if(magfile.open(QIODevice::ReadOnly))
        {
            if(installmagfile.open(QIODevice::WriteOnly))
            {
                QByteArray tmparray = magfile.readAll();
                installmagfile.write(tmparray);
                installmagfile.close();
                magfile.close();
           }
        }
    }
    magicmimeptr = magic_open(MAGIC_MIME);
    magicptr = magic_open(MAGIC_NONE);
    magic_load(magicptr, magicpath.toStdString().c_str());
    magic_load(magicmimeptr, magicpath.toStdString().c_str());
    SetupHexPage();
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
            DisplayError("2.0", "Cases Folder Creation Failed.", "New Case folder was not created.");
        // CREATE CaseLog.DB HERE
        logdb = QSqlDatabase::database("logdb");
        if(!logdb.isValid())
            logdb = QSqlDatabase::addDatabase("QSQLITE", "logdb");
        logdb.setDatabaseName(wombatvarptr->caseobject.dirpath + "caselog.db");
        if(!FileExists((wombatvarptr->caseobject.dirpath + "caselog.db").toStdString()))
        {
            wombatdatabase->CreateLogDB();
            if(wombatvarptr->curerrmsg.compare("") != 0)
                DisplayError("1.1", "Course Log Creation Error", wombatvarptr->curerrmsg);
        }
        else
        {
            wombatdatabase->OpenLogDB();
            if(wombatvarptr->curerrmsg.compare("") != 0)
                DisplayError("1.1", "SQL Error", wombatvarptr->curerrmsg);
        }
        thumbdb = QSqlDatabase::database("thumbdb");
        if(!thumbdb.isValid())
            thumbdb = QSqlDatabase::addDatabase("QSQLITE", "thumbdb");
        thumbdb.setDatabaseName(wombatvarptr->caseobject.dirpath + "thumbs.db");
        if(!FileExists((wombatvarptr->caseobject.dirpath + "thumbs.db").toStdString()))
        {
            wombatdatabase->CreateThumbDB();
            if(wombatvarptr->curerrmsg.compare("") != 0)
                DisplayError("1.01", "Thumb DB Error", wombatvarptr->curerrmsg);
        }
        else
        {
            wombatdatabase->OpenThumbDB();
            if(wombatvarptr->curerrmsg.compare("") != 0)
                DisplayError("1.02", "Thumb DB Open Error", wombatvarptr->curerrmsg);
        }
        // CREATE CASEID-CASENAME.DB RIGHT HERE.
        errorcount = 0;
        StartJob(0, wombatvarptr->caseobject.id, 0);
        LogEntry(wombatvarptr->caseobject.id, 0, currentjobid, 1, "Started Creating Case Structure");
        wombatvarptr->caseobject.dbname = wombatvarptr->caseobject.dirpath + casestring + ".db";
        wombatvarptr->casedb = QSqlDatabase::database("casedb");
        if(!wombatvarptr->casedb.isValid())
            wombatvarptr->casedb = QSqlDatabase::addDatabase("QSQLITE", "casedb"); // may not need this
        wombatvarptr->casedb.setDatabaseName(wombatvarptr->caseobject.dbname);
        if(!FileExists(wombatvarptr->caseobject.dbname.toStdString()))
        {
            wombatdatabase->CreateCaseDB();
            if(wombatvarptr->curerrmsg.compare("") != 0)
            {
                LogEntry(wombatvarptr->caseobject.id, 0, currentjobid, 0, "Case DB Creation Error.");
                errorcount++;
                DisplayError("1.2", "Case DB Creation Error", wombatvarptr->curerrmsg);
            }
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
        ui->actionAdd_Evidence->setEnabled(true);
        EndJob(currentjobid, 1, 1, errorcount);
        LogEntry(wombatvarptr->caseobject.id, 0, currentjobid, 1, "Case was created");
    }
}

void WombatForensics::InitializeOpenCase()
{
    // open case here
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
        // CREATE CaseLog.DB HERE
        logdb = QSqlDatabase::database("logdb");
        if(!logdb.isValid())
            logdb = QSqlDatabase::addDatabase("QSQLITE", "logdb");
        logdb.setDatabaseName(wombatvarptr->caseobject.dirpath + "caselog.db");
        if(!FileExists((wombatvarptr->caseobject.dirpath + "caselog.db").toStdString()))
        {
            wombatdatabase->CreateLogDB();
            if(wombatvarptr->curerrmsg.compare("") != 0)
                DisplayError("1.1", "Course Log Creation Error", wombatvarptr->curerrmsg);
        }
        else
        {
            wombatdatabase->OpenLogDB();
            if(wombatvarptr->curerrmsg.compare("") != 0)
                DisplayError("1.1", "SQL Error", wombatvarptr->curerrmsg);
        }
        // CREATE CASEID-CASENAME.DB RIGHT HERE.
        wombatvarptr->caseobject.dbname = wombatvarptr->caseobject.dirpath + casestring + ".db";
        wombatvarptr->casedb = QSqlDatabase::database("casedb");
        if(!wombatvarptr->casedb.isValid()) // casedb has not been added yet, so add now.
            wombatvarptr->casedb = QSqlDatabase::addDatabase("QSQLITE", "casedb");
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
        ui->actionAdd_Evidence->setEnabled(true);
        processcountlabel->setText("Processed: 0");
        filecountlabel->setText("Files: 0");
        wombatdatabase->GetEvidenceObjects();

        for(int i=0; i < wombatvarptr->evidenceobjectvector.count(); i++)
        {
            wombatvarptr->currentevidencename = QString::fromStdString(wombatvarptr->evidenceobjectvector.at(i).fullpathvector[0]).split("/").last(); 
            currentevidencename = wombatvarptr->currentevidencename;
            wombatdatabase->ReturnFileSystemObjectList(wombatvarptr->evidenceobjectvector.at(i).id);
            wombatvarptr->currentevidenceid = wombatvarptr->evidenceobjectvector.at(i).id;
            wombatvarptr->evidenceobject = wombatvarptr->evidenceobjectvector.at(i);
            statuslabel->setText("Opening Case Evidence...");
            OpenEvidenceStructure();
            if(ui->dirTreeView->model() != NULL)
                ui->actionRemove_Evidence->setEnabled(true);
        }
    }

}

void WombatForensics::InitializeQueryModel()
{
    fcasedb.commit();
    if(ProcessingComplete())
    {
        EndJob(currentjobid, filesfound, filesprocessed, errorcount);
        LogEntry(wombatvarptr->caseobject.id, wombatvarptr->currentevidenceid, currentjobid, 1, "Adding Evidence Finished");
        statuslabel->setText(QString("Adding Evidence Finished with " + QString::number(errorcount) + " error(s)"));
        LogEntry(wombatvarptr->caseobject.id, wombatvarptr->currentevidenceid, currentjobid, 1, "All Threads have finished");
        fcasedb.commit();
        LogEntry(wombatvarptr->caseobject.id, wombatvarptr->currentevidenceid, currentjobid, 1, "DB Commit finished");
        treemodel->AddEvidence(wombatvarptr->currentevidenceid);
        ui->dirTreeView->setCurrentIndex(treemodel->index(0, 0, QModelIndex()));
        ResizeColumns();
        ui->actionRemove_Evidence->setEnabled(true);
        wombatframework->CloseInfoStructures();
    }
}

void WombatForensics::SelectionChanged(const QItemSelection &curitem, const QItemSelection &previtem)
{
    if(previtem.indexes().count() > 0)
        oldselectedindex = previtem.indexes().at(0);
    if(curitem.indexes().count() > 0)
    {
        selectedindex = curitem.indexes().at(0);
        ui->actionView_Properties->setEnabled(true);
        ui->actionView_File->setEnabled(true);
        ui->actionView_Image_Gallery->setEnabled(true);
        ui->actionExport_Evidence->setEnabled(true);
        wombatvarptr->selectedobject.id = selectedindex.sibling(selectedindex.row(), 0).data().toInt(); // object id
        wombatvarptr->selectedobject.name = selectedindex.sibling(selectedindex.row(), 1).data().toString(); // object name
        wombatdatabase->GetObjectValues(); // now i have selectedobject.values.
        LoadHexContents();
        if(propertywindow->isVisible())
            UpdateProperties();
    }
}

void WombatForensics::TreeContextMenu(const QPoint &pt)
{
    QModelIndex index = ui->dirTreeView->indexAt(pt);
    if(index.isValid())
    {
        treemenu->exec(ui->dirTreeView->mapToGlobal(pt));
    }
}

/*
// FUNCTION GETS IMPLEMNTED WHEN YOU CLICK ON A CHECKBOX, BUT DO NOT SELECT THE ROW
void WombatForensics::CurrentChanged(const QModelIndex &curindex, const QModelIndex &previndex)
{
    qDebug() << "current index changed.";
    //dirTreeView_selectionChanged(curindex);
}
*/

void WombatForensics::InitializeEvidenceStructure()
{
    wombatframework->OpenEvidenceImage();
    wombatdatabase->InsertEvidenceObject(); // add evidence to data and image parts to dataruns
    errorcount = 0;
    StartJob(1, wombatvarptr->caseobject.id, wombatvarptr->currentevidenceid);
    LogEntry(wombatvarptr->caseobject.id, wombatvarptr->currentevidenceid, currentjobid, 1, "Started Adding Evidence");
    wombatframework->OpenVolumeSystem();
    wombatframework->GetVolumeSystemName();
    wombatdatabase->InsertVolumeObject(); // add volume to data
    wombatframework->OpenPartitions();
    wombatdatabase->InsertPartitionObjects();
    wombatdatabase->ReturnFileSystemObjectList(wombatvarptr->currentevidenceid);
    wombatframework->OpenFiles();
}

void WombatForensics::OpenEvidenceStructure()
{
    filesfound = filesfound + wombatdatabase->GetEvidenceFileCount();
    filesprocessed = filesfound;
    filtercountlabel->setText("Filtered: " + QString::number(filesprocessed));
    processcountlabel->setText("Processed: " + QString::number(filesprocessed));
    filecountlabel->setText("Files: " + QString::number(filesfound));
    treemodel->AddEvidence(wombatvarptr->currentevidenceid);
    ui->dirTreeView->setCurrentIndex(treemodel->index(0, 0, QModelIndex()));
    statuslabel->setText("Opening Case Evidence Completed");
}

void WombatForensics::AddEvidence()
{
    int isnew = 1;
    threadvector.clear();
    wombatdatabase->GetEvidenceObjects();
    QStringList tmplist = QFileDialog::getOpenFileNames(this, tr("Select Evidence Image(s)"), tr("./"));
    if(tmplist.count())
    {
        // need to figure out how to force the damn action to be untoggled.
        //ui->actionAdd_Evidence->setEnabled(false);
        // maybe set focus for something else when it's done. 
        wombatvarptr->currentevidencename = tmplist[0].split("/").last();
        for(int i=0; i < wombatvarptr->evidenceobjectvector.count(); i++)
        {
            if(wombatvarptr->evidenceobjectvector.at(i).fullpathvector[0].compare(tmplist[0].toStdString()) == 0)
                isnew = 0;
        }
        if(isnew == 1)
        {
            currentevidencename = wombatvarptr->currentevidencename;
            for(int i=0; i < tmplist.count(); i++)
            {
                wombatvarptr->evidenceobject.fullpathvector.push_back(tmplist[i].toStdString());
            }
            wombatvarptr->evidenceobject.itemcount = tmplist.count();
            processcountlabel->setText("Processed: 0");
            filecountlabel->setText("Files: 0");
            statuslabel->setText("Processed 0%");
            // THIS SHOULD HANDLE WHEN THE THREADS ARE ALL DONE.

            sqlfuture = QtConcurrent::run(this, &WombatForensics::InitializeEvidenceStructure);
            sqlwatcher.setFuture(sqlfuture);
            threadvector.append(sqlfuture);
        }
        else
            DisplayError("1.8", "Evidence already exists in the case.", "Add Evidence cancelled");
    }
}

void WombatForensics::UpdateProperties()
{
    wombatdatabase->ReturnObjectPropertyList();
    propertywindow->UpdateTableView();
}

void WombatForensics::LoadHexContents()
{
    if(tskobjptr->readimginfo != NULL)
        tsk_img_close(tskobjptr->readimginfo);
    if(tskobjptr->readfsinfo != NULL)
        tsk_fs_close(tskobjptr->readfsinfo);
    if(tskobjptr->readfileinfo != NULL)
        tsk_fs_file_close(tskobjptr->readfileinfo);

    OpenParentImage(wombatvarptr->selectedobject.id);

    if(wombatvarptr->selectedobject.objtype == 1) // image file
    {
        OpenParentImage(wombatvarptr->selectedobject.id);
        tskobjptr->offset = 0;
        tskobjptr->objecttype = 1;
        tskobjptr->length = wombatvarptr->selectedobject.size;
        tskobjptr->imglength = wombatvarptr->selectedobject.size;
        tskobjptr->sectsize = wombatvarptr->selectedobject.sectsize;
        tskobjptr->blocksize = wombatvarptr->selectedobject.sectsize;
    }
    else if(wombatvarptr->selectedobject.objtype == 2) // volume object
    {
        OpenParentImage(wombatvarptr->selectedobject.parimgid);
        tskobjptr->objecttype = 2;
        tskobjptr->offset = wombatvarptr->selectedobject.sectstart * wombatvarptr->selectedobject.sectsize;
        tskobjptr->length = wombatvarptr->selectedobject.size;
        tskobjptr->sectsize = wombatvarptr->selectedobject.sectsize;
        tskobjptr->blocksize = wombatvarptr->selectedobject.sectsize;
    }
    else if(wombatvarptr->selectedobject.objtype == 3) // unallocated partition
    {
        OpenParentImage(wombatvarptr->selectedobject.parimgid);
        tskobjptr->offset = wombatvarptr->selectedobject.sectstart * wombatvarptr->selectedobject.sectsize;
        tskobjptr->length = wombatvarptr->selectedobject.sectlength * wombatvarptr->selectedobject.sectsize;
        tskobjptr->objecttype = 3;
        tskobjptr->sectsize = wombatvarptr->selectedobject.sectsize;
        tskobjptr->blocksize = wombatvarptr->selectedobject.sectsize;
    }
    else if(wombatvarptr->selectedobject.objtype == 4) // fs object
    {
        OpenParentImage(wombatvarptr->selectedobject.parimgid);
        tskobjptr->offset = wombatvarptr->selectedobject.byteoffset;
        tskobjptr->objecttype = 4;
        tskobjptr->length = wombatvarptr->selectedobject.size;
        tskobjptr->sectsize = wombatvarptr->selectedobject.sectsize;
        tskobjptr->blocksize = wombatvarptr->selectedobject.blocksize;
    }
    else if(wombatvarptr->selectedobject.objtype == 5) // file object
    {
        OpenParentImage(wombatvarptr->selectedobject.parimgid);
        OpenParentFileSystem(wombatvarptr->selectedobject.parfsid);
        tskobjptr->blocksize = tskobjptr->readfsinfo->block_size;
        tskobjptr->offset = 0;
        qDebug() << "tskobjptr->blocksize:" << tskobjptr->blocksize;
        if(wombatvarptr->selectedobject.blockaddress.compare("") != 0)
            tskobjptr->offset = wombatvarptr->selectedobject.blockaddress.split("|", QString::SkipEmptyParts).at(0).toInt()*tskobjptr->blocksize;
        else
            tskobjptr->offset = 0;
        qDebug() << "file object offset:" << tskobjptr->offset;
        tskobjptr->objecttype = 5;
        tskobjptr->address = wombatvarptr->selectedobject.address;
        tskobjptr->length = wombatvarptr->selectedobject.size;
        tskobjptr->blockaddress = wombatvarptr->selectedobject.blockaddress;
        tskobjptr->blkaddrlist = wombatvarptr->selectedobject.blockaddress.split("|", QString::SkipEmptyParts);
        //OpenFileSystemFile();
    }
    if(wombatvarptr->selectedobject.objtype == 1)
    {
        hexwidget->openimage();
        hexwidget->set1BPC();
        hexwidget->setBaseHex();
        hexwidget->SetTopLeft(0);
    }
    else
    {
        hexwidget->SetTopLeft(tskobjptr->offset);
        if(wombatvarptr->selectedobject.objtype == 5)
        {
            fileviewer->filehexview->openimage();
            fileviewer->filehexview->set1BPC();
            fileviewer->filehexview->setBaseHex();
            fileviewer->filehexview->SetTopLeft(0);
        }
    }
}


/*
void WombatForensics::LoadTxtContents()
{
}

void WombatForensics::LoadWebContents()
{
    if(wombatvarptr->selectedobject.objtype == 1)
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
*/

/*
void WombatForensics::LoadComplete(bool isok)
{
    wombatvarptr->htmlcontent = "";
    int curidx = 0;
    //int curidx = wombatframework->DetermineVectorIndex();
    if(isok && curidx > -1)
    {
        if(wombatvarptr->selectedobject.objtype == 1) // image file
        {
            wombatvarptr->htmlcontent += "<div id='infotitle'>image information</div><br/>";
            wombatvarptr->htmlcontent += "<table><tr><td class='property'>imagetype:</td><td class='pvalue'>";
            wombatvarptr->htmlcontent += QString(tsk_img_type_todesc((TSK_IMG_TYPE_ENUM)wombatvarptr->selectedobject.type)) + "</td></tr>";
            wombatvarptr->htmlcontent += "<tr><td class='property'>size:</td><td class='pvalue'>";
            wombatvarptr->htmlcontent += QLocale::system().toString((int)wombatvarptr->selectedobject.size) + " bytes</td></tr>";
            wombatvarptr->htmlcontent += "<tr><td class='property'>sector size:</td><td class='pvalue'>";
            wombatvarptr->htmlcontent += QLocale::system().toString(wombatvarptr->selectedobject.sectlength) + " bytes</td></tr>";
            wombatvarptr->htmlcontent += "<tr><td class='property'>sector count:</td><td class='pvalue'>";
            wombatvarptr->htmlcontent += QLocale::system().toString((int)((float)wombatvarptr->selectedobject.size/(float)wombatvarptr->selectedobject.sectlength));
            wombatvarptr->htmlcontent += " sectors</td></tr>";
            // might not want to do the volume type one if there's no volume. have to think on it.
            //wombatvarptr->htmlcontent += " sectors</td></tr><tr><td class='property'>volume type</td><td class='pvalue'>";
            //wombatvarptr->htmlcontent += wombatvarptr->volumeobject.name + "</td></tr>";
            //wombatframework->GetBootCode(curidx); // determine boot type in this function and populate html string information into wombatvarptr value
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
        else if(wombatvarptr->selectedobject.objtype == 2) // volume file (it should never be a volume since i don't add it to the image tree)
        {
        }
        else if(wombatvarptr->selectedobject.objtype == 3) // partition file
        {
        }
        else if(wombatvarptr->selectedobject.objtype == 4) // file system file
        {
        }
        else // implement for files, directories etc.. as i go.
        {
        }
    }
}
*/

void WombatForensics::OpenParentImage(int imgid)
{
    wombatdatabase->GetEvidenceObjects();
    int curidx = 0;
    for(int i=0; i < wombatvarptr->evidenceobjectvector.count(); i++)
    {
        if(imgid == wombatvarptr->evidenceobjectvector[i].id)
            curidx = i;
    }
    tskobjptr->imagepartspath = (const char**)malloc(wombatvarptr->evidenceobjectvector[curidx].fullpathvector.size()*sizeof(char*));
    tskobjptr->partcount = wombatvarptr->evidenceobjectvector[curidx].fullpathvector.size();
    for(uint i=0; i < wombatvarptr->evidenceobjectvector[curidx].fullpathvector.size(); i++)
    {
        tskobjptr->imagepartspath[i] = wombatvarptr->evidenceobjectvector[curidx].fullpathvector[i].c_str();
    }
    tskobjptr->readimginfo = tsk_img_open(tskobjptr->partcount, tskobjptr->imagepartspath, TSK_IMG_TYPE_DETECT, 0);
    if(tskobjptr->readimginfo == NULL)
    {
        LogEntry(wombatvarptr->caseobject.id, wombatvarptr->currentevidenceid, 0, 0, "Image opening error");
    }
    free(tskobjptr->imagepartspath);
}

void WombatForensics::OpenParentFileSystem(int fsid)
{
    int fsoffset = wombatdatabase->ReturnFileSystemOffset(fsid);
    tskobjptr->readfsinfo = tsk_fs_open_img(tskobjptr->readimginfo, fsoffset, TSK_FS_TYPE_DETECT);
}

void WombatForensics::OpenFileSystemFile()
{
    tskobjptr->readfileinfo = tsk_fs_file_open_meta(tskobjptr->readfsinfo, NULL, tskobjptr->address);
}

void WombatForensics::CloseCurrentCase()
{
    setWindowTitle("WombatForensics");
    wombatdatabase->GetEvidenceObjects();
    for(int i=0; i < wombatvarptr->evidenceobjectvector.count(); i++)
    {
        treemodel->RemEvidence(wombatvarptr->evidenceobjectvector.at(i).id);
    }
    filesprocessed = 0;
    filesfound = 0;
    filtercountlabel->setText("Filtered: 0");
    processcountlabel->setText("Processed: " + QString::number(filesprocessed));
    filecountlabel->setText("Files: " + QString::number(filesfound));
    statuslabel->setText("Current Case was Closed Successfully"); 
    wombatdatabase->CloseCaseDB();
}

void WombatForensics::RemEvidence()
{
    threadvector.clear();
    wombatvarptr->evidencenamelist.clear();
    wombatdatabase->ReturnEvidenceNameList();
    bool ok;
    wombatvarptr->evidremovestring = QInputDialog::getItem(this, tr("Remove Existing Evidence"), tr("Select the Evidence to Remove: "), wombatvarptr->evidencenamelist, 0, false, &ok);
    if(ok && !wombatvarptr->evidremovestring.isEmpty()) // remove selected evidence
    {
        wombatvarptr->evidremoveid = wombatvarptr->evidremovestring.split(".").at(0).toInt();
        if(wombatvarptr->evidremoveid > 0)
        {
            StartJob(4, wombatvarptr->caseobject.id, wombatvarptr->evidremoveid);
            errorcount = 0;
            LogEntry(wombatvarptr->caseobject.id, wombatvarptr->evidremoveid, currentjobid, 1, "Evidence Removal Started"); 
            treemodel->RemEvidence(wombatvarptr->evidremoveid);
            remfuture = QtConcurrent::run(wombatdatabase, &WombatDatabase::RemoveEvidence);
            remwatcher.setFuture(remfuture);
            threadvector.append(remfuture);
 
        }
    }
}

void WombatForensics::GetExportData(Node* curnode, FileExportData* exportdata)
{
    if(curnode->nodevalues.at(4).toInt() == 5)
    {
        QVariant tmpvariant;
        if(exportdata->filestatus == FileExportData::checked)
        {
            if(curnode->checkstate == 2)
            {
                TskObject tmpobj;
                tmpobj.address = curnode->nodevalues.at(5).toInt();
                tmpobj.length = curnode->nodevalues.at(3).toInt();
                tmpobj.type = curnode->nodevalues.at(12).toInt();
                tmpobj.objecttype = 5;
                tmpobj.offset = 0;
                tmpobj.readimginfo = NULL;
                tmpobj.readfsinfo = NULL;
                tmpobj.readfileinfo = NULL;
                curlist.append(tmpobj);
                exportdata->exportcount = totalchecked;
                exportdata->id = curnode->nodevalues.at(0).toInt();
                exportdata->name = curnode->nodevalues.at(1).toString().toStdString();
                exportdata->fullpath = exportdata->exportpath;
                exportdata->fullpath += "/";
                exportdata->fullpath += currentevidencename.toStdString();
                exportdata->fullpath += "/";
                if(exportdata->pathstatus == FileExportData::include)
                    exportdata->fullpath += curnode->nodevalues.at(2).toString().toStdString();
                exportdata->fullpath += "/";
                exportfilelist.push_back(*exportdata);
            }
        }
        else
        {
            TskObject tmpobj;
            tmpobj.address = curnode->nodevalues.at(5).toInt();
            tmpobj.length = curnode->nodevalues.at(3).toInt();
            tmpobj.type = curnode->nodevalues.at(12).toInt();
            tmpobj.objecttype = 5;
            tmpobj.offset = 0;
            tmpobj.readimginfo = NULL;
            tmpobj.readfsinfo = NULL;
            tmpobj.readfileinfo = NULL;
            curlist.append(tmpobj);
            exportdata->exportcount = totalchecked;
            exportdata->id = curnode->nodevalues.at(0).toInt();
            exportdata->name = curnode->nodevalues.at(1).toString().toStdString();
            exportdata->fullpath = exportdata->exportpath;
            exportdata->fullpath += "/";
            exportdata->fullpath += currentevidencename.toStdString();
            exportdata->fullpath += "/";
            if(exportdata->pathstatus == FileExportData::include)
                exportdata->fullpath += curnode->nodevalues.at(2).toString().toStdString();
            exportdata->fullpath += "/";
            exportfilelist.push_back(*exportdata);
        }
    }
    if(curnode->haschildren)
    {
        for(int i=0; i < curnode->children.count(); i++)
            GetExportData(curnode->children[i], exportdata);
    }
};

void WombatForensics::ExportEvidence()
{
    totalcount = 0;
    totalchecked = 0;
    exportcount = 0;
    ((TreeModel*)ui->dirTreeView->model())->GetModelCount(rootnode);
    exportdialog = new ExportDialog(this, totalchecked, totalcount);
    connect(exportdialog, SIGNAL(FileExport(FileExportData*)), this, SLOT(FileExport(FileExportData*)), Qt::DirectConnection);
    exportdialog->show();
}

void WombatForensics::FileExport(FileExportData* exportdata)
{
    exportfuture = QtConcurrent::run(this, &WombatForensics::ExportFiles, exportdata);
}

void WombatForensics::FinishRemoval()
{
    if(ProcessingComplete())
    {
        filesprocessed = filesprocessed - wombatvarptr->evidrowsremoved;
        filesfound = filesfound - wombatvarptr->evidrowsremoved;
        processcountlabel->setText("Processed: " + QString::number(filesprocessed));
        filecountlabel->setText("Files: " + QString::number(filesfound));
        filtercountlabel->setText("Filtered: " + QString::number(filesfound));
        EndJob(currentjobid, wombatvarptr->evidrowsremoved, wombatvarptr->evidrowsremoved, errorcount);
        LogEntry(wombatvarptr->caseobject.id, wombatvarptr->evidremoveid, currentjobid, 1, "Evidence Removal Completed");
        statuslabel->setText("Evidence Removal of " + QString::number(wombatvarptr->evidrowsremoved) + " completed.");
    }
    else
        LogEntry(wombatvarptr->caseobject.id, wombatvarptr->evidremoveid, currentjobid, 2, "Still Removing Files");
}

void WombatForensics::FinishExport()
{
    if(ProcessingComplete())
    {
        EndJob(currentjobid, exportcount, exportcount, errorcount);
        LogEntry(wombatvarptr->caseobject.id, wombatvarptr->currentevidenceid, currentjobid, 1, "Export Completed");
        statuslabel->setText("Exporting completed with " + QString::number(errorcount) + "error(s)");
    }
    else
        LogEntry(wombatvarptr->caseobject.id, wombatvarptr->evidremoveid, currentjobid, 2, "Still Removing Files");
}

void WombatForensics::ExportFiles(FileExportData* exportdata)
{
    threadvector.clear();
    exportfilelist.clear();
    curlist.clear();
    errorcount = 0;
    StartJob(3, wombatvarptr->caseobject.id, wombatvarptr->currentevidenceid);
    LogEntry(wombatvarptr->caseobject.id, wombatvarptr->currentevidenceid, currentjobid, 1, "Started Exporting EVidence");
    if(exportdata->filestatus == FileExportData::selected)
    {
        exportdata->exportcount = 1;
        exportdata->id = wombatvarptr->selectedobject.id;
        exportdata->name = wombatvarptr->selectedobject.name.toStdString();
        exportdata->fullpath = exportdata->exportpath;
        exportdata->fullpath += "/";
        exportdata->fullpath += currentevidencename.toStdString();
        exportdata->fullpath += "/";
        if(exportdata->pathstatus == FileExportData::include)
            exportdata->fullpath += selectedindex.sibling(selectedindex.row(), 2).data().toString().toStdString();
        exportdata->fullpath += "/";
        exportfilelist.push_back(*exportdata);
        TskObject tmpobj;
        tmpobj.address = selectedindex.sibling(selectedindex.row(), 5).data().toInt();
        tmpobj.offset = 0;
        tmpobj.length = selectedindex.sibling(selectedindex.row(), 3).data().toInt();
        tmpobj.type = selectedindex.sibling(selectedindex.row(), 12).data().toInt();
        tmpobj.objecttype = 5;
        tmpobj.readimginfo = NULL;
        tmpobj.readfsinfo = NULL;
        tmpobj.readfileinfo = NULL;
        curlist.append(tmpobj);
    }
    else
        GetExportData(rootnode, exportdata);
    int curprogress = (int)((((float)exportcount)/(float)curlist.count())*100);
    statuslabel->setText("Exported: " + QString::number(exportcount) + " of " + QString::number(curlist.count()) + " " + QString::number(curprogress) + "%");
    for(int i=0; i < curlist.count(); i++)
    {
        QFuture<void> tmpfuture = QtConcurrent::run(this, &WombatForensics::ProcessExport, curlist.at(i), exportfilelist[i].fullpath, exportfilelist[i].name);
        exportwatcher.setFuture(tmpfuture);
        threadvector.append(tmpfuture);
    }
}

void WombatForensics::ProcessExport(TskObject curobj, std::string fullpath, std::string name)
{
    if(curobj.readimginfo != NULL)
        tsk_img_close(curobj.readimginfo);
    if(curobj.readfsinfo != NULL)
        tsk_fs_close(curobj.readfsinfo);
    if(curobj.readfileinfo != NULL)
        tsk_fs_file_close(curobj.readfileinfo);

    int curidx = 0;
    for(int i=0; i < wombatvarptr->evidenceobjectvector.count(); i++)
    {
        if(wombatvarptr->currentevidenceid == wombatvarptr->evidenceobjectvector[i].id)
            curidx = i;
    }
    curobj.imagepartspath = (const char**)malloc(wombatvarptr->evidenceobjectvector[curidx].fullpathvector.size()*sizeof(char*));
    curobj.partcount = wombatvarptr->evidenceobjectvector[curidx].fullpathvector.size();
    for(uint i=0; i < wombatvarptr->evidenceobjectvector[curidx].fullpathvector.size(); i++)
    {
        curobj.imagepartspath[i] = wombatvarptr->evidenceobjectvector[curidx].fullpathvector[i].c_str();
    }
    curobj.readimginfo = tsk_img_open(curobj.partcount, curobj.imagepartspath, TSK_IMG_TYPE_DETECT, 0);
    if(curobj.readimginfo == NULL)
    {
        LogEntry(wombatvarptr->caseobject.id, wombatvarptr->currentevidenceid, currentjobid, 0, "Image was not loaded properly");
        errorcount++;
    }
    free(curobj.imagepartspath);


    curobj.readfsinfo = tsk_fs_open_img(curobj.readimginfo, 0, TSK_FS_TYPE_DETECT);
    curobj.readfileinfo = tsk_fs_file_open_meta(curobj.readfsinfo, NULL, curobj.address);
    if(curobj.type == 3) // directory
    {
        bool tmpdir = (new QDir())->mkpath(QString::fromStdString(fullpath));
        if(!tmpdir)
        {
            LogEntry(wombatvarptr->caseobject.id, wombatvarptr->currentevidenceid, currentjobid, 0, "creation of export dirtree for file: " + QString::fromStdString(name) + "failed.");
            errorcount++;
        }
    }
    else
    {
        int retval = 0;
        char* contentbuffer = new char[curobj.length];
        retval = tsk_fs_file_read(curobj.readfileinfo, curobj.offset, contentbuffer, curobj.length, TSK_FS_FILE_READ_FLAG_SLACK);
        if(retval > 0)
        {
            bool tmpdir = (new QDir())->mkpath(QDir::cleanPath(QString::fromStdString(fullpath)));
            if(tmpdir == true)
            {
                std::string filepath = fullpath + "/" + name;
                QFile tmpfile(QString::fromStdString(filepath));
                if(tmpfile.open(QIODevice::WriteOnly))
                {
                    QDataStream outbuffer(&tmpfile);
                    outbuffer.writeRawData(contentbuffer, curobj.length);
                    tmpfile.close();
                }
            }
        }
    }
    exportcount++;
    int curprogress = (int)((((float)exportcount)/(float)curlist.count())*100);
    StatusUpdate(QString("Exported " + QString::number(exportcount) + " of " + QString::number(curlist.count()) + " " + QString::number(curprogress) + "%"));
}

void WombatForensics::UpdateProgress(int filecount, int processcount)
{
    int curprogress = (int)((((float)processcount)/(float)filecount)*100);
    processcountlabel->setText("Processed: " + QString::number(filesprocessed));
    filecountlabel->setText("Files: " + QString::number(filesfound));
    statuslabel->setText("Processed: " + QString::number(curprogress) + "%");
    if(curprogress == 100 && ProcessingComplete())
    {
        filtercountlabel->setText("Filtered: " + QString::number(filesfound));
        statuslabel->setText("Processing Complete");
    }
}

void WombatForensics::DisplayError(QString errorNumber, QString errorType, QString errorValue)
{
    QString tmpString = errorNumber;
    tmpString += ". Error: ";
    tmpString += errorType;
    tmpString += " Returned ";
    tmpString += errorValue;
    QMessageBox::warning(this, "Error", tmpString, QMessageBox::Ok);
}

void WombatForensics::ResizeColumns(void)
{
    for(int i=0; i < ((TreeModel*)ui->dirTreeView->model())->columnCount(QModelIndex()); i++)
    {
        ui->dirTreeView->resizeColumnToContents(i);
    }
}

void WombatForensics::SetupHexPage(void)
{
    // hex editor page
    QBoxLayout* mainlayout = new QBoxLayout(QBoxLayout::TopToBottom, ui->hexPage);
    QHBoxLayout* hexLayout = new QHBoxLayout();
    hexwidget = new ImageHexViewer(ui->hexPage, tskobjptr);
    setBackgroundRole(QPalette::Base);
    setAutoFillBackground(true);
    hexwidget->setObjectName("bt-hexview");
    hexwidget->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
    hexLayout->addWidget(hexwidget);
    hexvsb = new QScrollBar(hexwidget);
    hexLayout->addWidget(hexvsb);
    hexvsb->setRange(0, 0);
    mainlayout->addLayout(hexLayout);
    connect(hexwidget, SIGNAL(rangeChanged(off_t,off_t)), this, SLOT(setScrollBarRange(off_t,off_t)));
    connect(hexwidget, SIGNAL(topLeftChanged(off_t)), this, SLOT(setScrollBarValue(off_t)));
    connect(hexwidget, SIGNAL(offsetChanged(off_t)), this, SLOT(SetOffsetLabel(off_t)));
    connect(hexvsb, SIGNAL(valueChanged(int)), hexwidget, SLOT(setTopLeftToPercent(int)));
    connect(hexwidget, SIGNAL(selectionChanged(const QString &)), this, SLOT(UpdateSelectValue(const QString&)));
    connect(hexwidget, SIGNAL(StepValues(int, int)), this, SLOT(SetStepValues(int, int)));
}

void WombatForensics::SetStepValues(int singlestep, int pagestep)
{
    hexvsb->setSingleStep(singlestep);
    hexvsb->setPageStep(pagestep);
}

WombatForensics::~WombatForensics()
{
    delete ui;
}

void WombatForensics::closeEvent(QCloseEvent* event)
{
    // possibly will need to set dirtreeview model to nothing. or the treemodel to nothing to clear values so it'll close properly.
    if(ui->dirTreeView->model() != NULL)
    {
        //event->ignore();
    }
    
    propertywindow->close();
    fileviewer->close();
    imagewindow->close();
    RemoveTmpFiles(); // can get rid of this function right now. I don't need to make temporary files to read.
    if(ProcessingComplete())
    {
        event->accept();
        LogEntry(0, 0, 0, 1, "All threads are done. Exiting...");
    }
    else
    {
        event->ignore();
        LogEntry(0, 0, 0, 0, "All threads aren't done yet. Exiting Cancelled.");
    }
    magic_close(magicptr);
    magic_close(magicmimeptr);
    wombatdatabase->CloseLogDB();
    wombatdatabase->CloseCaseDB();
    wombatdatabase->CloseAppDB();
    wombatdatabase->CloseThumbDB();
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
    // determine if a case is open
    if(wombatvarptr->caseobject.id > 0)
    {
        int ret = QMessageBox::question(this, tr("Close Current Case"), tr("There is a case already open. Are you sure you want to close it?"), QMessageBox::Yes | QMessageBox::No);
        if (ret == QMessageBox::Yes)
            CloseCurrentCase();
    }
    InitializeCaseStructure();
}

void WombatForensics::on_actionOpen_Case_triggered()
{
    // determine if a case is open
    if(wombatvarptr->caseobject.id > 0)
    {
        int ret = QMessageBox::question(this, tr("Close Current Case"), tr("There is a case already open. Are you sure you want to close it?"), QMessageBox::Yes | QMessageBox::No);
        if (ret == QMessageBox::Yes)
        {
            statuslabel->setText("Closing Current Case...");
            CloseCurrentCase();
        }
    }
    InitializeOpenCase();
}

void WombatForensics::on_actionView_Properties_triggered(bool checked)
{
    if(!checked)
    {
        propertywindow->hide();
    }
    else
    {
        propertywindow->show();
        if(ui->dirTreeView->selectionModel()->hasSelection())
            UpdateProperties();
    }
}

void WombatForensics::on_actionView_File_triggered(bool checked)
{
    if(!checked)
    {
        fileviewer->hide();
    }
    else
    {
        fileviewer->show();
    }
}

void WombatForensics::on_actionView_Progress_triggered(bool checked) // modify this to be the logviewer.
{
    if(!checked) // hide logviewer
        qDebug() << "hide logviewer here.";
    else// show logviewer
        qDebug() << "show logviewer here.";
}

void WombatForensics::on_actionView_Image_Gallery_triggered(bool checked)
{
    if(!checked) // hide viewer
    {
        imagewindow->hide();
    }
    else
    {
        //qDebug() << "thumblist count:" << thumblist.count();
        //QStringList images = QFileDialog::getOpenFileNames(0, QObject::tr("Choose images"));
        /*
        ImageWatcher watcher;
        QFuture<QImage> result = QtConcurrent::mapped(thumblist, MakeThumb);
        watcher.setFuture(result);
        imagewindow->SetFutureWatcher(&watcher);
        imagewindow->setModal(false);
        //imagewindow->showMaximized();
        */
        imagewindow->show();
    }
}

void WombatForensics::UpdateThumbnails(int tsize)
{
    thumbsize = tsize;
    // POSSIBLY MAKE THE BELOW CODE A SEPARATE FUNCTION WHICH IS QTCONCURRENTLY HANDLED.
    imagewindow->lw->clear();
    wombatdatabase->GetThumbnails();
    for(int i=0; i < thumblist.count(); i++)
    {
        //QByteArray ba = QByteArray::fromBase64(QByteArray::fromStdString(thumblist.at(i).toStdString()));
        QListWidgetItem* tmpitem = new QListWidgetItem(imagewindow->lw);
        //tmpitem->setData(Qt::DecorationRole, QPixmap::fromImage(QImage::fromData(ba, "PNG")));
        tmpitem->setData(Qt::DecorationRole, QPixmap::fromImage(MakeThumb(thumblist.at(i))));
    }
}

void WombatForensics::UpdateSelectValue(const QString &txt)
{
    // set hex (which i'll probably remove anyway since it's highlighted in same window)
    int sellength = txt.size()/2;
    QString tmptext = "Length: " + QString::number(sellength);
    selectedhex->setText(tmptext);
    /*
    // get initial bytes value and then update ascii
    std::vector<uchar> bytes;
    Translate::HexToByte(bytes, txt);
    QString ascii;
    Translate::ByteToChar(ascii, bytes);
    tmptext = "Ascii: " + ascii;
    //selectedascii->setText(tmptext);
    QString strvalue;
    uchar * ucharPtr;
    // update the int entry:
    // pad right with 0x00
    int intvalue = 0;
    ucharPtr = (uchar*) &intvalue;
    memcpy(&intvalue,&bytes.begin()[0], min(sizeof(int),bytes.size()));
    strvalue.setNum(intvalue);
    tmptext = "Int: " + strvalue;
    //selectedinteger->setText(tmptext);
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
    //selectedfloat->setText(tmptext);
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
    //selecteddouble->setText(tmptext);
    */
}

void WombatForensics::SetOffsetLabel(off_t pos)
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
   hexvsb->setRange(low, high);
}

void WombatForensics::setScrollBarValue(off_t pos)
{
    // THIS IS THE LINE # THAT THE OFFSET FALLS UNDER
    hexvsb->setValue(pos);
  // pos is the topLeft pos, set the scrollbar to the
  // location of the last byte on the page
  // Note: offsetToPercent now rounds up, so we don't
  // have to worry about if this is the topLeft or bottom right
  //hexvsb->setValue(hexwidget->offsetToPercent(pos));
}

void WombatForensics::SetFilter(int headercolumn)
{
    if(headercolumn == 0)
        idfilterview->DisplayFilter();
    if(headercolumn == 1)
        namefilterview->DisplayFilter();
    if(headercolumn == 2)
        pathfilterview->DisplayFilter();
    if(headercolumn == 3)
        sizefilterview->DisplayFilter();
    if(headercolumn == 6)
        createfilterview->DisplayFilter();
    if(headercolumn == 7)
        accessfilterview->DisplayFilter();
    if(headercolumn == 8)
        modifyfilterview->DisplayFilter();
    if(headercolumn == 9)
        changefilterview->DisplayFilter();
    if(headercolumn == 16)
        filetypefilterview->DisplayFilter();
    if(headercolumn == 10)
        hashfilterview->DisplayFilter();
    ResizeColumns();
}

void WombatForensics::NextItem()
{
    QModelIndex curindex = ui->dirTreeView->currentIndex();
    QModelIndexList tmplist = ((TreeModel*)ui->dirTreeView->model())->match(ui->dirTreeView->model()->index(0, 0), Qt::ForegroundRole, QVariant(), -1, Qt::MatchFlags(Qt::MatchExactly | Qt::MatchRecursive));
    if(tmplist.isEmpty() == false)
    {
        for(int i=0; i < tmplist.count(); i++)
        {
            if(tmplist.at(i).internalId() == curindex.internalId() && i < tmplist.count() - 1)
            {
                ui->dirTreeView->setCurrentIndex(tmplist.at(i+1));
                break;
            }
        }
    }
}

void WombatForensics::PreviousItem()
{
    QModelIndex curindex = ui->dirTreeView->currentIndex();
    QModelIndexList tmplist = ((TreeModel*)ui->dirTreeView->model())->match(ui->dirTreeView->model()->index(0, 0), Qt::ForegroundRole, QVariant(), -1, Qt::MatchFlags(Qt::MatchExactly | Qt::MatchWrap | Qt::MatchRecursive));
    if(tmplist.isEmpty() == false)
    {
        for(int i=0; i < tmplist.count(); i++)
        {
            if(tmplist.at(i).internalId() == curindex.internalId() && i > 0)
            {
                ui->dirTreeView->setCurrentIndex(tmplist.at(i-1));
                break;
            }
        }
    }
}

void WombatForensics::UpdateFilterCount()
{
    QModelIndexList tmplist = ((TreeModel*)ui->dirTreeView->model())->match(ui->dirTreeView->model()->index(0, 0), Qt::ForegroundRole, QVariant(), -1, Qt::MatchFlags(Qt::MatchExactly | Qt::MatchWrap | Qt::MatchRecursive));
    filtercountlabel->setText("Filtered: " + QString::number(tmplist.count()));
}
