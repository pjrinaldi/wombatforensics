#include "wombatforensics.h"

// Copyright 2015 Pasquale J. Rinaldi, Jr.
// Distrubted under the terms of the GNU General Public License version 2

WombatForensics::WombatForensics(QWidget *parent) : QMainWindow(parent), ui(new Ui::WombatForensics)
{
    ui->setupUi(this);
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
    StatusUpdate("");
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
    ui->analysisToolBar->addAction(ui->actionAbout);
    tskexternalptr = &tskexternalobject;
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
    jumpfilterview = new JumpFilter(this);
    namefilterview = new NameFilter(this);
    pathfilterview = new PathFilter(this);
    sizefilterview = new SizeFilter(this);
    createfilterview = new CreatedDateFilter(this);
    accessfilterview = new AccessedDateFilter(this);
    modifyfilterview = new ModifiedDateFilter(this);
    changefilterview = new ChangedDateFilter(this);
    filetypefilterview = new FileTypeFilter(this);
    filecategoryfilterview = new FileCategoryFilter(this);
    hashfilterview = new HashFilter(this);
    imagewindow = new ImageViewer();
    videowindow = new VideoViewer();
    textviewer = new TextViewer();
    htmlviewer = new HtmlViewer();
    msgviewer = new MessageViewer();
    byteviewer = new ByteConverter();
    aboutbox = new AboutBox(this);
    propertywindow->setWindowIcon(QIcon(":/bar/propview"));
    fileviewer->setWindowIcon(QIcon(":/bar/fileview"));
    imagewindow->setWindowIcon(QIcon(":/bar/bwimageview"));
    textviewer->setWindowIcon(QIcon(":/bar/textencode"));
    msgviewer->setWindowIcon(QIcon(":/bar/logview"));
    byteviewer->setWindowIcon(QIcon(":/bar/byteconverter"));
    aboutbox->setWindowIcon(QIcon(":/bar/about"));
    filtervalues.maxcreate = QDateTime::currentDateTimeUtc().toTime_t();
    filtervalues.mincreate = QDateTime::currentDateTimeUtc().toTime_t();
    filtervalues.maxaccess = QDateTime::currentDateTimeUtc().toTime_t();
    filtervalues.minaccess = QDateTime::currentDateTimeUtc().toTime_t();
    filtervalues.maxmodify = QDateTime::currentDateTimeUtc().toTime_t();
    filtervalues.minmodify = QDateTime::currentDateTimeUtc().toTime_t();
    filtervalues.maxchange = QDateTime::currentDateTimeUtc().toTime_t();
    filtervalues.minchange = QDateTime::currentDateTimeUtc().toTime_t();
    qRegisterMetaType<QVector<int> >();
    connect(imagewindow->sb, SIGNAL(valueChanged(int)), this, SLOT(UpdateThumbnails(int)), Qt::QueuedConnection);
    connect(imagewindow, SIGNAL(HideImageWindow(bool)), this, SLOT(HideImageWindow(bool)), Qt::DirectConnection);
    connect(textviewer, SIGNAL(HideTextViewerWindow(bool)), this, SLOT(HideTextViewer(bool)), Qt::DirectConnection);
    connect(msgviewer, SIGNAL(HideMessageViewerWindow(bool)), this, SLOT(HideMessageViewer(bool)), Qt::DirectConnection);
    connect(byteviewer, SIGNAL(HideByteConverterWindow(bool)), this, SLOT(HideByteViewer(bool)), Qt::DirectConnection);
    connect(propertywindow, SIGNAL(HidePropertyWindow(bool)), this, SLOT(HidePropertyWindow(bool)), Qt::DirectConnection);
    connect(fileviewer, SIGNAL(HideFileViewer(bool)), this, SLOT(HideFileViewer(bool)), Qt::DirectConnection);
    connect(isignals, SIGNAL(ProgressUpdate(unsigned long long, unsigned long long)), this, SLOT(UpdateProgress(unsigned long long, unsigned long long)), Qt::QueuedConnection);
    wombatvarptr->caseobject.id = 0;
    connect(wombatdatabase, SIGNAL(DisplayError(QString, QString, QString)), this, SLOT(DisplayError(QString, QString, QString)), Qt::DirectConnection);
    propertywindow->setModal(false);
    InitializeAppStructure();
    connect(&sqlwatcher, SIGNAL(finished()), this, SLOT(InitializeQueryModel()), Qt::QueuedConnection);
    connect(&secondwatcher, SIGNAL(finished()), this, SLOT(UpdateStatus()), Qt::QueuedConnection);
    connect(&thumbwatcher, SIGNAL(finished()), this, SLOT(FinishThumbs()), Qt::QueuedConnection);
    connect(&digwatcher, SIGNAL(finished()), this, SLOT(UpdateDigging()), Qt::QueuedConnection);
    connect(&remwatcher, SIGNAL(finished()), this, SLOT(FinishRemoval()), Qt::QueuedConnection);
    connect(ui->actionSection, SIGNAL(triggered(bool)), this, SLOT(AddSection()), Qt::DirectConnection);
    connect(ui->actionTextSection, SIGNAL(triggered(bool)), this, SLOT(AddTextSection()), Qt::DirectConnection);
    connect(ui->actionFile, SIGNAL(triggered(bool)), this, SLOT(CarveFile()), Qt::DirectConnection);

    // NOT SURE IF THIS WILL WORK
    selectionmenu = new QMenu();
    selectionmenu->addAction(ui->actionSection);
    selectionmenu->addAction(ui->actionTextSection);
    selectionmenu->addAction(ui->actionFile);
    QWidget* testwidget = ui->analysisToolBar->widgetForAction(ui->actionCopy_Selection_To);
    QToolButton* testbutton = qobject_cast<QToolButton*>(testwidget);
    if(testbutton)
    {
        connect(ui->actionCopy_Selection_To, SIGNAL(triggered(bool)), testbutton, SLOT(showMenu()));
    }
    ui->actionCopy_Selection_To->setMenu(selectionmenu);

    treemenu = new QMenu(ui->dirTreeView);
    for(int i=0; i < externallist.count(); i++)
    {
        QAction* tmpaction = new QAction(externallist.at(i), this);
        connect(tmpaction, SIGNAL(triggered()), this, SLOT(ShowExternalViewer()));
        ui->menuView_With->addAction(tmpaction);
    }
    treemenu->addAction(ui->menuView_With->menuAction());
    treemenu->addAction(ui->actionCheck);
    treemenu->addAction(ui->actionDigDeeper);
    treemenu->addAction(ui->actionExport);
    treemodel = new TreeModel(this);
    ui->dirTreeView->setModel(treemodel);
    ui->dirTreeView->hideColumn(4);
    ui->dirTreeView->hideColumn(5);
    ui->dirTreeView->hideColumn(11);
    ui->dirTreeView->hideColumn(12);
    ui->dirTreeView->hideColumn(13);
    ui->dirTreeView->hideColumn(14);
    ui->dirTreeView->hideColumn(15);
    ui->dirTreeView->hideColumn(18);
    ui->dirTreeView->hideColumn(19);
    ui->dirTreeView->setSortingEnabled(true); // enables the sorting arrow, but doesn't sort anything.
    ui->dirTreeView->header()->setSortIndicatorShown(false);
    ui->dirTreeView->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->dirTreeView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->dirTreeView, SIGNAL(collapsed(const QModelIndex &)), this, SLOT(ExpandCollapseResize(const QModelIndex &)));
    connect(ui->dirTreeView, SIGNAL(expanded(const QModelIndex &)), this, SLOT(ExpandCollapseResize(const QModelIndex &)));
    connect(ui->dirTreeView->selectionModel(), SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)), this, SLOT(SelectionChanged(const QItemSelection &, const QItemSelection &)));
    connect(ui->dirTreeView, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(TreeContextMenu(const QPoint &)));
    connect(ui->dirTreeView->header(), SIGNAL(sectionClicked(int)), this, SLOT(SetFilter(int)));
    connect(ui->dirTreeView, SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(ShowFile(const QModelIndex &)));
    connect(imagewindow, SIGNAL(SendObjectToTreeView(unsigned long long)), this, SLOT(SetSelectedFromImageViewer(unsigned long long)));
    connect(idfilterview, SIGNAL(HeaderChanged()), this, SLOT(FilterApplied()));
    connect(namefilterview, SIGNAL(HeaderChanged()), this, SLOT(FilterApplied()));
    connect(pathfilterview, SIGNAL(HeaderChanged()), this, SLOT(FilterApplied()));
    connect(sizefilterview, SIGNAL(HeaderChanged()), this, SLOT(FilterApplied()));
    connect(createfilterview, SIGNAL(HeaderChanged()), this, SLOT(FilterApplied()));
    connect(accessfilterview, SIGNAL(HeaderChanged()), this, SLOT(FilterApplied()));
    connect(modifyfilterview, SIGNAL(HeaderChanged()), this, SLOT(FilterApplied()));
    connect(changefilterview, SIGNAL(HeaderChanged()), this, SLOT(FilterApplied()));
    connect(filetypefilterview, SIGNAL(HeaderChanged()), this, SLOT(FilterApplied()));
    connect(filecategoryfilterview, SIGNAL(HeaderChanged()), this, SLOT(FilterApplied()));
    connect(hashfilterview, SIGNAL(HeaderChanged()), this, SLOT(FilterApplied()));
    jumpforward = new QShortcut(ui->dirTreeView);
    jumpbackward = new QShortcut(ui->dirTreeView);
    showitem = new QShortcut(ui->dirTreeView);
    jumpforward->setKey(Qt::CTRL + Qt::Key_J);
    jumpbackward->setKey(Qt::CTRL + Qt::SHIFT + Qt::Key_J);
    showitem->setKey(Qt::Key_Return);
    connect(jumpforward, SIGNAL(activated()), this, SLOT(NextItem()));
    connect(jumpbackward, SIGNAL(activated()), this, SLOT(PreviousItem()));
    connect(showitem, SIGNAL(activated()), this, SLOT(ShowItem()));
    checkhash.clear();
    autosavetimer = new QTimer(this);
    connect(autosavetimer, SIGNAL(timeout()), this, SLOT(AutoSaveState()));
}
void WombatForensics::ShowExternalViewer()
{
    //OpenParentImage
    std::vector<std::string> pathvector;
    unsigned long long imgid = 0;
    unsigned long long fsid = 0;
    unsigned long long fsoffset = 0;
    unsigned long long address = 0;
    unsigned long long filelength = 0;
    int objtype = 0;
    unsigned long long mftattrid = 0;
    pathvector.clear();
    QSqlQuery pimgquery(fcasedb);
    pimgquery.prepare("SELECT parimgid, parfsid, address, parentid, objecttype, mftattrid, size FROM data WHERE objectid = ?;");
    pimgquery.addBindValue(wombatvarptr->selectedobject.id);
    pimgquery.exec();
    pimgquery.next();
    imgid = pimgquery.value(0).toULongLong();
    fsid = pimgquery.value(1).toULongLong();
    objtype = pimgquery.value(4).toInt();
    if(objtype == 5)
        address = pimgquery.value(2).toULongLong();
    else
        address = pimgquery.value(3).toULongLong();
    mftattrid = pimgquery.value(5).toULongLong();
    filelength = pimgquery.value(6).toULongLong();
    pimgquery.finish();
    pimgquery.prepare("SELECT fullpath FROM dataruns WHERE objectid = ? ORDER BY seqnum;");
    pimgquery.addBindValue(imgid);
    if(pimgquery.exec())
    {
        while(pimgquery.next())
        {
            pathvector.push_back(pimgquery.value(0).toString().toStdString());
        }
    }
    pimgquery.finish();
    tskexternalptr->imagepartspath = (const char**)malloc(pathvector.size()*sizeof(char*));
    for(uint i=0; i < pathvector.size(); i++)
    {
        tskexternalptr->imagepartspath[i] = pathvector.at(i).c_str();
    }
    tskexternalptr->readimginfo = tsk_img_open(pathvector.size(), tskexternalptr->imagepartspath, TSK_IMG_TYPE_DETECT, 0);
    free(tskexternalptr->imagepartspath);
    // OpenParentFileSystem
    pimgquery.prepare("SELECT byteoffset FROM data where objectid = ?;");
    pimgquery.addBindValue(fsid);
    pimgquery.exec();
    pimgquery.next();
    fsoffset = pimgquery.value(0).toULongLong();
    pimgquery.finish();
    tskexternalptr->readfsinfo = tsk_fs_open_img(tskexternalptr->readimginfo, fsoffset, TSK_FS_TYPE_DETECT);
    // OpenFile
    tskexternalptr->readfileinfo = tsk_fs_file_open_meta(tskexternalptr->readfsinfo, NULL, address);
    // ReadFileToImageUsingByteArray
    ssize_t filelen = 0;
    char* ibuffer = new char[filelength];
    if(objtype == 5)
        filelen = tsk_fs_file_read(tskexternalptr->readfileinfo, 0, ibuffer, filelength, TSK_FS_FILE_READ_FLAG_NONE);
    else
        filelen = tsk_fs_file_read_type(tskexternalptr->readfileinfo, TSK_FS_ATTR_TYPE_NTFS_DATA, mftattrid, 0, ibuffer, filelength, TSK_FS_FILE_READ_FLAG_NONE);
    QString tmpstring = wombatvarptr->tmpfilepath + QString::number(wombatvarptr->selectedobject.id) + "-tmp";
    QFile tmpfile(tmpstring);
    if(tmpfile.open(QIODevice::WriteOnly))
    {
        QDataStream outbuffer(&tmpfile);
        outbuffer.writeRawData(ibuffer, filelen);
        tmpfile.close();
    }
    QProcess* process = new QProcess(this);
    QStringList arguments;
    arguments << tmpstring;
    process->startDetached(((QAction*)QObject::sender())->text(), arguments);
}

