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
    statuslabel = new StatusLabel();
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
    bookmarkmenu = new QMenu();
    bookmarkmenu->addAction(ui->actionNew_Bookmark);
    bookmarkmenu->addAction(ui->actionExisting_Bookmarks);
    QWidget* bookwidget = ui->analysisToolBar->widgetForAction(ui->actionBookmark_Manager);
    QToolButton* bookbutton = qobject_cast<QToolButton*>(bookwidget);
    if(bookbutton)
        connect(ui->actionBookmark_Manager, SIGNAL(triggered(bool)), bookbutton, SLOT(showMenu()));
    ui->actionBookmark_Manager->setMenu(bookmarkmenu);
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    ui->analysisToolBar->addWidget(spacer);
    ui->analysisToolBar->addAction(ui->actionAbout);
    tskexternalptr = &tskexternalobject;
    isignals = new InterfaceSignals();
    idfilterview = new IdFilter(this);
    jumpfilterview = new JumpHex(this);
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
    msgviewer = new MessageViewer();
    byteviewer = new ByteConverter();
    aboutbox = new AboutBox(this);
    digstatusdialog = new DigStatus(this);
    imagewindow->setWindowIcon(QIcon(":/thumb"));
    msgviewer->setWindowIcon(QIcon(":/bar/logview"));
    byteviewer->setWindowIcon(QIcon(":/bar/byteconverter"));
    aboutbox->setWindowIcon(QIcon(":/bar/about"));
    imagewindow->hide();
    filtervalues.maxcreate = QDateTime::currentDateTimeUtc().toTime_t();
    filtervalues.mincreate = QDateTime::currentDateTimeUtc().toTime_t();
    filtervalues.maxaccess = QDateTime::currentDateTimeUtc().toTime_t();
    filtervalues.minaccess = QDateTime::currentDateTimeUtc().toTime_t();
    filtervalues.maxmodify = QDateTime::currentDateTimeUtc().toTime_t();
    filtervalues.minmodify = QDateTime::currentDateTimeUtc().toTime_t();
    filtervalues.maxchange = QDateTime::currentDateTimeUtc().toTime_t();
    filtervalues.minchange = QDateTime::currentDateTimeUtc().toTime_t();
    qRegisterMetaType<QVector<int> >();
    oiiniterr = DAInitEx(DATHREAD_INIT_PTHREADS, OI_INIT_NOLOADOPTIONS|OI_INIT_NOSAVEOPTIONS); // Initialize Data Access at Application Start
    SetOptionDWORD((VTHDOC)NULL, SCCOPT_FIFLAGS, SCCUT_FI_NORMAL);
    SetOptionBOOL((VTHDOC)NULL, SCCOPT_EX_UNICODECALLBACKSTR, FALSE);
    SetOptionDWORD((VTHDOC)NULL, SCCOPT_FONT_REFERENCE_METHOD, SCCFONTS_REFERENCE_EXPORTED);

    // THIS ONE IS NOT NEEDED AS THE OUTPUT FILES DEFAULT TO THE MAIN HTML OUTPUT PATH.
    //SetOptionString((VTHDOC)NULL, SCCOPT_URLPATH_OUTPUT, QString("/home/pasquale/.wombatforensics/oiwv/").toStdString().c_str());
    SetOptionString((VTHDOC)NULL, SCCOPT_URLPATH_RESOURCES, QString("/home/pasquale/.wombatforensics/oiwv/assets/").toStdString().c_str());
    connect(imagewindow, SIGNAL(HideImageWindow(bool)), this, SLOT(HideImageWindow(bool)), Qt::DirectConnection);
    connect(msgviewer, SIGNAL(HideMessageViewerWindow(bool)), this, SLOT(HideMessageViewer(bool)), Qt::DirectConnection);
    connect(byteviewer, SIGNAL(HideByteConverterWindow(bool)), this, SLOT(HideByteViewer(bool)), Qt::DirectConnection);
    connect(isignals, SIGNAL(ProgressUpdate(qint64)), this, SLOT(UpdateProgress(qint64)), Qt::QueuedConnection);
    connect(statuslabel, SIGNAL(clicked()), this, SLOT(ShowDigStatus()), Qt::DirectConnection);
    connect(isignals, SIGNAL(DigUpdate(int, int)), this, SLOT(UpdateDig(int, int)), Qt::QueuedConnection);
    connect(isignals, SIGNAL(ExportUpdate(void)), this, SLOT(UpdateExport()), Qt::QueuedConnection);
    connect(digstatusdialog, SIGNAL(CancelImgThumbThread()), &thumbwatcher, SLOT(cancel()), Qt::QueuedConnection);
    connect(digstatusdialog, SIGNAL(CancelHashThread()), &hashingwatcher, SLOT(cancel()), Qt::QueuedConnection);
    CheckWombatConfiguration();
    InitializeAppStructure();
    connect(&sqlwatcher, SIGNAL(finished()), this, SLOT(UpdateStatus()), Qt::QueuedConnection);
    connect(&openwatcher, SIGNAL(finished()), this, SLOT(OpenUpdate()), Qt::QueuedConnection);
    connect(&thumbwatcher, SIGNAL(finished()), this, SLOT(FinishThumbs()), Qt::QueuedConnection);
    connect(&thashwatcher, SIGNAL(finished()), this, SLOT(ThashFinish()), Qt::QueuedConnection);
    connect(&thashsavewatcher, SIGNAL(finished()), this, SLOT(ThashSaveFinish()), Qt::QueuedConnection);
    connect(&hashingwatcher, SIGNAL(finished()), this, SLOT(HashingFinish()), Qt::QueuedConnection);
    connect(&exportwatcher, SIGNAL(finished()), this, SLOT(FinishExport()), Qt::QueuedConnection);
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
    treemenu->addAction(ui->actionView_File);
    treemenu->addAction(ui->actionView_Properties);
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
    connect(jumpfilterview, SIGNAL(SetOffset()), this, SLOT(SetHexOffset()));
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
    treenodemodel = new TreeNodeModel();
    //autosavetimer = new QTimer(this);
    //connect(autosavetimer, SIGNAL(timeout()), this, SLOT(AutoSaveState()));
}

void WombatForensics::ShowDigStatus()
{
    QPoint p = QCursor::pos();
    p += QPoint(40, -300);
    digstatusdialog->move(p);
    digstatusdialog->show();
}

//////////////////////////////////////////////////////////////
void WombatForensics::ShowExternalViewer()
{
    qint64 curobjaddr = selectedindex.sibling(selectedindex.row(), 10).data().toString().split("-f").at(1).toLongLong();
    QDir eviddir = QDir(wombatvariable.tmpmntpath);
    QStringList evidfiles = eviddir.entryList(QStringList("*." + selectedindex.sibling(selectedindex.row(), 10).data().toString().split("-").at(0)), QDir::NoSymLinks | QDir::Dirs);
    QString evidencename = evidfiles.at(0).split(".e").first();
    QString tmpstr = "";
    QStringList evidlist;
    evidlist.clear();
    std::vector<std::string> pathvector;
    pathvector.clear();
    QString estring = selectedindex.sibling(selectedindex.row(), 10).data().toString().split("-", QString::SkipEmptyParts).at(0);
    QString vstring = selectedindex.sibling(selectedindex.row(), 10).data().toString().split("-", QString::SkipEmptyParts).at(1);
    QString pstring = selectedindex.sibling(selectedindex.row(), 10).data().toString().split("-", QString::SkipEmptyParts).at(2);
    QString fstring = selectedindex.sibling(selectedindex.row(), 10).data().toString().split("-", QString::SkipEmptyParts).at(3);
    QFile evidfile(wombatvariable.tmpmntpath + evidencename + "." + estring + "/stat");
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
        //LogMessage("Image opening error");
    }
    free(tskexternalptr->imagepartspath);
    tmpstr = "";
    QStringList partlist;
    partlist.clear();
    QFile partfile(wombatvariable.tmpmntpath + evidencename + "." + estring + "/" + vstring + "/" + pstring + "/stat");
    partfile.open(QIODevice::ReadOnly);
    tmpstr = partfile.readLine();
    partfile.close();
    partlist = tmpstr.split(",");
    tskexternalptr->readfsinfo = tsk_fs_open_img(tskexternalptr->readimginfo, partlist.at(4).toLongLong(), TSK_FS_TYPE_DETECT);
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
        QString tmpstring = wombatvariable.tmpfilepath + selectedindex.sibling(selectedindex.row(), 10).data().toString() + "-tmp";
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
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
    if(index.sibling(index.row(), 8).data().toString().contains("image"))
    {
        imageviewer = new ImageWindow();
        imageviewer->setWindowIcon(QIcon(":/img"));
        imageviewer->setWindowTitle(selectedindex.sibling(selectedindex.row(), 10).data().toString() + " Image Viewer");
        imageviewer->setAttribute(Qt::WA_DeleteOnClose);
        imageviewer->GetImage(selectedindex.sibling(selectedindex.row(), 10).data().toString());
        imageviewer->show();
    }
    else if(index.sibling(index.row(), 8).data().toString().contains("video"))
    {
        videowindow = new VideoViewer();
        videowindow->setAttribute(Qt::WA_DeleteOnClose);
        videowindow->setWindowIcon(QIcon(":/vid"));
        videowindow->setWindowTitle(selectedindex.sibling(selectedindex.row(), 10).data().toString() + " Video Viewer");
        videowindow->ShowVideo(index);
    }
    else if(index.sibling(index.row(), 8).data().toString().contains("text"))
    {
        // toggle the button...
        textviewer = new TextViewer();
        textviewer->setWindowIcon(QIcon(":/textencode"));
        textviewer->setWindowTitle(selectedindex.sibling(selectedindex.row(), 10).data().toString() + " Text Viewer");
        textviewer->setAttribute(Qt::WA_DeleteOnClose);
        // THIS FAILS ON ADS
        textviewer->ShowText(index);
    }
    else if(index.sibling(index.row(), 8).data().toString().contains("text/html"))
    {
        htmlviewer = new HtmlViewer();
        htmlviewer->setAttribute(Qt::WA_DeleteOnClose);
        htmlviewer->setWindowIcon(QIcon(":/web"));
        htmlviewer->setWindowTitle(selectedindex.sibling(selectedindex.row(), 10).data().toString() + " HTML Viewer");
        htmlviewer->ShowHtml(index);
    }
    else
    {
        // TRY OUTSIDE IN VIEWER EXPORT HERE.. if it fails, popup right menu...
        // STARTING ON STEP 2
        qDebug() << "Starting Document Export Conversion.";
        qDebug() << "hexstring:" << hexstring;
        oiopndocerr = DAOpenDocument(&oidoc, PATH_TYPE, (VTLPVOID)(hexstring.toStdString().c_str()), 0);
        qDebug() << "open document error:" << oiopndocerr;
        // IT WORKS IF I DON'T SET THE FULL PATH, JUST HTE FILE NAME AND THEN IT PLACES ITSELF IN THE EXE WORKING DIR.
        // THIS IS A PROBLEM. I CAN SET THE ASSET DIRECTORY CORRECTLY, AS WELL AS THE SCRIPT LOCATIONS PROPERLY, JUST NEED TO BE
        // ABLE TO FIGURE OUT HOW TO SET THE FILE NAME WITH PATH CORRECTLY
        //std::string oiout = "/home/pasquale/.wombatforensics/oiwv/oiex.html";
        std::string oiout = hexstring.toStdString() + ".html";
        fprintf(stdout, "std::string export file path: %s\n", oiout.c_str());
        oiopnexperr = EXOpenExport(oidoc, FI_HTML5, PATH_TYPE, ((VTLPVOID)(oiout.c_str())), 0, 0, NULL, 0, &oiexport);
        //oierr = EXOpenExport(oidoc, FI_HTML5, PATH_TYPE, ((VTLPVOID)(oiout.c_str())), 0, 0, (EXCALLBACKPROC)ExportCallback, 0, &oiexport); // THIS ONE WORKS
        VTCHAR szError[256];
        //qDebug() << "export path:" << QDir::homePath() + "/oiex.html";
        DAGetErrorString(oiopnexperr, szError, sizeof(szError));
        fprintf(stderr, "open export error: %s\n", szError);
        //qDebug() << "open export error:" << DAGetErrorString(oierr, szError, sizeof(szError));
        oirunexperr = EXRunExport(oiexport);
        DAGetErrorString(oirunexperr, szError, sizeof(szError));
        fprintf(stderr, "run export error: %s\n", szError);
        //qDebug() << "run export error:" << oierr;
        oiclsexperr = EXCloseExport(oiexport);
        DAGetErrorString(oiclsexperr, szError, sizeof(szError));
        fprintf(stderr, "close export error: %s\n", szError);
        //qDebug() << "close export error;" << oierr;
        oiclsdocerr = DACloseDocument(oidoc);
        DAGetErrorString(oiclsdocerr, szError, sizeof(szError));
        fprintf(stderr, "close document error: %s\n", szError);
        qDebug() << "close document error:" << oiclsdocerr;
        qDebug() << "Finished Document Export Conversion.";
        if(oiopnexperr == 0 && oiopndocerr == 0 && oirunexperr == 0 && oiclsexperr == 0 && oiclsdocerr == 0)
        {
            htmlviewer = new HtmlViewer();
            htmlviewer->setAttribute(Qt::WA_DeleteOnClose);
            htmlviewer->setWindowIcon(QIcon(":/outsidein"));
            htmlviewer->setWindowTitle(selectedindex.sibling(selectedindex.row(), 10).data().toString() + " OutsideIn Viewer");
            htmlviewer->LoadHtml(QString::fromStdString(oiout));
        }
        else
        {
            if(index.sibling(index.row(), 10).data().toString().split("-").count() == 4) // file
                treemenu->exec(QCursor::pos());
        }
    }
    QApplication::restoreOverrideCursor();
}

