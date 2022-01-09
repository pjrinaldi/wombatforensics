#include "wombatforensics.h"
//#include "fusefunctions.h"
//#include "affuse.h"
//#include "ewffuse.h"
//#include "sqfuse.h"
//#include "zmgfuse.h"
//#include "ewfimage.h"
//#include "forimg.h"

// Copyright 2013-2020 Pasquale J. Rinaldi, Jr.
// Distrubted under the terms of the GNU General Public License version 2

WombatForensics::WombatForensics(QWidget *parent) : QMainWindow(parent), ui(new Ui::WombatForensics)
{
    ui->setupUi(this);
    this->menuBar()->hide();
    pathtreeview = new PathTreeView();
    ui->splitter->insertWidget(0, pathtreeview);
    this->statusBar()->setSizeGripEnabled(true);
    selectedoffset = new QLabel(this);
    selectedoffset->setText("Offset: 00");
    selectedhex = new QLabel(this);
    selectedhex->setText("Length: 0");
    filtercountlabel = new QLabel(this);
    filtercountlabel->setText("Filtered: 0");
    filecountlabel = new QLabel(this);
    filecountlabel->setText("Parsed: 0");
    checkedcountlabel = new QLabel(this);
    checkedcountlabel->setText("Checked: 0");
    digcountlabel = new QLabel(this);
    carvestatuslabel = new QLabel(this);
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
    vline3 = new QFrame(this);
    vline3->setFrameStyle(QFrame::VLine | QFrame::Raised);
    vline3->setLineWidth(1);
    vline3->setMidLineWidth(0);
    this->statusBar()->addWidget(selectedoffset, 0);
    this->statusBar()->addWidget(selectedhex, 0);
    this->statusBar()->addWidget(vline1, 0);
    this->statusBar()->addWidget(filecountlabel, 0);
    this->statusBar()->addWidget(checkedcountlabel, 0);
    this->statusBar()->addWidget(filtercountlabel, 0);
    this->statusBar()->addWidget(vline2, 0);
    this->statusBar()->addWidget(digcountlabel, 0);
    this->statusBar()->addWidget(carvestatuslabel, 0);
    this->statusBar()->addPermanentWidget(vline3, 0);
    this->statusBar()->addPermanentWidget(statuslabel, 0);
    QWidget* spacer = new QWidget();
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
    tagfilterview = new TagFilter(this);
    hashmatchfilterview = new HashMatchFilter(this);
    imagewindow = new ImageViewer(this);
    msgviewer = new MessageViewer(this);
    byteviewer = new ByteConverter(this);
    previewreport = new HtmlViewer(this);
    aboutbox = new AboutBox(this);
    searchdialog = new SearchDialog(ui->hexview, this);
    verevidencedialog = new VerEvidenceDialog(this);
    imagewindow->setWindowIcon(QIcon(":/thumb"));
    msgviewer->setWindowIcon(QIcon(":/bar/logview"));
    byteviewer->setWindowIcon(QIcon(":/bar/byteconverter"));
    previewreport->setWindowIcon(QIcon(":/bar/reportpreview"));
    previewreport->setWindowTitle("Report Preview");
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
    qRegisterMetaType<QTextCursor>("QTextCursor");
    connect(imagewindow, SIGNAL(HideImageWindow(bool)), this, SLOT(HideImageWindow(bool)), Qt::DirectConnection);
    connect(msgviewer, SIGNAL(HideMessageViewerWindow(bool)), this, SLOT(HideMessageViewer(bool)), Qt::DirectConnection);
    connect(byteviewer, SIGNAL(HideByteConverterWindow(bool)), this, SLOT(HideByteViewer(bool)), Qt::DirectConnection);
    connect(previewreport, SIGNAL(HideReportPreviewWindow(bool)), this, SLOT(HidePreviewReport(bool)), Qt::DirectConnection);
    //connect(verevidencedialog, SIGNAL(VerEvid(QStringList)), this, SLOT(VerifyEvidence(QStringList)));
    connect(verevidencedialog, SIGNAL(HideVerifyWindow(bool)), this, SLOT(HideVerifyWindow(bool)), Qt::DirectConnection);
    connect(isignals, SIGNAL(ProgressUpdate(qint64)), this, SLOT(UpdateProgress(qint64)), Qt::QueuedConnection);
    connect(isignals, SIGNAL(DigUpdate(int, int)), this, SLOT(UpdateDig(int, int)), Qt::QueuedConnection);
    connect(isignals, SIGNAL(ExportUpdate(void)), this, SLOT(UpdateExport()), Qt::QueuedConnection);
    connect(isignals, SIGNAL(ReloadPreview()), previewreport, SLOT(Reload()), Qt::QueuedConnection);
    //connect(isignals, SIGNAL(CarveUpdate(QString, int)), this, SLOT(UpdateCarve(QString, int)), Qt::QueuedConnection);
    connect(isignals, SIGNAL(StatUpdate(QString)), this, SLOT(StatusUpdate(QString)), Qt::QueuedConnection);
    //connect(isignals, SIGNAL(VerifyUpdate(qint64)), this, SLOT(VerifyUpdate(qint64)), Qt::QueuedConnection);
    InitializeAppStructure();
    bookmarkmenu = new QMenu();
    bookmarkmenu->setTitle("Tag Selected As");
    bookmarkmenu->setIcon(QIcon(":/bar/addtotag"));
    tagcheckedmenu = new QMenu();
    tagcheckedmenu->setTitle("Tag Checked As");
    tagcheckedmenu->setIcon(QIcon(":/bar/addtotag"));
    hashlistmenu = new QMenu();
    hashlistmenu->setTitle("Add Selected To");
    hashlistmenu->setIcon(QIcon(":/bar/hashlist"));
    hashcheckedmenu = new QMenu();
    hashcheckedmenu->setTitle("Add Checked To");
    hashcheckedmenu->setIcon(QIcon(":/bar/hashlist"));

    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    ui->analysisToolBar->addWidget(spacer);
    ui->analysisToolBar->addAction(ui->actionchomp);
    ui->analysisToolBar->addAction(ui->actionAbout);

    connect(&volwatcher, SIGNAL(finished()), this, SLOT(UpdateStatus()), Qt::QueuedConnection);
    //connect(&sqlwatcher, SIGNAL(finished()), this, SLOT(UpdateStatus()), Qt::QueuedConnection);
    connect(&openwatcher, SIGNAL(finished()), this, SLOT(OpenUpdate()), Qt::QueuedConnection);
    connect(&digwatcher, SIGNAL(finished()), this, SLOT(FinishDigging()), Qt::QueuedConnection);
    connect(&predigwatcher, SIGNAL(finished()), this, SLOT(FinishPreDigging()), Qt::QueuedConnection);
    connect(&thashwatcher, SIGNAL(finished()), this, SLOT(ThashFinish()), Qt::QueuedConnection);
    connect(&thashsavewatcher, SIGNAL(finished()), this, SLOT(ThashSaveFinish()), Qt::QueuedConnection);
    connect(&exportwatcher, SIGNAL(finished()), this, SLOT(FinishExport()), Qt::QueuedConnection);
    connect(&carvewatcher, SIGNAL(finished()), this, SLOT(FinishCarve()), Qt::QueuedConnection);
    connect(&savewcfwatcher, SIGNAL(finished()), this, SLOT(FinishWombatCaseFile()), Qt::QueuedConnection);
    connect(ui->actionSection, SIGNAL(triggered(bool)), this, SLOT(AddSection()), Qt::DirectConnection);
    //connect(ui->actionTextSection, SIGNAL(triggered(bool)), this, SLOT(AddTextSection()), Qt::DirectConnection);
    connect(ui->actionFile, SIGNAL(triggered(bool)), this, SLOT(CarveFile()), Qt::DirectConnection);
    connect(ui->actionsearchhex, SIGNAL(triggered()), this, SLOT(ShowSearchDialog()), Qt::DirectConnection);
    connect(ui->actionpublishresults, SIGNAL(triggered()), this, SLOT(PublishResults()), Qt::DirectConnection);
    connect(ui->actionchomp, SIGNAL(triggered()), this, SLOT(LaunchChomp()), Qt::DirectConnection);
    connect(ui->actionCreateForensicImage, SIGNAL(triggered()), this, SLOT(ShowForImgDialog()), Qt::DirectConnection);
    connect(ui->actionExportForensicImage, SIGNAL(triggered()), this, SLOT(ExportForensicImage()), Qt::DirectConnection);
    connect(ui->actionVerifyForensicImage, SIGNAL(triggered(bool)), this, SLOT(VerEvidence(bool)), Qt::DirectConnection);
    //connect(ui->actionVerifyForensicImage, SIGNAL(triggered()), this, SLOT(VerEvidence()), Qt::DirectConnection);

    selectionmenu = new QMenu();
    selectionmenu->addAction(ui->actionSection);
    //selectionmenu->addAction(ui->actionTextSection);
    selectionmenu->addAction(ui->actionFile);

    remtagaction = new QAction("Remove Selected Tag", bookmarkmenu);
    remtagaction->setIcon(QIcon(":/bar/tag-rem"));
    connect(remtagaction, SIGNAL(triggered()), this, SLOT(RemoveTag()));
    remtagaction1 = new QAction("Remove All Checked Tags", tagcheckedmenu);
    remtagaction1->setIcon(QIcon(":/bar/tag-rem"));
    connect(remtagaction1, SIGNAL(triggered()), this, SLOT(RemoveTag()));

    remcheckedaction = new QAction("UnCheck all Checked", this);
    remcheckedaction->setIcon(QIcon(":/remcheck"));
    connect(remcheckedaction, SIGNAL(triggered()), this, SLOT(UnCheckChecked()));

    viewerfile.open(QIODevice::ReadOnly);
    viewmenu = new QMenu();
    viewmenu->setTitle("View With");
    viewmenu->setIcon(QIcon(":/bar/setview"));
    QStringList itemlist = QString(viewerfile.readLine()).split(",", Qt::SkipEmptyParts);
    itemlist.removeDuplicates();
    viewerfile.close();
    for(int i=0; i < itemlist.count(); i++)
    {
        QAction* tmpaction = new QAction(itemlist.at(i), this);
        connect(tmpaction, SIGNAL(triggered()), this, SLOT(ShowExternalViewer()));
        viewmenu->addAction(tmpaction);
    }

    treemenu = new QMenu(pathtreeview);
    
    treemenu->addAction(ui->actionView_File);
    treemenu->addAction(ui->actionView_Properties);
    treemenu->addAction(viewmenu->menuAction());
    treemenu->addSeparator();
    treemenu->addAction(ui->actionCheck);
    treemenu->addAction(remcheckedaction);
    treemenu->addSeparator();
    treemenu->addMenu(bookmarkmenu);
    treemenu->addAction(remtagaction);
    treemenu->addMenu(hashlistmenu);
    treemenu->addSeparator();
    treemenu->addMenu(tagcheckedmenu);
    treemenu->addAction(remtagaction1);
    treemenu->addMenu(hashcheckedmenu);
    treemenu->addSeparator();
    treemenu->addAction(ui->actionDigDeeper);
    treemenu->addAction(ui->actionExport);
    treemenu->addAction(ui->actionExportForensicImage);

    pathtreeview->setSortingEnabled(true);
    pathtreeview->setUniformRowHeights(true);
    pathtreeview->header()->setSortIndicatorShown(false);
    pathtreeview->header()->setSectionResizeMode(QHeaderView::Interactive);
    //pathtreeview->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
    pathtreeview->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(pathtreeview, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(TreeContextMenu(const QPoint &)));
    connect(pathtreeview->header(), SIGNAL(sectionClicked(int)), this, SLOT(SetFilter(int)));
    connect(pathtreeview, SIGNAL(LaunchFile(const QModelIndex &)), this, SLOT(ShowFile(const QModelIndex &)));

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
    connect(tagfilterview, SIGNAL(HeaderChanged()), this, SLOT(FilterApplied()));
    connect(hashmatchfilterview, SIGNAL(HeaderChanged()), this, SLOT(FilterApplied()));
    connect(jumpfilterview, SIGNAL(SetOffset()), this, SLOT(SetHexOffset()));
    /*
    jumpforward = new QShortcut(ui->dirTreeView);
    jumpbackward = new QShortcut(ui->dirTreeView);
    showitem = new QShortcut(ui->dirTreeView);
    jumpforward->setKey(Qt::CTRL + Qt::Key_J);
    jumpbackward->setKey(Qt::CTRL + Qt::SHIFT + Qt::Key_J);
    showitem->setKey(Qt::Key_Return);
    connect(jumpforward, SIGNAL(activated()), this, SLOT(NextItem()));
    connect(jumpbackward, SIGNAL(activated()), this, SLOT(PreviousItem()));
    connect(showitem, SIGNAL(activated()), this, SLOT(ShowItem()));
    */
    checkhash.clear();
    listeditems.clear();
    //existingevidence.clear();
    partitionlist.clear();
    carvecounthash.clear();
    treenodemodel = new TreeNodeModel();
    autosavetimer = new QTimer(this);
    //digrotatetimer = new QTimer(this);
    //carverotatetimer = new QTimer(this);
    connect(autosavetimer, SIGNAL(timeout()), this, SLOT(AutoSaveState()));
    //connect(digrotatetimer, SIGNAL(timeout()), this, SLOT(RotateDig()));
    //connect(carverotatetimer, SIGNAL(timeout()), this, SLOT(RotateCarve()));
}

void WombatForensics::UnCheckChecked()
{
    QStringList checkeditems;
    checkeditems.clear();
    checkeditems = GetFileLists(1);
    //qDebug() << "checekeditems:" << checkeditems;
    for(int i=0; i < checkeditems.count(); i++)
    {
        QModelIndexList indexlist = treenodemodel->match(treenodemodel->index(0, treenodemodel->GetColumnIndex("id"), QModelIndex()), Qt::DisplayRole, QVariant(checkeditems.at(i)), -1, Qt::MatchFlags(Qt::MatchExactly | Qt::MatchRecursive));
        if(indexlist.count() > 0)
        {
            actionitem = static_cast<TreeNode*>(indexlist.first().internalPointer());
            if(!actionitem->IsChecked())
                actionitem->SetChecked(true);
            else
                actionitem->SetChecked(false);
        }
    }
    emit treenodemodel->CheckedNodesChanged();
}

void WombatForensics::ReadHashLists()
{
    hashlistmenu->clear();
    hashcheckedmenu->clear();
    QDir hashdir(wombatvariable.tmpmntpath + "hashlists/");
    QFileInfoList whllist = hashdir.entryInfoList(QStringList() << "*.whl", QDir::Files);
    QAction* newhashlistaction = new QAction("New Empty List", hashlistmenu);
    newhashlistaction->setIcon(QIcon(":/bar/newhash"));
    connect(newhashlistaction, SIGNAL(triggered()), this, SLOT(CreateEmptyHashList()));
    hashlistmenu->addAction(newhashlistaction);
    QAction* newhashlistaction1 = new QAction("New Empty List", hashcheckedmenu);
    newhashlistaction1->setIcon(QIcon(":/bar/newhash"));
    connect(newhashlistaction1, SIGNAL(triggered()), this, SLOT(CreateEmptyHashList()));
    hashcheckedmenu->addAction(newhashlistaction1);
    hashlistmenu->addSeparator();
    hashcheckedmenu->addSeparator();
    for(int i=0; i < whllist.count(); i++)
    {
        QAction* tmpaction = new QAction(whllist.at(i).fileName(), hashlistmenu);
        tmpaction->setIcon(QIcon(":/bar/addtohash"));
        tmpaction->setData(QVariant("h" + QString::number(i)));
        connect(tmpaction, SIGNAL(triggered()), this, SLOT(AddExistingHashList()));
        QAction* tmpaction1 = new QAction(whllist.at(i).fileName(), hashcheckedmenu);
        tmpaction1->setIcon(QIcon(":/bar/addtohash"));
        tmpaction1->setData(QVariant("h" + QString::number(i)));
        connect(tmpaction1, SIGNAL(triggered()), this, SLOT(AddExistingHashList()));
        hashlistmenu->addAction(tmpaction);
        hashcheckedmenu->addAction(tmpaction1);
    }
}

void WombatForensics::ReadBookmarks()
{
    QStringList bookitemlist;
    bookitemlist.clear();
    bookmarkfile.open(QIODevice::ReadOnly | QIODevice::Text);
    if(bookmarkfile.isOpen())
        bookitemlist = QString(bookmarkfile.readLine()).split(",", Qt::SkipEmptyParts);
    bookmarkfile.close();
    bookmarkmenu->clear();
    tagcheckedmenu->clear();
    QAction* newtagaction = new QAction("New Tag", bookmarkmenu);
    newtagaction->setIcon(QIcon(":/bar/newtag"));
    connect(newtagaction, SIGNAL(triggered()), this, SLOT(CreateNewTag()));
    bookmarkmenu->addAction(newtagaction);
    QAction* newtagaction1 = new QAction("New Tag", tagcheckedmenu);
    newtagaction1->setIcon(QIcon(":/bar/newtag"));
    connect(newtagaction1, SIGNAL(triggered()), this, SLOT(CreateNewTag()));
    tagcheckedmenu->addAction(newtagaction1);
    bookmarkmenu->addSeparator();
    tagcheckedmenu->addSeparator();
    for(int i=0; i < bookitemlist.count(); i++)
    {
        QAction* tmpaction = new QAction(bookitemlist.at(i), bookmarkmenu);
        tmpaction->setIcon(QIcon(":/bar/addtotag"));
        tmpaction->setData(QVariant("t" + QString::number(i)));
        QAction* tmpaction1 = new QAction(bookitemlist.at(i), tagcheckedmenu);
        tmpaction1->setIcon(QIcon(":/bar/addtotag"));
        tmpaction1->setData(QVariant(QString("t" + QString::number(i))));
        connect(tmpaction, SIGNAL(triggered()), this, SLOT(SetBookmark()));
        connect(tmpaction1, SIGNAL(triggered()), this, SLOT(SetBookmark()));
        bookmarkmenu->addAction(tmpaction);
        tagcheckedmenu->addAction(tmpaction1);
    }
}

void WombatForensics::RemoveTag()
{
    QAction* tagaction = qobject_cast<QAction*>(sender());
    if(QString(tagaction->iconText()).contains("Selected")) // single file
    {
        //QString paridstr = selectedindex.parent().sibling(selectedindex.parent().row(), 11).data().toString().split("-f").last();
        //QDir eviddir = QDir(wombatvariable.tmpmntpath);
        //QStringList evidfiles = eviddir.entryList(QStringList("*-" + selectedindex.sibling(selectedindex.row(), 11).data().toString().split("-").first()), QDir::NoSymLinks | QDir::Dirs);
        //QString evidencename = evidfiles.first();
        /*
        QString estring = selectedindex.sibling(selectedindex.row(), 11).data().toString().split("-", Qt::SkipEmptyParts).at(0);
        QString vstring = selectedindex.sibling(selectedindex.row(), 11).data().toString().split("-", Qt::SkipEmptyParts).at(1);
        QString pstring = selectedindex.sibling(selectedindex.row(), 11).data().toString().split("-", Qt::SkipEmptyParts).at(2);
        QString fstring = selectedindex.sibling(selectedindex.row(), 11).data().toString().split("-", Qt::SkipEmptyParts).at(3);
        QStringList partlist;
        partlist.clear();
        QFile partfile(wombatvariable.tmpmntpath + evidencename + "/" + vstring + "/" + pstring + "/stat");
        partfile.open(QIODevice::ReadOnly | QIODevice::Text);
        if(partfile.isOpen())
            partlist = QString(partfile.readLine()).split(",");
        partfile.close();
        qint64 rootinum = partlist.at(3).toLongLong();
        if(paridstr.contains("-"))
            paridstr = QString::number(rootinum);
        QFile filefile;
        QString filefilepath = wombatvariable.tmpmntpath + evidencename + "/" + vstring + "/" + pstring + "/" + fstring;
        //if(fstring.split(":").count() > 1)
        //    filefilepath += fstring.split(":").first() + "-" + fstring.split(":").last();
        //else
        //    filefilepath += fstring.split(":").first();
        filefilepath += ".a" + paridstr + ".stat";
        filefile.setFileName(filefilepath);
        filefile.open(QIODevice::ReadOnly | QIODevice::Text);
        QString tmpstr = "";
        QStringList tmplist;
        tmplist.clear();
        if(filefile.isOpen())
            tmpstr = filefile.readLine();
        filefile.close();
        */
        int colindex = treenodemodel->GetColumnIndex("id");
        RemoveFileItem(selectedindex.sibling(selectedindex.row(), colindex).data().toString());
        /*
        if(tmpstr.split(",").count() > 15)
            tmplist = tmpstr.split(",", Qt::SkipEmptyParts);
        tmplist[15] = "0";
        tmpstr = "";
        for(int i = 0; i < tmplist.count(); i++)
        {
            tmpstr += tmplist.at(i);
            if(i < tmplist.count() - 1)
                tmpstr += ",";
        }
        filefile.open(QIODevice::WriteOnly | QIODevice::Text);
        if(filefile.isOpen())
            filefile.write(tmpstr.toStdString().c_str());
        filefile.close();
        */
        treenodemodel->UpdateNode(selectedindex.sibling(selectedindex.row(), colindex).data().toString(), "tag", "0");
    }
    else if(QString(tagaction->iconText()).contains("Checked")) // single file
    {
        QStringList checkeditems = GetFileLists(1);
        for(int i=0; i < checkeditems.count(); i++)
        {
            int colindex = treenodemodel->GetColumnIndex("id");
            QModelIndexList indexlist = treenodemodel->match(treenodemodel->index(0, colindex, QModelIndex()), Qt::DisplayRole, QVariant(checkeditems.at(i)), -1, Qt::MatchFlags(Qt::MatchExactly | Qt::MatchRecursive));
            if(indexlist.count() > 0)
            {
                QModelIndex curindex = ((QModelIndex)indexlist.first());
                /*
                QModelIndex parindex = ((QModelIndex)indexlist.first()).parent();
                QString paridstr = parindex.sibling(parindex.row(), 11).data().toString().split("-f").last();
                QDir eviddir = QDir(wombatvariable.tmpmntpath);
                QStringList evidfiles = eviddir.entryList(QStringList("*-" + curindex.sibling(curindex.row(), 11).data().toString().split("-").first()), QDir::NoSymLinks | QDir::Dirs);
                QString evidencename = evidfiles.first();
                QString estring = curindex.sibling(curindex.row(), 11).data().toString().split("-", Qt::SkipEmptyParts).at(0);
                QString vstring = curindex.sibling(curindex.row(), 11).data().toString().split("-", Qt::SkipEmptyParts).at(1);
                QString pstring = curindex.sibling(curindex.row(), 11).data().toString().split("-", Qt::SkipEmptyParts).at(2);
                QString fstring = curindex.sibling(curindex.row(), 11).data().toString().split("-", Qt::SkipEmptyParts).at(3);
                QStringList partlist;
                partlist.clear();
                QFile partfile(wombatvariable.tmpmntpath + evidencename + "/" + vstring + "/" + pstring + "/stat");
                partfile.open(QIODevice::ReadOnly | QIODevice::Text);
                if(partfile.isOpen())
                    partlist = QString(partfile.readLine()).split(",");
                partfile.close();
                qint64 rootinum = partlist.at(3).toLongLong();
                if(paridstr.contains("-"))
                    paridstr = QString::number(rootinum);
                QFile filefile;
                QString filefilepath = wombatvariable.tmpmntpath + evidencename + "/" + vstring + "/" + pstring + "/" + fstring;
                //if(fstring.split(":").count() > 1)
                //    filefilepath += fstring.split(":").first() + "-" + fstring.split(":").last();
                //else
                //    filefilepath += fstring.split(":").first();
                filefilepath += ".a" + paridstr + ".stat";
                QString tmpstr = "";
                QStringList tmplist;
                tmplist.clear();
                filefile.setFileName(filefilepath);
                filefile.open(QIODevice::ReadOnly | QIODevice::Text);
                if(filefile.isOpen())
                    tmpstr = filefile.readLine();
                filefile.close();
                */
                RemoveFileItem(curindex.sibling(curindex.row(), colindex).data().toString());
                /*
                if(tmpstr.split(",").count() > 15)
                    tmplist = tmpstr.split(",", Qt::SkipEmptyParts);
                tmplist[15] = "0";
                tmpstr = "";
                for(int i = 0; i < tmplist.count(); i++)
                {
                    tmpstr += tmplist.at(i);
                    if(i < tmplist.count() - 1)
                        tmpstr += ",";
                }
                filefile.open(QIODevice::WriteOnly | QIODevice::Text);
                if(filefile.isOpen())
                    filefile.write(tmpstr.toStdString().c_str());
                filefile.close();
                */
                treenodemodel->UpdateNode(curindex.sibling(curindex.row(), colindex).data().toString(), "tag", "0");
            }
        }
    }
}

void WombatForensics::CreateEmptyHashList(void)
{
    QAction* tagaction = qobject_cast<QAction*>(sender());
    QString parentmenu = qobject_cast<QMenu*>(tagaction->parentWidget())->title();
    QString emptyfilename = "";
    QStringList filestohash;
    filestohash.clear();
    QInputDialog* newdialog = new QInputDialog(this);
    newdialog->setCancelButtonText("Cancel");
    newdialog->setInputMode(QInputDialog::TextInput);
    newdialog->setLabelText("Enter Hash List Name");
    newdialog->setOkButtonText("Create Empty List");
    newdialog->setTextEchoMode(QLineEdit::Normal);
    newdialog->setWindowTitle("Create Empty Wombat Hash List");
    if(newdialog->exec())
        emptyfilename = newdialog->textValue();
    if(!emptyfilename.isEmpty())
    {
        //qDebug() << "Create Empty Hash List.";
        if(!emptyfilename.endsWith(".whl"))
            emptyfilename = emptyfilename + ".whl";
        QFile tmpfile(wombatvariable.tmpmntpath + "hashlists/" + emptyfilename);
        if(!tmpfile.exists())
        {
            tmpfile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);
            tmpfile.close();
            ReadHashLists();
            if(parentmenu.contains("Selected")) // single file
            {
                int colindex = treenodemodel->GetColumnIndex("id");
                filestohash.append(selectedindex.sibling(selectedindex.row(), colindex).data().toString());
            }
            else if(parentmenu.contains("Checked")) // checked files
            {
                QStringList checkeditems = GetFileLists(1);
                filestohash.append(checkeditems);
            }
	    //qDebug() << "existing evidence:" << existingevidence;
	    // SHOULD SWITCH FROM NEWEVIDENCE/EXISTINGEVIDENCE TO NEWFORIMGLIST/EXISTINGFORIMGLIST
	    //qDebug() << "existingforimglist:" << existingforimglist.first()->ImgPath() << existingforimglist.first()->MountPath();
            //qDebug() << "filestoshash:" << filestohash;
            QStringList fileshashes = QtConcurrent::blockingMapped(filestohash, HashFiles);
	    //qDebug() << "filehashes:" << fileshashes;
            if(!tmpfile.isOpen())
                tmpfile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);
            QTextStream out;
            out.setDevice(&tmpfile);
            for(int i=0; i < fileshashes.count(); i++)
                out << fileshashes.at(i) << Qt::endl;
            tmpfile.close();
        }
        else
            QMessageBox::information(this, "Hash List Exists", "Hash List not created, already exists.", QMessageBox::Ok);
    }
}

