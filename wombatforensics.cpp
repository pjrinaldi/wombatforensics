#include "wombatforensics.h"

// Copyright 2013-2019 Pasquale J. Rinaldi, Jr.
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
    //statuslabel->setAlignment(Qt::AlignRight);
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
    tagfilterview = new TagFilter(this);
    imagewindow = new ImageViewer(this);
    msgviewer = new MessageViewer(this);
    byteviewer = new ByteConverter(this);
    previewreport = new HtmlViewer(this);
    aboutbox = new AboutBox(this);
    digstatusdialog = new DigStatus(this);
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
    connect(statuslabel, SIGNAL(clicked()), this, SLOT(ShowDigStatus()), Qt::DirectConnection);
    connect(isignals, SIGNAL(DigUpdate(int, int)), this, SLOT(UpdateDig(int, int)), Qt::QueuedConnection);
    connect(isignals, SIGNAL(ExportUpdate(void)), this, SLOT(UpdateExport()), Qt::QueuedConnection);
    connect(digstatusdialog, SIGNAL(CancelImgThumbThread()), &thumbwatcher, SLOT(cancel()), Qt::QueuedConnection);
    connect(digstatusdialog, SIGNAL(CancelHashThread()), &hashingwatcher, SLOT(cancel()), Qt::QueuedConnection);
    connect(digstatusdialog, SIGNAL(CancelVidThumbThread()), &videowatcher, SLOT(cancel()), Qt::QueuedConnection);
    connect(isignals, SIGNAL(ReloadPreview()), previewreport, SLOT(Reload()), Qt::QueuedConnection);
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
    connect(&thumbwatcher, SIGNAL(finished()), this, SLOT(FinishThumbs()), Qt::QueuedConnection);
    connect(&videowatcher, SIGNAL(finished()), this, SLOT(FinishVideos()), Qt::QueuedConnection);
    connect(&thashwatcher, SIGNAL(finished()), this, SLOT(ThashFinish()), Qt::QueuedConnection);
    connect(&thashsavewatcher, SIGNAL(finished()), this, SLOT(ThashSaveFinish()), Qt::QueuedConnection);
    connect(&hashingwatcher, SIGNAL(finished()), this, SLOT(HashingFinish()), Qt::QueuedConnection);
    connect(&exportwatcher, SIGNAL(finished()), this, SLOT(FinishExport()), Qt::QueuedConnection);
    //connect(&carvewatcher, SIGNAL(finished()), this, SLOT(FinishCarve()), Qt::QueuedConnection);
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
    QStringList itemlist = QString(viewerfile.readLine()).split(",", QString::SkipEmptyParts);
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
    treenodemodel = new TreeNodeModel();
    autosavetimer = new QTimer(this);
    connect(autosavetimer, SIGNAL(timeout()), this, SLOT(AutoSaveState()));
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
        bookitemlist = QString(bookmarkfile.readLine()).split(",", QString::SkipEmptyParts);
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
        //qDebug() << "i:" << i;
        QAction* tmpaction = new QAction(bookitemlist.at(i), bookmarkmenu);
        tmpaction->setIcon(QIcon(":/bar/addtotag"));
        tmpaction->setData(QVariant("t" + QString::number(i)));
        //qDebug() << "tmpaction data;" << tmpaction->data().toString();
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
    // TO CLEAN UP CODE, I SHOULD DETERMINE IF IT's CHECKED OR SELECTED HERE...
    // THEN I CAN CALL THE SINGLE TAGFILE CALL OR THE FOR LOOP FOR TAGFILE'S
    // THIS SHOULD REDUCE THE DUPLICATION OF THE CODE FOR SELECTED OR CHECKED

    //qDebug() << tagaction->iconText();
    //QString parentmenu = qobject_cast<QMenu*>(tagaction->parentWidget())->title();
    //if(parentmenu.contains("Selected")) // single file
    if(QString(tagaction->iconText()).contains("Selected")) // single file
    {
        QString paridstr = selectedindex.parent().sibling(selectedindex.parent().row(), 11).data().toString().split("-f").last();
        QDir eviddir = QDir(wombatvariable.tmpmntpath);
        QStringList evidfiles = eviddir.entryList(QStringList("*." + selectedindex.sibling(selectedindex.row(), 11).data().toString().split("-").first()), QDir::NoSymLinks | QDir::Dirs);
        QString evidencename = evidfiles.first();
        QString estring = selectedindex.sibling(selectedindex.row(), 11).data().toString().split("-", QString::SkipEmptyParts).at(0);
        QString vstring = selectedindex.sibling(selectedindex.row(), 11).data().toString().split("-", QString::SkipEmptyParts).at(1);
        QString pstring = selectedindex.sibling(selectedindex.row(), 11).data().toString().split("-", QString::SkipEmptyParts).at(2);
        QString fstring = selectedindex.sibling(selectedindex.row(), 11).data().toString().split("-", QString::SkipEmptyParts).at(3);
        //qDebug() << "evidencename:" << evidencename;
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
        QString filefilepath = wombatvariable.tmpmntpath + evidencename + "/" + vstring + "/" + pstring + "/";
        if(fstring.split(":").count() > 1)
            filefilepath += fstring.split(":").first() + "-" + fstring.split(":").last();
        else
            filefilepath += fstring.split(":").first();
        filefilepath += ".a" + paridstr + ".stat";
        //qDebug() << "filepath:" << filefilepath;
        filefile.setFileName(filefilepath);
        filefile.open(QIODevice::ReadOnly | QIODevice::Text);
        QString tmpstr = "";
        QStringList tmplist;
        tmplist.clear();
        if(filefile.isOpen())
            tmpstr = filefile.readLine();
        filefile.close();
        //qDebug() << "tmpstr before:" << tmpstr;
        RemoveFileItem(selectedindex.sibling(selectedindex.row(), 11).data().toString());
        //RemoveFileItem(selectedindex.sibling(selectedindex.row(), 10).data().toString(), selectedindex.sibling(selectedindex.row(), 11).data().toString());
        if(tmpstr.split(",").count() > 15)
            tmplist = tmpstr.split(",", QString::SkipEmptyParts);
        tmplist[15] = "0";
        tmpstr = "";
        for(int i = 0; i < tmplist.count(); i++)
        {
            tmpstr += tmplist.at(i);
            if(i < tmplist.count() - 1)
                tmpstr += ",";
        }
        //qDebug() << "tmpstr after:" << tmpstr;
        filefile.open(QIODevice::WriteOnly | QIODevice::Text);
        if(filefile.isOpen())
            filefile.write(tmpstr.toStdString().c_str());
        filefile.close();
        treenodemodel->UpdateNode(selectedindex.sibling(selectedindex.row(), 11).data().toString(), 10, "0");
        //RemovePreviewItem(selectedindex.sibling(selectedindex.row(), 11).data().toString());
    }
    else if(QString(tagaction->iconText()).contains("Checked")) // single file
    {
        QStringList checkeditems = GetFileLists(1);
        //qDebug() << "Remove Tag Checked Items:" << checkeditems;
        for(int i=0; i < checkeditems.count(); i++)
        {
            QModelIndexList indexlist = treenodemodel->match(treenodemodel->index(0, 11, QModelIndex()), Qt::DisplayRole, QVariant(checkeditems.at(i)), -1, Qt::MatchFlags(Qt::MatchExactly | Qt::MatchRecursive));
            //qDebug() << "indexlist:" << indexlist.count();
            if(indexlist.count() > 0)
            {
                QModelIndex curindex = ((QModelIndex)indexlist.first());
                QModelIndex parindex = ((QModelIndex)indexlist.first()).parent();
                QString paridstr = parindex.sibling(parindex.row(), 11).data().toString().split("-f").last();
                QDir eviddir = QDir(wombatvariable.tmpmntpath);
                QStringList evidfiles = eviddir.entryList(QStringList("*." + curindex.sibling(curindex.row(), 11).data().toString().split("-").first()), QDir::NoSymLinks | QDir::Dirs);
                QString evidencename = evidfiles.first();
                QString estring = curindex.sibling(curindex.row(), 11).data().toString().split("-", QString::SkipEmptyParts).at(0);
                QString vstring = curindex.sibling(curindex.row(), 11).data().toString().split("-", QString::SkipEmptyParts).at(1);
                QString pstring = curindex.sibling(curindex.row(), 11).data().toString().split("-", QString::SkipEmptyParts).at(2);
                QString fstring = curindex.sibling(curindex.row(), 11).data().toString().split("-", QString::SkipEmptyParts).at(3);
                //qDebug() << "evidencename:" << evidencename;
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
                QString filefilepath = wombatvariable.tmpmntpath + evidencename + "/" + vstring + "/" + pstring + "/";
                if(fstring.split(":").count() > 1)
                    filefilepath += fstring.split(":").first() + "-" + fstring.split(":").last();
                else
                    filefilepath += fstring.split(":").first();
                filefilepath += ".a" + paridstr + ".stat";
                //qDebug() << "filepath:" << filefilepath;
                QString tmpstr = "";
                QStringList tmplist;
                tmplist.clear();
                filefile.setFileName(filefilepath);
                filefile.open(QIODevice::ReadOnly | QIODevice::Text);
                if(filefile.isOpen())
                    tmpstr = filefile.readLine();
                filefile.close();
                RemoveFileItem(curindex.sibling(curindex.row(), 11).data().toString());
                //RemoveFileItem(curindex.sibling(curindex.row(), 10).data().toString(), curindex.sibling(curindex.row(), 11).data().toString());
                //qDebug() << "tmpstr before:" << tmpstr;
                if(tmpstr.split(",").count() > 15)
                    tmplist = tmpstr.split(",", QString::SkipEmptyParts);
                tmplist[15] = "0";
                tmpstr = "";
                for(int i = 0; i < tmplist.count(); i++)
                {
                    tmpstr += tmplist.at(i);
                    if(i < tmplist.count() - 1)
                        tmpstr += ",";
                }
                //qDebug() << "tmpstr after:" << tmpstr;
                filefile.open(QIODevice::WriteOnly | QIODevice::Text);
                if(filefile.isOpen())
                    filefile.write(tmpstr.toStdString().c_str());
                filefile.close();
                treenodemodel->UpdateNode(curindex.sibling(curindex.row(), 11).data().toString(), 10, "0");
                //RemovePreviewItem(curindex.sibling(curindex.row(), 11).data().toString());
            }
        }
    }
}