void WombatForensics::SetSelectedFromImageViewer(unsigned long long objectid)
{
    QModelIndexList indexlist = ((TreeModel*)ui->dirTreeView->model())->match(((TreeModel*)ui->dirTreeView->model())->index(0, 0, QModelIndex()), Qt::DisplayRole, QVariant(objectid), -1, Qt::MatchFlags(Qt::MatchExactly | Qt::MatchRecursive));
    if(indexlist.count() > 0)
        ui->dirTreeView->setCurrentIndex(indexlist.at(0));
    //else
       //DisplayError("!", "The Image Path Not Discovered", "The image has not been loaded in the treeview yet, so it cannot be selected."); 
}

void WombatForensics::ShowFile(const QModelIndex &index)
{
    if(index.sibling(index.row(), 16).data().toString().contains("image/"))
    {
        imagewindow->ShowImage(index);
    }
    else if(index.sibling(index.row(), 16).data().toString().contains("video/"))
    {
        videowindow->ShowVideo(wombatvarptr->tmpfilepath, index);
    }
    else if(index.sibling(index.row(), 16).data().toString().contains("text/plain"))
    {
        // toggle the button...
        ui->actionTextViewer->setChecked(true);
        textviewer->ShowText(index);
    }
    else if(index.sibling(index.row(), 16).data().toString().contains("text/html"))
    {
        htmlviewer->ShowHtml(index);
    }
    else
    {
        if(index.sibling(index.row(), 4).data().toInt() == 5 || index.sibling(index.row(), 4).data().toInt() == 6)
            treemenu->exec(QCursor::pos());
    }
}

void WombatForensics::HidePropertyWindow(bool checkedstate)
{
    ui->actionView_Properties->setChecked(checkedstate);
}

void WombatForensics::HideFileViewer(bool checkedstate)
{
    ui->actionView_File->setChecked(checkedstate);
}

void WombatForensics::HideImageWindow(bool checkstate)
{
    ui->actionView_Image_Gallery->setChecked(checkstate);
}

void WombatForensics::HideViewerManager()
{
    treemenu->clear();
    ui->menuView_With->clear();
    for(int i=0; i < externallist.count(); i++)
    {
        QAction* tmpaction = new QAction(externallist.at(i), this);
        connect(tmpaction, SIGNAL(triggered()), this, SLOT(ShowExternalViewer()));
        ui->menuView_With->addAction(tmpaction);
    }
    treemenu->addAction(ui->menuView_With->menuAction());
    treemenu->addAction(ui->actionCheck);
    treemenu->addAction(ui->actionDigDeeper);
    treemenu->addAction(ui->actionExport);

}

void WombatForensics::HideTextViewer(bool checkstate)
{
    ui->actionTextViewer->setChecked(checkstate);
}

void WombatForensics::HideMessageViewer(bool checkstate)
{
    ui->actionViewMessageLog->setChecked(checkstate);
}

void WombatForensics::HideByteViewer(bool checkstate)
{
    ui->actionByteConverter->setChecked(checkstate);
}