void WombatForensics::CreateNewTag()
{
    QAction* tagaction = qobject_cast<QAction*>(sender());
    QString parentmenu = qobject_cast<QMenu*>(tagaction->parentWidget())->title();
    QString tagname = "";
    QInputDialog* newtagdialog = new QInputDialog(this);
    newtagdialog->setCancelButtonText("Cancel");
    newtagdialog->setInputMode(QInputDialog::TextInput);
    newtagdialog->setLabelText("Enter Tag Name");
    newtagdialog->setOkButtonText("Create Tag");
    newtagdialog->setTextEchoMode(QLineEdit::Normal);
    newtagdialog->setWindowTitle("New Tag");
    if(newtagdialog->exec())
        tagname = newtagdialog->textValue();
    if(!tagname.isEmpty())
    {
        int tagid = UpdateBookmarkItems(tagname);
        if(tagid != -15)
        {
            ReadBookmarks();
            AddTLinkItem(tagid, tagname);
            AddTagItem(tagid, tagname);
            if(parentmenu.contains("Selected")) // single file
            {
                TagFile(selectedindex, tagname);
            }
            else if(parentmenu.contains("Checked")) // checked files
            {
                QStringList checkeditems = GetFileLists(1);
                for(int i=0; i < checkeditems.count(); i++)
                {
                    int colindex = treenodemodel->GetColumnIndex("id");
                    QModelIndexList indexlist = treenodemodel->match(treenodemodel->index(0, colindex, QModelIndex()), Qt::DisplayRole, QVariant(checkeditems.at(i)), -1, Qt::MatchFlags(Qt::MatchExactly | Qt::MatchRecursive));
                    if(indexlist.count() > 0)
                    {
                        QModelIndex curindex = ((QModelIndex)indexlist.first());
                        TagFile(curindex, tagname);
                    }
                }
            }
        }
        else
            QMessageBox::information(this, "Tag Exists", "Tag Not Added. Tag Name Already Exists.", QMessageBox::Ok);
    }
}

void WombatForensics::TagFile(QModelIndex curindex, QString tagname)
{
    int colindex = treenodemodel->GetColumnIndex("id");
    //if(curindex.sibling(curindex.row(), 11).data().toString().split("-").count() == 5 || curindex.sibling(curindex.row(), 11).data().toString().contains("-c"))
    if(curindex.sibling(curindex.row(), colindex).data().toString().split("-").count() == 3 || curindex.sibling(curindex.row(), colindex).data().toString().contains("-c") || curindex.sibling(curindex.row(), colindex).data().toString().contains("-z"))
    {
        QTimeZone tmpzone = QTimeZone(reporttimezone);
        //taggedhash.insert(curindex.sibling(curindex.row(), 11).data().toString(), tagname);
        treenodemodel->UpdateNode(curindex.sibling(curindex.row(), colindex).data().toString(), "tag", tagname);
        QString filestr = "<td style='" + ReturnCssString(11) + "' id='" + curindex.sibling(curindex.row(), colindex).data().toString() + "'>";
        filestr += "<table width='300px'><tr><th style='" + ReturnCssString(6) + "' colspan='2'>" + curindex.sibling(curindex.row(), treenodemodel->GetColumnIndex("name")).data().toString() + "</th></tr>";
        filestr += "<tr style='" + ReturnCssString(12) + "'><td style='" + ReturnCssString(13) + "'>File Path:</td><td style='" + ReturnCssString(14) + "'><span style='word-wrap:break-word;'>" + curindex.sibling(curindex.row(), treenodemodel->GetColumnIndex("path")).data().toString() + "</span></td></tr>";
        filestr += "<tr style='" + ReturnCssString(5) + "'><td style='" + ReturnCssString(13) + "'>File Size:</td><td style='" + ReturnCssString(14) + "'>" + curindex.sibling(curindex.row(), treenodemodel->GetColumnIndex("size")).data().toString() + " bytes</td></tr>";
        if(!curindex.sibling(curindex.row(), treenodemodel->GetColumnIndex("create")).data().toString().isEmpty())
            filestr += "<tr style='" + ReturnCssString(4) + "'><td style='" + ReturnCssString(13) + "'>Created:</td><td style='" + ReturnCssString(14) + "'>" + QDateTime::fromSecsSinceEpoch(QDateTime::fromString(curindex.sibling(curindex.row(), treenodemodel->GetColumnIndex("create")).data().toString(), "MM/dd/yyyy hh:mm:ss AP").toSecsSinceEpoch(), tmpzone).toString("MM/dd/yyyy hh:mm:ss AP") + "</td></tr>";
        if(!curindex.sibling(curindex.row(), treenodemodel->GetColumnIndex("access")).data().toString().isEmpty())
            filestr += "<tr style='" + ReturnCssString(4) + "'><td style='" + ReturnCssString(13) + "'>Accessed:</td><td style='" + ReturnCssString(14) + "'>" + QDateTime::fromSecsSinceEpoch(QDateTime::fromString(curindex.sibling(curindex.row(), treenodemodel->GetColumnIndex("access")).data().toString(), "MM/dd/yyyy hh:mm:ss AP").toSecsSinceEpoch(), tmpzone).toString("MM/dd/yyyy hh:mm:ss AP") + "</td></tr>";
        if(!curindex.sibling(curindex.row(), treenodemodel->GetColumnIndex("modify")).data().toString().isEmpty())
            filestr += "<tr style='" + ReturnCssString(4) + "'><td style='" + ReturnCssString(13) + "'>Modified:</td><td style='" + ReturnCssString(14) + "'>" + QDateTime::fromSecsSinceEpoch(QDateTime::fromString(curindex.sibling(curindex.row(), treenodemodel->GetColumnIndex("modify")).data().toString(), "MM/dd/yyyy hh:mm:ss AP").toSecsSinceEpoch(), tmpzone).toString("MM/dd/yyyy hh:mm:ss AP") + "</td></tr>";
        if(!curindex.sibling(curindex.row(), treenodemodel->GetColumnIndex("status")).data().toString().isEmpty())
            filestr += "<tr style='" + ReturnCssString(4) + "'><td style='" + ReturnCssString(13) + "'>Changed:</td><td style='" + ReturnCssString(14) + "'>" + QDateTime::fromSecsSinceEpoch(QDateTime::fromString(curindex.sibling(curindex.row(), treenodemodel->GetColumnIndex("status")).data().toString(), "MM/dd/yyyy hh:mm:ss AP").toSecsSinceEpoch(), tmpzone).toString("MM/dd/yyyy hh:mm:ss AP") + "</td></tr>";
        if(!curindex.sibling(curindex.row(), treenodemodel->GetColumnIndex("hash")).data().toString().isEmpty())
        {
            filestr += "<tr style='" + ReturnCssString(4) + "'><td style='" + ReturnCssString(13) + "'>";
            /*
            if(hashsum == 1)
                filestr += "MD5";
            else if(hashsum == 2)
                filestr += "SHA1";
            else if(hashsum == 4)
                filestr += "SHA256";
            else if(hashsum == 11)
                filestr += "BLAKE3";
            */
            filestr += "BLAKE3 Hash:</td><td style='" + ReturnCssString(14) + "'>" + curindex.sibling(curindex.row(), treenodemodel->GetColumnIndex("hash")).data().toString() + "</td></tr>";
        }
        filestr += "<tr style='" + ReturnCssString(5) + "'><td style='" + ReturnCssString(13) + "'>Category:</td><td style='" + ReturnCssString(14) + "'>" + curindex.sibling(curindex.row(), treenodemodel->GetColumnIndex("cat")).data().toString() + "</td></tr>";
        filestr += "<tr style='" + ReturnCssString(4) + "'><td style='" + ReturnCssString(13) + "'>Signature:</td><td style='" + ReturnCssString(14) + "'>" + curindex.sibling(curindex.row(), treenodemodel->GetColumnIndex("sig")).data().toString() + "</td></tr>";
        filestr += "<tr style='" + ReturnCssString(5) + "'><td style='" + ReturnCssString(13) + "'>ID:</td><td style='" + ReturnCssString(14) + "'>" + curindex.sibling(curindex.row(), colindex).data().toString() + "</td></tr>";
	// IF HTMLVIEWER WON'T OPEN FILE WITHOUT .HTML EXTENSION, THEN I WILL NEED TO ADD THE SHORTCUT AND OTHER HTML PARSED ARTIFACTS TO THIS IF/ELSE STATEMENT...
        if(curindex.sibling(curindex.row(), treenodemodel->GetColumnIndex("cat")).data().toString().contains("Image") || curindex.sibling(curindex.row(), treenodemodel->GetColumnIndex("cat")).data().toString().contains("Video"))
            filestr += "<tr style='" + ReturnCssString(4) + "'><td style='" + ReturnCssString(13) + "'>&nbsp;</td><td style='lvalue'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href='./files/" + curindex.sibling(curindex.row(), colindex).data().toString() + "'><img src='./thumbs/" + curindex.sibling(curindex.row(), colindex).data().toString() + ".png'/></a></td></tr>";
        else
            filestr += "<tr style='" + ReturnCssString(4) + "'><td style='" + ReturnCssString(13) + "'>&nbsp;</td><td style='lvalue'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href='./files/" + curindex.sibling(curindex.row(), colindex).data().toString() + "'>Link</a></td></tr>";
        filestr += "</table></td>";
        RemoveFileItem(curindex.sibling(curindex.row(), colindex).data().toString());
        AddFileItem(tagname, filestr);
        emit treenodemodel->layoutChanged(); // this resolves the issues with the add evidence not updating when you add it later
    }
    /*
    else if(curindex.sibling(curindex.row(), 11).data().toString().contains("-c"))
    {
        QTimeZone tmpzone = QTimeZone(reporttimezone);
        taggedhash.insert(curindex.sibling(curindex.row(), 11).data().toString(), tagname);
        treenodemodel->UpdateNode(curindex.sibling(curindex.row(), 11).data().toString(), 10, tagname);
    }
    */
    else
        qInfo() << "Can only tag files and directories, not evidence images, volumes or partitions.";
}

void WombatForensics::AddExistingHashList(void)
{
    QAction* hashaction = qobject_cast<QAction*>(sender());
    QString parentmenu = qobject_cast<QMenu*>(hashaction->parentWidget())->title();
    QString hashlistfilename = hashaction->text();
    QStringList filestohash;
    filestohash.clear();
    // get selected file or checked files, run hashfiles function
    // open this file, adn loop over returned hash+files and add to the resepective file.
    if(parentmenu.contains("Selected")) // single file
    {
        filestohash.append(selectedindex.sibling(selectedindex.row(), treenodemodel->GetColumnIndex("id")).data().toString());
    }
    else if(parentmenu.contains("Checked")) // checked files
    {
        QStringList checkeditems = GetFileLists(1);
        filestohash.append(checkeditems);
    }
    QStringList fileshashes = QtConcurrent::blockingMapped(filestohash, HashFiles);
    QFile tmpfile(wombatvariable.tmpmntpath + "hashlists/" + hashlistfilename);
    if(!tmpfile.isOpen())
        tmpfile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);
    QTextStream out;
    out.setDevice(&tmpfile);
    for(int i=0; i < fileshashes.count(); i++)
        out << fileshashes.at(i) << Qt::endl;
    tmpfile.close();
}

void WombatForensics::SetBookmark()
{
    QAction* tagaction = qobject_cast<QAction*>(sender());
    QString parentmenu = qobject_cast<QMenu*>(tagaction->parentWidget())->title();
    QString tag = tagaction->text();

    if(parentmenu.contains("Selected")) // single file
    {
        TagFile(selectedindex, tag);
    }
    else if(parentmenu.contains("Checked")) // checked files
    {
        QStringList checkeditems = GetFileLists(1);
        for(int i=0; i < checkeditems.count(); i++)
        {
            QModelIndexList indexlist = treenodemodel->match(treenodemodel->index(0, treenodemodel->GetColumnIndex("id"), QModelIndex()), Qt::DisplayRole, QVariant(checkeditems.at(i)), -1, Qt::MatchFlags(Qt::MatchExactly | Qt::MatchRecursive));
            if(indexlist.count() > 0)
            {
                QModelIndex curindex = ((QModelIndex)indexlist.first());
                TagFile(curindex, tag);
            }
        }
    }
}

void WombatForensics::ShowSearchDialog()
{
    searchdialog->show();
}

void WombatForensics::ShowForImgDialog()
{
    forimgdialog = new ForImgDialog(this);
    forimgdialog->show();
}

void WombatForensics::ExportForensicImage()
{
    qDebug() << "popup export dialog here...";
}

//void WombatForensics::VerEvidence()
void WombatForensics::VerEvidence(bool checked)
{
    if(!checked) // hide viewer
        verevidencedialog->hide();
    else
        verevidencedialog->Show();
        //verevidencedialog->show();
    //verevidencedialog = new VerEvidenceDialog(this);
    //connect(verevidencedialog, SIGNAL(VerEvid(QStringList)), this, SLOT(VerifyEvidence(QStringList)));
    //verevidencedialog->exec();
}

void WombatForensics::ShowExternalViewer()
{
    QString tmpstring = wombatvariable.tmpfilepath + selectedindex.sibling(selectedindex.row(), treenodemodel->GetColumnIndex("id")).data().toString() + "-fhex";
    QProcess* process = new QProcess(this);
    QStringList arguments;
    arguments << tmpstring;
    process->startDetached(((QAction*)QObject::sender())->text(), arguments);
}

void WombatForensics::ShowHideColumn()
{
    int headerindex = ((QAction*)QObject::sender())->data().toInt();
    if(pathtreeview->header()->isSectionHidden(headerindex))
	pathtreeview->header()->showSection(headerindex);
    else
	pathtreeview->header()->hideSection(headerindex);
}

void WombatForensics::SetSelectedFromImageViewer(QString objectid)
{
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
    QModelIndexList indexlist = treenodemodel->match(treenodemodel->index(0, treenodemodel->GetColumnIndex("id"), QModelIndex()), Qt::DisplayRole, QVariant(objectid), -1, Qt::MatchFlags(Qt::MatchExactly | Qt::MatchRecursive));
    if(indexlist.count() > 0)
        pathtreeview->setCurrentIndex(indexlist.at(0));
    QApplication::restoreOverrideCursor();
}

void WombatForensics::ShowFile(const QModelIndex &index)
{
    int colindex = treenodemodel->GetColumnIndex("id");
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
    if(index.sibling(index.row(), treenodemodel->GetColumnIndex("cat")).data().toString().contains("Image"))
    {
        imageviewer = new ImageWindow();
        imageviewer->setWindowIcon(QIcon(":/img"));
        imageviewer->setWindowTitle(selectedindex.sibling(selectedindex.row(), colindex).data().toString() + " Image Viewer");
        imageviewer->setAttribute(Qt::WA_DeleteOnClose);
        imageviewer->GetImage(selectedindex.sibling(selectedindex.row(), colindex).data().toString());
        imageviewer->show();
    }
    else if(index.sibling(index.row(), treenodemodel->GetColumnIndex("cat")).data().toString().contains("Video"))
    {
        videowindow = new VideoViewer();
        videowindow->setAttribute(Qt::WA_DeleteOnClose);
        videowindow->setWindowIcon(QIcon(":/vid"));
        videowindow->setWindowTitle(selectedindex.sibling(selectedindex.row(), colindex).data().toString() + " Video Viewer");
        videowindow->ShowVideo(index);
    }
    else if(index.sibling(index.row(), treenodemodel->GetColumnIndex("sig")).data().toString().contains("HTML"))
    {
        htmlviewer = new HtmlViewer();
        htmlviewer->setAttribute(Qt::WA_DeleteOnClose);
        htmlviewer->setWindowIcon(QIcon(":/web"));
        htmlviewer->setWindowTitle(selectedindex.sibling(selectedindex.row(), colindex).data().toString() + " HTML Viewer");
        htmlviewer->ShowHtml(index);
    }
    else if(index.sibling(index.row(), treenodemodel->GetColumnIndex("cat")).data().toString().contains("Text"))
    {
        // toggle the button...
        textviewer = new TextViewer();
        textviewer->setWindowIcon(QIcon(":/textencode"));
        textviewer->setWindowTitle(selectedindex.sibling(selectedindex.row(), colindex).data().toString() + " Text Viewer");
        textviewer->setAttribute(Qt::WA_DeleteOnClose);
        // THIS FAILS ON ADS
        textviewer->ShowText(index);
    }
    else if(index.sibling(index.row(), treenodemodel->GetColumnIndex("sig")).data().toString().contains("Shortcut"))
    {
        htmlviewer = new HtmlViewer();
        htmlviewer->setAttribute(Qt::WA_DeleteOnClose);
        htmlviewer->setWindowTitle(selectedindex.sibling(selectedindex.row(), colindex).data().toString() + " LNK Viewer");
	htmlviewer->ShowArtifact(0, index); // LNK
    }
    else if(index.sibling(index.row(), treenodemodel->GetColumnIndex("sig")).data().toString().contains("Directory"))
    {
        htmlviewer = new HtmlViewer();
        htmlviewer->setAttribute(Qt::WA_DeleteOnClose);
        htmlviewer->setWindowTitle("$I30 Viewer " + selectedindex.sibling(selectedindex.row(), colindex).data().toString());
	htmlviewer->ShowArtifact(1, index); // I30
    }
    else if(index.sibling(index.row(), treenodemodel->GetColumnIndex("sig")).data().toString().contains("Recycler"))
    {
        htmlviewer = new HtmlViewer();
        htmlviewer->setAttribute(Qt::WA_DeleteOnClose);
        htmlviewer->setWindowTitle("INFO2 Viewer " + selectedindex.sibling(selectedindex.row(), colindex).data().toString());
	htmlviewer->ShowArtifact(2, index); // INFO2
    }
    else if(index.sibling(index.row(), treenodemodel->GetColumnIndex("sig")).data().toString().startsWith("Recycle.Bin"))
    {
        htmlviewer = new HtmlViewer();
        htmlviewer->setAttribute(Qt::WA_DeleteOnClose);
        htmlviewer->setWindowTitle("$I Viewer " + selectedindex.sibling(selectedindex.row(), colindex).data().toString());
	htmlviewer->ShowArtifact(3, index); // $I
    }
    else if(index.sibling(index.row(), treenodemodel->GetColumnIndex("sig")).data().toString().contains("Prefetch"))
    {
        htmlviewer = new HtmlViewer();
        htmlviewer->setAttribute(Qt::WA_DeleteOnClose);
        htmlviewer->setWindowTitle("Prefetch Viewer " + selectedindex.sibling(selectedindex.row(), colindex).data().toString());
	htmlviewer->ShowArtifact(4, index); // Prefetch
    }
    else if(index.sibling(index.row(), treenodemodel->GetColumnIndex("sig")).data().toString().startsWith("Zip"))
    {
        htmlviewer = new HtmlViewer();
        htmlviewer->setAttribute(Qt::WA_DeleteOnClose);
        htmlviewer->setWindowTitle("Archive Viewer " + selectedindex.sibling(selectedindex.row(), colindex).data().toString());
        htmlviewer->ShowArtifact(5, index); // Archive
    }
    else if(index.sibling(index.row(), treenodemodel->GetColumnIndex("sig")).data().toString().startsWith("Pdf"))
    {
	pdfviewer = new PdfViewer();
	pdfviewer->setAttribute(Qt::WA_DeleteOnClose);
	pdfviewer->setWindowTitle("PDF Viewer " + selectedindex.sibling(selectedindex.row(), colindex).data().toString());
	pdfviewer->ShowPdf(index.sibling(index.row(), colindex).data().toString());
    }
    else if(index.sibling(index.row(), treenodemodel->GetColumnIndex("sig")).data().toString().startsWith("Registry"))
    {
        regviewer = new RegistryDialog();
        regviewer->setAttribute(Qt::WA_DeleteOnClose);
        regviewer->setWindowTitle("Registry Viewer " + selectedindex.sibling(selectedindex.row(), colindex).data().toString());
        regviewer->LoadRegistryFile(selectedindex.sibling(selectedindex.row(), colindex).data().toString(), selectedindex.sibling(selectedindex.row(), treenodemodel->GetColumnIndex("name")).data().toString());
    }
    else
    {
        QApplication::restoreOverrideCursor();
        actionitem = static_cast<TreeNode*>(selectedindex.internalPointer());
        if(!actionitem->IsChecked())
        {
            ui->actionCheck->setText("Check Selected");
            ui->actionCheck->setIcon(QIcon(":/echeck"));
        }
        else
        {
            ui->actionCheck->setText("UnCheck Selected");
            ui->actionCheck->setIcon(QIcon(":/remcheck"));
        }
        treemenu->exec(QCursor::pos()); // might want to add the enable/disable depending on whether its a file/dir or not. (4-)
    }
    QApplication::restoreOverrideCursor();
}

void WombatForensics::HideImageWindow(bool checkstate)
{
    ui->actionView_Image_Gallery->setChecked(checkstate);
}

void WombatForensics::HideHashListManager()
{
}

void WombatForensics::HideViewerManager()
{
    treemenu->clear();
    viewmenu->clear();
    viewerfile.open(QIODevice::ReadOnly);
    QStringList itemlist = QString(viewerfile.readLine()).split(",", Qt::SkipEmptyParts);
    viewerfile.close();
    itemlist.removeDuplicates();
    for(int i=0; i < itemlist.count(); i++)
    {
        QAction* tmpaction = new QAction(itemlist.at(i), this);
        connect(tmpaction, SIGNAL(triggered()), this, SLOT(ShowExternalViewer()));
        viewmenu->addAction(tmpaction);
    }
    treemenu->addAction(ui->actionView_File);
    treemenu->addAction(ui->actionView_Properties);
    treemenu->addAction(viewmenu->menuAction());
    treemenu->addSeparator();
    treemenu->addAction(ui->actionCheck);
    treemenu->addAction(remcheckedaction);
    treemenu->addSeparator();
    treemenu->addMenu(bookmarkmenu);
    treemenu->addMenu(tagcheckedmenu);
    treemenu->addAction(remtagaction);
    treemenu->addAction(remtagaction1);
    treemenu->addSeparator();
    treemenu->addAction(ui->actionDigDeeper);
    treemenu->addAction(ui->actionExport);
    treemenu->addAction(ui->actionExportForensicImage);
}

void WombatForensics::HideTagManager()
{
    treemenu->clear();
    ReadBookmarks();
    treemenu->addAction(ui->actionView_File);
    treemenu->addAction(ui->actionView_Properties);
    treemenu->addAction(viewmenu->menuAction());
    treemenu->addSeparator();
    treemenu->addAction(ui->actionCheck);
    treemenu->addAction(remcheckedaction);
    treemenu->addSeparator();
    treemenu->addMenu(bookmarkmenu);
    treemenu->addAction(remtagaction);
    treemenu->addSeparator();
    treemenu->addMenu(tagcheckedmenu);
    treemenu->addAction(remtagaction1);
    treemenu->addSeparator();
    treemenu->addAction(ui->actionDigDeeper);
    treemenu->addAction(ui->actionExport);
    treemenu->addAction(ui->actionExportForensicImage);
}

void WombatForensics::HideSettingsWindow()
{
    ReadSettings();
}

void WombatForensics::ReadSettings()
{
    settingsfile.open(QIODevice::ReadOnly);
    QStringList tmplist = QString(settingsfile.readLine()).split(",", Qt::SkipEmptyParts);
    settingsfile.close();
    // split settings and implement them...
    for(int i = 0; i < tmplist.count(); i++)
    {
        if(tmplist.at(i).split(":").at(0) == "thumb")
            thumbsize = tmplist.at(i).split(":").at(1).toInt();
        else if(tmplist.at(i).split(":").at(0) == "casepath")
        {
            QByteArray ba;
            ba.append(tmplist.at(i).split(":").at(1).toUtf8());
            casepath = QByteArray::fromBase64(ba);
        }
        else if(tmplist.at(i).split(":").at(0) == "reportpath")
        {
            QByteArray ba;
            ba.append(tmplist.at(i).split(":").at(1).toUtf8());
            reportpath = QByteArray::fromBase64(ba);
        }
        else if(tmplist.at(i).split(":").at(0) == "video")
            vidcount = tmplist.at(i).split(":").at(1).toInt();
        else if(tmplist.at(i).split(":").at(0) == "autosave")
            autosave = tmplist.at(i).split(":").at(1).toInt();
        else if(tmplist.at(i).split(":").at(0) == "timezone")
            reporttimezone = QByteArray::fromStdString(QString(tmplist.at(i).split(":").at(1)).toStdString());
        //else if(tmplist.at(i).split(":").at(0) == "save")
        // etc...
    }
    QDir dir;
    if(dir.mkpath(casepath) == false)
        DisplayError("S.1", "App casepath folder failed", "App casepath folder was not created");
    if(dir.mkpath(reportpath) == false)
        DisplayError("S.2", "App reportpath folder failed", "App reportpath folder was not created");
}

void WombatForensics::HideMessageViewer(bool checkstate)
{
    ui->actionViewMessageLog->setChecked(checkstate);
}

void WombatForensics::HideVerifyWindow(bool checkstate)
{
    ui->actionVerifyForensicImage->setChecked(checkstate);
}

void WombatForensics::HideByteViewer(bool checkstate)
{
    ui->actionByteConverter->setChecked(checkstate);
}

void WombatForensics::HidePreviewReport(bool checkstate)
{
    ui->actionpreviewreport->setChecked(checkstate);
}