void WombatForensics::HideImageWindow(bool checkstate)
{
    ui->actionView_Image_Gallery->setChecked(checkstate);
}

void WombatForensics::HideViewerManager()
{
    treemenu->clear();
    ui->menuView_With->clear();
    treemenu->addAction(ui->actionView_File);
    treemenu->addAction(ui->actionView_Properties);
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

void WombatForensics::HideSettingsWindow()
{
    ReadSettings();
}

void WombatForensics::ReadSettings()
{
    //qDebug() << "readsettings settingsfile:" << settingsfile.fileName();
    settingsfile.open(QIODevice::ReadOnly);
    //qDebug() << "is file at end:" << settingsfile.atEnd();
    QStringList tmplist = QString(settingsfile.readLine()).split(",", QString::SkipEmptyParts);
    settingsfile.close();
    // split settings and implement them...
    for(int i = 0; i < tmplist.count(); i++)
    {
        if(tmplist.at(i).split(":").at(0) == "thumb")
            thumbsize = tmplist.at(i).split(":").at(1).toInt();
        //else if(tmplist.at(i).split(":").at(0) == "save")
        // etc...
    }
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
            if(line.compare("/tmp/wombatforensics/currentwfc /tmp/wombatforensics/mntpt auto defaults,rw,users,noauto 0 0", Qt::CaseSensitive) == 0)
                fstabbool = 1;
        }
        fstabfile.close();
        // SHOULD PROBABLY SWITCH THIS TO A DIALOG ALERT...
        if(fstabbool == 1)
            qDebug() << "fstab is correct";
        else
            qDebug() << "fstab is wrong";
    }
    // check if mntpt link exists, if it does, remove it
    if(QFileInfo::exists("/tmp/wombatforensics/mntpt"))
        QFile::remove("/tmp/wombatforensics/mntpt");
    // check if case link exists, if it does, remove it
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
    settingsfile.setFileName(homepath + "settings");
    //qDebug() << "settingsfile:" << settingsfile.fileName();
    if(!FileExists(QString(homepath + "settings").toStdString()))
    {
        settingsfile.open(QIODevice::WriteOnly | QIODevice::Text);
        settingsfile.close();
    }
    else
        ReadSettings();
    viewmanage = new ViewerManager(this);
    settingsdialog = new SettingsDialog(this);
    viewmanage->setWindowIcon(QIcon(":/bar/viewermanager"));
    settingsdialog->setWindowIcon(QIcon(":/bar/settings"));
    connect(viewmanage, SIGNAL(HideManagerWindow()), this, SLOT(HideViewerManager()), Qt::DirectConnection);
    connect(settingsdialog, SIGNAL(HideSettingsWindow()), this, SLOT(HideSettingsWindow()), Qt::DirectConnection);
    ui->actionSaveState->setEnabled(false);
    ui->actionAdd_Evidence->setEnabled(false);
    ui->actionRemove_Evidence->setEnabled(false);
    ui->actionView_Progress->setEnabled(false);
    ui->actionExport_Evidence->setEnabled(false);
    ui->actionDigDeeper->setEnabled(false);
    ui->actionBookmark_Manager->setEnabled(false);
    ui->actionView_Image_Gallery->setEnabled(false);
    ui->actionCopy_Selection_To->setEnabled(false);
    ui->actionTextViewer->setEnabled(false);
    ui->actionByteConverter->setEnabled(false);
    ui->actionJumpToHex->setEnabled(false);
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
    /*
    QFileDialog newcasedialog(this, tr("Create New Case"), QDir::homePath(), tr("Wombat Forensics Case (*.wfc)"));
    newcasedialog.setLabelText(QFileDialog::Accept, "Create");
    newcasedialog.setOption(QFileDialog::DontUseNativeDialog, true);
    if(newcasedialog.exec())
        wombatvariable.casename = newcasedialog.selectedFiles().first();
    //wombatvariable.casename = QFileDialog::getSaveFileName(this, tr("Create New Case File"), QDir::homePath(), tr("WombatForensics Case (*.wfc)"));
    */
    //bool ok;
    //QString wombatvariable.casename = QInputDialog::getText(this, tr("Create New Case"), tr("Case Name"), QLineEdit::Normal, "My Case", &ok);
    QInputDialog* casedialog = new QInputDialog(this);
    casedialog->setCancelButtonText("Cancel");
    casedialog->setInputMode(QInputDialog::TextInput);
    casedialog->setLabelText("Enter Case Name");
    casedialog->setOkButtonText("Create Case");
    casedialog->setTextEchoMode(QLineEdit::Normal);
    casedialog->setWindowTitle("New Case");
    if(casedialog->exec())
        wombatvariable.casename = casedialog->textValue();
    qDebug() << "case name:" << wombatvariable.casename;
    if(!wombatvariable.casename.isEmpty())
    {
        QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
        this->setWindowTitle(QString("Wombat Forensics - ") + wombatvariable.casename);
        qDebug() << "tmpmntpath before:" << wombatvariable.tmpmntpath;
        wombatvariable.tmpmntpath = wombatvariable.tmpmntpath + wombatvariable.casename + "/";
        (new QDir())->mkpath(wombatvariable.tmpmntpath);
        qDebug() << "tmpmntpath after:" << wombatvariable.tmpmntpath;
        //QStringList tmplist = wombatvariable.casename.split("/");
        //tmplist.removeLast();
        //wombatvariable.casepath = tmplist.join("/");
        /*
        // SPARSE FILE METHOD
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
        casefile.resize(1000000000000);
        casefile.close();
        // make ext4 partition
        QString mkfsstr = "mkfs.ext4 -F -q -E root_owner=" + QString::number(getuid()) + ":" + QString::number(getgid()) + " ";
        mkfsstr += wombatvariable.casename;
        //qDebug() << mkfsstr;
        QProcess::execute(mkfsstr);
        QString lnkstr = "ln -s " + wombatvariable.casename + " /tmp/wombatforensics/currentwfc";
        QProcess::execute(lnkstr);
        QString lnkmnt = "ln -s " + wombatvariable.tmpmntpath + " /tmp/wombatforensics/mntpt";
        QProcess::execute(lnkmnt);
        QProcess::execute("mount /tmp/wombatforensics/mntpt");
        // END SPARSE FILE METHOD
        */
        // BEGIN TAR METHOD
        // make case directory..
        // THIS ALMOST WORKS, BUT HAS ISSUES WITH WHERE IT DROPS IT...

        //(new QDir())->mkpath(tmppath);

        //QString casedirectory = QDir::homePath() + "/" + wombatvariable.casename.split("/").last().split(".").first();
        //casedir.mkpath(QDir::homePath() + casedirectory);
        //casedir(QDir::homePath() + wombatvariable.casename.split("/").last());
        //qDebug() << "tar method:" << casedirectory;
        wombatvariable.iscaseopen = true;
        logfile.setFileName(wombatvariable.tmpmntpath + "msglog");
        logfile.open(QIODevice::ReadWrite | QIODevice::Append | QIODevice::Text);
        msglog->clear();
        qInfo() << "Log File Created";
        //LogMessage("Log File Created");
        //treefile.setFileName(wombatvariable.tmpmntpath + "treemodel");
        thumbdir.mkpath(wombatvariable.tmpmntpath + "thumbs/");
        InitializeCheckState();
        ui->actionAdd_Evidence->setEnabled(true);
        qInfo() << "Case Was Created";
        //LogMessage("Case was Created");
        QApplication::restoreOverrideCursor();
        StatusUpdate("Ready");
        //autosavetimer->start(10000); // 10 seconds in milliseconds for testing purposes
        //autosavetimer->start(600000); // 10 minutes in milliseconds for a general setting for real.
    }
}

void WombatForensics::InitializeOpenCase()
{
    hashsum = 0;
    QFileDialog opencasedialog(this, tr("Open Existing Case"), QDir::homePath(), tr("Wombat Forensics Case (*.wfc)"));
    opencasedialog.setLabelText(QFileDialog::Accept, "Open");
    opencasedialog.setOption(QFileDialog::DontUseNativeDialog, true);
    if(opencasedialog.exec())
        wombatvariable.casename = opencasedialog.selectedFiles().first();
    //wombatvariable.casename = QFileDialog::getOpenFileName(this, tr("Open Existing Case"), QDir::homePath(), tr("WombatForensics Case (*.wfc)"));
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
        OpenCaseMountFinished(0, QProcess::NormalExit);
    }
}

void WombatForensics::OpenCaseMountFinished(int exitcode, QProcess::ExitStatus exitstatus)
{
    if(exitcode) {}
    if(exitstatus) {}
    wombatvariable.iscaseopen = true;
    logfile.setFileName(wombatvariable.tmpmntpath + "msglog");
    logfile.open(QIODevice::ReadWrite | QIODevice::Append | QIODevice::Text);
    msglog->clear();
    InitializeCheckState();
    ui->actionAdd_Evidence->setEnabled(true);
    //autosavetimer->start(10000); // 10 seconds in milliseconds for testing purposes
    //autosavetimer->start(600000); // 10 minutes in milliseconds for a general setting for real.
    QDir eviddir = QDir(wombatvariable.tmpmntpath);
    QStringList evidfiles = eviddir.entryList(QStringList(QString("*.e*")), QDir::Dirs | QDir::NoSymLinks, QDir::Type);
    //qDebug() << evidfiles;
    evidencelist.clear();
    for(int i=0; i < evidfiles.count(); i++)
    {
        QFile evidfile(wombatvariable.tmpmntpath + evidfiles.at(i) + "/stat");
        evidfile.open(QIODevice::ReadOnly | QIODevice::Text);
        if(evidfile.isOpen())
            evidencelist.append(QString(evidfile.readLine()).split(",").at(3));
        evidfile.close();
    }
    if(evidencelist.count() > 0)
    {
        QFuture<void> tmpfuture = QtConcurrent::map(evidencelist, PopulateTreeModel);
        openwatcher.setFuture(tmpfuture);
    }
}