void WombatForensics::CreateNewTag()
{
    QAction* tagaction = qobject_cast<QAction*>(sender());
    // NEED TO FIGURE OUT A WAY TO GET THE TAG ID ASSIGNED...
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
                //qDebug() << "Tag File Checked Items:" << checkeditems;
                for(int i=0; i < checkeditems.count(); i++)
                {
                    QModelIndexList indexlist = treenodemodel->match(treenodemodel->index(0, 11, QModelIndex()), Qt::DisplayRole, QVariant(checkeditems.at(i)), -1, Qt::MatchFlags(Qt::MatchExactly | Qt::MatchRecursive));
                    //qDebug() << "indexlist:" << indexlist.count();
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
    if(curindex.sibling(curindex.row(), 11).data().toString().split("-").count() == 5)
    {
        /*
        QModelIndex parindex = curindex.parent();
        QString paridstr = parindex.sibling(parindex.row(), 11).data().toString().split("-f").last();
        QDir eviddir = QDir(wombatvariable.tmpmntpath);
        QStringList evidfiles = eviddir.entryList(QStringList("*." + curindex.sibling(curindex.row(), 11).data().toString().split("-").first()), QDir::NoSymLinks | QDir::Dirs);
        QString evidencename = evidfiles.first();
        QString estring = curindex.sibling(curindex.row(), 11).data().toString().split("-", QString::SkipEmptyParts).at(0);
        QString vstring = curindex.sibling(curindex.row(), 11).data().toString().split("-", QString::SkipEmptyParts).at(1);
        QString pstring = curindex.sibling(curindex.row(), 11).data().toString().split("-", QString::SkipEmptyParts).at(2);
        QString fstring = curindex.sibling(curindex.row(), 11).data().toString().split("-", QString::SkipEmptyParts).at(3);
        //qDebug() << "evidencename:" << evidencename;
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
        QString filefilepath = wombatvariable.tmpmntpath + evidencename + "/" + vstring + "/" + pstring + "/";
        if(fstring.split(":").count() > 1)
            filefilepath += fstring.split(":").first() + "-" + fstring.split(":").last();
        else
            filefilepath += fstring.split(":").first();
        filefilepath += ".a" + paridstr + ".stat";
        //qDebug() << "filepath:" << filefilepath;
        QString tmpstr = "";
        QStringList tmplist;
        tmplist.clear();
        filefile.setFileName(filefilepath);
        filefile.open(QIODevice::ReadOnly | QIODevice::Text);
        if(filefile.isOpen())
            tmpstr = filefile.readLine();
        filefile.close();
        //qDebug() << "tmpstr before:" << tmpstr;
        if(tmpstr.split(",").count() > 15)
            tmplist = tmpstr.split(",", QString::SkipEmptyParts);
        tmplist[15] = tagname;
        tmpstr = "";
        for(int i = 0; i < tmplist.count(); i++)
        {
            tmpstr += tmplist.at(i);
            if(i < tmplist.count() - 1)
                tmpstr += ",";
        }
        */
        QTimeZone tmpzone = QTimeZone(reporttimezone);
        // APPLY ABOVE CODE TO THE BELOW CODE TO PUSH PREVIEW DATA WITH CORRECT TIMEZONE
        /*
        filefile.open(QIODevice::WriteOnly | QIODevice::Text);
        if(filefile.isOpen())
            filefile.write(tmpstr.toStdString().c_str());
        filefile.close();
        */
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
        /*
        if(!curindex.sibling(curindex.row(), 3).data().toString().isEmpty())
            filestr += "<tr class='odd'><td class='pvalue'>Created:</td><td class='property'>" + QDateTime::fromSecsSinceEpoch(tmplist.at(6).toInt(), tmpzone).toString("MM/dd/yyyy hh:mm:ss AP") + "</td></tr>";
        if(!curindex.sibling(curindex.row(), 4).data().toString().isEmpty())
            filestr += "<tr class='even'><td class='pvalue'>Accessed:</td><td class='property'>" + QDateTime::fromSecsSinceEpoch(tmplist.at(4).toInt(), tmpzone).toString("MM/dd/yyyy hh:mm:ss AP") + "</td></tr>";
        if(!curindex.sibling(curindex.row(), 5).data().toString().isEmpty())
            filestr += "<tr class='odd'><td class='pvalue'>Modified:</td><td class='property'>" + QDateTime::fromSecsSinceEpoch(tmplist.at(7).toInt(), tmpzone).toString("MM/dd/yyyy hh:mm:ss AP") + "</td></tr>";
        if(!curindex.sibling(curindex.row(), 6).data().toString().isEmpty())
            filestr += "<tr class='even'><td class='pvalue'>Changed:</td><td class='property'>" + QDateTime::fromSecsSinceEpoch(tmplist.at(5).toInt(), tmpzone).toString("MM/dd/yyyy hh:mm:ss AP") + "</td></tr>";
        */
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
        if(curindex.sibling(curindex.row(), 8).data().toString().contains("Image") || curindex.sibling(curindex.row(), 8).data().toString().contains("Video"))
            filestr += "<tr class='odd'><td class='pvalue'>&nbsp;</td><td class='lvalue'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href='javascript:void(0)' onclick='ShowContent(\"./files/" + curindex.sibling(curindex.row(), 11).data().toString() + "\")'><img src='./thumbs/" + curindex.sibling(curindex.row(), 11).data().toString() + ".jpg'/></a></td></tr>";
        else
            filestr += "<tr class='odd'><td class='pvalue'>&nbsp;</td><td class='lvalue'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href='javascript:void(0)' onclick='ShowContent('\"./files/" + curindex.sibling(curindex.row(), 11).data().toString() + "\")'>Link</a></td></tr>";
        filestr += "</table></td>";
        RemoveFileItem(curindex.sibling(curindex.row(), 11).data().toString());
        //RemoveFileItem(tagname, curindex.sibling(curindex.row(), 11).data().toString());
        AddFileItem(tagname, filestr);
        emit treenodemodel->layoutChanged(); // this resolves the issues with the add evidence not updating when you add it later
    }
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
        //qDebug() << "Tag File Checked Items:" << checkeditems;
        for(int i=0; i < checkeditems.count(); i++)
        {
            QModelIndexList indexlist = treenodemodel->match(treenodemodel->index(0, 11, QModelIndex()), Qt::DisplayRole, QVariant(checkeditems.at(i)), -1, Qt::MatchFlags(Qt::MatchExactly | Qt::MatchRecursive));
            //qDebug() << "indexlist:" << indexlist.count();
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

void WombatForensics::ShowDigStatus()
{
    QPoint p = QCursor::pos();
    p += QPoint(40, -300);
    digstatusdialog->move(p);
    digstatusdialog->show();
}

void WombatForensics::ShowExternalViewer()
{
    QString tmpstring = wombatvariable.tmpfilepath + selectedindex.sibling(selectedindex.row(), 11).data().toString() + "-fhex";
    QProcess* process = new QProcess(this);
    QStringList arguments;
    arguments << tmpstring;
    process->startDetached(((QAction*)QObject::sender())->text(), arguments);
    /*
    char* imgbuf = new char[0];
    ssize_t imglen = 0;
    TSK_IMG_INFO* readimginfo;
    TSK_FS_INFO* readfsinfo;
    TSK_FS_FILE* readfileinfo;
    const TSK_TCHAR** imagepartspath;
    qint64 curobjaddr = selectedindex.sibling(selectedindex.row(), 11).data().toString().split("-f").at(1).split("-a").at(0).split(":").at(0).toLongLong(); 
    QDir eviddir = QDir(wombatvariable.tmpmntpath);
    QStringList evidfiles = eviddir.entryList(QStringList("*." + selectedindex.sibling(selectedindex.row(), 11).data().toString().split("-").at(0)), QDir::NoSymLinks | QDir::Dirs);
    QString evidencename = evidfiles.at(0).split(".e").first();
    QString tmpstr = "";
    QStringList evidlist;
    evidlist.clear();
    std::vector<std::string> pathvector;
    pathvector.clear();
    QString estring = selectedindex.sibling(selectedindex.row(), 11).data().toString().split("-", QString::SkipEmptyParts).at(0);
    QString vstring = selectedindex.sibling(selectedindex.row(), 11).data().toString().split("-", QString::SkipEmptyParts).at(1);
    QString pstring = selectedindex.sibling(selectedindex.row(), 11).data().toString().split("-", QString::SkipEmptyParts).at(2);
    QString fstring = selectedindex.sibling(selectedindex.row(), 11).data().toString().split("-", QString::SkipEmptyParts).at(3);
    if(fstring.contains(":") == true)
        fstring = fstring.split(":").first() + "-" + fstring.split(":").last();
    QFile evidfile(wombatvariable.tmpmntpath + evidencename + "." + estring + "/stat");
    evidfile.open(QIODevice::ReadOnly);
    tmpstr = evidfile.readLine();
    evidlist = tmpstr.split(",");
    int partcount = evidlist.at(3).split("|").size();
    evidfile.close();
    for(int i = 0; i < evidlist.at(3).split("|").size(); i++)
        pathvector.push_back(evidlist.at(3).split("|").at(i).toStdString());
    imagepartspath = (const char**)malloc(pathvector.size()*sizeof(char*));
    for(uint i = 0; i < pathvector.size(); i++)
        imagepartspath[i] = pathvector[i].c_str();
    readimginfo = tsk_img_open(partcount, imagepartspath, TSK_IMG_TYPE_DETECT, 0);
    if(readimginfo == NULL)
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
    readfsinfo = tsk_fs_open_img(readimginfo, partlist.at(4).toLongLong(), TSK_FS_TYPE_DETECT);
    readfileinfo = tsk_fs_file_open_meta(readfsinfo, NULL, curobjaddr);
    QDir filedir = QDir(wombatvariable.tmpmntpath + evidencename + "." + estring + "/" + vstring + "/" + pstring);
    QStringList filefiles = filedir.entryList(QStringList(fstring + ".a*.stat"), QDir::NoSymLinks | QDir::Files);
    QFile filefile(wombatvariable.tmpmntpath + evidencename + "." + estring + "/" + vstring + "/" + pstring + "/" + filefiles.at(0));
    filefile.open(QIODevice::ReadOnly);
    tmpstr = filefile.readLine();
    if(filefile.isOpen())
        filefile.close();
    if(partlist.at(0).toInt() == TSK_FS_TYPE_NTFS_DETECT) // IF NTFS (ADS/FILE/DIR/RES/NONRES)
    {
        if(selectedindex.sibling(selectedindex.row(), 11).data().toString().split("-").at(3).split(":").count() > 1) // IF ADS
        {
            if(readfileinfo->meta != NULL)
            {
                imgbuf = new char[tmpstr.split(",").at(8).toULongLong()];
                imglen = tsk_fs_file_read_type(readfileinfo, TSK_FS_ATTR_TYPE_NTFS_DATA, selectedindex.sibling(selectedindex.row(), 11).data().toString().split("-").at(3).split(":").at(1).toInt(), 0, imgbuf, tmpstr.split(",").at(8).toULongLong(), TSK_FS_FILE_READ_FLAG_SLACK);
                if(imglen == -1)
                    qDebug() << tsk_error_get_errstr();
            }
        }
        else // IF NOT ADS
        {
            if(readfileinfo->meta != NULL)
            {
                imgbuf = new char[readfileinfo->meta->size];
                imglen = tsk_fs_file_read(readfileinfo, 0, imgbuf, readfileinfo->meta->size, TSK_FS_FILE_READ_FLAG_SLACK);
            }
        }
    }
    else // OTHER FILE SYSTEM
    {
        imgbuf = new char[readfileinfo->meta->size];
        imglen = tsk_fs_file_read(readfileinfo, 0, imgbuf, readfileinfo->meta->size, TSK_FS_FILE_READ_FLAG_SLACK);
    }
    if(readfileinfo->meta != NULL)
    {
        //else
        QDir dir;
        dir.mkpath(wombatvariable.tmpfilepath);
        QString tmpstring = wombatvariable.tmpfilepath + selectedindex.sibling(selectedindex.row(), 11).data().toString() + "-tmp";
        QFile tmpfile(tmpstring);
        if(tmpfile.open(QIODevice::WriteOnly))
        {
            QDataStream outbuffer(&tmpfile);
            outbuffer.writeRawData(imgbuf, imglen);
            tmpfile.close();
        }
        QProcess* process = new QProcess(this);
        QStringList arguments;
        arguments << tmpstring;
        process->startDetached(((QAction*)QObject::sender())->text(), arguments);
    }
    else
        qDebug() << "no file meta for file size";
    delete[] imgbuf;
    tsk_fs_file_close(readfileinfo);
    tsk_fs_close(readfsinfo);
    tsk_img_close(readimginfo);
    */
}

void WombatForensics::SetSelectedFromImageViewer(QString objectid)
{
    QModelIndexList indexlist = treenodemodel->match(treenodemodel->index(0, 11, QModelIndex()), Qt::DisplayRole, QVariant(objectid), -1, Qt::MatchFlags(Qt::MatchExactly | Qt::MatchRecursive));
    if(indexlist.count() > 0)
        ui->dirTreeView->setCurrentIndex(indexlist.at(0));
}

void WombatForensics::ShowFile(const QModelIndex &index)
{
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
    //qDebug() << "index8:" << index.sibling(index.row(), 8).data().toString();
    //qDebug() << "index9:" << index.sibling(index.row(), 9).data().toString();
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
    QStringList itemlist = QString(viewerfile.readLine()).split(",", QString::SkipEmptyParts);
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
    // ensure casepath exists
    //QDir::mkpath(casepath);
    //if((new QDir())->mkpath(casepath) == false)
    //    DisplayError("S.1", "App casepath folder failed", "App casepath folder was not created");
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
    //qDebug() << "case name:" << wombatvariable.casename;
    if(!wombatvariable.casename.isEmpty())
    {
        QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
        this->setWindowTitle(QString("Wombat Forensics - ") + wombatvariable.casename);
        //qDebug() << "tmpmntpath before:" << wombatvariable.tmpmntpath;
        wombatvariable.tmpmntpath = wombatvariable.tmpmntpath + wombatvariable.casename + "/";
        QDir dir;
        dir.mkpath(wombatvariable.tmpmntpath);
        dir.mkpath(wombatvariable.tmpmntpath + "carved/");
        //(new QDir())->mkpath(wombatvariable.tmpmntpath);
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
        //qDebug() << "tmparray:" << tmparray.data();
        QByteArray tmparray2 = QString(wombatvariable.tmpmntpath).toLocal8Bit();
        //qDebug() << "tmparray2:" << tmparray2.data();
        TAR* tarhand;
        tar_open(&tarhand, tmparray.data(), NULL, O_RDONLY, 0644, TAR_GNU);
        tar_extract_all(tarhand, tmparray2.data());
        tar_close(tarhand);
        wombatvariable.tmpmntpath = wombatvariable.tmpmntpath + wombatvariable.casename.split("/").last().split(".wfc").first() + "/";
        //qDebug() << "open tmpmntpath:" << wombatvariable.tmpmntpath;
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
    qInfo() << "Bookmarks File Created";
    ReadBookmarks();
    logfile.setFileName(wombatvariable.tmpmntpath + "msglog");
    logfile.open(QIODevice::ReadWrite | QIODevice::Append | QIODevice::Text);
    msglog->clear();
    InitializeCheckState();
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
        QtConcurrent::map(cfiles, PopulateCarvedFiles);
        //QFuture<void> tfuture = QtConcurrent::map(cfiles, PopulateCarvedFiles);
        //carvewatcher.setFuture(tfuture);
        carvedcount = cfiles.count() + 1;
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

void WombatForensics::FinishCarve(void)
{
    StatusUpdate("Carved Files Successfully Added");
    qInfo() << "Carved Files Successfully Added";
    StatusUpdate("Ready");
}

void WombatForensics::ThashSaveFinish(void)
{
    qDebug() << "SaveImageHashes Finished";
    StatusUpdate("Thumbnail Library Saved");
    if(hashingwatcher.isFinished())
        StatusUpdate("Ready");
    else
        StatusUpdate("Digging Deeper...");
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
        ui->actionsearchhex->setEnabled(true);
        StatusUpdate("Loading Hex Contents...");
        QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
        LoadHexContents();
        GenerateHexFile(selectedindex);
        QApplication::restoreOverrideCursor();
        StatusUpdate("Ready");
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
                //(new QDir())->mkpath(tmpstring);
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
        }
        if(!mntstr.isEmpty())
        {
            xmntprocess = new QProcess();
            connect(xmntprocess, SIGNAL(readyReadStandardOutput()), this, SLOT(ReadXMountOut()), Qt::QueuedConnection);
            connect(xmntprocess, SIGNAL(readyReadStandardError()), this, SLOT(ReadXMountErr()), Qt::QueuedConnection);
            xmntprocess->start(mntstr); // removes WARNING Messages but does not capture them..
        }
        }
    }
    //qDebug() << "evidnecename:" << QString::fromStdString(wombatvariable.fullpathvector.at(0));
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
        //AddEvidItem(evidrepdatalist.at(i).evidid, evidrepdatalist.at(i).evidcontent);
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
        //ui->dirTreeView->setCurrentIndex(treenodemodel->index(0, 0, QModelIndex()));
        ui->dirTreeView->setCurrentIndex(indexlist.at(0));
        //selectedindex = indexlist.at(0);
    }
    else
        ui->dirTreeView->setCurrentIndex(treenodemodel->index(0, 0, QModelIndex()));
    ui->actionRemove_Evidence->setEnabled(true);
    ui->actionSaveState->setEnabled(true);
    ui->actionDigDeeper->setEnabled(true);
    ui->actionsearchhex->setEnabled(true);
    qInfo() << "Processing Complete";
    //LogMessage("Processing Complete.");
    StatusUpdate("Evidence ready");
    QApplication::restoreOverrideCursor();
    QtConcurrent::run(GenerateWombatCaseFile);
    //QFuture<void> tmpfuture = QtConcurrent::run(GenerateWombatCaseFile);
    //savewcfwatcher.setFuture(tmpfuture);
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
    //qDebug() << "evidfiles:" << evidfiles;
    QString evidencename = evidfiles.at(0).split(".e").first();
    //qDebug() << "evidencename:" << evidencename;
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
        QString tmpfvalue = "";
        QString parentstr = "5"; // NTFS ROOT INUM
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
        if(selectedindex.sibling(selectedindex.row(), 11).data().toString().split("-").at(3).contains(":"))
            tmpfvalue = selectedindex.sibling(selectedindex.row(), 11).data().toString().split("-").at(3).split(":").at(0) + QString("-") + selectedindex.sibling(selectedindex.row(), 11).data().toString().split("-").at(3).split(":").at(1);
        else
            tmpfvalue = selectedindex.sibling(selectedindex.row(), 11).data().toString().split("-").at(3);
        propfile.setFileName(wombatvariable.tmpmntpath + evidencename + "." + selectedindex.sibling(selectedindex.row(), 11).data().toString().split("-").at(0) + "/" + selectedindex.sibling(selectedindex.row(), 11).data().toString().split("-").at(1) + "/" + selectedindex.sibling(selectedindex.row(), 11).data().toString().split("-").at(2) + "/" + tmpfvalue + ".a" + parentstr + ".prop");
    }
    propfile.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream in(&propfile);
    while(!in.atEnd())
    {
        QString tmpstr = "";
        QString line = in.readLine();
        //qDebug() << "line:" << line;
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
        if(curindex.sibling(curindex.row(), 2).data().toLongLong() > 0)
            RewriteSelectedIdContent(curindex);
    }
}