void WombatForensics::InitializeAppStructure()
{
    QString homePath = QDir::homePath();
    homePath += "/WombatForensics/";
    wombatvarptr->settingspath = homePath + "settings";
    wombatvarptr->datapath = homePath + "data/";
    wombatvarptr->casespath = homePath + "cases/";
    wombatvarptr->tmpfilepath = homePath + "tmpfiles/";
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
    fappdb = wombatvarptr->appdb;
    viewmanage = new ViewerManager(this);
    viewmanage->setWindowIcon(QIcon(":/bar/viewermanager"));
    connect(viewmanage, SIGNAL(HideManagerWindow()), this, SLOT(HideViewerManager()), Qt::DirectConnection);
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
    ui->actionSaveState->setEnabled(false);
    ui->actionAdd_Evidence->setEnabled(false);
    ui->actionRemove_Evidence->setEnabled(false);
    ui->actionView_Progress->setEnabled(false);
    ui->actionView_Properties->setEnabled(false);
    ui->actionView_File->setEnabled(false);
    ui->actionExport_Evidence->setEnabled(false);
    ui->actionDigDeeper->setEnabled(false);
    ui->menuBookmark_Manager->setEnabled(false);
    ui->actionView_Image_Gallery->setEnabled(false);
    ui->actionCopy_Selection_To->setEnabled(false);
    ui->actionTextViewer->setEnabled(false);
    ui->actionByteConverter->setEnabled(false);
    ui->actionTextViewer->setVisible(false);
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
    //magicmimeptr = magic_open(MAGIC_MIME);
    //magicptr = magic_open(MAGIC_NONE);
    //magic_load(magicptr, magicpath.toStdString().c_str());
    //magic_load(magicmimeptr, magicpath.toStdString().c_str());
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
        // CREATE case log file HERE
        logfile.setFileName(wombatvarptr->caseobject.dirpath + "msglog");
        msglog->clear();
        LogMessage("Log File Created");
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
        LogMessage("Started Creating Case Structure");
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
                LogMessage("Case DB Creation Error");
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
        LogMessage("Case was created");
        autosavetimer->start(10000); // 10 seconds in milliseconds for testing purposes
        //autosavetimer->start(600000); // 10 minutes in milliseconds

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
        logfile.setFileName(wombatvarptr->caseobject.dirpath + "msglog");
        msglog->clear();
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
        autosavetimer->start(10000); // 10 seconds in milliseconds for testing purposes
        //autosavetimer->start(600000); // 10 minutes in milliseconds
        wombatdatabase->GetEvidenceObjects();

        for(int i=0; i < wombatvarptr->evidenceobjectvector.count(); i++)
        {
            wombatvarptr->currentevidencename = QString::fromStdString(wombatvarptr->evidenceobjectvector.at(i).fullpathvector[0]).split("/").last(); 
            currentevidencename = wombatvarptr->currentevidencename;
            wombatdatabase->ReturnFileSystemObjectList(wombatvarptr->evidenceobjectvector.at(i).id);
            wombatvarptr->currentevidenceid = wombatvarptr->evidenceobjectvector.at(i).id;
            wombatvarptr->evidenceobject = wombatvarptr->evidenceobjectvector.at(i);
            StatusUpdate("Opening Case Evidence...");
            OpenEvidenceStructure();
            if(ui->dirTreeView->model() != NULL)
            {
                ui->actionRemove_Evidence->setEnabled(true);
                ui->actionSaveState->setEnabled(true);
                ui->actionDigDeeper->setEnabled(true);
                hexrocker->setEnabled(true);
            }
        }
    }

}

void WombatForensics::InitializeQueryModel()
{
    StatusUpdate("Building Evidence Tree...");
    LogMessage("Building Evidence Tree...");
    treemodel->AddEvidence(wombatvarptr->currentevidenceid);
    ui->dirTreeView->setCurrentIndex(treemodel->index(0, 0, QModelIndex()));
    ResizeColumns();
    StatusUpdate("Evidence Added. Begin File Structure Analysis...");
    LogMessage("Evidence Added. Begin File Structure Analysis...");
    StatusUpdate("File Structure Analysis Finished. Begin Secondary Processing...");
    LogMessage("File Structure Analysis Finished. Begin Secondary Processing...");
    secondprocessvector.clear();
    //adsprocessvector.clear();
    QSqlQuery filequery(fcasedb);
    filequery.prepare("SELECT objectid, parimgid, parfsid, address, name FROM data WHERE objecttype = 5 AND parimgid = ?;");
    filequery.addBindValue(wombatvarptr->currentevidenceid);
    if(filequery.exec())
    {
        while(filequery.next())
        {
            SecondaryProcessObject tmpprocessobject;
            tmpprocessobject.adsprocessvector.clear();
            tmpprocessobject.objectid = filequery.value(0).toULongLong();
            tmpprocessobject.parimgid = filequery.value(1).toULongLong();
            tmpprocessobject.parfsid = filequery.value(2).toULongLong();
            tmpprocessobject.address = filequery.value(3).toULongLong();
            tmpprocessobject.name = filequery.value(4).toString();
            secondprocessvector.append(tmpprocessobject);
        }
    }
    filequery.finish();
    // IMG INFO HERE....
    const TSK_TCHAR** imagepartspath;
    // Open Parent Image
    std::vector<std::string> pathvector;
    pathvector.clear();
    QSqlQuery imgquery(fcasedb);
    imgquery.prepare("SELECT fullpath FROM dataruns WHERE objectid = ? ORDER BY seqnum;");
    imgquery.bindValue(0, wombatvarptr->currentevidenceid);
    if(imgquery.exec())
    {
        while(imgquery.next())
        {
            pathvector.push_back(imgquery.value(0).toString().toStdString());
        }
    }
    imgquery.finish();
    imagepartspath = (const char**)malloc(pathvector.size()*sizeof(char*));
    for(uint i=0; i < pathvector.size(); i++)
    {
        imagepartspath[i] = pathvector.at(i).c_str();
    }
    IMG_2ND_PROC = tsk_img_open(pathvector.size(), imagepartspath, TSK_IMG_TYPE_DETECT, 0);
    free(imagepartspath);
    secondwatcher.setFuture(QtConcurrent::map(secondprocessvector, SecondaryProcessing));
/*
 *    QSqlQuery filequery(fcasedb);
    unsigned long long fsoffset = 0;
    unsigned long long parfsid = 0;
    filequery.prepare("SELECT objectid, parimgid, parfsid, address, name FROM data WHERE objecttype = 5 AND parimgid = ?;");
    filequery.addBindValue(wombatvarptr->currentevidenceid);
    if(filequery.exec())
    {
        while(filequery.next())
        {

 *
 *
 */ 
    // ATTEMPT MULTI THREADING WITH SECONDARY PROCESSING FUNCTIONALITY......
    //secondfuture = QtConcurrent::run(this, &WombatForensics::SecondaryProcessing);
    //secondwatcher.setFuture(secondfuture);
    StatusUpdate("Evidence ready");
    LogMessage("Evidence Ready");
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
        ui->actionTextViewer->setEnabled(true);
        ui->actionExport_Evidence->setEnabled(true);
        ui->actionByteConverter->setEnabled(true);
        wombatvarptr->selectedobject.id = selectedindex.sibling(selectedindex.row(), 0).data().toULongLong(); // object id
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
        actionnode = static_cast<Node*>(index.internalPointer());
        if(index.sibling(index.row(), 4).data().toInt() == 5 || index.sibling(index.row(), 4).data().toInt() == 6)
            treemenu->exec(ui->dirTreeView->mapToGlobal(pt));
    }
}

void WombatForensics::ImgHexMenu(const QPoint &pt)
{
    if(hexselection.compare("") != 0)
        selectionmenu->exec(hexwidget->mapToGlobal(pt));
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
    StatusUpdate("Processing...");
    //LogMessage(tr("Started Adding Evidence"));
    wombatframework->OpenVolumeSystem();
    wombatframework->GetVolumeSystemName();
    wombatdatabase->InsertVolumeObject(); // add volume to data
    wombatframework->OpenPartitions();
    wombatdatabase->InsertPartitionObjects();
    wombatdatabase->ReturnFileSystemObjectList(wombatvarptr->currentevidenceid);
    wombatframework->OpenFiles();
}


void WombatForensics::UpdateDataTable()
{
    StatusUpdate("Processing Complete");
    LogMessage("Evidence has been successfully added");
    StatusUpdate("Evidence ready");
}
void WombatForensics::UpdateStatus()
{

    tsk_img_close(IMG_2ND_PROC);
    ui->actionRemove_Evidence->setEnabled(true);
    ui->actionSaveState->setEnabled(true);
    ui->actionDigDeeper->setEnabled(true);
    hexrocker->setEnabled(true);
    ResizeColumns();
    LogMessage("Processing Complete.");
    StatusUpdate("Evidence ready");
    wombatframework->CloseInfoStructures();
}

void WombatForensics::UpdateDigging()
{
    ResizeColumns();
    LogMessage("Digging Complete");
    StatusUpdate("Evidence ready");
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
    StatusUpdate("Opening Case Evidence Completed");
    LogMessage("Case evidence successfully opened");
    StatusUpdate("Evidence ready");
}