void WombatForensics::OpenUpdate()
{
    QString hashstr = "MD5 Hash";
    // update Hash header: 32 = md5, 40 = sha1, 64 = sha256
    if(hashsum == 2)
        hashstr = "SHA1 Hash";
    else if(hashsum == 4)
        hashstr = "SHA256 Hash";
    treenodemodel->UpdateHeaderNode(7, hashstr);
    thumbdir.mkpath(wombatvariable.tmpmntpath + "thumbs/"); // won't do anything if it already exists
    QDir tdir = QDir(QString(wombatvariable.tmpmntpath + "thumbs/"));
    if(!tdir.isEmpty())
    {
        QFuture<void> tmpfuture = QtConcurrent::run(LoadImagesHash); // load images hash after case open to speed up thumbnail viewing
        thashwatcher.setFuture(tmpfuture);
    }
    PrepareEvidenceImage();
    ui->dirTreeView->setModel(treenodemodel);
    connect(treenodemodel, SIGNAL(CheckedNodesChanged()), this, SLOT(UpdateCheckCount()));
    connect(ui->dirTreeView->selectionModel(), SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)), this, SLOT(SelectionChanged(const QItemSelection &, const QItemSelection &)));
    // this currently fails to load content data in the hexviewer. until it does, the char bytes are zero...
    QModelIndexList indexlist = treenodemodel->match(treenodemodel->index(0, 0, QModelIndex()), Qt::DisplayRole, QVariant(InitializeSelectedState()), 1, Qt::MatchFlags(Qt::MatchExactly | Qt::MatchRecursive));
    UpdateCheckCount();
    if(indexlist.count() > 0)
    {
        qDebug() << indexlist.at(0).sibling(indexlist.at(0).row(), 0).data().toString() << indexlist.count();
        //ui->dirTreeView->setCurrentIndex(treenodemodel->index(0, 0, QModelIndex()));
        //selectedindex = treenodemodel->index(0, 0, QModelIndex());
        //LoadHexContents();
        //ui->hexview->ensureVisible();
        ui->dirTreeView->setCurrentIndex(indexlist.at(0));
        //QThread::sleep(60);
        ui->dirTreeView->selectionModel()->select(indexlist.at(0), QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows | QItemSelectionModel::Select);
        //selectedindex = indexlist.first();
        //LoadHexContents();
        //bool test = false;
        //qDebug() << ui->hexview->addressOffset() << selectedoffset->text().split("0x").last().toLongLong(&test, 16);
        //test = false;
        //ui->hexview->setCursorPosition(selectedoffset->text().split("0x").last().toLongLong(&test, 16)*2 + 56600*2);
        //ui->hexview->setCursorPosition(ui->hexview->addressOffset()*2);
        //ui->hexview->ensureVisible();
    }
    else
        ui->dirTreeView->setCurrentIndex(treenodemodel->index(0, 0, QModelIndex()));
    //ui->dirTreeView->setCurrentIndex(treenodemodel->index(0, 0, QModelIndex()));
    if(ui->dirTreeView->model() != NULL)
    {
        ui->actionRemove_Evidence->setEnabled(true);
        ui->actionSaveState->setEnabled(true);
        ui->actionDigDeeper->setEnabled(true);
        ui->actionJumpToHex->setEnabled(true);
        //ui->actionExpandAll->setEnabled(true);
        //ui->actionCollapseAll->setEnabled(true);
        //ui->actionBookmark_Manager->setEnabled(true);
    }
    QApplication::restoreOverrideCursor();
    qInfo() << "Case was Opened Successfully";
    StatusUpdate("Ready");
    if(!tdir.isEmpty())
        StatusUpdate("Loading Thumbnail Library...");
    ui->actionView_Image_Gallery->setEnabled(false);
}

void WombatForensics::ThashSaveFinish(void)
{
    qDebug() << "SaveImageHashes Finished";
    StatusUpdate("Thumbnail Library Saved");
    StatusUpdate("Ready");
}

void WombatForensics::ThashFinish(void)
{
    qDebug() << "LoadImageHashes Finished";
    StatusUpdate("Thumbnail Library Ready");
    ui->actionView_Image_Gallery->setEnabled(true);
    StatusUpdate("Ready");
}

void WombatForensics::SelectionChanged(const QItemSelection &curitem, const QItemSelection &previtem)
{
    if(previtem.indexes().count() > 0)
        oldselectedindex = previtem.indexes().at(0);
    if(curitem.indexes().count() > 0)
    {
        selectedindex = curitem.indexes().at(0);
        if(selectedindex.sibling(selectedindex.row(), 2).data().toLongLong() > 0) // file size
            ui->actionView_File->setEnabled(true);
        else
            ui->actionView_File->setEnabled(false);
        //ui->actionView_Properties->setEnabled(true);
        //ui->actionView_File->setEnabled(true);
        ui->actionView_Image_Gallery->setEnabled(true);
        ui->actionTextViewer->setEnabled(true);
        ui->actionExport_Evidence->setEnabled(true);
        ui->actionByteConverter->setEnabled(true);
        ui->actionJumpToHex->setEnabled(true);
        StatusUpdate("Loading Hex Contents...");
        QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
        LoadHexContents();
        GenerateHexFile(selectedindex);
        QApplication::restoreOverrideCursor();
        StatusUpdate("Ready");
        /*
        if(propertywindow->isVisible())
            UpdateProperties();
        */
    }
}

void WombatForensics::TreeContextMenu(const QPoint &pt)
{
    QModelIndex index = ui->dirTreeView->indexAt(pt);
    if(index.isValid())
    {
        actionitem = static_cast<TreeNode*>(index.internalPointer());
        if(index.sibling(index.row(), 10).data().toString().split("-").count() == 4) // file
            treemenu->exec(ui->dirTreeView->mapToGlobal(pt));
    }
}

void WombatForensics::ImgHexMenu(const QPoint &pt)
{
    if(ui->hexview->selectionToReadableString().size() > 0)
        selectionmenu->exec(ui->hexview->mapToGlobal(pt));
}

void WombatForensics::UpdateDataTable()
{
    StatusUpdate("Processing Complete");
    qInfo() << "Evidence has been successfully added";
    //LogMessage("Evidence has been successfully added");
    StatusUpdate("Evidence ready");
}

void WombatForensics::PrepareEvidenceImage()
{
    QString tmpstr = "";
    QString mntstr = "";
    for(int i=0; i < evidencelist.count(); i++)
    {
        //qDebug() << "evidence list:" << evidencelist.at(i);
        QDir eviddir(wombatvariable.tmpmntpath);
        QStringList evidfiles = eviddir.entryList(QStringList(QString(evidencelist.at(i).split("/").last() + ".e*")), QDir::NoSymLinks | QDir::Dirs);
        //qDebug() << wombatvariable.tmpmntpath + evidfiles.at(0) + "/stat";
        QFile evidfile(wombatvariable.tmpmntpath + evidfiles.at(0) + "/stat");
        evidfile.open(QIODevice::ReadOnly | QIODevice::Text);
        if(evidfile.isOpen())
           tmpstr = evidfile.readLine();
        evidfile.close();
        if(!tmpstr.isEmpty())
        {
        int imgtype = tmpstr.split(",").at(0).toInt();
        QString imagefile = tmpstr.split(",").at(3);
        if(TSK_IMG_TYPE_ISAFF((TSK_IMG_TYPE_ENUM)imgtype)) // AFF
        {
            if(!QFileInfo::exists(wombatvariable.imgdatapath + tmpstr.split(",").at(3).split("/").last() + ".raw"))
                mntstr = "affuse " + tmpstr.split(",").at(3) + " " + wombatvariable.imgdatapath;
        }
        else if(TSK_IMG_TYPE_ISEWF((TSK_IMG_TYPE_ENUM)imgtype)) // EWF
        {
            if(!QFileInfo::exists(wombatvariable.imgdatapath + tmpstr.split(",").at(3).split("/").last() + "/ewf1"))
            {
                QString tmpstring = wombatvariable.imgdatapath + tmpstr.split(",").at(3).split("/").last() + "/";
                (new QDir())->mkpath(tmpstring);
                mntstr = "ewfmount " + tmpstr.split(",").at(3) + " " + tmpstring;
            }
        }
        else if(TSK_IMG_TYPE_ISRAW((TSK_IMG_TYPE_ENUM)imgtype)) // RAW
        {
            QString imgext = tmpstr.split(",").at(3).split("/").last().split(".").last();
            //qDebug() << "imgext:" << imgext;
            if(imgext.contains(".000"))
            {
                if(!QFileInfo::exists(wombatvariable.imgdatapath + tmpstr.split(",").at(3).split("/").last() + ".raw"))
                    mntstr = "affuse " + tmpstr.split(",").at(3) + " " + wombatvariable.imgdatapath;
            }
            else
                mntstr = "";
        }
        else
        {
            qDebug() << QString("Image type: " + QString(tsk_img_type_toname((TSK_IMG_TYPE_ENUM)imgtype)) + " is not supported.");
            // this error should go in add evidence dialog bit...
            //DisplayError("0.5", QString("Image type: " + QString(tsk_img_type_toname(imgtype)) + " is not supported."), "Unsupported Image");
        }
        if(!mntstr.isEmpty())
        {
            xmntprocess = new QProcess();
            connect(xmntprocess, SIGNAL(readyReadStandardOutput()), this, SLOT(ReadXMountOut()));
            connect(xmntprocess, SIGNAL(readyReadStandardError()), this, SLOT(ReadXMountErr()));
            xmntprocess->start(mntstr); // removes WARNING Messages but does not capture them..
        }
        }
    }
    //qDebug() << evidencelist;
    // 1. LOAD EVIDENCE STAT FILE TO GET IMGTYPE VARIABLE.
    // 2. CHECK IF SEE IF IT IS ALREADY OPEN
    // 3. DETERMINE IF IT MIGHT HAVE SEGMENTS FOR RAW BASED ON EXTENSION.
    // 4. HANDLE RAW ACCORDINGLY
    // NEED TO RUN THIS FOR EACH IMAGE...
    //qDebug() << "evidnecename:" << QString::fromStdString(wombatvariable.fullpathvector.at(0));
    /*
    QString xmntstr = "";
    if(TSK_IMG_TYPE_ISAFF(wombatvariable.imgtype))
        xmntstr += "affuse " + QString::fromStdString(wombatvariable.fullpathvector.at(0)) + " " + wombatvariable.imgdatapath;
    else if(TSK_IMG_TYPE_ISEWF(wombatvariable.imgtype))
    {
        QString tmpstr = wombatvariable.imgdatapath + wombatvariable.evidencename + "/";
        (new QDir())->mkpath(tmpstr);
        xmntstr += "ewfmount " + QString::fromStdString(wombatvariable.fullpathvector.at(0)) + " " + tmpstr; 
    }
    else if(TSK_IMG_TYPE_ISRAW(wombatvariable.imgtype))
    {
        if(wombatvariable.segmentcount > 1)
            xmntstr += "affuse " + QString::fromStdString(wombatvariable.fullpathvector.at(0)) + " " + wombatvariable.imgdatapath;
    }
    else
    {
        qDebug() << "image format:" << tsk_img_type_toname(wombatvariable.imgtype) << "not supported";
    }
    xmntprocess = new QProcess();
    connect(xmntprocess, SIGNAL(readyReadStandardOutput()), this, SLOT(ReadXMountOut()));
    connect(xmntprocess, SIGNAL(readyReadStandardError()), this, SLOT(ReadXMountErr()));
    xmntprocess->start(xmntstr); // removes WARNING Messages but does not capture them..
    */
}