void WombatForensics::LoadHexContents()
{
    // NEED TO GET EVIDENCE NAME FROM STAT FILE
    selectednode = static_cast<TreeNode*>(selectedindex.internalPointer());
    //qDebug() << "selectednode id:" << selectednode->Data(11).toString();
    //qDebug() << "selectednode evid id:" << selectednode->Data(11).toString().split("-").first();
    QString nodeid = selectednode->Data(11).toString();
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
    else if(nodeid.split("-").count() == 2 && nodeid.contains("-c")) // carved file
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
    else if(nodeid.split("-").count() == 5) // dir/file
    {
        QString paridstr = selectedindex.parent().sibling(selectedindex.parent().row(), 11).data().toString().split("-f").last();
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
        /*
        QFile filefile;
        if(nodeid.split("-").at(3).split(":").count() > 1)
            filefile.setFileName(wombatvariable.tmpmntpath + evidfiles.first() + "/" + nodeid.split("-").at(1) + "/" + nodeid.split("-").at(2) + "/" + nodeid.split("-").at(3).split(":").first() + "-" + nodeid.split("-").at(3).split(":").last() + ".a" + paridstr + ".stat");
        else
            filefile.setFileName(wombatvariable.tmpmntpath + evidfiles.first() + "/" + nodeid.split("-").at(1) + "/" + nodeid.split("-").at(2) + "/" + nodeid.split("-").at(3) + ".a" + paridstr + ".stat");
        filefile.open(QIODevice::ReadOnly | QIODevice::Text);
        if(filefile.isOpen())
            tmpstr = filefile.readLine();
        filefile.close();
        */
        int mftaddress = 0;
        if(nodeid.split("-").at(3).split(":").count() > 1) // ads attribute
            mftaddress = nodeid.split("-a").last().toInt();
        else
            mftaddress = nodeid.split("-f").last().split("-").first().toInt();
        /*
        if(nodeid.split("-").at(3).split(":").count() > 1) // ads attribute
            mftaddress = tmpstr.split(",").at(2).toInt();
        else
            mftaddress = tmpstr.split(",").at(9).toInt();
        */
        ui->hexview->setEnabled(true);
        ui->actionsearchhex->setEnabled(true);
        //if(tmpstr.split(",").at(8).toInt() == 0) // zero file
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
                        ui->hexview->SetColorInformation(partlist.at(4).toLongLong(), partlist.at(6).toLongLong(), blockstring, residentstring, bytestring, selectednode->Data(2).toLongLong(), 0);
                        ui->hexview->setCursorPosition(bytestring.toLongLong()*2);
                    }
                    else // IF RESIDENT
                    {
                        if(selectednode->Data(2).toLongLong() < 700) // takes care of $BadClus which is non-resident but doesn't have blocks
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
    
                        ui->hexview->SetColorInformation(partlist.at(4).toLongLong(), partlist.at(6).toLongLong(), blockstring, QString::number(residentoffset + curoffset + resoffset - fsoffset), bytestring, selectednode->Data(2).toLongLong(), (curoffset + resoffset));
                        ui->hexview->setCursorPosition((residentoffset + curoffset + resoffset)*2);
                        }
                        else
                            ui->hexview->setCursorPosition(0);
                    }
                }
                else // IF NOT ADS
                {
                    //if(tmpstr.split(",").at(1).toInt() == 3) // IF DIR
                    if(selectednode->itemtype == 2 || selectednode->itemtype == 11)
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
                            ui->hexview->SetColorInformation(partlist.at(4).toLongLong(), partlist.at(6).toLongLong(), blockstring, QString::number(residentoffset + curoffset + resoffset - fsoffset), bytestring, selectednode->Data(2).toLongLong(), (curoffset + resoffset));
                            ui->hexview->setCursorPosition((residentoffset + curoffset + resoffset)*2);
                        }
                    }
                }
            }
            else // OTHER FILE SYSTEM
            {
                //qDebug() << "not ntfs";
                ui->hexview->SetColorInformation(partlist.at(4).toLongLong(), partlist.at(6).toLongLong(), blockstring, residentstring, bytestring, selectednode->Data(2).toLongLong(), 0);
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
        UpdateSelectedState(selectedindex.sibling(selectedindex.row(), 11).data().toString());
        UpdateCheckState();
        SaveTaggedList();
        SaveHashList();
        ui->dirTreeView->clearSelection();
        delete treenodemodel;
        //qDebug() << "treenodemodel deleted";
        autosavetimer->stop();
    }
    if(ui->hexview->data().size() > 0)
    {
        casedatafile.setFileName(QDir::tempPath() + "/zfile");
        casedatafile.open(QIODevice::WriteOnly | QIODevice::Text);
        casedatafile.write("dummy zerofile");
        casedatafile.close();
        ui->hexview->BypassColor(true);
        ui->hexview->setData(casedatafile);
    }
    setWindowTitle("WombatForensics");
    filesfound = 0;
    fileschecked = 0;
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
    existingevidence.clear();
    newevidence.clear();
    // BEGIN TAR METHOD
    //qDebug() << "close tmpmntpath:" << wombatvariable.tmpmntpath;
    QString tmptar = casepath + "/" + wombatvariable.casename + ".wfc";
    //qDebug() << "tmptar:" << tmptar;
    //qDebug() << "casename:" << wombatvariable.casename;
    QByteArray tmparray = tmptar.toLocal8Bit();
    QByteArray tmparray2 = wombatvariable.tmpmntpath.toLocal8Bit();
    QByteArray tmparray3 = QString("./" + wombatvariable.casename).toLocal8Bit();
    TAR* casehandle;
    tar_open(&casehandle, tmparray.data(), NULL, O_WRONLY | O_CREAT, 0644, TAR_GNU);
    tar_append_tree(casehandle, tmparray2.data(), tmparray3.data());
    tar_close(casehandle);
    // END TAR METHOD
    StatusUpdate("Saved...");
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
    //qDebug() << "remevidlist:" << remevidlist;
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
                //qDebug() << "tfiles:" << tfiles;
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
            // 3. Delete evid directory.
            QDir edir = QDir(wombatvariable.tmpmntpath + evidfiles.first());
            edir.removeRecursively();
            // 4. Delete from existingevidence.
            existingevidence.removeOne(remevidlist.at(i));
            // 5. Remove TreeNode.
            QModelIndexList indexlist = treenodemodel->match(treenodemodel->index(0, 11, QModelIndex()), Qt::DisplayRole, QVariant("e" + evidfiles.first().split(".e").last()), 1, Qt::MatchFlags(Qt::MatchExactly | Qt::MatchRecursive));
            if(!indexlist.isEmpty())
            {
                //qDebug() << "index found:" << indexlist.first().sibling(indexlist.first().row(), 11).data().toString();
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
        //qDebug() << "current digoption:" << digoptions.at(i);
        if(dtype == 0) // selected
        {
            TreeNode* itemnode = static_cast<TreeNode*>(selectedindex.internalPointer());
            digfilelist.append(itemnode->Data(11).toString());
        }
        else
            digfilelist = GetFileLists(dtype);
        //qDebug() << "digfilelist count:" << digfilelist;
        digstatusdialog->SetInitialDigState(digoptions.at(i), digfilelist.count());
        if(digoptions.at(i) == 0 || digoptions.at(i) == 4 || digoptions.at(i) == 5) // Generate Image Thumbnails || video || both
        {
            genthmbpath = wombatvariable.tmpmntpath;
            if(digoptions.at(i) == 0 || digoptions.at(i) == 5)
            {
                thumbfuture = QtConcurrent::map(digfilelist, GenerateThumbnails); // Process Thumbnails
                thumbwatcher.setFuture(thumbfuture);
            }
            if(digoptions.at(i) == 4 || digoptions.at(i) == 5)
            {
                videofuture = QtConcurrent::map(digfilelist, GenerateVidThumbnails);
                videowatcher.setFuture(videofuture);
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
            //qInfo() << "Generating Hash...";
                    //StatusUpdate("Generating Hash...");
            hashingfuture = QtConcurrent::map(digfilelist, GenerateHash);
            hashingwatcher.setFuture(hashingfuture);
        }
    }
}