void WombatForensics::AddEvidence()
{
    int isnew = 1;
    wombatdatabase->GetEvidenceObjects();
    QStringList tmplist = QFileDialog::getOpenFileNames(this, tr("Select Evidence Image(s)"), tr("./"));
    if(tmplist.count())
    {
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

            LogMessage("Start Adding Evidence");
            sqlfuture = QtConcurrent::run(this, &WombatForensics::InitializeEvidenceStructure);
            sqlwatcher.setFuture(sqlfuture);
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
        tskobjptr->fsoffset = wombatvarptr->selectedobject.byteoffset;
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
        tskobjptr->fsoffset = tskobjptr->readfsinfo->offset;
        tskobjptr->offset = 0;
        if(wombatvarptr->selectedobject.blockaddress.compare("") != 0)
        {
            tskobjptr->offset = wombatvarptr->selectedobject.blockaddress.split("|", QString::SkipEmptyParts).at(0).toULongLong()*tskobjptr->blocksize + tskobjptr->fsoffset;
        }
        else
        {
            if(tskobjptr->readfsinfo->ftype == TSK_FS_TYPE_NTFS_DETECT)
            {
                tskobjptr->resoffset = wombatdatabase->GetResidentOffset(wombatvarptr->selectedobject.address);
                tskobjptr->offset = tskobjptr->resoffset + tskobjptr->fsoffset;
            }
            else
                tskobjptr->offset = tskobjptr->fsoffset;
        }
        tskobjptr->objecttype = 5;
        tskobjptr->address = wombatvarptr->selectedobject.address;
        tskobjptr->length = wombatvarptr->selectedobject.size;
        tskobjptr->blockaddress = wombatvarptr->selectedobject.blockaddress;
        tskobjptr->blkaddrlist = wombatvarptr->selectedobject.blockaddress.split("|", QString::SkipEmptyParts);
        OpenFileSystemFile();
    }
    else if(wombatvarptr->selectedobject.objtype == 6) // ads file object
    {
        OpenParentImage(wombatvarptr->selectedobject.parimgid);
        OpenParentFileSystem(wombatvarptr->selectedobject.parfsid);
        tskobjptr->blocksize = tskobjptr->readfsinfo->block_size;
        tskobjptr->fsoffset = tskobjptr->readfsinfo->offset;
        tskobjptr->offset = 0;
        tskobjptr->adsoffset = wombatvarptr->selectedobject.address;
        tskobjptr->mftattrid = wombatvarptr->selectedobject.mftattrid;
        if(wombatvarptr->selectedobject.blockaddress.compare("") != 0)
        {
            if(wombatvarptr->selectedobject.blockaddress.split("|", QString::SkipEmptyParts).at(0).toULongLong() == 0)
            {
                tskobjptr->resoffset = wombatdatabase->GetResidentOffset(wombatvarptr->selectedobject.parentid);
                tskobjptr->offset = tskobjptr->resoffset + tskobjptr->fsoffset + wombatvarptr->selectedobject.address;
            }
            else
                tskobjptr->offset = wombatvarptr->selectedobject.blockaddress.split("|", QString::SkipEmptyParts).at(0).toULongLong()*tskobjptr->blocksize + tskobjptr->fsoffset;
        }
        else
        {
            tskobjptr->resoffset = wombatdatabase->GetResidentOffset(wombatvarptr->selectedobject.parentid);
            tskobjptr->offset = tskobjptr->resoffset + tskobjptr->fsoffset + wombatvarptr->selectedobject.address;
        }
        tskobjptr->objecttype = 6;
        tskobjptr->address = wombatvarptr->selectedobject.parentid;
        tskobjptr->length = wombatvarptr->selectedobject.size;
        tskobjptr->blockaddress = wombatvarptr->selectedobject.blockaddress;
        tskobjptr->blkaddrlist = wombatvarptr->selectedobject.blockaddress.split("|", QString::SkipEmptyParts);
        OpenFileSystemFile();
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
        if(wombatvarptr->selectedobject.objtype == 5 || wombatvarptr->selectedobject.objtype == 6)
        {
            fileviewer->filehexview->openimage();
            fileviewer->filehexview->set1BPC();
            fileviewer->filehexview->setBaseHex();
            fileviewer->filehexview->SetTopLeft(0);
        }
    }
}

void WombatForensics::OpenParentImage(unsigned long long imgid)
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
        LogMessage("Image opening error");
    }
    free(tskobjptr->imagepartspath);
}

void WombatForensics::OpenParentFileSystem(unsigned long long fsid)
{
    unsigned long long fsoffset = wombatdatabase->ReturnFileSystemOffset(fsid);
    tskobjptr->readfsinfo = tsk_fs_open_img(tskobjptr->readimginfo, fsoffset, TSK_FS_TYPE_DETECT);
}

void WombatForensics::OpenFileSystemFile()
{
    tskobjptr->readfileinfo = tsk_fs_file_open_meta(tskobjptr->readfsinfo, NULL, tskobjptr->address);
}

void WombatForensics::CloseCurrentCase()
{
    autosavetimer->stop();
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
    StatusUpdate("Current Case was Closed Successfully"); 
    wombatdatabase->CloseCaseDB();
}

void WombatForensics::RemEvidence()
{
    wombatvarptr->evidencenamelist.clear();
    wombatdatabase->ReturnEvidenceNameList();
    bool ok;
    wombatvarptr->evidremovestring = QInputDialog::getItem(this, tr("Remove Existing Evidence"), tr("Select the Evidence to Remove: "), wombatvarptr->evidencenamelist, 0, false, &ok);
    if(ok && !wombatvarptr->evidremovestring.isEmpty()) // remove selected evidence
    {
        wombatvarptr->evidremoveid = wombatvarptr->evidremovestring.split(".").at(0).toULongLong();
        if(wombatvarptr->evidremoveid > 0)
        {
            errorcount = 0;
            LogMessage("Evidence Removal Started");
            treemodel->RemEvidence(wombatvarptr->evidremoveid);
            remfuture = QtConcurrent::run(wombatdatabase, &WombatDatabase::RemoveEvidence);
            remwatcher.setFuture(remfuture);
 
        }
    }
}