void WombatForensics::ReadXMountOut()
{
    qWarning() << xmntprocess->readAllStandardOutput();
}

void WombatForensics::ReadXMountErr()
{
    qWarning() << xmntprocess->readAllStandardError();
}

void WombatForensics::UpdateStatus()
{
    StatusUpdate("Preparing Evidence Image...");
    qInfo() << "Preparing Evidence Image...";
    //LogMessage("Preparing Evidence Image...");
    StatusUpdate("Evidence Image Ready...");
    qInfo() << "Evidence Image Ready...";
    //LogMessage("Evidence Image Ready...");
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
    StatusUpdate("Building Initial Evidence Tree...");
    qInfo() << "Building Initial Evidence Tree...";
    //qInfo() << QTime::currentTime().toString(
    PrepareEvidenceImage();
    //LogMessage("Building Initial Evidence Tree...");
    ui->dirTreeView->setModel(treenodemodel);
    connect(treenodemodel, SIGNAL(CheckedNodesChanged()), this, SLOT(UpdateCheckCount()));
    connect(ui->dirTreeView->selectionModel(), SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)), this, SLOT(SelectionChanged(const QItemSelection &, const QItemSelection &)));
    UpdateCheckCount();
    QModelIndexList indexlist = treenodemodel->match(treenodemodel->index(0, 10, QModelIndex()), Qt::DisplayRole, QVariant(InitializeSelectedState()), -1, Qt::MatchFlags(Qt::MatchExactly | Qt::MatchRecursive));
    if(indexlist.count() > 0)
    {
        //ui->dirTreeView->setCurrentIndex(treenodemodel->index(0, 0, QModelIndex()));
        ui->dirTreeView->setCurrentIndex(indexlist.at(0));
        //selectedindex = indexlist.at(0);
    }
    else
        ui->dirTreeView->setCurrentIndex(treenodemodel->index(0, 0, QModelIndex()));
    ui->actionRemove_Evidence->setEnabled(true);
    ui->actionSaveState->setEnabled(true);
    ui->actionDigDeeper->setEnabled(true);
    //ui->actionBookmark_Manager->setEnabled(true);
    qInfo() << "Processing Complete";
    //LogMessage("Processing Complete.");
    StatusUpdate("Evidence ready");
    QApplication::restoreOverrideCursor();
}

void WombatForensics::UpdateDigging()
{
    qInfo() << "Digging Complete";
    //LogMessage("Digging Complete");
    StatusUpdate("Evidence ready");
    statuslabel->setToolTip("");
}

void WombatForensics::AddEvidence()
{
    // HERE IS WHERE I WOULD OPEN THE NEW DIALOG
    //wombatvariable.fullpathvector.clear();
    //wombatvariable.itemcount = 0;
    int isnew = 1;
    addevidencedialog = new AddEvidenceDialog(this);
    addevidencedialog->exec();
    QDir eviddir = QDir(wombatvariable.tmpmntpath);
    QStringList evidfiles = eviddir.entryList(QStringList(QString("*.e*")), QDir::NoSymLinks | QDir::Dirs);
    ecount = evidfiles.count();
    //qDebug() << "ecount before:" << ecount;
    for(int i=0; i < evidencelist.count(); i++)
    {
        QString evidencepath = wombatvariable.tmpmntpath + evidencelist.at(i).split("/").last() + ".e" + QString::number(ecount) + "/";
        (new QDir())->mkpath(evidencepath);
        ecount++;
    }
    //qDebug() << "ecount after:" << ecount;
    if(evidencelist.count() > 0)
    {
        QFuture<void> tmpfuture = QtConcurrent::map(evidencelist, InitializeEvidenceStructure);
        sqlwatcher.setFuture(tmpfuture);
    }
    /*
    QStringList tmplist = QFileDialog::getOpenFileNames(this, tr("Select Evidence Image(s)"), QDir::homePath());
    if(tmplist.count())
    {
        wombatvariable.evidencename = tmplist.at(0).split("/").last();
        if(isnew == 1)
        {
            for(int i=0; i < tmplist.count(); i++)
                wombatvariable.fullpathvector.push_back(tmplist.at(i).toStdString());
            wombatvariable.itemcount = tmplist.count();
            qInfo() << "Start Adding Evidence";
            //LogMessage("Start Adding Evidence");
            if(ui->dirTreeView->model() != NULL)
                delete treenodemodel;
                treenodemodel = new TreeNodeModel();
            QFuture<void> tmpfuture = QtConcurrent::run(InitializeEvidenceStructure);
            sqlwatcher.setFuture(tmpfuture);
        }
        else
            DisplayError("3.0", "Evidence already exists in the case", "Add Evidence Cancelled");
    }
    */
}

void WombatForensics::UpdateProperties()
{
    QFile propfile;
    QDir eviddir = QDir(wombatvariable.tmpmntpath);
    QStringList evidfiles = eviddir.entryList(QStringList("*." + selectedindex.sibling(selectedindex.row(), 10).data().toString().split("-").at(0)), QDir::NoSymLinks | QDir::Dirs);
    QString evidencename = evidfiles.at(0).split(".e").first();
    propertylist.clear();
    if(selectedindex.sibling(selectedindex.row(), 10).data().toString().split("-").count() == 1) // evidence image
    {
        propfile.setFileName(wombatvariable.tmpmntpath + evidencename + "." + selectedindex.sibling(selectedindex.row(), 10).data().toString().split("-").first() + "/prop");
    }
    if(selectedindex.sibling(selectedindex.row(), 10).data().toString().split("-").count() == 2) // volume
    {
        propfile.setFileName(wombatvariable.tmpmntpath + evidencename + "." + selectedindex.sibling(selectedindex.row(), 10).data().toString().split("-").at(0) + "/" + selectedindex.sibling(selectedindex.row(), 10).data().toString().split("-").at(1) + "/prop");
    }
    if(selectedindex.sibling(selectedindex.row(), 10).data().toString().split("-").count() == 3) // file system
    {
        propfile.setFileName(wombatvariable.tmpmntpath + evidencename + "." + selectedindex.sibling(selectedindex.row(), 10).data().toString().split("-").at(0) + "/" + selectedindex.sibling(selectedindex.row(), 10).data().toString().split("-").at(1) + "/" + selectedindex.sibling(selectedindex.row(), 10).data().toString().split("-").at(2) + "/prop");
    }
    if(selectedindex.sibling(selectedindex.row(), 10).data().toString().split("-").count() == 4) // file
    {
        QString tmpfvalue = "";
        QString parentstr = "5";
        if(selectedindex.parent().sibling(selectedindex.parent().row(), 10).data().toString().split("-").count() == 3) // root inum
            parentstr = "5";
        else
            parentstr = selectedindex.parent().sibling(selectedindex.parent().row(), 10).data().toString().split("-").at(3).mid(1);
        if(selectedindex.sibling(selectedindex.row(), 10).data().toString().split("-").last().contains(":"))
            tmpfvalue = selectedindex.sibling(selectedindex.row(), 10).data().toString().split("-").last().split(":").at(0) + QString("-") + selectedindex.sibling(selectedindex.row(), 10).data().toString().split("-").last().split(":").at(1);
        else
            tmpfvalue = selectedindex.sibling(selectedindex.row(), 10).data().toString().split("-").last();
        propfile.setFileName(wombatvariable.tmpmntpath + evidencename + "." + selectedindex.sibling(selectedindex.row(), 10).data().toString().split("-").at(0) + "/" + selectedindex.sibling(selectedindex.row(), 10).data().toString().split("-").at(1) + "/" + selectedindex.sibling(selectedindex.row(), 10).data().toString().split("-").at(2) + "/" + tmpfvalue + ".a" + parentstr + ".prop");
    }
    propfile.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream in(&propfile);
    while(!in.atEnd())
    {
        QString tmpstr = "";
        QString line = in.readLine();
        if(line.split("||").at(1).contains("^^"))
            tmpstr = QString(line.split("||").at(1)).replace(QString("^^"), QString(", "));
        else
            tmpstr = line.split("||").at(1);
        propertylist << line.split("||").at(0) << tmpstr << line.split("||").at(2);
    }
    propfile.close();
    propertywindow->UpdateTableView();
    propertywindow->show();
}

