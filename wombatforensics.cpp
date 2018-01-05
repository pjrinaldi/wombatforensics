#include "wombatforensics.h"

// Copyright 2015 Pasquale J. Rinaldi, Jr.
// Distrubted under the terms of the GNU General Public License version 2

WombatForensics::WombatForensics(QWidget *parent) : QMainWindow(parent), ui(new Ui::WombatForensics)
{
    ui->setupUi(this);
    //wombatvarptr = &wombatvariable;
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
    //wombatdatabase = new WombatDatabase(wombatvarptr);
    //wombatframework = new WombatFramework(wombatvarptr);
    //propertywindow = new PropertiesWindow(wombatdatabase);
    propertywindow = new PropertiesWindow(this);
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
    cancelthread = new CancelThread(this);
    propertywindow->setWindowIcon(QIcon(":/bar/propview"));
    fileviewer->setWindowIcon(QIcon(":/bar/fileview"));
    imagewindow->setWindowIcon(QIcon(":/bar/bwimageview"));
    textviewer->setWindowIcon(QIcon(":/bar/textencode"));
    msgviewer->setWindowIcon(QIcon(":/bar/logview"));
    byteviewer->setWindowIcon(QIcon(":/bar/byteconverter"));
    aboutbox->setWindowIcon(QIcon(":/bar/about"));
    //cancelthread->setWindowIcon(QIcon(""));
    filtervalues.maxcreate = QDateTime::currentDateTimeUtc().toTime_t();
    filtervalues.mincreate = QDateTime::currentDateTimeUtc().toTime_t();
    filtervalues.maxaccess = QDateTime::currentDateTimeUtc().toTime_t();
    filtervalues.minaccess = QDateTime::currentDateTimeUtc().toTime_t();
    filtervalues.maxmodify = QDateTime::currentDateTimeUtc().toTime_t();
    filtervalues.minmodify = QDateTime::currentDateTimeUtc().toTime_t();
    filtervalues.maxchange = QDateTime::currentDateTimeUtc().toTime_t();
    filtervalues.minchange = QDateTime::currentDateTimeUtc().toTime_t();
    qRegisterMetaType<QVector<int> >();
    //qRegisterMetaType<QVector<WombatVariable> >();
    connect(imagewindow->sb, SIGNAL(valueChanged(int)), this, SLOT(UpdateThumbnails(int)), Qt::QueuedConnection);
    connect(imagewindow, SIGNAL(HideImageWindow(bool)), this, SLOT(HideImageWindow(bool)), Qt::DirectConnection);
    connect(textviewer, SIGNAL(HideTextViewerWindow(bool)), this, SLOT(HideTextViewer(bool)), Qt::DirectConnection);
    connect(msgviewer, SIGNAL(HideMessageViewerWindow(bool)), this, SLOT(HideMessageViewer(bool)), Qt::DirectConnection);
    connect(byteviewer, SIGNAL(HideByteConverterWindow(bool)), this, SLOT(HideByteViewer(bool)), Qt::DirectConnection);
    connect(propertywindow, SIGNAL(HidePropertyWindow(bool)), this, SLOT(HidePropertyWindow(bool)), Qt::DirectConnection);
    connect(fileviewer, SIGNAL(HideFileViewer(bool)), this, SLOT(HideFileViewer(bool)), Qt::DirectConnection);
    connect(isignals, SIGNAL(ProgressUpdate(unsigned long long, unsigned long long)), this, SLOT(UpdateProgress(unsigned long long, unsigned long long)), Qt::QueuedConnection);
    //wombatvariable.caseobject.id = 0;
    //wombatvarptr->caseobject.id = 0;
    //connect(wombatdatabase, SIGNAL(DisplayError(QString, QString, QString)), this, SLOT(DisplayError(QString, QString, QString)), Qt::DirectConnection);
    propertywindow->setModal(false);
    InitializeAppStructure();
    //connect(&sqlwatcher, SIGNAL(finished()), this, SLOT(InitializeQueryModel()), Qt::QueuedConnection);
    //connect(isignals, SIGNAL(FinishSql()), this, SLOT(UpdateStatus()), Qt::QueuedConnection);
    //connect(&secondwatcher, SIGNAL(finished()), this, SLOT(UpdateStatus()), Qt::QueuedConnection);
    connect(cancelthread, SIGNAL(CancelCurrentThread()), &secondwatcher, SLOT(cancel()), Qt::QueuedConnection);
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
    //for(int i=0; i < externallist.count(); i++)
    viewerfile.open(QIODevice::ReadOnly);
    QStringList itemlist = QString(viewerfile.readLine()).split(",", QString::SkipEmptyParts);
    itemlist.removeDuplicates();
    viewerfile.close();
    for(int i=0; i < itemlist.count(); i++)
    {
        QAction* tmpaction = new QAction(itemlist.at(i), this);
        connect(tmpaction, SIGNAL(triggered()), this, SLOT(ShowExternalViewer()));
        ui->menuView_With->addAction(tmpaction);
    }
    treemenu->addAction(ui->menuView_With->menuAction());
    treemenu->addAction(ui->actionCheck);
    treemenu->addAction(ui->actionDigDeeper);
    treemenu->addAction(ui->actionExport);
    treemodel = new TreeModel(this);
    ui->dirTreeView->setModel(treemodel);
    /*
    ui->dirTreeView->hideColumn(4);
    ui->dirTreeView->hideColumn(5);
    ui->dirTreeView->hideColumn(11);
    ui->dirTreeView->hideColumn(12);
    ui->dirTreeView->hideColumn(13);
    ui->dirTreeView->hideColumn(14);
    ui->dirTreeView->hideColumn(15);
    ui->dirTreeView->hideColumn(18);
    ui->dirTreeView->hideColumn(19);
    */
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
    //autosavetimer = new QTimer(this);
    //connect(autosavetimer, SIGNAL(timeout()), this, SLOT(AutoSaveState()));
}
//////////////////////////////////////////////////////////////
void WombatForensics::ShowExternalViewer()
{
    unsigned long long filelength = 0;
    unsigned long long curobjaddr = selectedindex.sibling(selectedindex.row(), 0).data().toString().split("-f").at(1).toULongLong();
    QString tmpstr = "";
    QStringList evidlist;
    evidlist.clear();
    std::vector<std::string> pathvector;
    pathvector.clear();
    QDir eviddir = QDir(wombatvariable.tmpmntpath);
    QStringList evidfiles = eviddir.entryList(QStringList("*.evid." + selectedindex.sibling(selectedindex.row(), 0).data().toString().split("-").at(0).mid(1)), QDir::NoSymLinks | QDir::Files);
    wombatvariable.evidenceobject.name = evidfiles.at(0);
    QFile evidfile(wombatvariable.tmpmntpath + wombatvariable.evidenceobject.name.split(".evid").at(0) + ".evid." + selectedindex.sibling(selectedindex.row(), 0).data().toString().split("-").at(0).mid(1));
    evidfile.open(QIODevice::ReadOnly);
    tmpstr = evidfile.readLine();
    evidlist = tmpstr.split(",");
    tskexternalptr->partcount = evidlist.at(3).split("|").size();
    evidfile.close();
    for(int i = 0; i < evidlist.at(3).split("|").size(); i++)
        pathvector.push_back(evidlist.at(3).split("|").at(i).toStdString());
    tskexternalptr->imagepartspath = (const char**)malloc(pathvector.size()*sizeof(char*));
    for(int i = 0; i < pathvector.size(); i++)
        tskexternalptr->imagepartspath[i] = pathvector[i].c_str();
    tskexternalptr->readimginfo = tsk_img_open(tskexternalptr->partcount, tskexternalptr->imagepartspath, TSK_IMG_TYPE_DETECT, 0);
    if(tskexternalptr->readimginfo == NULL)
    {
        qDebug() << tsk_error_get_errstr();
        LogMessage("Image opening error");
    }
    free(tskexternalptr->imagepartspath);
    tmpstr = "";
    QStringList partlist;
    partlist.clear();
    QStringList partfiles = eviddir.entryList(QStringList(wombatvariable.evidenceobject.name.split(".evid").at(0) + ".part." + selectedindex.sibling(selectedindex.row(), 0).data().toString().split("-").at(2).mid(1)), QDir::NoSymLinks | QDir::Files);
    QFile partfile(wombatvariable.tmpmntpath + partfiles.at(0));
    partfile.open(QIODevice::ReadOnly);
    tmpstr = partfile.readLine();
    partfile.close();
    partlist = tmpstr.split(",");
    tskexternalptr->readfsinfo = tsk_fs_open_img(tskexternalptr->readimginfo, partlist.at(4).toULongLong(), TSK_FS_TYPE_DETECT);
    tskexternalptr->readfileinfo = tsk_fs_file_open_meta(tskexternalptr->readfsinfo, NULL, curobjaddr);
    // ReadFileToImageUsingByteArray
    ssize_t filelen = 0;
    //char tbuffer[tskptr->readfileinfo->meta->size];
    if(tskexternalptr->readfileinfo->meta != NULL)
    {
        char* ibuffer = new char[tskexternalptr->readfileinfo->meta->size];
        // WILL NEED TO FIGURE OUT IF ITS AN ATTRIBUTE OR NOT AND HANDLE THE IF BELOW...
        //if(objtype == 5)
        filelen = tsk_fs_file_read(tskexternalptr->readfileinfo, 0, ibuffer, tskexternalptr->readfileinfo->meta->size, TSK_FS_FILE_READ_FLAG_NONE);
        //else
        //    filelen = tsk_fs_file_read_type(tskexternalptr->readfileinfo, TSK_FS_ATTR_TYPE_NTFS_DATA, mftattrid, 0, ibuffer, filelength, TSK_FS_FILE_READ_FLAG_NONE);
    //QString tmpstring = wombatvarptr->tmpfilepath + QString::number(wombatvarptr->selectedobject.id) + "-tmp";
        QString tmpstring = wombatvariable.tmpfilepath + selectedindex.sibling(selectedindex.row(), 0).data().toString() + "-tmp";
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
    else
        qDebug() << "no file meta for file size";

    /*
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
    */
}

void WombatForensics::SetSelectedFromImageViewer(unsigned long long objectid)
{
    QModelIndexList indexlist = ((TreeModel*)ui->dirTreeView->model())->match(((TreeModel*)ui->dirTreeView->model())->index(0, 0, QModelIndex()), Qt::DisplayRole, QVariant(objectid), -1, Qt::MatchFlags(Qt::MatchExactly | Qt::MatchRecursive));
    if(indexlist.count() > 0)
        ui->dirTreeView->setCurrentIndex(indexlist.at(0));
    //else
       //DisplayError("!", "The Image Path Not Discovered", "The image has not been loaded in the treeview yet, so it cannot be selected."); 
}

////////////////////////////////////////////////////////////////////////////////////////
void WombatForensics::ShowFile(const QModelIndex &index)
{
    if(index.sibling(index.row(), 9).data().toString().contains("image/"))
    {
        imagewindow->ShowImage(index);
    }
    else if(index.sibling(index.row(), 9).data().toString().contains("video/"))
    {
        videowindow->ShowVideo(index);
        //videowindow->ShowVideo(wombatvarptr->tmpfilepath, index);
    }
    else if(index.sibling(index.row(), 9).data().toString().contains("text/plain"))
    {
        // toggle the button...
        ui->actionTextViewer->setChecked(true);
        textviewer->ShowText(index);
    }
    else if(index.sibling(index.row(), 9).data().toString().contains("text/html"))
    {
        htmlviewer->ShowHtml(index);
    }
    else
    {
        //else if(wombatvariable.selectedobject.modid.split("-").count() == 4) // file file
        //if(index.sibling(index.row(), 4).data().toInt() == 5 || index.sibling(index.row(), 4).data().toInt() == 6)
        if(index.sibling(index.row(), 0).data().toString().split("-").count() == 4) // file
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
    viewerfile.open(QIODevice::ReadOnly);
    QStringList itemlist = QString(viewerfile.readLine()).split(",", QString::SkipEmptyParts);
    itemlist.removeDuplicates();
    viewerfile.close();
    for(int i=0; i < itemlist.count(); i++)
    {
        QAction* tmpaction = new QAction(itemlist.at(i), this);
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

void WombatForensics::CreateAppDB()
{
    /*
    if(fappdb.open())
    {
        fappdb.transaction();
        QSqlQuery appquery(fappdb);
        appquery.exec("CREATE TABLE cases(caseid INTEGER PRIMARY KEY, name TEXT, creation TEXT, deleted INTEGER NOT NULL DEFAULT 0);");
        appquery.exec("CREATE TABLE external viewers(viewerid INTEGER PRIMARY KEY, path TEXT, deleted INTEGER NOT NULL DEFAULT 0);");
        fappdb.commit();
        appquery.finish();
    }
    else
        DisplayError("1.6", "Create App DB Error", fappdb.lastError().text());
    */
}

void WombatForensics::OpenAppDB()
{
    /*
    if(!fappdb.isOpen())
        fappdb.open();
    */
}

unsigned long long WombatForensics::ReturnCaseCount()
{
    unsigned long long a = 0;
    casesfile.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream in(&casesfile);
    while(!in.atEnd())
    {
        QString line = in.readLine();
        a++;
    }
    //qDebug() << "a" << a;
    casesfile.close();
    return a;
    /*
    QSqlQuery countquery("SELECT COUNT(caseid) FROM cases WHERE deleted = 0;", fappdb);
    if(countquery.first())
        return countquery.value(0).toULongLong();
    */
}

void WombatForensics::InitializeAppStructure()
{
    wombatid = 1;
    wombatvariable.iscaseopen = false;
    QString homepath = QDir::homePath();
    // changing storing settings and such from /WombatForensics/ to /.wombatforensics/
    //homepath += "/WombatForensics/";
    homepath += "/.wombatforensics/";
    //wombatvariable.homepath = homepath; // may not need
    // settings are now just stored in .wombatforensics
    //wombatvariable.settingspath = homepath + "settings/";
    // there would be no data now.
    //wombatvariable.datapath = homepath + "data/";
    // there would be no cases now
    //wombatvariable.casespath = homepath + "cases/";
    // tmp files would still exist.
    wombatvariable.tmpfilepath = homepath + "tmpfiles/";
    wombatvariable.tmpmntpath = homepath + "mntpt/";
    /*
    if((new QDir())->mkpath(wombatvariable.settingspath) == false)
        DisplayError("1.0", "App Settings Folder Failed.", "App Settings Folder was not created.");
    if((new QDir())->mkpath(wombatvariable.datapath) == false)
        DisplayError("1.1", "App Data Folder Failed", "App Data Folder was not created");
    if((new QDir())->mkpath(wombatvariable.casespath) == false)
        DisplayError("1.2", "App Cases Folder Failed", "App Cases Folder was not created");
    */
    if((new QDir())->mkpath(wombatvariable.tmpfilepath) == false)
        DisplayError("1.3", "App tmpfile folder failed", "App Tmpfile folder was not created");
    if((new QDir())->mkpath(wombatvariable.tmpmntpath) == false)
        DisplayError("1.2", "App tmpmnt folder failed", "App tmpmnt folder was not created");
    // there would be no WombatApp.db anymore
    //wombatvariable.wombatdbname = wombatvariable.datapath + "WombatApp.db";
    // SHOULDN'T CALL THESE UNTIL I ACTUALLY USE THEM...
    //wombatvariable.settingfilename = wombatvariable.settingspath + "settings";
    //wombatvariable.viewerfilename = wombatvariable.settingspath + "viewers";
    // shouldn't need this anymore.
    //wombatvariable.casesfilename = wombatvariable.datapath + "cases";
    //fappdb = QSqlDatabase::addDatabase("QSQLITE", "appdb");
    //fappdb.setDatabaseName(wombatvariable.wombatdbname);
    // shouldn't need this anymore
    //casesfile.setFileName(wombatvariable.casesfilename);
    //settingsfile.setFileName(wombatvariable.settingfilename);
    //viewerfile.setFileName(wombatvariable.viewerfilename);
    /*
    if(!FileExists(wombatvariable.wombatdbname.toStdString()))
    {
        CreateAppDB(); // used to be wombatdatabase
        if(wombatvariable.curerrmsg.compare("") != 0)
            DisplayError("1.4", "Create App DB Error", wombatvariable.curerrmsg);
    }
    else
    {
        OpenAppDB(); // used to be wombatdatabase
        if(wombatvariable.curerrmsg.compare("") != 0)
            DisplayError("1.5", "Open App DB Error", wombatvariable.curerrmsg);
    }
    */
    viewerfile.setFileName(homepath + "viewers");
    if(!FileExists(QString(homepath + "viewers").toStdString()))
    {
        viewerfile.open(QIODevice::WriteOnly | QIODevice::Text);
        viewerfile.close();
    }
    //qDebug() << "initialize filename:" << viewerfile.fileName();
    viewmanage = new ViewerManager(this);
    viewmanage->setWindowIcon(QIcon(":/bar/viewermanager"));
    connect(viewmanage, SIGNAL(HideManagerWindow()), this, SLOT(HideViewerManager()), Qt::DirectConnection);
    // shouldn't need open to be disabled cause i'm not managing that anymore.
    //ui->actionOpen_Case->setEnabled(false);
    // shouldn't need the case count information...
    /*
    if(ReturnCaseCount() > 0) // used to be wombatdatabase
    {
        ui->actionOpen_Case->setEnabled(true);
    }
    */
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
    ui->actionHtmlViewer->setVisible(false);
    ui->actionMediaViewer->setVisible(false);
    QList<int> sizelist;
    sizelist.append(height()/2);
    sizelist.append(height()/2);
    ui->splitter->setSizes(sizelist);
    SetupHexPage();
}
// NO LONGER NEEDED
void WombatForensics::InsertCase()
{
    /*
    QSqlQuery casequery(fappdb);
    casequery.prepare("INSERT INTO cases (name, creation) VALUES(?, ?);");
    casequery.bindValue(0, wombatvariable.caseobject.name);
    casequery.bindValue(1, QString::fromStdString(GetTime()));
    casequery.exec();
    wombatvariable.caseobject.id = casequery.lastInsertId().toULongLong();
    casequery.finish();
    qDebug() << "caseid:" << wombatvariable.caseobject.id;
    */
    if(ReturnCaseCount() > 0)
        wombatvariable.caseobject.id = ReturnCaseCount();
    else
        wombatvariable.caseobject.id = 1;
    casesfile.open(QIODevice::Append | QIODevice::Text);
    QTextStream out(&casesfile);
    out << wombatvariable.caseobject.id << "," << wombatvariable.caseobject.name;
    casesfile.close();
}
//NO LONGER NEEDED
void WombatForensics::CreateThumbDB()
{
    /*
    if(thumbdb.open())
    {
        QSqlQuery thumbquery(thumbdb);
        thumbquery.exec("CREATE TABLE thumbs(thumbid INTEGER PRIMARY KEY, objectid INTEGER, thumbblob TEXT);");
        thumbquery.finish();
    }
    else
    {
        LogMessage(thumbdb.lastError().text());
    }
    */
}
//NO LONGER NEEDED
void WombatForensics::OpenThumbDB()
{
    /*
    if(!thumbdb.isOpen())
        thumbdb.open();
    */
}
// NO LONGER NEEDED
void WombatForensics::CreateCaseDB()
{
    /*
    #define IMGDB_CHUNK_SIZE 1024*1024*1 // what size chunks should the database use when growing and shrinking
    #define IMGDB_MAX_RETRY_COUNT 50    // how many times will we retry a SQL statement
    #define IMGDB_RETRY_WAIT 100   // how long (in milliseconds) are we willing to wait between retries
    QStringList wombattableschema;
    wombattableschema.clear();
    wombattableschema << "CREATE TABLE settings(settingid INTEGER PRIMARY KEY, name TEXT, value TEXT, type INTEGER);";
    wombattableschema << "CREATE TABLE dataruns(id INTEGER PRIMARY KEY, objectid INTEGER, fullpath TEXT, seqnum INTEGER, start INTEGER, length INTEGER, datattype INTEGER, originalsectstart INTEGER, allocationstatus INTEGER);";
    wombattableschema << "CREATE TABLE attributes(id INTEGER PRIMARY KEY, objectid INTEGER, context TEXT, attrtype INTEGER, valuetype INTEGER value BLOB);";
    wombattableschema << "CREATE TABLE properties(id INTEGER PRIMARY KEY, objectid INTEGER, name TEXT, description TEXT, value BLOB);";
    wombattableschema << "CREATE TABLE data(id INTEGER PRIMARY KEY, objtype INTEGER, type INTEGER, size INTEGER, name TEXT, fullpath TEXT, addr INTEGER, parid INTEGER, parimgid INTEGER, parfsid INTEGER, offset INTEGER, sectsize INTEGER, sectstart INTEGER, sectlength INTEGER, blocksize INTEGER, blockcount INTEGER, ctime INTEGER, crtime INTEGER, atime INTEGER, mtime INTEGER, md5 TEXT, filemime TEXT, known INTEGER, mftattrid INTEGER, checked INTEGER NOT NULL DEFAULT 0)";
    //wombattableschema << "CREATE TABLE data(objectid INTEGER PRIMARY KEY, objecttype INTEGER, type INTEGER, name TEXT, fullpath TEXT, parentid INTEGER, parimgid INTEGER, parfsid INTEGER, flags INTEGER, childcount INTEGER, address INTEGER, size INTEGER, sectsize INTEGER, sectstart INTEGER, sectlength INTEGER, ctime INTEGER, crtime INTEGER, atime INTEGER, mtime INTEGER, status INTEGER, md5 TEXT, sha1 TEXT, sha_256 TEXT, sha_512 TEXT, filesignature TEXT, filemime TEXT, known INTEGER, blockaddress TEXT, mftattrid INTEGER, mftattrtype INTEGER, byteoffset INTEGER, blocksize INTEGER, blockcount INTEGER, rootinum INTEGER, firstinum INTEGER, lastinum INTEGER, derivationdetails TEXT, checked INTEGER NOT NULL DEFAULT 0);";
    if(!fcasedb.isOpen())
        fcasedb.open();
    fcasedb.transaction();
    QSqlQuery casequery(fcasedb);
    for(int i=0; i < wombattableschema.count(); i++)
    {
        casequery.exec(wombattableschema.at(i));
    }
    fcasedb.commit();
    casequery.finish();
    */
}
// NO LONGER NEEDED
void WombatForensics::OpenCaseDB()
{
    /*
    if(!fcasedb.isOpen())
        fcasedb.open();
    */
}

void WombatForensics::InitializeCaseStructure()
{
    StatusUpdate("Generating Case Structure...");
    // create new case here
    wombatvariable.caseobject.name = QFileDialog::getSaveFileName(this, tr("Create New Case File"), QDir::homePath(), tr("WombatForensics Case (*.wfc)"));
    if(!wombatvariable.caseobject.name.isEmpty())
    {
        QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
        QStringList tmplist = wombatvariable.caseobject.name.split("/");
        tmplist.removeLast();
        wombatvariable.caseobject.dirpath = tmplist.join("/");
        //qDebug() << wombatvariable.caseobject.dirpath;
        // create log file here
        //this->setWindowTitle(QString("Wombat Forensics - ") + wombatvariable.caseobject.name); // update application window
        if(!wombatvariable.caseobject.name.contains(".wfc"))
        {
            this->setWindowTitle(QString("Wombat Forensics - ") + wombatvariable.caseobject.name.split("/").last());
            wombatvariable.caseobject.name += ".wfc";
        }
        else
        {
            this->setWindowTitle(QString("Wombat Forensics - ") + wombatvariable.caseobject.name.split("/").last().split(".").first());
        }
        QFile casefile(wombatvariable.caseobject.name);
        casefile.open(QIODevice::ReadWrite);
        casefile.resize(1000000000);
        casefile.close();
        QString mkfsstr = "mkfs.btrfs -q ";
        mkfsstr += wombatvariable.caseobject.name;
        QString name = qgetenv("USER");
        if(name.isEmpty())
            name = qgetenv("USERNAME");
        /*
        //pkexec calls the required gui prompt for sudo or a terminal if gui not available.
        QString mntstr = "pkexec mount -o loop ";
        mntstr += wombatvariable.caseobject.name;
        mntstr += " ";
        mntstr += wombatvariable.tmpmntpath;
        qDebug() << mntstr;
        QString chownstr = "pkexec chown -R " + name + ":" + name + " " + wombatvariable.tmpmntpath;
        QProcess::execute(mkfsstr);
        QProcess::execute(mntstr);
        QProcess::execute(chownstr);
        */
        // POSSIBLE PKEXEC WORKAROUND - works without privilege escalation, just a little slower...
        // now requires guestmount which is a part of libguestfs
        QProcess::execute(mkfsstr);
        QString mntstr = "guestmount -a " + wombatvariable.caseobject.name + " -m /dev/sda " + wombatvariable.tmpmntpath;
        QProcess::execute(mntstr);
        wombatvariable.iscaseopen = true;
        logfile.setFileName(wombatvariable.tmpmntpath + "msglog");
        msglog->clear();
        LogMessage("Log File Created");
        //viewerfile.setFileName(wombatvariable.tmpmntpath + "viewers");
        //viewerfile.open(QIODevice::WriteOnly | QIODevice::Text);
        //viewerfile.close();
        //LogMessage("External Viewer File Created");
        ui->actionAdd_Evidence->setEnabled(true);
        LogMessage("Case was Created");
        QApplication::restoreOverrideCursor();
        StatusUpdate("Ready");
        //autosavetimer->start(10000); // 10 seconds in milliseconds for testing purposes
        //autosavetimer->start(600000); // 10 minutes in milliseconds for a general setting for real.

        // this works with privilege escalation. only way to mount it though. now requires linux and btrfs.
    }
    /*
        thumbdb = QSqlDatabase::database("thumbdb");
        if(!thumbdb.isValid())
            thumbdb = QSqlDatabase::addDatabase("QSQLITE", "thumbdb");
        thumbdb.setDatabaseName(wombatvariable.caseobject.dirpath + "thumbs.db");
        if(!FileExists((wombatvariable.caseobject.dirpath + "thumbs.db").toStdString()))
        {
            CreateThumbDB();
            if(wombatvariable.curerrmsg.compare("") != 0)
                DisplayError("2.1", "Thumb DB Error", wombatvariable.curerrmsg);
        }
        else
        {
            OpenThumbDB();
            if(wombatvariable.curerrmsg.compare("") != 0)
                DisplayError("2.2", "Thumb DB Error", wombatvariable.curerrmsg);
        }
        */
}

void WombatForensics::InitializeOpenCase()
{
    wombatvariable.caseobject.name = QFileDialog::getOpenFileName(this, tr("Open Existing Case"), QDir::homePath(), tr("WombatForensics Case (*.wfc)"));
    if(!wombatvariable.caseobject.name.isEmpty())
    {
        StatusUpdate("Case Opening...");
        QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
        QStringList tmplist = wombatvariable.caseobject.name.split("/");
        tmplist.removeLast();
        wombatvariable.caseobject.dirpath = tmplist.join("/");
        //logfile.setFileName(wombatvariable.caseobject.dirpath + "/msglog");
        logfile.setFileName(wombatvariable.tmpmntpath + "msglog");
        //viewerfile.setFileName(wombatvariable.tmpmntpath + "viewers");
        if(!wombatvariable.caseobject.name.contains(".wfc"))
        {
            this->setWindowTitle(QString("Wombat Forensics - ") + wombatvariable.caseobject.name.split("/").last());
            wombatvariable.caseobject.name += ".wfc";
        }
        else
        {
            this->setWindowTitle(QString("Wombat Forensics - ") + wombatvariable.caseobject.name.split("/").last().split(".").first());
        }
        QFile casefile(wombatvariable.caseobject.name);
        QString name = qgetenv("USER");
        if(name.isEmpty())
            name = qgetenv("USERNAME");
        //qDebug() << name;
        /*
        QString mntstr = "pkexec mount -o loop ";
        mntstr += wombatvariable.caseobject.name;
        mntstr += " ";
        mntstr += wombatvariable.tmpmntpath;
        QString chownstr = "pkexec chown -R " + name + ":" + name + " " + wombatvariable.tmpmntpath;
        QProcess::execute(mntstr);
        QProcess::execute(chownstr);
        */
        //QProcess::execute(mkfsstr); // don't need since i'm opening an existing case, not creating a new case.
        QString mntstr = "guestmount -a " + wombatvariable.caseobject.name + " -m /dev/sda " + wombatvariable.tmpmntpath;
        QProcess* openprocess = new QProcess(this);
        connect(openprocess, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(OpenCaseMountFinished(int, QProcess::ExitStatus)));
        openprocess->start(mntstr);
        //QProcess::execute(mntstr);
        /*
        wombatvariable.iscaseopen = true;
        ui->actionAdd_Evidence->setEnabled(true);
        LogMessage("Case was Opened");
        //autosavetimer->start(10000); // 10 seconds in milliseconds for testing purposes
        //autosavetimer->start(600000); // 10 minutes in milliseconds for a general setting for real.
        QDir eviddir = QDir(wombatvariable.tmpmntpath);
        QStringList files = eviddir.entryList(QStringList(QString("*.evid.*")), QDir::Files | QDir::NoSymLinks);
        for(int i=0; i < files.count(); i++)
        {
            wombatvariable.evidenceobject.name = QFile(files.at(i)).fileName().split(".").at(0) + QString(".") + QFile(files.at(i)).fileName().split(".").at(1);

            treemodel->AddEvidence();
            evidcnt++;
        }
        ui->dirTreeView->setCurrentIndex(treemodel->index(0, 0, QModelIndex()));
        if(ui->dirTreeView->model() != NULL)
        {
            ui->actionRemove_Evidence->setEnabled(true);
            ui->actionSaveState->setEnabled(true);
            ui->actionDigDeeper->setEnabled(true);
            hexrocker->setEnabled(true);
            //StatusUpdate("Opening Case Evidence...");
        }
        QApplication::restoreOverrideCursor();
        */
    }
    /*
        thumbdb = QSqlDatabase::database("thumbdb");
        if(!thumbdb.isValid())
            thumbdb = QSqlDatabase::addDatabase("QSQLITE", "thumbdb");
        thumbdb.setDatabaseName(wombatvariable.caseobject.dirpath + "thumbs.db");
        if(!FileExists((wombatvariable.caseobject.dirpath + "thumbs.db").toStdString()))
        {
            CreateThumbDB();
            if(wombatvariable.curerrmsg.compare("") != 0)
                DisplayError("4.1", "Thumb DB Error", wombatvariable.curerrmsg);
        }
        else
        {
            OpenThumbDB();
            if(wombatvariable.curerrmsg.compare("") != 0)
                DisplayError("4.2", "Thumb DB Open Error", wombatvariable.curerrmsg);
        }
        processcountlabel->setText("Processed: 0");
        filecountlabel->setText("Files: 0");
        QSqlQuery evidquery(fcasedb);
        evidquery.prepare("SELECT id, name FROM data WHERE objtype = 1");
        evidquery.exec();
        while(evidquery.next())
        {
            fsobjectlist.clear();
            FileSystemObject tmpobject;
            QSqlQuery fsquery(fcasedb);
            fsquery.prepare("SELECT id, addr FROM data WHERE objtype = 4 AND parimgid = ?");
            fsquery.bindValue(0, evidquery.value(0).toULongLong());
            fsquery.exec();
            while(fsquery.next())
            {
                tmpobject.id = fsquery.value(0).toULongLong();
                tmpobject.rootinum = fsquery.value(1).toULongLong();
                fsobjectlist.append(tmpobject);
            }
            fsquery.finish();
            treemodel->AddEvidence(evidquery.value(0).toULongLong()); 
            wombatvariable.currentevidencename = evidquery.value(1).toString();
            currentevidenceid = evidquery.value(0).toULongLong();
            wombatvariable.evidenceobject.id = evidquery.value(0).toULongLong();
            ui->dirTreeView->setCurrentIndex(treemodel->index(0, 0, QModelIndex()));
            QSqlQuery countquery(fcasedb);
            countquery.prepare("SELECT COUNT(id) FROM data WHERE parimgid = ? AND objtype = 5");
            countquery.bindValue(0, evidquery.value(0).toULongLong());
            countquery.exec();
            countquery.first();
            filesfound = filesfound + countquery.value(0).toULongLong();
            countquery.finish();
            filesprocessed = filesfound;
            filtercountlabel->setText("Filtered: " + QString::number(filesprocessed));
            processcountlabel->setText("Processed: " + QString::number(filesprocessed));
            filecountlabel->setText("Files: " + QString::number(filesfound));
        }
        evidquery.finish();
        //autosavetimer->start(10000); // 10 seconds in milliseconds for testing purposes
        //autosavetimer->start(600000); // 10 minutes in milliseconds
        //wombatdatabase->GetEvidenceObjects();
        if(ui->dirTreeView->model() != NULL)
        {
            ui->actionRemove_Evidence->setEnabled(true);
            ui->actionSaveState->setEnabled(true);
            ui->actionDigDeeper->setEnabled(true);
            hexrocker->setEnabled(true);
            //StatusUpdate("Opening Case Evidence...");
        }
        */
}

void WombatForensics::OpenCaseMountFinished(int exitcode, QProcess::ExitStatus exitstatus)
{
    wombatvariable.iscaseopen = true;
    ui->actionAdd_Evidence->setEnabled(true);
    LogMessage("Case was Opened");
    //autosavetimer->start(10000); // 10 seconds in milliseconds for testing purposes
    //autosavetimer->start(600000); // 10 minutes in milliseconds for a general setting for real.
    QDir eviddir = QDir(wombatvariable.tmpmntpath);
    QStringList files = eviddir.entryList(QStringList(QString("*.evid.*")), QDir::Files | QDir::NoSymLinks);
    qDebug() << "file list:" << files;
    for(int i=0; i < files.count(); i++)
    {
        wombatvariable.evidenceobject.name = QFile(files.at(i)).fileName().split(".").at(0) + QString(".") + QFile(files.at(i)).fileName().split(".").at(1);
        qDebug() << "evidencename:" << wombatvariable.evidenceobject.name;
        treemodel->AddEvidence();
        evidcnt++;
    }
    ui->dirTreeView->setCurrentIndex(treemodel->index(0, 0, QModelIndex()));
    if(ui->dirTreeView->model() != NULL)
    {
        ui->actionRemove_Evidence->setEnabled(true);
        ui->actionSaveState->setEnabled(true);
        ui->actionDigDeeper->setEnabled(true);
        hexrocker->setEnabled(true);
        //StatusUpdate("Opening Case Evidence...");
    }
    QApplication::restoreOverrideCursor();
    LogMessage("Case was Opened Successfully");
    StatusUpdate("Ready");
}

void WombatForensics::InitializeQueryModel()
{
    StatusUpdate("Building Initial Evidence Tree...");
    LogMessage("Building Initial Evidence Tree...");
    //qDebug() << "filedatevector count: " << filedatavector.count();
    //secondwatcher.setFuture(QtConcurrent::map(filedatavector, FileMap));
    //connect(secondwatcher, SIGNAL(finished()), isignals, SLOT(FinishSql()), Qt::QueuedConnection);
    //secondwatcher.setFuture(QtConcurrent::map(filedatavector, SqlMap));
    /*
     *
     *
     *
     *
    //connect(secondwatcher, SIGNAL(finished()), isignals, SLOT(FinishSql()), Qt::QueuedConnection);
    //secondwatcher.setFuture(QtConcurrent::map(filedatavector, SqlMap));

     /*
     *
     *
     *
     *
     *
     *
     *
    //ResizeColumns();
    //StatusUpdate("Evidence Added. Begin File Structure Analysis...");
    //LogMessage("Evidence Added. Begin File Structure Analysis...");
    //StatusUpdate("File Structure Analysis Finished. Begin Secondary Processing...");
    //LogMessage("File Structure Analysis Finished. Begin Secondary Processing...");
    /*
    secondprocessvector.clear();
    //jsonstorevector.clear();
    //secondprocessvector.adsprocessvector.clear();
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
            */
            /*
            QVariantMap tmpmap;
            tmpmap.insert("objectid", filequery.value(0));
            tmpmap.insert("parimgid", filequery.value(1));
            tmpmap.insert("parfsid", filequery.value(2));
            tmpmap.insert("address", filequery.value(3));
            tmpmap.insert("name", filequery.value(4));
            jsonstorevector.append(tmpmap);
            */
        /*}
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
    */
    //secondwatcher.setFuture(QtConcurrent::map(jsonstorevector, SecondaryProcessing));
    //secondwatcher.setFuture(QtConcurrent::map(secondprocessvector, SecondaryProcessing));
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
    //StatusUpdate("Evidence ready");
    //LogMessage("Evidence Ready");
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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
        //wombatvariable.selectedobject.modid = selectedindex.sibling(selectedindex.row(), 0).data().toString(); // mod id
        LoadHexContents();
        if(propertywindow->isVisible())
            UpdateProperties();
        //wombatvarptr->selectedobject.id = selectedindex.sibling(selectedindex.row(), 0).data().toULongLong(); // object id
        //wombatvarptr->selectedobject.name = selectedindex.sibling(selectedindex.row(), 1).data().toString(); // object name
        //wombatdatabase->GetObjectValues(); // now i have selectedobject.values.
        //LoadHexContents();
        //if(propertywindow->isVisible())
        //    UpdateProperties();
    }
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void WombatForensics::TreeContextMenu(const QPoint &pt)
{
    QModelIndex index = ui->dirTreeView->indexAt(pt);
    if(index.isValid())
    {
        actionnode = static_cast<Node*>(index.internalPointer());
        //if(index.sibling(index.row(), 4).data().toInt() == 5 || index.sibling(index.row(), 4).data().toInt() == 6)
        if(index.sibling(index.row(), 0).data().toString().split("-").count() == 4) // file
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

void WombatForensics::AddNewEvidence()
{
    const TSK_TCHAR** images;
    images = (const char**)malloc(wombatvariable.evidenceobject.fullpathvector.size()*sizeof(char*));
    for(uint i=0; i < wombatvariable.evidenceobject.fullpathvector.size(); i++)
    {
        images[i] = wombatvariable.evidenceobject.fullpathvector[i].c_str();
    }
    readimginfo = tsk_img_open(wombatvariable.evidenceobject.itemcount, images, TSK_IMG_TYPE_DETECT, 0);
    if(readimginfo == NULL)
    {
        LogMessage("Evidence Image acess failed");
        errorcount++;
    }
    free(images);
    fsobjectlist.clear();
    QFile evidfile(wombatvariable.caseobject.dirpath + wombatvariable.evidenceobject.name + ".evid." + QString::number(evidcnt));
    evidfile.open(QIODevice::Append | QIODevice::Text);
    QTextStream out(&evidfile);
    out << (int)readimginfo->itype << "," << (unsigned long long)readimginfo->size << "," << (int)readimginfo->sector_size;
    for(unsigned int i=0; i < wombatvariable.evidenceobject.itemcount; i++)
        out << QString::fromStdString(wombatvariable.evidenceobject.fullpathvector[i]) << "," << i+1;
    evidfile.close();

    /*
    QSqlQuery evidquery(fcasedb);
    evidquery.prepare("INSERT INTO data (objtype, type, size, name, fullpath, sectsize) VALUES (1, ?, ?, ?, ?, ?)");
    evidquery.bindValue(0, (int)readimginfo->itype);
    evidquery.bindValue(1, (unsigned long long)readimginfo->size);
    evidquery.bindValue(2, wombatvariable.evidenceobject.name);
    evidquery.bindValue(3, QString::fromStdString(wombatvariable.evidenceobject.fullpathvector[0]));
    evidquery.bindValue(4, (int)readimginfo->sector_size);
    evidquery.exec();
    wombatvariable.evidenceobject.id = evidquery.lastInsertId().toULongLong();
    evidquery.finish();
    currentevidenceid = wombatvariable.evidenceobject.id;
    for(unsigned int i=0; i < wombatvariable.evidenceobject.itemcount; i++)
    {
        QSqlQuery runquery(fcasedb);
        runquery.prepare("INSERT INTO dataruns (objectid, fullpath, seqnum) VALUES (?, ?, ?)");
        runquery.bindValue(0, wombatvariable.evidenceobject.id);
        runquery.bindValue(1, QString::fromStdString(wombatvariable.evidenceobject.fullpathvector[i]));
        runquery.bindValue(2, i+1);
        runquery.exec();
        runquery.finish();
    }
    // finished initial evidence information, now onto the volume information...
    */
    readvsinfo = tsk_vs_open(readimginfo, 0, TSK_VS_TYPE_DETECT);
    QString volname = "Dummy Volume";
    int voltype = 240;
    int volsectorsize = (int)readimginfo->sector_size;
    unsigned long long voloffset = 0;
    if(readvsinfo != NULL)
    {
        voltype = (int)readvsinfo->vstype;
        volname = QString::fromUtf8(tsk_vs_type_todesc(readvsinfo->vstype));
        volsectorsize = (int)readvsinfo->block_size;
        voloffset = (unsigned long long)readvsinfo->offset;
    }

    QFile volfile(wombatvariable.caseobject.dirpath + wombatvariable.evidenceobject.name + ".vol");
    volfile.open(QIODevice::Append | QIODevice::Text);
    out.setDevice(&volfile);
    out << voltype << "," << (unsigned long long)readimginfo->size << "," << volname << "," << volsectorsize << "," << voloffset;
    volfile.close();
    /*
    QSqlQuery volquery(fcasedb);
    volquery.prepare("INSERT INTO data (objtype, type, size, parid, parimgid, name, offset, sectsize) VALUES (2, ?, ?, ?, ?, ?, ?, ?)");
    if(readvsinfo == NULL)
        volquery.bindValue(0, 240);
    else
        volquery.bindValue(0, (int)readvsinfo->vstype);
    volquery.bindValue(1, (unsigned long long)readimginfo->size);
    volquery.bindValue(2, wombatvariable.evidenceobject.id);
    volquery.bindValue(3, wombatvariable.evidenceobject.id);
    if(readvsinfo == NULL)
    {
        volquery.bindValue(4, QString("Dummy Volume"));
        volquery.bindValue(5, 0);
        volquery.bindValue(6, (int)readimginfo->sector_size);
    }
    else
    {
        volquery.bindValue(4, QString::fromUtf8(tsk_vs_type_todesc(readvsinfo->vstype)));
        volquery.bindValue(5, (unsigned long long)readvsinfo->offset);
        volquery.bindValue(6, (int)readvsinfo->block_size);
    }
    volquery.exec();
    wombatvariable.currentvolumeid = volquery.lastInsertId().toULongLong();
    volquery.finish();
    */
    // finished initial volume system information, now onto the file parition information...
    if(readvsinfo == NULL) // No volume, so a single file system is all there is in the image.
    {
        readfsinfo = tsk_fs_open_img(readimginfo, 0, TSK_FS_TYPE_DETECT);
        QFile pfile(wombatvariable.caseobject.dirpath + wombatvariable.evidenceobject.name + ".part.0");
        pfile.open(QIODevice::Append | QIODevice::Text);
        out.setDevice(&pfile);
        out << readfsinfo->ftype << "," << (unsigned long long)readfsinfo->block_size * (unsigned long long)readfsinfo->block_count << "," << GetFileSystemLabel(readfsinfo) << "," << (unsigned long long)readfsinfo->root_inum << "," << (unsigned long long)readfsinfo->offset << "," << (unsigned long long)readfsinfo->block_count << "," << (unsigned long long)readfsinfo->block_size;
        pfile.close();
        /*
        QSqlQuery fsquery(fcasedb);
        fsquery.prepare("INSERT INTO data (objtype, type, size, parid, parimgid, name, fullpath, addr, offset, sectstart, sectlength, sectsize, blocksize, blockcount) VALUES (4, ?, ?, ?, ?, ?, '/', ?, ?, ?, ?, ?, ?, ?)");
        fsquery.bindValue(0, readfsinfo->ftype);
        fsquery.bindValue(1, (unsigned long long)readfsinfo->block_size * (unsigned long long)readfsinfo->block_count);
        fsquery.bindValue(2, wombatvariable.currentvolumeid);
        fsquery.bindValue(3, wombatvariable.evidenceobject.id);
        fsquery.bindValue(4, GetFileSystemLabel(readfsinfo));
        fsquery.bindValue(5, (unsigned long long)readfsinfo->root_inum);
        fsquery.bindValue(6, (unsigned long long)readfsinfo->offset);
        fsquery.bindValue(7, (unsigned long long)readfsinfo->offset);
        fsquery.bindValue(8, (unsigned long long)readfsinfo->block_count);
        fsquery.bindValue(9, (int)readfsinfo->block_size);
        fsquery.bindValue(10, (int)readfsinfo->block_size);
        fsquery.bindValue(11, (unsigned long long)readfsinfo->block_count);
        fsquery.exec();
        currentfilesystemid = fsquery.lastInsertId().toULongLong();
        fsquery.finish();
        */

        /*
        FileSystemObject tmpobject;
        tmpobject.id = currentfilesystemid;
        tmpobject.rootinum = readfsinfo->root_inum;
        fsobjectlist.append(tmpobject);
        */
        uint8_t walkreturn;
        int walkflags = TSK_FS_DIR_WALK_FLAG_ALLOC | TSK_FS_DIR_WALK_FLAG_UNALLOC | TSK_FS_DIR_WALK_FLAG_RECURSE;
        walkreturn = tsk_fs_dir_walk(readfsinfo, readfsinfo->root_inum, (TSK_FS_DIR_WALK_FLAG_ENUM)walkflags, FileEntries, NULL);
    }
    else
    {
        QFile pfile;
        if(readvsinfo->part_count > 0)
        {
            for(uint32_t i=0; i < readvsinfo->part_count; i++)
            {
                readpartinfo = tsk_vs_part_get(readvsinfo, i);
                pfile.setFileName(wombatvariable.caseobject.dirpath + wombatvariable.evidenceobject.name + ".part." + QString::number(i));
                pfile.open(QIODevice::Append | QIODevice::Text);
                out.setDevice(&pfile);
                if(readpartinfo->flags == 0x02) // unallocated partition
                {
                    out << readpartinfo->desc << "," << readpartinfo->flags << "," << (unsigned long long)readpartinfo->len * readvsinfo->block_size << "," << (unsigned long long)readpartinfo->start << "," << (unsigned long long)readpartinfo->len << "," << (int)readvsinfo->block_size;
                    /*
                    QSqlQuery partquery(fcasedb);
                    partquery.prepare("INSERT INTO data (objtype, name, parid, parimgid, size, sectstart, sectlength, sectsize) VALUES(3, ?, ?, ?, ?, ?, ?, ?)");
                    partquery.bindValue(0, readpartinfo->desc);
                    partquery.bindValue(1, wombatvariable.currentvolumeid);
                    partquery.bindValue(2, wombatvariable.evidenceobject.id);
                    partquery.bindValue(3, (unsigned long long)readpartinfo->len * readvsinfo->block_size);
                    partquery.bindValue(4, (unsigned long long)readpartinfo->start);
                    partquery.bindValue(5, (unsigned long long)readpartinfo->len);
                    partquery.bindValue(6, (int)readvsinfo->block_size);
                    partquery.exec();
                    partquery.finish();
                    */
                }
                else if(readpartinfo->flags == 0x01) // allocated partition
                {
                    readfsinfo = tsk_fs_open_vol(readpartinfo, TSK_FS_TYPE_DETECT);
                    if(readfsinfo != NULL)
                    {
                        out << GetFileSystemLabel(readfsinfo) << "," << readpartinfo->flags << "," << readfsinfo->ftype << "," << (unsigned long long)readfsinfo->block_size * (unsigned long long)readfsinfo->block_count << "," << (unsigned long long)readfsinfo->root_inum << "," << (unsigned long long)readfsinfo->offset << "," << (unsigned long long)readpartinfo->start << "," << (unsigned long long)readpartinfo->len << "," << (int)readfsinfo->dev_bsize << "," << (int)readfsinfo->block_size << "," << (unsigned long long)readfsinfo->block_count;
                        /*
                        QSqlQuery fsquery(fcasedb);
                        fsquery.prepare("INSERT INTO data (objtype, name, fullpath, type, parid, parimgid, size, addr, offset, sectstart, sectlength, sectsize, blocksize, blockcount) VALUES(4, ?, '/', ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");
                        fsquery.bindValue(0, GetFileSystemLabel(readfsinfo));
                        fsquery.bindValue(1, readfsinfo->ftype);
                        fsquery.bindValue(2, wombatvariable.currentvolumeid);
                        fsquery.bindValue(3, wombatvariable.evidenceobject.id);
                        fsquery.bindValue(4, (unsigned long long)readfsinfo->block_size * (unsigned long long)readfsinfo->block_count);
                        fsquery.bindValue(5, (unsigned long long)readfsinfo->root_inum);
                        fsquery.bindValue(6, (unsigned long long)readfsinfo->offset);
                        fsquery.bindValue(7, (unsigned long long)readpartinfo->start);
                        fsquery.bindValue(8, (unsigned long long)readpartinfo->len);
                        fsquery.bindValue(9, (int)readfsinfo->dev_bsize);
                        fsquery.bindValue(10, (int)readfsinfo->block_size);
                        fsquery.bindValue(11, (unsigned long long)readfsinfo->block_count);
                        fsquery.exec();
                        currentfilesystemid = fsquery.lastInsertId().toULongLong();
                        fsquery.finish();
                        */
                        /*
                        FileSystemObject tmpobject;
                        tmpobject.id = currentfilesystemid;
                        tmpobject.rootinum = readfsinfo->root_inum;
                        fsobjectlist.append(tmpobject);
                        */
                        uint8_t walkreturn;
                        int walkflags = TSK_FS_DIR_WALK_FLAG_ALLOC | TSK_FS_DIR_WALK_FLAG_UNALLOC | TSK_FS_DIR_WALK_FLAG_RECURSE;
                        walkreturn = tsk_fs_dir_walk(readfsinfo, readfsinfo->root_inum, (TSK_FS_DIR_WALK_FLAG_ENUM)walkflags, FileEntries, NULL);
                        if(walkreturn == 1)
                        {
                            LogMessage("Issues with traversing the file structure were encountered");
                            errorcount++;
                        }
                    }
                }
                pfile.close();
            }
        }
    }
    // finished initial partition/file system information including file info stored into the vector 
}
/*
void WombatForensics::InitializeEvidenceStructure()
{
    //WombatVariable wombatvariable = wombatvarvector.at(0);
    readimginfo = NULL;
    readvsinfo = NULL;
    readfsinfo = NULL;
    readfileinfo = NULL;
    //AddNewEvidence();
    /*
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
    */
/*
    const TSK_TCHAR** images;
    images = (const char**)malloc(wombatvariable.evidenceobject.fullpathvector.size()*sizeof(char*));
    for(uint i=0; i < wombatvariable.evidenceobject.fullpathvector.size(); i++)
    {
        images[i] = wombatvariable.evidenceobject.fullpathvector[i].c_str();
    }
    readimginfo = tsk_img_open(wombatvariable.evidenceobject.itemcount, images, TSK_IMG_TYPE_DETECT, 0);
    if(readimginfo == NULL)
    {
        LogMessage("Evidence Image acess failed");
        errorcount++;
    }
    free(images);
    fsobjectlist.clear();
    // PUT FILES IN THE SPARSE FILE NOW.
    QFile evidfile(wombatvariable.tmpmntpath + wombatvariable.evidenceobject.name + ".evid");
    //QFile evidfile(wombatvariable.caseobject.dirpath + wombatvariable.evidenceobject.name + ".evid");
    evidfile.open(QIODevice::Append | QIODevice::Text);
    QTextStream out(&evidfile);
    out << (int)readimginfo->itype << "," << (unsigned long long)readimginfo->size << "," << (int)readimginfo->sector_size;
    for(unsigned int i=0; i < wombatvariable.evidenceobject.itemcount; i++)
        out << QString::fromStdString(wombatvariable.evidenceobject.fullpathvector[i]) << "," << i+1;
    evidfile.close();
    readvsinfo = tsk_vs_open(readimginfo, 0, TSK_VS_TYPE_DETECT);
    QString volname = "Dummy Volume";
    int voltype = 240;
    int volsectorsize = (int)readimginfo->sector_size;
    unsigned long long voloffset = 0;
    if(readvsinfo != NULL)
    {
        voltype = (int)readvsinfo->vstype;
        volname = QString::fromUtf8(tsk_vs_type_todesc(readvsinfo->vstype));
        volsectorsize = (int)readvsinfo->block_size;
        voloffset = (unsigned long long)readvsinfo->offset;
    }

    QFile volfile(wombatvariable.tmpmntpath + wombatvariable.evidenceobject.name + ".vol");
    //QFile volfile(wombatvariable.caseobject.dirpath + wombatvariable.evidenceobject.name + ".vol");
    volfile.open(QIODevice::Append | QIODevice::Text);
    out.setDevice(&volfile);
    out << voltype << "," << (unsigned long long)readimginfo->size << "," << volname << "," << volsectorsize << "," << voloffset;
    volfile.close();
    if(readvsinfo == NULL) // No volume, so a single file system is all there is in the image.
    {
        readfsinfo = tsk_fs_open_img(readimginfo, 0, TSK_FS_TYPE_DETECT);
        QFile pfile(wombatvariable.tmpmntpath + wombatvariable.evidenceobject.name + "p1");
        //QFile pfile(wombatvariable.caseobject.dirpath + wombatvariable.evidenceobject.name + ".p1");
        pfile.open(QIODevice::Append | QIODevice::Text);
        out.setDevice(&pfile);
        out << readfsinfo->ftype << "," << (unsigned long long)readfsinfo->block_size * (unsigned long long)readfsinfo->block_count << "," << GetFileSystemLabel(readfsinfo) << "," << (unsigned long long)readfsinfo->root_inum << "," << (unsigned long long)readfsinfo->offset << "," << (unsigned long long)readfsinfo->block_count << "," << (unsigned long long)readfsinfo->block_size;
        pfile.close();
        uint8_t walkreturn;
        int walkflags = TSK_FS_DIR_WALK_FLAG_ALLOC | TSK_FS_DIR_WALK_FLAG_UNALLOC | TSK_FS_DIR_WALK_FLAG_RECURSE;
        walkreturn = tsk_fs_dir_walk(readfsinfo, readfsinfo->root_inum, (TSK_FS_DIR_WALK_FLAG_ENUM)walkflags, FileEntries, NULL);
    }
    else
    {
        QFile pfile;
        if(readvsinfo->part_count > 0)
        {
            for(uint32_t i=0; i < readvsinfo->part_count; i++)
            {
                readpartinfo = tsk_vs_part_get(readvsinfo, i);
                pfile.setFileName(wombatvariable.tmpmntpath + wombatvariable.evidenceobject.name + ".p" + QString::number(i));
                //pfile.setFileName(wombatvariable.caseobject.dirpath + wombatvariable.evidenceobject.name + ".p" + QString::number(i));
                pfile.open(QIODevice::Append | QIODevice::Text);
                out.setDevice(&pfile);
                if(readpartinfo->flags == 0x02) // unallocated partition
                {
                    out << readpartinfo->desc << "," << readpartinfo->flags << "," << (unsigned long long)readpartinfo->len * readvsinfo->block_size << "," << (unsigned long long)readpartinfo->start << "," << (unsigned long long)readpartinfo->len << "," << (int)readvsinfo->block_size;
                }
                else if(readpartinfo->flags == 0x01) // allocated partition
                {
                    readfsinfo = tsk_fs_open_vol(readpartinfo, TSK_FS_TYPE_DETECT);
                    if(readfsinfo != NULL)
                    {
                        out << GetFileSystemLabel(readfsinfo) << "," << readpartinfo->flags << "," << readfsinfo->ftype << "," << (unsigned long long)readfsinfo->block_size * (unsigned long long)readfsinfo->block_count << "," << (unsigned long long)readfsinfo->root_inum << "," << (unsigned long long)readfsinfo->offset << "," << (unsigned long long)readpartinfo->start << "," << (unsigned long long)readpartinfo->len << "," << (int)readfsinfo->dev_bsize << "," << (int)readfsinfo->block_size << "," << (unsigned long long)readfsinfo->block_count;
                        uint8_t walkreturn;
                        int walkflags = TSK_FS_DIR_WALK_FLAG_ALLOC | TSK_FS_DIR_WALK_FLAG_UNALLOC | TSK_FS_DIR_WALK_FLAG_RECURSE;
                        walkreturn = tsk_fs_dir_walk(readfsinfo, readfsinfo->root_inum, (TSK_FS_DIR_WALK_FLAG_ENUM)walkflags, FileEntries, NULL);
                        if(walkreturn == 1)
                        {
                            LogMessage("Issues with traversing the file structure were encountered");
                            errorcount++;
                        }
                    }
                }
                pfile.close();
            }
        }
    }
            // finished initial partition/file system information including file info stored into the vector 
            // place InitializeQueryModel(); in here...

}
*/

void WombatForensics::UpdateDataTable()
{
    StatusUpdate("Processing Complete");
    LogMessage("Evidence has been successfully added");
    StatusUpdate("Evidence ready");
}
void WombatForensics::UpdateStatus()
{
    StatusUpdate("Building Initial Evidence Tree...");
    LogMessage("Building Initial Evidence Tree...");
    //tsk_img_close(IMG_2ND_PROC);
    filedatavector.clear();
    readfileinfo = NULL;
    tsk_fs_close(readfsinfo);
    readfsinfo = NULL;
    tsk_vs_close(readvsinfo);
    readvsinfo = NULL;
    tsk_img_close(readimginfo);
    readimginfo = NULL;
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
    treemodel->AddEvidence();
    QApplication::restoreOverrideCursor();
    ui->dirTreeView->setCurrentIndex(treemodel->index(0, 0, QModelIndex()));
    evidcnt++;
    volcnt = 0;
    partint = 0;
    //treemodel->AddEvidence(wombatvariable.evidenceobject.id);
    //ui->dirTreeView->setCurrentIndex(treemodel->index(0, 0, QModelIndex()));
    ui->actionRemove_Evidence->setEnabled(true);
    ui->actionSaveState->setEnabled(true);
    ui->actionDigDeeper->setEnabled(true);
    hexrocker->setEnabled(true);
    //ResizeColumns();
    cancelthread->close();
    LogMessage("Processing Complete.");
    StatusUpdate("Evidence ready");
    //wombatframework->CloseInfoStructures();
}

void WombatForensics::UpdateDigging()
{
    //ResizeColumns();
    LogMessage("Digging Complete");
    StatusUpdate("Evidence ready");
}

void WombatForensics::OpenEvidenceStructure()
{
    /*
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
    */
}

/*
void WombatForensics::GetEvidenceObjects()
{
    wombatptr->evidenceobjectvector.clear();
    wombatptr->evidenceobject.Clear();
    wombatptr->bindvalues.clear();
    wombatptr->sqlrecords.clear();
    wombatptr->sqlrecords = GetSqlResults("SELECT objectid, objecttype, type, size, name, fullpath FROM data WHERE objecttype = 1;", wombatptr->bindvalues);
    //wombatptr->sqlrecords = GetSqlResults("SELECT objectid, objecttype, type, size, sectsize, name, fullpath FROM data WHERE objecttype = 1;", wombatptr->bindvalues);
    for(int i=0; i < wombatptr->sqlrecords.count(); i++)
    {
        wombatptr->evidenceobject.id = wombatptr->sqlrecords[i].value(0).toULongLong();
        currentevidenceid = wombatptr->evidenceobject.id;
        wombatptr->evidenceobject.objecttype = wombatptr->sqlrecords[i].value(1).toInt();
        wombatptr->evidenceobject.type = wombatptr->sqlrecords[i].value(2).toInt();
        wombatptr->evidenceobject.size = wombatptr->sqlrecords[i].value(3).toULongLong();
        //wombatptr->evidenceobject.sectsize = wombatptr->sqlrecords[i].value(4).toInt();
        wombatptr->evidenceobject.name = wombatptr->sqlrecords[i].value(5).toString();
        wombatptr->evidenceobject.fullpath = wombatptr->sqlrecords[i].value(6).toString();
        wombatptr->evidenceobject.parimgid = 0;
        wombatptr->evidenceobjectvector.append(wombatptr->evidenceobject);
    }
    for(int i=0; i < wombatptr->evidenceobjectvector.count(); i++)
    {
        wombatptr->bindvalues.clear();
        wombatptr->sqlrecords.clear();
        wombatptr->bindvalues.append(wombatptr->evidenceobjectvector[i].id);
        wombatptr->sqlrecords = GetSqlResults("SELECT fullpath FROM dataruns WHERE objectid = ? ORDER BY seqnum", wombatptr->bindvalues);
        for(int j=0; j < wombatptr->sqlrecords.count(); j++)
            wombatptr->evidenceobjectvector[i].fullpathvector.push_back(wombatptr->sqlrecords[j].value(0).toString().toStdString());
        wombatptr->evidenceobjectvector[i].itemcount = wombatptr->evidenceobjectvector.at(i).fullpathvector.size();
    }
}
*/

void WombatForensics::AddEvidence()
{
    //QList<WombatVariable> wombatvarvector;
    int isnew = 1;
    QStringList tmplist = QFileDialog::getOpenFileNames(this, tr("Select Evidence Image(s)"), QDir::homePath());
    if(tmplist.count())
    {
        wombatvariable.evidenceobject.name = tmplist.at(0).split("/").last();
        /*
        QSqlQuery evidquery(fcasedb);
        evidquery.prepare("SELECT fullpath FROM data WHERE objtype = 1");
        evidquery.exec();
        while(evidquery.next())
        {
            if(wombatvariable.evidenceobject.name.compare(evidquery.value(0).toString().split("/").last()) == 0)
                isnew = 0;
        }
        evidquery.finish();
        */
        if(isnew == 1)
        {
            for(int i=0; i < tmplist.count(); i++)
                wombatvariable.evidenceobject.fullpathvector.push_back(tmplist.at(i).toStdString());
            wombatvariable.evidenceobject.itemcount = tmplist.count();
            LogMessage("Start Adding Evidence");
            // TRY A QTCONCURRENT::MAP() WITH A 1 ITEM VECTOR SO I CAN CANCEL IT...
            connect(&sqlwatcher, SIGNAL(finished()), this, SLOT(UpdateStatus()), Qt::QueuedConnection);
            //connect(&sqlwatcher, SIGNAL(finished()), this, SLOT(InitializeQueryModel()), Qt::QueuedConnection);
            connect(cancelthread, SIGNAL(CancelCurrentThread()), &sqlwatcher, SLOT(cancel()), Qt::QueuedConnection);
            wombatvarvector.append(wombatvariable);
            //qDebug() << wombatvarvector.at(0).evidenceobject.name;
            //InitializeEvidenceStructure(wombatvariable);
            sqlwatcher.setFuture(QtConcurrent::map(wombatvarvector, InitializeEvidenceStructure));
            cancelthread->show();
            /*
            evidencethread = new QThread;
            evidenceworker = new EvidenceWorker();
            evidenceworker->moveToThread(evidencethread);
            //connect(evidenceworker, SIGNAL(error(QString)), this, SLOT(errorString(QString)));
            connect(evidencethread, SIGNAL(started()), evidenceworker, SLOT(process()));
            connect(evidenceworker, SIGNAL(finished()), evidencethread, SLOT(quit()));
            //connect(evidenceworker, SIGNAL(finished()), this, SLOT(InitializeQueryModel()), Qt::QueuedConnection);
            // need to link to the next step....
            //connect(&sqlwatcher, SIGNAL(finished()), this, SLOT(InitializeQueryModel()), Qt::QueuedConnection);
            connect(evidenceworker, SIGNAL(finished()), evidenceworker, SLOT(deleteLater()));
            connect(evidencethread, SIGNAL(finished()), evidencethread, SLOT(deleteLater()));
            evidencethread->start();
            //sqlwatcher.setFuture(QtConcurrent::run(this, &WombatForensics::InitializeEvidenceStructure));
            */
        }
        else
            DisplayError("3.0", "Evidence already exists in the case", "Add Evidence Cancelled");
    }
    /*
    int isnew = 1;
    //wombatdatabase->GetEvidenceObjects();
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
    */
}

/////////////////////////////////////////////////////
void WombatForensics::UpdateProperties()
{
    //wombatvariable.selectedobject.modid = selectedindex.sibling(selectedindex.row(), 0).data().toString(); // mod id
    //wombatdatabase->ReturnObjectPropertyList();
    QFile propfile;
    propertylist.clear();
    if(selectedindex.sibling(selectedindex.row(), 0).data().toString().split("-").count() == 1) // evidence image
    {
        propfile.setFileName(wombatvariable.tmpmntpath + selectedindex.sibling(selectedindex.row(), 1).data().toString().split("evid").at(0) + ".eprop." + selectedindex.sibling(selectedindex.row(), 0).data().toString().mid(1));
    }
    QDir eviddir = QDir(wombatvariable.tmpmntpath);
    QStringList evidfiles = eviddir.entryList(QStringList("*.evid." + selectedindex.sibling(selectedindex.row(), 0).data().toString().split("-").at(0).mid(1)), QDir::NoSymLinks | QDir::Files);
    QString evidfilename = evidfiles.at(0).split(".evid").at(0);
    //qDebug() << evidfiles.at(0);
    QFile evidfile(wombatvariable.tmpmntpath + wombatvariable.evidenceobject.name.split(".evid").at(0) + ".evid." + wombatvariable.selectedobject.modid.split("-").at(0).mid(1));
    if(selectedindex.sibling(selectedindex.row(), 0).data().toString().split("-").count() == 2) // volume
    {
        propfile.setFileName(wombatvariable.tmpmntpath + evidfilename + ".volprop");
    }
    if(selectedindex.sibling(selectedindex.row(), 0).data().toString().split("-").count() == 3) // file system
    {
        propfile.setFileName(wombatvariable.tmpmntpath + evidfilename + ".partprop." + selectedindex.sibling(selectedindex.row(), 0).data().toString().split("-").last());
    }
    if(selectedindex.sibling(selectedindex.row(), 0).data().toString().split("-").count() == 4) // file
    {
        propfile.setFileName(wombatvariable.tmpmntpath + evidfilename + "." + selectedindex.sibling(selectedindex.row(), 0).data().toString().split("-").at(2) + "." + selectedindex.sibling(selectedindex.row(), 0).data().toString().split("-").last() + ".prop");
    }
    qDebug() << propfile.fileName();
    propfile.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream in(&propfile);
    while(!in.atEnd())
    {
        QString line = in.readLine();
        propertylist << line.split("||").at(0) << line.split("||").at(1) << line.split("||").at(2);
    }
    propfile.close();
    propertywindow->UpdateTableView();
}

void WombatForensics::LoadHexContents()
{
    if(tskobjptr->readimginfo != NULL)
    {
        tsk_img_close(tskobjptr->readimginfo);
        tskobjptr->readimginfo = NULL;
    }
    if(tskobjptr->readfsinfo != NULL)
    {
        tsk_fs_close(tskobjptr->readfsinfo);
    }
    if(tskobjptr->readfileinfo != NULL)
    {
        tsk_fs_file_close(tskobjptr->readfileinfo);
    }
    wombatvariable.selectedobject.modid = selectedindex.sibling(selectedindex.row(), 0).data().toString(); // mod object id
    //qDebug() << "- count:" << wombatvariable.selectedobject.modid.split("-").count();
    //wombatvariable.selectedobject.id = selectedindex.sibling(selectedindex.row(), 0).data().toULongLong(); // object id
    //wombatvariable.selectedobject.size = selectedindex.sibling(selectedindex.row(), 3).data().toULongLong(); // object size
    //wombatvariable.selectedobject.name = selectedindex.sibling(selectedindex.row(), 1).data().toString(); // object name
    blockstring = "";
    QString tmpstr = "";
    QStringList evidlist;
    evidlist.clear();
    if(wombatvariable.selectedobject.modid.split("-").count() == 1) // image file
    {
        wombatvariable.evidenceobject.name = selectedindex.sibling(selectedindex.row(), 1).data().toString(); // current evidence name
        //OpenParentImage();
        QFile evidfile(wombatvariable.tmpmntpath + wombatvariable.evidenceobject.name + ".evid." + wombatvariable.selectedobject.modid.mid(1));
        evidfile.open(QIODevice::ReadOnly);
        tmpstr = evidfile.readLine();
        evidlist = tmpstr.split(",");
        std::vector<std::string> tmpvec;
        tmpvec.clear();
        evidfile.close();
        tskobjptr->offset = 0;
        tskobjptr->objecttype = 1;
        tskobjptr->length = selectedindex.sibling(selectedindex.row(), 3).data().toULongLong(); // object size
        tskobjptr->imglength = selectedindex.sibling(selectedindex.row(), 3).data().toULongLong(); // image size
        tskobjptr->sectsize = evidlist.at(2).toInt();
        tskobjptr->blocksize = evidlist.at(2).toInt();
        tskobjptr->partcount = evidlist.at(3).split("|").size();
        for(int i = 0; i < evidlist.at(3).split("|").size(); i++)
        {
            tmpvec.push_back(evidlist.at(3).split("|").at(i).toStdString());
        }
        tskobjptr->imagepartspath = (const char**)malloc(tmpvec.size()*sizeof(char*));
        for(int i =0; i < tmpvec.size(); i++)
        {
            tskobjptr->imagepartspath[i] = tmpvec[i].c_str();
        }
        tskobjptr->readimginfo = tsk_img_open(tskobjptr->partcount, tskobjptr->imagepartspath, TSK_IMG_TYPE_DETECT, 0);
        if(tskobjptr->readimginfo == NULL)
        {
            qDebug() << tsk_error_get_errstr();
            LogMessage("Image opening error");
        }
        free(tskobjptr->imagepartspath);
    }
    else if(wombatvariable.selectedobject.modid.split("-").count() == 2) // volume file
    {
        QDir eviddir = QDir(wombatvariable.tmpmntpath);
        QStringList evidfiles = eviddir.entryList(QStringList("*.evid." + wombatvariable.selectedobject.modid.split("-").at(0).mid(1)), QDir::NoSymLinks | QDir::Files);
        wombatvariable.evidenceobject.name = evidfiles.at(0);
        //qDebug() << evidfiles.at(0);

        QFile evidfile(wombatvariable.tmpmntpath + wombatvariable.evidenceobject.name.split(".evid").at(0) + ".evid." + wombatvariable.selectedobject.modid.split("-").at(0).mid(1));
        evidfile.open(QIODevice::ReadOnly);
        tmpstr = evidfile.readLine();
        evidlist = tmpstr.split(",");
        evidfile.close();
        std::vector<std::string> tmpvec;
        tmpvec.clear();
        for(int i = 0; i < evidlist.at(3).split("|").size(); i++)
        {
            tmpvec.push_back(evidlist.at(3).split("|").at(i).toStdString());
        }
        tskobjptr->imagepartspath = (const char**)malloc(tmpvec.size()*sizeof(char*));
        for(int i =0; i < tmpvec.size(); i++)
        {
            tskobjptr->imagepartspath[i] = tmpvec[i].c_str();
        }
        tskobjptr->readimginfo = tsk_img_open(tskobjptr->partcount, tskobjptr->imagepartspath, TSK_IMG_TYPE_DETECT, 0);
        if(tskobjptr->readimginfo == NULL)
        {
            qDebug() << tsk_error_get_errstr();
            LogMessage("Image opening error");
        }
        free(tskobjptr->imagepartspath); 
        //OpenParentImage();
        tmpstr = "";
        QStringList vollist;
        vollist.clear();
        QFile volfile(wombatvariable.tmpmntpath + wombatvariable.evidenceobject.name.split(".evid").at(0) + ".vol");
        volfile.open(QIODevice::ReadOnly);
        tmpstr = volfile.readLine();
        volfile.close();
        vollist = tmpstr.split(",");
        tskobjptr->offset = vollist.at(4).toInt();
        tskobjptr->length = selectedindex.sibling(selectedindex.row(), 3).data().toULongLong();
        tskobjptr->imglength = selectedindex.sibling(selectedindex.row(), 3).data().toULongLong();
        tskobjptr->sectsize = vollist.at(3).toInt();
        tskobjptr->blocksize = vollist.at(3).toInt();
    }
    else if(wombatvariable.selectedobject.modid.split("-").count() == 3) // partition file
    {
        QDir eviddir = QDir(wombatvariable.tmpmntpath);
        QStringList evidfiles = eviddir.entryList(QStringList("*.evid." + wombatvariable.selectedobject.modid.split("-").at(0).mid(1)), QDir::NoSymLinks | QDir::Files);
        wombatvariable.evidenceobject.name = evidfiles.at(0);
        //qDebug() << evidfiles.at(0);

        QFile evidfile(wombatvariable.tmpmntpath + wombatvariable.evidenceobject.name.split(".evid").at(0) + ".evid." + wombatvariable.selectedobject.modid.split("-").at(0).mid(1));
        evidfile.open(QIODevice::ReadOnly);
        tmpstr = evidfile.readLine();
        evidlist = tmpstr.split(",");
        evidfile.close();
        std::vector<std::string> tmpvec;
        tmpvec.clear();
        for(int i = 0; i < evidlist.at(3).split("|").size(); i++)
        {
            tmpvec.push_back(evidlist.at(3).split("|").at(i).toStdString());
        }
        tskobjptr->imagepartspath = (const char**)malloc(tmpvec.size()*sizeof(char*));
        for(int i =0; i < tmpvec.size(); i++)
        {
            tskobjptr->imagepartspath[i] = tmpvec[i].c_str();
        }
        tskobjptr->readimginfo = tsk_img_open(tskobjptr->partcount, tskobjptr->imagepartspath, TSK_IMG_TYPE_DETECT, 0);
        if(tskobjptr->readimginfo == NULL)
        {
            qDebug() << tsk_error_get_errstr();
            LogMessage("Image opening error");
        }
        free(tskobjptr->imagepartspath);
        tmpstr = "";
        QStringList partlist = eviddir.entryList(QStringList(wombatvariable.evidenceobject.name.split(".evid").at(0) + ".part." + wombatvariable.selectedobject.modid.split("-").at(2).mid(1)), QDir::NoSymLinks | QDir::Files);
        QFile partfile(wombatvariable.tmpmntpath + partlist.at(0));
        partfile.open(QIODevice::ReadOnly);
        tmpstr = partfile.readLine();
        partfile.close();
        partlist = tmpstr.split(",");
        tskobjptr->offset = partlist.at(4).toULongLong();
        tskobjptr->fsoffset = partlist.at(4).toULongLong();
        tskobjptr->objecttype = 4;
        tskobjptr->length = partlist.at(1).toULongLong();
        tskobjptr->sectsize = evidlist.at(2).toInt();
        tskobjptr->blocksize = partlist.at(6).toInt();
        //OpenParentImage();
    }
    else if(wombatvariable.selectedobject.modid.split("-").count() == 4) // file file
    {
        QDir eviddir = QDir(wombatvariable.tmpmntpath);
        QStringList evidfiles = eviddir.entryList(QStringList("*.evid." + wombatvariable.selectedobject.modid.split("-").at(0).mid(1)), QDir::NoSymLinks | QDir::Files);
        wombatvariable.evidenceobject.name = evidfiles.at(0);
        //qDebug() << evidfiles.at(0);

        QFile evidfile(wombatvariable.tmpmntpath + wombatvariable.evidenceobject.name.split(".evid").at(0) + ".evid." + wombatvariable.selectedobject.modid.split("-").at(0).mid(1));
        evidfile.open(QIODevice::ReadOnly);
        tmpstr = evidfile.readLine();
        evidlist = tmpstr.split(",");
        tskobjptr->partcount = evidlist.at(3).split("|").size();
        evidfile.close();
        std::vector<std::string> tmpvec;
        tmpvec.clear();
        for(int i = 0; i < evidlist.at(3).split("|").size(); i++)
        {
            tmpvec.push_back(evidlist.at(3).split("|").at(i).toStdString());
        }
        tskobjptr->imagepartspath = (const char**)malloc(tmpvec.size()*sizeof(char*));
        for(int i =0; i < tmpvec.size(); i++)
        {
            tskobjptr->imagepartspath[i] = tmpvec[i].c_str();
        }
        tskobjptr->readimginfo = tsk_img_open(tskobjptr->partcount, tskobjptr->imagepartspath, TSK_IMG_TYPE_DETECT, 0);
        if(tskobjptr->readimginfo == NULL)
        {
            qDebug() << tsk_error_get_errstr();
            LogMessage("Image opening error");
        }
        free(tskobjptr->imagepartspath);
        tmpstr = "";
        QStringList partlist;
        partlist.clear();
        QStringList partfiles = eviddir.entryList(QStringList(wombatvariable.evidenceobject.name.split(".evid").at(0) + ".part." + wombatvariable.selectedobject.modid.split("-").at(2).mid(1)), QDir::NoSymLinks | QDir::Files);
        QFile partfile(wombatvariable.tmpmntpath + partfiles.at(0));
        partfile.open(QIODevice::ReadOnly);
        tmpstr = partfile.readLine();
        partfile.close();
        partlist = tmpstr.split(",");
        //OpenParentImage();
        QStringList filelist;
        filelist.clear();
        QStringList filefiles = eviddir.entryList(QStringList(wombatvariable.evidenceobject.name.split(".evid").at(0) + ".p" + wombatvariable.selectedobject.modid.split("-").at(2).mid(1) + ".f" + wombatvariable.selectedobject.modid.split("-").at(3).mid(1) + ".a*"), QDir::NoSymLinks | QDir::Files);
        //qDebug() << filefiles;
        QFile filefile;
        if(filefiles.count() == 1)
            filefile.setFileName(wombatvariable.tmpmntpath + filefiles.at(0));
        else
        {
            for(int i = 0; i < filefiles.count(); i++)
            {
                if(filefiles.at(i).split(".a").at(1).toInt() == selectedindex.parent().sibling(selectedindex.parent().row(), 0).data().toString().split("-f").at(1).toInt())
                {
                    qDebug() << filefiles.at(i);
                    filefile.setFileName(wombatvariable.tmpmntpath + filefiles.at(i));
                }
            }
        }
        filefile.open(QIODevice::ReadOnly);
        tmpstr = filefile.readLine();
        QFile filefileprop(filefile.fileName().split(".a").at(0) + ".prop");
        QString blockstring;
        QString residentstring;
        QString bytestring;
        //qDebug() << "File Proprety File Name:" << filefileprop.fileName();
        filefile.close();
        filelist = tmpstr.split(",");
        tskobjptr->readfsinfo = tsk_fs_open_img(tskobjptr->readimginfo, partlist.at(4).toULongLong(), TSK_FS_TYPE_DETECT);
        tskobjptr->fsoffset = tskobjptr->readfsinfo->offset;
        tskobjptr->blocksize = tskobjptr->readfsinfo->block_size;
        tskobjptr->objecttype = 5;
        tskobjptr->address = wombatvariable.selectedobject.modid.split("-").at(3).mid(1).toInt();
        tskobjptr->length = selectedindex.sibling(selectedindex.row(), 3).data().toULongLong();
        tskobjptr->offset = 0;
        filefileprop.open(QIODevice::ReadOnly);
        while(!filefileprop.atEnd())
        {
            QString tmpstring = filefileprop.readLine();
            if(tmpstring.contains("Block Address"))
            {
                blockstring = tmpstring.split("||").at(1);
            }
            else if(tmpstring.contains("Resident Offset"))
            {
                residentstring = tmpstring.split("||").at(1);
            }
            else if(tmpstring.contains("Byte Offset"))
            {
                bytestring = tmpstring.split("||").at(1);
            }
        }
        filefileprop.close();
        if(blockstring.compare("") != 0)
        {
            tskobjptr->offset = bytestring.toULongLong();
        }
        else
        {
            if(tskobjptr->readfsinfo->ftype == TSK_FS_TYPE_NTFS_DETECT)
            {
                tskobjptr->resoffset = residentstring.toULongLong();
                tskobjptr->offset = tskobjptr->resoffset + tskobjptr->fsoffset;
            }
            else
                tskobjptr->offset = tskobjptr->fsoffset;
        }
        tskobjptr->blockaddress = blockstring;
        tskobjptr->blkaddrlist = blockstring.split("^^", QString::SkipEmptyParts);

        /* BEGIN old ads code
         *
         *
         *
         *  if(wombatvarptr->selectedobject.blockaddress.compare("") != 0)
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
            tskobjptr->blockaddress = wombatvarptr->selectedobject.blockaddress;
            tskobjptr->blkaddrlist = wombatvarptr->selectedobject.blockaddress.split("|", QString::SkipEmptyParts);
            OpenFileSystemFile(); 
         *
         * END old ads code
         */




        //tskobjptr->blockaddress = wombatvarptr->selectedobject.blockaddress;
        //tskobjptr->blkaddrlist = wombatvarptr->selectedobject.blockaddress.split("|", QString::SkipEmptyParts);
        /*
        if(blockstring.compare("") != 0)
        {
            tskobjptr->offset = blockstring.split("^^", QString::SkipEmptyParts).at(0).toULongLong()*tskobjptr->blocksize + tskobjptr->fsoffset;
        }
        else
        {
            if(tskobjptr->readfsinfo->ftype == TSK_FS_TYPE_NTFS_DETECT)
            {
            }
            else
                tskobjptr->offset = tskobjptr->fsoffset;
        }
        */
        /*
         *
            if(tskobjptr->readfsinfo->ftype == TSK_FS_TYPE_NTFS_DETECT)
            {
                // NEED TO IMPLEMENT PROPERTIES BEFORE I CAN TEST OUT GETTING THE RESOFFSET PROPERLY
                //tskobjptr->resoffset = GetResidentOffset(tskobjptr->address, partlist.at(4).toULongLong());
                //tskobjptr->offset = tskobjptr->resoffset + tskobjptr->fsoffset;
            }
            else
                tskobjptr->offset = tskobjptr->fsoffset;
        }
        //tskobjptr->blockaddress = blockstring; // probably don't need this variable
         *
         */ 
        if(filelist.at(11).toULongLong() == 0)
            tskobjptr->readfileinfo = tsk_fs_file_open_meta(tskobjptr->readfsinfo, NULL, tskobjptr->address);

        QStringList tmpchildren = GetChildFiles("*.a" + wombatvariable.selectedobject.modid.split("-").at(3).mid(1));

        if(tskobjptr->readfsinfo->ftype == TSK_FS_TYPE_NTFS_DETECT)
        {
        }

        //tskobjptr->blockaddress = wombatvarptr->selectedobject.blockaddress;
        //tskobjptr->blkaddrlist = wombatvarptr->selectedobject.blockaddress.split("|", QString::SkipEmptyParts);
        //QVector<unsigned long long> adsobjid;
        //QVector<unsigned long long> adsattrid;
        //adsobjid.clear();
        //adsattrid.clear();

        // REWORK THIS SECTION TO PULL THE FILE'S PROPERTIES TO GET THIS INFORMATION
        // BLOCKLIST, FILE BYTE OFFSET, RESIDENT OFFSET
        //
        // ALSO NEED TO FIGURE OUT ADS STUFF
        
        /*
        if(tskobjptr->readfsinfo->ftype == TSK_FS_TYPE_NTFS_DETECT)
        {
            unsigned long long minads = 1000;
            for(int i = 0; i < tmpchildren.count(); i++)
            {
                QString childstr = "";
                QStringList childlist;
                childlist.clear();
                QFile childfile(wombatvariable.tmpmntpath + tmpchildren.at(i));
                childfile.open(QIODevice::ReadOnly);
                childstr = childfile.readLine();
                childfile.close();
                childlist = childstr.split(",");
                if(childlist.at(11).toULongLong() > 0)
                {
                    if(childlist.at(11).toULongLong() < minads)
                        minads = childlist.at(11).toULongLong();
                }
            }
            //unsigned long long adsattrid = filelist.at(11).toULongLong();
            /*
            if(adsattrid < minads)
                minads = adsattrid;
            */
        /*
            if(tskobjptr->readfileinfo->meta != NULL)
            {
                if(tskobjptr->readfileinfo->meta->attr)
                {
                    int cnt, i;
                    cnt = tsk_fs_file_attr_getsize(tskobjptr->readfileinfo);
                    for(i = 0; i < cnt; i++)
                    {
                        const TSK_FS_ATTR* tmpattr = tsk_fs_file_attr_get_idx(tskobjptr->readfileinfo, i);
                        if(tmpattr->flags & TSK_FS_ATTR_NONRES) // non-resident attribute
                        {
                            if(tmpattr->type == TSK_FS_ATTR_TYPE_NTFS_DATA && tmpattr->id < (int)minads)
                            {
                                tsk_fs_file_walk_type(tskobjptr->readfileinfo, tmpattr->type, tmpattr->id, (TSK_FS_FILE_WALK_FLAG_ENUM)(TSK_FS_FILE_WALK_FLAG_AONLY | TSK_FS_FILE_WALK_FLAG_SLACK), GetBlockAddress, NULL);
                            }
                        }
                    }
                }
            }
        }
        else if(tskobjptr->readfsinfo->ftype == TSK_FS_TYPE_HFS_DETECT)
        {
            tsk_fs_file_walk_type(tskobjptr->readfileinfo, TSK_FS_ATTR_TYPE_HFS_DATA, HFS_FS_ATTR_ID_DATA, (TSK_FS_FILE_WALK_FLAG_ENUM)(TSK_FS_FILE_WALK_FLAG_AONLY | TSK_FS_FILE_WALK_FLAG_SLACK), GetBlockAddress, NULL);
        }
        else if(tskobjptr->readfsinfo->ftype == TSK_FS_TYPE_FAT_DETECT)
        {
            tsk_fs_file_walk(tskobjptr->readfileinfo, (TSK_FS_FILE_WALK_FLAG_ENUM)(TSK_FS_FILE_WALK_FLAG_AONLY | TSK_FS_FILE_WALK_FLAG_SLACK), GetBlockAddress, NULL);
        }
        else if(tskobjptr->readfsinfo->ftype == TSK_FS_TYPE_ISO9660_DETECT)
        {
            iso9660_inode* dinode;
            dinode = (iso9660_inode*)tsk_malloc(sizeof(iso9660_inode));
            iso9660_inode_node* n;
            n = ((ISO_INFO*)tskobjptr->readfileinfo->fs_info)->in_list;
            while(n && (n->inum != tskobjptr->readfileinfo->meta->addr))
                n = n->next;
            if(n)
                memcpy(dinode, &n->inode, sizeof(iso9660_inode));
            int block = tsk_getu32(tskobjptr->readfileinfo->fs_info->endian, dinode->dr.ext_loc_m);
            TSK_OFF_T size = tskobjptr->readfileinfo->meta->size;
            while((int64_t)size > 0)
            {
                blockstring += QString::number(block++) + "^^";
                size -= tskobjptr->readfileinfo->fs_info->block_size;
            }
        }
        else
        {
            tsk_fs_file_walk(tskobjptr->readfileinfo, (TSK_FS_FILE_WALK_FLAG_ENUM)(TSK_FS_FILE_WALK_FLAG_AONLY | TSK_FS_FILE_WALK_FLAG_SLACK), GetBlockAddress, NULL);
        }
        //qDebug() << "blkstring:" << blockstring;
        tskobjptr->blkaddrlist = blockstring.split("^^", QString::SkipEmptyParts);
        if(blockstring.compare("") != 0)
        {
            tskobjptr->offset = blockstring.split("^^", QString::SkipEmptyParts).at(0).toULongLong()*tskobjptr->blocksize + tskobjptr->fsoffset;
        }
        else
        {
            if(tskobjptr->readfsinfo->ftype == TSK_FS_TYPE_NTFS_DETECT)
            {
                // NEED TO IMPLEMENT PROPERTIES BEFORE I CAN TEST OUT GETTING THE RESOFFSET PROPERLY
                //tskobjptr->resoffset = GetResidentOffset(tskobjptr->address, partlist.at(4).toULongLong());
                //tskobjptr->offset = tskobjptr->resoffset + tskobjptr->fsoffset;
            }
            else
                tskobjptr->offset = tskobjptr->fsoffset;
        }
        //tskobjptr->blockaddress = blockstring; // probably don't need this variable
    */
    }
    /*
    else if(wombatvariable.selectedobject.objtype == 6) // ads file object
    {
        OpenParentImage(wombatvariable.selectedobject.parimgid);
        OpenParentFileSystem(wombatvariable.selectedobject.parfsid);
        tskobjptr->blocksize = tskobjptr->readfsinfo->block_size;
        tskobjptr->fsoffset = tskobjptr->readfsinfo->offset;
        tskobjptr->offset = 0;
        tskobjptr->adsoffset = wombatvariable.selectedobject.address;
        tskobjptr->mftattrid = wombatvariable.selectedobject.mftattrid;
        tskobjptr->address = wombatvariable.selectedobject.parentid;
        tskobjptr->objecttype = 6;
        tskobjptr->length = wombatvariable.selectedobject.size;
        OpenFileSystemFile();
        // DO BLOCK ADDRESS STUFF HERE...
        // DO THE REST HERE AS WELL...
    }
*/
    /*
    else if(wombatvarptr->selectedobject.objtype == 5) // file object
    {
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
        tskobjptr->blockaddress = wombatvarptr->selectedobject.blockaddress;
        tskobjptr->blkaddrlist = wombatvarptr->selectedobject.blockaddress.split("|", QString::SkipEmptyParts);
    }
    else if(wombatvarptr->selectedobject.objtype == 6) // ads file object
    {
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
        tskobjptr->blockaddress = wombatvarptr->selectedobject.blockaddress;
        tskobjptr->blkaddrlist = wombatvarptr->selectedobject.blockaddress.split("|", QString::SkipEmptyParts);
        OpenFileSystemFile();
 
         *
         *
         */ 
    //qDebug() << selectedindex.sibling(selectedindex.row(), 0).data().toString().split("-").count();
/*    
    if(wombatvariable.selectedobject.objtype == 1) // image file
    {
        OpenParentImage(wombatvariable.selectedobject.id);
        tskobjptr->offset = 0;
        tskobjptr->objecttype = 1;
        tskobjptr->length = wombatvariable.selectedobject.size;
        tskobjptr->imglength = wombatvariable.selectedobject.size;
        tskobjptr->sectsize = wombatvariable.selectedobject.sectsize;
        tskobjptr->blocksize = wombatvariable.selectedobject.sectsize;
    }
    else if(wombatvariable.selectedobject.objtype == 2) // volume object
    {
        OpenParentImage(wombatvariable.selectedobject.parimgid);
        tskobjptr->offset = wombatvariable.selectedobject.sectstart * wombatvariable.selectedobject.sectsize;
        tskobjptr->length = wombatvariable.selectedobject.size;
        tskobjptr->sectsize = wombatvariable.selectedobject.sectsize;
        tskobjptr->blocksize = wombatvariable.selectedobject.sectsize;
    }
    else if(wombatvariable.selectedobject.objtype == 3) // unallocated partition
    {
        OpenParentImage(wombatvariable.selectedobject.parimgid);
        tskobjptr->offset = wombatvariable.selectedobject.sectstart * wombatvariable.selectedobject.sectsize;
        tskobjptr->length = wombatvariable.selectedobject.sectlength * wombatvariable.selectedobject.sectsize;
        tskobjptr->objecttype = 3;
        tskobjptr->sectsize = wombatvariable.selectedobject.sectsize;
        tskobjptr->blocksize = wombatvariable.selectedobject.sectsize;
    }
    else if(wombatvariable.selectedobject.objtype == 4) // fs object
    {
        OpenParentImage(wombatvariable.selectedobject.parimgid);
        tskobjptr->offset = wombatvariable.selectedobject.offset;
        tskobjptr->fsoffset = wombatvariable.selectedobject.offset;
        tskobjptr->objecttype = 4;
        tskobjptr->length = wombatvariable.selectedobject.size;
        tskobjptr->sectsize = wombatvariable.selectedobject.sectsize;
        tskobjptr->blocksize = wombatvariable.selectedobject.blocksize;
    }
    else if(wombatvariable.selectedobject.objtype == 5) // file object
    {
        OpenParentImage(wombatvariable.selectedobject.parimgid);
        OpenParentFileSystem(wombatvariable.selectedobject.parfsid);
        tskobjptr->fsoffset = tskobjptr->readfsinfo->offset;
        tskobjptr->blocksize = tskobjptr->readfsinfo->block_size;
        tskobjptr->offset = 0;
        tskobjptr->objecttype = 5;
        tskobjptr->address = wombatvariable.selectedobject.address;
        tskobjptr->length = wombatvariable.selectedobject.size;
        OpenFileSystemFile();
        QVector<unsigned long long> adsobjid;
        QVector<unsigned long long> adsattrid;
        adsobjid.clear();
        adsattrid.clear();
        if(tskobjptr->readfsinfo->ftype == TSK_FS_TYPE_NTFS_DETECT)
        {
*/                /*
                QSqlQuery adsquery(fcasedb);
                adsquery.prepare("SELECT objectid, mftattrid FROM data WHERE objecttype = 6 and parentid = ?;");
                adsquery.bindValue(0, wombatvariable.selectedobject.address);
                if(adsquery.exec())
                {
                    while(adsquery.next())
                    {
                        adsobjid.append(adsquery.value(0).toULongLong());
                        adsattrid.append(adsquery.value(1).toULongLong());
                    }
                }
                adsquery.finish();
                */
/*
            }
            unsigned long long minads = 1000;
            for(int i = 0; i < adsattrid.count(); i++)
            {
                if(adsattrid.at(i) < minads)
                    minads = adsattrid.at(i);
            }
            if(tskobjptr->readfileinfo->meta != NULL)
            {
                if(tskobjptr->readfileinfo->meta->attr)
                {
                    int cnt, i;
                    cnt = tsk_fs_file_attr_getsize(tskobjptr->readfileinfo);
                    for(i = 0; i < cnt; i++)
                    {
                        const TSK_FS_ATTR* tmpattr = tsk_fs_file_attr_get_idx(tskobjptr->readfileinfo, i);
                        if(tmpattr->flags & TSK_FS_ATTR_NONRES) // non resident attribute
                        {
                            if(tmpattr->type == TSK_FS_ATTR_TYPE_NTFS_DATA && tmpattr->id < (int)minads)
                            {
                                tsk_fs_file_walk_type(tskobjptr->readfileinfo, tmpattr->type, tmpattr->id, (TSK_FS_FILE_WALK_FLAG_ENUM)(TSK_FS_FILE_WALK_FLAG_AONLY | TSK_FS_FILE_WALK_FLAG_SLACK), GetBlockAddress, NULL);
                            }
                        }
                    }
                }
            }
        }
        else if(tskobjptr->readfsinfo->ftype == TSK_FS_TYPE_HFS_DETECT)
        {
            tsk_fs_file_walk_type(tskobjptr->readfileinfo, TSK_FS_ATTR_TYPE_HFS_DATA, HFS_FS_ATTR_ID_DATA, (TSK_FS_FILE_WALK_FLAG_ENUM)(TSK_FS_FILE_WALK_FLAG_AONLY | TSK_FS_FILE_WALK_FLAG_SLACK), GetBlockAddress, NULL);
        }
        else if(tskobjptr->readfsinfo->ftype == TSK_FS_TYPE_FAT_DETECT)
        {
            tsk_fs_file_walk(tskobjptr->readfileinfo, (TSK_FS_FILE_WALK_FLAG_ENUM)(TSK_FS_FILE_WALK_FLAG_AONLY | TSK_FS_FILE_WALK_FLAG_SLACK), GetBlockAddress, NULL);
        }
        else if(tskobjptr->readfsinfo->ftype == TSK_FS_TYPE_ISO9660_DETECT)
        {
            iso9660_inode* dinode;
            dinode = (iso9660_inode*)tsk_malloc(sizeof(iso9660_inode));
            iso9660_inode_node* n;
            n = ((ISO_INFO*)tskobjptr->readfileinfo->fs_info)->in_list;
            while(n && (n->inum != tskobjptr->readfileinfo->meta->addr))
                n = n->next;
            if(n)
                memcpy(dinode, &n->inode, sizeof(iso9660_inode));
            int block = tsk_getu32(tskobjptr->readfileinfo->fs_info->endian, dinode->dr.ext_loc_m);
            TSK_OFF_T size = tskobjptr->readfileinfo->meta->size;
            while((int64_t)size > 0)
            {
                blockstring += QString::number(block++) + "|";
                size -= tskobjptr->readfileinfo->fs_info->block_size;
            }
        }
        else
        {
            tsk_fs_file_walk(tskobjptr->readfileinfo, (TSK_FS_FILE_WALK_FLAG_ENUM)(TSK_FS_FILE_WALK_FLAG_AONLY | TSK_FS_FILE_WALK_FLAG_SLACK), GetBlockAddress, NULL);
        }
        qDebug() << "blkstring:" << blockstring;
        tskobjptr->blkaddrlist = blockstring.split("|", QString::SkipEmptyParts);
        if(blockstring.compare("") != 0)
        {
            tskobjptr->offset = blockstring.split("|", QString::SkipEmptyParts).at(0).toULongLong()*tskobjptr->blocksize + tskobjptr->fsoffset;
        }
        else
        {
            if(tskobjptr->readfsinfo->ftype == TSK_FS_TYPE_NTFS_DETECT)
            {
                //tskobjptr->resoffset = GetResidentOffset(wombatvariable.selectedobject.address);
            }
            else
            {
                tskobjptr->offset = tskobjptr->fsoffset;
            }
        }
        //tskobjptr->blockaddress = blockstring; // probably don't need this variable
    }
    else if(wombatvariable.selectedobject.objtype == 6) // ads file object
    {
        OpenParentImage(wombatvariable.selectedobject.parimgid);
        OpenParentFileSystem(wombatvariable.selectedobject.parfsid);
        tskobjptr->blocksize = tskobjptr->readfsinfo->block_size;
        tskobjptr->fsoffset = tskobjptr->readfsinfo->offset;
        tskobjptr->offset = 0;
        tskobjptr->adsoffset = wombatvariable.selectedobject.address;
        tskobjptr->mftattrid = wombatvariable.selectedobject.mftattrid;
        tskobjptr->address = wombatvariable.selectedobject.parentid;
        tskobjptr->objecttype = 6;
        tskobjptr->length = wombatvariable.selectedobject.size;
        OpenFileSystemFile();
        // DO BLOCK ADDRESS STUFF HERE...
        // DO THE REST HERE AS WELL...
    }
*/
    /*
    else if(wombatvarptr->selectedobject.objtype == 5) // file object
    {
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
        tskobjptr->blockaddress = wombatvarptr->selectedobject.blockaddress;
        tskobjptr->blkaddrlist = wombatvarptr->selectedobject.blockaddress.split("|", QString::SkipEmptyParts);
    }
    else if(wombatvarptr->selectedobject.objtype == 6) // ads file object
    {
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
        tskobjptr->blockaddress = wombatvarptr->selectedobject.blockaddress;
        tskobjptr->blkaddrlist = wombatvarptr->selectedobject.blockaddress.split("|", QString::SkipEmptyParts);
        OpenFileSystemFile();
    }
    */
    //if(wombatvariable.selectedobject.objtype == 1)
    if(wombatvariable.selectedobject.modid.split("-").count() < 4) // image file
    {
        hexwidget->openimage();
        hexwidget->set1BPC();
        hexwidget->setBaseHex();
        hexwidget->SetTopLeft(0);
    }
    else
    {
        hexwidget->SetTopLeft(tskobjptr->offset);
        //if(wombatvariable.selectedobject.objtype == 5 || wombatvariable.selectedobject.objtype == 6)
        if(wombatvariable.selectedobject.modid.split("-").count() == 4)
        {
            fileviewer->filehexview->openimage();
            fileviewer->filehexview->set1BPC();
            fileviewer->filehexview->setBaseHex();
            fileviewer->filehexview->SetTopLeft(0);
        }
    }
}

/*
 *
          * select objectid, parimgid, parfsid, address, name from data where objecttype = 5 and parimgid = ?;
                     parfsid = filequery.value(2).toULongLong();
            const TSK_TCHAR** imagepartspath;
            unsigned long long objectid = 0;
            QString objectname = "";
            TSK_IMG_INFO* readimginfo;
            TSK_FS_INFO* readfsinfo;
            TSK_FS_FILE* readfileinfo;
            // Open Parent Image
            std::vector<std::string> pathvector;
            pathvector.clear();
            QSqlQuery imgquery(fcasedb);
            imgquery.prepare("SELECT fullpath FROM dataruns WHERE objectid = ? ORDER BY seqnum;");
            imgquery.bindValue(0, filequery.value(1).toULongLong());
            if(imgquery.exec())
            {
                while(imgquery.next())
                {
                    pathvector.push_back(imgquery.value(0).toString().toStdString());
                }
            }
            imgquery.finish();

            objectid = filequery.value(0).toULongLong();
            objectname = filequery.value(4).toString();
            imagepartspath = (const char**)malloc(pathvector.size()*sizeof(char*));

            for(uint i=0; i < pathvector.size(); i++)
            {
                imagepartspath[i] = pathvector.at(i).c_str();
            }
            readimginfo = tsk_img_open(pathvector.size(), imagepartspath, TSK_IMG_TYPE_DETECT, 0);
            free(imagepartspath);
            //OpenParentFileSystem
            QSqlQuery fsquery(fcasedb);
            fsquery.prepare("SELECT byteoffset FROM data where objectid = ?;");
            fsquery.bindValue(0, filequery.value(2).toULongLong());
            fsquery.exec();
            fsquery.next();
            fsoffset = fsquery.value(0).toULongLong();
            readfsinfo = tsk_fs_open_img(readimginfo, fsquery.value(0).toULongLong(), TSK_FS_TYPE_DETECT);
            fsquery.finish();
            QVector<unsigned long long> adsobjid;
            QVector<unsigned long long> adsattrid;
            adsobjid.clear();
            adsattrid.clear();
            if(readfsinfo->ftype == TSK_FS_TYPE_NTFS_DETECT)
            {
                if(QString::compare(objectname, ".") == 0 || QString::compare(objectname, "..") == 0)
                {
                }
                else
                {
                // get ads dataset
                QSqlQuery adsquery(fcasedb);
                adsquery.prepare("SELECT objectid, mftattrid FROM data WHERE objecttype = 6 and parentid = ?;");
                adsquery.bindValue(0, filequery.value(3).toULongLong());
                if(adsquery.exec())
                {
                    while(adsquery.next())
                    {
                        adsobjid.append(adsquery.value(0).toULongLong());
                        adsattrid.append(adsquery.value(1).toULongLong());
                    }
                }
                adsquery.finish();
                }
            }
            //OpenFile
            readfileinfo = tsk_fs_file_open_meta(readfsinfo, NULL, filequery.value(3).toULongLong());
            QModelIndexList indexlist = ui->dirTreeView->model()->match(ui->dirTreeView->model()->index(0, 0), Qt::DisplayRole, QVariant(objectid), 1, Qt::MatchFlags(Qt::MatchRecursive));
            if(indexlist.count() > 0)
                ui->dirTreeView->model()->setData(indexlist.at(0), MagicFile(readfileinfo, objectid), Qt::DisplayRole);
            BlockFile(readfileinfo, objectid, adsattrid);
            PropertyFile(readfileinfo, objectid, fsoffset, readfsinfo->block_size, parfsid);
            if(readfileinfo->fs_info->ftype == TSK_FS_TYPE_NTFS_DETECT)
            {
                if(QString::compare(objectname, ".") == 0 || QString::compare(objectname, "..") == 0)
                {
                }
                else
                {
                    for(int i = 0; i < adsobjid.count(); i++)
                    {
                        QModelIndexList indexlist = ui->dirTreeView->model()->match(ui->dirTreeView->model()->index(0, 0), Qt::DisplayRole, QVariant(adsobjid.at(i)), 1, Qt::MatchFlags(Qt::MatchRecursive));
                        if(indexlist.count() > 0)
                            ui->dirTreeView->model()->setData(indexlist.at(0), AlternateDataStreamMagicFile(readfileinfo, adsobjid.at(i)), Qt::DisplayRole);
                    }
                    AlternateDataStreamBlockFile(readfileinfo, adsobjid, adsattrid);
                    AlternateDataStreamPropertyFile(readfileinfo, adsobjid, adsattrid);
                }
            }
         */
    //wombatvarptr->selectedobject.id = selectedindex.sibling(selectedindex.row(), 0).data().toULongLong(); // object id
    /*
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
    */

//void WombatForensics::OpenParentImage(unsigned long long imgid)
void WombatForensics::OpenParentImage(void)
{
    QString tmpstr = "";
    QStringList tmplist;
    tmplist.clear();
    QFile evidfile(wombatvariable.tmpmntpath + wombatvariable.evidenceobject.name + ".evid." + QString::number(evidcnt));
    evidfile.open(QIODevice::ReadOnly);
    tmpstr = evidfile.readLine();
    tmplist = tmpstr.split(",");
    std::vector<std::string> tmpvec;
    tmpvec.clear();
    evidfile.close();
    tskobjptr->offset = 0;
    tskobjptr->objecttype = 1;
    tskobjptr->length = selectedindex.sibling(selectedindex.row(), 3).data().toULongLong(); // object size
    tskobjptr->imglength = selectedindex.sibling(selectedindex.row(), 3).data().toULongLong(); // image size
    tskobjptr->sectsize = tmplist.at(2).toInt();
    tskobjptr->blocksize = tmplist.at(2).toInt();
    tskobjptr->partcount = tmplist.at(3).split("|").size();
    for(int i = 0; i < tmplist.at(3).split("|").size(); i++)
    {
        tmpvec.push_back(tmplist.at(3).split("|").at(i).toStdString());
    }
    tskobjptr->imagepartspath = (const char**)malloc(tmpvec.size()*sizeof(char*));
    for(int i =0; i < tmpvec.size(); i++)
    {
        tskobjptr->imagepartspath[i] = tmpvec[i].c_str();
    }
    tskobjptr->readimginfo = tsk_img_open(tskobjptr->partcount, tskobjptr->imagepartspath, TSK_IMG_TYPE_DETECT, 0);
    if(tskobjptr->readimginfo == NULL)
    {
        qDebug() << tsk_error_get_errstr();
        LogMessage("Image opening error");
    }
    free(tskobjptr->imagepartspath);

    /*
    QSqlQuery cntquery(fcasedb);
    cntquery.prepare("SELECT count(fullpath) FROM dataruns WHERE objectid = ?");
    cntquery.bindValue(0, imgid);
    cntquery.exec();
    cntquery.first();
    unsigned long long imgcnt = cntquery.value(0).toULongLong();
    cntquery.finish();
    QSqlQuery evidquery(fcasedb);
    evidquery.prepare("SELECT fullpath FROM dataruns WHERE objectid = ? order by seqnum");
    evidquery.bindValue(0, imgid);
    evidquery.exec();
    tskobjptr->imagepartspath = (const char**)malloc(imgcnt*sizeof(char*));
    for(int i=0; i < imgcnt; i++)
    {
        evidquery.next();
        tskobjptr->imagepartspath[i] = evidquery.value(0).toString().toStdString().c_str();
    }
    tskobjptr->partcount = imgcnt;
    evidquery.finish();
    */
    /*
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
    */
    tskobjptr->readimginfo = tsk_img_open(tskobjptr->partcount, tskobjptr->imagepartspath, TSK_IMG_TYPE_DETECT, 0);
    if(tskobjptr->readimginfo == NULL)
    {
        LogMessage("Image opening error");
    }
    free(tskobjptr->imagepartspath);
}

void WombatForensics::OpenParentFileSystem(unsigned long long fsid)
{
    /*
    QSqlQuery fsquery(fcasedb);
    fsquery.prepare("SELECT offset FROM data WHERE id = ?");
    fsquery.bindValue(0, fsid);
    fsquery.exec();
    fsquery.first();
    tskobjptr->readfsinfo = tsk_fs_open_img(tskobjptr->readimginfo, fsquery.value(0).toULongLong(), TSK_FS_TYPE_DETECT);
    fsquery.finish();
    */
    /*
     *    wombatptr->bindvalues.append(fsid);
    wombatptr->sqlrecords.clear();
    wombatptr->sqlrecords = GetSqlResults("SELECT byteoffset FROM data WHERE objectid = ?", wombatptr->bindvalues);
    return wombatptr->sqlrecords.at(0).value(0).toULongLong();

     */ 
    //unsigned long long fsoffset = wombatdatabase->ReturnFileSystemOffset(fsid);
    //tskobjptr->readfsinfo = tsk_fs_open_img(tskobjptr->readimginfo, fsoffset, TSK_FS_TYPE_DETECT);
}

void WombatForensics::OpenFileSystemFile()
{
    tskobjptr->readfileinfo = tsk_fs_file_open_meta(tskobjptr->readfsinfo, NULL, tskobjptr->address);
}

/*
unsigned long long WombatForensics::GetResidentOffset(unsigned long long fileaddress, unsigned long long fsaddress)
{
    QSqlQuery resquery(fcasedb);
    QStringList inputs;
    QList<unsigned long long> outputs;
    inputs << "%0x0B%" << "%0x0D%" << "%0x30%" << "%0x40%";
    for(int i=0; i < inputs.count(); i++)
    {
        resquery.prepare("SELECT value from properties where objectid = ? and description like(?);");
        resquery.addBindValue(wombatptr->selectedobject.parfsid);
        resquery.addBindValue(inputs.at(i));
        resquery.exec();
        resquery.next();
        outputs.append(resquery.value(0).toULongLong());
    }
    resquery.finish();
    mftrecordsize = outputs.at(3);
    return ((outputs.at(0) * outputs.at(1) * outputs.at(2)) + (outputs.at(3)*fileaddress));
    return 1;
}
*/

void WombatForensics::CloseCurrentCase()
{

    QDir eviddir = QDir(wombatvariable.tmpmntpath);
    QStringList evidfiles = eviddir.entryList(QStringList("*.evid.*"), QDir::NoSymLinks | QDir::Files);
    //qDebug() << "evid files:" << evidfiles;
    for(int i=0; i < evidfiles.count(); i++)
    {
        //qDebug() << "remove item:" << QString("e" + evidfiles.at(i).split(".", QString::SkipEmptyParts).last());
        treemodel->RemEvidence(QString("e" + evidfiles.at(i).split(".", QString::SkipEmptyParts).last()));
        evidcnt--;
    }
    //autosavetimer->stop();
    
    /*
    QSqlQuery evidquery(fcasedb);
    evidquery.prepare("SELECT id FROM data where objtype = 1");
    evidquery.exec();
    while(evidquery.next())
        treemodel->RemEvidence(evidquery.value(0).toULongLong());
    evidquery.finish();
    */
    //autosavetimer->stop();
    //GetEvidenceObjects(); // was previously wombatdabase
    /*
    for(int i=0; i < wombatvarptr->evidenceobjectvecotr.count(); i++)
    {
        treemodel->RemEvidence(wombatvarptr->evidenceobjectvector.at(i).id);
    }
     */
    // DETERMINE IF THERE IS A WAY TO REMOVE ALL ELEMENTS FROM A TREENODE AT ONCE...
    setWindowTitle("WombatForensics");
    filesprocessed = 0;
    filesfound = 0;
    filtercountlabel->setText("Filtered: 0");
    processcountlabel->setText("Processed: " + QString::number(filesprocessed));
    filecountlabel->setText("Files: " + QString::number(filesfound));
    
    //QString umntstr = "pkexec umount ";
    //umntstr += wombatvariable.tmpmntpath;
    if(wombatvariable.iscaseopen)
    {
        QString umntstr = "guestunmount " + wombatvariable.tmpmntpath;
        QProcess::execute(umntstr);
        StatusUpdate("Current Case was closed successfully");
    }
    RemoveTmpFiles();
    wombatvariable.iscaseopen = false;
    //CloseCaseDB(); // was previously wombatdatabase
}

void WombatForensics::RemEvidence()
{

    /*
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
            evidcnt--;
            remfuture = QtConcurrent::run(wombatdatabase, &WombatDatabase::RemoveEvidence);
            remwatcher.setFuture(remfuture);
 
        }
    }
    */
}

void WombatForensics::GetExportData(Node* curnode, FileExportData* exportdata)
{
    /*
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
    */
}

void WombatForensics::GetDigData(Node* curnode, FileDeepData* deepdata)
{
    /*
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
    */
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
    /*
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
    */
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
    /*
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
    */
}

void WombatForensics::DigFiles(FileDeepData* deepdata)
{
    /*
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
    */
}

void WombatForensics::ProcessExport(TskObject curobj, std::string fullpath, std::string name)
{
    /*
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
    */
}

void WombatForensics::ProcessDig(TskObject curobj, unsigned long long objectid, std::vector<FileDeepData::DigOptions> digoptions)
{
    /*
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
    */
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
        CloseCurrentCase();
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
    cancelthread->close();
    RemoveTmpFiles();
    event->accept();
    msglog->clear();
    msgviewer->close();
    /*
    if(fappdb.isOpen())
        fappdb.close();
    fappdb = QSqlDatabase();
    QSqlDatabase::removeDatabase("appdb");
    if(thumbdb.isOpen())
        thumbdb.close();
    thumbdb = QSqlDatabase();
    QSqlDatabase::removeDatabase("thumbdb");
    if(fcasedb.isOpen())
        fcasedb.close();
    fcasedb = QSqlDatabase();
    QSqlDatabase::removeDatabase("casedb");
    */

    /*
    wombatdatabase->CloseCaseDB();
    wombatdatabase->CloseAppDB();
    wombatdatabase->CloseThumbDB();
    */
}

void WombatForensics::RemoveTmpFiles()
{
    //qDebug() << "remove tmp files start:" << wombatvariable.tmpfilepath;
    //QString homePath = QDir::homePath();
    //homePath += ".wombatforensics/tmpfiles/";
    //homePath += "/WombatForensics/";
    QDir tmpdir(wombatvariable.tmpfilepath);
    if(!tmpdir.removeRecursively())
    {
        DisplayError("2.7", "Tmp File Removal", "All tmp files may not have been removed. Please manually remove them to avoid evidence contamination.");
    }
}

void WombatForensics::on_actionNew_Case_triggered()
{
    // determine if a case is open
    // NEED A NEW WAY TO CHECK IF A CASE IS OPEN...
    //if(wombatvariable.caseobject.id > 0)
    if(wombatvariable.iscaseopen)
    {
        int ret = QMessageBox::question(this, tr("Close Current Case"), tr("There is a case already open. Are you sure you want to close it?"), QMessageBox::Yes | QMessageBox::No);
        qDebug() << "msgbox return:" << ret << "msgbox yes:" << QMessageBox::Yes;
        if(ret == QMessageBox::Yes)
        {
            qDebug() << "detected an open case";
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
    //if(wombatvariable.caseobject.id > 0)
    if(wombatvariable.iscaseopen)
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
    /*
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
    */
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
    /*
    wombatdatabase->GetThumbnails();
    imagewindow->UpdateGeometries();
    imagewindow->show();
    LogMessage("Evidence ready");
    StatusUpdate("Evidence ready");
    */
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
        if(selectedindex.sibling(selectedindex.row(), 9).data().toString().contains("text/"))
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
    //cancelthread->show();
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
    RemoveTmpFiles();
    /*
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
    */
}

void WombatForensics::AutoSaveState()
{
    // change display text
    StatusUpdate("Saving State Started");
    SaveState();
    StatusUpdate("Evidence ready");
    // change display text
}
//void SecondaryProcessing(SecondaryProcessObject &secprocobj)
//void SecondaryProcessing(QVariantMap &jsonstore)
/*
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
    //fsquery.bindValue(0, jsonstore.value("parfsid").toULongLong());
    fsquery.bindValue(0, secprocobj.parfsid);
    fsquery.exec();
    fsquery.next();
    //jsonstore.insert("fsoffset", fsquery.value(0));
    //jsonstore.insert("fstype", fsquery.value(1));
    fsoffset = fsquery.value(0).toULongLong();
    fstype = fsquery.value(1).toInt();
    fsquery.finish();
    //readfsinfo = tsk_fs_open_img(IMG_2ND_PROC, jsonstore.value("fsoffset").toULongLong(), TSK_FS_TYPE_DETECT);
    readfsinfo = tsk_fs_open_img(IMG_2ND_PROC, fsoffset, TSK_FS_TYPE_DETECT);
    //if(jsonstore.value("fstype").toInt() == 1)
    if(fstype == 1)
    {
        //if(QString::compare(jsonstore.value("name").toString(), ".") == 0 || QString::compare(jsonstore.value("name").toString(), "..") == 0)
        if(QString::compare(secprocobj.name, ".") == 0 || QString::compare(secprocobj.name, "..") == 0)
        {
        }
        else
        {
            QSqlQuery adsquery(fcasedb);
            adsquery.prepare("SELECT objectid, mftattrid FROM data WHERE objecttype = 6 AND parentid = ?;");
            adsquery.bindValue(0, secprocobj.address);
            //adsquery.bindValue(0, jsonstore.value("address").toULongLong());
            if(adsquery.exec())
            {
                while(adsquery.next())
                {*/
                    /*
                    QVariantMap tmpmap;
                    tmpmap.insert("objectid", adsquery.value(0));
                    tmpmap.insert("attrid", adsquery.value(1));
                    jsonstore.insert("adsmapentry", tmpmap);
                    *//*
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
    //readfileinfo = tsk_fs_file_open_meta(readfsinfo, NULL, jsonstore.value("address").toULongLong());
    readfileinfo = tsk_fs_file_open_meta(readfsinfo, NULL, secprocobj.address);
    char magicbuffer[1024];
    tsk_fs_file_read(readfileinfo, 0, magicbuffer, 1024, TSK_FS_FILE_READ_FLAG_NONE);


    // Begin Mime Type Determination
    QMimeDatabase mimedb;
    QMimeType mimetype = mimedb.mimeTypeForData(QByteArray((char*)magicbuffer));
    secprocobj.mimetype = mimetype.name();
    //jsonstore.insert("mimetype", mimetype.name());
    // End Mime Type Determination
    //int fstype = jsonstore.value("fstype").toInt();*/
    /*
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
            //for(int i = 0; i < jsonstore.count("adsmapentry"); i++)
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
    //secprocobj.blockaddress = blockstring;
    //jsonstore.insert("blockaddress", blockstring);
    //
    //
    */
    /*
    QVariantMap tmpmap;
    tmpmap.insert("objectid", secprocobj.objectid);
    tmpmap.insert("mimetype", mimetype.name());
    tmpmap.insert("blockaddress", blockstring);
    QJsonObject tmpobject = QJsonObject::fromVariantMap(tmpmap);
    //QJsonObject object = QJsonObject::fromVariantMap(jsonstore);
    QJsonDocument document;
    document.setObject(tmpobject);
    QFile jsonfile("../testfile.json");
    QFile binfile("../testfile.dat");
    jsonfile.open(QFile::Append);
    jsonfile.write(document.toJson(QJsonDocument::Compact));
    jsonfile.close();
    binfile.open(QFile::Append);
    binfile.write(document.toBinaryData());
    binfile.close();

    */

    /*
     * TOO SLOW, SO I WILL STORE IN MEMORY USING SECOBJPROP AND THEN DO A TRANSACT WHEN THIS FINISHES...
    QSqlQuery updatequery(fcasedb);
    updatequery.prepare("UPDATE data SET filemime = ?, filesignature = ?, blockaddress = ? WHERE objectid = ?;");
    updatequery.bindValue(0, mimetype.name());
    updatequery.bindValue(1, mimetype.name().split("/").at(0));
    updatequery.bindValue(2, blockstring);
    updatequery.bindValue(3, secprocobj.objectid);
    updatequery.exec();
    updatequery.next();
    updatequery.finish();

    */
    /*
     *
     *    QSqlQuery mimequery(fcasedb);
    mimequery.prepare("UPDATE data SET filemime = ?, filesignature = ? WHERE objectid = ?;");
    if(readlen > 0)
    {
        tmpvariant = QVariant(QString::fromStdString(sigp2));
        mimequery.bindValue(0, QString::fromStdString(sigp1));
        mimequery.bindValue(1, QString::fromStdString(sigp2));

     *
     */ 


    //qDebug() << "current id. filename:" << secprocobj.objectid << "." << secprocobj.name;

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
    *//*
    filesprocessed++;
    processphase++;
    isignals->ProgUpd();
    tsk_fs_file_close(readfileinfo);
    tsk_fs_close(readfsinfo);
}*/