void WombatForensics::GetExportData(Node* curnode, FileExportData* exportdata)
{
    if(curnode->nodevalues.at(4).toInt() == 5 || curnode->nodevalues.at(4).toInt() == 6)
    {
        QVariant tmpvariant;
        if(exportdata->filestatus == FileExportData::checked)
        {
            if(curnode->checkstate == 2)
            {
                TskObject tmpobj;
                if(curnode->nodevalues.at(4).toInt() == 6)
                    tmpobj.address = curnode->nodevalues.at(11).toULongLong();
                else
                    tmpobj.address = curnode->nodevalues.at(5).toULongLong();
                tmpobj.length = curnode->nodevalues.at(3).toULongLong();
                tmpobj.type = curnode->nodevalues.at(12).toULongLong();
                tmpobj.objecttype = curnode->nodevalues.at(4).toInt();
                tmpobj.offset = 0;
                tmpobj.readimginfo = NULL;
                tmpobj.readfsinfo = NULL;
                tmpobj.readfileinfo = NULL;
                if(curnode->nodevalues.at(4).toInt() == 6)
                    tmpobj.mftattrid = curnode->nodevalues.at(19).toULongLong();
                curlist.append(tmpobj);
                exportdata->exportcount = totalchecked;
                exportdata->id = curnode->nodevalues.at(0).toULongLong();
                if(curnode->nodevalues.at(4).toInt() == 6)
                    exportdata->name = curnode->nodevalues.at(0).toString().toStdString() + curnode->nodevalues.at(1).toString().toStdString() + string(".ads.dat");
                else
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
            if(curnode->nodevalues.at(4).toInt() == 6)
                tmpobj.address = curnode->nodevalues.at(11).toULongLong();
            else
                tmpobj.address = curnode->nodevalues.at(5).toULongLong();
            tmpobj.length = curnode->nodevalues.at(3).toULongLong();
            tmpobj.type = curnode->nodevalues.at(12).toULongLong();
            tmpobj.objecttype = curnode->nodevalues.at(4).toInt();
            tmpobj.offset = 0;
            tmpobj.readimginfo = NULL;
            tmpobj.readfsinfo = NULL;
            tmpobj.readfileinfo = NULL;
            if(curnode->nodevalues.at(4).toInt() == 6)
                tmpobj.mftattrid = curnode->nodevalues.at(19).toULongLong();
            curlist.append(tmpobj);
            exportdata->exportcount = totalchecked; // I THINK THIS SHOULD BE TOTAL LISTED
            exportdata->id = curnode->nodevalues.at(0).toULongLong();
            if(curnode->nodevalues.at(4).toInt() == 6)
                exportdata->name = curnode->nodevalues.at(0).toString().toStdString() + curnode->nodevalues.at(1).toString().toStdString() + string(".ads.dat");
            else
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
            GetExportData(curnode->children.at(i), exportdata);
    }
}

void WombatForensics::GetDigData(Node* curnode, FileDeepData* deepdata)
{
    if(curnode->nodevalues.at(4).toInt() == 5 || curnode->nodevalues.at(4).toInt() == 6)
    {
        QVariant tmpvariant;
        if(deepdata->filestatus == FileDeepData::checked)
        {
            if(curnode->checkstate == 2)
            {
                TskObject tmpobj;
                if(curnode->nodevalues.at(4).toInt() == 6)
                    tmpobj.address = curnode->nodevalues.at(11).toULongLong();
                else
                    tmpobj.address = curnode->nodevalues.at(5).toULongLong();
                tmpobj.length = curnode->nodevalues.at(3).toULongLong();
                tmpobj.type = curnode->nodevalues.at(12).toULongLong();
                tmpobj.objecttype = curnode->nodevalues.at(4).toInt();
                tmpobj.offset = 0;
                tmpobj.readimginfo = NULL;
                tmpobj.readfsinfo = NULL;
                tmpobj.readfileinfo = NULL;
                if(curnode->nodevalues.at(4).toInt() == 6)
                    tmpobj.mftattrid = curnode->nodevalues.at(19).toULongLong();
                curlist.append(tmpobj);
                deepdata->digcount = totalchecked;
                deepdata->id = curnode->nodevalues.at(0).toULongLong();
                digfilelist.push_back(*deepdata);
            }
        }
        else
        {
            TskObject tmpobj;
            if(curnode->nodevalues.at(4).toInt() == 6)
                tmpobj.address = curnode->nodevalues.at(11).toULongLong();
            else
                tmpobj.address = curnode->nodevalues.at(5).toULongLong();
            tmpobj.objecttype = curnode->nodevalues.at(4).toInt();
            tmpobj.offset = 0;
            tmpobj.readimginfo = NULL;
            tmpobj.readfsinfo = NULL;
            tmpobj.readfileinfo = NULL;
            if(curnode->nodevalues.at(4).toInt() == 6)
                tmpobj.mftattrid = curnode->nodevalues.at(19).toULongLong();
            curlist.append(tmpobj);
            deepdata->digcount = totalchecked; //  I THINK THIS SHOULD BE TOTAL LISTED...
            deepdata->id = curnode->nodevalues.at(0).toULongLong();
            digfilelist.push_back(*deepdata);
        }
    }
    if(curnode->haschildren)
    {
        for(int i = 0; i < curnode->children.count(); i++)
            GetDigData(curnode->children.at(i), deepdata);
    }
}

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

void WombatForensics::FileDig(FileDeepData* deeperdata)
{
    DigFiles(deeperdata);
    //if(deeperdata)
        //qDebug() << "launch in a new thread the dig deeper functionality...";
    //qDebug() << "update message and status text";
}

void WombatForensics::FileExport(FileExportData* exportdata)
{
    ExportFiles(exportdata);
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
        LogMessage(QString("Evidence Removal of " + QString::number(wombatvarptr->evidrowsremoved) + " completed"));
        StatusUpdate("Evidence Removal of " + QString::number(wombatvarptr->evidrowsremoved) + " completed.");
    }
    else
    {
        LogMessage("Still Removing Files");
    }
}

void WombatForensics::FinishExport()
{
    if(ProcessingComplete())
    {
        LogMessage(QString("Export Completed with " + QString::number(errorcount) + " error(s)"));
        StatusUpdate("Exporting completed with " + QString::number(errorcount) + "error(s)");
    }
    else
    {
        LogMessage("Still Removing Files");
    }
}

void WombatForensics::ExportFiles(FileExportData* exportdata)
{
    exportfilelist.clear();
    curlist.clear();
    errorcount = 0;
    LogMessage("Started Exporting Evidence");
    if(exportdata->filestatus == FileExportData::selected)
    {
        exportdata->exportcount = 1;
        exportdata->id = wombatvarptr->selectedobject.id;
        if(wombatvarptr->selectedobject.objtype == 6)
            exportdata->name = QString::number(wombatvarptr->selectedobject.id).toStdString() + wombatvarptr->selectedobject.name.toStdString() + string(".ads.dat");
        else
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
        if(wombatvarptr->selectedobject.objtype == 6)
            tmpobj.address = selectedindex.sibling(selectedindex.row(), 11).data().toULongLong();
        else
            tmpobj.address = selectedindex.sibling(selectedindex.row(), 5).data().toULongLong();
        tmpobj.offset = 0;
        tmpobj.length = selectedindex.sibling(selectedindex.row(), 3).data().toULongLong();
        tmpobj.type = selectedindex.sibling(selectedindex.row(), 12).data().toULongLong();
        tmpobj.objecttype = wombatvarptr->selectedobject.objtype;
        tmpobj.mftattrid = wombatvarptr->selectedobject.mftattrid;
        tmpobj.readimginfo = NULL;
        tmpobj.readfsinfo = NULL;
        tmpobj.readfileinfo = NULL;
        curlist.append(tmpobj);
    }
    else
        GetExportData(rootnode, exportdata);
    int curprogress = (int)((((float)exportcount)/(float)curlist.count())*100);
    LogMessage("Exported: " + QString::number(exportcount) + " of " + QString::number(curlist.count()) + " " + QString::number(curprogress) + "%");
    StatusUpdate("Exported: " + QString::number(exportcount) + " of " + QString::number(curlist.count()) + " " + QString::number(curprogress) + "%");
    for(int i=0; i < curlist.count(); i++)
    {
        QFuture<void> tmpfuture = QtConcurrent::run(this, &WombatForensics::ProcessExport, curlist.at(i), exportfilelist[i].fullpath, exportfilelist[i].name);
        exportwatcher.setFuture(tmpfuture);
    }
}

void WombatForensics::DigFiles(FileDeepData* deepdata)
{
    digfilelist.clear();
    curlist.clear();
    errorcount = 0;
    LogMessage("Digging Deeper into Evidence");
    if(deepdata->filestatus == FileDeepData::selected)
    {
        deepdata->digcount = 1;
        deepdata->id = wombatvarptr->selectedobject.id;
        digfilelist.push_back(*deepdata);
        TskObject tmpobj;
        if(wombatvarptr->selectedobject.objtype == 6)
            tmpobj.address = selectedindex.sibling(selectedindex.row(), 11).data().toULongLong();
        else
            tmpobj.address = selectedindex.sibling(selectedindex.row(), 5).data().toULongLong();
        tmpobj.offset = 0;
        tmpobj.length = selectedindex.sibling(selectedindex.row(), 3).data().toULongLong();
        tmpobj.type = selectedindex.sibling(selectedindex.row(), 12).data().toULongLong();
        tmpobj.objecttype = wombatvarptr->selectedobject.objtype;
        tmpobj.mftattrid = wombatvarptr->selectedobject.mftattrid;
        tmpobj.readimginfo = NULL;
        tmpobj.readfsinfo = NULL;
        tmpobj.readfileinfo = NULL;
        curlist.append(tmpobj);
    }
    else
        GetDigData(rootnode, deepdata);
    int curprogress = (int)((((float)digcount)/(float)curlist.count())*100);
    LogMessage("Dug: " + QString::number(digcount) + " of " + QString::number(curlist.count()) + " " + QString::number(curprogress) + "%");
    StatusUpdate("Dug: " + QString::number(digcount) + " of " + QString::number(curlist.count()) + " " + QString::number(curprogress) + "%");
    for(int i = 0; i < curlist.count(); i++)
    {
        QFuture<void> tmpfuture = QtConcurrent::run(this, &WombatForensics::ProcessDig, curlist.at(i), digfilelist.at(i).id, digfilelist.at(i).digoptions);
        digwatcher.setFuture(tmpfuture);
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
        LogMessage("Image was not loaded properly");
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
            LogMessage(QString("Creation of export directory tree for file: " + QString::fromStdString(name) + " failed"));
            errorcount++;
        }
    }
    else
    {
        int retval = 0;
        char* contentbuffer = new char[curobj.length];
        if(curobj.objecttype == 5)
        {
            retval = tsk_fs_file_read(curobj.readfileinfo, curobj.offset, contentbuffer, curobj.length, TSK_FS_FILE_READ_FLAG_SLACK);
        }
        else
        {
            retval = tsk_fs_file_read_type(curobj.readfileinfo, TSK_FS_ATTR_TYPE_NTFS_DATA, curobj.mftattrid, 0, contentbuffer, curobj.length, TSK_FS_FILE_READ_FLAG_SLACK);
        }
        //qDebug() << "retval:" << retval;
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

void WombatForensics::ProcessDig(TskObject curobj, unsigned long long objectid, std::vector<FileDeepData::DigOptions> digoptions)
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
        LogMessage("Image was not loaded properly");
        errorcount++;
    }
    free(curobj.imagepartspath);

    curobj.readfsinfo = tsk_fs_open_img(curobj.readimginfo, 0, TSK_FS_TYPE_DETECT);
    curobj.readfileinfo = tsk_fs_file_open_meta(curobj.readfsinfo, NULL, curobj.address);
    for(uint i=0; i < digoptions.size(); i++)
    {
        if(digoptions.at(i) == 0) // generate md5 hash
        {
            QModelIndexList indexlist = ui->dirTreeView->model()->match(ui->dirTreeView->model()->index(0, 0), Qt::DisplayRole, QVariant(objectid), 1, Qt::MatchFlags(Qt::MatchRecursive));
            if(indexlist.count() > 0)
                ui->dirTreeView->model()->setData(indexlist.at(0), HashFile(curobj.readfileinfo, objectid), Qt::DisplayRole);
        }
        if(digoptions.at(i) == 1) // generate thumbnails
        {
            QModelIndexList indexlist = ui->dirTreeView->model()->match(ui->dirTreeView->model()->index(0, 0), Qt::DisplayRole, QVariant(objectid), 1, Qt::MatchFlags(Qt::MatchRecursive));
            if(indexlist.count() > 0)
            {
                ThumbFile(curobj.readfileinfo, objectid);
                ui->dirTreeView->model()->setData(indexlist.at(0), QVariant(0), Qt::DisplayRole);
            }
        }
        if(digoptions.at(i) == 2)
        {
            // do seomthing else here...
        }
    }
    digcount++;
    int curprogress = (int)((((float)digcount/(float)curlist.count()))*100);
    StatusUpdate(QString("Dug " + QString::number(digcount) + " of " + QString::number(curlist.count()) + " " + QString::number(curprogress) + "%"));
}