void WombatForensics::GenerateHexFile(const QModelIndex curindex)
{
    if(curindex.sibling(curindex.row(), 10).data().toString().split("-").count() == 4)
    {
        QDir eviddir = QDir(wombatvariable.tmpmntpath);
        QStringList evidfiles = eviddir.entryList(QStringList("*." + curindex.sibling(curindex.row(), 10).data().toString().split("-").at(0)), QDir::NoSymLinks | QDir::Dirs);
        QString evidencename = evidfiles.at(0).split(".e").first();
        QString tmpstr = "";
        //qDebug() << "fileviewer:" << curindex.sibling(curindex.row(), 10).data().toString();
        QString paridstr = curindex.parent().sibling(curindex.parent().row(), 10).data().toString().split("-f").last();
        TSK_IMG_INFO* fileheximginfo;
        TSK_FS_INFO* filehexfsinfo;
        TSK_FS_FILE* filehexfileinfo;
        std::vector<std::string> pathvector;
        const TSK_TCHAR** imagepartspath;
        pathvector.clear();
        QString estring = curindex.sibling(curindex.row(), 10).data().toString().split("-").at(0);
        QString vstring = curindex.sibling(curindex.row(), 10).data().toString().split("-").at(1);
        QString pstring = curindex.sibling(curindex.row(), 10).data().toString().split("-").at(2);
        QString fstring = curindex.sibling(curindex.row(), 10).data().toString().split("-").at(3);
        QString curid = curindex.sibling(curindex.row(), 10).data().toString();
        QFile evidfile(wombatvariable.tmpmntpath + evidencename + "." + estring + "/stat");
        //qDebug() << "Initial string info (e-v-p-f):" << estring + vstring + pstring + fstring;
        evidfile.open(QIODevice::ReadOnly);
        if(evidfile.isOpen())
            tmpstr = evidfile.readLine();
        evidfile.close();
        int partcount = tmpstr.split(",").at(3).split("|").size();
        for(int i=0; i < partcount; i++)
            pathvector.push_back(tmpstr.split(",").at(3).split("|").at(i).toStdString());
        imagepartspath = (const char**)malloc(pathvector.size()*sizeof(char*));
        for(uint i=0; i < pathvector.size(); i++)
            imagepartspath[i] = pathvector[i].c_str();
        fileheximginfo = tsk_img_open(partcount, imagepartspath, TSK_IMG_TYPE_DETECT, 0);
        if(fileheximginfo == NULL)
        {
            qDebug() << tsk_error_get_errstr();
            //LogMessage("Image opening error");
        }
        free(imagepartspath);

        tmpstr = "";
        QStringList partlist;
        partlist.clear();
        QFile partfile(wombatvariable.tmpmntpath + evidencename + "." + estring + "/" + vstring + "/" + pstring + "/stat");
        partfile.open(QIODevice::ReadOnly);
        tmpstr = partfile.readLine();
        partfile.close();
        partlist = tmpstr.split(",");
        // FILE SYSTEM PIECE OF FILE HEX CONTENT CODE
        filehexfsinfo = tsk_fs_open_img(fileheximginfo, partlist.at(4).toLongLong(), TSK_FS_TYPE_DETECT);
        QStringList filelist;
        filelist.clear();
        QString tmpfilename = "";
        if(curid.split("-").at(3).mid(1).contains(":"))
            tmpfilename = curid.split("-").at(3).mid(1).split(":").at(0) + QString("-") + curid.split("-").at(3).mid(1).split(":").at(1);
        else
            tmpfilename = curid.split("-").at(3).mid(1);
        //qDebug() << "f value:" << tmpfilename;
        // INITIAL FILE SYSTEM FILE PIECE OF FILE HEX CONTENT CODE
        if(tmpfilename.contains("-"))
            filehexfileinfo = tsk_fs_file_open_meta(filehexfsinfo, NULL, tmpfilename.split("-").at(0).toInt());
        else
            filehexfileinfo = tsk_fs_file_open_meta(filehexfsinfo, NULL, tmpfilename.toInt());
        char* fhexbuf;
        ssize_t fhexlen = 0;
    
        QFile filefile;
        if(fstring.split(":").count() > 1)
            filefile.setFileName(wombatvariable.tmpmntpath + evidencename + "." + estring + "/" + vstring + "/" + pstring + "/" + fstring.split(":").first() + "-" + fstring.split(":").last() + ".a" + paridstr + ".stat");
        else
            filefile.setFileName(wombatvariable.tmpmntpath + evidencename + "." + estring + "/" + vstring + "/" + pstring + "/" + fstring.split(":").first() + ".a" + paridstr + ".stat");
        filefile.open(QIODevice::ReadOnly);
        if(filefile.isOpen())
            tmpstr = filefile.readLine();
        filefile.close();
        filelist = tmpstr.split(",");
    
        //if(!ui->hexview->isEnabled())
            ui->hexview->setEnabled(true);

        if(curindex.sibling(curindex.row(), 2).data().toLongLong() == 0)
        {
            //qDebug() << "zero file";
            ui->hexview->setEnabled(false);
        }
        else
        {
            if(partlist.at(0).toInt() == TSK_FS_TYPE_NTFS_DETECT) // IF NTFS (ADS/FILE/DIR/RES/NONRES)
            {
                if(curid.split("-").at(3).split(":").count() > 1) // IF ADS
                {
                    if(filehexfileinfo->meta != NULL)
                    {
                        fhexbuf = new char[filelist.at(8).toLongLong()];
                        fhexlen = tsk_fs_file_read_type(filehexfileinfo, TSK_FS_ATTR_TYPE_NTFS_DATA, curid.split("-").at(3).split(":").at(1).toInt(), 0, fhexbuf, filelist.at(8).toLongLong(), TSK_FS_FILE_READ_FLAG_NONE);
                        if(fhexlen == -1)
                            qDebug() << tsk_error_get_errstr();
                    }
                }
                else // IF NOT ADS
                {
                    if(filehexfileinfo->meta != NULL)
                    {
                        fhexbuf = new char[filehexfileinfo->meta->size];
                        fhexlen = tsk_fs_file_read(filehexfileinfo, 0, fhexbuf, filehexfileinfo->meta->size, TSK_FS_FILE_READ_FLAG_NONE);
                    }
                }
            }
            else // OTHER FILE SYSTEM
            {
                if(filehexfileinfo->meta != NULL)
                {
                    fhexbuf = new char[filehexfileinfo->meta->size];
                    fhexlen = tsk_fs_file_read(filehexfileinfo, 0, fhexbuf, filehexfileinfo->meta->size, TSK_FS_FILE_READ_FLAG_NONE);
                }
            }
        }
        if(curindex.sibling(curindex.row(), 2).data().toLongLong() > 0)
        {
            (new QDir())->mkpath(wombatvariable.tmpfilepath);
            hexstring = wombatvariable.tmpfilepath + curindex.sibling(curindex.row(), 10).data().toString() + "-fhex";
            QFile tmpfile(hexstring);
            tmpfile.open(QIODevice::WriteOnly);
            QDataStream outbuffer(&tmpfile);
            outbuffer.writeRawData(fhexbuf, fhexlen);
            tmpfile.close();
            delete[] fhexbuf;
        }
        tsk_fs_file_close(filehexfileinfo);
        tsk_fs_close(filehexfsinfo);
        tsk_img_close(fileheximginfo);
    }
}

