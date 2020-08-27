#include "wombatforensics.h"

// Copyright 2013-2020 Pasquale J. Rinaldi, Jr.
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
    imagewindow = new ImageViewer(this);
    msgviewer = new MessageViewer(this);
    byteviewer = new ByteConverter(this);
    previewreport = new HtmlViewer(this);
    aboutbox = new AboutBox(this);
    searchdialog = new SearchDialog(ui->hexview, this);
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
    connect(isignals, SIGNAL(ProgressUpdate(qint64)), this, SLOT(UpdateProgress(qint64)), Qt::QueuedConnection);
    connect(isignals, SIGNAL(DigUpdate(int, int)), this, SLOT(UpdateDig(int, int)), Qt::QueuedConnection);
    connect(isignals, SIGNAL(ExportUpdate(void)), this, SLOT(UpdateExport()), Qt::QueuedConnection);
    connect(isignals, SIGNAL(ReloadPreview()), previewreport, SLOT(Reload()), Qt::QueuedConnection);
    connect(isignals, SIGNAL(CarveUpdate(QString, int)), this, SLOT(UpdateCarve(QString, int)), Qt::QueuedConnection);
    InitializeAppStructure();
    bookmarkmenu = new QMenu();
    bookmarkmenu->setTitle("Tag Selected As");
    bookmarkmenu->setIcon(QIcon(":/bar/addtotag"));
    tagcheckedmenu = new QMenu();
    tagcheckedmenu->setTitle("Tag Checked As");
    tagcheckedmenu->setIcon(QIcon(":/bar/addtotag"));
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    ui->analysisToolBar->addWidget(spacer);
    ui->analysisToolBar->addAction(ui->actionchomp);
    ui->analysisToolBar->addAction(ui->actionAbout);

    connect(&sqlwatcher, SIGNAL(finished()), this, SLOT(UpdateStatus()), Qt::QueuedConnection);
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

    treemenu = new QMenu(ui->dirTreeView);
    
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
    connect(tagfilterview, SIGNAL(HeaderChanged()), this, SLOT(FilterApplied()));
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
    existingevidence.clear();
    partitionlist.clear();
    carvecounthash.clear();
    treenodemodel = new TreeNodeModel();
    autosavetimer = new QTimer(this);
    digrotatetimer = new QTimer(this);
    carverotatetimer = new QTimer(this);
    connect(autosavetimer, SIGNAL(timeout()), this, SLOT(AutoSaveState()));
    connect(digrotatetimer, SIGNAL(timeout()), this, SLOT(RotateDig()));
    connect(carverotatetimer, SIGNAL(timeout()), this, SLOT(RotateCarve()));
}