void WombatForensics::InitializeAppStructure()
{
    wombatvariable.iscaseopen = false;
    QString tmppath = QDir::tempPath();
    //tmppath += "/wombatforensics/";
    QString homepath = QDir::homePath();
    homepath += "/.local/share/wombatforensics/";
    wombatvariable.tmpfilepath = tmppath + "/wombatforensics/";
    wombatvariable.tmpmntpath = homepath + "mntpt/";
    //wombatvariable.imgdatapath = tmppath + "datamnt/";
    QDir dir;
    if(dir.mkpath(tmppath) == false)
        DisplayError("1.6", "App tmppath folder failed", "App tmppath folder was not created");
    if(dir.mkpath(wombatvariable.tmpfilepath) == false)
        DisplayError("1.3", "App tmpfile folder failed", "App Tmpfile folder was not created");
    if(dir.mkpath(wombatvariable.tmpmntpath) == false)
        DisplayError("1.2", "App tmpmnt folder failed", "App tmpmnt folder was not created");
    if(dir.mkpath(homepath) == false)
        DisplayError("1.4", "App homepath folder failed", "App homepath folder was not created");
    //if(dir.mkpath(wombatvariable.imgdatapath) == false)
    //    DisplayError("1.5", "App imgdatapath folder failed", "App imgdatapath folder was not created");

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
    if(!FileExists(QString(homepath + "xchomp").toStdString()))
    {
        QFile::copy(":/xchomp", homepath + "xchomp");
        QFile chompfile(homepath + "xchomp");
        chompfile.setPermissions(QFileDevice::ExeOwner);
    }
    /*
    if(!FileExists(QString(homepath + "aff4imager").toStdString()))
    {
        QFile::copy(":/aff4imager", homepath + "aff4imager");
        QFile aff4file(homepath + "aff4imager");
        aff4file.setPermissions(QFileDevice::ExeOwner);
    }
    */
    QFile ctypes(homepath + "carvetypes");
    if(!FileExists(QString(homepath + "carvetypes").toStdString()))
    {
        QFile::copy(":/files/carvetypes", homepath + "carvetypes");
        QFile ctypefile(homepath + "carvetypes");
        ctypefile.setPermissions(QFileDevice::WriteOwner | QFileDevice::ReadOwner | QFileDevice::WriteGroup | QFileDevice::ReadGroup | QFileDevice::ReadOther);
    }
    viewmanage = new ViewerManager(this);
    settingsdialog = new SettingsDialog(this);
    viewmanage->setWindowIcon(QIcon(":/bar/viewermanager"));
    settingsdialog->setWindowIcon(QIcon(":/bar/settings"));
    //hashlistmanager = new HashListManager(this);
    //hashlistmanager->setWindowIcon(QIcon(":/bar/hashlist"));
    connect(viewmanage, SIGNAL(HideManagerWindow()), this, SLOT(HideViewerManager()), Qt::DirectConnection);
    connect(settingsdialog, SIGNAL(HideSettingsWindow()), this, SLOT(HideSettingsWindow()), Qt::DirectConnection);
    connect(settingsdialog, SIGNAL(UpdateTZ(QString)), this, SLOT(UpdateTimeZone(QString)), Qt::DirectConnection);
    //connect(hashlistmanager, SIGNAL(HideHashListManager()), this, SLOT(HideHashListManager()), Qt::DirectConnection);
    ui->actionSaveState->setEnabled(false);
    ui->actionAdd_Evidence->setEnabled(false);
    ui->actionRemove_Evidence->setEnabled(false);
    ui->actionExport_Evidence->setEnabled(false);
    ui->actionDigDeeper->setEnabled(false);
    ui->actionBookmark_Manager->setEnabled(false);
    ui->actionView_Image_Gallery->setEnabled(false);
    ui->actionTextViewer->setEnabled(false);
    ui->actionByteConverter->setEnabled(false);
    ui->actionJumpToHex->setEnabled(false);
    ui->actionpreviewreport->setEnabled(false);
    ui->actionsearchhex->setEnabled(false);
    ui->actionpublishresults->setEnabled(false);
    ui->actionCarve->setEnabled(false);
    ui->actionExportForensicImage->setEnabled(false);
    ui->actionHashListManager->setEnabled(false);
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
    QInputDialog* casedialog = new QInputDialog(this);
    casedialog->setCancelButtonText("Cancel");
    casedialog->setInputMode(QInputDialog::TextInput);
    casedialog->setLabelText("Enter Case Name");
    casedialog->setOkButtonText("Create Case");
    casedialog->setTextEchoMode(QLineEdit::Normal);
    casedialog->setWindowTitle("New Case");
    if(casedialog->exec())
        wombatvariable.casename = casedialog->textValue();
    if(!wombatvariable.casename.isEmpty())
    {
        QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
        this->setWindowTitle(QString("Wombat Forensics - ") + wombatvariable.casename);
        wombatvariable.tmpmntpath = wombatvariable.tmpmntpath + wombatvariable.casename + "/";
        QDir dir;
        dir.mkpath(wombatvariable.tmpmntpath);
        dir.mkpath(wombatvariable.tmpmntpath + "carved/");
        dir.mkpath(wombatvariable.tmpmntpath + "archives/");
        dir.mkpath(wombatvariable.tmpmntpath + "hashlists/");
        wombatvariable.iscaseopen = true;
        InitializePreviewReport();
        bookmarkfile.setFileName(wombatvariable.tmpmntpath + "bookmarks");
        if(!FileExists(QString(wombatvariable.tmpmntpath + "bookmarks").toStdString()))
        {
            bookmarkfile.open(QIODevice::WriteOnly | QIODevice::Text);
            bookmarkfile.close();
        }
        qInfo() << "Bookmarks File Created";
        ReadBookmarks();
        ReadHashLists();
        logfile.setFileName(wombatvariable.tmpmntpath + "msglog");
        logfile.open(QIODevice::ReadWrite | QIODevice::Append | QIODevice::Text);
        msglog->clear();
        qInfo() << "Log File Created";
        //LogMessage("Log File Created");
        thumbdir.mkpath(wombatvariable.tmpmntpath + "thumbs/");
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
    }
}

void WombatForensics::InitializePreviewReport()
{
    QString initialhtml = "";
    /*
    previewfile.setFileName(":/html/initialhtml");
    previewfile.open(QIODevice::ReadOnly);
    if(previewfile.isOpen())
        initialhtml = previewfile.readAll();
    previewfile.close();
    */
    previewfile.setFileName(wombatvariable.tmpmntpath + "previewreport.html");
    previewfile.open(QIODevice::WriteOnly | QIODevice::Text);
    if(previewfile.isOpen())
    {
        QTimeZone itz = QTimeZone(reporttimezone);
        //previewfile.write(initialhtml.toStdString().c_str());
        QString initialstr = "<html><body style='" + ReturnCssString(0) + "'>";
        initialstr = "<div style='" + ReturnCssString(1) + "'><h1>Case Title:&nbsp;<span id='casename'>" + wombatvariable.casename + "</span></h1></div>\n";
        initialstr += "<div id='tz'><h4>Report Time Zone:&nbsp;" + reporttimezone + "</h4><div><br/>\n";
        initialstr += "<div id='toc'><h2>Contents</h2>";
        initialstr += "<div id='elinks'>";
        initialstr += "<!--firstelink-->";
        initialstr += "<!--lastelink-->";
        initialstr += "</div><br/>";
        initialstr += "<div id='tlinks'>";
        initialstr += "<!--firsttlink-->";
        initialstr += "<!--lasttlink-->";
        initialstr += "</div><br/><br/>";
        initialstr += "</div><br/><br/>";
        initialstr += "<h2>Evidence Items</h2>";
        initialstr += "<div id='evidence'>";
        initialstr += "<!--firstevid-->";
        initialstr += "<!--lastevid-->";
        initialstr += "\n</div><br/><br/>";
        initialstr += "<h2>Tagged Items</h2>";
        initialstr += "<div id='tags'>";
        initialstr += "<!--firsttag-->";
        initialstr += "<!--lasttag--></div>";
        previewfile.write(initialstr.toStdString().c_str());
    }
    previewfile.close();
}

void WombatForensics::InitializeOpenCase()
{
    hashsum = 0;
    QFileDialog opencasedialog(this, tr("Open Existing Case"), casepath, tr("Wombat Forensics Case (*.wfc)"));
    opencasedialog.setLabelText(QFileDialog::Accept, "Open");
    opencasedialog.setOption(QFileDialog::DontUseNativeDialog, true);
    if(opencasedialog.exec())
        wombatvariable.casename = opencasedialog.selectedFiles().first().split("/").last().split(".wfc").first();
    if(!wombatvariable.casename.isEmpty())
    {
        StatusUpdate("Case Opening...");
        QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
        this->setWindowTitle(QString("Wombat Forensics - ") + wombatvariable.casename);
        QByteArray tmparray = QString(casepath + "/" + wombatvariable.casename + ".wfc").toLocal8Bit();
        QByteArray tmparray2 = QString(wombatvariable.tmpmntpath).toLocal8Bit();
        TAR* tarhand;
        tar_open(&tarhand, tmparray.data(), NULL, O_RDONLY, 0644, TAR_GNU);
        tar_extract_all(tarhand, tmparray2.data());
        tar_close(tarhand);
        wombatvariable.tmpmntpath = wombatvariable.tmpmntpath + wombatvariable.casename.split("/").last().split(".wfc").first() + "/";
        OpenCaseMountFinished(0, QProcess::NormalExit); // REMOVE THE WEIRD PROCESS STUFF...
    }
}

void WombatForensics::OpenCaseMountFinished(int exitcode, QProcess::ExitStatus exitstatus)
{
    if(exitcode) {}
    if(exitstatus) {}
    wombatvariable.iscaseopen = true;
    bookmarkfile.setFileName(wombatvariable.tmpmntpath + "bookmarks");
    if(!FileExists(QString(wombatvariable.tmpmntpath + "bookmarks").toStdString()))
    {
        bookmarkfile.open(QIODevice::WriteOnly | QIODevice::Text);
        bookmarkfile.close();
    }
    previewfile.setFileName(wombatvariable.tmpmntpath + "previewreport.html");
    if(!FileExists(QString(wombatvariable.tmpmntpath + "previewreport.html").toStdString()))
        InitializePreviewReport();
    //qInfo() << "Bookmarks File Created";
    ReadBookmarks();
    ReadHashLists();
    logfile.setFileName(wombatvariable.tmpmntpath + "msglog");
    logfile.open(QIODevice::ReadWrite | QIODevice::Append | QIODevice::Text);
    msglog->clear();
    InitializeCheckState();
    InitializePasswordList();
    ui->actionAdd_Evidence->setEnabled(true);
    ui->actionBookmark_Manager->setEnabled(true);
    ui->actionpreviewreport->setEnabled(true);
    ui->actionpublishresults->setEnabled(true);
    autosavetimer->start(autosave * 60000); // 10 minutes in milliseconds for a general setting for real.
    UpdateEvidenceList();
    //if(existingevidence.count() > 0)
    if(existingforimglist.count() > 0)
    {
        /*
        for(int i=0; i < existingevidence.count(); i++)
        {
            QString emntpath = wombatvariable.imgdatapath + existingevidence.at(i).split("/").last() + "/";
	    if(existingevidence.at(i).toLower().endsWith("sfs") || existingevidence.at(i).toLower().endsWith(".zmg") || existingevidence.at(i).toLower().endsWith(".aff") || existingevidence.at(i).endsWith(".000") || existingevidence.at(i).endsWith(".001") || existingevidence.at(i).toLower().endsWith(".e01"))
	    {
                QDir dir;
	        dir.mkpath(emntpath);
	    }
            QStringList args;
            args << existingevidence.at(i) << emntpath;
            if(existingevidence.at(i).endsWith(".sfs"))
            {
                //SquashFuser(emntpath,  existingevidence.at(i));
            }
            else if(existingevidence.at(i).endsWith(".zmg"))
            {
                QProcess::execute("zmgmnt", args);
                //fuserlist.push_back(ZmgFuser(emntpath.toStdString(), existingevidence.at(i).toStdString()));
            }
            else if(existingevidence.at(i).toLower().endsWith(".aff") || existingevidence.at(i).endsWith(".000") || existingevidence.at(i).endsWith(".001"))
            {
                QProcess::execute("affuse", args);
                //fuserlist.push_back(AffFuser(emntpath, existingevidence.at(i)));
            }
            else if(existingevidence.at(i).toLower().endsWith(".e01"))
            {
                QProcess::execute("ewfmount", args);
                //fuserlist.push_back(EwfFuser(emntpath, existingevidence.at(i)));
            }
        }
        */
        QFuture<void> tmpfuture = QtConcurrent::run(LoadTreeModel);
        openwatcher.setFuture(tmpfuture);
        /*
        for(int i=0; i < existingevidence.count(); i++)
        {
            LoadTreeModel(existingevidence.at(i));
            //int libewf_handle_get_media_size(libewf_handle_t *handle, size64_t *media_size, libewf_error_t **error );
            //ProcessVolume(existingevidence.at(i));
        }
        OpenUpdate();
        */
        
        //QFuture<void> tmpfuture = QtConcurrent::map(existingevidence, LoadTreeModel);
        //openwatcher.setFuture(tmpfuture);
        //QFuture<void> tmpfuture = QtConcurrent::map(existingevidence, PopulateTskTree);
	//QFuture<void> tmpfuture = QtConcurrent::map(existingevidence, PopulateTreeModel);
        //openwatcher.setFuture(tmpfuture);
    }
    else
    {
        QApplication::restoreOverrideCursor();
        qInfo() << "Case was Opened Successfully";
        StatusUpdate("Ready");
        ui->actionView_Image_Gallery->setEnabled(false);
    }
}

void WombatForensics::OpenUpdate()
{
    /*
    QString hashstr = "MD5 Hash";
    // update Hash header: 32 = md5, 40 = sha1, 64 = sha256
    if(hashsum == 2)
        hashstr = "SHA1 Hash";
    else if(hashsum == 4)
        hashstr = "SHA256 Hash";
    else if(hashsum == 11)
        hashstr = "BLAKE3 Hash";
        */
    //treenodemodel->UpdateHeaderNode(7, "BLAKE3 Hash");
    thumbdir.mkpath(wombatvariable.tmpmntpath + "carved/");
    thumbdir.mkpath(wombatvariable.tmpmntpath + "archives/");
    thumbdir.mkpath(wombatvariable.tmpmntpath + "thumbs/"); // won't do anything if it already exists
    QDir tdir = QDir(QString(wombatvariable.tmpmntpath + "thumbs/"));
    if(!tdir.isEmpty())
    {
        QFuture<void> tmpfuture = QtConcurrent::run(LoadImagesHash); // load images hash after case open to speed up thumbnail viewing
        thashwatcher.setFuture(tmpfuture);
    }
    /*
    // add manual carved files
    QDir cdir = QDir(wombatvariable.tmpmntpath + "carved/");
    QStringList cfiles = cdir.entryList(QStringList("e*-c*"), QDir::NoSymLinks | QDir::Files);
    if(!cfiles.isEmpty())
    {
        QFuture<void> tmpfuture = QtConcurrent::map(cfiles, PopulateCarvedFiles);
        carvedcount = cfiles.count();
	filesfound = filesfound + carvedcount;
        tmpfuture.waitForFinished();
    }
    QDir adir = QDir(wombatvariable.tmpmntpath + "archives/");
    QStringList afiles = adir.entryList(QStringList("*fz*.stat"), QDir::NoSymLinks | QDir::Files);
    if(!afiles.isEmpty())
    {
        QFuture<void> tmpfuture = QtConcurrent::map(afiles, PopulateArchiveFiles);
        filesfound = filesfound + afiles.count();
        isignals->ProgUpd();
        tmpfuture.waitForFinished();
        //QFuture<void> tmpfuture = QtConcurrent::map(existingevidence, PopulateTreeModel);
        InitializeHashList();
        InitializeTaggedList();
        // should update file count to include the archive files....filesfound = filesfound + afiles.count();
    }
    else
    {
        InitializeHashList();
        InitializeTaggedList();
    }
    */
    // THE ABOVE ARE REPLACED BY TREEMODEL POPULATION FROM FILE...
    // THE BELOW MAY ALSO BE REPLACED, BUT HAVE TO CHECK
    //InitializeHashList();
    //InitializeTaggedList();
    //PrepareEvidenceImage();
    pathtreeview->setModel(treenodemodel);
    connect(treenodemodel, SIGNAL(CheckedNodesChanged()), this, SLOT(UpdateCheckCount()));
    pathtreeview->header()->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(pathtreeview->header(), SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(HeaderContextMenu(const QPoint &)));
    //connect(pathtreeview->selectionModel(), SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)), this, SLOT(SelectionChanged(const QItemSelection &, const QItemSelection &)), Qt::DirectConnection);
    connect(pathtreeview->selectionModel(), SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)), this, SLOT(PathSelectionChanged(const QItemSelection &, const QItemSelection &)), Qt::DirectConnection);
    pathtreeview->header()->setSectionsMovable(true);
    
    //QString curid = InitializeSelectedState();
    //QModelIndexList indexlist = treenodemodel->match(treenodemodel->index(0, treenodemodel->GetColumnIndex("id"), QModelIndex()), Qt::DisplayRole, QVariant(curid), 1, Qt::MatchFlags(Qt::MatchExactly | Qt::MatchRecursive));
    UpdateCheckCount();
    /*
    if(indexlist.count() > 0)
    {
        pathtreeview->setCurrentIndex(indexlist.at(0));
        pathtreeview->selectionModel()->select(indexlist.at(0), QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows | QItemSelectionModel::Select);
    }
    else
        pathtreeview->setCurrentIndex(treenodemodel->index(0, 0, QModelIndex()));
    */
    pathtreeview->setCurrentIndex(treenodemodel->index(0, 0, QModelIndex()));
    if(pathtreeview->model() != NULL)
    {
        ui->actionRemove_Evidence->setEnabled(true);
        ui->actionSaveState->setEnabled(true);
        ui->actionDigDeeper->setEnabled(true);
        ui->actionCarve->setEnabled(true);
        ui->actionJumpToHex->setEnabled(true);
        ui->actionsearchhex->setEnabled(true);
        ui->actionHashListManager->setEnabled(true);
        //ui->actionExpandAll->setEnabled(true);
        //ui->actionCollapseAll->setEnabled(true);
    }
    ui->hexview->ensureVisible();
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
    /*
    if(!digrotatetimer->isActive())
        StatusUpdate("Ready");
    else
        StatusUpdate("Digging Deeper...");
    */
}

void WombatForensics::ThashFinish(void)
{
    qDebug() << "LoadImageHashes Finished";
    StatusUpdate("Thumbnail Library Ready");
    /*
    ui->actionView_Image_Gallery->setEnabled(true);
    if(!digrotatetimer->isActive())
    	StatusUpdate("Ready");
    else
	StatusUpdate("Digging Deeper...");
        */
}

void WombatForensics::PathSelectionChanged(const QItemSelection &curitem, const QItemSelection &previtem)
{
    if(previtem.indexes().count() > 0)
        oldselectedindex = previtem.indexes().at(0);
    if(curitem.indexes().count() > 0)
    {
        selectedindex = curitem.indexes().at(0);
        bool boolok;
	QLocale clocale(QLocale(QLocale::English, QLocale::UnitedStates));
        qint64 sizeval = clocale.toLongLong(selectedindex.sibling(selectedindex.row(), treenodemodel->GetColumnIndex("size")).data().toString(), &boolok);
        //qDebug() << "sizevalue:" << sizeval;
        //qDebug() << "size locale to longlong:" << QLocale(selectedindex.sibling(selectedindex.row(), 2).data().toString())::toLongLong();
        //if(selectedindex.sibling(selectedindex.row(), 2).data().toLongLong() > 0) // file size
        if(sizeval > 0)
            ui->actionView_File->setEnabled(true);
        else
            ui->actionView_File->setEnabled(false);
        //ui->actionView_Properties->setEnabled(true);
        //ui->actionView_File->setEnabled(true);
        ui->actionView_Image_Gallery->setEnabled(true);
        ui->actionTextViewer->setEnabled(true);
        ui->actionExport_Evidence->setEnabled(true);
        ui->actionExportForensicImage->setEnabled(true);
        ui->actionByteConverter->setEnabled(true);
        ui->actionJumpToHex->setEnabled(true);
        ui->actionsearchhex->setEnabled(true);
        QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
        TreeNode* curnode = static_cast<TreeNode*>(selectedindex.internalPointer());
        /*
        if(treenodemodel->rowCount(selectedindex) > 0)
            qDebug() << selectedindex.sibling(selectedindex.row(), 0).data().toString() << "has children";
        else
            qDebug() << selectedindex.sibling(selectedindex.row(), 0).data().toString() << "doesn't have children";
        */
        //if(curnode->itemtype == -1 || curnode->itemtype == 3 || curnode->itemtype == 2 || curnode->itemtype == 1)
        if(treenodemodel->rowCount(selectedindex) > 0)
        {
            ui->pathToolBar->clear();
            uint8_t hasparent = 1;
            QList<QAction*> actionlist;
            actionlist.clear();
            QModelIndex curindex = selectedindex;
            while(hasparent == 1)
            {
                QAction* tmpaction = new QAction(curindex.sibling(curindex.row(), treenodemodel->GetColumnIndex("name")).data().toString(), this);
                tmpaction->setData(curindex.sibling(curindex.row(), treenodemodel->GetColumnIndex("id")).data());
                connect(tmpaction, SIGNAL(triggered()), this, SLOT(SetRootIndex()));
                actionlist.prepend(tmpaction);
                QModelIndex curparent = curindex.parent();
                if(curparent == QModelIndex())
                    hasparent = 0;
                else
                    curindex = curparent;
            }
            QAction* voidaction = new QAction(QIcon(":/bar/burrow"), "BURROW", this);
            voidaction->setData(QVariant("INDEXVOID"));
            connect(voidaction, SIGNAL(triggered()), this, SLOT(SetRootIndex()));
            actionlist.prepend(voidaction);

            ui->pathToolBar->addActions(actionlist);
        }
        PopulateHexContents();
        //LoadHexContents();
        GenerateHexFile(selectedindex);
        QApplication::restoreOverrideCursor();
    }
}


/*
void WombatForensics::SelectionChanged(const QItemSelection &curitem, const QItemSelection &previtem)
{
    if(previtem.indexes().count() > 0)
        oldselectedindex = previtem.indexes().at(0);
    if(curitem.indexes().count() > 0)
    {
        selectedindex = curitem.indexes().at(0);
        bool boolok;
	QLocale clocale(QLocale(QLocale::English, QLocale::UnitedStates));
        qint64 sizeval = clocale.toLongLong(selectedindex.sibling(selectedindex.row(), treenodemodel->GetColumnIndex("size")).data().toString(), &boolok);
        //qDebug() << "sizevalue:" << sizeval;
        //qDebug() << "size locale to longlong:" << QLocale(selectedindex.sibling(selectedindex.row(), 2).data().toString())::toLongLong();
        //if(selectedindex.sibling(selectedindex.row(), 2).data().toLongLong() > 0) // file size
        if(sizeval > 0)
            ui->actionView_File->setEnabled(true);
        else
            ui->actionView_File->setEnabled(false);
        //ui->actionView_Properties->setEnabled(true);
        //ui->actionView_File->setEnabled(true);
        ui->actionView_Image_Gallery->setEnabled(true);
        ui->actionTextViewer->setEnabled(true);
        ui->actionExport_Evidence->setEnabled(true);
        ui->actionExportForensicImage->setEnabled(true);
        ui->actionByteConverter->setEnabled(true);
        ui->actionJumpToHex->setEnabled(true);
        ui->actionsearchhex->setEnabled(true);
        QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
        /*
        ui->pathToolBar->clear();
        uint8_t hasparent = 1;
        QList<QAction*> actionlist;
        actionlist.clear();
        QModelIndex curindex = selectedindex;
        while(hasparent == 1)
        {
            TreeNode* curnode = static_cast<TreeNode*>(curindex.internalPointer());
            if(curnode->itemtype == -1 || curnode->itemtype == 3 || curnode->itemtype == 2 || curnode->itemtype == 1)
            {
                QAction* tmpaction = new QAction(curindex.sibling(curindex.row(), 0).data().toString(), this);
                tmpaction->setData(curindex.sibling(curindex.row(), 11).data());
                connect(tmpaction, SIGNAL(triggered()), this, SLOT(TestData()));
                actionlist.prepend(tmpaction);
            }
            QModelIndex curparent = curindex.parent();
            if(curparent == QModelIndex())
                hasparent = 0;
            else
                curindex = curparent;
        }
        ui->pathToolBar->addActions(actionlist);
        */
        //PopulateHexContents();
        //LoadHexContents();
/*        GenerateHexFile(selectedindex);
        QApplication::restoreOverrideCursor();
    }
}
*/

void WombatForensics::SetRootIndex()
{
    QAction* tagaction = qobject_cast<QAction*>(sender());
    //QString parentmenu = qobject_cast<QMenu*>(tagaction->parentWidget())->title();
    QString text = tagaction->text();
    QString data = tagaction->data().toString();
    //qDebug() << "text:" << text << "data:" << data;
    if(data == "INDEXVOID")
    {
        pathtreeview->setRootIndex(QModelIndex());
    }
    else
    {
	QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
        QModelIndexList indexlist = treenodemodel->match(treenodemodel->index(0, treenodemodel->GetColumnIndex("id"), QModelIndex()), Qt::DisplayRole, QVariant(data), -1, Qt::MatchFlags(Qt::MatchExactly | Qt::MatchRecursive));
        if(indexlist.count() > 0)
        {
            pathtreeview->setRootIndex(indexlist.at(0).sibling(indexlist.at(0).row(), 0));
        }
	QApplication::restoreOverrideCursor();
    }
}

void WombatForensics::TreeContextMenu(const QPoint &pt)
{
    QModelIndex index = pathtreeview->indexAt(pt);
    if(index.isValid())
    {
        actionitem = static_cast<TreeNode*>(index.internalPointer());
        if(!actionitem->IsChecked())
        {
            ui->actionCheck->setText("Check Selected");
            ui->actionCheck->setIcon(QIcon(":/echeck"));
        }
        else
        {
            ui->actionCheck->setText("UnCheck Selected");
            ui->actionCheck->setIcon(QIcon(":/remcheck"));
        }
        treemenu->exec(pathtreeview->mapToGlobal(pt));
    }
}

