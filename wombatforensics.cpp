#include "wombatforensics.h"

// Copyright 2015 Pasquale J. Rinaldi, Jr.
// Distrubted under the terms of the GNU General Public License version 2

WombatForensics::WombatForensics(QWidget *parent) : QMainWindow(parent), ui(new Ui::WombatForensics)
{
    ui->setupUi(this);
    this->menuBar()->hide();
    this->statusBar()->setSizeGripEnabled(true);
    selectedoffset = new QLabel(this);
    selectedoffset->setText("Offset: 00");
    selectedhex = new QLabel(this);
    selectedhex->setText("Length: 0");
    filtercountlabel = new QLabel(this);
    filtercountlabel->setText("Filtered: 0");
    filecountlabel = new QLabel(this);
    filecountlabel->setText("Found: 0");
    checkedcountlabel = new QLabel(this);
    checkedcountlabel->setText("Checked: 0");
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
    this->statusBar()->addWidget(filecountlabel, 0);
    this->statusBar()->addWidget(checkedcountlabel, 0);
    this->statusBar()->addWidget(filtercountlabel, 0);
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
    connect(imagewindow->sb, SIGNAL(valueChanged(int)), this, SLOT(UpdateThumbnails(int)), Qt::QueuedConnection);
    connect(imagewindow, SIGNAL(HideImageWindow(bool)), this, SLOT(HideImageWindow(bool)), Qt::DirectConnection);
    connect(textviewer, SIGNAL(HideTextViewerWindow(bool)), this, SLOT(HideTextViewer(bool)), Qt::DirectConnection);
    connect(msgviewer, SIGNAL(HideMessageViewerWindow(bool)), this, SLOT(HideMessageViewer(bool)), Qt::DirectConnection);
    connect(byteviewer, SIGNAL(HideByteConverterWindow(bool)), this, SLOT(HideByteViewer(bool)), Qt::DirectConnection);
    connect(propertywindow, SIGNAL(HidePropertyWindow(bool)), this, SLOT(HidePropertyWindow(bool)), Qt::DirectConnection);
    connect(fileviewer, SIGNAL(HideFileViewer(bool)), this, SLOT(HideFileViewer(bool)), Qt::DirectConnection);
    connect(isignals, SIGNAL(ProgressUpdate(unsigned long long)), this, SLOT(UpdateProgress(unsigned long long)), Qt::QueuedConnection);
    connect(isignals, SIGNAL(DigUpdate(void)), this, SLOT(UpdateDig()), Qt::QueuedConnection);
    connect(isignals, SIGNAL(ExportUpdate(void)), this, SLOT(UpdateExport()), Qt::QueuedConnection);
    propertywindow->setModal(false);
    CheckWombatConfiguration();
    InitializeAppStructure();
    //connect(cancelthread, SIGNAL(CancelCurrentThread()), &secondwatcher, SLOT(cancel()), Qt::QueuedConnection);
    connect(&sqlwatcher, SIGNAL(finished()), this, SLOT(UpdateStatus()), Qt::QueuedConnection);
    connect(&thumbwatcher, SIGNAL(finished()), this, SLOT(FinishThumbs()), Qt::QueuedConnection);
    connect(cancelthread, SIGNAL(CancelCurrentThread()), &thumbwatcher, SLOT(cancel()), Qt::QueuedConnection);
    connect(&exportwatcher, SIGNAL(finished()), this, SLOT(FinishExport()), Qt::QueuedConnection);
    connect(cancelthread, SIGNAL(CancelCurrentThread()), &exportwatcher, SLOT(cancel()), Qt::QueuedConnection);
    connect(&digwatcher, SIGNAL(finished()), this, SLOT(UpdateDigging()), Qt::QueuedConnection);
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
    ui->dirTreeView->setSortingEnabled(true); // enables the sorting arrow, but doesn't sort anything.
    ui->dirTreeView->setUniformRowHeights(true);
    ui->dirTreeView->header()->setSortIndicatorShown(false);
    ui->dirTreeView->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->dirTreeView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->dirTreeView, SIGNAL(collapsed(const QModelIndex &)), this, SLOT(ResizeViewColumns(const QModelIndex &)));
    connect(ui->dirTreeView, SIGNAL(expanded(const QModelIndex &)), this, SLOT(ResizeViewColumns(const QModelIndex &)));
    connect(ui->dirTreeView, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(TreeContextMenu(const QPoint &)));
    connect(ui->dirTreeView->header(), SIGNAL(sectionClicked(int)), this, SLOT(SetFilter(int)));
    connect(ui->dirTreeView, SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(ShowFile(const QModelIndex &)));
    connect(imagewindow, SIGNAL(SendObjectToTreeView(QString)), this, SLOT(SetSelectedFromImageViewer(QString)));
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
    listeditems.clear();
    //autosavetimer = new QTimer(this);
    //connect(autosavetimer, SIGNAL(timeout()), this, SLOT(AutoSaveState()));
}
//////////////////////////////////////////////////////////////
void WombatForensics::ShowExternalViewer()
{
    unsigned long long curobjaddr = selectedindex.sibling(selectedindex.row(), 0).data().toString().split("-f").at(1).toULongLong();
    QString tmpstr = "";
    QStringList evidlist;
    evidlist.clear();
    std::vector<std::string> pathvector;
    pathvector.clear();
    QDir eviddir = QDir(wombatvariable.tmpmntpath);
    QStringList evidfiles = eviddir.entryList(QStringList("*.evid." + selectedindex.sibling(selectedindex.row(), 0).data().toString().split("-").at(0).mid(1)), QDir::NoSymLinks | QDir::Files);
    wombatvariable.evidencename = evidfiles.at(0);
    QFile evidfile(wombatvariable.tmpmntpath + wombatvariable.evidencename.split(".evid").at(0) + ".evid." + selectedindex.sibling(selectedindex.row(), 0).data().toString().split("-").at(0).mid(1));
    evidfile.open(QIODevice::ReadOnly);
    tmpstr = evidfile.readLine();
    evidlist = tmpstr.split(",");
    tskexternalptr->partcount = evidlist.at(3).split("|").size();
    evidfile.close();
    for(int i = 0; i < evidlist.at(3).split("|").size(); i++)
        pathvector.push_back(evidlist.at(3).split("|").at(i).toStdString());
    tskexternalptr->imagepartspath = (const char**)malloc(pathvector.size()*sizeof(char*));
    for(uint i = 0; i < pathvector.size(); i++)
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
    QStringList partfiles = eviddir.entryList(QStringList(wombatvariable.evidencename.split(".evid").at(0) + ".part." + selectedindex.sibling(selectedindex.row(), 0).data().toString().split("-").at(2).mid(1)), QDir::NoSymLinks | QDir::Files);
    QFile partfile(wombatvariable.tmpmntpath + partfiles.at(0));
    partfile.open(QIODevice::ReadOnly);
    tmpstr = partfile.readLine();
    partfile.close();
    partlist = tmpstr.split(",");
    tskexternalptr->readfsinfo = tsk_fs_open_img(tskexternalptr->readimginfo, partlist.at(4).toULongLong(), TSK_FS_TYPE_DETECT);
    tskexternalptr->readfileinfo = tsk_fs_file_open_meta(tskexternalptr->readfsinfo, NULL, curobjaddr);
    ssize_t filelen = 0;
    if(tskexternalptr->readfileinfo->meta != NULL)
    {
        char* ibuffer = new char[tskexternalptr->readfileinfo->meta->size];
        // WILL NEED TO FIGURE OUT IF ITS AN ATTRIBUTE OR NOT AND HANDLE THE IF BELOW...
        //if(objtype == 5)
        filelen = tsk_fs_file_read(tskexternalptr->readfileinfo, 0, ibuffer, tskexternalptr->readfileinfo->meta->size, TSK_FS_FILE_READ_FLAG_NONE);
        //else
        (new QDir())->mkpath(wombatvariable.tmpfilepath);
        QString tmpstring = wombatvariable.tmpfilepath + selectedindex.sibling(selectedindex.row(), 0).data().toString() + "-tmp";
        QFile tmpfile(tmpstring);
        if(tmpfile.open(QIODevice::WriteOnly))
        {
            QDataStream outbuffer(&tmpfile);
            outbuffer.writeRawData(ibuffer, filelen);
            tmpfile.close();
        }
        delete[] ibuffer;
        QProcess* process = new QProcess(this);
        QStringList arguments;
        arguments << tmpstring;
        process->startDetached(((QAction*)QObject::sender())->text(), arguments);
    }
    else
        qDebug() << "no file meta for file size";
}

void WombatForensics::SetSelectedFromImageViewer(QString objectid)
{
    QModelIndexList indexlist = treenodemodel->match(treenodemodel->index(0, 0, QModelIndex()), Qt::DisplayRole, QVariant(objectid), -1, Qt::MatchFlags(Qt::MatchExactly | Qt::MatchRecursive));
    if(indexlist.count() > 0)
        ui->dirTreeView->setCurrentIndex(indexlist.at(0));
}