void WombatForensics::LoadHexContents()
{
    // NEED TO GET EVIDENCE NAME FROM STAT FILE
    // TRY TreeNode* itemnode = static_cast<TreeNode*>(index.internalPointer());
    //wombatvariable.selectedid = selectedindex.sibling(selectedindex.row(), 10).data().toString(); // mod object id
    selectednode = static_cast<TreeNode*>(selectedindex.internalPointer());
    //qDebug() << "evidencelist:" << evidencelist; // original evidence path
    //qDebug() << "selectednode id:" << selectednode->Data(10).toString();
    //qDebug() << "selectednode evid id:" << selectednode->Data(10).toString().split("-").first();
    QString nodeid = selectednode->Data(10).toString();
    QString evidid = nodeid.split("-").first();
    QDir eviddir = QDir(wombatvariable.tmpmntpath);
    QStringList evidfiles = eviddir.entryList(QStringList(QString("*." + evidid)), QDir::NoSymLinks | QDir::Dirs);
    //qDebug() << "evidfile name:" << evidfiles.first();
    QString tmpstr = "";
    QFile evidfile(wombatvariable.tmpmntpath + evidfiles.first() + "/stat");
    evidfile.open(QIODevice::ReadOnly | QIODevice::Text);
    if(evidfile.isOpen())
        tmpstr = evidfile.readLine();
    evidfile.close();
    QString datastring = wombatvariable.imgdatapath;
    if(TSK_IMG_TYPE_ISAFF((TSK_IMG_TYPE_ENUM)tmpstr.split(",").at(0).toInt()))
        datastring += tmpstr.split(",").at(3).split("/").last() + ".raw";
    else if(TSK_IMG_TYPE_ISEWF((TSK_IMG_TYPE_ENUM)tmpstr.split(",").at(0).toInt()))
        datastring += tmpstr.split(",").at(3).split("/").last() + "/ewf1";
    else if(TSK_IMG_TYPE_ISRAW((TSK_IMG_TYPE_ENUM)tmpstr.split(",").at(0).toInt()))
    {
        QString imgext = tmpstr.split(",").at(3).split("/").last().split(".").last();
        if(imgext.contains(".000"))
            datastring += tmpstr.split(",").at(3).split("/").last() + ".raw";
        else
            datastring = tmpstr.split(",").at(3);
    }
    else
        qDebug() << QString("Image type: " + QString(tsk_img_type_toname((TSK_IMG_TYPE_ENUM)tmpstr.split(",").at(0).toInt())) + " is not supported.");
    casedatafile.setFileName(datastring);
    ui->hexview->setData(casedatafile);

    // determine offset location in the editor
    if(nodeid.split("-").count() == 1) // image file
        ui->hexview->setCursorPosition(0);
    else if(nodeid.split("-").count() == 2) // volume file
    {
        QFile volfile(wombatvariable.tmpmntpath + evidfiles.first() + "/" + nodeid.split("-").at(1) + "/stat");
        volfile.open(QIODevice::ReadOnly | QIODevice::Text);
        if(volfile.isOpen())
            tmpstr = volfile.readLine();
        volfile.close();
        ui->hexview->setCursorPosition(tmpstr.split(",").at(4).toInt()*2);
    }
    else if(nodeid.split("-").count() == 3) // partition/file system
    {
        QFile partfile(wombatvariable.tmpmntpath + evidfiles.first() + "/" + nodeid.split("-").at(1) + "/" + nodeid.split("-").at(2) + "/stat");
        partfile.open(QIODevice::ReadOnly | QIODevice::Text);
        if(partfile.isOpen())
            tmpstr = partfile.readLine();
        partfile.close();
        ui->hexview->setCursorPosition(tmpstr.split(",").at(4).toLongLong()*2);
    }
    else if(nodeid.split("-").count() == 5) // dir/file
    {
        QString paridstr = selectedindex.parent().sibling(selectedindex.parent().row(), 10).data().toString().split("-f").last();
        QStringList partlist;
        partlist.clear();
        QFile partfile(wombatvariable.tmpmntpath + evidfiles.first() + "/" + nodeid.split("-").at(1) + "/" + nodeid.split("-").at(2) + "/stat");
        partfile.open(QIODevice::ReadOnly | QIODevice::Text);
        if(partfile.isOpen())
            partlist = QString(partfile.readLine()).split(",");
        partfile.close();
        qint64 fsoffset = partlist.at(4).toLongLong();
        qint64 rootinum = partlist.at(3).toLongLong();
        if(paridstr.contains("-"))
            paridstr = QString::number(rootinum);
        QString mftentryoffset = "";
        QFile partpropfile(wombatvariable.tmpmntpath + evidfiles.first() + "/" + nodeid.split("-").at(1) + "/" + nodeid.split("-").at(2) + "/prop");
        partpropfile.open(QIODevice::ReadOnly | QIODevice::Text);
        while(!partpropfile.atEnd())
        {
            QString tmpstring = partpropfile.readLine();
            if(tmpstring.contains("MFT Starting Byte Address"))
                mftentryoffset = tmpstring.split("||").at(1);
        }
        partpropfile.close();
        QFile filefile;
        if(nodeid.split("-").at(3).split(":").count() > 1)
            filefile.setFileName(wombatvariable.tmpmntpath + evidfiles.first() + "/" + nodeid.split("-").at(1) + "/" + nodeid.split("-").at(2) + "/" + nodeid.split("-").at(3).split(":").first() + "-" + nodeid.split("-").at(3).split(":").last() + ".a" + paridstr + ".stat");
        else
            filefile.setFileName(wombatvariable.tmpmntpath + evidfiles.first() + "/" + nodeid.split("-").at(1) + "/" + nodeid.split("-").at(2) + "/" + nodeid.split("-").at(3) + ".a" + paridstr + ".stat");
        filefile.open(QIODevice::ReadOnly | QIODevice::Text);
        if(filefile.isOpen())
            tmpstr = filefile.readLine();
        filefile.close();
        int mftaddress = 0;
        if(nodeid.split("-").at(3).split(":").count() > 1) // ads attribute
            mftaddress = tmpstr.split(",").at(2).toInt();
        else
            mftaddress = tmpstr.split(",").at(9).toInt();
        ui->hexview->setEnabled(true);
        if(tmpstr.split(",").at(8).toInt() == 0) // zero file
            ui->hexview->setEnabled(false);
        else
        {
            QString blockstring = "";
            QString residentstring = "";
            QString bytestring = "";
            QFile filefileprop;
            if(nodeid.split("-").at(3).split(":").count() > 1)
                filefileprop.setFileName(wombatvariable.tmpmntpath + evidfiles.first() + "/" + nodeid.split("-").at(1) + "/" + nodeid.split("-").at(2) + "/" + nodeid.split("-").at(3).split(":").first() + "-" + nodeid.split("-").at(3).split(":").last() + ".a" + paridstr + ".prop");
            else
                filefileprop.setFileName(wombatvariable.tmpmntpath + evidfiles.first() + "/" + nodeid.split("-").at(1) + "/" + nodeid.split("-").at(2) + "/" + nodeid.split("-").at(3) + ".a" + paridstr + ".prop");
            filefileprop.open(QIODevice::ReadOnly | QIODevice::Text);
            while(!filefileprop.atEnd())
            {
                QString tmpstring = filefileprop.readLine();
                if(tmpstring.contains("Block Address"))
                    blockstring = tmpstring.split("||").at(1);
                else if(tmpstring.contains("Resident Offset"))
                    residentstring = tmpstring.split("||").at(1);
                else if(tmpstring.contains("Byte Offset"))
                    bytestring = tmpstring.split("||").at(1);
            }
            filefileprop.close();
            if(partlist.at(0).toInt() == TSK_FS_TYPE_NTFS_DETECT) // IF NTFS (ADS/FILE/DIR/RES/NONRES)
            {
                unsigned int curoffset = 0;
                uint8_t mftoffset[2];
                uint8_t nextattrid[2];
                uint8_t mftlen[4];
                uint8_t attrtype[4];
                uint32_t atrtype = 0;
                uint8_t namelength = 0;
                uint32_t contentlength = 0;
                uint16_t resoffset = 0;
                if(nodeid.split("-").at(3).split(":").count() > 1) // IF ADS
                {
                    if(blockstring.compare("") != 0 && blockstring.compare("0^^") != 0) // IF NON-RESIDENT
                    {
                        ui->hexview->SetColorInformation(partlist.at(4).toLongLong(), partlist.at(6).toLongLong(), blockstring, residentstring, bytestring, tmpstr.split(",").at(8).toLongLong(), 0);
                        ui->hexview->setCursorPosition(bytestring.toLongLong()*2);
                    }
                    else // IF RESIDENT
                    {
                        if(tmpstr.split(",").at(8).toLongLong() < 700) // takes care of $BadClus which is non-resident but doesn't have blocks
                        {
                        qint64 residentoffset = mftentryoffset.toLongLong() + (1024 * mftaddress) + fsoffset;
                        //qDebug() << "(resident ads) residentoffset:" << residentoffset;
                        QByteArray resbuffer = ui->hexview->dataAt(residentoffset, 1024); // MFT Entry
                        curoffset = 0;
                        //qDebug() << "resbuffer MFT SIG:" << QString(resbuffer.at(0)) << QString(resbuffer.at(1)) << QString(resbuffer.at(2)) << QString(resbuffer.at(3));
                        mftoffset[0] = (uint8_t)resbuffer.at(20);
                        mftoffset[1] = (uint8_t)resbuffer.at(21);
                        nextattrid[0] = (uint8_t)resbuffer.at(40);
                        nextattrid[1] = (uint8_t)resbuffer.at(41);
                        curoffset += tsk_getu16(TSK_LIT_ENDIAN, mftoffset);
                        int attrcnt = tsk_getu16(TSK_LIT_ENDIAN, nextattrid);
                        for(int i = 0; i < attrcnt; i++)
                        {
                            attrtype[0] = (uint8_t)resbuffer.at(curoffset);
                            attrtype[1] = (uint8_t)resbuffer.at(curoffset + 1);
                            attrtype[2] = (uint8_t)resbuffer.at(curoffset + 2);
                            attrtype[3] = (uint8_t)resbuffer.at(curoffset + 3);
                            atrtype = tsk_getu32(TSK_LIT_ENDIAN, attrtype);
                            namelength = (uint8_t)resbuffer.at(curoffset + 9);
                            mftlen[0] = (uint8_t)resbuffer.at(curoffset + 4);
                            mftlen[1] = (uint8_t)resbuffer.at(curoffset + 5);
                            mftlen[2] = (uint8_t)resbuffer.at(curoffset + 6);
                            mftlen[3] = (uint8_t)resbuffer.at(curoffset + 7);
                            contentlength = tsk_getu32(TSK_LIT_ENDIAN, mftlen);
                            if(namelength > 0 && atrtype == 128)
                                break;
                            curoffset += contentlength;
                        }
                        mftoffset[0] = (uint8_t)resbuffer.at(curoffset + 20);
                        mftoffset[1] = (uint8_t)resbuffer.at(curoffset + 21);
                        resoffset = tsk_getu16(TSK_LIT_ENDIAN, mftoffset);
    
                        ui->hexview->SetColorInformation(partlist.at(4).toLongLong(), partlist.at(6).toLongLong(), blockstring, QString::number(residentoffset + curoffset + resoffset - fsoffset), bytestring, tmpstr.split(",").at(8).toLongLong(), (curoffset + resoffset));
                        ui->hexview->setCursorPosition((residentoffset + curoffset + resoffset)*2);
                        }
                        else
                            ui->hexview->setCursorPosition(0);
                    }
                }
                else // IF NOT ADS
                {
                    if(tmpstr.split(",").at(1).toInt() == 3) // IF DIR
                    {
                        qint64 residentoffset = mftentryoffset.toLongLong() + (1024 * mftaddress) + fsoffset;
                        //qDebug() << "(resident dir) residentoffset:" << residentoffset;
                        QByteArray resbuffer = ui->hexview->dataAt(residentoffset, 1024); // MFT Entry
                        //qDebug() << "resbuffer length:" << resbuffer.length();
                        curoffset = 0;
                        resoffset = 0;
                        mftoffset[0] = (uint8_t)resbuffer.at(20);
                        mftoffset[1] = (uint8_t)resbuffer.at(21);
                        nextattrid[0] = (uint8_t)resbuffer.at(40);
                        nextattrid[1] = (uint8_t)resbuffer.at(41);
                        curoffset += tsk_getu16(TSK_LIT_ENDIAN, mftoffset);
                        int attrcnt = tsk_getu16(TSK_LIT_ENDIAN, nextattrid);
                        //qDebug() << "attrcnt:" << attrcnt;
                        for(int i = 0; i < attrcnt; i++)
                        {
                            if(curoffset < (unsigned)resbuffer.size())
                            {
                                attrtype[0] = (uint8_t)resbuffer.at(curoffset);
                                attrtype[1] = (uint8_t)resbuffer.at(curoffset + 1);
                                attrtype[2] = (uint8_t)resbuffer.at(curoffset + 2);
                                attrtype[3] = (uint8_t)resbuffer.at(curoffset + 3);
                                atrtype = tsk_getu32(TSK_LIT_ENDIAN, attrtype);
                                mftlen[0] = (uint8_t)resbuffer.at(curoffset + 4);
                                mftlen[1] = (uint8_t)resbuffer.at(curoffset + 5);
                                mftlen[2] = (uint8_t)resbuffer.at(curoffset + 6);
                                mftlen[3] = (uint8_t)resbuffer.at(curoffset + 7);
                                contentlength = tsk_getu32(TSK_LIT_ENDIAN, mftlen);
                                if(atrtype == 144)
                                    break;
                                curoffset += contentlength;
                            }
                        }
                        //qDebug() << "curoffset:" << curoffset;
                        // offset to type 144 resident attribute content
                        mftoffset[0] = (uint8_t)resbuffer.at(curoffset + 20);
                        mftoffset[1] = (uint8_t)resbuffer.at(curoffset + 21);
                        curoffset += tsk_getu16(TSK_LIT_ENDIAN, mftoffset);
                        ui->hexview->SetColorInformation(partlist.at(4).toLongLong(), partlist.at(6).toLongLong(), "", QString::number(residentoffset + curoffset - fsoffset), bytestring, tmpstr.split(",").at(8).toLongLong(), curoffset);
                        ui->hexview->setCursorPosition((residentoffset + curoffset)*2);
                    }
                    else // IF FILE AND OTHER STUFF
                    {
                        if(blockstring.compare("") != 0 && blockstring.compare("0^^") != 0) // IF NON-RESIDENT
                        {
                            ui->hexview->SetColorInformation(partlist.at(4).toLongLong(), partlist.at(6).toLongLong(), blockstring, residentstring, bytestring, tmpstr.split(",").at(8).toLongLong(), 0);
                            ui->hexview->setCursorPosition(bytestring.toLongLong()*2);
                        }
                        else // IF RESIDENT
                        {
                            qint64 residentoffset = mftentryoffset.toLongLong() + (1024 * mftaddress) + fsoffset;
                            //qDebug() << "(resident file) residentoffset:" << residentoffset;
                            QByteArray resbuffer = ui->hexview->dataAt(residentoffset, 1024); // MFT Entry
                            curoffset = 0;
                            mftoffset[0] = (uint8_t)resbuffer.at(20);
                            mftoffset[1] = (uint8_t)resbuffer.at(21);
                            nextattrid[0] = (uint8_t)resbuffer.at(40);
                            nextattrid[1] = (uint8_t)resbuffer.at(41);
                            curoffset += tsk_getu16(TSK_LIT_ENDIAN, mftoffset);
                            int attrcnt = tsk_getu16(TSK_LIT_ENDIAN, nextattrid);
                            //qDebug() << "attrcnt:" << attrcnt;
                            for(int i = 0; i < attrcnt; i++)
                            {
                                if(curoffset < (unsigned)resbuffer.size())
                                {
                                    attrtype[0] = (uint8_t)resbuffer.at(curoffset);
                                    attrtype[1] = (uint8_t)resbuffer.at(curoffset + 1);
                                    attrtype[2] = (uint8_t)resbuffer.at(curoffset + 2);
                                    attrtype[3] = (uint8_t)resbuffer.at(curoffset + 3);
                                    atrtype = tsk_getu32(TSK_LIT_ENDIAN, attrtype);
                                    namelength = (uint8_t)resbuffer.at(curoffset + 9);
                                    mftlen[0] = (uint8_t)resbuffer.at(curoffset + 4);
                                    mftlen[1] = (uint8_t)resbuffer.at(curoffset + 5);
                                    mftlen[2] = (uint8_t)resbuffer.at(curoffset + 6);
                                    mftlen[3] = (uint8_t)resbuffer.at(curoffset + 7);
                                    contentlength = tsk_getu32(TSK_LIT_ENDIAN, mftlen);
                                    if(namelength == 0 && atrtype == 128)
                                        break;
                                    curoffset += contentlength;
                                }
                            }
                            //qDebug() << "curoffset:" << curoffset;
                            if(curoffset < (unsigned)resbuffer.size())
                            {
                                mftoffset[0] = (uint8_t)resbuffer.at(curoffset + 20);
                                mftoffset[1] = (uint8_t)resbuffer.at(curoffset + 21);
                                resoffset = tsk_getu16(TSK_LIT_ENDIAN, mftoffset);
                            }
                            ui->hexview->SetColorInformation(partlist.at(4).toLongLong(), partlist.at(6).toLongLong(), blockstring, QString::number(residentoffset + curoffset + resoffset - fsoffset), bytestring, tmpstr.split(",").at(8).toLongLong(), (curoffset + resoffset));
                            ui->hexview->setCursorPosition((residentoffset + curoffset + resoffset)*2);
                        }
                    }
                }
            }
            else // OTHER FILE SYSTEM
            {
                qDebug() << "not ntfs";
                ui->hexview->SetColorInformation(partlist.at(4).toLongLong(), partlist.at(6).toLongLong(), blockstring, residentstring, bytestring, tmpstr.split(",").at(8).toLongLong(), 0);
                ui->hexview->setCursorPosition(bytestring.toLongLong()*2);
            }
        }
    }
    else
        qDebug() << "count for nodeid:" << nodeid.split("-").count();
    ui->hexview->ensureVisible();
}