void WombatForensics::HeaderContextMenu(const QPoint &pt)
{
    headermenu->clear();
    for(int i=0; i < pathtreeview->header()->count(); i++)
    {
	QString tmpstring = "";
	if(pathtreeview->header()->isSectionHidden(i))
	    tmpstring += "Show ";
	else
	    tmpstring += "Hide ";
	tmpstring += treenodemodel->headerData(i, Qt::Horizontal).toString();
	QAction* tmpaction = new QAction(tmpstring, this);
	tmpaction->setData(i);
	connect(tmpaction, SIGNAL(triggered()), this, SLOT(ShowHideColumn()));
	headermenu->addAction(tmpaction);
    }
    int headerindex = pathtreeview->header()->logicalIndexAt(pt);
    headermenu->exec(pathtreeview->header()->mapToGlobal(pt));
}

void WombatForensics::ImgHexMenu(const QPoint &pt)
{
    if(ui->hexview->selectionToReadableString().size() > 0)
        selectionmenu->exec(ui->hexview->mapToGlobal(pt));
}

// I DONT THINK THIS FUNCTION IS USED ANYMORE
void WombatForensics::UpdateDataTable()
{
    StatusUpdate("Processing Complete");
    qInfo() << "Evidence has been successfully added";
    //LogMessage("Evidence has been successfully added");
    StatusUpdate("Evidence ready");
}

void WombatForensics::PrepareEvidenceImage()
{
    /*
    QString tmpstr = "";
    QString mntstr = "";
    for(int i=0; i < existingevidence.count(); i++)
    {
        QDir eviddir(wombatvariable.tmpmntpath);
        QStringList evidfiles = eviddir.entryList(QStringList(QString(existingevidence.at(i).split("/").last() + ".e*")), QDir::NoSymLinks | QDir::Dirs);
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
            //qDebug() << "imagefile:" << imagefile;
            //qDebug() << "imgtype:" << imgtype;
	    if(TSK_IMG_TYPE_ISAFF((TSK_IMG_TYPE_ENUM)imgtype) || imagefile.endsWith(".aff")) // AFF
	    {
                //qDebug() << "is aff";
		if(!QFileInfo::exists(wombatvariable.imgdatapath + tmpstr.split(",").at(3).split("/").last() + ".raw"))
                    AffFuser(wombatvariable.imgdatapath, tmpstr.split(",").at(3));
	    }
	    else if(TSK_IMG_TYPE_ISEWF((TSK_IMG_TYPE_ENUM)imgtype) || imagefile.endsWith(".e01")) // EWF
	    {
		//if(!QFileInfo::exists(wombatvariable.imgdatapath + tmpstr.split(",").at(3).split("/").last() + "/ewf1"))
		if(!QFileInfo::exists(wombatvariable.imgdatapath + tmpstr.split(",").at(3).split("/").last() + ".raw"))
                    EwfFuser(wombatvariable.imgdatapath, tmpstr.split(",").at(3));
	    }
	    else if(imagefile.endsWith(".sfs")) // SFS
	    {
                /*
	        if(!QFileInfo::exists(wombatvariable.imgdatapath + tmpstr.split(",").at(3).split("/").last() + ".dd"))
                    SquashFuser(wombatvariable.imgdatapath,  existingevidence.at(i));
                */
/*	    }
            else if(imagefile.endsWith(".zmg")) // ZMG
            {
		if(!QFileInfo::exists(wombatvariable.imgdatapath + tmpstr.split(",").at(3).split("/").last() + ".dd"))
                    ZmgFuser(QString(wombatvariable.imgdatapath + tmpstr.split(",").at(3).split("/").last()).toStdString(), existingevidence.at(i).toStdString());
            }
	    else if(TSK_IMG_TYPE_ISRAW((TSK_IMG_TYPE_ENUM)imgtype)) // RAW
	    {
		QString imgext = tmpstr.split(",").at(3).split("/").last().split(".").last();
		if(imgext.contains(".000"))
		{
		    if(!QFileInfo::exists(wombatvariable.imgdatapath + tmpstr.split(",").at(3).split("/").last() + ".raw"))
                        AffFuser(wombatvariable.imgdatapath, tmpstr.split(",").at(3));
			//mntstr = "affuse " + tmpstr.split(",").at(3) + " " + wombatvariable.imgdatapath;
		}
		else
		    mntstr = "";
	    }
	    else
	    {
		qDebug() << QString("Image type: " + QString(tsk_img_type_toname((TSK_IMG_TYPE_ENUM)imgtype)) + " is not supported.");
	    }
	    if(!mntstr.isEmpty())
	    {
                /*
                qDebug() << "call affuse:" << mntstr;
		xmntprocess = new QProcess();
		connect(xmntprocess, SIGNAL(readyReadStandardOutput()), this, SLOT(ReadXMountOut()), Qt::QueuedConnection);
		connect(xmntprocess, SIGNAL(readyReadStandardError()), this, SLOT(ReadXMountErr()), Qt::QueuedConnection);
		xmntprocess->setProgram(mntstr);
		xmntprocess->start();
		//xmntprocess->start(mntstr); // removes WARNING Messages but does not capture them.. NEED TO FIX
		//xmntprocess->start(mntstr, QStringList());
                */
/*	    }
            //else
            //    qDebug() << "affuse command not called, function call instead..";
        }
    }*/
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
    //UpdateEvidenceList();
    /*
    for(int i=0; i < newevidence.count(); i++)
    {
	//existingevid.append(newevid.at(i));
	existingevidence.append(newevidence.at(i));
    }
    */
    //newevid.clear();
    //for(int i=0; i < newforimglist.count(); i++)
    existingforimglist.append(newforimglist);

    //newevidence.clear();
    newforimglist.clear();
    //PrepareEvidenceImage();
    //qDebug() << "evidrepdatalist count" << evidrepdatalist.count();
    for(int i=0; i < evidrepdatalist.count(); i++)
    {
        AddELinkItem(evidrepdatalist.at(i).evidid, evidrepdatalist.at(i).evidname);
        AddEvidItem(evidrepdatalist.at(i).evidcontent);
    }
    //LogMessage("Building Initial Evidence Tree...");
    pathtreeview->setModel(treenodemodel);

    headermenu = new QMenu();
    headermenu->clear();
    for(int i=0; i < pathtreeview->header()->count(); i++)
    {
	QString tmpstring = "";
	if(pathtreeview->header()->isSectionHidden(i))
	    tmpstring += "Show ";
	else
	    tmpstring += "Hide ";
	tmpstring += treenodemodel->headerData(i, Qt::Horizontal).toString();
	QAction* tmpaction = new QAction(tmpstring, this);
	tmpaction->setData(i);
	connect(tmpaction, SIGNAL(triggered()), this, SLOT(ShowHideColumn()));
	headermenu->addAction(tmpaction);
    }
    pathtreeview->header()->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(pathtreeview->header(), SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(HeaderContextMenu(const QPoint &)));

    ReadBookmarks();
    ReadHashLists();
    emit treenodemodel->layoutChanged(); // this resolves the issues with the add evidence not updating when you add it later
    pathtreeview->header()->setSectionsMovable(true);
    connect(treenodemodel, SIGNAL(CheckedNodesChanged()), this, SLOT(UpdateCheckCount()));
    connect(pathtreeview->selectionModel(), SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)), this, SLOT(PathSelectionChanged(const QItemSelection &, const QItemSelection &)), Qt::DirectConnection);
    UpdateCheckCount();
    QModelIndexList indexlist = treenodemodel->match(treenodemodel->index(0, treenodemodel->GetColumnIndex("id"), QModelIndex()), Qt::DisplayRole, QVariant(InitializeSelectedState()), -1, Qt::MatchFlags(Qt::MatchExactly | Qt::MatchRecursive));
    if(indexlist.count() > 0)
    {
        pathtreeview->setCurrentIndex(indexlist.at(0));
    }
    else
        pathtreeview->setCurrentIndex(treenodemodel->index(0, 0, QModelIndex()));
    ui->actionRemove_Evidence->setEnabled(true);
    ui->actionSaveState->setEnabled(true);
    ui->actionDigDeeper->setEnabled(true);
    ui->actionCarve->setEnabled(true);
    ui->actionsearchhex->setEnabled(true);
    SavePasswordList();
    qInfo() << "Processing Complete";
    //LogMessage("Processing Complete.");
    StatusUpdate("Evidence ready");
    QApplication::restoreOverrideCursor();
    QtConcurrent::run(GenerateWombatCaseFile);
}

void WombatForensics::AddEvidence()
{
    /*
QList<ForImg*> newforimglist;
QList<ForImg*> existingforimglist;
     */ 
    //newevid.clear();
    newforimglist.clear();
    //newevidence.clear();
    addevidencedialog = new AddEvidenceDialog(this);
    addevidencedialog->exec();
    QDir eviddir = QDir(wombatvariable.tmpmntpath);
    QStringList evidfiles = eviddir.entryList(QStringList(QString("*-e*")), QDir::NoSymLinks | QDir::Dirs);
    ecount = evidfiles.count();
    //for(int i=0; i < newevidence.count(); i++)
    for(int i=0; i < newforimglist.count(); i++)
    {
        //QString evidencepath = wombatvariable.tmpmntpath + newevidence.at(i).split("/").last() + "-e" + QString::number(ecount) + "/";
        QString evidencepath = wombatvariable.tmpmntpath + newforimglist.at(i)->ImgPath().split("/").last() + "-e" + QString::number(ecount) + "/";
	QString emntpath = "";
        QDir dir;
        dir.mkpath(evidencepath);
        newforimglist.at(i)->SetMountPath(evidencepath);
	/*
	if(newevidence.at(i).toLower().endsWith(".zmg") || newevidence.at(i).toLower().endsWith(".aff") || newevidence.at(i).endsWith(".000") || newevidence.at(i).endsWith(".001") || newevidence.at(i).toLower().endsWith(".e01"))
	{
            emntpath = wombatvariable.imgdatapath + newevidence.at(i).split("/").last() + "/";
	    dir.mkpath(emntpath);
	}
	*/
        // need to delete emntpath directories on close for cleanup purposes after unmount...
        ecount++;
	/*
        QStringList args;
        args << newevidence.at(i) << emntpath;
        if(newevidence.at(i).toLower().endsWith(".zmg"))
        {
            QProcess::execute("zmgmnt", args);
            //ZmgFuser(emntpath.toStdString(), newevidence.at(i).toStdString());
            //QtConcurrent::run(ZmgFuser, emntpath.toStdString(), newevidence.at(i).toStdString());
            //fuserlist.push_back(ZmgFuser(emntpath.toStdString(), newevidence.at(i).toStdString()));
            //system(QString("zmgmnt " + newevidence.at(i) + " " + emntpath).toStdString().c_str());
        }
        else if(newevidence.at(i).toLower().endsWith(".aff") || newevidence.at(i).endsWith(".000") || newevidence.at(i).endsWith(".001"))
        {
            QProcess::execute("affuse", args);
        }
        else if(newevidence.at(i).toLower().endsWith(".e01"))
        {
            //ForensicImage* tmpimage = new ForensicImage(newevidence.at(i));
            //evidimglist.push_back(tmpimage);
            //EwfImage* tmpimage = new EwfImage(newevidence.at(i));
            //evidimglist.push_back(tmpimage);
            //testimage->size();
            //ui->hexview->setData(*testimage);
            //QProcess::execute("ewfmount", args);
        }
	*/
        /*
        else if(newevidence.at(i).toLower().endsWith(".aff") || newevidence.at(i).endsWith(".000") || newevidence.at(i).endsWith(".001"))
            fuserlist.push_back(AffFuser(emntpath, newevidence.at(i)));
        else if(newevidence.at(i).toLower().endsWith(".e01"))
            fuserlist.push_back(EwfFuser(emntpath, newevidence.at(i)));
        */
    }
    //if(newevidence.count() > 0)
    if(newforimglist.count() > 0)
    {
        evidrepdatalist.clear();
        /*
        for(int i=0; i < newevidence.count(); i++)
        {
            //int libewf_handle_get_media_size(libewf_handle_t *handle, size64_t *media_size, libewf_error_t **error );
            ProcessVolume(newevidence.at(i));
        }
        */
        //QFuture<void> tmpfuture = QtConcurrent::map(newevidence, ProcessVolume);
        QFuture<void> tmpfuture = QtConcurrent::map(newforimglist, ProcessForensicImage);
        volwatcher.setFuture(tmpfuture);
        //QFuture<void> tmpfuture = QtConcurrent::map(newforimglist, ProcessVolume);
        //QFuture<void> tmpfuture = QtConcurrent::map(newevid, ProcessVolume);
        //UpdateStatus();
        //QFuture<void> tmpfuture = QtConcurrent::map(newevidence, InitializeEvidenceStructure);
        //sqlwatcher.setFuture(tmpfuture);
    }
}

void WombatForensics::UpdateProperties()
{
    QFile propfile;
    QDir eviddir = QDir(wombatvariable.tmpmntpath);
    QString nodeid = selectedindex.sibling(selectedindex.row(), treenodemodel->GetColumnIndex("id")).data().toString();
    QStringList evidfiles = eviddir.entryList(QStringList("*-*" + nodeid.split("-").at(0)), QDir::NoSymLinks | QDir::Dirs);
    //qDebug() << "evidfiles:" << evidfiles;
    QString evidencename = evidfiles.at(0).split("-e").first();
    propertylist.clear();
    if(nodeid.split("-").count() == 1) // evidence image
    {
        propfile.setFileName(wombatvariable.tmpmntpath + evidfiles.at(0) + "/prop"); // not implemented yet
    }
    else if(nodeid.split("-").count() == 2) // file system/partition or carved file (maybe)
    {
        propfile.setFileName(wombatvariable.tmpmntpath + evidfiles.at(0) + "/" + nodeid.split("-").at(1) + "/prop");
    }
    else if(nodeid.split("-").count() == 3) // file/directory
    {
        if(nodeid.contains("fz")) // archive, change default directory
            propfile.setFileName(wombatvariable.tmpmntpath + "archives" + nodeid + ".prop");
        else
            propfile.setFileName(wombatvariable.tmpmntpath + evidfiles.at(0) + "/" + nodeid.split("-").at(1) + "/" + nodeid.split("-").at(2) + ".prop");
    }

    propfile.open(QIODevice::ReadOnly | QIODevice::Text);
    if(propfile.isOpen())
    {
        QTextStream in(&propfile);
        while(!in.atEnd())
        {
            //QString tmpstr = "";
            QString line = in.readLine();
            //if(line.split("||").at(1).contains("^^"))
            //    tmpstr = QString(line.split("||").at(1)).replace(QString("^^"), QString(", "));
            //else
            //    tmpstr = line.split("||").at(1);
            //propertylist << line.split("||").at(0) << tmpstr << line.split("||").at(2);
            propertylist << line.split("|").at(0) << line.split("|").at(1) << line.split("|").at(2);
        }
        propfile.close();
    }
    /*****
    if(selectedindex.sibling(selectedindex.row(), 11).data().toString().split("-").count() == 1) // evidence image
    {
        propfile.setFileName(wombatvariable.tmpmntpath + evidencename + "." + selectedindex.sibling(selectedindex.row(), 11).data().toString().split("-").first() + "/prop");
    }
    if(selectedindex.sibling(selectedindex.row(), 11).data().toString().split("-").count() == 2) // volume
    {
        propfile.setFileName(wombatvariable.tmpmntpath + evidencename + "." + selectedindex.sibling(selectedindex.row(), 11).data().toString().split("-").at(0) + "/" + selectedindex.sibling(selectedindex.row(), 11).data().toString().split("-").at(1) + "/prop");
    }
    if(selectedindex.sibling(selectedindex.row(), 11).data().toString().split("-").count() == 3) // file system
    {
        propfile.setFileName(wombatvariable.tmpmntpath + evidencename + "." + selectedindex.sibling(selectedindex.row(), 11).data().toString().split("-").at(0) + "/" + selectedindex.sibling(selectedindex.row(), 11).data().toString().split("-").at(1) + "/" + selectedindex.sibling(selectedindex.row(), 11).data().toString().split("-").at(2) + "/prop");
    }
    if(selectedindex.sibling(selectedindex.row(), 11).data().toString().split("-").count() == 5) // file
    {
        QString itemid = selectedindex.sibling(selectedindex.row(), 11).data().toString();
        if(itemid.contains("fz")) // archive, change default directory
            propfile.setFileName(wombatvariable.tmpmntpath + "archives/" + itemid + ".prop");
        else
            propfile.setFileName(wombatvariable.tmpmntpath + evidencename + "." + itemid.split("-").at(0) + "/" + itemid.split("-").at(1) + "/" + itemid.split("-").at(2) + "/" + itemid.split("-").at(3) + "." + itemid.split("-").at(4) + ".prop");
    *********/
        /*
        QString tmpfvalue = "";
        //QString parentstr = "5"; // NTFS ROOT INUM
        if(selectedindex.parent().sibling(selectedindex.parent().row(), 11).data().toString().split("-").count() == 3) // root inum
        {
            QFile fsstatfile(wombatvariable.tmpmntpath + evidencename + "." + selectedindex.sibling(selectedindex.row(), 11).data().toString().split("-").at(0) + "/" + selectedindex.sibling(selectedindex.row(), 11).data().toString().split("-").at(1) + "/" + selectedindex.sibling(selectedindex.row(), 11).data().toString().split("-").at(2) + "/stat");
            fsstatfile.open(QIODevice::ReadOnly | QIODevice::Text);
            QString tmpstr = "";
            if(fsstatfile.isOpen())
                tmpstr = fsstatfile.readLine();
            fsstatfile.close();
            if(tmpstr.split(",").count() > 0)
                parentstr = tmpstr.split(",").at(3);
        }
        else
            parentstr = selectedindex.parent().sibling(selectedindex.parent().row(), 11).data().toString().split("-").at(3).mid(1);
        //if(selectedindex.sibling(selectedindex.row(), 11).data().toString().split("-").at(3).contains(":"))
        //    tmpfvalue = selectedindex.sibling(selectedindex.row(), 11).data().toString().split("-").at(3).split(":").at(0) + QString("-") + selectedindex.sibling(selectedindex.row(), 11).data().toString().split("-").at(3).split(":").at(1);
        //else
            tmpfvalue = selectedindex.sibling(selectedindex.row(), 11).data().toString().split("-").at(3);
        propfile.setFileName(wombatvariable.tmpmntpath + evidencename + "." + selectedindex.sibling(selectedindex.row(), 11).data().toString().split("-").at(0) + "/" + selectedindex.sibling(selectedindex.row(), 11).data().toString().split("-").at(1) + "/" + selectedindex.sibling(selectedindex.row(), 11).data().toString().split("-").at(2) + "/" + tmpfvalue + ".a" + parentstr + ".prop");
        */
    /********
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
    *********/
    propertywindow->UpdateTableView();
    propertywindow->show();
}

void WombatForensics::GenerateHexFile(const QModelIndex curindex)
{
    QString curid = curindex.sibling(curindex.row(), treenodemodel->GetColumnIndex("id")).data().toString();
    if(curid.contains("-z")) // zip file
    {
        int err = 0;
        RewriteSelectedIdContent(curindex.parent()); // writes parent content to use to load zip content.
        QString fnamestr = wombatvariable.tmpfilepath + curid.split("-z").at(0) + "-fhex";
        zip* curzip = zip_open(fnamestr.toStdString().c_str(), ZIP_RDONLY, &err);
        struct zip_stat zipstat;
        zip_stat_init(&zipstat);
        int zipid = curid.split("-z").at(1).toInt();
        zip_stat_index(curzip, zipid, 0, &zipstat);
        char* zipbuf = new char[zipstat.size];
        zip_file_t* curfile = NULL;
        if(zipstat.encryption_method == ZIP_EM_NONE)
            curfile = zip_fopen_index(curzip, zipid, 0);
        if(curfile != NULL)
        {
            zip_fread(curfile, zipbuf, zipstat.size);
            zip_fclose(curfile);
        }
        QFile ztmp(wombatvariable.tmpfilepath + curid + "-fhex");
        if(!ztmp.isOpen())
            ztmp.open(QIODevice::WriteOnly);
        if(ztmp.isOpen())
        {
            QDataStream zbuffer(&ztmp);
            zbuffer.writeRawData(zipbuf, zipstat.size);
            ztmp.close();
        }
        delete[] zipbuf;
        hexstring = wombatvariable.tmpfilepath + curid + "-fhex";
    }
    else if(curid.endsWith("-cm") || curid.endsWith("-cv") || curid.endsWith("-cu")) // manual carved virtual directory
    {
    }
    else if(curid.contains("-c")) // carved file
    {
        QFile cfile(wombatvariable.tmpmntpath + "carved/" + curid + ".prop");
        QString tmpstr;
        if(!cfile.isOpen())
            cfile.open(QIODevice::ReadOnly | QIODevice::Text);
        if(cfile.isOpen())
        {
            tmpstr = QString(cfile.readLine()).split(";").at(0);
            cfile.close();
        }
        QByteArray carvebuf = ui->hexview->dataAt(tmpstr.split(",", Qt::SkipEmptyParts).at(0).toLongLong(), tmpstr.split(",", Qt::SkipEmptyParts).at(1).toLongLong()); // carved data
        QFile hexfile(hexstring);
        if(!hexfile.isOpen())
            hexfile.open(QIODevice::WriteOnly);
        if(hexfile.isOpen())
        {
            hexfile.write(carvebuf);
            hexfile.close();
        }
    }
    else if(curid.split("-").count() == 3) // file/dir
    {
        bool boolok;
	QLocale clocale(QLocale(QLocale::English, QLocale::UnitedStates));
        qint64 sizeval = clocale.toLongLong(curindex.sibling(curindex.row(), treenodemodel->GetColumnIndex("size")).data().toString(), &boolok);
        if(sizeval > 0)
            RewriteSelectedIdContent(curindex);
    }
    else if(curid.split("-").count() == 1 && curindex.sibling(curindex.row(), treenodemodel->GetColumnIndex("sig")).data().toString().isEmpty() == false)
    {
	bool boolok;
	QLocale clocale(QLocale(QLocale::English, QLocale::UnitedStates));
	qint64 sizeval = clocale.toLongLong(curindex.sibling(curindex.row(), treenodemodel->GetColumnIndex("size")).data().toString(), &boolok);
	if(sizeval > 0)
	    RewriteSelectedIdContent(curindex);
    }
    // MAY NEED TO FIGURE OUT AN ELSE IF FOR THE ADDING A FILE AS FORENSIC IMAGE...
    /*
    if(curindex.sibling(curindex.row(), 11).data().toString().split("-").count() == 5)
    {
        bool boolok;
	QLocale clocale(QLocale(QLocale::English, QLocale::UnitedStates));
        qint64 sizeval = clocale.toLongLong(curindex.sibling(curindex.row(), 2).data().toString(), &boolok);
        //if(curindex.sibling(curindex.row(), 2).data().toLongLong() > 0)
        if(sizeval > 0)
            RewriteSelectedIdContent(curindex);
    }
    else if(curindex.sibling(curindex.row(), 11).data().toString().contains("-c"))
    {
        QString tmpstr;
        QFile cfile(wombatvariable.tmpmntpath + "carved/" + curindex.sibling(curindex.row(), 11).data().toString() + ".stat");
        if(!cfile.isOpen())
            cfile.open(QIODevice::ReadOnly | QIODevice::Text);
        if(cfile.isOpen())
            tmpstr = cfile.readLine();
        cfile.close();
        //qDebug() << "name|id|offset|size:" << tmpstr.split(",").at(0) << tmpstr.split(",").at(12) << tmpstr.split(",").at(16) << tmpstr.split(",").at(8);
        QByteArray carvebuffer = ui->hexview->dataAt(tmpstr.split(",").at(16).toLongLong(), tmpstr.split(",").at(8).toLongLong()); // Carved Data
        hexstring = wombatvariable.tmpfilepath + tmpstr.split(",").at(12) + "-fhex";
        QFile hexfile(hexstring);
        if(!hexfile.isOpen())
            hexfile.open(QIODevice::WriteOnly);
        if(hexfile.isOpen())
            hexfile.write(carvebuffer);
        hexfile.close();
    }
    //else
    //    qDebug() << "generatehex isn't setup for:" << curindex.sibling(curindex.row(), 11).data().toString() << "types yet.";
    */
}