void WombatForensics::ShowFile(const QModelIndex &index)
{
    if(index.sibling(index.row(), 9).data().toString().contains("image/"))
    {
        imagewindow->ShowImage(index);
    }
    else if(index.sibling(index.row(), 9).data().toString().contains("video/"))
    {
        videowindow->ShowVideo(index);
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

void WombatForensics::CheckWombatConfiguration()
{
    // check fstab entry
    int fstabbool = 0;
    QFile fstabfile("/etc/fstab");
    if(fstabfile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream fstabin(&fstabfile);
        while(!fstabin.atEnd())
        {
            QString line = fstabin.readLine();
            if(line.compare("/tmp/wombatforensics/currentwfc /tmp/wombatforensics/mntpt auto defaults,rw,user,noauto 0 0", Qt::CaseSensitive) == 0)
                fstabbool = 1;
        }
        fstabfile.close();
        if(fstabbool == 1)
            qDebug() << "fstab is correct";
        else
            qDebug() << "fstab is wrong";
    }
    // check mntpt directory exists and has root ownership
    /*
    if(QFileInfo::exists("/tmp/wombatforensics/mntpt"))
    {
        QFileInfo mntptfile("/tmp/wombatforensics/mntpt");
        if(mntptfile.groupId() == 0 && mntptfile.ownerId() == 0)
            qDebug() << "it is set correctly:" << mntptfile.ownerId() << ":" << mntptfile.groupId();
        else
            qDebug() << "error, mntpt dir isn't owned by root:" << mntptfile.ownerId() << ":" << mntptfile.groupId();
    }
    else
        qDebug() << "error, the mntpt doesn't exist and needs to be created as root...";
    */
    // check if mntpt link exists, if it does, remove it
    // check if case link exists, if it does, remove it
    if(QFileInfo::exists("/tmp/wombatforensics/mntpt"))
        QFile::remove("/tmp/wombatforensics/mntpt");
    if(QFileInfo::exists("/tmp/wombatforensics/currentwfc"))
        QFile::remove("/tmp/wombatforensics/currentwfc");
}

void WombatForensics::InitializeAppStructure()
{
    wombatvariable.iscaseopen = false;
    QString tmppath = QDir::tempPath();
    tmppath += "/wombatforensics/";
    QString homepath = QDir::homePath();
    homepath += "/.wombatforensics/";
    wombatvariable.tmpfilepath = tmppath + "tmpfiles/";
    wombatvariable.tmpmntpath = homepath + "mntpt/";
    wombatvariable.imgdatapath = tmppath + "datamnt/";
    //wombatvariable.tmpfilepath = homepath + "tmpfiles/"; // old one
    //wombatvariable.tmpmntpath = homepath + "mntpt/"; // old one
    if((new QDir())->mkpath(tmppath) == false)
        DisplayError("1.6", "App tmppath folder failed", "App tmppath folder was not created");
    if((new QDir())->mkpath(wombatvariable.tmpfilepath) == false)
        DisplayError("1.3", "App tmpfile folder failed", "App Tmpfile folder was not created");
    if((new QDir())->mkpath(wombatvariable.tmpmntpath) == false)
        DisplayError("1.2", "App tmpmnt folder failed", "App tmpmnt folder was not created");
    if((new QDir())->mkpath(homepath) == false)
        DisplayError("1.4", "App homepath folder failed", "App homepath folder was not created");
    if((new QDir())->mkpath(wombatvariable.imgdatapath) == false)
        DisplayError("1.5", "App imgdatapath folder failed", "App imgdatapath folder was not created");
    viewerfile.setFileName(homepath + "viewers");
    if(!FileExists(QString(homepath + "viewers").toStdString()))
    {
        viewerfile.open(QIODevice::WriteOnly | QIODevice::Text);
        viewerfile.close();
    }
    viewmanage = new ViewerManager(this);
    viewmanage->setWindowIcon(QIcon(":/bar/viewermanager"));
    connect(viewmanage, SIGNAL(HideManagerWindow()), this, SLOT(HideViewerManager()), Qt::DirectConnection);
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

void WombatForensics::InitializeCaseStructure()
{
    StatusUpdate("Generating Case Structure...");
    // create new case here
    wombatvariable.casename = QFileDialog::getSaveFileName(this, tr("Create New Case File"), QDir::homePath(), tr("WombatForensics Case (*.wfc)"));
    if(!wombatvariable.casename.isEmpty())
    {
        QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
        QStringList tmplist = wombatvariable.casename.split("/");
        tmplist.removeLast();
        wombatvariable.casepath = tmplist.join("/");
        if(!wombatvariable.casename.contains(".wfc"))
        {
            this->setWindowTitle(QString("Wombat Forensics - ") + wombatvariable.casename.split("/").last());
            wombatvariable.casename += ".wfc";
        }
        else
        {
            this->setWindowTitle(QString("Wombat Forensics - ") + wombatvariable.casename.split("/").last().split(".").first());
        }
        QFile casefile(wombatvariable.casename);
        casefile.open(QIODevice::ReadWrite);
        casefile.resize(1000000000);
        casefile.close();
        // make ext4 partition
        QString mkfsstr = "mkfs.ext4 -q -E root_owner=" + QString::number(getuid()) + ":" + QString::number(getgid()) + " ";
        mkfsstr += wombatvariable.casename;
        qDebug() << mkfsstr;
        QProcess::execute(mkfsstr);
        /*
        // determine the current username for chown
        QString name = qgetenv("USER");
        if(name.isEmpty())
            name = qgetenv("USERNAME");

        */
        /*
        // call guestfs using code
        guestg = guestfs_create();
        guestfs_add_drive(guestg, wombatvariable.casename.toStdString().c_str());
        guestfs_launch(guestg);
        guestfs_mount(guestg, "dev/sda1", wombatvariable.tmpmntpath.toStdString().c_str());
        */

        /*
        QString mntstr = "sudo mount -o loop " + wombatvariable.casename + " " + wombatvariable.tmpmntpath;
        QProcess::execute(mntstr);
        QString chownstr = "sudo chown -R " + name + ":" + name + " " + wombatvariable.tmpmntpath;
        QProcess::execute(chownstr);
        */

        QString lnkstr = "ln -s " + wombatvariable.casename + " /tmp/wombatforensics/currentwfc";
        QProcess::execute(lnkstr);
        QString lnkmnt = "ln -s " + wombatvariable.tmpmntpath + " /tmp/wombatforensics/mntpt";
        QProcess::execute(lnkmnt);
        QProcess::execute("mount /tmp/wombatforensics/mntpt");

        wombatvariable.iscaseopen = true;
        logfile.setFileName(wombatvariable.tmpmntpath + "msglog");
        msglog->clear();
        LogMessage("Log File Created");
        treefile.setFileName(wombatvariable.tmpmntpath + "treemodel");
        thumbdir.mkpath(wombatvariable.tmpmntpath + "thumbs/");
        InitializeCheckState();
        ui->actionAdd_Evidence->setEnabled(true);
        LogMessage("Case was Created");
        QApplication::restoreOverrideCursor();
        StatusUpdate("Ready");
        //autosavetimer->start(10000); // 10 seconds in milliseconds for testing purposes
        //autosavetimer->start(600000); // 10 minutes in milliseconds for a general setting for real.

        // this works with privilege escalation. only way to mount it though. now requires linux and btrfs.
    }
}

void WombatForensics::InitializeOpenCase()
{
    wombatvariable.casename = QFileDialog::getOpenFileName(this, tr("Open Existing Case"), QDir::homePath(), tr("WombatForensics Case (*.wfc)"));
    if(!wombatvariable.casename.isEmpty())
    {
        StatusUpdate("Case Opening...");
        QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
        QStringList tmplist = wombatvariable.casename.split("/");
        tmplist.removeLast();
        wombatvariable.casepath = tmplist.join("/");
        if(!wombatvariable.casename.contains(".wfc"))
        {
            this->setWindowTitle(QString("Wombat Forensics - ") + wombatvariable.casename.split("/").last());
            wombatvariable.casename += ".wfc";
        }
        else
        {
            this->setWindowTitle(QString("Wombat Forensics - ") + wombatvariable.casename.split("/").last().split(".").first());
        }
        QFile casefile(wombatvariable.casename);
        QString lnkstr = "ln -s " + wombatvariable.casename + " /tmp/wombatforensics/currentwfc";
        QProcess::execute(lnkstr);
        QString lnkmnt = "ln -s " + wombatvariable.tmpmntpath + " /tmp/wombatforensics/mntpt";
        QProcess::execute(lnkmnt);
        QProcess::execute("mount /tmp/wombatforensics/mntpt");
        /*
        QString name = qgetenv("USER");
        if(name.isEmpty())
            name = qgetenv("USERNAME");
        */
        /* TEMP FIX UNTIL UBUNTU 18.04 LTS */
        /*
        QString mntstr = "sudo mount -o loop ";
        mntstr += wombatvariable.casename;
        mntstr += " ";
        mntstr += wombatvariable.tmpmntpath;
        QString chownstr = "sudo chown -R " + name + ":" + name + " " + wombatvariable.tmpmntpath;
        QProcess::execute(mntstr);
        QProcess::execute(chownstr);
        */
        /* END TEMP CODE */
        /*
        // call guestfs using code
        guestg = guestfs_create();
        guestfs_add_drive(guestg, wombatvariable.casename.toStdString().c_str());
        guestfs_launch(guestg);
        guestfs_mount(guestg, "dev/sda1", wombatvariable.tmpmntpath.toStdString().c_str());
        */
        OpenCaseMountFinished(0, QProcess::NormalExit);
    }
}

void WombatForensics::OpenCaseMountFinished(int exitcode, QProcess::ExitStatus exitstatus)
{
    if(exitcode)
    {
    }
    if(exitstatus)
    {
    }
    wombatvariable.iscaseopen = true;
    logfile.setFileName(wombatvariable.tmpmntpath + "msglog");
    msglog->clear();
    InitializeCheckState();
    ui->actionAdd_Evidence->setEnabled(true);
    //autosavetimer->start(10000); // 10 seconds in milliseconds for testing purposes
    //autosavetimer->start(600000); // 10 minutes in milliseconds for a general setting for real.
    QDir eviddir = QDir(wombatvariable.tmpmntpath);
    QString tmpstr = "";
    QStringList foundlist = eviddir.entryList(QStringList(QString("*.p*.f*.a*")), QDir::Files | QDir::NoSymLinks);
    filesfound = foundlist.count();
    filecountlabel->setText("Found: " + QString::number(filesfound));
    QStringList files = eviddir.entryList(QStringList(QString("*.evid.*")), QDir::Files | QDir::NoSymLinks);
    for(int i=0; i < files.count(); i++)
    {
        tmpstr = "";
        wombatvariable.fullpathvector.clear();
        wombatvariable.evidencename = files.at(i).split(".").at(0) + QString(".") + files.at(i).split(".").at(1);
        QFile evidfile(QString(wombatvariable.tmpmntpath + files.at(i)));
        evidfile.open(QIODevice::ReadOnly | QIODevice::Text);
        tmpstr = evidfile.readLine();
        evidfile.close();
        int partcount = tmpstr.split(",").at(3).split("|").size();
        wombatvariable.segmentcount = partcount;
        wombatvariable.imgtype = (TSK_IMG_TYPE_ENUM)(tmpstr.split(",").first().toInt());
        for(int j=0; j < partcount; j++)
            wombatvariable.fullpathvector.push_back(tmpstr.split(",").at(3).split("|").at(j).toStdString());
        PrepareEvidenceImage();
        evidcnt++;
    }
    listeditems.clear();
    treefile.setFileName(wombatvariable.tmpmntpath + "treemodel");
    treefile.open(QIODevice::ReadOnly | QIODevice::Text);
    QStringList tmplist = QString(treefile.readAll()).split("\n", QString::SkipEmptyParts);
    if(tmplist.count() > 0)
    {
        treefile.reset();
        treenodemodel = new TreeNodeModel(treefile.readAll());
        treefile.close();
        for(int i=0; i < tmplist.count(); i++)
        {
            if(tmplist.at(i).split(",").first().split("-").count() == 5)
                listeditems.append(tmplist.at(i).split(",").first());
        }
        ui->dirTreeView->setModel(treenodemodel);
        connect(treenodemodel, SIGNAL(CheckedNodesChanged()), this, SLOT(UpdateCheckCount()));
        //connect(treenodemodel, SIGNAL(layoutChanged()), this, SLOT(ResizeColumns()));
        connect(ui->dirTreeView->selectionModel(), SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)), this, SLOT(SelectionChanged(const QItemSelection &, const QItemSelection &)));
        QModelIndexList indexlist = treenodemodel->match(treenodemodel->index(0, 0, QModelIndex()), Qt::DisplayRole, QVariant(InitializeSelectedState()), -1, Qt::MatchFlags(Qt::MatchExactly | Qt::MatchRecursive));
        UpdateCheckCount();
        if(indexlist.count() > 0)
            ui->dirTreeView->setCurrentIndex(indexlist.at(0));
        else
            ui->dirTreeView->setCurrentIndex(treenodemodel->index(0, 0, QModelIndex()));
    }
    else
        treefile.close();
    if(ui->dirTreeView->model() != NULL)
    {
        ui->actionRemove_Evidence->setEnabled(true);
        ui->actionSaveState->setEnabled(true);
        ui->actionDigDeeper->setEnabled(true);
        //hexrocker->setEnabled(true);
    }
    QApplication::restoreOverrideCursor();
    LogMessage("Case was Opened Successfully");
    StatusUpdate("Ready");
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
        StatusUpdate("Loading Hex Contents...");
        QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
        LoadHexContents();
        QApplication::restoreOverrideCursor();
        StatusUpdate("Ready");
        if(propertywindow->isVisible())
            UpdateProperties();
    }
}