void WombatForensics::UpdateProgress(unsigned long long filecount, unsigned long long processcount)
{
    if(filecount > 0)
    {
    }
    if(processcount > 0)
    {
    }
    double curprogress = (((double)processphase)/(((double)filesfound)*3.0))*100;
    if(curprogress > 100)
        curprogress = 100;
    processcountlabel->setText("Processed: " + QString::number(filesprocessed));
    filecountlabel->setText("Files: " + QString::number(filesfound));
    StatusUpdate("Processing: " + QString::number(curprogress, 'f', 2) + "%");
    filtercountlabel->setText("Filtered: " + QString::number(filesprocessed));
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
    hexwidget->setContextMenuPolicy(Qt::CustomContextMenu);
    lineup = new QPushButton(QIcon(":/basic/lineup"), "", ui->hexPage);
    linedown = new QPushButton(QIcon(":/basic/linedown"), "", ui->hexPage);
    pageup = new QPushButton(QIcon(":/basic/pageup"), "", ui->hexPage);
    pagedown = new QPushButton(QIcon(":/basic/pagedown"), "", ui->hexPage);
    linedown->setAutoRepeat(true);
    lineup->setAutoRepeat(true);
    pagedown->setAutoRepeat(true);
    pageup->setAutoRepeat(true);
    linedown->setVisible(false);
    lineup->setVisible(false);
    pageup->setVisible(false);
    pagedown->setVisible(false);
    hexLayout->addWidget(hexwidget);
    hexrocker = new WombatSlider(ui->hexPage);
    hexrocker->setRange(-100, 100);
    hexrocker->setValue(0);
    hexrocker->setSingleStep(1);
    hexrocker->setEnabled(false);
    hexLayout->addWidget(hexrocker);
    mainlayout->addLayout(hexLayout);
    connect(linedown, SIGNAL(clicked()), hexwidget, SLOT(nextLine()));
    connect(lineup, SIGNAL(clicked()), hexwidget, SLOT(prevLine()));
    connect(pagedown, SIGNAL(clicked()), hexwidget, SLOT(nextPage()));
    connect(pageup, SIGNAL(clicked()), hexwidget, SLOT(prevPage()));
    connect(hexrocker, SIGNAL(ShowJumpFilter()), jumpfilterview, SLOT(DisplayFilter()));
    connect(jumpfilterview, SIGNAL(SetOffset()), hexwidget, SLOT(SetOffset()));
    connect(hexwidget, SIGNAL(offsetChanged(off_t)), this, SLOT(SetOffsetLabel(off_t)));
    connect(hexrocker, SIGNAL(sliderMoved(int)), hexwidget, SLOT(setTopLeftToPercent(int)));
    connect(hexrocker, SIGNAL(sliderMoved(int)), this, SLOT(ShowRockerToolTip(int)));
    connect(hexrocker, SIGNAL(sliderReleased()), this, SLOT(ResetSlider()));
    connect(hexwidget, SIGNAL(selectionChanged(const QString &)), this, SLOT(UpdateSelectValue(const QString&)));
    connect(hexwidget, SIGNAL(SkipDown()), this, SLOT(SkipDown()));
    connect(hexwidget, SIGNAL(SkipUp()), this, SLOT(SkipUp()));
    connect(hexwidget, SIGNAL(PageUp()), this, SLOT(PageUp()));
    connect(hexwidget, SIGNAL(PageDown()), this, SLOT(PageDown()));
    connect(hexwidget, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(ImgHexMenu(const QPoint &)));
}

void WombatForensics::ShowRockerToolTip(int moved)
{
    QToolTip::showText(QCursor::pos(), QString::number(abs(moved)), hexrocker);
}

void WombatForensics::ResetSlider()
{
    if(linedown->isDown())
        linedown->setDown(false);
    if(lineup->isDown())
        lineup->setDown(false);
    if(pagedown->isDown())
        pagedown->setDown(false);
    if(pageup->isDown())
        pageup->setDown(false);
    hexrocker->setValue(0);
}

void WombatForensics::SkipDown()
{
    if(pagedown->isDown())
        pagedown->setDown(false);
    if(lineup->isDown())
        lineup->setDown(false);
    if(pageup->isDown())
        pageup->setDown(false);
    linedown->setDown(true);
}

void WombatForensics::SkipUp()
{
    if(pageup->isDown())
        pageup->setDown(false);
    if(linedown->isDown())
        linedown->setDown(false);
    if(pagedown->isDown())
        pagedown->setDown(false);
    lineup->setDown(true);
}

void WombatForensics::PageUp()
{
    if(lineup->isDown())
        lineup->setDown(false);
    if(linedown->isDown())
        linedown->setDown(false);
    if(pagedown->isDown())
        pagedown->setDown(false);
    pageup->setDown(true);
}

void WombatForensics::PageDown()
{
    if(linedown->isDown())
        linedown->setDown(false);
    if(lineup->isDown())
        lineup->setDown(false);
    if(pageup->isDown())
        pageup->setDown(false);
    pagedown->setDown(true);
}

WombatForensics::~WombatForensics()
{
    delete ui;
}

void WombatForensics::mouseDoubleClickEvent(QMouseEvent* event)
{
    if(event)
    {
    }
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
    videowindow->close();
    viewmanage->close();
    textviewer->close();
    htmlviewer->close();
    byteviewer->close();
    aboutbox->close();
    RemoveTmpFiles();
    event->accept();
    //magic_close(magicptr);
    //magic_close(magicmimeptr);
    msglog->clear();
    msgviewer->close();
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
        {
            CloseCurrentCase();
            InitializeCaseStructure();
        }
    }
    else
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
            StatusUpdate("Closing Current Case...");
            CloseCurrentCase();
            InitializeOpenCase();
        }
    }
    else
        InitializeOpenCase();
}

void WombatForensics::on_actionSaveState_triggered()
{
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
    SaveState();
    QApplication::restoreOverrideCursor();
}

void WombatForensics::on_actionCheck_triggered()
{
    if(actionnode->checkstate < 2)
        actionnode->checkstate = 2;
    else
        actionnode->checkstate = 0;
}

void WombatForensics::on_actionExport_triggered()
{
    totalcount = 0;
    totalchecked = 0;
    exportcount = 0;
    exportdialog = new ExportDialog(this, totalchecked, totalcount);
    connect(exportdialog, SIGNAL(FileExport(FileExportData*)), this, SLOT(FileExport(FileExportData*)), Qt::DirectConnection);
    exportdialog->show();
}