// MAYBE I SHOULD HAVE A POPULATEHEXCONTENTS WHICH GET'S IT FROM THE EVIDENCE IMAGE...
// THEN I CAN HAVE AN LOCATEOFFSET WHICH THEN NAVIAGTES TO THE OFFSET...
// OR I CAN DO IT ALL IN POPULATEHEXCONTENTS, BUT NEED TO SEE IF CURRENT HEXCONTENTS == NEW HEXCONTENTS AND THEN SKIP LOADING HEX, AND JUST GO TO LOCATING NEW OFFSET...
void WombatForensics::PopulateHexContents()
{
    //qDebug() << "populatehexcontents";
    ForImg* curimg = NULL;
    selectednode = static_cast<TreeNode*>(selectedindex.internalPointer());
    QString nodeid = selectednode->Data("id").toString();
    if(nodeid.contains("z")) // since we can't navigate to file contents which are part of a zip due to compression, navigate to the parent.zip file instead.
    {
        selectednode = static_cast<TreeNode*>(selectedindex.parent().internalPointer());
        nodeid = selectednode->Data("id").toString();
    }
    for(int i=0; i < existingforimglist.count(); i++)
    {
	curimg = existingforimglist.at(i);
	if(curimg->MountPath().endsWith(nodeid.split("-").at(0)))
	{
            break;
            //qDebug() << curimg->ImgPath() << curimg->MountPath();
        }
    }
    if(curimg != NULL)
    {
        QString tmpstr = "";
        /*
        QString evidid = nodeid.split("-").first();
        QDir eviddir = QDir(wombatvariable.tmpmntpath);
        QStringList evidfiles = eviddir.entryList(QStringList(QString("*-*" + evidid)), QDir::NoSymLinks | QDir::Dirs);
        QString evidname = evidfiles.first().split(QString("-" + evidid)).first();
        QString tmpstr = "";
        QFile evidfile(wombatvariable.tmpmntpath + evidfiles.first() + "/stat");
        evidfile.open(QIODevice::ReadOnly | QIODevice::Text);
        if(evidfile.isOpen())
            tmpstr = evidfile.readLine(); // original evidence filename, evidence mount string, imgsize, id
        evidfile.close();
        */
        //qDebug() << "tmpstr:" << tmpstr;
        /*
        for(int i=0; i < existingevid.count(); i++)
        {
            if(tmpstr.split(",", Qt::SkipEmptyParts).at(1) == existingevid.at(i)->ImgPath())
                ui->hexview->setData(*existingevid.at(i));
            //qDebug() << QString("newevid " + QString::number(i) + ":") << newevid.at(i)->ImgPath();
        }
        */
        // EwfImage need to get the imgpath from the correct img from the vector of qiodevices
        //casedatafile.setFileName(tmpstr.split(",", Qt::SkipEmptyParts).at(1));
        //casedatafile.setFileName(curimg->ImgPath());
        ui->hexview->BypassColor(false);
        ui->hexview->setData(*curimg);

        //ui->hexview->setData(casedatafile);
        //ui->hexview->setData(casedatafile);
        //(newevid.at(0))->open(QIODevice::ReadOnly);
        //ui->hexview->setData((*newevid.at(0)));
        
        if(nodeid.split("-").count() == 1) // image file
        {
            ui->hexview->setCursorPosition(0);
            ui->hexview->SetColor(QString("0," + QString::number(selectednode->Data("size").toLongLong()) + ";"), selectednode->Data("size").toLongLong());
        }
        else if(nodeid.split("-").count() == 2) // unallocated, file system, or partition, possibly carved, zip, etc...
        {
            if(nodeid.contains("-c")) // manually carved file
            {
                QFile cfile(wombatvariable.tmpmntpath + "carved/" + nodeid + ".prop");
                if(!cfile.isOpen())
                    cfile.open(QIODevice::ReadOnly | QIODevice::Text);
                if(cfile.isOpen())
                {
                    tmpstr = QString(cfile.readLine()).split(";", Qt::SkipEmptyParts).at(0);
                    cfile.close();
                }
                //qDebug() << "offset 2:" << QString::number(tmpstr.split(",").at(0).toLongLong(), 16);
                ui->hexview->BypassColor(true);
                ui->hexview->setCursorPosition(tmpstr.split(",", Qt::SkipEmptyParts).at(0).toULongLong()*2);
                ui->hexview->SetColor(tmpstr, selectednode->Data("size").toLongLong());
            }
            else
            {
                QFile partfile(curimg->MountPath() + "/" + nodeid.split("-").at(1) + "/stat");
                //qDebug() << "partfile:" << partfile.fileName();
                //QFile partfile(wombatvariable.tmpmntpath + evidfiles.first() + "/" + nodeid.split("-").at(1) + "/stat");
                partfile.open(QIODevice::ReadOnly | QIODevice::Text);
                if(partfile.isOpen())
                    tmpstr = partfile.readLine(); // partition name, offset, size, partition type, id
                partfile.close();
                //qDebug() << "tmpstr:" << tmpstr;
                ui->hexview->setCursorPosition(tmpstr.split(",", Qt::SkipEmptyParts).at(0).toLongLong()*2);
                ui->hexview->SetColor(QString(tmpstr.split(",", Qt::SkipEmptyParts).at(0) + "," + tmpstr.split(",", Qt::SkipEmptyParts).at(1) + ";"), tmpstr.split(",", Qt::SkipEmptyParts).at(1).toLongLong());
            }
        }
        else if(nodeid.split("-").count() == 3) // file/directory
        {
            QFile partfile(curimg->MountPath() + "/" + nodeid.split("-").at(1) + "/stat");
            //QFile partfile(wombatvariable.tmpmntpath + evidfiles.first() + "/" + nodeid.split("-").at(1) + "/stat");
            partfile.open(QIODevice::ReadOnly | QIODevice::Text);
            if(partfile.isOpen())
                tmpstr = partfile.readLine(); // part name, offset, size, part type, id
            partfile.close();
            //qDebug() << "selectedindex name:" << selectedindex.sibling(selectedindex.row(), 0).data().toString();
            if(selectedindex.sibling(selectedindex.row(), treenodemodel->GetColumnIndex("name")).data().toString() == "$MBR")
            {
                ui->hexview->setCursorPosition(tmpstr.split(",", Qt::SkipEmptyParts).at(1).toULongLong()*2);
                ui->hexview->SetColor(QString("0,512;"), 512);
            }
            else if(selectedindex.sibling(selectedindex.row(), treenodemodel->GetColumnIndex("name")).data().toString().startsWith("$FAT"))
            {
                uint fatoffset = 0;
                uint fatsize = 0;
                uint bytespersector = 0;
                QFile ppropfile(curimg->MountPath() + "/" + nodeid.split("-").at(1) + "/prop");
                //QFile ppropfile(wombatvariable.tmpmntpath + evidfiles.first() + "/" + nodeid.split("-").at(1) + "/prop");
                ppropfile.open(QIODevice::ReadOnly | QIODevice::Text);
                if(ppropfile.isOpen())
                {
                    while(!ppropfile.atEnd())
                    {
                        tmpstr = ppropfile.readLine();
                        if(tmpstr.startsWith("FAT Offset"))
                            fatoffset = tmpstr.split("|").at(1).toUInt();
                        else if(tmpstr.startsWith("Bytes Per Sector"))
                            bytespersector = tmpstr.split("|").at(1).toUInt();
                        else if(tmpstr.startsWith("FAT Size"))
                            fatsize = tmpstr.split("|").at(1).toUInt() * bytespersector;
                    }
                    ppropfile.close();
                }
                uint fatnum = selectedindex.sibling(selectedindex.row(), treenodemodel->GetColumnIndex("name")).data().toString().right(1).toUInt();
                ui->hexview->setCursorPosition((fatoffset + fatsize * (fatnum - 1)) * 2);
                ui->hexview->SetColor(QString(QString::number(fatoffset + fatsize * (fatnum - 1)) + "," + QString::number(fatsize) + ";"), fatsize - 1);
            }
            else
            {
                QString layout = "";
                QFile fpropfile(curimg->MountPath() + "/" + nodeid.split("-").at(1) + "/" + nodeid.split("-").at(2) + ".prop");
                //QFile fpropfile(wombatvariable.tmpmntpath + evidfiles.first() + "/" + nodeid.split("-").at(1) + "/" + nodeid.split("-").at(2) + ".prop");
                fpropfile.open(QIODevice::ReadOnly | QIODevice::Text);
                if(fpropfile.isOpen())
                {
                    QString tmpstr = "";
                    while(!fpropfile.atEnd())
                    {
                        tmpstr = fpropfile.readLine();
                        if(tmpstr.startsWith("Layout"))
                        {
                            layout = tmpstr.split("|").at(1);
                            break;
                        }
                    }
                    fpropfile.close();
                }
                ui->hexview->setCursorPosition(layout.split(",").at(0).toUInt() * 2);
                ui->hexview->SetColor(layout, selectednode->Data("size").toLongLong());
            }
        }
        else if(nodeid.split("-").count() == 4) // zip file
        {
            qDebug() << "get zip's parent layout here...";
        }
        ui->hexview->ensureVisible();
    }


    //(newevid.at(0))->close();
    /*
    QString datastring = wombatvariable.imgdatapath;
    if(evidname.toLower().endsWith(".aff"))
    {
        datastring += evidname + "/" + evidname + ".raw";
        //datastring += tmpstr.split(",").at(3).split("/").last() + "/" + tmpstr.split(",").at(3).split("/").last() + ".raw";
    }
    else if(evidname.toLower().endsWith(".e01"))
    {
        datastring += evidname + "/" + evidname + ".raw";
        //datastring += tmpstr.split(",").at(3).split("/").last() + "/ewf1";
        //datastring += tmpstr.split(",").at(3).split("/").last() + "/" + tmpstr.split(",").at(3).split("/").last() + ".raw";
    }
    else if(datastring.endsWith(".zmg"))
    {
	datastring += evidname + "/" + evidname.split(".zmg").first() + ".dd";
        //datastring = wombatvariable.imgdatapath + tmpstr.split(",").at(3).split("/").last() + "/" + tmpstr.split(",").at(3).split("/").last().split(".zmg").first() + ".dd";
    }
    else
    {
        if(imgext.contains(".000") || imgext.contains(".001"))
        {
            datastring += evidname + "/" + evidname + ".raw";
            //datastring += tmpstr.split(",").at(3).split("/").last() + "/" + tmpstr.split(",").at(3).split("/").last() + ".raw";
        }
        else
        {
            datastring = "get raw path...";
            //datastring = tmpstr.split(",").at(3);
        }
    }

    //qDebug() << "datastring:" << datastring;
    casedatafile.setFileName(datastring);
    ui->hexview->BypassColor(false);
    ui->hexview->setData(casedatafile);
    */
    /*
     *
    // NEED TO GET EVIDENCE NAME FROM STAT FILE
    selectednode = static_cast<TreeNode*>(selectedindex.internalPointer());
    QString nodeid = selectednode->Data(11).toString();
    qDebug() << "nodeid:" << nodeid;
    if(nodeid.split("-f").last().startsWith("z"))
    {
        selectednode = static_cast<TreeNode*>(selectedindex.parent().internalPointer());
        nodeid = selectednode->Data(11).toString();
    }
    QString evidid = nodeid.split("-").first();
    qDebug() << "evidid:" << evidid;
    QDir eviddir = QDir(wombatvariable.tmpmntpath);
    QStringList evidfiles = eviddir.entryList(QStringList(QString("*-*" + evidid)), QDir::NoSymLinks | QDir::Dirs);
    qDebug() << "evidfiles:" << evidfiles;
    QString evidname = evidfiles.first().split(QString("-" + evidid)).first();
    QString tmpstr = "";
    */
    /*
    QFile evidfile(wombatvariable.tmpmntpath + evidfiles.first() + "/stat");
    evidfile.open(QIODevice::ReadOnly | QIODevice::Text);
    if(evidfile.isOpen())
        tmpstr = evidfile.readLine();
    evidfile.close();
    */ /*
    QString datastring = wombatvariable.imgdatapath;
    //if(TSK_IMG_TYPE_ISAFF((TSK_IMG_TYPE_ENUM)tmpstr.split(",").at(0).toInt()) || tmpstr.split(",").at(3).endsWith(".aff"))
    if(evidname.toLower().endsWith(".aff"))
    {
        datastring += evidname + "/" + evidname + ".raw";
        //datastring += tmpstr.split(",").at(3).split("/").last() + "/" + tmpstr.split(",").at(3).split("/").last() + ".raw";
    }
    else if(evidname.toLower().endsWith(".e01"))
    //else if(TSK_IMG_TYPE_ISEWF((TSK_IMG_TYPE_ENUM)tmpstr.split(",").at(0).toInt()) || tmpstr.split(",").at(3).toLower().endsWith(".e01"))
    {
        datastring += evidname + "/" + evidname + ".raw";
        //datastring += tmpstr.split(",").at(3).split("/").last() + "/ewf1";
        //datastring += tmpstr.split(",").at(3).split("/").last() + "/" + tmpstr.split(",").at(3).split("/").last() + ".raw";
    }
    */
    /*else if(TSK_IMG_TYPE_ISRAW((TSK_IMG_TYPE_ENUM)tmpstr.split(",").at(0).toInt()))
    {
        QString imgext = evidname.split(".").last();
        //QString imgext = tmpstr.split(",").at(3).split("/").last().split(".").last();
        if(imgext.contains(".000") || imgext.contains(".001"))
        {
            datastring += evidname + "/" + evidname + ".raw";
            //datastring += tmpstr.split(",").at(3).split("/").last() + "/" + tmpstr.split(",").at(3).split("/").last() + ".raw";
        }
        else
        {
            datastring = "get raw path...";
            //datastring = tmpstr.split(",").at(3);
        }
    }*/ /*
    //else
    //{
    //    qDebug() << QString("Image type: " + QString(tsk_img_type_toname((TSK_IMG_TYPE_ENUM)tmpstr.split(",").at(0).toInt())) + " is not supported.");
    //}
    //if(datastring.endsWith(".sfs"))
    //{
	//datastring = 
        //datastring = wombatvariable.imgdatapath + tmpstr.split(",").at(3).split("/").last() + "/" + tmpstr.split(",").at(3).split("/").last().split(".sfs").first() + ".dd";
    //}
    else if(datastring.endsWith(".zmg"))
    {
	datastring += evidname + "/" + evidname.split(".zmg").first() + ".dd";
        //datastring = wombatvariable.imgdatapath + tmpstr.split(",").at(3).split("/").last() + "/" + tmpstr.split(",").at(3).split("/").last().split(".zmg").first() + ".dd";
    }
    //qDebug() << "datastring:" << datastring;
    casedatafile.setFileName(datastring);
    ui->hexview->BypassColor(false);
    ui->hexview->setData(casedatafile);

    // determine offset location in the editor
    if(nodeid.split("-").count() == 1) // image file
        ui->hexview->setCursorPosition(0);
    else if(nodeid.split("-").count() == 2 && !nodeid.contains("-c")) // volume file
    {
        QFile volfile(wombatvariable.tmpmntpath + evidfiles.first() + "/" + nodeid.split("-").at(1) + "/stat");
        volfile.open(QIODevice::ReadOnly | QIODevice::Text);
        if(volfile.isOpen())
            tmpstr = volfile.readLine();
        volfile.close();
        ui->hexview->setCursorPosition(tmpstr.split(",").at(4).toInt()*2);
    }
    else if(nodeid.contains("-c"))//else if(nodeid.split("-").count() == 2 && nodeid.contains("-c")) // carved file
    {
        QFile cfile(wombatvariable.tmpmntpath + "carved/" + nodeid + ".stat");
        if(!cfile.isOpen())
            cfile.open(QIODevice::ReadOnly | QIODevice::Text);
        if(cfile.isOpen())
            tmpstr = cfile.readLine();
        cfile.close();
        ui->hexview->BypassColor(true);
        ui->hexview->SetColorInformation(0, 0, 0, 0, tmpstr.split(",").at(16), tmpstr.split(",").at(8).toULongLong(), 0);
        if(tmpstr.split(",").count() > 15)
            ui->hexview->setCursorPosition(tmpstr.split(",").at(16).toULongLong()*2);
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
    else if(nodeid.split("-").count() == 4) // $Carved directory
    {
        casedatafile.setFileName(QDir::tempPath() + "/zfile");
        casedatafile.open(QIODevice::WriteOnly | QIODevice::Text);
        casedatafile.write("dummy zerofile");
        casedatafile.close();
        ui->hexview->BypassColor(true);
        ui->hexview->setData(casedatafile);
        ui->hexview->setEnabled(false);
        ui->actionsearchhex->setEnabled(false);
    }
    else if(nodeid.split("-").count() == 5) // dir/file
    {
        QString estring = nodeid.split("-", Qt::SkipEmptyParts).at(0);
        QString vstring = nodeid.split("-", Qt::SkipEmptyParts).at(1);
        QString pstring = nodeid.split("-", Qt::SkipEmptyParts).at(2);
        QString fstring = nodeid.split("-", Qt::SkipEmptyParts).at(3);
        QString astring = nodeid.split("-", Qt::SkipEmptyParts).at(4);
        QString paridstr = astring.mid(1);
        QStringList partlist;
        partlist.clear();
        QFile partfile(wombatvariable.tmpmntpath + evidfiles.first() + "/" + nodeid.split("-").at(1) + "/" + nodeid.split("-").at(2) + "/stat");
        partfile.open(QIODevice::ReadOnly | QIODevice::Text);
        if(partfile.isOpen())
            partlist = QString(partfile.readLine()).split(",");
        partfile.close();
        qint64 fsoffset = partlist.at(4).toLongLong();
        qint64 rootinum = partlist.at(3).toLongLong();
        int blocksize = partlist.at(6).toInt();
        int fstype = partlist.at(0).toInt();
        if(paridstr.contains("-"))
            paridstr = QString::number(rootinum);
        int mftaddress = 0;
        if(fstring.contains("a"))
            mftaddress = astring.mid(2).toInt();
        else
            mftaddress = fstring.mid(1).toInt();
        ui->hexview->setEnabled(true);
        ui->actionsearchhex->setEnabled(true);
        if(selectednode->Data(2).toInt() == 0) // zero file
        {
            casedatafile.setFileName(QDir::tempPath() + "/zfile");
            casedatafile.open(QIODevice::WriteOnly | QIODevice::Text);
            casedatafile.write("dummy zerofile");
            casedatafile.close();
            ui->hexview->BypassColor(true);
            ui->hexview->setData(casedatafile);
            ui->hexview->setEnabled(false);
            ui->actionsearchhex->setEnabled(false);
        }
        else
        {
            QString blockstring = "";
            QString residentstring = "";
            QString bytestring = "";
            QFile filefileprop;
            filefileprop.setFileName(wombatvariable.tmpmntpath + evidfiles.first() + "/" + vstring + "/" + pstring + "/" + fstring + ".a" + paridstr + ".prop");
            filefileprop.open(QIODevice::ReadOnly | QIODevice::Text);
            while(!filefileprop.atEnd())
            {
                QString tmpstring = filefileprop.readLine();
                if(tmpstring.contains("Block Address"))
                    blockstring = tmpstring.split("||").at(1);
                else if(tmpstring.contains("Byte Offset"))
                    bytestring = tmpstring.split("||").at(1);
                else if(tmpstring.contains("Data Attribute"))
                    residentstring = tmpstring.split("||").at(1);
            }
            filefileprop.close();
            bool isresident = true;
            if(residentstring.contains("Non"))
                isresident = false;
            // ALTERNATIVE IF/ELSE METHOD TO SHORTEN CODE
            bool isres = isresident;
            bool isntfs = false;
            bool isads = false;
            bool isdir = false;
	    bool isapfs = false;
	    bool ishfs = false;
	    if(fstype == TSK_FS_TYPE_APFS_DETECT)
		isapfs = true;
	    if(fstype == TSK_FS_TYPE_HFS_DETECT)
		ishfs = true;
            if(fstype == TSK_FS_TYPE_NTFS_DETECT)
                isntfs = true;
            if(fstring.contains("a"))
	        isads = true;
            if(selectednode->itemtype == 3 || selectednode->itemtype == 11) // IF DIRECTORY (ALWAYS RESIDENT)
	        isdir = true;

	    // ADD APFS FEATURES HERE...
            if(isntfs && isres) // NTFS & RESIDENT
            {
    	        unsigned int curoffset = 0;
		uint16_t mftoffset = 0;
	        uint32_t atrtype = 0;
	        uint8_t namelength = 0;
	        uint32_t attrlength = 0;
	        uint16_t resoffset = 0;
		QStringList mftblocklist;
		mftblocklist.clear();
		QString mftid = nodeid.split("-").first() + "-" + nodeid.split("-").at(1) + "-" + nodeid.split("-").at(2) + "-f0-a5";
		mftblocklist = mftblockhash.value(mftid).split("^^", Qt::SkipEmptyParts);
                // NEW RESIDENT OFFSET CALCULATION
                qint64 residentoffset = 0;
                if((mftaddress * 1024/blocksize) % 2 == 0) // even number, get the starting block.
                {
                    residentoffset = (mftblocklist.at(mftaddress * 1024/blocksize).toLongLong() * blocksize) + fsoffset;
                }
                else // odd number, get starting block and jump the fractional amount to get to the correct entry.
                {
                    float mftblock = mftaddress * 1024.0/blocksize;
                    int mftblockint = floor(mftblock);
                    residentoffset = (mftblocklist.at(mftblockint).toLongLong() * blocksize) + fsoffset + (blocksize * (mftblock - mftblockint));
                }
                QByteArray resbuffer = ui->hexview->dataAt(residentoffset, 1024); // MFT Entry
	        if(resbuffer.count() > 0)
	        {
                    curoffset = 0;
		    mftoffset = qFromLittleEndian<uint16_t>(resbuffer.mid(20, 2)); // offset to first attribute
		    uint16_t attrcnt = qFromLittleEndian<uint16_t>(resbuffer.mid(40, 2)); // next attribute id
		    curoffset += mftoffset;
		    // Loop over attributes...
                    for(uint i = 0; i < attrcnt; i++)
                    {
			atrtype = qFromLittleEndian<uint32_t>(resbuffer.mid(curoffset, 4)); // attribute type
			namelength = qFromLittleEndian<uint8_t>(resbuffer.mid(curoffset + 9, 1)); // length of name
			attrlength = qFromLittleEndian<uint32_t>(resbuffer.mid(curoffset + 4, 4)); // attribute length
		        if(isdir && atrtype == 144)
                        {
			    break;
                        }
		        if(!isdir && isads && namelength > 0 && atrtype == 128)
                        {
			    break;
                        }
		        else if(!isdir && !isads && namelength == 0 && atrtype == 128)
                        {
			    break;
                        }
                        if(atrtype == 4294967295)
                        {
                            //qDebug() << "next attribute is 0xFFFFFFFF";
                            break;
                        }
                        curoffset += attrlength;
                    }
		    resoffset = qFromLittleEndian<uint16_t>(resbuffer.mid(curoffset + 20, 2)); // resident attribute content offset
                    ui->hexview->SetColorInformation(partlist.at(4).toLongLong(), partlist.at(6).toLongLong(), blockstring, QString::number(residentoffset + curoffset + resoffset - fsoffset), bytestring, selectednode->Data(2).toLongLong(), (curoffset + resoffset));
                    ui->hexview->setCursorPosition((residentoffset + curoffset + resoffset)*2);
                }
            }
            else if(isapfs || ishfs)
            {
                // NO BYTESTRING AND BLOCKLIST FOR APFS... NEED TO FIGURE OUT HOW APFS DOES IT...
                ui->hexview->SetColorInformation(partlist.at(4).toLongLong(), partlist.at(6).toLongLong(), blockstring, "", bytestring, selectednode->Data(2).toLongLong(), 0);
                ui->hexview->setCursorPosition(bytestring.toLongLong()*2);
                //qDebug() << "apfs works...";
            }
            else // NTFS NON-RESIDENT or ALTERNATIVE FILE SYSTEM
	    {
                ui->hexview->SetColorInformation(partlist.at(4).toLongLong(), partlist.at(6).toLongLong(), blockstring, "", bytestring, selectednode->Data(2).toLongLong(), 0);
                ui->hexview->setCursorPosition(bytestring.toLongLong()*2);
	    }
        }
    }
    //else
    //    qDebug() << "count for nodeid:" << nodeid.split("-").count();
    ui->hexview->ensureVisible();

     */ 
}
void WombatForensics::LoadHexContents()
{
    /*
    // NEED TO GET EVIDENCE NAME FROM STAT FILE
    selectednode = static_cast<TreeNode*>(selectedindex.internalPointer());
    QString nodeid = selectednode->Data(11).toString();
    qDebug() << "nodeid:" << nodeid;
    if(nodeid.split("-f").last().startsWith("z"))
    {
        selectednode = static_cast<TreeNode*>(selectedindex.parent().internalPointer());
        nodeid = selectednode->Data(11).toString();
    }
    QString evidid = nodeid.split("-").first();
    qDebug() << "evidid:" << evidid;
    QDir eviddir = QDir(wombatvariable.tmpmntpath);
    QStringList evidfiles = eviddir.entryList(QStringList(QString("*-*" + evidid)), QDir::NoSymLinks | QDir::Dirs);
    qDebug() << "evidfiles:" << evidfiles;
    QString evidname = evidfiles.first().split(QString("-" + evidid)).first();
    QString tmpstr = "";
    /*
    QFile evidfile(wombatvariable.tmpmntpath + evidfiles.first() + "/stat");
    evidfile.open(QIODevice::ReadOnly | QIODevice::Text);
    if(evidfile.isOpen())
        tmpstr = evidfile.readLine();
    evidfile.close();
    */
/*    QString datastring = wombatvariable.imgdatapath;
    //if(TSK_IMG_TYPE_ISAFF((TSK_IMG_TYPE_ENUM)tmpstr.split(",").at(0).toInt()) || tmpstr.split(",").at(3).endsWith(".aff"))
    if(evidname.toLower().endsWith(".aff"))
    {
        datastring += evidname + "/" + evidname + ".raw";
        //datastring += tmpstr.split(",").at(3).split("/").last() + "/" + tmpstr.split(",").at(3).split("/").last() + ".raw";
    }
    else if(evidname.toLower().endsWith(".e01"))
    //else if(TSK_IMG_TYPE_ISEWF((TSK_IMG_TYPE_ENUM)tmpstr.split(",").at(0).toInt()) || tmpstr.split(",").at(3).toLower().endsWith(".e01"))
    {
        datastring += evidname + "/" + evidname + ".raw";
        //datastring += tmpstr.split(",").at(3).split("/").last() + "/ewf1";
        //datastring += tmpstr.split(",").at(3).split("/").last() + "/" + tmpstr.split(",").at(3).split("/").last() + ".raw";
    }
    /*else if(TSK_IMG_TYPE_ISRAW((TSK_IMG_TYPE_ENUM)tmpstr.split(",").at(0).toInt()))
    {
        QString imgext = evidname.split(".").last();
        //QString imgext = tmpstr.split(",").at(3).split("/").last().split(".").last();
        if(imgext.contains(".000") || imgext.contains(".001"))
        {
            datastring += evidname + "/" + evidname + ".raw";
            //datastring += tmpstr.split(",").at(3).split("/").last() + "/" + tmpstr.split(",").at(3).split("/").last() + ".raw";
        }
        else
        {
            datastring = "get raw path...";
            //datastring = tmpstr.split(",").at(3);
        }
    }*/
    //else
    //{
    //    qDebug() << QString("Image type: " + QString(tsk_img_type_toname((TSK_IMG_TYPE_ENUM)tmpstr.split(",").at(0).toInt())) + " is not supported.");
    //}
    //if(datastring.endsWith(".sfs"))
    //{
	//datastring = 
        //datastring = wombatvariable.imgdatapath + tmpstr.split(",").at(3).split("/").last() + "/" + tmpstr.split(",").at(3).split("/").last().split(".sfs").first() + ".dd";
    //}
/*    else if(datastring.endsWith(".zmg"))
    {
	datastring += evidname + "/" + evidname.split(".zmg").first() + ".dd";
        //datastring = wombatvariable.imgdatapath + tmpstr.split(",").at(3).split("/").last() + "/" + tmpstr.split(",").at(3).split("/").last().split(".zmg").first() + ".dd";
    }
    //qDebug() << "datastring:" << datastring;
    casedatafile.setFileName(datastring);
    ui->hexview->BypassColor(false);
    ui->hexview->setData(casedatafile);

    // determine offset location in the editor
    if(nodeid.split("-").count() == 1) // image file
        ui->hexview->setCursorPosition(0);
    else if(nodeid.split("-").count() == 2 && !nodeid.contains("-c")) // volume file
    {
        QFile volfile(wombatvariable.tmpmntpath + evidfiles.first() + "/" + nodeid.split("-").at(1) + "/stat");
        volfile.open(QIODevice::ReadOnly | QIODevice::Text);
        if(volfile.isOpen())
            tmpstr = volfile.readLine();
        volfile.close();
        ui->hexview->setCursorPosition(tmpstr.split(",").at(4).toInt()*2);
    }
    else if(nodeid.contains("-c"))//else if(nodeid.split("-").count() == 2 && nodeid.contains("-c")) // carved file
    {
        QFile cfile(wombatvariable.tmpmntpath + "carved/" + nodeid + ".stat");
        if(!cfile.isOpen())
            cfile.open(QIODevice::ReadOnly | QIODevice::Text);
        if(cfile.isOpen())
            tmpstr = cfile.readLine();
        cfile.close();
        ui->hexview->BypassColor(true);
        ui->hexview->SetColorInformation(0, 0, 0, 0, tmpstr.split(",").at(16), tmpstr.split(",").at(8).toULongLong(), 0);
        if(tmpstr.split(",").count() > 15)
            ui->hexview->setCursorPosition(tmpstr.split(",").at(16).toULongLong()*2);
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
    else if(nodeid.split("-").count() == 4) // $Carved directory
    {
        casedatafile.setFileName(QDir::tempPath() + "/zfile");
        casedatafile.open(QIODevice::WriteOnly | QIODevice::Text);
        casedatafile.write("dummy zerofile");
        casedatafile.close();
        ui->hexview->BypassColor(true);
        ui->hexview->setData(casedatafile);
        ui->hexview->setEnabled(false);
        ui->actionsearchhex->setEnabled(false);
    }
    else if(nodeid.split("-").count() == 5) // dir/file
    {
        QString estring = nodeid.split("-", Qt::SkipEmptyParts).at(0);
        QString vstring = nodeid.split("-", Qt::SkipEmptyParts).at(1);
        QString pstring = nodeid.split("-", Qt::SkipEmptyParts).at(2);
        QString fstring = nodeid.split("-", Qt::SkipEmptyParts).at(3);
        QString astring = nodeid.split("-", Qt::SkipEmptyParts).at(4);
        QString paridstr = astring.mid(1);
        QStringList partlist;
        partlist.clear();
        QFile partfile(wombatvariable.tmpmntpath + evidfiles.first() + "/" + nodeid.split("-").at(1) + "/" + nodeid.split("-").at(2) + "/stat");
        partfile.open(QIODevice::ReadOnly | QIODevice::Text);
        if(partfile.isOpen())
            partlist = QString(partfile.readLine()).split(",");
        partfile.close();
        qint64 fsoffset = partlist.at(4).toLongLong();
        qint64 rootinum = partlist.at(3).toLongLong();
        int blocksize = partlist.at(6).toInt();
        int fstype = partlist.at(0).toInt();
        if(paridstr.contains("-"))
            paridstr = QString::number(rootinum);
        int mftaddress = 0;
        if(fstring.contains("a"))
            mftaddress = astring.mid(2).toInt();
        else
            mftaddress = fstring.mid(1).toInt();
        ui->hexview->setEnabled(true);
        ui->actionsearchhex->setEnabled(true);
        if(selectednode->Data(2).toInt() == 0) // zero file
        {
            casedatafile.setFileName(QDir::tempPath() + "/zfile");
            casedatafile.open(QIODevice::WriteOnly | QIODevice::Text);
            casedatafile.write("dummy zerofile");
            casedatafile.close();
            ui->hexview->BypassColor(true);
            ui->hexview->setData(casedatafile);
            ui->hexview->setEnabled(false);
            ui->actionsearchhex->setEnabled(false);
        }
        else
        {
            QString blockstring = "";
            QString residentstring = "";
            QString bytestring = "";
            QFile filefileprop;
            filefileprop.setFileName(wombatvariable.tmpmntpath + evidfiles.first() + "/" + vstring + "/" + pstring + "/" + fstring + ".a" + paridstr + ".prop");
            filefileprop.open(QIODevice::ReadOnly | QIODevice::Text);
            while(!filefileprop.atEnd())
            {
                QString tmpstring = filefileprop.readLine();
                if(tmpstring.contains("Block Address"))
                    blockstring = tmpstring.split("||").at(1);
                else if(tmpstring.contains("Byte Offset"))
                    bytestring = tmpstring.split("||").at(1);
                else if(tmpstring.contains("Data Attribute"))
                    residentstring = tmpstring.split("||").at(1);
            }
            filefileprop.close();
            bool isresident = true;
            if(residentstring.contains("Non"))
                isresident = false;
            // ALTERNATIVE IF/ELSE METHOD TO SHORTEN CODE
            bool isres = isresident;
            bool isntfs = false;
            bool isads = false;
            bool isdir = false;
	    bool isapfs = false;
	    bool ishfs = false;
	    if(fstype == TSK_FS_TYPE_APFS_DETECT)
		isapfs = true;
	    if(fstype == TSK_FS_TYPE_HFS_DETECT)
		ishfs = true;
            if(fstype == TSK_FS_TYPE_NTFS_DETECT)
                isntfs = true;
            if(fstring.contains("a"))
	        isads = true;
            if(selectednode->itemtype == 3 || selectednode->itemtype == 11) // IF DIRECTORY (ALWAYS RESIDENT)
	        isdir = true;

	    // ADD APFS FEATURES HERE...
            if(isntfs && isres) // NTFS & RESIDENT
            {
    	        unsigned int curoffset = 0;
		uint16_t mftoffset = 0;
	        uint32_t atrtype = 0;
	        uint8_t namelength = 0;
	        uint32_t attrlength = 0;
	        uint16_t resoffset = 0;
		QStringList mftblocklist;
		mftblocklist.clear();
		QString mftid = nodeid.split("-").first() + "-" + nodeid.split("-").at(1) + "-" + nodeid.split("-").at(2) + "-f0-a5";
		mftblocklist = mftblockhash.value(mftid).split("^^", Qt::SkipEmptyParts);
                // NEW RESIDENT OFFSET CALCULATION
                qint64 residentoffset = 0;
                if((mftaddress * 1024/blocksize) % 2 == 0) // even number, get the starting block.
                {
                    residentoffset = (mftblocklist.at(mftaddress * 1024/blocksize).toLongLong() * blocksize) + fsoffset;
                }
                else // odd number, get starting block and jump the fractional amount to get to the correct entry.
                {
                    float mftblock = mftaddress * 1024.0/blocksize;
                    int mftblockint = floor(mftblock);
                    residentoffset = (mftblocklist.at(mftblockint).toLongLong() * blocksize) + fsoffset + (blocksize * (mftblock - mftblockint));
                }
                QByteArray resbuffer = ui->hexview->dataAt(residentoffset, 1024); // MFT Entry
	        if(resbuffer.count() > 0)
	        {
                    curoffset = 0;
		    mftoffset = qFromLittleEndian<uint16_t>(resbuffer.mid(20, 2)); // offset to first attribute
		    uint16_t attrcnt = qFromLittleEndian<uint16_t>(resbuffer.mid(40, 2)); // next attribute id
		    curoffset += mftoffset;
		    // Loop over attributes...
                    for(uint i = 0; i < attrcnt; i++)
                    {
			atrtype = qFromLittleEndian<uint32_t>(resbuffer.mid(curoffset, 4)); // attribute type
			namelength = qFromLittleEndian<uint8_t>(resbuffer.mid(curoffset + 9, 1)); // length of name
			attrlength = qFromLittleEndian<uint32_t>(resbuffer.mid(curoffset + 4, 4)); // attribute length
		        if(isdir && atrtype == 144)
                        {
			    break;
                        }
		        if(!isdir && isads && namelength > 0 && atrtype == 128)
                        {
			    break;
                        }
		        else if(!isdir && !isads && namelength == 0 && atrtype == 128)
                        {
			    break;
                        }
                        if(atrtype == 4294967295)
                        {
                            //qDebug() << "next attribute is 0xFFFFFFFF";
                            break;
                        }
                        curoffset += attrlength;
                    }
		    resoffset = qFromLittleEndian<uint16_t>(resbuffer.mid(curoffset + 20, 2)); // resident attribute content offset
                    ui->hexview->SetColorInformation(partlist.at(4).toLongLong(), partlist.at(6).toLongLong(), blockstring, QString::number(residentoffset + curoffset + resoffset - fsoffset), bytestring, selectednode->Data(2).toLongLong(), (curoffset + resoffset));
                    ui->hexview->setCursorPosition((residentoffset + curoffset + resoffset)*2);
                }
            }
            else if(isapfs || ishfs)
            {
                // NO BYTESTRING AND BLOCKLIST FOR APFS... NEED TO FIGURE OUT HOW APFS DOES IT...
                ui->hexview->SetColorInformation(partlist.at(4).toLongLong(), partlist.at(6).toLongLong(), blockstring, "", bytestring, selectednode->Data(2).toLongLong(), 0);
                ui->hexview->setCursorPosition(bytestring.toLongLong()*2);
                //qDebug() << "apfs works...";
            }
            else // NTFS NON-RESIDENT or ALTERNATIVE FILE SYSTEM
	    {
                ui->hexview->SetColorInformation(partlist.at(4).toLongLong(), partlist.at(6).toLongLong(), blockstring, "", bytestring, selectednode->Data(2).toLongLong(), 0);
                ui->hexview->setCursorPosition(bytestring.toLongLong()*2);
	    }
        }
    }
    //else
    //    qDebug() << "count for nodeid:" << nodeid.split("-").count();
    ui->hexview->ensureVisible();*/
}