void WombatForensics::TreeContextMenu(const QPoint &pt)
{
    QModelIndex index = ui->dirTreeView->indexAt(pt);
    if(index.isValid())
    {
        actionitem = static_cast<TreeNode*>(index.internalPointer());
        if(index.sibling(index.row(), 0).data().toString().split("-").count() == 4) // file
            treemenu->exec(ui->dirTreeView->mapToGlobal(pt));
    }
}

void WombatForensics::ImgHexMenu(const QPoint &pt)
{
    if(ui->hexview->selectionToReadableString().size() > 0)
        selectionmenu->exec(ui->hexview->mapToGlobal(pt));
    /*
    if(hexselection.compare("") != 0)
        selectionmenu->exec(hexwidget->mapToGlobal(pt));
    */
}

void WombatForensics::UpdateDataTable()
{
    StatusUpdate("Processing Complete");
    LogMessage("Evidence has been successfully added");
    StatusUpdate("Evidence ready");
}

void WombatForensics::PrepareEvidenceImage()
{
    qDebug() << "evidnecename:" << QString::fromStdString(wombatvariable.fullpathvector.at(0));
    QString xmntstr = "";
    if(wombatvariable.segmentcount > 1)
    {
        int digitcount = SegmentDigits(wombatvariable.segmentcount);

        xmntstr = "xmount --in ";

        if(TSK_IMG_TYPE_ISAFF(wombatvariable.imgtype))
        {
            xmntstr += "aff ";
            xmntstr += QString::fromStdString(wombatvariable.fullpathvector.at(0)).split(".").first() + ".A";
            qDebug() << "launch xmount with imgname.A and" << digitcount << "?'s.";
        }
        else if(TSK_IMG_TYPE_ISEWF(wombatvariable.imgtype))
        {
            xmntstr += "ewf ";
            xmntstr += QString::fromStdString(wombatvariable.fullpathvector.at(0)).split(".").first() + ".E";
            qDebug() << "launch xmount with imgname.E and" << digitcount << "?'s.";
        }
        else if(TSK_IMG_TYPE_ISRAW(wombatvariable.imgtype))
        {
            xmntstr += "raw ";
            xmntstr += QString::fromStdString(wombatvariable.fullpathvector.at(0)).split(".").first() + ".";
            qDebug() << "open with xmount with imgname. and" << digitcount << "?'s.";
        }
        else
        {
            qDebug() << "image format:" << tsk_img_type_toname(wombatvariable.imgtype) << "not supported";
        }
        if(wombatvariable.segmentcount > 1)
        {
            for(int i=0; i < digitcount; i++)
                xmntstr += "?";
        }
        else
            xmntstr += "??";
        xmntstr += " --out raw " + wombatvariable.imgdatapath;
    }
    else
    {
        xmntstr = "xmount --in ";
        if(TSK_IMG_TYPE_ISAFF(wombatvariable.imgtype))
        {
            xmntstr += "aff ";
        }
        else if(TSK_IMG_TYPE_ISEWF(wombatvariable.imgtype))
        {
            xmntstr += "ewf ";
        }
        else if(TSK_IMG_TYPE_ISRAW(wombatvariable.imgtype))
        {
            xmntstr += "raw ";
        }
        else
        {
            qDebug() << "unsupported format:" << tsk_img_type_toname(wombatvariable.imgtype);
        }
        xmntstr += QString::fromStdString(wombatvariable.fullpathvector.at(0)) + " --out raw " + wombatvariable.imgdatapath;

    }
    qDebug() << "xmntstr:" << xmntstr;
    QProcess::execute(xmntstr);
    /*
     *
     * NEED TO REIMPLEMENT THIS WHOLE FUCNTIONALITY WITH THE NEW EDITOR. WILL LEVERAGE EWFMOUNT TO MOUNT AN E01 AS VIRTUAL DD
     * THEN I'LL SIMPLY LOAD THAT FILE IN THE IMAGE HEX VIEWER. FOR FILES, PARTITIONS, ETC, I WILL SIMPLY GO TO THE OFFSET THE
     * SLEUTHKIT PROVIDES.
     *
     * ALSO CALL AFFMOUNT FOR THEIR IMAGES... USE TSK TO DETERMINE THE IMAGE TYPE...
     * THEN FOR THE FILEHEXVIEWER, IT WILL SIMPLY LOAD THE FILE HEX FOR FILES ONLY... THOSE GET WRITTEN TO TMP FILE PROBABLY
     *
     * THEN I NEED TO COLOR CODING IN THE HEXEDITOR, PROBABLY... CAN GET IT TO WORK...
     *
     */ 

    // FIXING THIS CODE SHOULD ENABLE REMOVAL OF THE TSKOBJPTR AND TSKOBJ AND TSKVARIABLE.H. I SHOULDN'T NEED TO PASS ANYTHING
    // COMPLEX OTHER THAN THE VARIABLES MENTIONED BELOW

    // PROCESS TO OPEN A FILE...
    // 1. GET THE OFFSET, FILE SIZE, AND BLOCKS/MFT ENTRY/ADS ATTRIBUTE FOR THE FILE...
    // 2. SET HEX EDITOR TO THE NECESSARY OFFSET FOR START OF THE FILE
    // 3. USE BLOCKS TO COLOR CODE THE FILE CONTENT BLUE, (BLOCK TOTAL * BLOCK SIZE) - FILE SIZE TO COLOR SLACK RED
    // 3A. IF I CAN'T COLOR CODE THE CONTENT DISPLAY, MIGHT WANT TO POPUP INFORMATION IN EDITOR WHICH SHOWS INFO FOR FILE...
    // 4. FOR FILEHEXVIEWER, JUST WRITE THE FILE TO TMP FILE AND THEN LOAD IN EDITOR. COLOR CODE THE SLACK USING ABOVE FORMULA SLACK.
 
}

void WombatForensics::UpdateStatus()
{
    StatusUpdate("Preparing Evidence Image...");
    LogMessage("Preparing Evidence Image...");
    PrepareEvidenceImage();
    StatusUpdate("Evidence Image Ready...");
    LogMessage("Evidence Image Ready...");
    StatusUpdate("Building Initial Evidence Tree...");
    LogMessage("Building Initial Evidence Tree...");
    treefile.close();
    listeditems.clear();
    treefile.open(QIODevice::ReadOnly | QIODevice::Text);
    QStringList tmplist = QString(treefile.readAll()).split("\n");
    treefile.reset();
    if(ui->dirTreeView->model() != NULL)
        delete treenodemodel;
    treenodemodel = new TreeNodeModel(treefile.readAll());
    treefile.close();
    for(int i=0; i < tmplist.count(); i++)
    {
        if(tmplist.at(i).split(",").first().split("-").count() == 5)
            listeditems.append(tmplist.at(i).split(",").first());
    }
    ui->dirTreeView->setModel(treenodemodel);
    connect(treenodemodel, SIGNAL(CheckedNodesChanged()), this, SLOT(UpdateCheckCount()));
    connect(ui->dirTreeView->selectionModel(), SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)), this, SLOT(SelectionChanged(const QItemSelection &, const QItemSelection &)));
    UpdateCheckCount();
    readfileinfo = NULL;
    tsk_fs_close(readfsinfo);
    readfsinfo = NULL;
    tsk_vs_close(readvsinfo);
    readvsinfo = NULL;
    tsk_img_close(readimginfo);
    readimginfo = NULL;
    evidcnt++;
    volcnt = 0;
    partint = 0;
    QModelIndexList indexlist = treenodemodel->match(treenodemodel->index(0, 0, QModelIndex()), Qt::DisplayRole, QVariant(InitializeSelectedState()), -1, Qt::MatchFlags(Qt::MatchExactly | Qt::MatchRecursive));
    if(indexlist.count() > 0)
        ui->dirTreeView->setCurrentIndex(indexlist.at(0));
    else
        ui->dirTreeView->setCurrentIndex(treenodemodel->index(0, 0, QModelIndex()));
    ui->actionRemove_Evidence->setEnabled(true);
    ui->actionSaveState->setEnabled(true);
    ui->actionDigDeeper->setEnabled(true);
    //hexrocker->setEnabled(true);
    //cancelthread->close();
    LogMessage("Processing Complete.");
    StatusUpdate("Evidence ready");
}

void WombatForensics::UpdateDigging()
{
    LogMessage("Digging Complete");
    StatusUpdate("Evidence ready");
}

void WombatForensics::AddEvidence()
{
    wombatvariable.fullpathvector.clear();
    wombatvariable.itemcount = 0;
    int isnew = 1;
    QStringList tmplist = QFileDialog::getOpenFileNames(this, tr("Select Evidence Image(s)"), QDir::homePath());
    if(tmplist.count())
    {
        wombatvariable.evidencename = tmplist.at(0).split("/").last();
        if(isnew == 1)
        {
            for(int i=0; i < tmplist.count(); i++)
                wombatvariable.fullpathvector.push_back(tmplist.at(i).toStdString());
            wombatvariable.itemcount = tmplist.count();
            LogMessage("Start Adding Evidence");
            QFuture<void> tmpfuture = QtConcurrent::run(InitializeEvidenceStructure);
            sqlwatcher.setFuture(tmpfuture);
        }
        else
            DisplayError("3.0", "Evidence already exists in the case", "Add Evidence Cancelled");
    }
}