void WombatForensics::UnCheckChecked()
{
    QStringList checkeditems;
    checkeditems.clear();
    checkeditems = GetFileLists(1);
    //qDebug() << "checekeditems:" << checkeditems;
    for(int i=0; i < checkeditems.count(); i++)
    {
        QModelIndexList indexlist = treenodemodel->match(treenodemodel->index(0, 11, QModelIndex()), Qt::DisplayRole, QVariant(checkeditems.at(i)), -1, Qt::MatchFlags(Qt::MatchExactly | Qt::MatchRecursive));
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
    newtagaction->setIcon(QIcon(":/bar/newtag"));
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
        QString paridstr = selectedindex.parent().sibling(selectedindex.parent().row(), 11).data().toString().split("-f").last();
        QDir eviddir = QDir(wombatvariable.tmpmntpath);
        QStringList evidfiles = eviddir.entryList(QStringList("*." + selectedindex.sibling(selectedindex.row(), 11).data().toString().split("-").first()), QDir::NoSymLinks | QDir::Dirs);
        QString evidencename = evidfiles.first();
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
        RemoveFileItem(selectedindex.sibling(selectedindex.row(), 11).data().toString());
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
        treenodemodel->UpdateNode(selectedindex.sibling(selectedindex.row(), 11).data().toString(), 10, "0");
    }
    else if(QString(tagaction->iconText()).contains("Checked")) // single file
    {
        QStringList checkeditems = GetFileLists(1);
        for(int i=0; i < checkeditems.count(); i++)
        {
            QModelIndexList indexlist = treenodemodel->match(treenodemodel->index(0, 11, QModelIndex()), Qt::DisplayRole, QVariant(checkeditems.at(i)), -1, Qt::MatchFlags(Qt::MatchExactly | Qt::MatchRecursive));
            if(indexlist.count() > 0)
            {
                QModelIndex curindex = ((QModelIndex)indexlist.first());
                QModelIndex parindex = ((QModelIndex)indexlist.first()).parent();
                QString paridstr = parindex.sibling(parindex.row(), 11).data().toString().split("-f").last();
                QDir eviddir = QDir(wombatvariable.tmpmntpath);
                QStringList evidfiles = eviddir.entryList(QStringList("*." + curindex.sibling(curindex.row(), 11).data().toString().split("-").first()), QDir::NoSymLinks | QDir::Dirs);
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
                RemoveFileItem(curindex.sibling(curindex.row(), 11).data().toString());
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
                treenodemodel->UpdateNode(curindex.sibling(curindex.row(), 11).data().toString(), 10, "0");
            }
        }
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
                    QModelIndexList indexlist = treenodemodel->match(treenodemodel->index(0, 11, QModelIndex()), Qt::DisplayRole, QVariant(checkeditems.at(i)), -1, Qt::MatchFlags(Qt::MatchExactly | Qt::MatchRecursive));
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
    if(curindex.sibling(curindex.row(), 11).data().toString().split("-").count() == 5 || curindex.sibling(curindex.row(), 11).data().toString().contains("-c"))
    {
        QTimeZone tmpzone = QTimeZone(reporttimezone);
        taggedhash.insert(curindex.sibling(curindex.row(), 11).data().toString(), tagname);
        treenodemodel->UpdateNode(curindex.sibling(curindex.row(), 11).data().toString(), 10, tagname);
        QString filestr = "<td class='fitem' id='" + curindex.sibling(curindex.row(), 11).data().toString() + "'>";
        filestr += "<table width='300px'><tr><th colspan='2'>" + curindex.sibling(curindex.row(), 0).data().toString() + "</th></tr>";
        filestr += "<tr class='odd vtop'><td class='pvalue'>File Path:</td><td class='property'><span style='word-wrap:break-word;'>" + curindex.sibling(curindex.row(), 1).data().toString() + "</span></td></tr>";
        filestr += "<tr class='even'><td class='pvalue'>File Size:</td><td class='property'>" + curindex.sibling(curindex.row(), 2).data().toString() + " bytes</td></tr>";
        if(!curindex.sibling(curindex.row(), 3).data().toString().isEmpty())
            filestr += "<tr class='odd'><td class='pvalue'>Created:</td><td class='property'>" + QDateTime::fromSecsSinceEpoch(QDateTime::fromString(curindex.sibling(curindex.row(), 3).data().toString(), "MM/dd/yyyy hh:mm:ss AP").toSecsSinceEpoch(), tmpzone).toString("MM/dd/yyyy hh:mm:ss AP") + "</td></tr>";
        if(!curindex.sibling(curindex.row(), 4).data().toString().isEmpty())
            filestr += "<tr class='odd'><td class='pvalue'>Accessed:</td><td class='property'>" + QDateTime::fromSecsSinceEpoch(QDateTime::fromString(curindex.sibling(curindex.row(), 4).data().toString(), "MM/dd/yyyy hh:mm:ss AP").toSecsSinceEpoch(), tmpzone).toString("MM/dd/yyyy hh:mm:ss AP") + "</td></tr>";
        if(!curindex.sibling(curindex.row(), 5).data().toString().isEmpty())
            filestr += "<tr class='odd'><td class='pvalue'>Modified:</td><td class='property'>" + QDateTime::fromSecsSinceEpoch(QDateTime::fromString(curindex.sibling(curindex.row(), 5).data().toString(), "MM/dd/yyyy hh:mm:ss AP").toSecsSinceEpoch(), tmpzone).toString("MM/dd/yyyy hh:mm:ss AP") + "</td></tr>";
        if(!curindex.sibling(curindex.row(), 6).data().toString().isEmpty())
            filestr += "<tr class='odd'><td class='pvalue'>Changed:</td><td class='property'>" + QDateTime::fromSecsSinceEpoch(QDateTime::fromString(curindex.sibling(curindex.row(), 6).data().toString(), "MM/dd/yyyy hh:mm:ss AP").toSecsSinceEpoch(), tmpzone).toString("MM/dd/yyyy hh:mm:ss AP") + "</td></tr>";
        if(!curindex.sibling(curindex.row(), 7).data().toString().isEmpty())
        {
            filestr += "<tr class='odd'><td class='pvalue'>";
            if(hashsum == 1)
                filestr += "MD5";
            else if(hashsum == 2)
                filestr += "SHA1";
            else if(hashsum == 4)
                filestr += "SHA256";
            filestr += " Hash:</td><td class='property'>" + curindex.sibling(curindex.row(), 7).data().toString() + "</td></tr>";
        }
        filestr += "<tr class='even'><td class='pvalue'>Category:</td><td class='property'>" + curindex.sibling(curindex.row(), 8).data().toString() + "</td></tr>";
        filestr += "<tr class='odd'><td class='pvalue'>Signature:</td><td class='property'>" + curindex.sibling(curindex.row(), 9).data().toString() + "</td></tr>";
        filestr += "<tr class='even'><td class='pvalue'>ID:</td><td class='property'>" + curindex.sibling(curindex.row(), 11).data().toString() + "</td></tr>";
	// IF HTMLVIEWER WON'T OPEN FILE WITHOUT .HTML EXTENSION, THEN I WILL NEED TO ADD THE SHORTCUT AND OTHER HTML PARSED ARTIFACTS TO THIS IF/ELSE STATEMENT...
        if(curindex.sibling(curindex.row(), 8).data().toString().contains("Image") || curindex.sibling(curindex.row(), 8).data().toString().contains("Video"))
            filestr += "<tr class='odd'><td class='pvalue'>&nbsp;</td><td class='lvalue'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href='javascript:void(0)' onclick='ShowContent(\"./files/" + curindex.sibling(curindex.row(), 11).data().toString() + "\")'><img src='./thumbs/" + curindex.sibling(curindex.row(), 11).data().toString() + ".png'/></a></td></tr>";
        else
            filestr += "<tr class='odd'><td class='pvalue'>&nbsp;</td><td class='lvalue'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href='javascript:void(0)' onclick='ShowContent(\"./files/" + curindex.sibling(curindex.row(), 11).data().toString() + "\")'>Link</a></td></tr>";
        filestr += "</table></td>";
        RemoveFileItem(curindex.sibling(curindex.row(), 11).data().toString());
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
            QModelIndexList indexlist = treenodemodel->match(treenodemodel->index(0, 11, QModelIndex()), Qt::DisplayRole, QVariant(checkeditems.at(i)), -1, Qt::MatchFlags(Qt::MatchExactly | Qt::MatchRecursive));
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

void WombatForensics::ShowExternalViewer()
{
    QString tmpstring = wombatvariable.tmpfilepath + selectedindex.sibling(selectedindex.row(), 11).data().toString() + "-fhex";
    QProcess* process = new QProcess(this);
    QStringList arguments;
    arguments << tmpstring;
    process->startDetached(((QAction*)QObject::sender())->text(), arguments);
}

void WombatForensics::SetSelectedFromImageViewer(QString objectid)
{
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
    QModelIndexList indexlist = treenodemodel->match(treenodemodel->index(0, 11, QModelIndex()), Qt::DisplayRole, QVariant(objectid), -1, Qt::MatchFlags(Qt::MatchExactly | Qt::MatchRecursive));
    if(indexlist.count() > 0)
        ui->dirTreeView->setCurrentIndex(indexlist.at(0));
    QApplication::restoreOverrideCursor();
}

void WombatForensics::ShowFile(const QModelIndex &index)
{
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
    if(index.sibling(index.row(), 8).data().toString().contains("Image"))
    {
        imageviewer = new ImageWindow();
        imageviewer->setWindowIcon(QIcon(":/img"));
        imageviewer->setWindowTitle(selectedindex.sibling(selectedindex.row(), 11).data().toString() + " Image Viewer");
        imageviewer->setAttribute(Qt::WA_DeleteOnClose);
        imageviewer->GetImage(selectedindex.sibling(selectedindex.row(), 11).data().toString());
        imageviewer->show();
    }
    else if(index.sibling(index.row(), 8).data().toString().contains("Video"))
    {
        videowindow = new VideoViewer();
        videowindow->setAttribute(Qt::WA_DeleteOnClose);
        videowindow->setWindowIcon(QIcon(":/vid"));
        videowindow->setWindowTitle(selectedindex.sibling(selectedindex.row(), 11).data().toString() + " Video Viewer");
        videowindow->ShowVideo(index);
    }
    else if(index.sibling(index.row(), 9).data().toString().contains("HTML"))
    {
        htmlviewer = new HtmlViewer();
        htmlviewer->setAttribute(Qt::WA_DeleteOnClose);
        htmlviewer->setWindowIcon(QIcon(":/web"));
        htmlviewer->setWindowTitle(selectedindex.sibling(selectedindex.row(), 11).data().toString() + " HTML Viewer");
        htmlviewer->ShowHtml(index);
    }
    else if(index.sibling(index.row(), 8).data().toString().contains("Text"))
    {
        // toggle the button...
        textviewer = new TextViewer();
        textviewer->setWindowIcon(QIcon(":/textencode"));
        textviewer->setWindowTitle(selectedindex.sibling(selectedindex.row(), 11).data().toString() + " Text Viewer");
        textviewer->setAttribute(Qt::WA_DeleteOnClose);
        // THIS FAILS ON ADS
        textviewer->ShowText(index);
    }
    else if(index.sibling(index.row(), 9).data().toString().contains("Shortcut"))
    {
        htmlviewer = new HtmlViewer();
        htmlviewer->setAttribute(Qt::WA_DeleteOnClose);
        htmlviewer->setWindowTitle(selectedindex.sibling(selectedindex.row(), 11).data().toString() + " LNK Viewer");
	htmlviewer->ShowArtifact(0, index); // LNK
    }
    else if(index.sibling(index.row(), 9).data().toString().contains("Directory"))
    {
        htmlviewer = new HtmlViewer();
        htmlviewer->setAttribute(Qt::WA_DeleteOnClose);
        htmlviewer->setWindowTitle("$I30 Viewer " + selectedindex.sibling(selectedindex.row(), 11).data().toString());
	htmlviewer->ShowArtifact(1, index); // I30
    }
    else if(index.sibling(index.row(), 9).data().toString().contains("Recycler"))
    {
        htmlviewer = new HtmlViewer();
        htmlviewer->setAttribute(Qt::WA_DeleteOnClose);
        htmlviewer->setWindowTitle("INFO2 Viewer " + selectedindex.sibling(selectedindex.row(), 11).data().toString());
	htmlviewer->ShowArtifact(2, index); // INFO2
    }
    else if(index.sibling(index.row(), 9).data().toString().startsWith("Recycle.Bin"))
    {
        htmlviewer = new HtmlViewer();
        htmlviewer->setAttribute(Qt::WA_DeleteOnClose);
        htmlviewer->setWindowTitle("$I Viewer " + selectedindex.sibling(selectedindex.row(), 11).data().toString());
	htmlviewer->ShowArtifact(3, index); // $I
    }
    else if(index.sibling(index.row(), 9).data().toString().contains("Prefetch"))
    {
        htmlviewer = new HtmlViewer();
        htmlviewer->setAttribute(Qt::WA_DeleteOnClose);
        htmlviewer->setWindowTitle("Prefetch Viewer " + selectedindex.sibling(selectedindex.row(), 11).data().toString());
	htmlviewer->ShowArtifact(4, index); // Prefetch
    }
    else if(index.sibling(index.row(), 9).data().toString().startsWith("Zip"))
    {
        htmlviewer = new HtmlViewer();
        htmlviewer->setAttribute(Qt::WA_DeleteOnClose);
        htmlviewer->setWindowTitle("Archive Viewer " + selectedindex.sibling(selectedindex.row(), 11).data().toString());
        htmlviewer->ShowArtifact(5, index); // Archive
    }
    else if(index.sibling(index.row(), 9).data().toString().startsWith("PDF"))
    {
	pdfviewer = new PdfViewer();
	pdfviewer->setAttribute(Qt::WA_DeleteOnClose);
	pdfviewer->setWindowTitle("PDF Viewer " + selectedindex.sibling(selectedindex.row(), 11).data().toString());
	pdfviewer->ShowPdf(index.sibling(index.row(), 11).data().toString());
    }
    else if(index.sibling(index.row(), 9).data().toString().startsWith("Registry"))
    {
        regviewer = new RegistryDialog();
        regviewer->setAttribute(Qt::WA_DeleteOnClose);
        regviewer->setWindowTitle("Registry Viewer " + selectedindex.sibling(selectedindex.row(), 11).data().toString());
        regviewer->LoadRegistryFile(selectedindex.sibling(selectedindex.row(), 11).data().toString(), selectedindex.sibling(selectedindex.row(), 0).data().toString());
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
            ba.append(tmplist.at(i).split(":").at(1));
            casepath = QByteArray::fromBase64(ba);
        }
        else if(tmplist.at(i).split(":").at(0) == "reportpath")
        {
            QByteArray ba;
            ba.append(tmplist.at(i).split(":").at(1));
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
    tmppath += "/wombatforensics/";
    QString homepath = QDir::homePath();
    homepath += "/.local/share/wombatforensics/";
    wombatvariable.tmpfilepath = tmppath + "tmpfiles/";
    wombatvariable.tmpmntpath = homepath + "mntpt/";
    wombatvariable.imgdatapath = tmppath + "datamnt/";
    QDir dir;
    if(dir.mkpath(tmppath) == false)
        DisplayError("1.6", "App tmppath folder failed", "App tmppath folder was not created");
    if(dir.mkpath(wombatvariable.tmpfilepath) == false)
        DisplayError("1.3", "App tmpfile folder failed", "App Tmpfile folder was not created");
    if(dir.mkpath(wombatvariable.tmpmntpath) == false)
        DisplayError("1.2", "App tmpmnt folder failed", "App tmpmnt folder was not created");
    if(dir.mkpath(homepath) == false)
        DisplayError("1.4", "App homepath folder failed", "App homepath folder was not created");
    if(dir.mkpath(wombatvariable.imgdatapath) == false)
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
    if(!FileExists(QString(homepath + "xchomp").toStdString()))
    {
        QFile::copy(":/xchomp", homepath + "xchomp");
        QFile chompfile(homepath + "xchomp");
        chompfile.setPermissions(QFileDevice::ExeOwner);
    }
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
    connect(viewmanage, SIGNAL(HideManagerWindow()), this, SLOT(HideViewerManager()), Qt::DirectConnection);
    connect(settingsdialog, SIGNAL(HideSettingsWindow()), this, SLOT(HideSettingsWindow()), Qt::DirectConnection);
    connect(settingsdialog, SIGNAL(UpdateTZ(QString)), this, SLOT(UpdateTimeZone(QString)), Qt::DirectConnection);
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
    previewfile.setFileName(":/html/initialhtml");
    previewfile.open(QIODevice::ReadOnly);
    if(previewfile.isOpen())
        initialhtml = previewfile.readAll();
    previewfile.close();
    previewfile.setFileName(wombatvariable.tmpmntpath + "previewreport.html");
    previewfile.open(QIODevice::WriteOnly | QIODevice::Text);
    if(previewfile.isOpen())
    {
        QTimeZone itz = QTimeZone(reporttimezone);
        previewfile.write(initialhtml.toStdString().c_str());
        QString initialstr = "";
        initialstr = "<div id='infotitle'><h1>Case Title:&nbsp;<span id='casename'>" + wombatvariable.casename + "</span></h1></div>\n";
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
        OpenCaseMountFinished(0, QProcess::NormalExit);
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
    if(existingevidence.count() > 0)
    {
        QFuture<void> tmpfuture = QtConcurrent::map(existingevidence, PopulateTreeModel);
        openwatcher.setFuture(tmpfuture);
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
    QString hashstr = "MD5 Hash";
    // update Hash header: 32 = md5, 40 = sha1, 64 = sha256
    if(hashsum == 2)
        hashstr = "SHA1 Hash";
    else if(hashsum == 4)
        hashstr = "SHA256 Hash";
    treenodemodel->UpdateHeaderNode(7, hashstr);
    thumbdir.mkpath(wombatvariable.tmpmntpath + "carved/");
    thumbdir.mkpath(wombatvariable.tmpmntpath + "archives/");
    thumbdir.mkpath(wombatvariable.tmpmntpath + "thumbs/"); // won't do anything if it already exists
    QDir tdir = QDir(QString(wombatvariable.tmpmntpath + "thumbs/"));
    if(!tdir.isEmpty())
    {
        QFuture<void> tmpfuture = QtConcurrent::run(LoadImagesHash); // load images hash after case open to speed up thumbnail viewing
        thashwatcher.setFuture(tmpfuture);
    }
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
    PrepareEvidenceImage();
    ui->dirTreeView->setModel(treenodemodel);
    connect(treenodemodel, SIGNAL(CheckedNodesChanged()), this, SLOT(UpdateCheckCount()));
    connect(ui->dirTreeView->selectionModel(), SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)), this, SLOT(SelectionChanged(const QItemSelection &, const QItemSelection &)), Qt::DirectConnection);
    // this currently fails to load content data in the hexviewer. until it does, the char bytes are zero...
    QModelIndexList indexlist = treenodemodel->match(treenodemodel->index(0, 11, QModelIndex()), Qt::DisplayRole, QVariant(InitializeSelectedState()), 1, Qt::MatchFlags(Qt::MatchExactly | Qt::MatchRecursive));
    UpdateCheckCount();
    if(indexlist.count() > 0)
    {
        //qDebug() << indexlist.at(0).sibling(indexlist.at(0).row(), 0).data().toString() << indexlist.count();
        //ui->dirTreeView->setCurrentIndex(treenodemodel->index(0, 0, QModelIndex()));
        //selectedindex = treenodemodel->index(0, 0, QModelIndex());
        //oadHexContents();
        //ui->hexview->ensureVisible();
        ui->dirTreeView->setCurrentIndex(indexlist.at(0));
        //QThread::sleep(60);
        ui->dirTreeView->selectionModel()->select(indexlist.at(0), QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows | QItemSelectionModel::Select);
        //selectedindex = indexlist.first();
        LoadHexContents();
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
        ui->actionCarve->setEnabled(true);
        ui->actionJumpToHex->setEnabled(true);
        ui->actionsearchhex->setEnabled(true);
        //ui->actionExpandAll->setEnabled(true);
        //ui->actionCollapseAll->setEnabled(true);
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
    if(!digrotatetimer->isActive())
        StatusUpdate("Ready");
    else
        StatusUpdate("Digging Deeper...");
}

void WombatForensics::ThashFinish(void)
{
    qDebug() << "LoadImageHashes Finished";
    StatusUpdate("Thumbnail Library Ready");
    ui->actionView_Image_Gallery->setEnabled(true);
    if(!digrotatetimer->isActive())
    	StatusUpdate("Ready");
    else
	StatusUpdate("Digging Deeper...");
}

void WombatForensics::SelectionChanged(const QItemSelection &curitem, const QItemSelection &previtem)
{
    if(previtem.indexes().count() > 0)
        oldselectedindex = previtem.indexes().at(0);
    if(curitem.indexes().count() > 0)
    {
        selectedindex = curitem.indexes().at(0);
        bool boolok;
	QLocale clocale(QLocale(QLocale::English, QLocale::UnitedStates));
        qint64 sizeval = clocale.toLongLong(selectedindex.sibling(selectedindex.row(), 2).data().toString(), &boolok);
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
        ui->actionByteConverter->setEnabled(true);
        ui->actionJumpToHex->setEnabled(true);
        ui->actionsearchhex->setEnabled(true);
        QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
        LoadHexContents();
        GenerateHexFile(selectedindex);
        QApplication::restoreOverrideCursor();
    }
}

void WombatForensics::TreeContextMenu(const QPoint &pt)
{
    QModelIndex index = ui->dirTreeView->indexAt(pt);
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
        treemenu->exec(ui->dirTreeView->mapToGlobal(pt));
    }
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
		    QDir dir;
		    dir.mkpath(tmpstring);
		    mntstr = "ewfmount " + tmpstr.split(",").at(3) + " " + tmpstring;
		}
	    }
	    else if(TSK_IMG_TYPE_ISRAW((TSK_IMG_TYPE_ENUM)imgtype)) // RAW
	    {
		QString imgext = tmpstr.split(",").at(3).split("/").last().split(".").last();
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
	    }
	    if(!mntstr.isEmpty())
	    {
		xmntprocess = new QProcess();
		connect(xmntprocess, SIGNAL(readyReadStandardOutput()), this, SLOT(ReadXMountOut()), Qt::QueuedConnection);
		connect(xmntprocess, SIGNAL(readyReadStandardError()), this, SLOT(ReadXMountErr()), Qt::QueuedConnection);
		xmntprocess->setProgram(mntstr);
		xmntprocess->start();
		//xmntprocess->start(mntstr); // removes WARNING Messages but does not capture them.. NEED TO FIX
		//xmntprocess->start(mntstr, QStringList());
	    }
        }
    }
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
    UpdateEvidenceList();
    PrepareEvidenceImage();
    //qDebug() << "evidrepdatalist count" << evidrepdatalist.count();
    for(int i=0; i < evidrepdatalist.count(); i++)
    {
        AddELinkItem(evidrepdatalist.at(i).evidid, evidrepdatalist.at(i).evidname);
        AddEvidItem(evidrepdatalist.at(i).evidcontent);
    }
    //LogMessage("Building Initial Evidence Tree...");
    ui->dirTreeView->setModel(treenodemodel);
    ReadBookmarks();
    emit treenodemodel->layoutChanged(); // this resolves the issues with the add evidence not updating when you add it later
    connect(treenodemodel, SIGNAL(CheckedNodesChanged()), this, SLOT(UpdateCheckCount()));
    connect(ui->dirTreeView->selectionModel(), SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)), this, SLOT(SelectionChanged(const QItemSelection &, const QItemSelection &)), Qt::DirectConnection);
    UpdateCheckCount();
    QModelIndexList indexlist = treenodemodel->match(treenodemodel->index(0, 11, QModelIndex()), Qt::DisplayRole, QVariant(InitializeSelectedState()), -1, Qt::MatchFlags(Qt::MatchExactly | Qt::MatchRecursive));
    if(indexlist.count() > 0)
    {
        ui->dirTreeView->setCurrentIndex(indexlist.at(0));
    }
    else
        ui->dirTreeView->setCurrentIndex(treenodemodel->index(0, 0, QModelIndex()));
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
    newevidence.clear();
    addevidencedialog = new AddEvidenceDialog(this);
    addevidencedialog->exec();
    QDir eviddir = QDir(wombatvariable.tmpmntpath);
    QStringList evidfiles = eviddir.entryList(QStringList(QString("*.e*")), QDir::NoSymLinks | QDir::Dirs);
    ecount = evidfiles.count();
    for(int i=0; i < newevidence.count(); i++)
    {
        QString evidencepath = wombatvariable.tmpmntpath + newevidence.at(i).split("/").last() + ".e" + QString::number(ecount) + "/";
        QDir dir;
        dir.mkpath(evidencepath);
        ecount++;
    }
    if(newevidence.count() > 0)
    {
        evidrepdatalist.clear();
        QFuture<void> tmpfuture = QtConcurrent::map(newevidence, InitializeEvidenceStructure);
        sqlwatcher.setFuture(tmpfuture);
    }
}