void WombatForensics::CloseCurrentCase()
{
    if(predigwatcher.isRunning())
    {
        predigwatcher.cancel();
        predigfuture.cancel();
        QThreadPool::globalInstance()->clear();
        isclosing = true;
    }
    if(digwatcher.isRunning())
    {
        //qDebug() << "digwatcher is running....";
        digwatcher.cancel();
        digfuture.cancel();
        QThreadPool::globalInstance()->clear();
        isclosing = true;
        //qDebug() << "digwatcher is cancelled...";
    }
    else
    {
        //qDebug() << "dig watcher wasn't running although it was..";
    }
    if(pathtreeview->model() != NULL)
    {
        UpdateSelectedState(selectedindex.sibling(selectedindex.row(), treenodemodel->GetColumnIndex("id")).data().toString());
        qInfo() << "Selected Item Saved";
        UpdateCheckState();
        qInfo() << "Check State Saved";
        SavePasswordList();
        qInfo() << "Password Items Saved";
        SaveImagesHash();
        qInfo() << "Thumbnailed Videos and Images Saved";
        SaveTreeModel();
        qInfo() << "Tree Model Saved";
        pathtreeview->clearSelection();
        delete treenodemodel;
        autosavetimer->stop();
    }
    if(ui->hexview->data().size() > 0)
    {
        // maybe i want to run this but also load a small empty image maybe...
        ui->hexview->Reset();
    /*
        casedatafile.setFileName(QDir::tempPath() + "/zfile");
        casedatafile.open(QIODevice::WriteOnly | QIODevice::Text);
        casedatafile.write("dummy zerofile");
        casedatafile.close();
        ui->hexview->BypassColor(true);
	//qInfo() << "start of error?";
        ui->hexview->setData(casedatafile);
	//qInfo() << "end of error?";
    */
    }
    qInfo() << "Hexviewer Reset";
    setWindowTitle("WombatForensics");
    filesfound = 0;
    fileschecked = 0;
    carvedcount = 0;
    filtercountlabel->setText("Filtered: 0");
    filecountlabel->setText("Parsed: " + QString::number(filesfound));
    checkedcountlabel->setText("Checked: " + QString::number(fileschecked));

    // UNMOUNT ALL FUSE MOUNTED IMAGES
    /*
    for(int i=0; i < fuserlist.size(); i++)
    {
        struct fuse_session* curfuser = (struct fuse_session*)(fuserlist.at(i));
        QString imgext = existingevidence.at(i).split("/").last().split(".").last().toLower();
        if(imgext.endsWith("e01") || imgext.endsWith("aff") || imgext.endsWith("000") || imgext.endsWith("zmg")) // ewfmount
        {
            if(curfuser != NULL)
            {
                fuse_session_unmount(curfuser);
                /*
                if(i == fuserlist.size() - 1)
                {
                    //fuse_destroy(curfuser);
                }
                */
            /*}
        }
    }
    if(fuserlist.size() > 0)
    {
        // REMOVE EWFMOUNT DIRECTORIES RECURSIVELY HERE...
        // might need to get an entrylist for the directory and then call remove() for each
        // directory or rmdir() for each directory...
        //QDir edir = QDir(wombatvariable.tmpmntpath + evidfiles.first());
        //edir.removeRecursively();
    }
    */
    // UNMOUNT EVIDENCEIMAGEDATAFILE
    // NEED TO CHANGE THIS LOOP FROM EXISTINGEVIDENCE.COUNT() TO FUSELIST.COUNT()
    /*
    for(int i=0; i < evidimglist.size(); i++)
    {
        delete evidimglist.at(i);
    }
    */
    /*
    for(int i=0; i < existingevidence.count(); i++)
    {
        QStringList args;
        args << "-u" << wombatvariable.imgdatapath + existingevidence.at(i).split("/").last();
        //qDebug() << "existing evidence:" << existingevidence.at(i);
        QString imgext = existingevidence.at(i).split("/").last().split(".").last().toLower();
        //qDebug() << "imgext:" << imgext;
        if(imgext.contains("e01")) // ewfmount
        {
            /*
            QProcess::execute("fusermount", args);
            QDir dir(wombatvariable.imgdatapath + existingevidence.at(i).split("/").last());
            if(dir.exists())
                dir.rmdir(dir.absolutePath());
            */
            /*
            if(ewfuser != NULL)
            {
                fuse_unmount(ewfuser);
                fuse_destroy(ewfuser);
            }
            else
                qDebug() << "ewfuser was null";
            */
       /* }
        else if(imgext.contains("aff") || imgext.contains("000") || imgext.contains("001")) // affuse
        {
            QProcess::execute("fusermount", args);
            //QDir::rmdir(QString(wombatvariable.imgdatapath + existingevidence.at(i).split("/").last()));
            QDir dir(wombatvariable.imgdatapath + existingevidence.at(i).split("/").last());
            if(dir.exists())
                dir.rmdir(dir.absolutePath());
            /*
            if(affuser != NULL)
            {
                fuse_unmount(affuser);
                fuse_destroy(affuser);
            }
            else
                qDebug() << "affuser was null";
            */
        /*}
        else if(imgext.contains("zmg")) // zmgfuse
        {
            QProcess::execute("fusermount", args);
            //QDir::rmdir(QString(wombatvariable.imgdatapath + existingevidence.at(i).split("/").last()));
            QDir dir(wombatvariable.imgdatapath + existingevidence.at(i).split("/").last());
            if(dir.exists())
                dir.rmdir(dir.absolutePath());
            /*
            if(zmgfuser != NULL)
            {
                fuse_unmount(zmgfuser);
                fuse_destroy(zmgfuser);
            }
            */
        /*}
        else if(imgext.contains("sfs")) // squashfuse
        {
            /*
            if(sqfuser != NULL)
            {
                fuse_unmount(sqfuser);
                fuse_destroy(sqfuser);
            }
            */
        /*}
        /*
        else // raw, so nothing to unmount
        {
        }
        */
    /*}
    qInfo() << "Forensic Image(s) unmounted";
    */
    carvecounthash.clear();
    partitionlist.clear();
    //existingevidence.clear();
    newforimglist.clear();
    existingforimglist.clear();
    //newevidence.clear();
    //newevid.clear();
    //existingevid.clear();
    // BEGIN TAR METHOD
    QString tmptar = casepath + "/" + wombatvariable.casename + ".wfc";
    QString oldtmptar = tmptar + ".old";
    if(FileExists(tmptar.toStdString()))
    {
        rename(tmptar.toStdString().c_str(), oldtmptar.toStdString().c_str());
    }
    QByteArray tmparray = tmptar.toLocal8Bit();
    QByteArray tmparray2 = wombatvariable.tmpmntpath.toLocal8Bit();
    QByteArray tmparray3 = QString("./" + wombatvariable.casename).toLocal8Bit();
    TAR* casehandle;
    tar_open(&casehandle, tmparray.data(), NULL, O_WRONLY | O_CREAT, 0644, TAR_GNU);
    tar_append_tree(casehandle, tmparray2.data(), tmparray3.data());
    tar_close(casehandle);
    std::remove(oldtmptar.toStdString().c_str());
    // END TAR METHOD
    StatusUpdate("Saved...");
    qInfo() << "Wombat Case File Saved";
    statuslabel->repaint();
    statuslabel->adjustSize();
    RemoveTmpFiles();
    wombatvariable.iscaseopen = false;
    if(logfile.isOpen())
        logfile.close();
    // remove existing case directory
    QDir cdir = QDir(wombatvariable.tmpmntpath);
    cdir.removeRecursively();
    QString homepath = QDir::homePath();
    homepath += "/.local/share/wombatforensics/";
    wombatvariable.tmpmntpath = homepath + "mntpt/";
}

void WombatForensics::RemEvidence()
{
    //qDebug() << "remove evidence clicked";
    remevidencedialog = new RemEvidenceDialog(this);
    connect(remevidencedialog, SIGNAL(RemEvid(QStringList)), this, SLOT(RemoveEvidence(QStringList)));
    remevidencedialog->exec();
}

void WombatForensics::VerifyUpdate(qint64 cursize)
{
    /*
    QProgressBar* curbar = verevidencedialog->findChild<QProgressBar*>();
    qDebug() << "curbar objectname:" << curbar->objectName();
    qDebug() << "cursize:" << cursize;
    //QPushButton *button = parentWidget->findChild<QPushButton *>("button1");
    */
}

void WombatForensics::FinishVerify()
{
    /*
    QString resultstring = "";
    for(int i=0; i < verfuture.resultCount(); i++)
    {
        resultstring += QString::fromStdString(verfuture.resultAt(i)) + "\n";
    }
    QMessageBox::information(this, "Finished", " " + resultstring, QMessageBox::Ok);
    */
}

void WombatForensics::VerProgChange(int progval)
{
    qDebug() << "prog val:" << progval;
    /*
    qDebug() << "prog value:" << progval;
    QProgressBar* curbar = verevidencedialog->findChild<QProgressBar*>();
    qDebug() << "curbar objectname:" << curbar->objectName();
    */
}

void WombatForensics::VerifyEvidence(QStringList verevidlist)
{
    // I should move this code to the verevidencedialog and cut out going back to wombatforensics at all...
    /*
    connect(&verifywatcher, SIGNAL(finished()), this, SLOT(FinishVerify()), Qt::QueuedConnection);
    // the below progressValueChanged is from 0 to 1, i.e. the number of items sent to the mapped command.
    // so this is worthless, I need to get the verify information... which means i need an isignal...
    //connect(&verifywatcher, SIGNAL(progressValueChanged(int)), this, SLOT(VerProgChange(int)));
    verfuture = QtConcurrent::mapped(verevidlist, Verify);
    verifywatcher.setFuture(verfuture);
    */
}