void WombatForensics::CloseCurrentCase()
{
    if(ui->dirTreeView->model() != NULL)
    {
        UpdateSelectedState(selectedindex.sibling(selectedindex.row(), 10).data().toString());
        delete treenodemodel;
        //evidcnt = 0;
        //autosavetimer->stop();
        UpdateCheckState();
    }
    if(ui->hexview->data().size() > 0)
    {
        casedatafile.resize(0);
        ui->hexview->setData(casedatafile);
    }
    setWindowTitle("WombatForensics");
    filesfound = 0;
    fileschecked = 0;
    filtercountlabel->setText("Filtered: 0");
    filecountlabel->setText("Found: " + QString::number(filesfound));
    checkedcountlabel->setText("Checked: " + QString::number(fileschecked));
    // WRITE MSGLOG TO FILE HERE...

    // UNMOUNT XMOUNT EVIDENCEIMAGEDATAFILE
    // NEED TO LOOP THIS FOR EACH EVIDENCE ITEM.. USE DIR FIND TO GET EVIDENCE NAMES
    for(int i=0; i < evidencelist.count(); i++)
    {
        //qDebug() << "evidencelist:" << evidencelist.at(i);
        QString imgext = evidencelist.at(i).split("/").last().split(".").last().toLower();
        if(imgext.contains("e01")) // ewfmount
        {
            QString xunmntstr = "fusermount -u " + wombatvariable.imgdatapath + evidencelist.at(i).split("/").last() + "/";
            QProcess::execute(xunmntstr);
        }
        else if(imgext.contains("aff") || imgext.contains("000")) // affuse
        {
            QString xunmntstr = "fusermount -u " + wombatvariable.imgdatapath;
            QProcess::execute(xunmntstr);
        }
        /*
        else // raw, so nothing to unmount
        {
        }
        */
    }
/*
    // BEGIN SPARSE FILE METHOD
    QString unmntstr = "umount " + wombatvariable.tmpmntpath;
    QProcess::execute(unmntstr);

    // delete two link files
    if(QFileInfo::exists("/tmp/wombatforensics/mntpt"))
        QFile::remove("/tmp/wombatforensics/mntpt");
    if(QFileInfo::exists("/tmp/wombatforensics/currentwfc"))
        QFile::remove("/tmp/wombatforensics/currentwfc");
    // END SPARSE FILE METHOD
*/
    // BEGIN TAR METHOD
    // THIS WORKS...
    TAR* casehandle;
    QString tmptar = QDir::homePath() + "/" + wombatvariable.casename + ".wfc";
    QByteArray tmparray = tmptar.toLocal8Bit();
    QByteArray tmparray2 = wombatvariable.tmpmntpath.toLocal8Bit();
    //QByteArray tmparray = wombatvariable.casename.toLocal8Bit();
    //char* casename = "tar.wfc";
    tar_open(&casehandle, tmparray.data(), NULL, O_WRONLY | O_CREAT, 0644, TAR_GNU);
    tar_append_tree(casehandle, tmparray2.data(), tmparray2.data());
    tar_close(casehandle);
    // remove existing case directory
    QDir cdir = QDir(wombatvariable.tmpmntpath);
    cdir.removeRecursively();
    // END TAR METHOD
    StatusUpdate("Current Case was closed successfully");
    RemoveTmpFiles();
    wombatvariable.iscaseopen = false;
}

void WombatForensics::RemEvidence()
{
    qDebug() << "remove evidence clicked";
    remevidencedialog = new RemEvidenceDialog(this);
    connect(remevidencedialog, SIGNAL(RemEvid(QStringList)), this, SLOT(RemoveEvidence(QStringList)));
    remevidencedialog->exec();
}

void WombatForensics::RemoveEvidence(QStringList remevidlist)
{
    qDebug() << "remevidlist:" << remevidlist;
    QDir eviddir = QDir(wombatvariable.tmpmntpath);
    for(int i=0; i < remevidlist.count(); i++)
    {
        QStringList evidfiles = eviddir.entryList(QStringList(remevidlist.at(i).split("/").last() + "*"), QDir::NoSymLinks | QDir::Dirs);
        if(!evidfiles.isEmpty())
        {
            //qDebug() << "evidfiles id:" << evidfiles.first().split(".e").last();
            //qDebug() << "evidfiles name:" << evidfiles.first().split(".e").first();

            // 1. Delete all thumbnails.
            QDir tdir = QDir(wombatvariable.tmpmntpath + "thumbs/");
            QStringList tfiles = tdir.entryList(QStringList("e" + evidfiles.first().split(".e").last() + "-*"), QDir::NoSymLinks | QDir::Files);
            if(!tfiles.isEmpty())
            {
                qDebug() << "tfiles:" << tfiles;
                for(int j = 0; j < tfiles.count(); j++)
                    tdir.remove(tfiles.at(j));
            }
            // 2. Remove e# entries from checkstate and selectedstate
            QHashIterator<QString, bool> m(checkhash);
            while(m.hasNext())
            {
                m.next();
                if(m.key().contains(QString("e" + evidfiles.first().split(".e").last())))
                    checkhash.remove(m.key());
            }
            UpdateCheckState();
            QFile selectfile(wombatvariable.tmpmntpath + "selectedstate");
            selectfile.open(QIODevice::WriteOnly | QIODevice::ReadOnly | QIODevice::Text);
            QString tmpstr = selectfile.readLine();
            if(tmpstr.contains("e" + evidfiles.first().split(".e").last()))
                selectfile.write("");
            selectfile.close();
            // 3. Delete evid directory.
            QDir edir = QDir(wombatvariable.tmpmntpath + evidfiles.first());
            edir.removeRecursively();
            // 4. Delete from evidencelist.
            evidencelist.removeOne(remevidlist.at(i));
            // 5. Remove TreeNode.
            QModelIndexList indexlist = treenodemodel->match(treenodemodel->index(0, 10, QModelIndex()), Qt::DisplayRole, QVariant("e" + evidfiles.first().split(".e").last()), 1, Qt::MatchFlags(Qt::MatchExactly | Qt::MatchRecursive));
            if(!indexlist.isEmpty())
            {
                qDebug() << "index found:" << indexlist.first().sibling(indexlist.first().row(), 10).data().toString();
                treenodemodel->removeRow(indexlist.first().row(), indexlist.first());
            }
        }
    }
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
    //ui->actionCancel_Operation->setEnabled(false);
    //cancelthread->close();
    qInfo() << "Export Completed with" << QString::number(errorcount) << "error(s)";
    //LogMessage(QString("Export Completed with " + QString::number(errorcount) + " error(s)"));
    StatusUpdate("Exporting completed with " + QString::number(errorcount) + " error(s)");
}

void WombatForensics::ExportFiles(int etype, bool opath, QString epath)
{
    exporttype = etype;
    exportpath = epath;
    originalpath = opath;
    exportlist.clear();
    errorcount = 0;
    exportcount = 0;
    qInfo() << "Started Exporting Evidence";
    //LogMessage("Started Exporting Evidence");
    if(etype == 0) // selected
    {
        TreeNode* itemnode = static_cast<TreeNode*>(selectedindex.internalPointer());
        exportlist.append(itemnode->Data(10).toString());
        //exportlist.append(selectedindex.sibling(selectedindex.row(), 10).data().toString());
    }
    else
        exportlist = GetFileLists(etype);
    int curprogress = (int)((((float)exportcount)/(float)exportlist.count())*100);
    qInfo() << "Exported" << QString::number(exportcount) << "of" << QString::number(exportlist.count()) << QString::number(curprogress) << "%";
    //LogMessage("Exported: " + QString::number(exportcount) + " of " + QString::number(exportlist.count()) + " " + QString::number(curprogress) + "%");
    StatusUpdate("Exported: " + QString::number(exportcount) + " of " + QString::number(exportlist.count()) + " " + QString::number(curprogress) + "%");
    // cancellable map
    QFuture<void> tmpfuture = QtConcurrent::map(exportlist, ProcessExport);
    exportwatcher.setFuture(tmpfuture);
    //ui->actionCancel_Operation->setEnabled(true);
    //QToolTip::showText(cancelwidget->mapToGlobal(QPoint(0, 0)), "Cancel Currently Running Opreation");
    //cancelthread->show();
}

void WombatForensics::DigFiles(int dtype, QVector<int> doptions)
{
    digtype = dtype;
    digoptions = doptions;
    digfilelist.clear();
    qInfo() << "Digging Deeper into Evidence";
    StatusUpdate("Digging Deeper...");
    statuslabel->setToolTip("Click for Details");
    //LogMessage("Digging Deeper into Evidence");
    for(int i = 0; i < digoptions.count(); i++)
    {
        if(dtype == 0) // selected
        {
            TreeNode* itemnode = static_cast<TreeNode*>(selectedindex.internalPointer());
            digfilelist.append(itemnode->Data(10).toString());
            //digfilelist.append(selectedindex.sibling(selectedindex.row(), 10).data().toString());
        }
        else
            digfilelist = GetFileLists(dtype);
        //qDebug() << digfilelist;
        digstatusdialog->SetInitialDigState(digoptions.at(i), digfilelist.count());
        if(digoptions.at(i) == 0) // Generate Thumbnails
        {
            thumbfuture = QtConcurrent::map(digfilelist, GenerateThumbnails); // Process Thumbnails
            thumbwatcher.setFuture(thumbfuture);
            //QFuture<void> tmpfuture = QtConcurrent::run(StartThumbnails, digfilelist);
            //thumbfuture = QtConcurrent::map(digfilelist, StartThumbnails); // Process Thumbnails
            //thumbwatcher.setFuture(thumbfuture);
            //QFuture<void> tmpfuture = QtConcurrent::run(&WombatForensics::StartThumbnails, digfilelist); // Process All Thumbnails
        }
        else if(digoptions.at(i) == 1 || digoptions.at(i) == 2 || digoptions.at(i) == 3) // 1 - MD5 || 2- SHA1 || 3- SHA256
        {
            if(digoptions.at(i) == 2)
                hashsum = 2;
            else if(digoptions.at(i) == 3)
                hashsum = 4;
            else if(digoptions.at(i) == 1)
                hashsum = 1;
            //qInfo() << "Generating Hash...";
            //StatusUpdate("Generating Hash...");
            hashingfuture = QtConcurrent::map(digfilelist, GenerateHash);
            hashingwatcher.setFuture(hashingfuture);
            //StartHash(digfilelist, 0);
        }
        /*
        else if(digoptions.at(i) == 2) // Generate SHA1
        {
            qInfo() << "Generating SHA1 Hash...";
            StatusUpdate("Generating SHA1 Hash...");
            hashingfuture = QtConcurrent::map(digfilelist, GenerateSHA1);
            hashingwatcher.setFuture(hashingfuture);
            //StartHash(digfilelist, 1);
        }
        */
    }
}

void WombatForensics::HashingFinish()
{
    StatusUpdate("Ready");
    if(hashsum == 1)
        treenodemodel->UpdateHeaderNode(7, "MD5 Hash");
    else if(hashsum == 2)
        treenodemodel->UpdateHeaderNode(7, "SHA1 Hash");
    else if(hashsum == 4)
        treenodemodel->UpdateHeaderNode(7, "SHA256 Hash");
    //qDebug() << hashsum;
    qDebug() << "Hashing should be Finished";
    // here is where i should update the column header...., which is possibly display and then update hash type...
}