void WombatForensics::HashingFinish()
{
    if(thumbwatcher.isFinished())
        StatusUpdate("Ready");
    else
        StatusUpdate("Digging Deeper...");
    digstatusdialog->UpdateDigState(1, -1);
    if(hashsum == 1)
        treenodemodel->UpdateHeaderNode(7, "MD5 Hash");
    else if(hashsum == 2)
        treenodemodel->UpdateHeaderNode(7, "SHA1 Hash");
    else if(hashsum == 4)
        treenodemodel->UpdateHeaderNode(7, "SHA256 Hash");
    //qDebug() << hashsum;
    qDebug() << "Hashing should be Finished";
    QtConcurrent::run(SaveHashList); // save ids/hashed values to hashlist file for re-opening a case.
}

void WombatForensics::UpdateDig(int digid, int digcnt)
{
    digstatusdialog->UpdateDigState(digid, digcnt);
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
    //QStringList tmplist;
    //tmplist.clear();
    //tmplist = tmpdir.entryList(QStringList("*-tmp"
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
    //QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
    SaveState();
    //qInfo() << "Current State Saved.";
    //QApplication::restoreOverrideCursor();
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
                genthmbpath = wombatvariable.tmpmntpath;
                StatusUpdate("Generating Thumbnails...");
                digstatusdialog->SetInitialDigState(0, filesfound);
                thumbfuture = QtConcurrent::map(listeditems, GenerateThumbnails); // Process All thumbnails
                thumbwatcher.setFuture(thumbfuture);
                videofuture = QtConcurrent::map(listeditems, GenerateVidThumbnails); // Process all Vid Thumbnails
                videowatcher.setFuture(videofuture);
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

void WombatForensics::FinishVideos()
{
    digstatusdialog->UpdateDigState(4, -1);
    StatusUpdate("Thumbnail Generation Finished");
    qInfo() << "Video Thumbnail Generation Finished";
    if(genthmbpath.contains("mntpt"))
    {
        QFuture<void> tmpfuture = QtConcurrent::run(SaveImagesHash);
        thashsavewatcher.setFuture(tmpfuture);
        QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
        imagewindow->LoadThumbnails();
        QApplication::restoreOverrideCursor();
        if((digoptions.isEmpty() || digoptions.contains(4) || digoptions.contains(5)) && thumbwatcher.isFinished())
            imagewindow->show();
    }
}

void WombatForensics::FinishThumbs()
{
    digstatusdialog->UpdateDigState(0, -1);
    StatusUpdate("Thumbnail generation finished.");
    qInfo() << "Thumbnail generation finished";
    //LogMessage("Thumbnail generation finished.");
    if(genthmbpath.contains("mntpt"))
    {
        QFuture<void> tmpfuture = QtConcurrent::run(SaveImagesHash);
        thashsavewatcher.setFuture(tmpfuture);
        QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
        imagewindow->LoadThumbnails(); // GUI Intensive
        QApplication::restoreOverrideCursor();
        if((digoptions.isEmpty() || digoptions.contains(0) || digoptions.contains(5)) && videowatcher.isFinished())
            imagewindow->show();
    }
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
        if(tmplist.at(i).sibling(tmplist.at(i).row(), 11).data().toString().split("-").count() == 4)
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
    char buffer[64];
    #if _LARGEFILE_SOURCE
    sprintf(buffer,"0x%llx",coffset);
    #else
    sprintf(buffer,"0x%x",coffset);
    #endif
    QString offstr = buffer;
    QByteArray tmparray = ui->hexview->selectionToByteArray();
    QMimeDatabase mimedb;
    const QMimeType mimetype = mimedb.mimeTypeForData(tmparray);
    QString mimestr = GenerateCategorySignature(mimetype);
    QByteArray ba;
    ba.clear();
    ba.append(ctitle);
    QByteArray ba2;
    ba2.clear();
    ba2.append(QString(enumber + "->" + offstr));
    // HASH INFO
    QString curhash = "0";
    QString hashval = selectedindex.sibling(selectedindex.row(), 7).data().toString();
    if(!hashval.isEmpty()) // hash exists, determine what it is...
    {
        QCryptographicHash tmphash((QCryptographicHash::Algorithm)hashsum);
        QByteArray hasharray = QByteArray::fromRawData(tmparray, clength);
        curhash = QString(tmphash.hash(hasharray, (QCryptographicHash::Algorithm)hashsum).toHex()).toUpper();
    }
    QString carvedstring = ba.toBase64() + ",5," + enumber.mid(1) + "," + ba2.toBase64() + ",0,0,0,0," + QString::number(clength) + "," + QString::number(carvedcount) + "," + mimestr + ",0," + enumber + "-c" + QString::number(carvedcount) + "," + curhash + ",0," +  ctag + "," + QString::number(coffset);
    //qDebug() << "carvedstring:" << carvedstring;
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
    //qDebug() << "nodedata:" << nodedata;
    treenodemodel->AddNode(nodedata, enumber, 15, 0);
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
    QString currentreportpath = reportpath + "/" + QDateTime::currentDateTimeUtc().toString("yyyy-mm-dd-HH-mm-ss") + "/";
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
        genthmbpath = currentreportpath;
        thumbfuture = QtConcurrent::map(curidlist, GenerateThumbnails); // Process Thumbnails
        thumbwatcher.setFuture(thumbfuture);
        videofuture = QtConcurrent::map(curidlist, GenerateVidThumbnails);
        videowatcher.setFuture(videofuture);
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
        //qDebug() << tmpstr;
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
    //QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
    StatusUpdate("Saving State Started");
    SaveState(); // send to another thread and then send the finish to spit out the saved log info and update the status..
    //qInfo() << "Current State Auto Saved.";
    //StatusUpdate("Evidence ready");
    //QApplication::restoreOverrideCursor();
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
    //qDebug() << "oldtz:" << oldtz.id() << "newtz:" << newtz.id();
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
        //qDebug() << "newhtml:" << newprevstr;
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
    StatusUpdate("Evidence ready");
    //QApplication::restoreOverrideCursor();
    // place status here....
}