void WombatForensics::RemoveEvidence(QStringList remevidlist)
{
    // MAY NEED TO ADD AN UPDATE TO TAG AND HASH FOR THE EVIDENCE REMOVAL
    QDir eviddir = QDir(wombatvariable.tmpmntpath);
    for(int i=0; i < remevidlist.count(); i++)
    {
        QStringList evidfiles = eviddir.entryList(QStringList(remevidlist.at(i).split("/").last() + "*"), QDir::NoSymLinks | QDir::Dirs);
        if(!evidfiles.isEmpty())
        {
            // 1. Delete all thumbnails.
            QDir tdir = QDir(wombatvariable.tmpmntpath + "thumbs/");
            QStringList tfiles = tdir.entryList(QStringList("e" + evidfiles.first().split(".e").last() + "-*"), QDir::NoSymLinks | QDir::Files);
            if(!tfiles.isEmpty())
            {
                for(int j = 0; j < tfiles.count(); j++)
                    tdir.remove(tfiles.at(j));
            }
            // 1.5 Delete all carved files
            QDir cdir = QDir(wombatvariable.tmpmntpath + "carved/");
            QStringList cfiles = cdir.entryList(QStringList("e" + evidfiles.first().split(".e").last() + "-*"), QDir::NoSymLinks | QDir::Files);
            if(!cfiles.isEmpty())
            {
                for(int j = 0; j < cfiles.count(); j++)
                    cdir.remove(cfiles.at(j));
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
            // 2.5 Remove e# entries from hashlist and taggedhash
            /*
            QHashIterator<QString, QString> n(hashlist);
            while(n.hasNext())
            {
                n.next();
                if(n.key().contains(QString("e" + evidfiles.first().split(".e").last())))
                    hashlist.remove(n.key());
            }
            SaveHashList();
            QHashIterator<QString, QString> o(taggedhash);
            while(o.hasNext())
            {
                o.next();
                if(o.key().contains(QString("e" + evidfiles.first().split(".e").last())))
                    taggedhash.remove(o.key());
            }
            SaveTaggedList();
            */
            QHashIterator<QString, QString> p(passwordhash);
            while(p.hasNext())
            {
                p.next();
                if(p.key().contains(QString(evidfiles.first().split(".e").first())))
                    passwordhash.remove(p.key());
            }
            SavePasswordList();
            // 3. Delete evid directory.
            QDir edir = QDir(wombatvariable.tmpmntpath + evidfiles.first());
            edir.removeRecursively();
            // 4.5 Delete from existingevidence.
            //existingevidence.removeOne(remevidlist.at(i));
	    // 4. Delete from existingforimglist.
	    for(int j=0; j < existingforimglist.count(); j++)
	    {
		if(existingforimglist.at(j)->ImgPath().contains(remevidlist.at(i)))
		    existingforimglist.removeAt(j);
	    }
            // 5. Remove TreeNode.
            QModelIndexList indexlist = treenodemodel->match(treenodemodel->index(0, treenodemodel->GetColumnIndex("id"), QModelIndex()), Qt::DisplayRole, QVariant("e" + evidfiles.first().split(".e").last()), 1, Qt::MatchFlags(Qt::MatchExactly | Qt::MatchRecursive));
            if(!indexlist.isEmpty())
            {
                treenodemodel->removeRow(indexlist.first().row(), indexlist.first());
            }
            //6. Remove ELink and EItem from Preview Report
            RemoveELinkItem(remevidlist.at(i).split("/").last());
            RemoveEvidItem(remevidlist.at(i).split("/").last());
        }
    }
    StatusUpdate("Evidence Item Successfully Removed");
}

QStringList WombatForensics::GetFileLists(int filelisttype)
{
    // I MAY NOT NEED 4,5,6 IF I DO THE CHECK ON THE LIST PRIOR TO CALCULATION OTHERWISE I WILL HAVE TO REDUCE THE SELECTED OR CHECKED OR ALL LIST HERE...
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
    else if(filelisttype == 3) // Generate list for Publish Report
    {
        QModelIndexList indexlist = treenodemodel->match(treenodemodel->index(0, treenodemodel->GetColumnIndex("tag"), QModelIndex()), Qt::DisplayRole, QVariant(tr("*[A-Za-z0-9]*")), -1, Qt::MatchFlags(Qt::MatchRecursive | Qt::MatchWildcard));
        foreach(QModelIndex index, indexlist)
            tmplist.append(QString(index.sibling(index.row(), treenodemodel->GetColumnIndex("id")).data().toString()));
        return tmplist;
    }
    else if(filelisttype == 4) // Generate list for Image category
    {
	QModelIndexList indexlist = treenodemodel->match(treenodemodel->index(0, treenodemodel->GetColumnIndex("cat"), QModelIndex()), Qt::DisplayRole, QVariant(tr("Image")), -1, Qt::MatchFlags(Qt::MatchRecursive | Qt::MatchExactly));
	foreach(QModelIndex index, indexlist)
	    tmplist.append(QString(index.sibling(index.row(), treenodemodel->GetColumnIndex("id")).data().toString()));
	return tmplist;
    }
    else if(filelisttype == 5) // Generate list for Video category
    {
	QModelIndexList indexlist = treenodemodel->match(treenodemodel->index(0, treenodemodel->GetColumnIndex("cat"), QModelIndex()), Qt::DisplayRole, QVariant(tr("Video")), -1, Qt::MatchFlags(Qt::MatchRecursive | Qt::MatchExactly));
	foreach(QModelIndex index, indexlist)
	    tmplist.append(QString(index.sibling(index.row(), treenodemodel->GetColumnIndex("id")).data().toString()));
	return tmplist;
    }
    else if(filelisttype == 6) // Generate list for Archive category
    {
        QModelIndexList indexlist = treenodemodel->match(treenodemodel->index(0, treenodemodel->GetColumnIndex("cat"), QModelIndex()), Qt::DisplayRole, QVariant(("Archive")), -1, Qt::MatchFlags(Qt::MatchRecursive | Qt::MatchExactly));
        //qDebug() << "archives found: indexlist count:" << indexlist.count();
        foreach(QModelIndex index, indexlist)
            tmplist.append(QString(index.sibling(index.row(), treenodemodel->GetColumnIndex("id")).data().toString()));
        //qDebug() << "archive tmplist value:" << tmplist.count();
        return tmplist;
    }
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
        exportlist.append(itemnode->Data("id").toString());
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
}

void WombatForensics::StartCarving(QStringList plist, QStringList flist)
{
    //carvedtypeslist.clear();
    //carvedtypeslist = flist;
    //qDebug() << "global carved types list test:" << carvedtypeslist;
    qInfo() << "Carving Started...";
    StatusUpdate("Carving Started...");
    carvestatuslabel->setText("Initiating Header(s) Search...");
    QFuture<void> tmpfuture = QtConcurrent::run(GenerateCarving, plist, flist);
    carvewatcher.setFuture(tmpfuture);
}

void WombatForensics::GetHashComparisons(QStringList whlfiles)
{
    whlcomparisonlist.append(whlfiles);
}

void WombatForensics::DigFiles(int dtype, QVector<int> doptions)
{
    // digtype = selected (0) | checked (1) | all (2)
    // digoptions = thumbimg (0) | thumbvid (1) | hash (2) | hashlistcompare (3) | expandarchive zip (4)
    // provides access to the whlcomparisonlist to give access to the files...
    //qDebug() << "whlfiles:" << whlcomparisonlist;
    /*
    digimgthumbtotal = 0;
    digvidthumbtotal = 0;
    dighashtotal = 0;
    */
    digtotalcount = 0;
    /*
    digimgthumbcount = 0;
    digvidthumbcount = 0;
    digarchivecount = 0;
    dighashcount = 0;
    digimgcountstring = "";
    digvidcountstring = "";
    dighashcountstring = "";
    */
    digtotalcountstring = "";

    digtype = dtype;
    digoptions = doptions;
    digfilelist.clear();
    qInfo() << "Digging Deeper into Evidence";
    StatusUpdate("Digging Deeper...");
    //LogMessage("Digging Deeper into Evidence");

    if(dtype == 0) // selected
    {
        TreeNode* itemnode = static_cast<TreeNode*>(selectedindex.internalPointer());
        digfilelist.append(itemnode->Data("id").toString());
    }
    else
        digfilelist = GetFileLists(dtype);
    digtotalcount = digfilelist.count();

    //qDebug() << "digfilelist:" << digfilelist;
    /*
    // THIS LOOP MIGHT BE JUST TO GET THE COUNTS FOR THE STATUS, NOT NECESSARILY THE FILEIDLISTS FOR RESPECTIVE DIGGING
    // GET COUNTS FOR EACH DIGGING CHOICE
    for(int i = 0; i < digoptions.count(); i++)
    {
        if(digoptions.at(i) == 0) // Generate Image Thumbnails
        {
            if(dtype == 1) // checked so get all images in a tmp list and then reduce the checked list to just checked images
            {
                //qDebug() << "all checked files:" << digfilelist;
                QStringList tmpimglist = GetFileLists(4);
                for(int j=0; j < digfilelist.count(); j++)
                {
                    if(!tmpimglist.contains(digfilelist.at(j)))
                        digimgthumbtotal++;
                        //digfilelist.removeAt(j);
                }
                //qDebug() << "checked images only:" << digfilelist;
            }
            else if(dtype == 2) // all files, replace all files with just images.
                digimgthumbtotal = GetFileLists(4).count();
        }
        else if(digoptions.at(i) == 1) // Generate Video Thumbnails
        {
            if(dtype == 1) // checked, so get all videos in a tmplist and then reduce the checked list to just checked videos
            {
                //qDebug() << "all checked files:" << digfilelist;
                QStringList tmpvidlist = GetFileLists(5);
                for(int j=0; j < digfilelist.count(); j++)
                {
                    if(!tmpvidlist.contains(digfilelist.at(j)))
                        digvidthumbtotal++;
                        //digfilelist.removeAt(j);
                }
                //qDebug() << "checked videos only:" << digfilelist;
            }
            else if(dtype == 2) // all files, replace all files with just videos
                digvidthumbtotal = GetFileLists(5).count();
        }
        else if(digoptions.at(i) == 2) // Generate BlAKE3 Hash
        {
            dighashtotal = digfilelist.count();
        }
        else if(digoptions.at(i) == 3) // Compare with Selected HashLists
        {
            digmatchtotal = digfilelist.count();
            // global variable is whlcomparisonlist
        }
        else if(digoptions.at(i) == 4) // Expand Zip Archives
        {
        }
        /*
        // I MAY NOT NEED 4,5,6 IF I DO THE CHECK ON THE LIST PRIOR TO CALCULATION OTHERWISE I WILL HAVE TO REDUCE THE SELECTED OR CHECKED OR ALL LIST HERE...
        if(digoptions.at(i) == 0 || digoptions.at(i) == 4 || digoptions.at(i) == 5) // Generate Image Thumbnails || video || both
        {
            genthmbpath = wombatvariable.tmpmntpath;
            if(digoptions.at(i) == 0 || digoptions.at(i) == 5)
            {
                isimgthumb = true;
                if(dtype == 2) // all files, only get images...
                {
                    digimgthumbtotal = GetFileLists(4).count(); // images only
                }
                else if(dtype == 1 || dtype == 3) // checked files, only get images...
                {
                    QStringList allimglist = GetFileLists(4); // all images
                    for(int i=0; i < digfilelist.count(); i++)
                    {
                        if(allimglist.contains(digfilelist.at(i)))
                            digimgthumbtotal++;
                    }
                }
                else if(dtype == 0) // selected file, only get image
                {
                    QStringList allimglist = GetFileLists(4); // all images
                    if(allimglist.contains(digfilelist.first()))
                        digimgthumbtotal = 1;
                }
		digimgcountstring = "Thumbnailed: 0 of " + QString::number(digimgthumbtotal) + " Images";
                if(digimgthumbtotal > 0)
                {
                    hasimg = true;
                }
            }
            if(digoptions.at(i) == 4 || digoptions.at(i) == 5)
            {
                isvidthumb = true;
                if(dtype == 2) // all files, only get videos
                {
                    digvidthumbtotal = GetFileLists(5).count(); // videos only
                }
                else if(dtype == 1 || dtype == 3)
                {
                    QStringList allvidlist = GetFileLists(5); // all videos
                    for(int i=0; i < digfilelist.count(); i++)
                    {
                        if(allvidlist.contains(digfilelist.at(i)))
                            digvidthumbtotal++;
                    }
                }
                else if(dtype == 0) // selected file, only get video
                {
                    QStringList allvidlist = GetFileLists(5); // all videos
                    if(allvidlist.contains(digfilelist.first()))
                        digvidthumbtotal = 1;
                }
		digvidcountstring = "Thumbnailed: 0 of " + QString::number(digvidthumbtotal) + " Videos";
                if(digvidthumbtotal > 0)
                {
                    hasvid = true;
                }
            }
        }
        else if(digoptions.at(i) == 1 || digoptions.at(i) == 2 || digoptions.at(i) == 3 || digoptions.at(i) == 7) // 1 - MD5 || 2 - SHA1 || 3 - SHA256 || 7 - BLAKE3
        {
            if(digoptions.at(i) == 2)
                hashsum = 2;
            else if(digoptions.at(i) == 3)
                hashsum = 4;
            else if(digoptions.at(i) == 1)
                hashsum = 1;
            else if(digoptions.at(i) == 7)
                hashsum = 11;
            dighashtotal = digfilelist.count();
	    dighashcountstring = "Hashed: 0 of " + QString::number(dighashtotal);
            if(dighashtotal > 0)
            {
                hashash = true;
            }
        }
        else if(digoptions.at(i) == 6) // 6- EXPAND ARCHIVES
        {
            if(dtype == 2) // all files, only get images...
            {
                digarchivetotal = GetFileLists(6).count(); // archives only
                qDebug() << "digarchivetotal:" << digarchivetotal;
            }
            else if(dtype == 1 || dtype == 3) // checked files, only get images...
            {
                QStringList archiveslist = GetFileLists(6); // all archives
                for(int i=0; i < digfilelist.count(); i++)
                {
                    if(archiveslist.contains(digfilelist.at(i)))
                        digarchivetotal++;
                }
            }
            else if(dtype == 0) // selected file, only get image
            {
                QStringList archiveslist = GetFileLists(6); // all archives
                if(archiveslist.contains(digfilelist.first()))
                    digarchivetotal = 1;
            }
            digarchivecountstring = "Expanded: 0 of " + QString::number(digarchivetotal) + " Archives";
            if(digarchivetotal > 0)
                hasarchive = true;
        }
        */
    /*}
    digtotalcount = digimgthumbtotal + digvidthumbtotal + dighashtotal + digarchivetotal;
    digtotalcountstring = "Dug: 0 of " + digtotalcount;
    */
    //qDebug() << "digoptions:" << digoptions;
    for(int i = 0; i < digoptions.count(); i++)
    {
        if(digoptions.at(i) == 0)
            hasimg = true;
        else if(digoptions.at(i) == 1)
            hasvid = true;
        else if(digoptions.at(i) == 2)
            hashash = true;
        else if(digoptions.at(i) == 3)
        {
            // hash list comparison here...
        }
        else if(digoptions.at(i) == 4)
            hasarchive = true;
    }
    //qDebug() << "hasvid:" << hasvid;
    //genthmbpath = wombatvariable.tmpmntpath;

    digtotalcountstring = "Dug: 0 of " + digtotalcount;

    // LAUNCH GENERATEPREDIGGING() MAP HERE...
    // IMPLEMENT PREDIGWATCHER
    // ON PREDIGWATCHER RETURN, THEN CALL THE BELOW CODE...
    // THE TIMER CAN START WITH PREDIG AND CONTINUE UNTIL DIGGING ENDS...
    // ENSURE ALL COUNTERS WHICH NEED TO GET UPDATED FOR EXPANDED FILES GET UPDATED ACCORDINGLY...
    predigfuture = QtConcurrent::map(digfilelist, GeneratePreDigging);
    predigwatcher.setFuture(predigfuture);
    //digfuture = QtConcurrent::map(digfilelist, GenerateDigging);
    //digwatcher.setFuture(digfuture);
    //digrotatetimer->start(1500);
}

/*
void WombatForensics::UpdateCarve(QString pid, int carvecnt)
{
    carvecounthash.insert(pid, carvecnt);
    // DO I WANT TO PUSH EVERY CARVED ITEM TO AN INFO ???? NOT RIGHT NOW.
    //qDebug() << "carvecounthash updated:" << carvecounthash;
    //SleepLoop(1500);
    carvestatuslabel->setText("Partition: " + pid + " Carved: " + QString::number(carvecnt));
    // have values for QHash<QString partitionid, int carvecount>carvecounthash;
}
*/

void WombatForensics::UpdateDig(int digid, int digcnt)
{
    /*
    if(digid == 0)
    {
	digimgcountstring = "Thumbnailed: " + QString::number(digcnt) + " of " + QString::number(digimgthumbtotal) + " Images";
    }
    else if(digid == 1 || digid == 2 || digid == 3)
    {
        dighashcountstring = "Hashed: " + QString::number(digcnt) + " of " + QString::number(dighashtotal); 
    }
    else if(digid == 4)
    {
	digvidcountstring = "Thumbnailed: " + QString::number(digcnt) + " of " + QString::number(digvidthumbtotal) + " Videos";
    }
    else if(digid == 5)
    {
        digarchivecountstring = "Expanded: " + QString::number(digcnt) + " of " + QString::number(digarchivetotal) + " Archives";
    }
    digtotalcountstring = "Dug: " + QString::number(digvidthumbcount + digimgthumbcount + dighashcount + digarchivecount) + " of " + QString::number(digtotalcount);
    */
    digtotalcountstring = "Dug: " + QString::number(digcnt) + " of " + QString::number(digtotalcount);
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
    filecountlabel->setText("Parsed: " + QString::number(filecount));
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
    {
        pathtreeview->resizeColumnToContents(i);
    }
}

void WombatForensics::SetupHexPage(void)
{
    setBackgroundRole(QPalette::Base);
    setAutoFillBackground(true);
    //QByteArray zarray;
    //ui->hexview->setData(zarray);
    ui->hexview->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->hexview, SIGNAL(currentAddressChanged(qint64)), this, SLOT(SetOffsetLabel(qint64)));
    connect(ui->hexview, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(ImgHexMenu(const QPoint &)));
    connect(ui->hexview, SIGNAL(selectionChanged()), this, SLOT(HexSelectionChanged()));
    connect(ui->hexview, SIGNAL(selectionChanged()), this, SLOT(UpdateSelectValue()));
}

WombatForensics::~WombatForensics()
{
    delete viewmanage;
    //delete hashlistmanager;
    delete bookmarkmenu;
    delete tagcheckedmenu;
    delete hashlistmenu;
    delete hashcheckedmenu;
    delete selectionmenu;
    delete msgviewer;
    delete verevidencedialog;
    delete idfilterview;
    delete jumpfilterview;
    delete namefilterview;
    delete pathfilterview;
    delete sizefilterview;
    delete createfilterview;
    delete accessfilterview;
    delete modifyfilterview;
    delete changefilterview;
    delete filetypefilterview;
    delete filecategoryfilterview;
    delete hashfilterview;
    delete tagfilterview;
    delete hashmatchfilterview;
    delete byteviewer;
    delete previewreport;
    delete imagewindow;
    delete aboutbox;
    delete settingsdialog;
    delete isignals;
    delete treemenu;
    delete ui;
}

void WombatForensics::mouseDoubleClickEvent(QMouseEvent* event)
{
    if(event)
    {
    }
}

/*
void WombatForensics::mousePressEvent(QMouseEvent* event)
{
    if(event)
    {
        //Qt::QMouseButton button = event->button();
        qDebug() << "button event:" << event->button();
    }
}
*/

void WombatForensics::closeEvent(QCloseEvent* event)
{
    StatusUpdate("Exiting...");
    statuslabel->repaint();
    statuslabel->adjustSize();
    if(wombatvariable.iscaseopen)
    {
        QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
        StatusUpdate("Saving...");
        statuslabel->adjustSize();
        statuslabel->repaint();
        CloseCurrentCase();
        StatusUpdate("Exiting...");
        statuslabel->repaint();
        statuslabel->adjustSize();
    }
    else
    {
        delete treenodemodel;
    }
    
    imagewindow->close();
    viewmanage->close();
    //hashlistmanager->close();
    byteviewer->close();
    previewreport->close();
    aboutbox->close();
    settingsdialog->close();
    /*
    if(forimgdialog->isVisible())
        forimgdialog->close();
    if(verevidencedialog->isVisible())
        verevidencedialog->close();
    if(regviewer->isVisible())
        regviewer->close();
    */
    RemoveTmpFiles();
    event->accept();
    //msglog->clear();
    msgviewer->close();
    verevidencedialog->close();
    //delete ui;
    //delete ui->mainStatusBar;
}

void WombatForensics::RemoveTmpFiles()
{
    QDir tmpdir(wombatvariable.tmpfilepath);
    if(!tmpdir.removeRecursively())
    {
        DisplayError("2.7", "Tmp File Removal", "All tmp files may not have been removed. Please manually remove them.");
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
            treenodemodel = new TreeNodeModel();
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
            treenodemodel = new TreeNodeModel();
            InitializeOpenCase();
        }
    }
    else
        InitializeOpenCase();
}

void WombatForensics::on_actionSaveState_triggered()
{
    SaveState();
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

void WombatForensics::on_actionCarve_triggered()
{
    filecarvedialog = new FileCarvingDialog(this);
    filecarvedialog->PopulatePartitions(partitionlist);
    filecarvedialog->PopulateFileTypes();
    connect(filecarvedialog, SIGNAL(StartCarve(QStringList, QStringList)), this, SLOT(StartCarving(QStringList, QStringList)), Qt::DirectConnection);
    filecarvedialog->show();
}
void WombatForensics::on_actionDigDeeper_triggered()
{
    totalcount = filesfound;
    totalchecked = fileschecked;
    dighashcount = 0;
    digimgthumbcount = 0;
    digvidthumbcount = 0;
    digdeeperdialog = new DigDeeperDialog(this, totalchecked, totalcount);
    connect(digdeeperdialog, SIGNAL(StartDig(int, QVector<int>)), this, SLOT(DigFiles(int, QVector<int>)), Qt::DirectConnection);
    connect(digdeeperdialog, SIGNAL(HashComparison(QStringList)), this, SLOT(GetHashComparisons(QStringList)), Qt::DirectConnection);
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
    propertywindow->setWindowTitle(selectedindex.sibling(selectedindex.row(), treenodemodel->GetColumnIndex("id")).data().toString() + " Properties");
    propertywindow->setAttribute(Qt::WA_DeleteOnClose);
    propertywindow->setModal(false);
    UpdateProperties();
}

void WombatForensics::on_actionView_File_triggered(bool checked)
{
    if(checked){}
    fileviewer = new FileViewer();
    fileviewer->setWindowIcon(QIcon(":/ehex"));
    fileviewer->setWindowTitle(selectedindex.sibling(selectedindex.row(), treenodemodel->GetColumnIndex("id")).data().toString() + " Hex");
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
            // NEED TO FIX THIS POPUP ONCE I'VE GOT DIGGING FLESHED OUT BETTER AGAIN!
            // POSSIBLY JUST MAKE THIS AN INDICATOR WHICH OPENS THE DIG DEEPER DIALOG
            qDebug() << "fix what to do here in terms of how to generate the thumbnails";
            /*
            int ret = QMessageBox::question(this, tr("Generate Thumbnails"), tr("Thumbnails have not been generated. Do you want to generate all IMG & VID thumbnails now?\r\n\r\nNote: This can take a while and will show the Thumbnail Gallery window when complete."), QMessageBox::Yes | QMessageBox::No);
            if(ret == QMessageBox::Yes)
            {
                QVector<int> tmplist;
                tmplist.append(5);
                DigFiles(2, tmplist);
            }
            else
                ui->actionView_Image_Gallery->setChecked(false);
            */
        }
        else
        {
            QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
            imagewindow->LoadThumbnails();
            QApplication::restoreOverrideCursor();
            imagewindow->show();
        }
    }
}

void WombatForensics::FinishPreDigging()
{
    //if(digtype == 0) // selected
    //else if(digtype == 1) // checked
    if(hasarchive && !isclosing && digtype == 2) // all items (now including expanded archives.)
        digfilelist = GetFileLists(2);
    qDebug() << "digfilelist:" << digfilelist;
    digfuture = QtConcurrent::map(digfilelist, GenerateDigging);
    digwatcher.setFuture(digfuture);
}

void WombatForensics::FinishCarve()
{
    emit treenodemodel->layoutChanged(); // this resolves the issues with the add evidence not updating when you add it later
    qInfo() << "Carving Completed";
    StatusUpdate("Carving Completed");
}

void WombatForensics::FinishDigging()
{
    if(hashash)
    {
        /*
        if(hashsum == 1)
            treenodemodel->UpdateHeaderNode(7, "MD5 Hash");
        else if(hashsum == 2)
            treenodemodel->UpdateHeaderNode(7, "SHA1 Hash");
        else if(hashsum == 4)
            treenodemodel->UpdateHeaderNode(7, "SHA256 Hash");
        else if(hashsum == 11)
            treenodemodel->UpdateHeaderNode(7, "BLAKE3 Hash");
        */
        //QtConcurrent::run(SaveHashList); // save ids/hashed values to hashlist file for re-opening a case.
        hashash = false;
    }
    if(hasimg || hasvid)
    {
        QFuture<void> tmpfuture = QtConcurrent::run(SaveImagesHash);
        thashsavewatcher.setFuture(tmpfuture);
        if(!isreport)
        {
            QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
            imagewindow->LoadThumbnails(); // GUI Intensive
            QApplication::restoreOverrideCursor();
            imagewindow->show();
        }
        hasimg = false;
        hasvid = false;
    }
    if(isreport)
    {
        QStringList curidlist = GetFileLists(3);
        QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
        foreach(QString id, curidlist)
        {
            TransferThumbnails(id, currentreportpath);
            TransferFiles(id, currentreportpath);
        }
        TransferArtifacts("registry", currentreportpath);
        QApplication::restoreOverrideCursor();
        isreport = false;
    }
    qInfo() << "Digging Finished";
    //digrotatetimer->stop();
    //digcountlabel->setText(digtotalcountstring);
    emit treenodemodel->layoutChanged(); // this resolves the issues with the add evidence not updating when you add it later
    StatusUpdate("Ready");
}

void WombatForensics::on_actionViewerManager_triggered()
{
    viewmanage->show();
}

void WombatForensics::on_actionHashListManager_triggered()
{
    hashlistmanager = new HashListManager(this);
    hashlistmanager->setWindowIcon(QIcon(":/bar/hashlist"));
    connect(hashlistmanager, SIGNAL(HideHashListManager()), this, SLOT(HideHashListManager()), Qt::DirectConnection);
    connect(hashlistmanager, SIGNAL(ReadHashLists()), this, SLOT(ReadHashLists()), Qt::DirectConnection);
    hashlistmanager->show();
}

void WombatForensics::on_actionBookmark_Manager_triggered()
{
    tagmanage = new TagManager(this);
    tagmanage->setWindowIcon(QIcon(":/bar/managetags"));
    connect(tagmanage, SIGNAL(HideManagerWindow()), this, SLOT(HideTagManager()), Qt::DirectConnection);
    connect(tagmanage, SIGNAL(ReadBookmarks()), this, SLOT(ReadBookmarks()), Qt::DirectConnection);
    tagmanage->show();
}