void WombatForensics::UpdateDig(int digid, int digcnt)
{
    digstatusdialog->UpdateDigState(digid, digcnt);
    /*
    int curprogress = (int)((((float)digcount)/(float)digfilelist.count())*100);
    //qInfo() << "Dug:" << QString::number(digcount) << "of" << QString::number(digfilelist.count()) << QString::number(curprogress) << "%";
    //LogMessage("Dug: " + QString::number(digcount) + " of " + QString::number(digfilelist.count()) + " " + QString::number(curprogress) + "%");
    StatusUpdate("Dug: " + QString::number(digcount) + " of " + QString::number(digfilelist.count()) + " " + QString::number(curprogress) + "%");
    */
}
void WombatForensics::UpdateExport()
{
    int curprogress = (int)((((float)exportcount)/(float)exportlist.count())*100);
    qInfo() << "Exported" << QString::number(exportcount) << "of" << QString::number(exportlist.count()) << QString::number(curprogress) << "%";
    //LogMessage(QString("Exported " + QString::number(exportcount) + " of " + QString::number(exportlist.count()) + " " + QString::number(curprogress) + "%"));
    StatusUpdate(QString("Exported " + QString::number(exportcount) + " of " + QString::number(exportlist.count()) + " " + QString::number(curprogress) + "%"));
}
void WombatForensics::UpdateProgress(qint64 filecount)
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
    setBackgroundRole(QPalette::Base);
    setAutoFillBackground(true);
    ui->hexview->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->hexview, SIGNAL(currentAddressChanged(qint64)), this, SLOT(SetOffsetLabel(qint64)));
    connect(ui->hexview, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(ImgHexMenu(const QPoint &)));
    connect(ui->hexview, SIGNAL(selectionChanged()), this, SLOT(HexSelectionChanged()));
    connect(ui->hexview, SIGNAL(selectionChanged()), this, SLOT(UpdateSelectValue()));
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
    logfile.close();
    if(wombatvariable.iscaseopen)
        CloseCurrentCase();
    
    //propertywindow->close();
    //fileviewer->close();
    imagewindow->close();
    /*
    if(videowindow->isVisible())
        videowindow->close();
    */
    viewmanage->close();
    //textviewer->close();
    //htmlviewer->close();
    byteviewer->close();
    aboutbox->close();
    //cancelthread->close();
    settingsdialog->close();
    if(oiiniterr == DAERR_OK)
        DADeInit();
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

void WombatForensics::on_actionCancel_Operation_triggered()
{
    /*
    emit CancelCurrentThread();
    ui->actionCancel_Operation->setEnabled(false);
    */
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
    //digcount = 0;
    dighashcount = 0;
    digimgthumbcount = 0;
    digdeeperdialog = new DigDeeperDialog(this, totalchecked, totalcount);
    connect(digdeeperdialog, SIGNAL(StartDig(int, QVector<int>)), this, SLOT(DigFiles(int, QVector<int>)), Qt::DirectConnection);
    digdeeperdialog->show();
}

void WombatForensics::on_actionJumpToHex_triggered()
{
    jumpfilterview->show();
}

void WombatForensics::on_actionView_Properties_triggered(bool checked)
{
    if(checked){}
    propertywindow = new PropertiesWindow(this);
    propertywindow->setWindowIcon(QIcon(":/info"));
    propertywindow->setWindowTitle(selectedindex.sibling(selectedindex.row(), 10).data().toString() + " Properties");
    propertywindow->setAttribute(Qt::WA_DeleteOnClose);
    propertywindow->setModal(false);
    UpdateProperties();
}

void WombatForensics::on_actionView_File_triggered(bool checked)
{
    if(checked){}
    fileviewer = new FileViewer();
    fileviewer->setWindowIcon(QIcon(":/ehex"));
    fileviewer->setWindowTitle(selectedindex.sibling(selectedindex.row(), 10).data().toString() + " Hex");
    fileviewer->setAttribute(Qt::WA_DeleteOnClose);
    fileviewer->UpdateHexView();
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
                thumbfuture = QtConcurrent::map(listeditems, GenerateThumbnails); // Process All thumbnails
                //QFuture<void> tmpfuture = QtConcurrent::run(StartThumbnails, listeditems); // Process all thumbnails
                //QFuture<void> tmpfuture = QtConcurrent::run(&WombatForensics::StartThumbnails, listeditems); // Process All Thumbnails
            }
            else
                ui->actionView_Image_Gallery->setChecked(false);
        }
        else
        {
            imagewindow->LoadThumbnails();
            imagewindow->show();
        }
    }
}

void WombatForensics::FinishThumbs()
{
    //ui->actionCancel_Operation->setEnabled(false);
    //cancelthread->close();
    StatusUpdate("Thumbnail generation finished.");
    qInfo() << "Thumbnail generation finished";
    QFuture<void> tmpfuture = QtConcurrent::run(SaveImagesHash);
    thashsavewatcher.setFuture(tmpfuture);
    //LogMessage("Thumbnail generation finished.");
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
    imagewindow->LoadThumbnails(); // GUI Intensive
    QApplication::restoreOverrideCursor();
    if(digoptions.isEmpty() || digoptions.contains(0))
        imagewindow->show();
    qInfo() << "Digging Complete";
    qInfo() << "Evidence Ready";
    //LogMessage("Digging Complete");
    //LogMessage("Evidence ready");
    StatusUpdate("Evidence ready");
}

void WombatForensics::on_actionViewerManager_triggered()
{
    viewmanage->show();
}

void WombatForensics::on_actionTextViewer_triggered(bool checked)
{
    if(checked){}
    if(selectedindex.sibling(selectedindex.row(), 8).data().toString().contains("text/"))
    {
        textviewer->ShowText(selectedindex);
        textviewer->show();
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

void WombatForensics::on_actionSettings_triggered()
{
    settingsdialog->LoadSettings();
    settingsdialog->show();
}

void WombatForensics::HexSelectionChanged()
{
    QString tmptext = "Length: " + QString::number(ui->hexview->GetSelectionLength());
    selectedhex->setText(tmptext);
}

void WombatForensics::UpdateSelectValue()
{
    QByteArray selectionbytes = ui->hexview->selectionToByteArray();
    QDataStream ds8(selectionbytes);
    ds8.setByteOrder(QDataStream::LittleEndian);
    int8_t asint8;
    ds8 >> asint8;
    QDataStream ds16(selectionbytes);
    ds16.setByteOrder(QDataStream::LittleEndian);
    int16_t asint16;
    ds16 >> asint16;
    QDataStream ds32(selectionbytes);
    ds32.setByteOrder(QDataStream::LittleEndian);
    int32_t asint32;
    ds32 >> asint32;
    if(selectionbytes.isEmpty())
        ui->actionCopy_Selection_To->setEnabled(false);
    else
        ui->actionCopy_Selection_To->setEnabled(true);
    QString tmptext = "Length: " + QString::number(selectionbytes.size());
    QString bytetext = "";
    selectedhex->setText(tmptext);
    bytetext += "<table border=0 width='100%' cellpadding=5>";
    bytetext += "<tr><td>8-bit Signed Integer:</td><td align=right>" + QString::number(asint8) + "</td></tr>";
    bytetext += "<tr><td>16-bit Signed Integer:</td><td align=right>" + QString::number(asint16) + "</td></tr>";
    bytetext += "<tr><td>32-bit Signed Integer:</td><td align=right>" + QString::number(asint32) + "</td></tr>";
    bytetext += "</table>";
    byteviewer->SetText(bytetext);

    /*
     *
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

void WombatForensics::SetOffsetLabel(qint64 pos)
{
    QString label;
    label = "Offset: ";
    char buffer[64];
    #if _LARGEFILE_SOURCE
    sprintf(buffer,"0x%llx",pos);
    #else
    sprintf(buffer,"0x%x",pos);
    #endif
    label += buffer;
    selectedoffset->setText(label);
}

void WombatForensics::SetFilter(int headercolumn)
{
    if(headercolumn == 10)
        idfilterview->DisplayFilter();
    if(headercolumn == 0)
        namefilterview->DisplayFilter();
    if(headercolumn == 1)
        pathfilterview->DisplayFilter();
    if(headercolumn == 2)
        sizefilterview->DisplayFilter();
    if(headercolumn == 3)
        createfilterview->DisplayFilter();
    if(headercolumn == 4)
        accessfilterview->DisplayFilter();
    if(headercolumn == 5)
        modifyfilterview->DisplayFilter();
    if(headercolumn == 6)
        changefilterview->DisplayFilter();
    if(headercolumn == 7)
        hashfilterview->DisplayFilter();
    if(headercolumn == 8)
        filecategoryfilterview->DisplayFilter();
    if(headercolumn == 9)
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
    qint64 filtercount = 0;
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
    QString carvefilename = QFileDialog::getSaveFileName(this, tr("Carve to a File"), QDir::homePath(), "", NULL, QFileDialog::DontUseNativeDialog); 
    if(carvefilename.compare("") != 0)
    {
        QByteArray tmparray = ui->hexview->selectionToByteArray();
        QFile tmpfile(carvefilename);
        tmpfile.open(QIODevice::WriteOnly);
        tmpfile.write(tmparray);
        tmpfile.close();
    }
}

void WombatForensics::SaveState()
{
    RemoveTmpFiles();
    UpdateCheckState();
    UpdateSelectedState(selectedindex.sibling(selectedindex.row(), 10).data().toString());
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
        qDebug() << tmpstr;
        //return "file-r-3.dat";
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

void WombatForensics::SetHexOffset()
{
    ui->hexview->setCursorPosition(jumpoffset*2);
    ui->hexview->ensureVisible();
}

void WombatForensics::ThreadCancelled()
{
    qInfo() << "Current Operation Cancelled";
}

SCCERR ExportCallback(VTHEXPORT hExport, VTSYSPARAM dwCallbackData, VTDWORD dwCommandID, VTLPVOID pCommandData)
{
    SCCERR seResult = SCCERR_NOTHANDLED;
    EXFILEIOCALLBACKDATA *pNewFileInfo;
    //EXURLFILEIOCALLBACKDATA *pExportData;
    UNUSED(hExport);
    UNUSED(dwCallbackData);

    /* This is a simple callback handler just to show how they are
     * coded.  The callback routine should always return
     * SCCERR_NOTHANDLED unless the user is returning data to the
     * conversion.
     */

    switch (dwCommandID)
    {
      case EX_CALLBACK_ID_NEWFILEINFO:

        /* Count files that are created by the conversion. */
        //gdwFileCount++;

        /* Print the name of each output file generated. */
        pNewFileInfo = (EXFILEIOCALLBACKDATA *)pCommandData;
        //pExportData  = (EXURLFILEIOCALLBACKDATA *)pNewFileInfo->pExportData;

        if( pNewFileInfo->dwSpecType == PATH_TYPE )
          printf("Creating file: %s\n", (char*)pNewFileInfo->pSpec );
        else
          printf("Creating not file: %s\n", (char*)pNewFileInfo->pSpec );

				//if (pNewFileInfo->dwAssociation == CU_ROOT && g_monitorPort)
					//SendReadyMessage();
        break;

      default:
        break;
    }
    
    return seResult;
}

void SetOptionDWORD(VTHDOC target, VTDWORD optionId, VTDWORD val)
{
    DASetOption(target, optionId, (VTLPVOID)&val, sizeof(VTDWORD));
}

void SetOptionBOOL(VTHDOC target, VTDWORD optionId, VTBOOL val)
{
    DASetOption(target, optionId, (VTLPVOID)&val, sizeof(VTBOOL));
}

void SetOptionString(VTHDOC target, VTDWORD optionId, const char* val)
{
    DASetOption(target, optionId, (VTLPVOID)val, STRLEN(val)+1);
}