void WombatForensics::on_actionDigDeeper_triggered()
{
    totalcount = 0;
    totalchecked = 0;
    ((TreeModel*)ui->dirTreeView->model())->GetModelCount(rootnode);
    digcount = 0;
    digdeeperdialog = new DigDeeperDialog(this, totalchecked, totalcount);
    connect(digdeeperdialog, SIGNAL(FileDig(FileDeepData*)), this, SLOT(FileDig(FileDeepData*)), Qt::DirectConnection);
    digdeeperdialog->show();
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

/*
void WombatForensics::on_actionView_Progress_triggered(bool checked) // modify this to be the logviewer.
{
    if(!checked) // hide logviewer
        //qDebug() << "hide logviewer here.";
    else// show logviewer
        //qDebug() << "show logviewer here.";
}*/

void WombatForensics::on_actionView_Image_Gallery_triggered(bool checked)
{
    if(!checked) // hide viewer
    {
        imagewindow->hide();
    }
    else
    {
        if(wombatdatabase->ThumbnailCount() == 0)
        {
            int ret = QMessageBox::question(this, tr("Generate Thumbnails"), tr("Thumbnails have not been generated. Do you want to generate all thumbnails now?\r\n\r\nNote: This can take a while and will show the Image Gallery window when complete."), QMessageBox::Yes | QMessageBox::No);
            if(ret == QMessageBox::Yes)
            {
                thumbfuture = QtConcurrent::run(this, &WombatForensics::StartThumbnails);
                thumbwatcher.setFuture(thumbfuture);
            }
            else
                ui->actionView_Image_Gallery->setChecked(false);
        }
        else
        {
            wombatdatabase->GetThumbnails();
            imagewindow->UpdateGeometries();
            imagewindow->show();
        }
    }
}

void WombatForensics::StartThumbnails()
{
    LogMessage("Generating Thumbnails...");
    StatusUpdate("Generating Thumbnails...");
    GenerateThumbnails();
    StatusUpdate("Thumbnail generation finished.");
    LogMessage("Thumbnail generation finished.");
}

void WombatForensics::FinishThumbs()
{
    wombatdatabase->GetThumbnails();
    imagewindow->UpdateGeometries();
    imagewindow->show();
    LogMessage("Evidence ready");
    StatusUpdate("Evidence ready");
}

void WombatForensics::on_actionViewerManager_triggered()
{
    viewmanage->show();
}

void WombatForensics::on_actionTextViewer_triggered(bool checked)
{
    if(!checked) // hide viewer
        textviewer->hide();
    else
    {
        if(selectedindex.sibling(selectedindex.row(), 16).data().toString().contains("text/"))
        {
            textviewer->ShowText(selectedindex);
            textviewer->show();
        }
    }
}

void WombatForensics::on_actionViewMessageLog_triggered(bool checked)
{
    if(!checked) // hide viewer
        msgviewer->hide();
    else
        msgviewer->show();
}

void WombatForensics::on_actionByteConverter_triggered(bool checked)
{
    if(!checked) // hide viewer
        byteviewer->hide();
    else
        byteviewer->show();
}

void WombatForensics::on_actionCopy_Selection_To_triggered()
{
    //qDebug() << "no idea why it won't work how i want it.";
    //selectionmenu->popup(QCursor::pos(), ui->actionCopy_Selection_To);
    //selectionmenu->exec(QCursor::pos());
    //treemenu->exec(QCursor::pos());
}

void WombatForensics::on_actionExpandAll_triggered()
{
    // this can take some time, must alert the user.
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
    ui->dirTreeView->expandAll();
    QApplication::restoreOverrideCursor();
}

void WombatForensics::on_actionCollapseAll_triggered()
{
    // this can take some time, must alert the user.
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
    ui->dirTreeView->collapseAll();
    QApplication::restoreOverrideCursor();
}

void WombatForensics::on_actionAbout_triggered()
{
    aboutbox->show();
}

void WombatForensics::UpdateThumbnails(int tsize)
{
    thumbsize = tsize;
    imagewindow->UpdateGeometries();
}

void WombatForensics::UpdateSelectValue(const QString &txt)
{
    if(txt.compare("") != 0)
    {
        ui->actionCopy_Selection_To->setEnabled(true);
    }
    else
    {
        ui->actionCopy_Selection_To->setEnabled(false);
    }
    hexselection = txt;
    char buf[128];
    int sellength = txt.size()/2;
    QString tmptext = "Length: " + QString::number(sellength);
    QString bytetext = "";
    selectedhex->setText(tmptext);
    // get initial bytes value and then update ascii
    std::vector<uchar> bytes;
    Translate::HexToByte(bytes, txt);
    QString ascii;
    Translate::ByteToChar(ascii, bytes);
    bytetext += "<table border=0 width='100%' cellpadding=5><tr><td>Ascii:</td><td align=right>" + ascii + "</td></tr>";
    QString strvalue;
    uchar * ucharPtr;
    // update the int entry:
    // pad right with 0x00
    int intvalue = 0;
    ucharPtr = (uchar*) &intvalue;
    memcpy(&intvalue,&bytes.begin()[0], min(sizeof(int),bytes.size()));
    strvalue.setNum(intvalue);
    bytetext += "<tr><td>Integer:</td><td align=right>" + strvalue + "</td></tr>";
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
    bytetext += "<tr><td>Float:</td><td align=right>" + strvalue + "</td></tr>";
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
    bytetext += "<tr><td>Double: </td><td align=right>" + strvalue + "</td></tr>";
    bytetext += "<tr><td>DOS Date:</td><td align=right>";
    if(bytes.size() == 2)
    {
        bytetext += QString::fromStdString(std::string(tsk_fs_time_to_str(fatfs_dos_2_unix_time(intvalue, intvalue, bytes[0]), buf)).substr(0, 10));
    }
    bytetext += "</td></tr>";
    bytetext += "<tr><td>DOS Time:</td><td align=right>";
    if(bytes.size() == 2)
    {
        bytetext += QString::fromStdString(std::string(tsk_fs_time_to_str(fatfs_dos_2_unix_time(intvalue, intvalue, bytes[0]), buf)).substr(11, 14));
    }
    bytetext += "</td></tr>";
    bytetext += "<tr><td>FILETIME:</td><td align=right>";
    if(bytes.size() == 8)
    {
        bytetext += tsk_fs_time_to_str(nt2unixtime(intvalue), buf);
    }
    bytetext += "</td></tr>";
    bytetext += "<tr><td>UNIX Time:</td><td align=right>";
    if(bytes.size() == 4)
    {
        bytetext += tsk_fs_time_to_str(intvalue, buf);
    }
    bytetext += "</td></tr>";
    bytetext += "</table>";
    byteviewer->SetText(bytetext);
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
    if(headercolumn == 17)
        filecategoryfilterview->DisplayFilter();
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

void WombatForensics::ShowItem()
{
    QModelIndex curindex = ui->dirTreeView->currentIndex();
    ShowFile(curindex);
}

void WombatForensics::UpdateFilterCount()
{
    QModelIndexList tmplist = ((TreeModel*)ui->dirTreeView->model())->match(ui->dirTreeView->model()->index(0, 0), Qt::ForegroundRole, QVariant(), -1, Qt::MatchFlags(Qt::MatchExactly | Qt::MatchWrap | Qt::MatchRecursive));
    filtercountlabel->setText("Filtered: " + QString::number(tmplist.count()));
}

void WombatForensics::AddSection()
{
    //qDebug() << "Add selected hex to a new section";
}

void WombatForensics::AddTextSection()
{
    //qDebug() << "Add selected text to a new section";
}

void WombatForensics::CarveFile()
{
    QString carvefilename = QFileDialog::getSaveFileName(this, tr("Carve to a File"), QDir::homePath()); 
    if(carvefilename.compare("") != 0)
    {
        std::vector<uchar> tmpbytes;
        Translate::HexToByte(tmpbytes, hexselection);
        ofstream tmpfile(carvefilename.toStdString().c_str(), ios::out | ios::binary);
        tmpfile.write((const char*)&tmpbytes[0], tmpbytes.size());
        tmpfile.close();
    }
}

void WombatForensics::SaveState()
{
    fcasedb.transaction();
    QSqlQuery hashquery(fcasedb);
    hashquery.prepare("UPDATE data SET checked = ? WHERE objectid = ?;");
    QMapIterator<unsigned long long, int> i(checkhash);
    while(i.hasNext())
    {
        i.next();
        if(checkhash.contains(i.key()))
        {
            hashquery.bindValue(0, i.value());
            hashquery.bindValue(1, i.key());
            hashquery.exec();
        }
    }
    fcasedb.commit();
    hashquery.finish();
}

void WombatForensics::AutoSaveState()
{
    // change display text
    StatusUpdate("Saving State Started");
    SaveState();
    StatusUpdate("Evidence ready");
    // change display text
}


void SecondaryProcessing(SecondaryProcessObject &secprocobj)
{
    QMutexLocker locker(&mutex);
    unsigned long long fsoffset = 0;
    int fstype = 0;
    TSK_FS_INFO* readfsinfo;
    TSK_FS_FILE* readfileinfo;
    //OpenParentFileSystem
    QSqlQuery fsquery(fcasedb);
    fsquery.prepare("SELECT byteoffset, type FROM data WHERE objectid = ?;");
    fsquery.bindValue(0, secprocobj.parfsid);
    fsquery.exec();
    fsquery.next();
    fsoffset = fsquery.value(0).toULongLong();
    fstype = fsquery.value(1).toInt();
    fsquery.finish();
    readfsinfo = tsk_fs_open_img(IMG_2ND_PROC, fsoffset, TSK_FS_TYPE_DETECT);
    if(fstype == 1)
    {
        if(QString::compare(secprocobj.name, ".") == 0 || QString::compare(secprocobj.name, "..") == 0)
        {
        }
        else
        {
            QSqlQuery adsquery(fcasedb);
            adsquery.prepare("SELECT objectid, mftattrid FROM data WHERE objecttype = 6 AND parentid = ?;");
            adsquery.bindValue(0, secprocobj.address);
            if(adsquery.exec())
            {
                while(adsquery.next())
                {
                    AdsProcessObject adsprocobj;
                    adsprocobj.objectid = adsquery.value(0).toULongLong();
                    adsprocobj.attrid = adsquery.value(1).toULongLong();
                    secprocobj.adsprocessvector.append(adsprocobj);
                }
            }
            adsquery.finish();
        }
    }
    //Open File
    readfileinfo = tsk_fs_file_open_meta(readfsinfo, NULL, secprocobj.address);
    char magicbuffer[1024];
    tsk_fs_file_read(readfileinfo, 0, magicbuffer, 1024, TSK_FS_FILE_READ_FLAG_NONE);


    // Begin Mime Type Determination
    QMimeDatabase mimedb;
    QMimeType mimetype = mimedb.mimeTypeForData(QByteArray((char*)magicbuffer));
    secprocobj.mimetype = mimetype.name();
    // End Mime Type Determination

    //Begin Block Address Determination
    if((TSK_FS_TYPE_ENUM)fstype == TSK_FS_TYPE_HFS_DETECT || (TSK_FS_TYPE_ENUM)fstype == TSK_FS_TYPE_ISO9660_DETECT || (TSK_FS_TYPE_ENUM)fstype == TSK_FS_TYPE_NTFS_DETECT || (TSK_FS_TYPE_ENUM)fstype == TSK_FS_TYPE_FAT_DETECT)
    {
        if((TSK_FS_TYPE_ENUM)fstype == TSK_FS_TYPE_HFS_DETECT)
        {
            tsk_fs_file_walk_type(readfileinfo, TSK_FS_ATTR_TYPE_HFS_DATA, HFS_FS_ATTR_ID_DATA, (TSK_FS_FILE_WALK_FLAG_ENUM)(TSK_FS_FILE_WALK_FLAG_AONLY | TSK_FS_FILE_WALK_FLAG_SLACK), GetBlockAddress, NULL);
        }
        else if((TSK_FS_TYPE_ENUM)fstype == TSK_FS_TYPE_ISO9660_DETECT)
        {
            iso9660_inode* dinode;
            dinode = (iso9660_inode*)tsk_malloc(sizeof(iso9660_inode));
            iso9660_inode_node* n;
            n = ((ISO_INFO*)readfileinfo->fs_info)->in_list;
            while(n->inum != readfileinfo->meta->addr)
                n = n->next;
            if(n)
                memcpy(dinode, &n->inode, sizeof(iso9660_inode));
            int block = tsk_getu32(readfileinfo->fs_info->endian, dinode->dr.ext_loc_m);
            TSK_OFF_T size = readfileinfo->meta->size;
            while((int64_t)size > 0)
            {
                blockstring += QString::number(block++) + "|";
                size -= readfileinfo->fs_info->block_size;
            }
        }
        else if((TSK_FS_TYPE_ENUM)fstype == TSK_FS_TYPE_NTFS_DETECT)
        {
            unsigned long long minads = 1000;
            for(int i = 0; i < secprocobj.adsprocessvector.count(); i++)
            {
                if(secprocobj.adsprocessvector.at(i).attrid < minads)
                    minads = secprocobj.adsprocessvector.at(i).attrid;
            }
            if(readfileinfo->meta->addr)
            {
                int cnt, i;
                cnt = tsk_fs_file_attr_getsize(readfileinfo);
                for(i = 0; i < cnt; i++)
                {
                    const TSK_FS_ATTR* tmpattr = tsk_fs_file_attr_get_idx(readfileinfo, i);
                    if(tmpattr->flags & TSK_FS_ATTR_NONRES) // non resident attribute
                    {
                        if(tmpattr->type == TSK_FS_ATTR_TYPE_NTFS_DATA && tmpattr->id < (int)minads)
                        {
                            tsk_fs_file_walk_type(readfileinfo, tmpattr->type, tmpattr->id, (TSK_FS_FILE_WALK_FLAG_ENUM)(TSK_FS_FILE_WALK_FLAG_AONLY | TSK_FS_FILE_WALK_FLAG_SLACK), GetBlockAddress, NULL);
                        }
                    }
                }
            }
        }
        else if((TSK_FS_TYPE_ENUM)fstype == TSK_FS_TYPE_FAT_DETECT)
        {
            tsk_fs_file_walk(readfileinfo, (TSK_FS_FILE_WALK_FLAG_ENUM)(TSK_FS_FILE_WALK_FLAG_AONLY | TSK_FS_FILE_WALK_FLAG_SLACK), GetBlockAddress, NULL);
        }
    }
    else
    {
        tsk_fs_file_walk(readfileinfo, TSK_FS_FILE_WALK_FLAG_AONLY, GetBlockAddress, NULL);
    }
    secprocobj.blockaddress = blockstring;
    qDebug() << "current id. filename:" << secprocobj.objectid << "." << secprocobj.name;

    /*
     * STILL NEED TO DO THE ADS BLOCK ADDRESS ......
     *
     *    blockstring = "";
    if(tmpfile->fs_info->ftype == TSK_FS_TYPE_NTFS_DETECT)
    {
        for(int j=0; j < adsattrid.count(); j++)
        {
            if(tmpfile->meta != NULL)
            {
                tsk_fs_file_walk_type(tmpfile, TSK_FS_ATTR_TYPE_NTFS_DATA, adsattrid.at(j), (TSK_FS_FILE_WALK_FLAG_ENUM)(TSK_FS_FILE_WALK_FLAG_AONLY | TSK_FS_FILE_WALK_FLAG_SLACK), GetBlockAddress, NULL);
            }
            QSqlQuery blockquery(fcasedb);
            blockquery.prepare("UPDATE data SET blockaddress = ? WHERE objectid = ?;");
            blockquery.bindValue(0, blockstring);
            blockquery.bindValue(1, adsobjid.at(j));
            blockquery.exec();
            blockquery.next();
            blockquery.finish();
        }
    }

     */


    //End Block Address Determination






    // STORE MIMETYPE IN SECPROCOBJ.MIME = MIMETYPE.NAME WHICH WOULD GO BACK IN MY VECTOR FUNCTION TO WRITE TO THE DB LATER AS A SINGLE TRANSACTION FOR SPEED....
    //QModelIndexList indexlist = ui->dirTreeView->model()->match(ui->dirTreeView->model()->index(0, 0), Qt::DisplayRole, QVariant(secprocjob.objectid), 1, Qt::MatchFlags(Qt:::MatchRecursive));
    //if(indexlist.count() > 0)
        //ui->dirTreeView->model()->setData(indexlist.at(0), MagicFile(readfileinfo, secprocjob.objectid), Qt::DisplayRole);
    //else
        //MagicFile(readfileinfo, secprocjob.objectid);
    //BlockFile(readfileinfo, secprocobj.objectid, adsattrid);
    //PropertyFile(readfileinfo, secprocobj.objectid, fsoffset, readfsinfo->block_size, secprocobj.parfsid);

    /*
    if(readfileinfo->fs_info->ftype == TSK_FS_TYPE_NTFS_DETECT) // TO AVOID A LOCK WILL NEED TO STORE THIS VALUE UP FRONT AS WELL AHEAD OF TIME
    {
        if(QString::compare(secprocobj.name, ".") == 0 || QString::compare(secprocobj.name, "..") == 0)
        {
        }
        else
        {
            //for(int i = 0; i < adsobjid.count(); i++)
            //{
                //AlternateDataStreamMagicFile(readfileinfo, adsobjid.at(i));
            //}
            //AlternateDataStreamBlockFile(readfileinfo, adsobjid, adsattrid);
            //AlternateDataStreamPropertyFile(readfileinfo, adsobjid, adsattrid);
        }
    }
    */
    filesprocessed++;
    isignals->ProgUpd();
    tsk_fs_file_close(readfileinfo);
    tsk_fs_close(readfsinfo);
}