void WombatForensics::UpdateProperties()
{
    QFile propfile;
    QDir eviddir = QDir(wombatvariable.tmpmntpath);
    QStringList evidfiles = eviddir.entryList(QStringList("*." + selectedindex.sibling(selectedindex.row(), 11).data().toString().split("-").at(0)), QDir::NoSymLinks | QDir::Dirs);
    QString evidencename = evidfiles.at(0).split(".e").first();
    propertylist.clear();
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
}

void WombatForensics::LoadHexContents()
{
    // NEED TO GET EVIDENCE NAME FROM STAT FILE
    selectednode = static_cast<TreeNode*>(selectedindex.internalPointer());
    QString nodeid = selectednode->Data(11).toString();
    if(nodeid.split("-f").last().startsWith("z"))
    {
        selectednode = static_cast<TreeNode*>(selectedindex.parent().internalPointer());
        nodeid = selectednode->Data(11).toString();
    }
    QString evidid = nodeid.split("-").first();
    QDir eviddir = QDir(wombatvariable.tmpmntpath);
    QStringList evidfiles = eviddir.entryList(QStringList(QString("*." + evidid)), QDir::NoSymLinks | QDir::Dirs);
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
    if(ui->dirTreeView->model() != NULL)
    {
        UpdateSelectedState(selectedindex.sibling(selectedindex.row(), 11).data().toString());
        qInfo() << "Selected Item Saved";
        UpdateCheckState();
        qInfo() << "Check State Saved";
        SaveTaggedList();
        qInfo() << "Tagged Items Saved";
        SaveHashList();
        qInfo() << "Hashed Items Saved";
        SavePasswordList();
        qInfo() << "Password Items Saved";
        SaveImagesHash();
        qInfo() << "Thumbnailed Videos and Images Saved";
        ui->dirTreeView->clearSelection();
        delete treenodemodel;
        autosavetimer->stop();
    }
    if(ui->hexview->data().size() > 0)
    {
        casedatafile.setFileName(QDir::tempPath() + "/zfile");
        casedatafile.open(QIODevice::WriteOnly | QIODevice::Text);
        casedatafile.write("dummy zerofile");
        casedatafile.close();
        ui->hexview->BypassColor(true);
	//qInfo() << "start of error?";
        ui->hexview->setData(casedatafile);
	//qInfo() << "end of error?";
    }
    qInfo() << "Hexviewer Reset";
    setWindowTitle("WombatForensics");
    filesfound = 0;
    fileschecked = 0;
    carvedcount = 0;
    filtercountlabel->setText("Filtered: 0");
    filecountlabel->setText("Found: " + QString::number(filesfound));
    checkedcountlabel->setText("Checked: " + QString::number(fileschecked));
    // UNMOUNT EVIDENCEIMAGEDATAFILE
    for(int i=0; i < existingevidence.count(); i++)
    {
        QString imgext = existingevidence.at(i).split("/").last().split(".").last().toLower();
        if(imgext.contains("e01")) // ewfmount
        {
            QString xunmntstr = "fusermount -u " + wombatvariable.imgdatapath + existingevidence.at(i).split("/").last() + "/";
            QProcess::execute(xunmntstr, QStringList());
        }
        else if(imgext.contains("aff") || imgext.contains("000")) // affuse
        {
            QString xunmntstr = "fusermount -u " + wombatvariable.imgdatapath;
            QProcess::execute(xunmntstr, QStringList());
        }
        /*
        else // raw, so nothing to unmount
        {
        }
        */
    }
    qInfo() << "Forensic Image unmounted";
    carvecounthash.clear();
    partitionlist.clear();
    existingevidence.clear();
    newevidence.clear();
    // BEGIN TAR METHOD
    QString tmptar = casepath + "/" + wombatvariable.casename + ".wfc";
    QByteArray tmparray = tmptar.toLocal8Bit();
    QByteArray tmparray2 = wombatvariable.tmpmntpath.toLocal8Bit();
    QByteArray tmparray3 = QString("./" + wombatvariable.casename).toLocal8Bit();
    TAR* casehandle;
    tar_open(&casehandle, tmparray.data(), NULL, O_WRONLY | O_CREAT, 0644, TAR_GNU);
    tar_append_tree(casehandle, tmparray2.data(), tmparray3.data());
    tar_close(casehandle);
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
            // 4. Delete from existingevidence.
            existingevidence.removeOne(remevidlist.at(i));
            // 5. Remove TreeNode.
            QModelIndexList indexlist = treenodemodel->match(treenodemodel->index(0, 11, QModelIndex()), Qt::DisplayRole, QVariant("e" + evidfiles.first().split(".e").last()), 1, Qt::MatchFlags(Qt::MatchExactly | Qt::MatchRecursive));
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
        QModelIndexList indexlist = treenodemodel->match(treenodemodel->index(0, 10, QModelIndex()), Qt::DisplayRole, QVariant(tr("*[A-Za-z0-9]*")), -1, Qt::MatchFlags(Qt::MatchRecursive | Qt::MatchWildcard));
        foreach(QModelIndex index, indexlist)
            tmplist.append(QString(index.sibling(index.row(), 11).data().toString()));
        return tmplist;
    }
    else if(filelisttype == 4) // Generate list for Image category
    {
	QModelIndexList indexlist = treenodemodel->match(treenodemodel->index(0, 8, QModelIndex()), Qt::DisplayRole, QVariant(tr("Image")), -1, Qt::MatchFlags(Qt::MatchRecursive | Qt::MatchExactly));
	foreach(QModelIndex index, indexlist)
	    tmplist.append(QString(index.sibling(index.row(), 11).data().toString()));
	return tmplist;
    }
    else if(filelisttype == 5) // Generate list for Video category
    {
	QModelIndexList indexlist = treenodemodel->match(treenodemodel->index(0, 8, QModelIndex()), Qt::DisplayRole, QVariant(tr("Video")), -1, Qt::MatchFlags(Qt::MatchRecursive | Qt::MatchExactly));
	foreach(QModelIndex index, indexlist)
	    tmplist.append(QString(index.sibling(index.row(), 11).data().toString()));
	return tmplist;
    }
    else if(filelisttype == 6) // Generate list for Archive category
    {
        QModelIndexList indexlist = treenodemodel->match(treenodemodel->index(0, 8, QModelIndex()), Qt::DisplayRole, QVariant(("Archive")), -1, Qt::MatchFlags(Qt::MatchRecursive | Qt::MatchExactly));
        //qDebug() << "archives found: indexlist count:" << indexlist.count();
        foreach(QModelIndex index, indexlist)
            tmplist.append(QString(index.sibling(index.row(), 11).data().toString()));
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
        exportlist.append(itemnode->Data(11).toString());
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

void WombatForensics::DigFiles(int dtype, QVector<int> doptions)
{
    digimgthumbtotal = 0;
    digvidthumbtotal = 0;
    dighashtotal = 0;
    digtotalcount = 0;
    digimgthumbcount = 0;
    digvidthumbcount = 0;
    digarchivecount = 0;
    dighashcount = 0;
    digimgcountstring = "";
    digvidcountstring = "";
    dighashcountstring = "";
    digtotalcountstring = "";
    digtype = dtype;
    digoptions = doptions;
    digfilelist.clear();
    qInfo() << "Digging Deeper into Evidence";
    StatusUpdate("Digging Deeper...");
    //LogMessage("Digging Deeper into Evidence");
    for(int i = 0; i < digoptions.count(); i++)
    {
        if(dtype == 0) // selected
        {
            TreeNode* itemnode = static_cast<TreeNode*>(selectedindex.internalPointer());
            digfilelist.append(itemnode->Data(11).toString());
        }
        else
            digfilelist = GetFileLists(dtype);
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
        else if(digoptions.at(i) == 1 || digoptions.at(i) == 2 || digoptions.at(i) == 3) // 1 - MD5 || 2- SHA1 || 3- SHA256
        {
            if(digoptions.at(i) == 2)
                hashsum = 2;
            else if(digoptions.at(i) == 3)
                hashsum = 4;
            else if(digoptions.at(i) == 1)
                hashsum = 1;
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
    }
    digtotalcount = digimgthumbtotal + digvidthumbtotal + dighashtotal + digarchivetotal;
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
    digrotatetimer->start(1500);
}

void WombatForensics::UpdateCarve(QString pid, int carvecnt)
{
    carvecounthash.insert(pid, carvecnt);
    // DO I WANT TO PUSH EVERY CARVED ITEM TO AN INFO ???? NOT RIGHT NOW.
    //qDebug() << "carvecounthash updated:" << carvecounthash;
    //SleepLoop(1500);
    carvestatuslabel->setText("Partition: " + pid + " Carved: " + QString::number(carvecnt));
    // have values for QHash<QString partitionid, int carvecount>carvecounthash;
}

void WombatForensics::UpdateDig(int digid, int digcnt)
{
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
    delete viewmanage;
    delete bookmarkmenu;
    delete tagcheckedmenu;
    delete selectionmenu;
    delete msgviewer;
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
    byteviewer->close();
    previewreport->close();
    aboutbox->close();
    settingsdialog->close();
    RemoveTmpFiles();
    event->accept();
    //msglog->clear();
    msgviewer->close();
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
    propertywindow->setWindowTitle(selectedindex.sibling(selectedindex.row(), 11).data().toString() + " Properties");
    propertywindow->setAttribute(Qt::WA_DeleteOnClose);
    propertywindow->setModal(false);
    UpdateProperties();
}

void WombatForensics::on_actionView_File_triggered(bool checked)
{
    if(checked){}
    fileviewer = new FileViewer();
    fileviewer->setWindowIcon(QIcon(":/ehex"));
    fileviewer->setWindowTitle(selectedindex.sibling(selectedindex.row(), 11).data().toString() + " Hex");
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
            int ret = QMessageBox::question(this, tr("Generate Thumbnails"), tr("Thumbnails have not been generated. Do you want to generate all IMG & VID thumbnails now?\r\n\r\nNote: This can take a while and will show the Thumbnail Gallery window when complete."), QMessageBox::Yes | QMessageBox::No);
            if(ret == QMessageBox::Yes)
            {
                QVector<int> tmplist;
                tmplist.append(5);
                DigFiles(2, tmplist);
            }
            else
                ui->actionView_Image_Gallery->setChecked(false);
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
        if(hashsum == 1)
            treenodemodel->UpdateHeaderNode(7, "MD5 Hash");
        else if(hashsum == 2)
            treenodemodel->UpdateHeaderNode(7, "SHA1 Hash");
        else if(hashsum == 4)
            treenodemodel->UpdateHeaderNode(7, "SHA256 Hash");
        QtConcurrent::run(SaveHashList); // save ids/hashed values to hashlist file for re-opening a case.
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
        QApplication::restoreOverrideCursor();
        isreport = false;
    }
    qInfo() << "Digging Finished";
    digrotatetimer->stop();
    digcountlabel->setText(digtotalcountstring);
    emit treenodemodel->layoutChanged(); // this resolves the issues with the add evidence not updating when you add it later
    StatusUpdate("Ready");
}

void WombatForensics::on_actionViewerManager_triggered()
{
    viewmanage->show();
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
    if(headercolumn == 11)
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
    if(headercolumn == 10)
        tagfilterview->DisplayFilter();
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
            else
            {
                ui->dirTreeView->setCurrentIndex(tmplist.at(0));
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
        if(tmplist.at(i).sibling(tmplist.at(i).row(), 11).data().toString().split("-").count() == 5)
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
    // NEED TO GET THE CARVED OFFSET, EVIDITEM, LENGTH/SIZE, ASSIGNED TAG, TITLE/NAME
    // THEN GENERATE WHAT NEEDS TO BE GENERATED... AND ADD IT TO TREEVIEW/ GENERATE .STAT FILE (E#-C#.STAT)/ ADD IT TO PREVIEW REPORT
    // DETERMINE HASH BASED OFF HASH QHASH...
    qint64 coffset = ui->hexview->GetOffset();
    qint64 clength = ui->hexview->GetSelectionLength();
    QString enumber = selectedindex.sibling(selectedindex.row(), 11).data().toString().split("-").first();
    //char buffer[64];
    //#if _LARGEFILE_SOURCE
    //sprintf(buffer,"0x%llx",coffset);
    //#else
    //sprintf(buffer,"0x%x",coffset);
    //#endif
    //QString offstr = buffer;
    QString offstr = "0x" + QString::number(coffset, 16);
    QByteArray tmparray = ui->hexview->selectionToByteArray();
    QMimeDatabase mimedb;
    const QMimeType mimetype = mimedb.mimeTypeForData(tmparray);
    QString mimestr = GenerateCategorySignature(mimetype);
    QByteArray ba;
    ba.clear();
    ba.append(ctitle);
    QByteArray ba2;
    ba2.clear();
    ba2.append(offstr);
    // HASH INFO
    QString curhash = "0";
    QString hashval = selectedindex.sibling(selectedindex.row(), 7).data().toString();
    if(!hashval.isEmpty()) // hash exists, determine what it is...
    {
        QCryptographicHash tmphash((QCryptographicHash::Algorithm)hashsum);
        QByteArray hasharray = QByteArray::fromRawData(tmparray, clength);
        curhash = QString(tmphash.hash(hasharray, (QCryptographicHash::Algorithm)hashsum).toHex()).toUpper();
    }
    QString carvedstring = ba.toBase64() + ",5," + enumber.mid(1) + "," + ba2.toBase64() + ",0,0,0,0," + QString::number(clength) + "," + QString::number(carvedcount) + "," + mimestr + ",0," + enumber + "-c" + QString::number(carvedcount) + "," + curhash + ",0," + ctag + "," + QString::number(coffset);
    // Add CARVED STAT FILE
    QFile cfile(wombatvariable.tmpmntpath + "carved/" + enumber + "-c" + QString::number(carvedcount) + ".stat");
    if(!cfile.isOpen())
        cfile.open(QIODevice::WriteOnly | QIODevice::Text);
    if(cfile.isOpen())
        cfile.write(carvedstring.toStdString().c_str());
    cfile.close();
    // ADD CARVED STAT FILE TO THE TREE
    QList<QVariant> nodedata;
    nodedata.clear();
    nodedata << ba.toBase64();
    nodedata << ba2.toBase64();
    nodedata << QString::number(clength);
    nodedata << "0";
    nodedata << "0";
    nodedata << "0";
    nodedata << "0";
    nodedata << curhash;
    nodedata << mimestr.split("/").first();
    nodedata << mimestr.split("/").last();
    nodedata << ctag;
    nodedata << QString(enumber + "-c" + QString::number(carvedcount));
    treenodemodel->AddNode(nodedata, QString(enumber + "-mc"), 15, 0);
    // ADD TO PREVIEW REPORT
    QString filestr = "<td class='fitem' id='" + QString(enumber + "-c" + QString::number(carvedcount)) + "'>";
    filestr += "<table width='300px'><tr><th colspan='2'>" + ctitle + "</th></tr>";
    filestr += "<tr class='odd vtop'><td class='pvalue'>File Path:</td><td class='property'><span style='word-wrap:break-word;'>" + enumber + "->" + offstr + "</span></td></tr>";
    filestr += "<tr class='even'><td class='pvalue'>File Size:</td><td class='property'>" + QString::number(clength) + " bytes</td></tr>";
    filestr += "<tr class='odd'><td class='pvalue'>Created:</td><td class='property'>-</td></tr>";
    filestr += "<tr class='even'><td class='pvalue'>Accessed:</td><td class='property'>-</td></tr>";
    filestr += "<tr class='odd'><td class='pvalue'>Modified:</td><td class='property'>-</td></tr>";
    filestr += "<tr class='even'><td class='pvalue'>Changed:</td><td class='property'>-</td></tr>";
    if(!curhash.isEmpty())
    {
        filestr += "<tr class='odd'><td class='pvalue'>";
        if(hashsum == 1)
            filestr += "MD5";
        else if(hashsum == 2)
            filestr += "SHA1";
        else if(hashsum == 4)
            filestr += "SHA256";
        filestr += " Hash:</td><td class='property'>" + curhash + "</td></tr>";
    }
    filestr += "<tr class='even'><td class='pvalue'>Category:</td><td class='property'>" + mimestr.split("/").first() + "</td></tr>";
    filestr += "<tr class='odd'><td class='pvalue'>Signature:</td><td class='property'>" + mimestr.split("/").last() + "</td></tr>";
    filestr += "<tr class='even'><td class='pvalue'>ID:</td><td class='property'>" + QString(enumber + "-c" + QString::number(carvedcount)) + "</td></tr>";
    filestr += "<tr class='odd'><td class='pvalue'>&nbsp;</td><td class='lvalue'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Link</td></tr>";
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
    UpdateSelectedState(selectedindex.sibling(selectedindex.row(), 11).data().toString());
    if(selectedindex.sibling(selectedindex.row(), 11).data().toString().split("-").count() == 5)
        RewriteSelectedIdContent(selectedindex);
    SaveTaggedList();
    SaveHashList();
    SavePasswordList();
    SaveImagesHash();
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
    SaveState(); // send to another thread and then send the finish to spit out the saved log info and update the status..
}

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
}

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
        while((i = posttzsplit.indexOf("<td class='pvalue'>Accessed:</td><td class='property'>", i)) != -1)
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
        while((i = posttzsplit.indexOf("<td class='pvalue'>Modified:</td><td class='property'>", i)) != -1)
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
        while((i = posttzsplit.indexOf("<td class='pvalue'>Changed:</td><td class='property'>", i)) != -1)
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
        while((i = posttzsplit.indexOf("<td class='pvalue'>Created:</td><td class='property'>", i)) != -1)
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
    if(digrotatetimer->isActive())
    	StatusUpdate("Digging Deeper...");
    else
	StatusUpdate("Ready");
}