void WombatForensics::UpdateProperties()
{
    QFile propfile;
    propertylist.clear();
    if(selectedindex.sibling(selectedindex.row(), 0).data().toString().split("-").count() == 1) // evidence image
    {
        propfile.setFileName(wombatvariable.tmpmntpath + selectedindex.sibling(selectedindex.row(), 1).data().toString().split("evid").at(0) + ".eprop." + selectedindex.sibling(selectedindex.row(), 0).data().toString().mid(1));
    }
    QDir eviddir = QDir(wombatvariable.tmpmntpath);
    QStringList evidfiles = eviddir.entryList(QStringList("*.evid." + selectedindex.sibling(selectedindex.row(), 0).data().toString().split("-").at(0).mid(1)), QDir::NoSymLinks | QDir::Files);
    QString evidfilename = evidfiles.at(0).split(".evid").at(0);
    QFile evidfile(wombatvariable.tmpmntpath + wombatvariable.evidencename.split(".evid").at(0) + ".evid." + wombatvariable.selectedid.split("-").at(0).mid(1));
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
        QString tmpfvalue = "";
        if(selectedindex.sibling(selectedindex.row(), 0).data().toString().split("-").last().contains(":"))
            tmpfvalue = selectedindex.sibling(selectedindex.row(), 0).data().toString().split("-").last().split(":").at(0) + QString("-") + selectedindex.sibling(selectedindex.row(), 0).data().toString().split("-").last().split(":").at(1);
        else
            tmpfvalue = selectedindex.sibling(selectedindex.row(), 0).data().toString().split("-").last();
        propfile.setFileName(wombatvariable.tmpmntpath + evidfilename + "." + selectedindex.sibling(selectedindex.row(), 0).data().toString().split("-").at(2) + "." + tmpfvalue + ".prop");
    }
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
    /*
    int digitcount = SegmentDigits(wombatvariable.segmentcount);

    if(TSK_IMG_TYPE_ISAFF(wombatvariable.imgtype))
    {
        qDebug() << "launch xmount with imgname.A and" << digitcount << "?'s.";
    }
    else if(TSK_IMG_TYPE_ISEWF(wombatvariable.imgtype))
    {
        qDebug() << "launch xmount with imgname.E and" << digitcount << "?'s.";
    }
    else if(TSK_IMG_TYPE_ISRAW(wombatvariable.imgtype))
    {
        if(wombatvariable.segmentcount > 1) // multi segment
        {
            qDebug() << "open with xmount with imgname. and" << digitcount << "?'s.";
        }
        else
        {
            qDebug() << "just load the image in the editor, no modifications needed";
        }
    }
    else
    {
        qDebug() << "image format:" << tsk_img_type_toname(wombatvariable.imgtype) << "not supported";
    }
    */

    /*
     *
     * NEED TO REIMPLEMENT THIS WHOLE FUCNTIONALITY WITH THE NEW EDITOR. WILL LEVERAGE EWFMOUNT TO MOUNT AN E01 AS VIRTUAL DD
     * THEN I'LL SIMPLY LOAD THAT FILE IN THE IMAGE HEX VIEWER. FOR FILES, PARTITIONS, ETC, I WILL SIMPLY GO TO THE OFFSET THE
     * SLEUTHKIT PROVIDES.
     *
     * ALSO CALL AFFMOUNT FOR THEIR IMAGES... USE TSK TO DETERMINE THE IMAGE TYPE...
     * THEN FOR THE FILEHEXVIEWER, IT WILL SIMPLY LOAD THE FILE HEX FOR FILES ONLY... THOSE GET WRITTEN TO TMP FILE PROBABLY
     *
     * THEN I NEED TO COLOR CODING IN THE HEXEDITOR, PROBABLY... CAN GET IT TO WORK...
     *
     */ 

    // FIXING THIS CODE SHOULD ENABLE REMOVAL OF THE TSKOBJPTR AND TSKOBJ AND TSKVARIABLE.H. I SHOULDN'T NEED TO PASS ANYTHING
    // COMPLEX OTHER THAN THE VARIABLES MENTIONED BELOW

    // PROCESS TO OPEN A FILE...
    // 1. GET THE OFFSET, FILE SIZE, AND BLOCKS/MFT ENTRY/ADS ATTRIBUTE FOR THE FILE...
    // 2. SET HEX EDITOR TO THE NECESSARY OFFSET FOR START OF THE FILE
    // 3. USE BLOCKS TO COLOR CODE THE FILE CONTENT BLUE, (BLOCK TOTAL * BLOCK SIZE) - FILE SIZE TO COLOR SLACK RED
    // 3A. IF I CAN'T COLOR CODE THE CONTENT DISPLAY, MIGHT WANT TO POPUP INFORMATION IN EDITOR WHICH SHOWS INFO FOR FILE...
    // 4. FOR FILEHEXVIEWER, JUST WRITE THE FILE TO TMP FILE AND THEN LOAD IN EDITOR. COLOR CODE THE SLACK USING ABOVE FORMULA SLACK.

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
    wombatvariable.selectedid = selectedindex.sibling(selectedindex.row(), 0).data().toString(); // mod object id
    blockstring = "";
    QString tmpstr = "";
    QStringList evidlist;
    evidlist.clear();
    // get offset and blockaddress list from properties file...
    // epropfile(wombatvariable.tmpmntpath + wombatvariable.evidencename + ".eprop." + QString::number(evidcnt));
    // vpropfile(wombatvariable.tmpmntpath + wombatvariable.evidencename + ".volprop");
    // fspropfile(wombatvariable.tmpmntpath + wombatvariable.evidencename + ".partprop.p" + QString::number(partint));
    // filepropfile.setFileName(wombatvariable.tmpmntpath + wombatvariable.evidencename + ".p" + QString::number(partint) + ".f" + QString::number(curfileinfo->meta->addr) + ".prop");

    QString datastring = wombatvariable.imgdatapath + wombatvariable.evidencename.split(".").first() + ".dd";
    //qDebug() << "datastring:" << datastring;
    casedatafile.setFileName(datastring);
    //ui->hexview->SetColorInformation();
    ui->hexview->setData(casedatafile);

    // determine offset location in the editor
    if(wombatvariable.selectedid.split("-").count() == 1) // image file
    {
        ui->hexview->setCursorPosition(0);
        //emit ui->hexview->currentAddressChanged(0);
        /*
        wombatvariable.evidencename = selectedindex.sibling(selectedindex.row(), 1).data().toString(); // current evidence name
        QFile evidfile(wombatvariable.tmpmntpath + wombatvariable.evidencename + ".evid." + wombatvariable.selectedid.mid(1));
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
        for(uint i =0; i < tmpvec.size(); i++)
        {
            tskobjptr->imagepartspath[i] = tmpvec[i].c_str();
        }
        

        testfile.setFileName(tskobjptr->imagepartspath[0]);
        

        //testdevice = new EvidenceDevice(tskobjptr->partcount, tskobjptr->imagepartspath);

        /*
        tskobjptr->readimginfo = tsk_img_open(tskobjptr->partcount, tskobjptr->imagepartspath, TSK_IMG_TYPE_DETECT, 0);
        if(tskobjptr->readimginfo == NULL)
        {
            qDebug() << tsk_error_get_errstr();
            LogMessage("Image opening error");
        }
        */
        //free(tskobjptr->imagepartspath);
    }
    else if(wombatvariable.selectedid.split("-").count() == 2) // volume file
    {
        QDir eviddir = QDir(wombatvariable.tmpmntpath);
        QStringList evidfiles = eviddir.entryList(QStringList("*.evid." + wombatvariable.selectedid.split("-").at(0).mid(1)), QDir::NoSymLinks | QDir::Files);
        wombatvariable.evidencename = evidfiles.at(0);
        QStringList vollist;
        vollist.clear();
        QFile volfile(wombatvariable.tmpmntpath + wombatvariable.evidencename.split(".evid").at(0) + ".vol");
        volfile.open(QIODevice::ReadOnly);
        tmpstr = volfile.readLine();
        volfile.close();
        vollist = tmpstr.split(",");
        ui->hexview->setCursorPosition(vollist.at(4).toInt()*2);
        /*
        tskobjptr->offset = vollist.at(4).toInt();
        tskobjptr->length = selectedindex.sibling(selectedindex.row(), 3).data().toULongLong();
        tskobjptr->imglength = selectedindex.sibling(selectedindex.row(), 3).data().toULongLong();
        tskobjptr->sectsize = vollist.at(3).toInt();
        tskobjptr->blocksize = vollist.at(3).toInt();
        */
    }
    else if(wombatvariable.selectedid.split("-").count() == 3) // partition file
    {
        QDir eviddir = QDir(wombatvariable.tmpmntpath);
        QStringList evidfiles = eviddir.entryList(QStringList("*.evid." + wombatvariable.selectedid.split("-").at(0).mid(1)), QDir::NoSymLinks | QDir::Files);
        wombatvariable.evidencename = evidfiles.at(0);
        /*
        QFile evidfile(wombatvariable.tmpmntpath + wombatvariable.evidencename.split(".evid").at(0) + ".evid." + wombatvariable.selectedid.split("-").at(0).mid(1));
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
        for(uint i =0; i < tmpvec.size(); i++)
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
        */
        QStringList partlist = eviddir.entryList(QStringList(wombatvariable.evidencename.split(".evid").at(0) + ".part." + wombatvariable.selectedid.split("-").at(2).mid(1)), QDir::NoSymLinks | QDir::Files);
        QFile partfile(wombatvariable.tmpmntpath + partlist.at(0));
        partfile.open(QIODevice::ReadOnly);
        tmpstr = partfile.readLine();
        partfile.close();
        partlist = tmpstr.split(",");
        ui->hexview->setCursorPosition(partlist.at(4).toULongLong()*2);
        /*
        tskobjptr->offset = partlist.at(4).toULongLong();
        tskobjptr->fsoffset = partlist.at(4).toULongLong();
        tskobjptr->objecttype = 4;
        tskobjptr->length = partlist.at(1).toULongLong();
        tskobjptr->sectsize = evidlist.at(2).toInt();
        tskobjptr->blocksize = partlist.at(6).toInt();
        */
    }
    else if(wombatvariable.selectedid.split("-").count() == 4) // file file
    {
        QDir eviddir = QDir(wombatvariable.tmpmntpath);
        QStringList evidfiles = eviddir.entryList(QStringList("*.evid." + wombatvariable.selectedid.split("-").at(0).mid(1)), QDir::NoSymLinks | QDir::Files);
        wombatvariable.evidencename = evidfiles.at(0);
        /*
        QFile evidfile(wombatvariable.tmpmntpath + wombatvariable.evidencename.split(".evid").at(0) + ".evid." + wombatvariable.selectedid.split("-").at(0).mid(1));
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
        for(uint i =0; i < tmpvec.size(); i++)
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
        */
        tmpstr = "";
        QStringList partlist;
        partlist.clear();
        QStringList partfiles = eviddir.entryList(QStringList(wombatvariable.evidencename.split(".evid").at(0) + ".part." + wombatvariable.selectedid.split("-").at(2).mid(1)), QDir::NoSymLinks | QDir::Files);
        QFile partfile(wombatvariable.tmpmntpath + partfiles.at(0));
        partfile.open(QIODevice::ReadOnly);
        tmpstr = partfile.readLine();
        partfile.close();
        partlist = tmpstr.split(",");
        QStringList filelist;
        filelist.clear();
        QString tmpfilename = "";
        if(wombatvariable.selectedid.split("-").at(3).mid(1).contains(":"))
            tmpfilename = wombatvariable.selectedid.split("-").at(3).mid(1).split(":").at(0) + QString("-") + wombatvariable.selectedid.split("-").at(3).mid(1).split(":").at(1);
        else
            tmpfilename = wombatvariable.selectedid.split("-").at(3).mid(1);
        qDebug() << "f value:" << tmpfilename;
        QStringList filefiles = eviddir.entryList(QStringList(wombatvariable.evidencename.split(".evid").at(0) + ".p" + wombatvariable.selectedid.split("-").at(2).mid(1) + ".f" + tmpfilename + ".a*"), QDir::NoSymLinks | QDir::Files);
        QFile filefile;
        if(filefiles.count() == 1)
            filefile.setFileName(wombatvariable.tmpmntpath + filefiles.at(0));
        else if(filefiles.count() > 1)
        {
            for(int i = 0; i < filefiles.count(); i++)
            {
                if(filefiles.at(i).split(".a").at(1).toInt() == selectedindex.parent().sibling(selectedindex.parent().row(), 0).data().toString().split("-f").at(1).toInt())
                {
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
        filefile.close();
        filelist = tmpstr.split(",");
        /*
        tskobjptr->readfsinfo = tsk_fs_open_img(tskobjptr->readimginfo, partlist.at(4).toULongLong(), TSK_FS_TYPE_DETECT);
        tskobjptr->fsoffset = tskobjptr->readfsinfo->offset;
        tskobjptr->blocksize = tskobjptr->readfsinfo->block_size;
        tskobjptr->objecttype = 5;
        tskobjptr->address = wombatvariable.selectedid.split("-").at(3).mid(1).toInt();
        tskobjptr->length = selectedindex.sibling(selectedindex.row(), 3).data().toULongLong();
        tskobjptr->offset = 0;
        */
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
	/*

        char* ibuffer = new char[tskexternalptr->readfileinfo->meta->size];
        // WILL NEED TO FIGURE OUT IF ITS AN ATTRIBUTE OR NOT AND HANDLE THE IF BELOW...
        //if(objtype == 5)
        filelen = tsk_fs_file_read(tskexternalptr->readfileinfo, 0, ibuffer, tskexternalptr->readfileinfo->meta->size, TSK_FS_FILE_READ_FLAG_NONE);
        //else
        (new QDir())->mkpath(wombatvariable.tmpfilepath);
        QString tmpstring = wombatvariable.tmpfilepath + selectedindex.sibling(selectedindex.row(), 0).data().toString() + "-tmp";
        QFile tmpfile(tmpstring);
        if(tmpfile.open(QIODevice::WriteOnly))
        {
            QDataStream outbuffer(&tmpfile);
            outbuffer.writeRawData(ibuffer, filelen);
            tmpfile.close();
        }
        delete[] ibuffer;


	*/
        //qDebug() << "partfile fstype:" << partlist.at(0);
        // IN IT'S CURRENT LAYOUT:
        // RESIDENT NTFS FILE WORKS, RESIDENT NTFS FILE ADS FAILS
        // RESIDENT DIR WORKS, RESIDENT DIR ADS FAILS
        // RESIDENT DIR FAILS, NON-RESIDENT DIR ADS WORKS
        // NON-RESIDENT FILE WORKS, NON-RESIDENT FILE ADS WORKS
        // RESIDENT DIR FAILS, NO ADS ASSOCIATED DATA
        //
        // THE PIECES ARE THERE, I JUST NEED TO IF/ELSE IT CORRECTLY TO CAPTURE THE RIGHT CONDITIONS
        if(partlist.at(0).toInt() == TSK_FS_TYPE_NTFS_DETECT)
        {
	    if(filelist.at(1).toInt() == 5) // regular file
	    {
	    }
	    else if(filelist.at(1).toInt() == 3) // directory
	    {
	    }
	    else
		qDebug() << "display/log error here.";
            // NOW DO THE IS A FILE OR DIRECTORY AND ACT ACCORDINGLY...
            // THEN DO DOES IT HAVE BLOCKS OR NOT FOR FILES, JUST DO THE RESIDENT FOR DIRECTORIES.
        }
        else
        {
            // NOW DO THE IS IT BLOCKS OR NOT, THEN ACT ACCORDINGLY WITH 
            qDebug() << "it's not ntfs?";
        }
        if(blockstring.compare("") != 0 && blockstring.compare("0^^") != 0)
        {
            if(filelist.at(1).toInt() == 5) // regular file
            {
            // fsoffset, blocksize, blockstring, residentoffset, byteoffset, file length
                ui->hexview->SetColorInformation(partlist.at(4).toULongLong(), partlist.at(6).toULongLong(), blockstring, residentstring, bytestring, selectedindex.sibling(selectedindex.row(), 3).data().toULongLong(), 0);
                ui->hexview->setCursorPosition(bytestring.toULongLong()*2);
            }
            else if(filelist.at(1).toInt() == 3) // directory
            {
                // NEED TO DETERMINE IF IT IS NTFS.. IF IT IS, THEN DON'T USE THE BELOW AND RATHER USE THE NTFS ELSE DOUBLE BELOW...
                ui->hexview->SetColorInformation(partlist.at(4).toULongLong(), partlist.at(6).toULongLong(), blockstring, residentstring, bytestring, selectedindex.sibling(selectedindex.row(), 3).data().toULongLong(), 0);
                ui->hexview->setCursorPosition(bytestring.toULongLong()*2);
                // this will screw up other fs, i will need to determine if it's ntfs or not to process accordingly...
            }
            else
                qDebug() << "its an other:" << filelist.at(1).toInt();
        }
        else // NTFS
        {
            //if(filelist.at(1).toInt() == 5) // regular file
            //{
            unsigned long long resval = 0;
            unsigned int off1 = 0;
            unsigned int attrtype = 0;
	    QByteArray rbuf = ui->hexview->dataAt(residentstring.toULongLong(), 1024);
            uint8_t* mftoff[2];
            uint8_t* mftlen[4];
            uint8_t attype = 0;
            mftoff[0] = (unsigned char*)rbuf.at(20);
            mftoff[1] = (unsigned char*)rbuf.at(21);
            off1 = tsk_getu16(TSK_LIT_ENDIAN, mftoff);
            qDebug() << rbuf.at(0) << rbuf.mid(0, 4) << off1;
            if(filelist.at(1).toInt() == 5) // regular file
            {
                while(attype < 127)
                {
                    attype = (unsigned char)rbuf.at(off1);
                    if(attype == 128)
                        break;
                    mftlen[0] = (unsigned char*)rbuf.at(off1 + 4);
                    mftlen[1] = (unsigned char*)rbuf.at(off1 + 5);
                    mftlen[2] = (unsigned char*)rbuf.at(off1 + 6);
                    mftlen[3] = (unsigned char*)rbuf.at(off1 + 7);
                    off1 = off1 + tsk_getu32(TSK_LIT_ENDIAN, mftlen);
                }
                mftoff[0] = (unsigned char*)rbuf.at(off1 + 20);
                mftoff[1] = (unsigned char*)rbuf.at(off1 + 21);
                off1 = off1 + tsk_getu16(TSK_LIT_ENDIAN, mftoff);
                qDebug() << "final offset value:" << off1;
                resval = residentstring.toULongLong() + off1;
                qDebug() << "resident data offset:" << resval;
            }
            else if(filelist.at(1).toInt() == 3) // directory
            {
                while(attype < 127)
                {
                    qDebug() << "off1:" << off1;
                    qDebug() << "residentstring:" << residentstring;
                    qDebug() << "attype:" << (unsigned char)rbuf.at(off1);
                    attype = (unsigned char)rbuf.at(off1);
                    if(attype == 128 || attype == 144)
                        break;
                    mftlen[0] = (unsigned char*)rbuf.at(off1 + 4);
                    mftlen[1] = (unsigned char*)rbuf.at(off1 + 5);
                    mftlen[2] = (unsigned char*)rbuf.at(off1 + 6);
                    mftlen[3] = (unsigned char*)rbuf.at(off1 + 7);
                    qDebug() << "attr len:" << tsk_getu32(TSK_LIT_ENDIAN, mftlen);
                    off1 = off1 + tsk_getu32(TSK_LIT_ENDIAN, mftlen);
                    qDebug() << "new off1:" << off1;
                }
                qDebug() << "final countdown";
                qDebug() << "final off1 start:" << off1;
                qDebug() << "off1+4" << (unsigned char)rbuf.at(off1 + 4);
                if(attype == 128)
                {
                    // manual way...
                    uint8_t val0 = (unsigned char)rbuf.at(off1 + 4);
                    uint8_t val1 = (unsigned char)rbuf.at(off1 + 5);
                    uint8_t val2 = (unsigned char)rbuf.at(off1 + 6);
                    uint8_t val3 = (unsigned char)rbuf.at(off1 + 7);
                    qDebug() << val0 << val1 << val2 << val3;
                    int manuel = (val3 << 24) | (val2 << 16) | (val1 << 8) | val0;
                    qDebug() << "manuel:" << manuel;
                    off1 = off1 + manuel;
                    qDebug() << "new off1:" << off1;
                    qDebug() << "get data length to jump to 144";
                    attype = (unsigned char)rbuf.at(off1);
                    if(attype == 144)
                    {
                        qDebug() << "yeah it's right!";
                        off1 = off1 + 32;
                    }
                }
                else if(attype == 144)
                {
                    qDebug() << "off1 is our man.";
                    off1 = off1 + 32;
                }
                else
                    qDebug() << "what happened?";
                resval = residentstring.toULongLong() + off1;
                qDebug() << "resident data offset:" << resval;
            }
            else
                resval = residentstring.toULongLong();

            ui->hexview->SetColorInformation(partlist.at(4).toULongLong(), partlist.at(6).toULongLong(), blockstring, QString::number(resval), bytestring, selectedindex.sibling(selectedindex.row(), 3).data().toULongLong(), off1);
    	    ui->hexview->setCursorPosition(residentstring.toULongLong()*2);
            //qDebug() << "its a file";
            //}
            //else if(filelist.at(1).toInt() == 3) // directory
            //{
            //    qDebug() << "its a dir";
            //}
            //else
            //{
            //    qDebug() << "its an other:" << filelist.at(1).toInt();
            //}
        }
        /*
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

       if(filelist.at(11).toULongLong() == 0)
            tskobjptr->readfileinfo = tsk_fs_file_open_meta(tskobjptr->readfsinfo, NULL, tskobjptr->address);

        QStringList tmpchildren = GetChildFiles("*.a" + wombatvariable.selectedid.split("-").at(3).mid(1));

        if(tskobjptr->readfsinfo->ftype == TSK_FS_TYPE_NTFS_DETECT)
        {
        }
        */
    }
    ui->hexview->ensureVisible();

    if(wombatvariable.selectedid.split("-").count() <= 4) // image file
    {

        //ui->hexview->setData(testfile);



        //qint64 bytesread = 0;
        //qDebug() << "e01 size:" << tskobjptr->readimginfo->size;
        //char* imgbuffer = new char[tskobjptr->readimginfo->size];
        //bytesread = testdevice->readData(imgbuffer, tskobjptr->readimginfo->size);
        //char* imgbuffer = new char[testdevice->ImageSize()];
        //bytesread = testdevice->readData(imgbuffer, testdevice->ImageSize());
        //qDebug() << "imgbuffer size:" << strlen(imgbuffer);
        //qDebug() << "bytes read:" << bytesread;
        //if(bytesread > 0)
        //ui->hexview->setData(*testdevice);
        /*
        off_t retval = 0;
        char* imgbuffer = new char[tskobjptr->readimginfo->size];
        retval = tsk_img_read(tskobjptr->readimginfo, 0, imgbuffer, tskobjptr->readimginfo->size);
        QByteArray imgarr = QByteArray::fromRawData(imgbuffer, tskobjptr->readimginfo->size);
        */
        //QBuffer imgbuf;
        //QBuffer imgbuf(&imgarr);
        //imgbuf.open(QIODevice::ReadOnly);
        //imgbuf.read(imgbuffer, tskobjptr->imglength);
        //hexview->clear();
        //ui->hexview->setData(imgbuf);
        //imgbuf.close();
        // this works, but using a bytearray loads whole thing into memory...
        // will either have to build my own chunck load from a bytearray or mount e01 to dd, so then i can
        // load it like a file and make use of QIODevice...

        /*
        ui->hexview->setData(imgarr);
        */

        //delete []imgbuffer;
        
        //QByteArray filedata = QByteArray::fromRawData(ibuffer, imglen);
        //retval = tsk_img_read(tskptr->readimginfo, pageIdx*_pageSize, (char*)_data[pageIdx], _pageSize);
        /*
        hexwidget->openimage();
        hexwidget->set1BPC();
        hexwidget->setBaseHex();
        hexwidget->SetTopLeft(0);
        */
    }
    else
    {
        /*
        hexwidget->SetTopLeft(tskobjptr->offset);
        if(wombatvariable.selectedid.split("-").count() == 4)
        {
            fileviewer->filehexview->openimage();
            fileviewer->filehexview->set1BPC();
            fileviewer->filehexview->setBaseHex();
            fileviewer->filehexview->SetTopLeft(0);
        }
        */
    }
}

void WombatForensics::CloseCurrentCase()
{
    if(ui->dirTreeView->model() != NULL)
    {
        UpdateSelectedState(selectedindex.sibling(selectedindex.row(), 0).data().toString());
        delete treenodemodel;
        evidcnt = 0;
        //autosavetimer->stop();
        UpdateCheckState();
    }
    setWindowTitle("WombatForensics");
    filesfound = 0;
    fileschecked = 0;
    filtercountlabel->setText("Filtered: 0");
    filecountlabel->setText("Found: " + QString::number(filesfound));
    checkedcountlabel->setText("Checked: " + QString::number(fileschecked));
    // WRITE MSGLOG TO FILE HERE...

    // UNMOUNT XMOUNT EVIDENCEIMAGEDATAFILE
    QString xunmntstr = "fusermount -u " + wombatvariable.imgdatapath;
    QProcess::execute(xunmntstr);

    QString unmntstr = "umount " + wombatvariable.tmpmntpath;
    //QString unmntstr = "sudo umount " + wombatvariable.tmpmntpath;
    QProcess::execute(unmntstr);

    // delete two link files
    if(QFileInfo::exists("/tmp/wombatforensics/mntpt"))
        QFile::remove("/tmp/wombatforensics/mntpt");
    if(QFileInfo::exists("/tmp/wombatforensics/currentwfc"))
        QFile::remove("/tmp/wombatforensics/currentwfc");

    /*
    guestfs_umount(guestg, wombatvariable.tmpmntpath.toStdString().c_str());
    guestfs_shutdown(guestg);
    guestfs_close(guestg);
    */

    StatusUpdate("Current Case was closed successfully");
    RemoveTmpFiles();
    wombatvariable.iscaseopen = false;
}

void WombatForensics::RemEvidence()
{
    // if an evidence item is not selected, then move selection to it and remove then it...
    // also need to remove any thumbnails in the thumbs.db associated with it...
    // DELETE TREEFILE AS WELL AS THE SINGLE FILeS *.p0.f*.a*
    // treefile.remove()
    // THEN I BUILD A NEW TREEFILE BASED OFF OF THE SINGLE FILES.
    QModelIndexList indexlist;
    QModelIndex curindex = selectedindex;
    if(selectedindex.sibling(selectedindex.row(), 0).data().toString().split("-").count() > 1)
    {
        indexlist = treenodemodel->match(treenodemodel->index(0, 0, QModelIndex()), Qt::DisplayRole, QVariant(selectedindex.sibling(selectedindex.row(), 0).data().toString().split("-").first()), -1, Qt::MatchFlags(Qt::MatchExactly | Qt::MatchRecursive));
        curindex = indexlist.at(0);
    }
    QDir eviddir = QDir(wombatvariable.tmpmntpath);
    QStringList evidfiles = eviddir.entryList(QStringList(QString(curindex.sibling(curindex.row(), 1).data().toString() + ".*")), QDir::NoSymLinks | QDir::Files);
    for(int i = 0; i < evidfiles.count(); i++)
    {
        eviddir.remove(evidfiles.at(i));
    }
    listeditems.clear();
    treefile.setFileName(wombatvariable.tmpmntpath + "treemodel");
    treefile.remove(); // delete treefile.
    // find evidence files
    // find volume files
    // find partition files
    // find files files
    evidfiles.clear();
    evidfiles = eviddir.entryList(QStringList(QString("*.p*.f*.a*")), QDir::Files | QDir::NoSymLinks);
    treefile.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream treeout(&treefile);
    for(int i=0; i < evidfiles.count(); i++)
    {
        QFile tmpfile(evidfiles.at(i));
        tmpfile.open(QIODevice::ReadOnly);
        QString tmpstr = tmpfile.readLine();
        QStringList tmplist = tmpstr.split(",", QString::SkipEmptyParts);
    }
    /*
    treefile.open(QIODevice::ReadOnly | QIODevice::Text);
    QStringList tmplist = QString(treefile.readAll()).split("\n", QString::SkipEmptyParts);
    treefile.close();
    treefile.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream treeout(&treefile);
    for(int i=0; i < tmplist.count(); i++)
    {
        if(!tmplist.at(i).split(",").first().contains(curindex.sibling(curindex.row(), 0).data().toString().split("-").first()))
        {
            listeditems.append(tmplist.at(i).split(",").first());
            treeout << tmplist.at(i) << endl;
        }
    }
    treefile.flush();
    treefile.close();
    qDebug() << "listeditems count:" << listeditems.count();
    QStringList foundlist = eviddir.entryList(QStringList(QString("*.p*.f*.a*")), QDir::Files | QDir::NoSymLinks);
    filesfound = foundlist.count();
    filecountlabel->setText("Found: " + QString::number(filesfound));
    qDebug() << "filesfound count:" << filesfound;
    QHashIterator<QString, bool> i(checkhash);
    while(i.hasNext())
    {
        i.next();
        if(i.key().contains(curindex.sibling(curindex.row(), 0).data().toString().split("-").first()))
            checkhash.remove(i.key());
    }
    UpdateCheckState();
    QFile selectfile(wombatvariable.tmpmntpath + "selectedstate");
    selectfile.open(QIODevice::WriteOnly);
    selectfile.write("");
    selectfile.close();
    delete treenodemodel;
    treefile.open(QIODevice::ReadOnly | QIODevice::Text);
    treenodemodel = new TreeNodeModel(treefile.readAll());
    treefile.close();
    ui->dirTreeView->setModel(treenodemodel);
    connect(treenodemodel, SIGNAL(CheckedNodesChanged()), this, SLOT(UpdateCheckCount()));
    //connect(treenodemodel, SIGNAL(layoutChanged()), this, SLOT(ResizeColumns()));
    connect(ui->dirTreeView->selectionModel(), SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)), this, SLOT(SelectionChanged(const QItemSelection &, const QItemSelection &)));
    evidcnt--;
    */
    StatusUpdate("Evidence Item Successfully Removed");
}

QStringList WombatForensics::GetFileLists(int filelisttype)
{
    QStringList tmplist;
    tmplist.clear();
    if(filelisttype == 1) // checked
    {
        QHashIterator<QString, bool> i(checkhash);
        while(i.hasNext())
        {
            i.next();
            if(i.value())
                tmplist.append(i.key());
        }
        return tmplist;
    }
    else if(filelisttype == 2) // all listed
        return listeditems;
    return tmplist;

}

void WombatForensics::ExportEvidence()
{
    totalcount = filesfound;
    totalchecked = fileschecked;
    exportcount = 0;
    exportdialog = new ExportDialog(this, totalchecked, totalcount);
    connect(exportdialog, SIGNAL(StartExport(int, bool, QString)), this, SLOT(ExportFiles(int, bool, QString)), Qt::DirectConnection);
    exportdialog->show();
}

void WombatForensics::FinishExport()
{
    cancelthread->close();
    LogMessage(QString("Export Completed with " + QString::number(errorcount) + " error(s)"));
    StatusUpdate("Exporting completed with " + QString::number(errorcount) + "error(s)");
}

void WombatForensics::ExportFiles(int etype, bool opath, QString epath)
{
    exporttype = etype;
    exportpath = epath;
    originalpath = opath;
    exportlist.clear();
    errorcount = 0;
    exportcount = 0;
    LogMessage("Started Exporting Evidence");
    if(etype == 0) // selected
    {
        exportlist.append(selectedindex.sibling(selectedindex.row(), 0).data().toString());
    }
    else
        exportlist = GetFileLists(etype);
    int curprogress = (int)((((float)exportcount)/(float)exportlist.count())*100);
    LogMessage("Exported: " + QString::number(exportcount) + " of " + QString::number(exportlist.count()) + " " + QString::number(curprogress) + "%");
    StatusUpdate("Exported: " + QString::number(exportcount) + " of " + QString::number(exportlist.count()) + " " + QString::number(curprogress) + "%");
    // cancellable map
    QFuture<void> tmpfuture = QtConcurrent::map(exportlist, ProcessExport);
    exportwatcher.setFuture(tmpfuture);
    cancelthread->show();
}

void WombatForensics::DigFiles(int dtype, QVector<int> doptions)
{
    digtype = dtype;
    digoptions = doptions;
    digfilelist.clear();
    LogMessage("Digging Deeper into Evidence");
    if(dtype == 0) // selected
    {
        digfilelist.append(selectedindex.sibling(selectedindex.row(), 0).data().toString());
    }
    else
        digfilelist = GetFileLists(dtype);
    for(int i = 0; i < digoptions.count(); i++)
    {
        if(digoptions.at(0) == 0) // Generate Thumbnails
        {
            StartThumbnails();
            //GenerateThumbnails();
        }
    }
    /*
    QFuture<void> tmpfuture = QtConcurrent::map(digfilelist, ProcessDig);
    digwatcher.setFuture(tmpfuture);
    cancelthread->show();
    */
    /*
    int curprogress = (int)((((float)digcount)/(float)digfilelist.count())*100);
    LogMessage("Dug: " + QString::number(digcount) + " of " + QString::number(digfilelist.count()) + " " + QString::number(curprogress) + "%");
    StatusUpdate("Dug: " + QString::number(digcount) + " of " + QString::number(digfilelist.count()) + " " + QString::number(curprogress) + "%");
    for(int i = 0; i < digfilelist.count(); i++)
    {
        QFuture<void> tmpfuture = QtConcurrent::run(this, &WombatForensics::ProcessDig, digfilelist.at(i));
        digwatcher.setFuture(tmpfuture);
    }
    */
}

void WombatForensics::ProcessDig(QString objectid)
{
    /*
    TSK_IMG_INFO* readimginfo;
    TSK_FS_INFO* readfsinfo;
    TSK_FS_FILE* readfileinfo;
    QString tmpstr = "";
    //QStringList evidlist;
    //evidlist.clear();
    QDir eviddir = QDir(wombatvariable.tmpmntpath);
    std::vector<std::string> pathvector;
    const TSK_TCHAR** imagepartspath;
    pathvector.clear();
    QString estring = objectid.split("-", QString::SkipEmptyParts).at(0);
    QString pstring = objectid.split("-", QString::SkipEmptyParts).at(2);
    QString fstring = objectid.split("-", QString::SkipEmptyParts).at(3);
    unsigned long long curaddress = objectid.split("-f").at(1).toULongLong();
    //unsigned long long curaddress = objectid.split("-f").at(1).split("-a").at(0).toULongLong(); 
    //qDebug() << "e p adr" << estring << pstring << curaddress;
    QStringList evidfiles = eviddir.entryList(QStringList("*.evid." + estring.mid(1)), QDir::NoSymLinks | QDir::Files);
    wombatvariable.evidenceobject.name = evidfiles.at(0);
    QFile evidfile(wombatvariable.tmpmntpath + wombatvariable.evidenceobject.name.split(".evid").at(0) + ".evid." + estring.mid(1));
    evidfile.open(QIODevice::ReadOnly);
    tmpstr = evidfile.readLine();
    int partcount = tmpstr.split(",").at(3).split("|").size();
    evidfile.close();
    for(int i=0; i < partcount; i++)
        pathvector.push_back(tmpstr.split(",").at(3).split("|").at(i).toStdString());
    imagepartspath = (const char**)malloc(pathvector.size()*sizeof(char*));
    for(int i=0; i < pathvector.size(); i++)
        imagepartspath[i] = pathvector[i].c_str();
    readimginfo = tsk_img_open(partcount, imagepartspath, TSK_IMG_TYPE_DETECT, 0);
    if(readimginfo == NULL)
    {
        qDebug() << tsk_error_get_errstr();
        LogMessage("Image opening error");
    }
    free(imagepartspath);
    tmpstr = "";
    QStringList partfiles = eviddir.entryList(QStringList(wombatvariable.evidenceobject.name.split(".evid").at(0) + ".part." + pstring.mid(1)), QDir::NoSymLinks | QDir::Files);
    //qDebug() << wombatvariable.evidenceobject.name.split(".evid").at(0) << ".part." << pstring.mid(1);
    QFile partfile(wombatvariable.tmpmntpath + partfiles.at(0));
    partfile.open(QIODevice::ReadOnly);
    tmpstr = partfile.readLine();
    partfile.close();
    readfsinfo = tsk_fs_open_img(readimginfo, tmpstr.split(",").at(4).toULongLong(), TSK_FS_TYPE_DETECT);
    readfileinfo = tsk_fs_file_open_meta(readfsinfo, NULL, curaddress);
    */

    if(objectid.contains("cheese"))
    {
    }

    for(int i = 0; i < digoptions.count(); i++)
    {
        if(digoptions.at(0) == 0) // Generate Thumbnails
        {
            StartThumbnails();
            //GenerateThumbnails();
        }
    }
    /*
    if(readfileinfo->meta != NULL)
    {
        char* imgbuf = reinterpret_cast<char*>(malloc(readfileinfo->meta->size));
        //qDebug() << "imgbuf size:" << sizeof(imgbuf);
        ssize_t imglen = tsk_fs_file_read(readfileinfo, 0, imgbuf, readfileinfo->meta->size, TSK_FS_FILE_READ_FLAG_NONE);
        QStringList filefiles = eviddir.entryList(QStringList(wombatvariable.evidenceobject.name.split(".evid").at(0) + "." + pstring + "." + fstring + ".a*"), QDir::NoSymLinks | QDir::Files);
        QFile filefile(wombatvariable.tmpmntpath + filefiles.at(0));
        filefile.open(QIODevice::ReadOnly);
        tmpstr = filefile.readLine();
        filefile.close();
        free(imgbuf);
    }
    */
    /*
    tsk_fs_file_close(readfileinfo);
    tsk_fs_close(readfsinfo);
    tsk_img_close(readimginfo);
    */
    digcount++;
    int curprogress = (int)((((float)digcount/(float)digfilelist.count()))*100);
    StatusUpdate(QString("Dug " + QString::number(digcount) + " of " + QString::number(digfilelist.count()) + " " + QString::number(curprogress) + "%"));
}

void WombatForensics::UpdateDig()
{
    int curprogress = (int)((((float)digcount)/(float)digfilelist.count())*100);
    LogMessage("Dug: " + QString::number(digcount) + " of " + QString::number(digfilelist.count()) + " " + QString::number(curprogress) + "%");
    StatusUpdate("Dug: " + QString::number(digcount) + " of " + QString::number(digfilelist.count()) + " " + QString::number(curprogress) + "%");
}
void WombatForensics::UpdateExport()
{
    int curprogress = (int)((((float)exportcount)/(float)exportlist.count())*100);
    LogMessage(QString("Exported " + QString::number(exportcount) + " of " + QString::number(exportlist.count()) + " " + QString::number(curprogress) + "%"));
    StatusUpdate(QString("Exported " + QString::number(exportcount) + " of " + QString::number(exportlist.count()) + " " + QString::number(curprogress) + "%"));
}
void WombatForensics::UpdateProgress(unsigned long long filecount)
{
    filecountlabel->setText("Found: " + QString::number(filecount));
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
    for(int i=0; i < treenodemodel->columnCount(QModelIndex()); i++)
        ui->dirTreeView->resizeColumnToContents(i);
}

void WombatForensics::SetupHexPage(void)
{
    // hex editor page
    //QBoxLayout* mainlayout = new QBoxLayout(QBoxLayout::TopToBottom, ui->hexPage);
    //QHBoxLayout* hexLayout = new QHBoxLayout();
    //hexwidget = new ImageHexViewer(ui->hexPage, tskobjptr);
    //QHexView* hexview = new QHexView;
    //hexviewwidget->clear();
    //hexview = new QHexEdit(this);
    setBackgroundRole(QPalette::Base);
    setAutoFillBackground(true);
    ui->hexview->setContextMenuPolicy(Qt::CustomContextMenu);
    //connect(ui->hexview, SIGNAL(dataChanged()), this, SLOT(DataChanged()));
    connect(ui->hexview, SIGNAL(currentAddressChanged(qint64)), this, SLOT(SetOffsetLabel(qint64)));
    connect(ui->hexview, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(ImgHexMenu(const QPoint &)));
    connect(ui->hexview, SIGNAL(selectionChanged()), this, SLOT(HexSelectionChanged()));

    //hexwidget->setObjectName("bt-hexview");
    //hexwidget->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
    //hexwidget->setContextMenuPolicy(Qt::CustomContextMenu);
    /*
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
    */
    //hexLayout->addWidget(hexwidget);
    //hexLayout->addWidget(hexview);
    //hexLayout->addWidget(hexview);
    //hexviewwidget = dynamic_cast<QHexView*>(centralWidget());
    /*
    hexrocker = new WombatSlider(ui->hexPage);
    hexrocker->setRange(-100, 100);
    hexrocker->setValue(0);
    hexrocker->setSingleStep(1);
    hexrocker->setEnabled(false);
    */
    //hexLayout->addWidget(hexrocker);
    //mainlayout->addLayout(hexLayout);
    //connect(hexview, SIGNAL(overwriteModeChanged(bool)), this, SLOT(setOverWriteMode(bool)));
    //connect(hexview, SIGNAL(dataChanged()), this, SLOT(dataChanged()));
    //connect(linedown, SIGNAL(clicked()), hexwidget, SLOT(nextLine()));
    //connect(lineup, SIGNAL(clicked()), hexwidget, SLOT(prevLine()));
    //connect(pagedown, SIGNAL(clicked()), hexwidget, SLOT(nextPage()));
    //connect(pageup, SIGNAL(clicked()), hexwidget, SLOT(prevPage()));
    //connect(hexrocker, SIGNAL(ShowJumpFilter()), jumpfilterview, SLOT(DisplayFilter()));
    //connect(jumpfilterview, SIGNAL(SetOffset()), hexwidget, SLOT(SetOffset()));
    //connect(hexwidget, SIGNAL(offsetChanged(off_t)), this, SLOT(SetOffsetLabel(off_t)));
    //connect(hexrocker, SIGNAL(sliderMoved(int)), hexwidget, SLOT(setTopLeftToPercent(int)));
    //connect(hexrocker, SIGNAL(sliderMoved(int)), this, SLOT(ShowRockerToolTip(int)));
    //connect(hexrocker, SIGNAL(sliderReleased()), this, SLOT(ResetSlider()));
    //connect(hexwidget, SIGNAL(selectionChanged(const QString &)), this, SLOT(UpdateSelectValue(const QString&)));
    //connect(hexwidget, SIGNAL(SkipDown()), this, SLOT(SkipDown()));
    //connect(hexwidget, SIGNAL(SkipUp()), this, SLOT(SkipUp()));
    //connect(hexwidget, SIGNAL(PageUp()), this, SLOT(PageUp()));
    //connect(hexwidget, SIGNAL(PageDown()), this, SLOT(PageDown()));
    //connect(hexwidget, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(ImgHexMenu(const QPoint &)));
}

void WombatForensics::ShowRockerToolTip(int moved)
{
    //QToolTip::showText(QCursor::pos(), QString::number(abs(moved)), hexrocker);
}

void WombatForensics::ResetSlider()
{
    /*
    if(linedown->isDown())
        linedown->setDown(false);
    if(lineup->isDown())
        lineup->setDown(false);
    if(pagedown->isDown())
        pagedown->setDown(false);
    if(pageup->isDown())
        pageup->setDown(false);
    hexrocker->setValue(0);
    */
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
    if(wombatvariable.iscaseopen)
        CloseCurrentCase();
    
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
}

void WombatForensics::RemoveTmpFiles()
{
    QDir tmpdir(wombatvariable.tmpfilepath);
    if(!tmpdir.removeRecursively())
    {
        DisplayError("2.7", "Tmp File Removal", "All tmp files may not have been removed. Please manually remove them to avoid evidence contamination.");
    }
}

void WombatForensics::on_actionNew_Case_triggered()
{
    // determine if a case is open
    if(wombatvariable.iscaseopen)
    {
        int ret = QMessageBox::question(this, tr("Close Current Case"), tr("There is a case already open. Are you sure you want to close it?"), QMessageBox::Yes | QMessageBox::No);
        if(ret == QMessageBox::Yes)
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
    if(!actionitem->IsChecked())
        actionitem->SetChecked(true);
    else
        actionitem->SetChecked(false);
    emit treenodemodel->CheckedNodesChanged();
}

void WombatForensics::on_actionExport_triggered()
{
    totalcount = 0;
    totalchecked = 0;
    exportcount = 0;
    exportdialog = new ExportDialog(this, totalchecked, totalcount);
    connect(exportdialog, SIGNAL(StartExport(int, bool, QString)), this, SLOT(ExportFiles(int, bool, QString)), Qt::DirectConnection);
    exportdialog->show();
}

void WombatForensics::on_actionDigDeeper_triggered()
{
    totalcount = filesfound;
    totalchecked = fileschecked;
    digcount = 0;
    digdeeperdialog = new DigDeeperDialog(this, totalchecked, totalcount);
    connect(digdeeperdialog, SIGNAL(StartDig(int, QVector<int>)), this, SLOT(DigFiles(int, QVector<int>)), Qt::DirectConnection);
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

void WombatForensics::on_actionView_Image_Gallery_triggered(bool checked)
{
    if(!checked) // hide viewer
    {
        imagewindow->hide();
    }
    else
    {
        thumbdir.mkpath(wombatvariable.tmpmntpath + "thumbs/");
        QDir tdir = QDir(QString(wombatvariable.tmpmntpath + "thumbs/"));
        if(tdir.isEmpty())
        {
            int ret = QMessageBox::question(this, tr("Generate Thumbnails"), tr("Thumbnails have not been generated. Do you want to generate all thumbnails now?\r\n\r\nNote: This can take a while and will show the Image Gallery window when complete."), QMessageBox::Yes | QMessageBox::No);
            if(ret == QMessageBox::Yes)
            {
                StartThumbnails();
            }
            else
                ui->actionView_Image_Gallery->setChecked(false);
        }
        else
        {
            imagewindow->UpdateGeometries();
            imagewindow->show();
        }
    }
}

void WombatForensics::StartThumbnails()
{
    LogMessage("Generating Thumbnails...");
    StatusUpdate("Generating Thumbnails...");
    QDir eviddir = QDir(wombatvariable.tmpmntpath);
    QFile tmpfile;
    QFile thumbfile;
    thumbfile.setFileName(wombatvariable.tmpmntpath + "thumbs/" + "thumbpathlist");
    QString tmpstr = "";
    thumblist.clear();
    thumbpathlist.clear();
    QStringList filefiles = eviddir.entryList(QStringList("*.p*.f*.a*"), QDir::NoSymLinks | QDir::Files);
    for(int i = 0; i < filefiles.count(); i++)
    {
        tmpstr = "";
        tmpfile.setFileName(wombatvariable.tmpmntpath + filefiles.at(i));
        tmpfile.open(QIODevice::ReadOnly);
        tmpstr = tmpfile.readLine();
        tmpfile.close();
        if(tmpstr.split(",", QString::SkipEmptyParts).at(10).split("/", QString::SkipEmptyParts).at(0).contains("image"))
        {
            thumblist.append(tmpstr.split(",", QString::SkipEmptyParts).at(12));
            QByteArray ba;
            QByteArray ba2;
            ba.append(tmpstr.split(",").at(0));
            ba2.append(tmpstr.split(",").at(3));
            QString fullpath = QString(QByteArray::fromBase64(ba2)) + QString(QByteArray::fromBase64(ba));
            ba.clear();
            ba.append(fullpath);
            thumbfile.open(QIODevice::Append);
            thumbfile.write(ba.toBase64());
            thumbfile.write(",");
            thumbfile.close();
        }
    }

    thumbfuture = QtConcurrent::map(thumblist, GenerateThumbnails);
    thumbwatcher.setFuture(thumbfuture);
    cancelthread->show();
    StatusUpdate("Thumbnail generation finished.");
    LogMessage("Thumbnail generation finished.");
}

void WombatForensics::FinishThumbs()
{
    imagewindow->UpdateGeometries();
    cancelthread->close();
    if(digoptions.isEmpty() || digoptions.contains(0))
        imagewindow->show();
    LogMessage("Digging Complete");
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
}

void WombatForensics::UpdateThumbnails(int tsize)
{
    thumbsize = tsize;
    imagewindow->UpdateGeometries();
}

void WombatForensics::HexSelectionChanged()
{
    QString tmptext = "Length: " + QString::number(ui->hexview->GetSelectionLength());
    selectedhex->setText(tmptext);
}

void WombatForensics::UpdateSelectValue(const QString &txt)
{
    /*
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
    */
}

//void WombatForensics::SetOffsetLabel(off_t pos)
void WombatForensics::SetOffsetLabel(qint64 pos)
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
    if(headercolumn == 4)
        createfilterview->DisplayFilter();
    if(headercolumn == 5)
        accessfilterview->DisplayFilter();
    if(headercolumn == 6)
        modifyfilterview->DisplayFilter();
    if(headercolumn == 7)
        changefilterview->DisplayFilter();
    if(headercolumn == 8)
        hashfilterview->DisplayFilter();
    if(headercolumn == 9)
        filecategoryfilterview->DisplayFilter();
    if(headercolumn == 10)
        filetypefilterview->DisplayFilter();
    ResizeColumns();
}

void WombatForensics::NextItem()
{
    QModelIndex curindex = ui->dirTreeView->currentIndex();
    QModelIndexList tmplist = treenodemodel->match(ui->dirTreeView->model()->index(0, 0), Qt::ForegroundRole, QVariant(), -1, Qt::MatchFlags(Qt::MatchExactly | Qt::MatchRecursive));
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
    QModelIndexList tmplist = treenodemodel->match(ui->dirTreeView->model()->index(0, 0), Qt::ForegroundRole, QVariant(), -1, Qt::MatchFlags(Qt::MatchExactly | Qt::MatchWrap | Qt::MatchRecursive));
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
    unsigned long long filtercount = 0;
    QModelIndexList tmplist = treenodemodel->match(treenodemodel->index(0, 0), Qt::ForegroundRole, QVariant(), -1, Qt::MatchFlags(Qt::MatchExactly | Qt::MatchWrap | Qt::MatchRecursive));
    for(int i=0; i < tmplist.count(); i++)
    {
        if(tmplist.at(i).sibling(tmplist.at(i).row(), 0).data().toString().split("-").count() == 4)
            filtercount++;
    }
    if(filtercount == filesfound)
        filtercount = 0;
    filtercountlabel->setText("Filtered: " + QString::number(filtercount));
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
    UpdateCheckState();
    UpdateSelectedState(selectedindex.sibling(selectedindex.row(), 0).data().toString());
}

void WombatForensics::UpdateCheckCount()
{
    fileschecked = 0;
    QHashIterator<QString, bool> i(checkhash);
    while(i.hasNext())
    {
        i.next();
        if(i.value())
            fileschecked++;
    }
    checkedcountlabel->setText("Checked: " + QString::number(fileschecked));
}

void WombatForensics::InitializeCheckState()
{
    QFile hashfile(wombatvariable.tmpmntpath + "checkstate");
    if(hashfile.exists())
    {
        hashfile.open(QIODevice::ReadOnly);
        QString tmpstr = hashfile.readLine();
        hashfile.close();
        QStringList hashlist = tmpstr.split(",", QString::SkipEmptyParts);
        for(int i=0; i < hashlist.count(); i++)
            checkhash.insert(hashlist.at(i).split("|", QString::SkipEmptyParts).at(0), hashlist.at(i).split("|", QString::SkipEmptyParts).at(1).toInt());
    }
}

void WombatForensics::UpdateCheckState()
{
    QFile hashfile(wombatvariable.tmpmntpath + "checkstate");
    hashfile.open(QIODevice::WriteOnly);
    QHashIterator<QString, bool> i(checkhash);
    QByteArray hasharray;
    while(i.hasNext())
    {
        i.next();
        if(checkhash.contains(i.key()))
        {
            hasharray.clear();
            hasharray.append(QString(i.key() + "|" + QString::number(i.value()) + ","));
            hashfile.write(hasharray);
        }
    }
    hashfile.close();
}

QString WombatForensics::InitializeSelectedState()
{
    QFile selectfile(wombatvariable.tmpmntpath + "selectedstate");
    if(selectfile.exists())
    {
        selectfile.open(QIODevice::ReadOnly);
        QString tmpstr = selectfile.readLine();
        selectfile.close();
        return tmpstr;
    }
    else return "";
}

void WombatForensics::UpdateSelectedState(QString id)
{
    QFile selectfile(wombatvariable.tmpmntpath + "selectedstate");
    selectfile.open(QIODevice::WriteOnly);
    QByteArray selectarray;
    selectarray.append(id);
    selectfile.write(selectarray);
    selectfile.close();
}

void WombatForensics::AutoSaveState()
{
    // change display text
    StatusUpdate("Saving State Started");
    SaveState();
    StatusUpdate("Evidence ready");
    // change display text
}