void WombatForensics::on_actionTextViewer_triggered(bool checked)
{
    if(checked){}
    if(selectedindex.sibling(selectedindex.row(), treenodemodel->GetColumnIndex("cat")).data().toString().contains("text/"))
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

void WombatForensics::on_actionpreviewreport_triggered(bool checked)
{
    if(!checked) // hide viewer
        previewreport->hide();
    else
    {
        previewreport->LoadHtml(QString(wombatvariable.tmpmntpath + "previewreport.html"));
        previewreport->show();
    }
}

void WombatForensics::on_actionExpandAll_triggered()
{
    // this can take some time, must alert the user.
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
    //ui->dirTreeView->expandAll();
    QApplication::restoreOverrideCursor();
}

void WombatForensics::on_actionCollapseAll_triggered()
{
    // this can take some time, must alert the user.
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
    //ui->dirTreeView->collapseAll();
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
    QString tmptext = "Length: " + QString::number(selectionbytes.size());
    QString bytetext = "";
    selectedhex->setText(tmptext);
    bytetext += "<table border=0 width='100%' cellpadding=5>";
    bytetext += "<tr><th>Format</th><th>Little Endian</th><th>Big Endian</th></tr>";
    bytetext += "<tr><td>8-bit Signed Integer:</td><td align=right>" + QString::number(qFromLittleEndian<int8_t>(selectionbytes.mid(0, 1))) + "</td><td align=right>" + QString::number(qFromBigEndian<int8_t>(selectionbytes.mid(0, 1))) + "</td></tr>";
    bytetext += "<tr><td>8-bit Unsigned Integer:</td><td align=right>" + QString::number(qFromLittleEndian<uint8_t>(selectionbytes.mid(0, 1))) + "</td><td align=right>" + QString::number(qFromBigEndian<uint8_t>(selectionbytes.mid(0, 1))) + "</td></tr>";
    bytetext += "<tr><td>16-bit Signed Integer:</td><td align=right>" + QString::number(qFromLittleEndian<int16_t>(selectionbytes.mid(0, 2))) + "</td><td align=right>" + QString::number(qFromBigEndian<int16_t>(selectionbytes.mid(0, 2))) + "</td></tr>";
    bytetext += "<tr><td>16-bit Unsigned Integer:</td><td align=right>" + QString::number(qFromLittleEndian<uint16_t>(selectionbytes.mid(0, 2))) + "</td><td align=right>" + QString::number(qFromBigEndian<uint16_t>(selectionbytes.mid(0, 2))) + "</td></tr>";
    bytetext += "<tr><td>32-bit Signed Integer:</td><td align=right>" + QString::number(qFromLittleEndian<int32_t>(selectionbytes.mid(0, 4))) + "</td><td align=right>" + QString::number(qFromBigEndian<int32_t>(selectionbytes.mid(0, 4))) + "</td></tr>";
    bytetext += "<tr><td>32-bit Unsigned Integer:</td><td align=right>" + QString::number(qFromLittleEndian<uint32_t>(selectionbytes.mid(0, 4))) + "</td><td align=right>" + QString::number(qFromBigEndian<uint32_t>(selectionbytes.mid(0, 4))) + "</td></tr>";
    bytetext += "<tr><td>64-bit Signed Integer:</td><td colspan=2 align=right>" + QString::number(qFromLittleEndian<int64_t>(selectionbytes.mid(0, 8))) + "</td></tr>";
    bytetext += "<tr><td>64-bit Unsigned Integer:</td><td colspan=2 align=right>" + QString::number(qFromLittleEndian<uint64_t>(selectionbytes.mid(0, 8))) + "</td></tr>";
    bytetext += "<tr><td>Windows 64-bit Timestamp:</td><td colspan=2 align=right>" + ConvertWindowsTimeToUnixTimeUTC(qFromLittleEndian<uint64_t>(selectionbytes.mid(0, 8))) + "</td></tr>";
    bytetext += "<tr><td>Unix 32-bit Timestamp:</td><td colspan=2 align=right>" + ConvertUnixTimeToString(qFromLittleEndian<uint32_t>(selectionbytes.mid(0, 4))) + "</td></tr>";
    bytetext += "</table>";
    byteviewer->SetText(bytetext);

    /*
     *
    bytetext += "<tr><td>DOS Date:</td><td align=right>";
    if(bytes.size() == 2)
    {
        bytetext += QString::fromStdString(std::string(tsk_fs_time_to_str(fatfs_dos_2_unix_time(intvalue, intvalue, bytes[0]), buf)).substr(0, 11));
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
    if(headercolumn == treenodemodel->GetColumnIndex("id"))
        idfilterview->DisplayFilter();
    else if(headercolumn == treenodemodel->GetColumnIndex("name"))
        namefilterview->DisplayFilter();
    else if(headercolumn == treenodemodel->GetColumnIndex("path"))
        pathfilterview->DisplayFilter();
    else if(headercolumn == treenodemodel->GetColumnIndex("size"))
        sizefilterview->DisplayFilter();
    else if(headercolumn == treenodemodel->GetColumnIndex("create"))
        createfilterview->DisplayFilter();
    else if(headercolumn == treenodemodel->GetColumnIndex("access"))
        accessfilterview->DisplayFilter();
    else if(headercolumn == treenodemodel->GetColumnIndex("modify"))
        modifyfilterview->DisplayFilter();
    else if(headercolumn == treenodemodel->GetColumnIndex("status"))
        changefilterview->DisplayFilter();
    else if(headercolumn == treenodemodel->GetColumnIndex("hash"))
        hashfilterview->DisplayFilter();
    else if(headercolumn == treenodemodel->GetColumnIndex("cat"))
        filecategoryfilterview->DisplayFilter();
    else if(headercolumn == treenodemodel->GetColumnIndex("sig"))
        filetypefilterview->DisplayFilter();
    else if(headercolumn == treenodemodel->GetColumnIndex("tag"))
        tagfilterview->DisplayFilter();
    else if(headercolumn == treenodemodel->GetColumnIndex("match"))
        hashmatchfilterview->DisplayFilter();
    ResizeColumns();
}

void WombatForensics::NextItem()
{
    /*
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
            else
            {
                ui->dirTreeView->setCurrentIndex(tmplist.at(0));
                break;
            }
        }
    }
    */
}

void WombatForensics::PreviousItem()
{
    /*
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
    */
}

void WombatForensics::ShowItem()
{
    QModelIndex curindex = pathtreeview->currentIndex();
    if(curindex.isValid())
        ShowFile(curindex);
}

void WombatForensics::UpdateFilterCount()
{
    qint64 filtercount = 0;
    QModelIndexList tmplist = treenodemodel->match(treenodemodel->index(0, 0), Qt::ForegroundRole, QVariant(), -1, Qt::MatchFlags(Qt::MatchExactly | Qt::MatchWrap | Qt::MatchRecursive));
    for(int i=0; i < tmplist.count(); i++)
    {
        //if(tmplist.at(i).sibling(tmplist.at(i).row(), 11).data().toString().split("-").count() == 5)
        if(tmplist.at(i).sibling(tmplist.at(i).row(), treenodemodel->GetColumnIndex("id")).data().toString().split("-").count() == 3)
            filtercount++;
    }
    if(filtercount == filesfound)
        filtercount = 0;
    filtercountlabel->setText("Filtered: " + QString::number(filtercount));
}

void WombatForensics::AddSection()
{
    carvedialog = new CarveDialog(this);
    connect(carvedialog, SIGNAL(TagCarved(QString, QString)), this, SLOT(TagSection(QString, QString)));
    carvedialog->show();
}

void WombatForensics::TagSection(QString ctitle, QString ctag)
{
    qint64 coffset = ui->hexview->GetOffset(); // current offset
    qint64 clength = ui->hexview->GetSelectionLength(); // current selection length (carved size)
    QString enumber = selectedindex.sibling(selectedindex.row(), treenodemodel->GetColumnIndex("id")).data().toString().split("-").first(); // current evidence item
    QString offstr = "0x" + QString::number(coffset, 16); // offset string 0xabc
    QByteArray tmparray = ui->hexview->selectionToByteArray(); // current carved content
    //QMimeDatabase mimedb;
    //const QMimeType mimetype = mimedb.mimeTypeForData(tmparray);
    //QString mimestr = GenerateCategorySignature(mimetype, ""); // category/signature
    QString mimestr = GenerateCategorySignature(tmparray, ""); // category/signature
    QString layoutstr = QString::number(coffset) + "," + QString::number(clength) + ";"; // Layout property
    QString curhash = "";
    if(hashsum == 11)
    {
    }
    else
    {
        QCryptographicHash tmphash((QCryptographicHash::Algorithm)hashsum);
        curhash = QString(tmphash.hash(tmparray, (QCryptographicHash::Algorithm)hashsum).toHex()).toUpper(); // HASH for carved content
    }
    // Add carved prop file
    QFile cfile(wombatvariable.tmpmntpath + "carved/" + enumber + "-c" + QString::number(carvedcount) + ".prop");
    if(!cfile.isOpen())
        cfile.open(QIODevice::WriteOnly | QIODevice::Text);
    if(cfile.isOpen())
    {
        cfile.write(layoutstr.toStdString().c_str());
        cfile.close();
    }

    QHash<QString, QVariant> nodedata;
    nodedata.insert("name", QByteArray(ctitle.toUtf8()).toBase64());
    nodedata.insert("path", QByteArray(offstr.toUtf8()).toBase64());
    nodedata.insert("size", clength);
    nodedata.insert("create", "0");
    nodedata.insert("access", "0");
    nodedata.insert("modify", "0");
    nodedata.insert("status", "0");
    nodedata.insert("hash", curhash);
    nodedata.insert("cat", mimestr.split("/").at(0));
    nodedata.insert("sig", mimestr.split("/").at(1));
    nodedata.insert("tag", ctag);
    nodedata.insert("id", QString(enumber + "-c" + QString::number(carvedcount)));
    /*
    QList<QVariant> nodedata;
    nodedata.clear();
    nodedata << QByteArray(ctitle.toUtf8()).toBase64() << QByteArray(offstr.toUtf8()).toBase64() << clength << "0" << "0" << "0" << "0" << curhash << mimestr.split("/").at(0) << mimestr.split("/").at(1) << ctag << QString(enumber + "-c" + QString::number(carvedcount));
    */
    mutex.lock();
    treenodemodel->AddNode(nodedata, QString(enumber + "-cm"), 15, 0);
    mutex.unlock();
    // ADD TO PREVIEW REPORT
    QString filestr = "<td style='" + ReturnCssString(11) + "' id='" + QString(enumber + "-c" + QString::number(carvedcount)) + "'>";
    filestr += "<table width='300px'><tr><th style='" + ReturnCssString(6) + "' colspan='2'>" + ctitle + "</th></tr>";
    filestr += "<tr style='" + ReturnCssString(12) + "'><td style='" + ReturnCssString(13) + "'>File Path:</td><td style='" + ReturnCssString(14) + "'><span style='word-wrap:break-word;'>" + enumber + "->" + offstr + "</span></td></tr>";
    filestr += "<tr style='" + ReturnCssString(5) + "'><td style='" + ReturnCssString(13) + "'>File Size:</td><td style='" + ReturnCssString(14) + "'>" + QString::number(clength) + " bytes</td></tr>";
    filestr += "<tr style='" + ReturnCssString(4) + "'><td style='" + ReturnCssString(13) + "'>Created:</td><td style='" + ReturnCssString(14) + "'>-</td></tr>";
    filestr += "<tr style='" + ReturnCssString(5) + "'><td style='" + ReturnCssString(13) + "'>Accessed:</td><td style='" + ReturnCssString(14) + "'>-</td></tr>";
    filestr += "<tr style='" + ReturnCssString(4) + "'><td style='" + ReturnCssString(13) + "'>Modified:</td><td style='" + ReturnCssString(14) + "'>-</td></tr>";
    filestr += "<tr style='" + ReturnCssString(5) + "'><td style='" + ReturnCssString(13) + "'>Changed:</td><td style='" + ReturnCssString(14) + "'>-</td></tr>";
    if(!curhash.isEmpty())
    {
        filestr += "<tr style='" + ReturnCssString(4) + "'><td style='" + ReturnCssString(13) + "'>";
        /*
        if(hashsum == 1)
            filestr += "MD5";
        else if(hashsum == 2)
            filestr += "SHA1";
        else if(hashsum == 4)
            filestr += "SHA256";
        */
        filestr += "BLAKE3 Hash:</td><td style='" + ReturnCssString(14) + "'>" + curhash + "</td></tr>";
    }
    filestr += "<tr style='" + ReturnCssString(5) + "'><td style='" + ReturnCssString(13) + "'>Category:</td><td style='" + ReturnCssString(14) + "'>" + mimestr.split("/").first() + "</td></tr>";
    filestr += "<tr style='" + ReturnCssString(4) + "'><td style='" + ReturnCssString(13) + "'>Signature:</td><td style='" + ReturnCssString(14) + "'>" + mimestr.split("/").last() + "</td></tr>";
    filestr += "<tr style='" + ReturnCssString(5) + "'><td style='" + ReturnCssString(13) + "'>ID:</td><td style='" + ReturnCssString(14) + "'>" + QString(enumber + "-c" + QString::number(carvedcount)) + "</td></tr>";
    filestr += "<tr style='" + ReturnCssString(4) + "'><td style='" + ReturnCssString(13) + "'>&nbsp;</td><td style='lvalue'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Link</td></tr>";
    filestr += "</table></td>";
    AddFileItem(ctag, filestr);
    emit treenodemodel->layoutChanged(); // this resolves the issues with the add evidence not updating when you add it later
    carvedcount++;
}

/*
void WombatForensics::AddTextSection()
{
    //qDebug() << "Add selected text to a new section";
}
*/

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

void WombatForensics::PublishResults()
{
    StatusUpdate("Publishing Analysis Results...");
    qInfo() << "Publishing Analysis Results...";
    QDir pdir;
    currentreportpath = reportpath + "/" + QDateTime::currentDateTimeUtc().toString("yyyy-MM-dd-HH-mm-ss") + "/";
    pdir.mkpath(currentreportpath);
    pdir.mkpath(currentreportpath + "thumbs/");
    pdir.mkpath(currentreportpath + "files/");
    QFile::copy(wombatvariable.tmpmntpath + "previewreport.html", currentreportpath + "index.html");
    QStringList curidlist = GetFileLists(3);
    if(!imageshash.isEmpty())
    {
        QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
        foreach(QString id, curidlist)
        {
            TransferThumbnails(id, currentreportpath);
            TransferFiles(id, currentreportpath);
        }
        TransferArtifacts("registry", currentreportpath);
        QApplication::restoreOverrideCursor();
    }
    else
    {
        isreport = true;
        QVector<int> tmplist;
        tmplist.append(5);
        DigFiles(3, tmplist);
    }
    StatusUpdate("Ready");
    qInfo() << "Publishing Analysis Results Finished";
}

void WombatForensics::SaveState()
{
    RemoveTmpFiles();
    UpdateCheckState();
    UpdateSelectedState(selectedindex.sibling(selectedindex.row(), treenodemodel->GetColumnIndex("id")).data().toString());
    //if(selectedindex.sibling(selectedindex.row(), 11).data().toString().split("-").count() == 5)
    if(selectedindex.sibling(selectedindex.row(), treenodemodel->GetColumnIndex("id")).data().toString().split("-").count() == 3)
        RewriteSelectedIdContent(selectedindex);
    //SaveTaggedList();
    //SaveHashList();
    SavePasswordList();
    SaveImagesHash();
    SaveTreeModel();
    QFuture<void> tmpfuture = QtConcurrent::run(GenerateWombatCaseFile);
    savewcfwatcher.setFuture(tmpfuture);
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
        QStringList hashlist = tmpstr.split(",", Qt::SkipEmptyParts);
        for(int i=0; i < hashlist.count(); i++)
            checkhash.insert(hashlist.at(i).split("|", Qt::SkipEmptyParts).at(0), hashlist.at(i).split("|", Qt::SkipEmptyParts).at(1).toInt());
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
            hasharray.append(QString(i.key() + "|" + QString::number(i.value()) + ",").toUtf8());
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
    selectarray.append(id.toUtf8());
    selectfile.write(selectarray);
    selectfile.close();
}

void WombatForensics::AutoSaveState()
{
    // change display text
    StatusUpdate("Saving State Started");
    SaveState(); // send to another thread and then send the finish to spit out the saved log info and update the status..
}

/*
void WombatForensics::RotateCarve()
{
    /*
    QHashIterator<QString, int> i(carvecounthash);
    while (i.hasNext())
    {
	i.next();
	carvestatuslabel->setText("Partition: " + i.key() + " Carved: " + QString::number(i.value()));
    }
    */
    /*
    if(carvetimercounter == 0)
    {
    }
    else if(carvetimercounter == 1)
    {
    }
    */
    /*
     * while(carvetimercounter < carvecounthash.count();
     */ 
/*}
*/

/*
void WombatForensics::RotateDig()
{
    if(digtimercounter == 0 && digimgthumbtotal > 0)
    {
        digcountlabel->setText(digimgcountstring);
	if(dighashtotal > 0)
	    digtimercounter = 1;
	else if(digvidthumbtotal > 0)
	    digtimercounter = 2;
        else if(digarchivetotal > 0)
            digtimercounter = 3;
	else
	    digtimercounter = 50;
    }
    else if(digtimercounter == 1 && dighashtotal > 0)
    {
	digcountlabel->setText(dighashcountstring);
	if(digvidthumbtotal > 0)
	    digtimercounter = 2;
        else if(digarchivetotal > 0)
            digtimercounter = 3;
	else
	    digtimercounter = 50;
    }
    else if(digtimercounter == 2 && digvidthumbtotal > 0)
    {
	digcountlabel->setText(digvidcountstring);
        if(digarchivetotal > 0)
            digtimercounter = 3;
	digtimercounter = 50;
    }
    else if(digtimercounter == 3 && digarchivetotal > 0)
    {
        digcountlabel->setText(digarchivecountstring);
        digtimercounter = 50;
    }
    else
    {
	digcountlabel->setText(digtotalcountstring);
	if(digimgthumbtotal > 0)
	    digtimercounter = 0;
	else if(dighashtotal > 0)
	    digtimercounter = 1;
	else if(digvidthumbtotal > 0)
	    digtimercounter = 2;
	else if(digarchivetotal > 0)
	    digtimercounter = 3;
    }
    //if(digtotalcount == (digvidthumbcount + digimgthumbcount + dighashcount))
	//digrotatetimer->stop();
}
*/

void WombatForensics::SetHexOffset()
{
    ui->hexview->setCursorPosition(jumpoffset*2);
    ui->hexview->ensureVisible();
}

void WombatForensics::ThreadCancelled()
{
    qInfo() << "Current Operation Cancelled";
}

void WombatForensics::LaunchChomp()
{
    // need to install xchomp when i install the app to /usr/local/bin...
    // set the high score file where it needs to be.
    // call process or whatever to launch it...
    QString xchompstr = QDir::homePath();
    xchompstr += "/.local/share/wombatforensics/xchomp";
    QProcess* process = new QProcess(this);
    process->startDetached(xchompstr, QStringList());
}

void WombatForensics::UpdateTimeZone(QString newtimezone)
{
    QTimeZone newtz = QTimeZone(newtimezone.toUtf8());
    QString prevstring = "";
    QString newprevstr = "";
    QFile prevfile(wombatvariable.tmpmntpath + "previewreport.html");
    if(!prevfile.isOpen())
        prevfile.open(QIODevice::ReadOnly | QIODevice::Text);
    if(prevfile.isOpen())
       prevstring = prevfile.readAll(); 
    prevfile.close();
    QString pretzsplit = prevstring.split("Report Time Zone:&nbsp;").first();
    QString tzsplit = prevstring.split("Report Time Zone:&nbsp;").last().split("</h4>").first();
    QTimeZone oldtz = QTimeZone(tzsplit.toUtf8());
    if(oldtz != newtz)
    {
        StatusUpdate("Updating Preview Report to New TimeZone...");
        QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
        newprevstr = pretzsplit + "Report Time Zone:&nbsp;" + newtimezone + "</h4>";
        QString posttzsplit = prevstring.split("Report Time Zone&nbsp;").last().split("</h4>").last();
        // REPLACE CREATED DATE TIME VALUES
        int i = 0;
        while((i = posttzsplit.indexOf("<td style='" + ReturnCssString(13) + "'>Accessed:</td><td style='" + ReturnCssString(14) + "'>", i)) != -1)
        {
            QString curdate = posttzsplit.mid(i+54, 22);
            QDateTime olddatetime;
            olddatetime = QDateTime::fromString(curdate, "MM/dd/yyyy hh:mm:ss AP");
            int oldoffset = oldtz.offsetFromUtc(olddatetime);
            QDateTime utcdatetime = olddatetime.addSecs(-oldoffset); // does opposite of what offset should be (sends local to utc)
            int newoffset = newtz.offsetFromUtc(utcdatetime);
            QDateTime newdatetime = utcdatetime.addSecs(newoffset); // does what it should and sends utc to new tz
            posttzsplit.replace(i+54, 22, newdatetime.toString("MM/dd/yyyy hh:mm:ss AP"));
            i++;
        }
        i = 0;
        while((i = posttzsplit.indexOf("<td style='" + ReturnCssString(13) + "'>Modified:</td><td style='" + ReturnCssString(14) + "'>", i)) != -1)
        {
            QString curdate = posttzsplit.mid(i+54, 22);
            QDateTime olddatetime;
            olddatetime = QDateTime::fromString(curdate, "MM/dd/yyyy hh:mm:ss AP");
            int oldoffset = oldtz.offsetFromUtc(olddatetime);
            QDateTime utcdatetime = olddatetime.addSecs(-oldoffset); // does opposite of what offset should be (sends local to utc)
            int newoffset = newtz.offsetFromUtc(utcdatetime);
            QDateTime newdatetime = utcdatetime.addSecs(newoffset); // does what it should and sends utc to new tz
            posttzsplit.replace(i+54, 22, newdatetime.toString("MM/dd/yyyy hh:mm:ss AP"));
            i++;
        }
        i = 0;
        while((i = posttzsplit.indexOf("<td style='" + ReturnCssString(13) + "'>Changed:</td><td style='" + ReturnCssString(14) + "'>", i)) != -1)
        {
            QString curdate = posttzsplit.mid(i+53, 22);
            QDateTime olddatetime;
            olddatetime = QDateTime::fromString(curdate, "MM/dd/yyyy hh:mm:ss AP");
            int oldoffset = oldtz.offsetFromUtc(olddatetime);
            QDateTime utcdatetime = olddatetime.addSecs(-oldoffset); // does opposite of what offset should be (sends local to utc)
            int newoffset = newtz.offsetFromUtc(utcdatetime);
            QDateTime newdatetime = utcdatetime.addSecs(newoffset); // does what it should and sends utc to new tz
            posttzsplit.replace(i+53, 22, newdatetime.toString("MM/dd/yyyy hh:mm:ss AP"));
            i++;
        }
        i = 0;
        while((i = posttzsplit.indexOf("<td style='" + ReturnCssString(13) + "'>Created:</td><td style='" + ReturnCssString(14) + "'>", i)) != -1)
        {
            QString curdate = posttzsplit.mid(i+53, 22);
            QDateTime olddatetime;
            olddatetime = QDateTime::fromString(curdate, "MM/dd/yyyy hh:mm:ss AP");
            int oldoffset = oldtz.offsetFromUtc(olddatetime);
            QDateTime utcdatetime = olddatetime.addSecs(-oldoffset); // does opposite of what offset should be (sends local to utc)
            int newoffset = newtz.offsetFromUtc(utcdatetime);
            QDateTime newdatetime = utcdatetime.addSecs(newoffset); // does what it should and sends utc to new tz
            posttzsplit.replace(i+53, 22, newdatetime.toString("MM/dd/yyyy hh:mm:ss AP"));
            i++;
        }
        newprevstr += posttzsplit;
        if(!prevfile.isOpen())
            prevfile.open(QIODevice::WriteOnly | QIODevice::Text);
        if(prevfile.isOpen())
           prevfile.write(newprevstr.toStdString().c_str()); 
        prevfile.close();
        QApplication::restoreOverrideCursor();
        StatusUpdate("Ready");
        isignals->ReloadPreview();
    }
}

void WombatForensics::FinishWombatCaseFile(void)
{
    qInfo() << "Current State Saved.";
    /*
    if(digrotatetimer->isActive())
    	StatusUpdate("Digging Deeper...");
    else
	StatusUpdate("Ready");
    */
}

void WombatForensics::SaveTreeModel(void)
{
    QFile treefile(wombatvariable.tmpmntpath + "treemodel");
    if(!treefile.isOpen())
        treefile.open(QIODevice::WriteOnly | QIODevice::Text);
    if(treefile.isOpen())
    {
        QTextStream stream(&treefile);
        // PRINT HEADER INFO HERE
        QStringList columnheaders = treenodemodel->ReturnColumnOrder();
        for(int i=0; i < columnheaders.count(); i++)
        {
            stream << columnheaders.at(i);
            if(i < columnheaders.count() - 1)
                stream << ",";
        }
        stream << ",itemtype,isdeleted,parentid" << Qt::endl;
	for(int i=0; i < existingforimglist.count(); i++)
	    PrintTree(0, treenodemodel->index(i, 0), stream);
	/*
        for(int i=0; i < existingevidence.count(); i++)
        {
            //qDebug() << "existingevidence:" << existingevidence.at(i);
            PrintTree(0, treenodemodel->index(i, 0), stream);
        }
	*/
        //PrintTree(0, treenodemodel->index(0, 0), stream);
        stream.flush();
        treefile.close();
    }
}

void WombatForensics::PrintTree(int level, const QModelIndex& index, QTextStream& stream)
{
    //selectednode = static_cast<TreeNode*>(selectedindex.internalPointer());
    TreeNode* curnode = static_cast<TreeNode*>(index.internalPointer());
    //TreeNode* curparnode = static_cast<TreeNode*>(index.parent().internalPointer());
    if(index.isValid())
    {
        //stream << QString("(" + QString::number(level) + "," + QString::number(index.row()) + ")") << ","; // parent, child
        //for(int c=0; c < 12; c++)
        for(int c=0; c < curnode->ColumnCount(); c++)
        {
            //stream << curnode->Data(columnorder.at(c)).toString() << ","; // column values for id
	    stream << curnode->Data(treenodemodel->ReturnColumnOrder().at(c)).toString();
            //stream << index.sibling(index.row(), c).data().toString();
	    if(c < curnode->ColumnCount() - 1)
		stream << ",";
        }
	stream << ";";
        stream << QString::number(curnode->itemtype) << ";"; // item type
        stream << QString::number(curnode->IsDeleted()) << ";"; // is deleted
        //stream << curparnode->Data("id").toString();
        stream << index.parent().sibling(index.parent().row(), treenodemodel->GetColumnIndex("id")).data().toString(); // parent str
        stream << Qt::endl;
        //qDebug() << "parent id:" << index.parent().sibling(index.parent().row(), 11).data().toString();
        // WILL PROBABLY NEED TO BASE64 1, 2, AND DO SOMETHING WITH THE DATES, AND PROBABLY SAVE THE ISDELETED STATE, ETC... TO POPULATE PROPERLY...
        // ALSO NEED TO ENSURE THE LAST ITEM, DOESN'T GET A "," BECAUSE I CAN'T SKIP EMPTY, ALTHOUGH I DON'T NEED TO ACESS THE LAST ONE, I.E. 15 EVER EITHER..
        // BUT CURRENTLY THIS IS INITIAL GROUNDWORK... GETTING THE ACTUAL TREENODE FROM THE INDEX, ALLOWS ME TO ACCESS ISDELETED ITEMTYPE AND OTHER ANSCILLARY VALUES
        // THIS WOULD ALLOW ME TO THEN READ IN THE FILE AND LOOP OVER IT WITH AN ADDNODE() TYPE FUNCTION... USING THE 0 (PARNT,CHILD) VALUE AND 11 (E#-P#-F#) VALUE
        //qDebug() << QString("(" + QString::number(level) + "," + QString::number(index.row()) + ")") << index.sibling(index.row(), 0).data().toString();
    }
    // print children
    for(int r=0; r < index.model()->rowCount(index); r++)
    {
        // child() is deprecated, i apparently need to use index.model()->index(row, col, parentindex) somehow
        QModelIndex childindex = index.child(r, 0);
        PrintTree(level+1, childindex, stream);
    }
}
/*
void someFunction()
{
        QFile file(filePath);
            if (file.open(QFile::WriteOnly))
            {
                QTextStream stream(&file);
                this->printTree( 0, QModelIndex(), stream );
                file.close();
            }
}

void printTree( int level, const QModelIndex & index, QTextStream  & stream )
{
         QString indent;
         for( int j = 0; j < level; ++j )
             indent.append( "   " );

        // print index itself
        if( index.isValid() )
        {
             stream << indent;
             for( int c = 0; c < index.model()->columnCount(index.parent()); ++c )
             {
                     QModelIndex columnIndex = index.sibling(index.row(), c);
                     stream << index.data().toString();
             }
             stream << "\\n";
       }
       //print children
       for (int r = 0; r < index.model()->rowCount(index); r++)
        {
               const QModelIndex childIndex = index.child( r, 0 );
                this->printTree( level+1, childIndex, stream );
        }
}

             for( int c = 0; c < index.model()->columnCount(index.parent()); c++)
             {
                     QModelIndex columnIndex = index.sibling(index.row(), c);
                     stream << columnIndex .data().toString();
             }

void someFunction()
{
        QFile file(filePath);
            if (file.open(QFile::WriteOnly))
            {
                QTextStream stream(&file);
                for (int r = 0; r < model->rowCount(); r++)
                        this->printTree( 0, model->index(r,0), stream );
                file.close();
            }
}

TreeModel *model = new TreeModel(headers, file.readAll());
setupModelData(data.split('\n'), rootItem);
void TreeModel::setupModelData(const QStringList &lines, TreeItem *parent)
{
    QVector<TreeItem*> parents;
    QVector<int> indentations;
    parents << parent;
    indentations << 0;

    int number = 0;

    while (number < lines.count()) {
        int position = 0;
        while (position < lines[number].length()) {
            if (lines[number].at(position) != ' ')
                break;
            ++position;
        }

        const QString lineData = lines[number].mid(position).trimmed();

        if (!lineData.isEmpty()) {
            // Read the column data from the rest of the line.
            const QStringList columnStrings =
                lineData.split(QLatin1Char('\t'), Qt::SkipEmptyParts);
            QVector<QVariant> columnData;
            columnData.reserve(columnStrings.size());
            for (const QString &columnString : columnStrings)
                columnData << columnString;

            if (position > indentations.last()) {
                // The last child of the current parent is now the new parent
                // unless the current parent has no children.

                if (parents.last()->childCount() > 0) {
                    parents << parents.last()->child(parents.last()->childCount()-1);
                    indentations << position;
                }
            } else {
                while (position < indentations.last() && parents.count() > 0) {
                    parents.pop_back();
                    indentations.pop_back();
                }
            }

            // Append a new item to the current parent's list of children.
            TreeItem *parent = parents.last();
            parent->insertChildren(parent->childCount(), 1, rootItem->columnCount());
            for (int column = 0; column < columnData.size(); ++column)
                parent->child(parent->childCount() - 1)->setData(column, columnData[column]);
        }
        ++number;
    }
}


setupModelData(data.split('\n'), rootItem);
void TreeModel::setupModelData(const QStringList &lines, TreeItem *parent)
{
    QVector<TreeItem*> parents;
    QVector<int> indentations;
    parents << parent;
    indentations << 0;

    int number = 0;

    while (number < lines.count()) {
        int position = 0;
        while (position < lines[number].length()) {
            if (lines[number].at(position) != ' ')
                break;
            position++;
        }

        const QString lineData = lines[number].mid(position).trimmed();

        if (!lineData.isEmpty()) {
            // Read the column data from the rest of the line.
            const QStringList columnStrings =
                lineData.split(QLatin1Char('\t'), Qt::SkipEmptyParts);
            QVector<QVariant> columnData;
            columnData.reserve(columnStrings.count());
            for (const QString &columnString : columnStrings)
                columnData << columnString;

            if (position > indentations.last()) {
                // The last child of the current parent is now the new parent
                // unless the current parent has no children.

                if (parents.last()->childCount() > 0) {
                    parents << parents.last()->child(parents.last()->childCount()-1);
                    indentations << position;
                }
            } else {
                while (position < indentations.last() && parents.count() > 0) {
                    parents.pop_back();
                    indentations.pop_back();
                }
            }

            // Append a new item to the current parent's list of children.
            parents.last()->appendChild(new TreeItem(columnData, parents.last()));
        }
        ++number;
    }
}
*/
